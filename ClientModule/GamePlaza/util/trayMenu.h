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
	void ResetControlSoon(CControlUI* control);		//��ʱ����Ҫ�ָ�״̬�Ŀؼ�(������̰�ť��ʾ����ʱ�ָ�״̬)

	~TrayMenu();

	void Notify(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

	void CreateMenu();
	
	//ɾ������ͼ��
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
	std::map<CDuiString, bool>	m_MenuCheckInfo; //����˵��ĵ�ѡ��ѡ��Ϣ
};

#endif // __TRAYMENU_H__


