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

		TCHAR					m_pzBuffer[MAX_PATH];	// �ַ���������
		CDuiString				m_strGameID;			// ��ϷID
		CRichEditUI*			m_pEdtGameID;

		CDuiString				m_strCurValue;			// ��ǰֵ
		CLabelUI*				m_pLabCurValue;

		CDuiString				m_strCurCapital;		// ��ǰֵ��д
		CLabelUI*				m_pLabCurCapital;

		CDuiString				m_strNewValue;			// ��ֵ
		CRichEditUI*			m_pEdtNewValue;

		CDuiString				m_strNewCapital;		// ��ֵ��д
		CLabelUI*				m_pLabNewCapital;

		CDuiString				m_strCancel;			// ȡ����ť
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// ȷ����ť
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// �رհ�ť
		CButtonUI*				m_pBtnClose;
	};
}
#endif // __MENU_FRAME_H__
