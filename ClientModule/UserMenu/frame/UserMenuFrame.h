#ifndef USER_MENU_H
#define USER_MENU_H

namespace UserMenuNSP {
	
	class UserMenu : public WindowImplBase
	{
	public:
		static UserMenu* GetInstance();
		static void Release();
		static bool IsExistInstance(){ return UserMenu::g_Instance != nullptr; }

	public:
		
		void CreatFrame(CPaintManagerUI* pManager);
		void CreatUserMenu(CPaintManagerUI* pManager);

		void InitWindow();
		void Notify(TNotifyUI& msg);
		UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnFinalMessage(HWND hWnd);
		void OnMenuEvent(CDuiString* strMenuName, CControlUI* pControl);

		void InitData(TNotifyUI& msg);
		void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	protected:

		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		CDuiString GetSkinFolder();
		CDuiString GetSkinFile();
		LPCTSTR GetWindowClassName(void) const{ return _T("UserMenu"); };

	private:

		UserMenu();
		~UserMenu();
		static UserMenu*				g_Instance;					// ����

		TCHAR							m_pzBuffer[MAX_PATH];		// �ַ���������
		std::map<CDuiString, bool>		m_MenuCheckInfo;			// ����˵��ĵ�ѡ��ѡ��Ϣ
		CPaintManagerUI*				m_pParentManager;			// �����ڻ�ͼ������
	};
}
#endif // __USER_MENU_H__