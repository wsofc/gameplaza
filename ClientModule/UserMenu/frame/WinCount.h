#ifndef WIN_COUNT_H
#define WIN_COUNT_H

namespace UserMenuNSP
{
	class WinCount : public WindowImplBase
	{
	public:
		static WinCount* GetInstance();
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
		LPCTSTR GetWindowClassName(void) const{ return _T("WinCount"); };
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		WinCount();
		~WinCount();
		static WinCount*		g_instance;

		TCHAR					m_pzBuffer[MAX_PATH];	// �ַ���������
		CDuiString				m_strGameID;			// ��ϷID
		CRichEditUI*			m_pEdtGameID;
		
		CDuiString				m_strCount;				// Ӯ�ľ���
		CRichEditUI*			m_pEdtCount;

		CDuiString				m_strCancel;			// ȡ����ť
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// ȷ����ť
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// �رհ�ť
		CButtonUI*				m_pBtnClose;
	};
}

#endif // WIN_COUNT_H
