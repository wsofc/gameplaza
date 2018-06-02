#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

class GameResource
{
private:
	GameResource(){}
	static GameResource* g_instance;

public:
	~GameResource(){}
	static GameResource* getInstance();

	const TCHAR* const MainWndClassName			= _T("HallFrame");						//主窗口类名
	const TCHAR* const MainWndTitleName			= _T("373游戏中心");					//主窗标题名

	const TCHAR* const kBtnUpdatePlaza_Close	= _T("update_plaza-btn_close");			// 关闭
	const TCHAR* const kBtnUpdatePlaza_Download = _T("update_plaza-btn_download");		// 官网下载
	const TCHAR* const kBtnUpdatePlaza_Cancel	= _T("update_plaza-btn_cancel");		// 取消下载
	const TCHAR* const kBtnUpdatePlaza_Progress = _T("update_plaza-progress");			// 下载进度条
	const TCHAR* const kLabUpdatePlaza_Total	= _T("update_plaza-lab_total");			// 下载进度描述
	const TCHAR* const kLabUpdatePlaza_Speed	= _T("update_plaza-lab_speed");			// 下载速度描述
};

class MainFrame : public WindowImplBase, public DownLoadDelegate
{
public:
	~MainFrame();
	static MainFrame* getInstance();
	int CreateFrame(LPCTSTR lpURL);

	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

	void InitData(TNotifyUI& msg);
	void InitDownload();
	void StartGamePlaza(LPCSTR lpApplicationName, LPSTR lpCommandLine);					// 启动大厅进程
	void SetFrozenEnable(bool bEnable);													// 设置面板事件

public:
	void OnOnLibcurDownloadSize(double dlSize);											// 下载文件的大小
	void OnLibcurlProgress(const double& dltotal, const double& dlnow);					// 进度回调
	void OnLibcurlSpeed(const double& dlspeed);											// 速度回调(限定为每秒一次)
	void OnLibcurlComplete();															// 下载完成
	void OnInstallComplete(int iResCode);												// 安装完成
	void OnLibcurlFailed(DownLoadStruct* pDownloadStruct);								// 主线程错误回调

protected:
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	LPCTSTR GetWindowClassName(void) const{ return _T("MainFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	MainFrame();
	static MainFrame*		g_instance;

private:
	TCHAR					m_pzBuff[MAX_PATH];
	CDuiString				m_strUrl;
	CProgressUI*			m_pProgress;
	CLabelUI*				m_pLabTotal;
	CLabelUI*				m_pLabSpeed;
	CButtonUI*				m_pBtnDownload;
	CButtonUI*				m_pBtnCancel;
	CButtonUI*				m_pBtnClose;
	CLibcurlDownloader*		m_pDownloader;
};

#endif //__MAIN_FRAME_H__