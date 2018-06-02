#include "stdafx.h"
#include "libcurlDownloader.h"
#include <io.h>

bool CLibcurlDownloader::g_bInitGlobal = false;
CURLSH* CLibcurlDownloader::g_share_handle = nullptr;

CLibcurlDownloader::CLibcurlDownloader(CPaintManagerUI* pManager)
	: m_hWnd(NULL)
	, m_pManager(nullptr)
	, m_pCurl(nullptr)
	, m_wCurlCode(CURLE_OK)
	, m_wFlag(LibcurlFlag::LIBCURLFLAG_NONE)
	, m_hLocalFile(INVALID_HANDLE_VALUE)
	, m_bReqSizeRunning(false)
	, m_bReqDownRunning(false)
	, m_bAbort(false)
	, m_bPause(false)
	, m_bIMINT(false)
	, m_dlLastTransferSize(0)
	, m_dwLastTransferTime(0)
	, m_dwLastValidTime(0)
{
	ASSERT(pManager);
	m_pManager = pManager;
	m_hWnd = m_pManager->GetPaintWindow();
	memset(&m_tDownloadInfo, 0, sizeof(DownLoadStruct));
}

CLibcurlDownloader::~CLibcurlDownloader()
{
}

bool CLibcurlDownloader::InitGlobal()
{
	if (g_bInitGlobal) return true;

	// 初始化全局CURL
	CURLcode wCurlCode = curl_global_init(CURL_GLOBAL_DEFAULT);

	// 初始化CURL共享句柄
	CLibcurlDownloader::g_share_handle = curl_share_init();
	if (CLibcurlDownloader::g_share_handle)
	{
		// 设置DNS共享
		CURLSHcode wCURLSHcode = curl_share_setopt(CLibcurlDownloader::g_share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	}
	
	g_bInitGlobal = wCurlCode == CURLE_OK;
	return g_bInitGlobal;
}

int CLibcurlDownloader::CleanGlobal()
{
	// 关闭CURL共享句柄
	CURLSHcode wCURLSHcode = curl_share_cleanup(CLibcurlDownloader::g_share_handle);

	// 释放全局CURL资源
	curl_global_cleanup();
	g_bInitGlobal = false;

	return wCURLSHcode;
}

bool CLibcurlDownloader::InitOption()
{
	if (!CLibcurlDownloader::InitGlobal())	return false;

	UINT wCurlCode = 0;
	m_pCurl = curl_easy_init();
	ASSERT(m_pCurl);

	// 执行写入文件流操作
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);			//指定回调参数
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, handleWrite);	//指定回调函数

	// 设置重定向的最大次数(查找次数，防止查找太深)
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);

	// 设置301、302跳转跟随location
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);

	// 禁用掉alarm信号，防止多线程中使用超时崩溃
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

	// 禁掉alarm后会有大量CLOSE_WAIT
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1);

	// 跳过服务器SSL验证，不使用CA证书
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);

	// 验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// 启用时会汇报所有的信息，存放在STDERR或指定的CURLOPT_STDERR中
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);

	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, "SUREN:SUREN");

	// 使用DNS共享
	// 由于每个CURL对象都会连接一次服务器,如果发送1000次HTTP请求都连接到同一服务器,libcurl就会返回大量连接错误和接收错误,为此使用DNS共享是很有必要的
	if (CLibcurlDownloader::g_share_handle)
	{
		UINT  wCURLSHcode = 0;
		wCURLSHcode |= curl_easy_setopt(m_pCurl, CURLOPT_SHARE, CLibcurlDownloader::g_share_handle);
		wCURLSHcode |= curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
	}

	m_wCurlCode = (CURLcode)wCurlCode;

	return m_wCurlCode == CURLE_OK;
}

void CLibcurlDownloader::ReqDownloadSize(LPCTSTR strFileName, LPCTSTR strFilePath, LPCTSTR strUrl, DownLoadDelegate* pDelegate)
{
	ASSERT(pDelegate);

	memset(&m_tDownloadInfo, 0, sizeof(DownLoadStruct));
	m_tDownloadInfo.pDelegate = pDelegate;
	m_tDownloadInfo.strUrl = strUrl;
	m_tDownloadInfo.strFileName = strFileName;
	m_tDownloadInfo.strLocalPath = strFilePath;

	std::thread std_thread = std::thread(&CLibcurlDownloader::_GetDownloadSizeThreadRun, this);
	std_thread.detach();
}

