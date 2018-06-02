#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

class DownloadFrame;
class DownloadTask: public DownLoadDelegate
{
public:
	DownloadTask(DownloadFrame* pOwnerFrame, DWORD dwIndex, tagGameKind* pGameKind);
	~DownloadTask();
	CControlUI* GetTaskControl();														// ��ȡ����ؼ�����
	DWORD GetTaskIndex();																// ��ȡ��������
	WORD GetGameKind();																	// ��ȡ�����Ӧ����ϷID
	void ForwardIndex(DWORD dwDis = 1);													// �ؼ�����ָ��λ��
	
	void Init();
	void Download();
	void StopDownload();
	
	static void ResetOnceBlock(){ DownloadTask::g_bOnceBlock = false; }

public:
	void OnOnLibcurDownloadSize(double dlSize);											// �����ļ��Ĵ�С
	void OnLibcurlProgress(const double& dltotal, const double& dlnow);					// ���Ȼص�
	void OnLibcurlSpeed(const double& dlspeed);											// �ٶȻص�(�޶�Ϊÿ��һ��)
	void OnLibcurlComplete();															// �������
	void OnInstallComplete(int iResCode);												// ��װ���
	void OnLibcurlFailed(DownLoadStruct* pDownloadStruct);								// ���̴߳���ص�

private:
	TCHAR					m_pzBuff[MAX_PATH];											// �ַ���������
	DWORD					m_dwIndex;													// ��������
	CLabelUI*				m_pLabName;													// ��Ϸ��lab
	CLabelUI*				m_pLabSize;													// ���ش�Сlab
	CLabelUI*				m_pLabStatus;												// ����״̬lab
	CLabelUI*				m_pLabSpeed;												// �����ٶ�lab
	CLabelUI*				m_pLabPercent;												// ���ذٷֱ�lab
	CProgressUI*			m_pProgress;												// �������ؼ�
	CButtonUI*				m_pBtnCancel;												// ȡ�����ذ�ť
	CControlUI*				m_pTaskCtl;													// �������ؼ�
	tagGameKind*			m_pGameKind;												// ��Ϸ�ṹ
	CLibcurlDownloader*		m_pDownloader;												// ������
	DownloadFrame*			m_pOwnerFrame;												// ��������
	static bool				g_bOnceBlock;												// �رմ���������
};

class DownloadFrame : public WindowImplBase
{
public:
	~DownloadFrame();
	static DownloadFrame* GetInstance();

	enum DownloadFrameMsgBoxID
	{
		MSG_BOX_ID_NONE = 0					// ��
		, MSG_BOX_ID_ONCEBLOCK				// �رմ��������Ի���
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

	void AddTaskItem();																// �����������
	void RemoveTaskItem(DWORD dwIndex);												// �Ƴ�ָ������������
	void RemoveAllTaskItem();														// �Ƴ���������
	void DealCompleteItem(DWORD dwIndex);											// ������ɵ������б�
	
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