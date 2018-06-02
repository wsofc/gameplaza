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
		CLOSE_EVENT_NONE = 0	// ��
		, CLOSE_EVENT_SWITCH	// �л��˺�
		, CLOSE_EVENT_CANCEL	// ȡ��
		, CLOSE_EVENT_EXIT		// �˳�
		, CLOSE_EVENT_EXTRA		// ����Ԥ����
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

	CWkeWebkitUI*		m_pWkeCtl;								// wke������ؼ�
};

#endif //__EXIT_FRAME_H__