DOUBLE CLibcurlDownloader::GetLocalFileSize(std::string& strFilePath)
{
	// 打开文件
	DWORD dwLocalFileSize = 0;
	HANDLE hLocalFile = ::CreateFile(strFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hLocalFile)
	{
		dwLocalFileSize = ::GetFileSize(hLocalFile, NULL);
		if (INVALID_FILE_SIZE == dwLocalFileSize)
		{
			dwLocalFileSize = 0;
		}
	}
	::CloseHandle(hLocalFile);

	return dwLocalFileSize;
}

void CLibcurlDownloader::CalculateByteDesc(double dlSize, std::string& strDesc)
{
	std::string unit = "B";

	DWORD dw_N = 1024;
	DWORD dw_K = dw_N;
	DWORD dw_M = dw_K * dw_N;
	DWORD dw_G = dw_M * dw_N;

	if (dlSize > dw_G)
	{
		unit = "G";
		dlSize /= dw_G;
	}
	else if (dlSize > dw_M)
	{
		unit = "M";
		dlSize /= dw_M;
	}
	else if (dlSize > dw_K)
	{
		unit = "KB";
		dlSize /= dw_K;
	}

	TCHAR pzBuff[MAX_PATH] = { 0 };
	_stprintf_s(pzBuff, _T("%.2f%s"), dlSize, unit.c_str());
	strDesc = pzBuff;
}

void CLibcurlDownloader::StartDownLoad()
{
	if (IsRunning() == true) return;

	m_thread = std::thread(&CLibcurlDownloader::_DownloadThreadRun, this);
	m_thread.detach();
}

void CLibcurlDownloader::ContinueDownload()
{
	this->_StopDownLoad();
	this->InitOption();

	m_tDownloadInfo.bStarted = TRUE;
	m_tDownloadInfo.dlLastSize = m_tDownloadInfo.dlNowSize;
	m_tDownloadInfo.dlSpeed = 0;

	this->StartDownLoad();
}

void CLibcurlDownloader::ReDownLoad()
{
	this->_StopDownLoad();
	this->DeleteLocalFile();
	this->InitOption();

	m_tDownloadInfo.bStarted = TRUE;
	m_tDownloadInfo.dlLastSize = 0;
	m_tDownloadInfo.dlNowSize = 0;
	m_tDownloadInfo.dlSpeed = 0;

	this->StartDownLoad();
}

bool CLibcurlDownloader::IsRunning() const
{
	return m_bReqDownRunning;
}

void CLibcurlDownloader::DeleteLocalFile()
{
	this->_CloseCurlHander();
	std::string strFilePath = m_tDownloadInfo.GetLocalFilePath();
	if (strFilePath.size() > 0)
	{
		::DeleteFile(strFilePath.c_str());
	}
}

