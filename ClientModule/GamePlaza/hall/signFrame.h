#ifndef __SIGNFRAME_H__
#define __SIGNFRAME_H__

class SignFrame : public WindowImplBase
{
public:
	static SignFrame* GetInstance();
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
	LPCTSTR GetWindowClassName(void) const{ return _T("SignFrame"); }
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	SignFrame();
	~SignFrame();
	static SignFrame*			g_instance;
	
	CWkeWebkitUI*				m_pWkeCtl;
	CPaintManagerUI*			m_pParentManager;

};

#endif //__SIGNFRAME_H__