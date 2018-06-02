#ifndef LOST_COUNT_H
#define LOST_COUNT_H

namespace UserMenuNSP
{
	class LostCount : public WindowImplBase
	{
	public:
		static LostCount* GetInstance();
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
		LPCTSTR GetWindowClassName(void) const{ return _T("LostCount"); };
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		LostCount();
		~LostCount();
		static LostCount*		g_instance;

		TCHAR					m_pzBuffer[MAX_PATH];	// �ַ���������
		CDuiString				m_strGameID;			// ��ϷID
		CRichEditUI*			m_pEdtGameID;

		CDuiString				m_strCount;				// ��ľ���
		CRichEditUI*			m_pEdtCount;

		CDuiString				m_strCancel;			// ȡ����ť
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// ȷ����ť
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// �رհ�ť
		CButtonUI*				m_pBtnClose;
	};
}

#endif // LOST_COUNT_H