eDownloadStatus CLibcurlDownloader::ExcuteDownload()
{
	// 创建目录
	LPCTSTR lpDir = m_tDownloadInfo.strLocalPath.c_str();
	if (FALSE == ::PathFileExists(lpDir))
	{
		if (FALSE == ::CreateDirectory(lpDir, NULL))
		{
			// 创建目录失败
			return eDownloadStatus::E_DOWNLOAD_STATUS_CREATE_DIR_FAILED;
		}
	}

	// 打开文件
	m_hLocalFile = ::CreateFile(m_tDownloadInfo.GetLocalFilePath().c_str()
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL
		, OPEN_ALWAYS
		, FILE_ATTRIBUTE_NORMAL
		, NULL);

	if (INVALID_HANDLE_VALUE == m_hLocalFile)
	{
		// 打开文件失败
		return eDownloadStatus::E_DOWNLOAD_STATUS_OPENFILE_FAILED;
	}

	// 获取本地已下载文件大小等
	curl_off_t dwLocalFileSize = ::GetFileSize(m_hLocalFile, NULL);
	if (INVALID_FILE_SIZE == dwLocalFileSize)
	{
		// 获取本地文件大小失败
		return eDownloadStatus::E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED;
	}
	else if (dwLocalFileSize > 0)
	{
		m_tDownloadInfo.dlLastSize = dwLocalFileSize;
		m_tDownloadInfo.dlNowSize = dwLocalFileSize;

		if (FALSE == m_tDownloadInfo.bStarted )
		{
			return eDownloadStatus::E_DOWNLOAD_STATUS_CONTINUE_TRANSFER;
		}
	}

	// 将文件指针偏移至文件末尾(用于追加读写)
	DWORD dwResult = ::SetFilePointer(m_hLocalFile, 0, NULL, FILE_END);
	if (HFILE_ERROR == dwResult)
	{
		// 设置本地文件偏移读写失败
		return eDownloadStatus::E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED;
	}
		
	//std::string strVersion = curl_version();											// 获取libcurl当前版本
	curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);									// 设置重定向的最大次数(查找次数，防止查找太深)
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);								// 设置301、302跳转跟随location
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);									// 禁用掉alarm信号，防止多线程中使用超时崩溃
	curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1);									// 禁掉alarm后会有大量CLOSE_WAIT
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);								// 跳过服务器SSL验证，不使用CA证书
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);								// 验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）
	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);										// 启用时会汇报所有的信息，存放在STDERR或指定的CURLOPT_STDERR中
	curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, "SUREN:SUREN");							// 传递一个形如[username]:[password]风格的字符串,作用PHP去连接

	curl_easy_setopt(m_pCurl, CURLOPT_URL, m_tDownloadInfo.strUrl.c_str());				// 设置下载url地址
	curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);										// 获取头	- 关闭
	curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 0L);										// 获取内容 - 开启
	curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, 0L);										// 上传		- 关闭
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 0L);										// Post		- 关闭
	curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM_LARGE, dwLocalFileSize);				// 设置续传起始位置
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0L);									// 进度条	- 开启
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);									// 指定回调参数
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, handleWrite);						// 指定回调函数
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);								// 指定回调参数
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, handleProgress);				// 指定回调函数
	curl_easy_setopt(m_pCurl, CURLOPT_ACCEPT_ENCODING, _T(""));							// 设置接收的编码格式
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 30);								// 设置连接前等待的时间
	//curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT,	30);								// 设置执行的最长秒数
	
	// 设置通信方式为:下载
	m_wFlag = LibcurlFlag::LIBCURLFLAG_DOWNLOAD;

	// 开始执行请求
	eDownloadStatus iStatusCode = eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_OK;
	m_wCurlCode = curl_easy_perform(m_pCurl);
	if (CURLcode::CURLE_OK != m_wCurlCode)
	{
		iStatusCode = eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_FAILED;
		if (this->IsAbortDownload())
		{
			iStatusCode = eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_ABORT;
		}
	}

	return iStatusCode;
}

void CLibcurlDownloader::AbortDownload()
{
	if (!m_bReqSizeRunning && !m_bReqDownRunning)
	{
		m_tDownloadInfo.eCode = eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_ABORT;
		::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
	}
	else if (m_bReqSizeRunning)
	{
		m_tDownloadInfo.eCode = eDownloadStatus::E_DOWNLOAD_STATUS_REQ_SIZE_BLOCK;
		::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
	}
	else if (m_bReqDownRunning)
	{
		m_bAbort = true;
	}
}

void CLibcurlDownloader::PauseDownload()
{
	if (m_pCurl)
	{
		m_wCurlCode = curl_easy_pause(m_pCurl, CURLPAUSE_RECV);
		m_bPause = m_wCurlCode == CURLcode::CURLE_OK;
	}
}

void CLibcurlDownloader::ResumeDownload()
{
	if (m_pCurl)
	{
		m_wCurlCode = curl_easy_pause(m_pCurl, CURLPAUSE_RECV_CONT);
		if (m_wCurlCode == CURLcode::CURLE_OK)
		{
			m_bPause = false;
		}
	}
}

bool CLibcurlDownloader::IsAbortDownload() const
{
	return m_bAbort;
}

bool CLibcurlDownloader::IsPauseDownload() const
{
	return m_bPause;
}

bool CLibcurlDownloader::IsIMINTDownload() const
{
	return m_bIMINT;
}

void CLibcurlDownloader::StartInstall()
{
	std::thread std_thread = std::thread(&CLibcurlDownloader::_InstallThreadRun, this);
	std_thread.detach();
}

