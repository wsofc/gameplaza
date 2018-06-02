/**********************************************************************
* @title:		基于libcurl库的下载器
*
* @author:		Aries
* @data:		2016/10/26
* @breif:		支持异步请求、多任务下载、暂停、断点续传、重新下载等
*
***********************************************************************/

#ifndef __LIBCURL_DOWNLOADER_H__
#define __LIBCURL_DOWNLOADER_H__

#include <thread>
#include <mutex>

#define WM_MY_LIBCURL		WM_USER + 0x1000		// libcurl 自定义消息(用于子线程PostMessage通知主线程更新UI等)

// 错误枚举
// 若创建了子线程,则PostMessage到主线程更新UI
enum eDownloadStatus
{
	E_DOWNLOAD_STATUS_NONE = 0						// 无
	, E_DOWNLOAD_STATUS_CREATE_DIR_FAILED			// 创建目录失败
	, E_DOWNLOAD_STATUS_OPENFILE_FAILED				// 打开文件失败
	, E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED			// 获取本地已下载文件大小失败
	, E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED		// 设置文件偏移读写失败(续传)
	, E_DOWNLOAD_STATUS_REQ_SIZE_BLOCK				// 请求获取下载文件大小线程阻塞
	, E_DOWNLOAD_STATUS_CONTINUE_TRANSFER			// 继续下载
	, E_DOWNLOAD_STATUS_DOWNLOAD_FAILED				// 下载失败
	, E_DOWNLOAD_STATUS_DOWNLOAD_OK					// 下载成功
	, E_DOWNLOAD_STATUS_DOWNLOAD_ABORT				// 终止下载
	, E_DOWNLOAD_STATUS_DOWNLOAD_PAUSE				// 暂停下载
	, E_DOWNLOAD_STATUS_DOWNLOAD_RESUME				// 继续下载
	, E_DOWNLOAD_STATUS_EXTRA						// 额外预留项
};

// 通信方式
enum LibcurlFlag
{
	LIBCURLFLAG_NONE = 0							// 无
	, LIBCURLFLAG_DOWNLOAD							// 下载
	, LIBCURLFLAG_POST								// Post请求
	, LIBCURLFLAG_GET								// Get请求
};

// 下载回调接口
class DownLoadDelegate
{
public:
	virtual void	OnOnLibcurDownloadSize(double dlSize)							= 0;			// 下载文件的大小
	virtual void	OnLibcurlProgress(const double& dltotal, const double& dlnow)	= 0;			// 进度回调
	virtual void	OnLibcurlSpeed(const double& dlspeed)							= 0;			// 速度回调(限定为每秒一次)
	virtual void	OnLibcurlComplete()												= 0;			// 下载完成回调
	virtual	void	OnInstallComplete(int iResCode)									= 0;			// 安装完成
};

// 下载结构(保存下载信息等)
struct DownLoadStruct
{
	DownLoadStruct()
	{
		memset(this, 0, sizeof(*this));
	}
	
	BOOL				bStarted;				// 是否已开始下载
	INT					eCode;					// 错误状态码
	INT					eCode2;					// 错误状态码
	DOUBLE				dlTotalSize;			// 下载文件总大小
	DOUBLE				dlLastSize;				// 上次已完成大小
	DOUBLE				dlNowSize;				// 当前已完成大小
	DOUBLE				dlSpeed;				// 下载速度
	std::string			strFileName;			// 文件名
	std::string			strUrl;					// 下载url地址
	std::string			strLocalPath;			// 本地路径
	DownLoadDelegate*	pDelegate;				// 下载回调委托对象

	std::string GetLocalFilePath() const
	{
		std::string strPath;
		strPath = strLocalPath;
		strPath += _T("\\");
		strPath += strFileName;
		return strPath;
	}
};

// Libcurl下载器
class CLibcurlDownloader
{
public:
	CLibcurlDownloader(CPaintManagerUI* pManager);
	~CLibcurlDownloader();

public:

	static bool InitGlobal();																						// 初始化libcurl全局环境
	static int CleanGlobal();																						// 清理全局环境
	
	bool InitOption();																								// 初始化设置
	void ReqDownloadSize(LPCTSTR strFileName, LPCTSTR strFilePath, LPCTSTR strUrl, DownLoadDelegate* pDelegate);	// 请求下载文件大小

	const DownLoadStruct& GetDownStruct()const { return m_tDownloadInfo; };											// 获取下载信息结构体
	DOUBLE GetLocalFileSize(std::string& strFilePath);																// 获取本地文件大小
	void CalculateByteDesc(double dlSize, std::string& strDesc);													// 计算字节大小对应的字符串描述
	
	void StartDownLoad();																							// 启动下载线程
	void ContinueDownload();																						// 继续下载
	void ReDownLoad();																								// 重新下载
	bool IsRunning() const;																							// 下载线程是否正在运行
	void DeleteLocalFile();																							// 删除本地文件

	eDownloadStatus ExcuteDownload();																				// 执行下载请求
	void AbortDownload();																							// 终止下载
	void PauseDownload();																							// 暂停下载
	void ResumeDownload();																							// 继续下载

	bool IsAbortDownload() const;																					// 是否终止下载
	bool IsPauseDownload() const;																					// 下载是否已暂停
	bool IsIMINTDownload() const;																					// 是否立即中断下载

	void StartInstall();																							// 启动安装线程

protected:
	static size_t handleWrite(void *buffer, size_t size, size_t nmemb, void *userp);								// 写入数据回调(所有对象共享接口)
	static size_t handleProgress(void *buffer, double dltotal, double dlnow, double ultotal, double ulnow);			// 进度回调(所有对象共享接口)

private:

	static CURLSH* g_share_handle;

	size_t _OnWriteFile(void *buffer, size_t size, size_t nmemb);													// 下载本地文件
	size_t _OnProgress(const double& dltotal, const double& dlnow);													// 进度回调
	size_t _OnComplete();																							// 下载完成

	void _GetDownloadSizeThreadRun();																				// 执行获取文件大小线程
	void _DownloadThreadRun();																						// 执行下载线程
	void _InstallThreadRun();																						// 执行安装线程

	void _StopDownLoad();																							// 关闭下载线程
	void _CloseCurlHander();																						// 关闭libcurl句柄
	void _CloseLocalFile();																							// 关闭文件句柄

private:
	static bool			g_bInitGlobal;																				// 全局环境是否初始化
	HWND				m_hWnd;																						// 目标窗口句柄
	CPaintManagerUI*	m_pManager;																					// 目标窗口绘制管理器
		
	CURL*				m_pCurl;																					// curl 句柄
	CURLcode			m_wCurlCode;																				// curl 错误码

	LibcurlFlag			m_wFlag;																					// 通信方式
	HANDLE				m_hLocalFile;																				// 文件句柄
	volatile bool		m_bReqSizeRunning;																			// 获取下载文件大小线程运行标识
	volatile bool		m_bReqDownRunning;																			// 下载线程运行标识
	std::thread			m_thread;																					// 空的thread执行对象
	bool				m_bIMINT;																					// 是否立即中断
	bool				m_bAbort;																					// 是否终止下载
	bool				m_bPause;																					// 是否暂停下载
	DownLoadStruct		m_tDownloadInfo;																			// 下载信息
	double				m_dlLastTransferSize;																		// 上次传输大小
	DWORD				m_dwLastTransferTime;																		// 上次传输时间
	DWORD				m_dwLastValidTime;																			// 上次有效时间(如果10秒内下载速度为0,则断开)
	std::string			m_strRespons;																				// 响应内容
};

#endif // __LIBCURL_DOWNLOADER_H__

