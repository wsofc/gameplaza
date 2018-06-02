#ifndef __TRAYMENU_H__
#define __TRAYMENU_H__

using namespace DuiLib;
class TrayMenu : public WindowImplBase
{
public:
	static TrayMenu* GetInstance();
	static bool IsExistInstance(){ return g_instance != nullptr; }

	BOOL CreateFrame(CPaintManagerUI* pParentManager);
	void ShowCurruntWindow(HWND hWnd, int nCmdShow);
	void ResetControlSoon(CControlUI* control);		//临时保存要恢复状态的控件(点击托盘按钮显示窗口时恢复状态)

	~TrayMenu();

	void Notify(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

	void CreateMenu();
	
	//删除托盘图标
	BOOL RemoveTrayIcon();

	//override
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("TrayMenu"); };

protected:

private:
	TrayMenu();
	static TrayMenu* g_instance;
	
	NOTIFYICONDATA m_nid;

	CStdPtrArray m_aWnd;
	CStdPtrArray m_aResetControl;

	void _ShowAllWindow(int nCmdShow);
	void _ResetControl();
	
	
	CPaintManagerUI*			m_pParentManager;
	HWND						m_hParentWnd;
	std::map<CDuiString, bool>	m_MenuCheckInfo; //保存菜单的单选复选信息
};

#endif // __TRAYMENU_H__