size_t CLibcurlDownloader::handleWrite(void *buffer, size_t size, size_t nmemb, void *userp)
{
	// 把下载到的数据以追加的方式写入文件(一定要有a，否则前面写入的内容就会被覆盖了)
	CLibcurlDownloader* pThis = static_cast<CLibcurlDownloader*>(userp);
	ASSERT(pThis);

	DWORD dwWritten = 0;
	switch (pThis->m_wFlag)
	{
		case LibcurlFlag::LIBCURLFLAG_NONE:
			break;

		// 下载
		case LibcurlFlag::LIBCURLFLAG_DOWNLOAD:
		{
			if (pThis->m_hLocalFile == INVALID_HANDLE_VALUE) return 0;
			dwWritten = pThis->_OnWriteFile(buffer, size, nmemb);
		}break;

		// Post
		case LibcurlFlag::LIBCURLFLAG_POST:

		// Get
		case LibcurlFlag::LIBCURLFLAG_GET:
		{
			dwWritten = size * nmemb;
			pThis->m_strRespons.append((const char*)buffer, dwWritten);
		}break;

		default:
			break;
	}

	if (pThis->IsAbortDownload())
	{
		dwWritten = CURL_READFUNC_ABORT;
	}

	return dwWritten;
}

size_t CLibcurlDownloader::handleProgress(void *buffer, double dltotal, double dlnow, double ultotal, double ulnow)
{
	size_t uReturnCode = 0;
	CLibcurlDownloader* pThis = static_cast<CLibcurlDownloader*>(buffer);
	if (!pThis || pThis->IsIMINTDownload() || pThis->IsAbortDownload())
	{
		uReturnCode = CURL_READFUNC_ABORT;
	}
	else if (!pThis->IsPauseDownload())
	{
		uReturnCode = pThis->_OnProgress(dltotal, dlnow);
	}

	return uReturnCode;
}

size_t CLibcurlDownloader::_OnWriteFile(void *buffer, size_t size, size_t nmemb)
{
	DWORD dwWritten = 0;
	DWORD dwDataSize = size * nmemb;
	::WriteFile(m_hLocalFile, buffer, dwDataSize, &dwWritten, NULL);

	return (size_t)dwWritten;
}

size_t CLibcurlDownloader::_OnProgress(const double& dltotal, const double& dlnow)
{
	// curl_get_info必须在curl_easy_perform之后调用
	// curl_easy_getinfo(m_pCurl, CURLINFO_SPEED_DOWNLOAD, &m_pDownloadInfo->dlSpeed);
	m_tDownloadInfo.dlNowSize = m_tDownloadInfo.dlLastSize + dlnow;
	double dlTotalSize = m_tDownloadInfo.dlTotalSize;
	double dlNowSize = m_tDownloadInfo.dlNowSize;

	// 及时更新进度条
	if (m_tDownloadInfo.pDelegate)
	{
		m_tDownloadInfo.pDelegate->OnLibcurlProgress(dlTotalSize, dlNowSize);
	}

	DWORD dwNowTime = ::GetTickCount();
	if (dlnow > 0)
	{
		// 每隔一秒更新一下速度
		if ((dwNowTime - m_dwLastTransferTime > 1000) && m_tDownloadInfo.pDelegate)
		{
			m_tDownloadInfo.dlSpeed = dlnow - m_dlLastTransferSize;
			m_dlLastTransferSize = dlnow;
			m_dwLastTransferTime = dwNowTime;
			m_tDownloadInfo.pDelegate->OnLibcurlSpeed(m_tDownloadInfo.dlSpeed);
		}
	}
	
	if (m_tDownloadInfo.dlSpeed <= 1)
	{
		if (m_dwLastValidTime <= 0)
		{
			m_dwLastValidTime = dwNowTime;
		}

		if (dwNowTime - m_dwLastValidTime > 10000)
		{
			m_dwLastValidTime = 0;
			m_bIMINT = true;
		}
	}

	return 0;
}

size_t CLibcurlDownloader::_OnComplete()
{
	if (m_tDownloadInfo.pDelegate)
	{
		m_tDownloadInfo.pDelegate->OnLibcurlComplete();
	}

	return 0;
}

