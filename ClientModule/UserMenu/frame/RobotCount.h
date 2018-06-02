#ifndef ROBOT_COUNT_H
#define ROBOT_COUNT_H

namespace UserMenuNSP
{
	class RobotCount : public WindowImplBase
	{
	public:
		static RobotCount* GetInstance();
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
		LPCTSTR GetWindowClassName(void) const{ return _T("RobotCount"); };
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		RobotCount();
		~RobotCount();
		static RobotCount*		g_instance;

		TCHAR					m_pzBuffer[MAX_PATH];	// 字符串缓冲区
		
		CDuiString				m_strCount;				// 当前机器人数量
		CRichEditUI*			m_pEdtCount;			// 

		CDuiString				m_strCancel;			// 取消按钮
		CButtonUI*				m_pBtnCancel;

		CDuiString				m_strEnsure;			// 确定按钮
		CButtonUI*				m_pBtnEnsure;

		CDuiString				m_strClose;				// 关闭按钮
		CButtonUI*				m_pBtnClose;
	};
}

#endif // ROBOT_COUNT_H
