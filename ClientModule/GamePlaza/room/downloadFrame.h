#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

class DownloadFrame;
class DownloadTask: public DownLoadDelegate
{
public:
	DownloadTask(DownloadFrame* pOwnerFrame, DWORD dwIndex, tagGameKind* pGameKind);
	~DownloadTask();
	CControlUI* GetTaskControl();														// 获取任务控件对象
	DWORD GetTaskIndex();																// 获取任务索引
	WORD GetGameKind();																	// 获取任务对应的游戏ID
	void ForwardIndex(DWORD dwDis = 1);													// 控件上移指定位数
	
	void Init();
	void Download();
	void StopDownload();
	
	static void ResetOnceBlock(){ DownloadTask::g_bOnceBlock = false; }

public:
	void OnOnLibcurDownloadSize(double dlSize);											// 下载文件的大小
	void OnLibcurlProgress(const double& dltotal, const double& dlnow);					// 进度回调
	void OnLibcurlSpeed(const double& dlspeed);											// 速度回调(限定为每秒一次)
	void OnLibcurlComplete();															// 下载完成
	void OnInstallComplete(int iResCode);												// 安装完成
	void OnLibcurlFailed(DownLoadStruct* pDownloadStruct);								// 主线程错误回调

private:
	TCHAR					m_pzBuff[MAX_PATH];											// 字符串缓冲区
	DWORD					m_dwIndex;													// 任务索引
	CLabelUI*				m_pLabName;													// 游戏名lab
	CLabelUI*				m_pLabSize;													// 下载大小lab
	CLabelUI*				m_pLabStatus;												// 描述状态lab
	CLabelUI*				m_pLabSpeed;												// 下载速度lab
	CLabelUI*				m_pLabPercent;												// 下载百分比lab
	CProgressUI*			m_pProgress;												// 进度条控件
	CButtonUI*				m_pBtnCancel;												// 取消下载按钮
	CControlUI*				m_pTaskCtl;													// 任务对象控件
	tagGameKind*			m_pGameKind;												// 游戏结构
	CLibcurlDownloader*		m_pDownloader;												// 下载器
	DownloadFrame*			m_pOwnerFrame;												// 所属窗口
	static bool				g_bOnceBlock;												// 关闭窗口阻塞中
};

class DownloadFrame : public WindowImplBase
{
public:
	~DownloadFrame();
	static DownloadFrame* GetInstance();

	enum DownloadFrameMsgBoxID
	{
		MSG_BOX_ID_NONE = 0					// 无
		, MSG_BOX_ID_ONCEBLOCK				// 关闭窗口阻塞对话框
		, MSG_BOX_ID_EXTRA
	};

public:

	BOOL CreateFrame(CPaintManagerUI* pParentManager, const tagGameKind* pGameKind);
	CPaintManagerUI& GetPaintManager(){ return m_PaintManager; }
	HWND GetHWND(){ return m_hWnd; }
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	
	void InitData(TNotifyUI& msg);

	void AddTaskItem();																// 添加下载任务
	void RemoveTaskItem(DWORD dwIndex);												// 移除指定索引的任务
	void RemoveAllTaskItem();														// 移除所有任务
	void DealCompleteItem(DWORD dwIndex);											// 处理完成的任务列表
	
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

protected:
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	LPCTSTR GetWindowClassName(void) const{ return _T("DownloadFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	CPaintManagerUI*			m_pParentManager;
	tagGameKind					m_tGameKind;
	CContainerUI*				m_pRootCtl;
	std::list<DownloadTask*>	m_vDownloadTask;

private:
	DownloadFrame();
	static DownloadFrame*		g_instance;

};

#endif //__MAIN_FRAME_H__