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

	// ��ʼ��ȫ��CURL
	CURLcode wCurlCode = curl_global_init(CURL_GLOBAL_DEFAULT);

	// ��ʼ��CURL������
	CLibcurlDownloader::g_share_handle = curl_share_init();
	if (CLibcurlDownloader::g_share_handle)
	{
		// ����DNS����
		CURLSHcode wCURLSHcode = curl_share_setopt(CLibcurlDownloader::g_share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	}
	
	g_bInitGlobal = wCurlCode == CURLE_OK;
	return g_bInitGlobal;
}

int CLibcurlDownloader::CleanGlobal()
{
	// �ر�CURL������
	CURLSHcode wCURLSHcode = curl_share_cleanup(CLibcurlDownloader::g_share_handle);

	// �ͷ�ȫ��CURL��Դ
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

	// ִ��д���ļ�������
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);			//ָ���ص�����
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, handleWrite);	//ָ���ص�����

	// �����ض����������(���Ҵ�������ֹ����̫��)
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);

	// ����301��302��ת����location
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);

	// ���õ�alarm�źţ���ֹ���߳���ʹ�ó�ʱ����
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

	// ����alarm����д���CLOSE_WAIT
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1);

	// ����������SSL��֤����ʹ��CA֤��
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);

	// ��֤�������˷��͵�֤�飬Ĭ���� 2(��)��1���У���0�����ã�
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// ����ʱ��㱨���е���Ϣ�������STDERR��ָ����CURLOPT_STDERR��
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);

	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, "SUREN:SUREN");

	// ʹ��DNS����
	// ����ÿ��CURL���󶼻�����һ�η�����,�������1000��HTTP�������ӵ�ͬһ������,libcurl�ͻ᷵�ش������Ӵ���ͽ��մ���,Ϊ��ʹ��DNS�����Ǻ��б�Ҫ��
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
	// ���ļ�
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
	// ����Ŀ¼
	LPCTSTR lpDir = m_tDownloadInfo.strLocalPath.c_str();
	if (FALSE == ::PathFileExists(lpDir))
	{
		if (FALSE == ::CreateDirectory(lpDir, NULL))
		{
			// ����Ŀ¼ʧ��
			return eDownloadStatus::E_DOWNLOAD_STATUS_CREATE_DIR_FAILED;
		}
	}

	// ���ļ�
	m_hLocalFile = ::CreateFile(m_tDownloadInfo.GetLocalFilePath().c_str()
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL
		, OPEN_ALWAYS
		, FILE_ATTRIBUTE_NORMAL
		, NULL);

	if (INVALID_HANDLE_VALUE == m_hLocalFile)
	{
		// ���ļ�ʧ��
		return eDownloadStatus::E_DOWNLOAD_STATUS_OPENFILE_FAILED;
	}

	// ��ȡ�����������ļ���С��
	curl_off_t dwLocalFileSize = ::GetFileSize(m_hLocalFile, NULL);
	if (INVALID_FILE_SIZE == dwLocalFileSize)
	{
		// ��ȡ�����ļ���Сʧ��
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

	// ���ļ�ָ��ƫ�����ļ�ĩβ(����׷�Ӷ�д)
	DWORD dwResult = ::SetFilePointer(m_hLocalFile, 0, NULL, FILE_END);
	if (HFILE_ERROR == dwResult)
	{
		// ���ñ����ļ�ƫ�ƶ�дʧ��
		return eDownloadStatus::E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED;
	}
		
	//std::string strVersion = curl_version();											// ��ȡlibcurl��ǰ�汾
	curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);									// �����ض����������(���Ҵ�������ֹ����̫��)
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);								// ����301��302��ת����location
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);									// ���õ�alarm�źţ���ֹ���߳���ʹ�ó�ʱ����
	curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1);									// ����alarm����д���CLOSE_WAIT
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);								// ����������SSL��֤����ʹ��CA֤��
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);								// ��֤�������˷��͵�֤�飬Ĭ���� 2(��)��1���У���0�����ã�
	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);										// ����ʱ��㱨���е���Ϣ�������STDERR��ָ����CURLOPT_STDERR��
	curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, "SUREN:SUREN");							// ����һ������[username]:[password]�����ַ���,����PHPȥ����

	curl_easy_setopt(m_pCurl, CURLOPT_URL, m_tDownloadInfo.strUrl.c_str());				// ��������url��ַ
	curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);										// ��ȡͷ	- �ر�
	curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 0L);										// ��ȡ���� - ����
	curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, 0L);										// �ϴ�		- �ر�
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 0L);										// Post		- �ر�
	curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM_LARGE, dwLocalFileSize);				// ����������ʼλ��
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0L);									// ������	- ����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);									// ָ���ص�����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, handleWrite);						// ָ���ص�����
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);								// ָ���ص�����
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, handleProgress);				// ָ���ص�����
	curl_easy_setopt(m_pCurl, CURLOPT_ACCEPT_ENCODING, _T(""));							// ���ý��յı����ʽ
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 30);								// ��������ǰ�ȴ���ʱ��
	//curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT,	30);								// ����ִ�е������
	
	// ����ͨ�ŷ�ʽΪ:����
	m_wFlag = LibcurlFlag::LIBCURLFLAG_DOWNLOAD;

	// ��ʼִ������
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
	// �����ص���������׷�ӵķ�ʽд���ļ�(һ��Ҫ��a������ǰ��д������ݾͻᱻ������)
	CLibcurlDownloader* pThis = static_cast<CLibcurlDownloader*>(userp);
	ASSERT(pThis);

	DWORD dwWritten = 0;
	switch (pThis->m_wFlag)
	{
		case LibcurlFlag::LIBCURLFLAG_NONE:
			break;

		// ����
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
	// curl_get_info������curl_easy_perform֮�����
	// curl_easy_getinfo(m_pCurl, CURLINFO_SPEED_DOWNLOAD, &m_pDownloadInfo->dlSpeed);
	m_tDownloadInfo.dlNowSize = m_tDownloadInfo.dlLastSize + dlnow;
	double dlTotalSize = m_tDownloadInfo.dlTotalSize;
	double dlNowSize = m_tDownloadInfo.dlNowSize;

	// ��ʱ���½�����
	if (m_tDownloadInfo.pDelegate)
	{
		m_tDownloadInfo.pDelegate->OnLibcurlProgress(dlTotalSize, dlNowSize);
	}

	DWORD dwNowTime = ::GetTickCount();
	if (dlnow > 0)
	{
		// ÿ��һ�����һ���ٶ�
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

	// ����ȡHTTPͷ
	m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_URL, m_tDownloadInfo.strUrl.c_str());
	m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 1L);				// ����������HTTP�ķ���������֧��HEAD��ʽ����ȡ(���������ķ�ʽ��ȡ)
	//m_wCurlCode = curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, "GET");		// ʹ��CURLOPT_CUSTOMREQUEST��ȡ
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

	// �����ȡ�Ĵ�С,�ص�֪ͨ
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
		// ���سɹ�
		case E_DOWNLOAD_STATUS_DOWNLOAD_OK:
		{
			this->_OnComplete();
		}break;

		// ��ֹ����
		case E_DOWNLOAD_STATUS_DOWNLOAD_ABORT:
		{
			m_tDownloadInfo.eCode = eStatus;
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;

		// ��������
		case E_DOWNLOAD_STATUS_CONTINUE_TRANSFER:
		{
			m_tDownloadInfo.eCode = eStatus;
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;

		// �ļ�����ʧ��
		case E_DOWNLOAD_STATUS_CREATE_DIR_FAILED:
		case E_DOWNLOAD_STATUS_OPENFILE_FAILED:
		case E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED:
		case E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED:
		{
			m_tDownloadInfo.eCode = eStatus;
			m_tDownloadInfo.eCode2 = GetLastError();
			::PostMessage(m_hWnd, WM_MY_LIBCURL, WPARAM(&m_tDownloadInfo), 0);
		}break;
		
		// ����ʧ��(������case,��Ϊʧ��)
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
	// ��ʼ��
	ZlibUtil* pZlibUtil = new ZlibUtil();
	std::string pzFile = m_tDownloadInfo.GetLocalFilePath();
	std::string pzOutDir = m_tDownloadInfo.strLocalPath + _T("\\..\\");

	// ��ʽ��·��
	TCHAR pzBuff[MAX_PATH] = { 0 };
	::PathCanonicalize(pzBuff, pzOutDir.c_str());
	pzOutDir = pzBuff;

	::PathCanonicalize(pzBuff, pzFile.c_str());
	pzFile = pzBuff;

	// ��ʼ��ѹ
	int iResCode = pZlibUtil->Decompress(pzFile.c_str(), pzOutDir.c_str());
	delete pZlibUtil;

	// ��������ص��ļ�
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
