#ifndef ROOM_SETTING_H
#define ROOM_SETTING_H

namespace UserMenuNSP
{
	class RoomSetting : public WindowImplBase
	{
	public:
		static RoomSetting* GetInstance();
		static bool IsExistInstance(){ return g_instance != nullptr; }
		int CreatFrame(CPaintManagerUI* pParentManager);

		void InitWindow();
		void Notify(TNotifyUI& msg);
		UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
		void OnFinalMessage(HWND hWnd);

		void InitData(TNotifyUI& msg);
		void UpdateLastData();
		void Check();
		
	protected:

		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		CDuiString GetSkinFolder();
		CDuiString GetSkinFile();
		LPCTSTR GetWindowClassName(void) const{ return _T("RoomSetting"); };
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		RoomSetting();
		~RoomSetting();
		static RoomSetting*		g_instance;

		TCHAR					m_pzBuffer[MAX_PATH];	// �ַ���������

		CDuiString				m_strRoomChat;			// ����������
		COptionUI*				m_pOptRoomChat;

		CDuiString				m_strRoomWisper;		// ������˽��
		COptionUI*				m_pOptRoomWisper;

		CDuiString				m_strGameChat;			// ������Ϸ����
		COptionUI*				m_pOptGameChat;

		CDuiString				m_strEnterRoom;			// ������뷿��
		COptionUI*				m_pOptEnterRoom;

		CDuiString				m_strEnterGame;			// ������Ϸ
		COptionUI*				m_pOptEnterGame;

		CDuiString				m_strCloseRoom;			// �رշ���
		COptionUI*				m_pOptCloseRoom;
		
		CDuiString				m_strCancel;			// ȡ����ť
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// ȷ����ť
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// �رհ�ť
		CButtonUI*				m_pBtnClose;
	};
}

#endif // ROOM_SETTING_H