void CLibcurlDownloader::_GetDownloadSizeThreadRun()
{
	this->m_bReqSizeRunning = true;

	double dlSize = 0;
	std::string strError;
	m_strRespons.clear();

	// 仅获取HTTP头
	m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_URL, m_tDownloadInfo.strUrl.c_str());
	m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 1L);				// 并不是所有HTTP的服务器都是支持HEAD方式来获取(因此用下面的方式获取)
	//m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, "GET");		// 使用CURLOPT_CUSTOMREQUEST获取
	m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 1L);

	m_wFlag = LibcurlFlag::LIBCURLFLAG_GET;
	m_wCurlCode = curl_easy_perform(m_pCurl);
	m_strRespons.clear();
	this->m_bReqSizeRunning = false;

	if (m_wCurlCode == CURLE_OK)
	{
		m_wCurlCode = curl_easy_getinfo(m_pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dlSize);
		if (CURLE_OK != m_wCurlCode)
		{
			strError = curl_easy_strerror(m_wCurlCode);
		}
	}
	else
	{
		strError = curl_easy_strerror(m_wCurlCode);
	}

	// 保存获取的大小,回调通知
	m_tDownloadInfo.dlTotalSize = dlSize;
	if (m_tDownloadInfo.pDelegate)
	{
		m_tDownloadInfo.pDelegate->OnOnLibcurDownloadSize(dlSize);
	}
}

void CLibcurlDownloader::_DownloadThreadRun()
{
	this->m_bReqDownRunning = true;
	this->m_wCurlCode = CURLE_OK;
	this->m_wFlag = LibcurlFlag::LIBCURLFLAG_DOWNLOAD;
	this->m_hLocalFile = INVALID_HANDLE_VALUE;
	this->m_bIMINT = false;
	this->m_bAbort = false;
	this->m_bPause = false;
	this->m_dlLastTransferSize = 0;
	this->m_dwLastTransferTime = 0;
	this->m_dwLastValidTime = 0;

	eDownloadStatus eStatus = this->ExcuteDownload();
	this->_StopDownLoad();

	switch (eStatus)
	{
		// 下载成功
		case E_DOWNLOAD_STATUS_DOWNLOAD_OK:
		{
			this->_OnComplete();
		}break;

		// 终止下载
		case E_DOWNLOAD_STATUS_DOWNLOAD_ABORT:
		{
			m_tDownloadInfo.eCode = eStatus;
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;

		// 继续下载
		case E_DOWNLOAD_STATUS_CONTINUE_TRANSFER:
		{
			m_tDownloadInfo.eCode = eStatus;
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;

		// 文件操作失败
		case E_DOWNLOAD_STATUS_CREATE_DIR_FAILED:
		case E_DOWNLOAD_STATUS_OPENFILE_FAILED:
		case E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED:
		case E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED:
		{
			m_tDownloadInfo.eCode = eStatus;
			m_tDownloadInfo.eCode2 = GetLastError();
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;
		
		// 下载失败(非上述case,则都为失败)
		case E_DOWNLOAD_STATUS_NONE:
		case E_DOWNLOAD_STATUS_EXTRA:
		case E_DOWNLOAD_STATUS_DOWNLOAD_FAILED:
		default:
		{
			m_tDownloadInfo.eCode = eStatus;
			m_tDownloadInfo.eCode2 = m_wCurlCode;
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;
	}
}

void CLibcurlDownloader::_InstallThreadRun()
{
	// 初始化
	ZlibUtil* pZlibUtil = new ZlibUtil();
	std::string pzFile = m_tDownloadInfo.GetLocalFilePath();
	std::string pzOutDir = m_tDownloadInfo.strLocalPath + _T("\\..\\");

	// 格式化路径
	TCHAR pzBuff[MAX_PATH] = { 0 };
	::PathCanonicalize(pzBuff, pzOutDir.c_str());
	pzOutDir = pzBuff;

	::PathCanonicalize(pzBuff, pzFile.c_str());
	pzFile = pzBuff;

	// 开始解压
	int iResCode = pZlibUtil->Decompress(pzFile.c_str(), pzOutDir.c_str());
	delete pZlibUtil;

	// 清除已下载的文件
	this->DeleteLocalFile();
	
	if (m_tDownloadInfo.pDelegate)
	{
		m_tDownloadInfo.pDelegate->OnInstallComplete(iResCode);
	}
}

void CLibcurlDownloader::_StopDownLoad()
{
	m_bReqDownRunning = false;

	this->_CloseCurlHander();
	this->_CloseLocalFile();
}

void CLibcurlDownloader::_CloseCurlHander()
{
	if (m_pCurl)
	{
		curl_easy_cleanup(m_pCurl);
		m_pCurl = NULL;
	}
}

void CLibcurlDownloader::_CloseLocalFile()
{
	if (INVALID_HANDLE_VALUE != m_hLocalFile)
	{
		::CloseHandle(m_hLocalFile);
		m_hLocalFile = INVALID_HANDLE_VALUE;
	}
}
