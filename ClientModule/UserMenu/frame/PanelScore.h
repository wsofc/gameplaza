#ifndef MENU_FRAME_H
#define MENU_FRAME_H

namespace UserMenuNSP
{
	class PanelScore : public WindowImplBase
	{
	public:
		static PanelScore* GetInstance();
		static bool IsExistInstance(){ return g_instance != nullptr; }
		int CreatFrame(CPaintManagerUI* pParentManager);

		void InitWindow();
		void Notify(TNotifyUI& msg);
		UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
		void OnFinalMessage(HWND hWnd);
		LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		void InitData(TNotifyUI& msg);
		void UpdateLastData();
		void Check();
		void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	protected:

		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		CDuiString GetSkinFolder();
		CDuiString GetSkinFile();
		LPCTSTR GetWindowClassName(void) const{ return _T("PanelScore"); };
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		PanelScore();
		~PanelScore();
		static PanelScore*		g_instance;

		TCHAR					m_pzBuffer[MAX_PATH];	// 字符串缓冲区
		CDuiString				m_strGameID;			// 游戏ID
		CRichEditUI*			m_pEdtGameID;

		CDuiString				m_strCurValue;			// 当前值
		CLabelUI*				m_pLabCurValue;

		CDuiString				m_strCurCapital;		// 当前值大写
		CLabelUI*				m_pLabCurCapital;

		CDuiString				m_strNewValue;			// 新值
		CRichEditUI*			m_pEdtNewValue;

		CDuiString				m_strNewCapital;		// 新值大写
		CLabelUI*				m_pLabNewCapital;

		CDuiString				m_strCancel;			// 取消按钮
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// 确定按钮
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// 关闭按钮
		CButtonUI*				m_pBtnClose;
	};
}
#endif // __MENU_FRAME_H__
