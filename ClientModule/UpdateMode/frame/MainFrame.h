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

	const TCHAR* const MainWndClassName			= _T("HallFrame");						//����������
	const TCHAR* const MainWndTitleName			= _T("373��Ϸ����");					//����������

	const TCHAR* const kBtnUpdatePlaza_Close	= _T("update_plaza-btn_close");			// �ر�
	const TCHAR* const kBtnUpdatePlaza_Download = _T("update_plaza-btn_download");		// ��������
	const TCHAR* const kBtnUpdatePlaza_Cancel	= _T("update_plaza-btn_cancel");		// ȡ������
	const TCHAR* const kBtnUpdatePlaza_Progress = _T("update_plaza-progress");			// ���ؽ�����
	const TCHAR* const kLabUpdatePlaza_Total	= _T("update_plaza-lab_total");			// ���ؽ�������
	const TCHAR* const kLabUpdatePlaza_Speed	= _T("update_plaza-lab_speed");			// �����ٶ�����
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
	void StartGamePlaza(LPCSTR lpApplicationName, LPSTR lpCommandLine);					// ������������
	void SetFrozenEnable(bool bEnable);													// ��������¼�

public:
	void OnOnLibcurDownloadSize(double dlSize);											// �����ļ��Ĵ�С
	void OnLibcurlProgress(const double& dltotal, const double& dlnow);					// ���Ȼص�
	void OnLibcurlSpeed(const double& dlspeed);											// �ٶȻص�(�޶�Ϊÿ��һ��)
	void OnLibcurlComplete();															// �������
	void OnInstallComplete(int iResCode);												// ��װ���
	void OnLibcurlFailed(DownLoadStruct* pDownloadStruct);								// ���̴߳���ص�

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