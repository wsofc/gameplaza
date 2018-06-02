#ifndef __DOLEFRAME_H__
#define __DOLEFRAME_H__

class DoleFrame : public WindowImplBase
{
public:
	static DoleFrame* GetInstance();
	BOOL CreateFrame(CPaintManagerUI* pParentManager);

public:

	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

	void InitData(TNotifyUI& msg);

protected:

	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	LPCTSTR GetWindowClassName(void) const{ return _T("DoleFrame"); }
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	DoleFrame();
	~DoleFrame();
	static DoleFrame*			g_instance;
	
	CWkeWebkitUI*				m_pWkeCtl;
	CPaintManagerUI*			m_pParentManager;

};

#endif //__DOLEFRAME_H__