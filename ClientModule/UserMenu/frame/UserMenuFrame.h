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
		static UserMenu*				g_Instance;					// 单例

		TCHAR							m_pzBuffer[MAX_PATH];		// 字符串缓冲区
		std::map<CDuiString, bool>		m_MenuCheckInfo;			// 保存菜单的单选复选信息
		CPaintManagerUI*				m_pParentManager;			// 父窗口绘图管理器
	};
}
#endif // __USER_MENU_H__