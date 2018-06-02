#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

class ActivityFrame : public WindowImplBase
{
public:
	static ActivityFrame* GetInstance();
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
	LPCTSTR GetWindowClassName(void) const{ return _T("ActivityFrame"); }
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	ActivityFrame();
	~ActivityFrame();
	static ActivityFrame*		g_instance;

	TCHAR						m_pzBuffer[MAX_PATH];		// ×Ö·û´®»º³åÇø
	CTabLayoutUI*				m_pTabLaout;				// »î¶¯Ãæ°åÈÝÆ÷
	CPaintManagerUI*			m_pParentManager;			// ¸¸´°¿Ú»æÍ¼¹ÜÀíÆ÷

};

#endif //__ACTIVITY_H__