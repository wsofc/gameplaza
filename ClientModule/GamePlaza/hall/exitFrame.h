#ifndef __EXIT_FRAME_H__
#define __EXIT_FRAME_H__

#include "hallFrame.h"

class ExitFrame : public WindowImplBase, public CWkeWebkitClientCallback
{
public:
	~ExitFrame();
	static ExitFrame* GetInstance();

	BOOL CreateFrame();

	enum CLOSE_EVENT
	{
		CLOSE_EVENT_NONE = 0	// 无
		, CLOSE_EVENT_SWITCH	// 切换账号
		, CLOSE_EVENT_CANCEL	// 取消
		, CLOSE_EVENT_EXIT		// 退出
		, CLOSE_EVENT_EXTRA		// 额外预留项
	};
public:

	void InitWindow();
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);

	void InitData(TNotifyUI& msg);

public:
	void OnWkeTitleChange(const CDuiString& strTitle);
	void OnWkeUrlChange(const CDuiString& strUrl);

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("ExitFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	CLOSE_EVENT			m_iCloseEvent;

	static ExitFrame*	g_instance;
	ExitFrame();

	CWkeWebkitUI*		m_pWkeCtl;								// wke浏览器控件
};

#endif //__EXIT_FRAME_H__