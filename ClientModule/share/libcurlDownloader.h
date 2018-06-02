/**********************************************************************
* @title:		����libcurl���������
*
* @author:		Aries
* @data:		2016/10/26
* @breif:		֧���첽���󡢶��������ء���ͣ���ϵ��������������ص�
*
***********************************************************************/

#ifndef __LIBCURL_DOWNLOADER_H__
#define __LIBCURL_DOWNLOADER_H__

#include <thread>
#include <mutex>

#define WM_MY_LIBCURL		WM_USER + 0x1000		// libcurl �Զ�����Ϣ(�������߳�PostMessage֪ͨ���̸߳���UI��)

// ����ö��
// �����������߳�,��PostMessage�����̸߳���UI
enum eDownloadStatus
{
	E_DOWNLOAD_STATUS_NONE = 0						// ��
	, E_DOWNLOAD_STATUS_CREATE_DIR_FAILED			// ����Ŀ¼ʧ��
	, E_DOWNLOAD_STATUS_OPENFILE_FAILED				// ���ļ�ʧ��
	, E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED			// ��ȡ�����������ļ���Сʧ��
	, E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED		// �����ļ�ƫ�ƶ�дʧ��(����)
	, E_DOWNLOAD_STATUS_REQ_SIZE_BLOCK				// �����ȡ�����ļ���С�߳�����
	, E_DOWNLOAD_STATUS_CONTINUE_TRANSFER			// ��������
	, E_DOWNLOAD_STATUS_DOWNLOAD_FAILED				// ����ʧ��
	, E_DOWNLOAD_STATUS_DOWNLOAD_OK					// ���سɹ�
	, E_DOWNLOAD_STATUS_DOWNLOAD_ABORT				// ��ֹ����
	, E_DOWNLOAD_STATUS_DOWNLOAD_PAUSE				// ��ͣ����
	, E_DOWNLOAD_STATUS_DOWNLOAD_RESUME				// ��������
	, E_DOWNLOAD_STATUS_EXTRA						// ����Ԥ����
};

// ͨ�ŷ�ʽ
enum LibcurlFlag
{
	LIBCURLFLAG_NONE = 0							// ��
	, LIBCURLFLAG_DOWNLOAD							// ����
	, LIBCURLFLAG_POST								// Post����
	, LIBCURLFLAG_GET								// Get����
};

// ���ػص��ӿ�
class DownLoadDelegate
{
public:
	virtual void	OnOnLibcurDownloadSize(double dlSize)							= 0;			// �����ļ��Ĵ�С
	virtual void	OnLibcurlProgress(const double& dltotal, const double& dlnow)	= 0;			// ���Ȼص�
	virtual void	OnLibcurlSpeed(const double& dlspeed)							= 0;			// �ٶȻص�(�޶�Ϊÿ��һ��)
	virtual void	OnLibcurlComplete()												= 0;			// ������ɻص�
	virtual	void	OnInstallComplete(int iResCode)									= 0;			// ��װ���
};

// ���ؽṹ(����������Ϣ��)
struct DownLoadStruct
{
	DownLoadStruct()
	{
		memset(this, 0, sizeof(*this));
	}
	
	BOOL				bStarted;				// �Ƿ��ѿ�ʼ����
	INT					eCode;					// ����״̬��
	INT					eCode2;					// ����״̬��
	DOUBLE				dlTotalSize;			// �����ļ��ܴ�С
	DOUBLE				dlLastSize;				// �ϴ�����ɴ�С
	DOUBLE				dlNowSize;				// ��ǰ����ɴ�С
	DOUBLE				dlSpeed;				// �����ٶ�
	std::string			strFileName;			// �ļ���
	std::string			strUrl;					// ����url��ַ
	std::string			strLocalPath;			// ����·��
	DownLoadDelegate*	pDelegate;				// ���ػص�ί�ж���

	std::string GetLocalFilePath() const
	{
		std::string strPath;
		strPath = strLocalPath;
		strPath += _T("\\");
		strPath += strFileName;
		return strPath;
	}
};

// Libcurl������
class CLibcurlDownloader
{
public:
	CLibcurlDownloader(CPaintManagerUI* pManager);
	~CLibcurlDownloader();

public:

