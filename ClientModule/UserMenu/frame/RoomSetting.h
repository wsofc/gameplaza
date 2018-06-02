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

		TCHAR					m_pzBuffer[MAX_PATH];	// 字符串缓冲区

		CDuiString				m_strRoomChat;			// 允许房间聊天
		COptionUI*				m_pOptRoomChat;

		CDuiString				m_strRoomWisper;		// 允许房间私聊
		COptionUI*				m_pOptRoomWisper;

		CDuiString				m_strGameChat;			// 允许游戏聊天
		COptionUI*				m_pOptGameChat;

		CDuiString				m_strEnterRoom;			// 允许进入房间
		COptionUI*				m_pOptEnterRoom;

		CDuiString				m_strEnterGame;			// 允许游戏
		COptionUI*				m_pOptEnterGame;

		CDuiString				m_strCloseRoom;			// 关闭房间
		COptionUI*				m_pOptCloseRoom;
		
		CDuiString				m_strCancel;			// 取消按钮
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// 确定按钮
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// 关闭按钮
		CButtonUI*				m_pBtnClose;
	};
}

#endif // ROOM_SETTING_H
