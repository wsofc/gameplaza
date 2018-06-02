#ifndef SENDSYSMSG_H
#define SENDSYSMSG_H

#define SYS_MSG_LEN			MAX_CHAT_LEN - 20
namespace UserMenuNSP
{
	class SendSysMsg : public WindowImplBase
	{
	public:
		static SendSysMsg* GetInstance();
		static bool IsExistInstance(){ return g_instance != nullptr; }
		int CreatFrame(CPaintManagerUI* pParentManager);

		void InitWindow();
		void Notify(TNotifyUI& msg);
		UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
		void OnFinalMessage(HWND hWnd);

		void InitData(TNotifyUI& msg);
		void ShowLeftWords();
		void Check();
		void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	protected:

		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		CDuiString GetSkinFolder();
		CDuiString GetSkinFile();
		LPCTSTR GetWindowClassName(void) const{ return _T("SendSysMsg"); };
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		SendSysMsg();
		~SendSysMsg();
		static SendSysMsg*		g_instance;

		TCHAR					m_pzBuffer[MAX_PATH];		// 字符串缓冲区
		TCHAR					m_pzSysMsg[MAX_CHAT_LEN];
		CDuiString				m_strSysMsg;				// 系统消息
		CRichEditUI*			m_pEdtSysMsg;

		CDuiString				m_strSendToGame;			// 发送到游戏
		COptionUI*				m_pOptSendToGame;

		CDuiString				m_strSendToRoom;			// 发送到房间
		COptionUI*				m_pOptSendToRoom;

		CDuiString				m_strLeftWords;				// 剩余字数
		CLabelUI*				m_pLabLeftWords;
		
		CDuiString				m_strCancel;				// 取消按钮
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;				// 确定按钮
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;					// 关闭按钮
		CButtonUI*				m_pBtnClose;
	};
}

#endif // SENDSYSMSG_H