	static bool InitGlobal();																						// ��ʼ��libcurlȫ�ֻ���
	static int CleanGlobal();																						// ����ȫ�ֻ���
	
	bool InitOption();																								// ��ʼ������
	void ReqDownloadSize(LPCTSTR strFileName, LPCTSTR strFilePath, LPCTSTR strUrl, DownLoadDelegate* pDelegate);	// ���������ļ���С

	const DownLoadStruct& GetDownStruct()const { return m_tDownloadInfo; };											// ��ȡ������Ϣ�ṹ��
	DOUBLE GetLocalFileSize(std::string& strFilePath);																// ��ȡ�����ļ���С
	void CalculateByteDesc(double dlSize, std::string& strDesc);													// �����ֽڴ�С��Ӧ���ַ�������
	
	void StartDownLoad();																							// ���������߳�
	void ContinueDownload();																						// ��������
	void ReDownLoad();																								// ��������
	bool IsRunning() const;																							// �����߳��Ƿ���������
	void DeleteLocalFile();																							// ɾ�������ļ�

	eDownloadStatus ExcuteDownload();																				// ִ����������
	void AbortDownload();																							// ��ֹ����
	void PauseDownload();																							// ��ͣ����
	void ResumeDownload();																							// ��������

	bool IsAbortDownload() const;																					// �Ƿ���ֹ����
	bool IsPauseDownload() const;																					// �����Ƿ�����ͣ
	bool IsIMINTDownload() const;																					// �Ƿ������ж�����

	void StartInstall();																							// ������װ�߳�

protected:
	static size_t handleWrite(void *buffer, size_t size, size_t nmemb, void *userp);								// д�����ݻص�(���ж�����ӿ�)
	static size_t handleProgress(void *buffer, double dltotal, double dlnow, double ultotal, double ulnow);			// ���Ȼص�(���ж�����ӿ�)

private:

	static CURLSH* g_share_handle;

	size_t _OnWriteFile(void *buffer, size_t size, size_t nmemb);													// ���ر����ļ�
	size_t _OnProgress(const double& dltotal, const double& dlnow);													// ���Ȼص�
	size_t _OnComplete();																							// �������

	void _GetDownloadSizeThreadRun();																				// ִ�л�ȡ�ļ���С�߳�
	void _DownloadThreadRun();																						// ִ�������߳�
	void _InstallThreadRun();																						// ִ�а�װ�߳�

	void _StopDownLoad();																							// �ر������߳�
	void _CloseCurlHander();																						// �ر�libcurl���
	void _CloseLocalFile();																							// �ر��ļ����

private:
	static bool			g_bInitGlobal;																				// ȫ�ֻ����Ƿ��ʼ��
	HWND				m_hWnd;																						// Ŀ�괰�ھ��
	CPaintManagerUI*	m_pManager;																					// Ŀ�괰�ڻ��ƹ�����
		
	CURL*				m_pCurl;																					// curl ���
	CURLcode			m_wCurlCode;																				// curl ������

	LibcurlFlag			m_wFlag;																					// ͨ�ŷ�ʽ
	HANDLE				m_hLocalFile;																				// �ļ����
	volatile bool		m_bReqSizeRunning;																			// ��ȡ�����ļ���С�߳����б�ʶ
	volatile bool		m_bReqDownRunning;																			// �����߳����б�ʶ
	std::thread			m_thread;																					// �յ�threadִ�ж���
	bool				m_bIMINT;																					// �Ƿ������ж�
	bool				m_bAbort;																					// �Ƿ���ֹ����
	bool				m_bPause;																					// �Ƿ���ͣ����
	DownLoadStruct		m_tDownloadInfo;																			// ������Ϣ
	double				m_dlLastTransferSize;																		// �ϴδ����С
	DWORD				m_dwLastTransferTime;																		// �ϴδ���ʱ��
	DWORD				m_dwLastValidTime;																			// �ϴ���Чʱ��(���10���������ٶ�Ϊ0,��Ͽ�)
	std::string			m_strRespons;																				// ��Ӧ����
};

#endif // __LIBCURL_DOWNLOADER_H__

