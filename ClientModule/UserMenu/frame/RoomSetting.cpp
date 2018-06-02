#include "stdafx.h"
#include "RoomSetting.h"

namespace UserMenuNSP
{
	extern NumberConvert 	g_NumberConvert;
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	RoomSetting* RoomSetting::g_instance = nullptr;
	RoomSetting::RoomSetting()
		: m_pOptEnterRoom(nullptr)
		, m_pOptRoomChat(nullptr)
		, m_pOptRoomWisper(nullptr)
		, m_pOptGameChat(nullptr)
		, m_pOptCloseRoom(nullptr)
		, m_pOptEnterGame(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	RoomSetting::~RoomSetting()
	{
	}

	RoomSetting* RoomSetting::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new RoomSetting();
		}

		return g_instance;
	}

	int RoomSetting::CreatFrame(CPaintManagerUI* pParentManager)
	{
		ASSERT(pParentManager);
		if (!pParentManager)
		{
			return FALSE;
		}

		if (FALSE == ::IsWindow(m_hWnd))
		{
			// 恢复大厅主窗口
			HWND hParentWnd = pParentManager->GetPaintWindow();
			m_hWnd = this->Create(hParentWnd, NULL, UI_CLASSSTYLE_DIALOG, 0L);
			this->CenterWindow();
			this->ShowWindow(true);
		}
		else
		{
			this->CenterWindow();
			this->ShowWindow(true);
		}

		return TRUE;
	}

	void RoomSetting::InitWindow()
	{
		// 初始化控件名
		m_strRoomChat		= _T("panel_room_setting-opt_room_chat");
		m_strRoomWisper		= _T("panel_room_setting-opt_room_wisper");
		m_strGameChat		= _T("panel_room_setting-opt_game_chat");
		m_strEnterRoom		= _T("panel_room_setting-opt_enter_room");
		m_strEnterGame		= _T("panel_room_setting-opt_enter_game");
		m_strCloseRoom		= _T("panel_room_setting-opt_close_room");
		m_strCancel			= _T("panel_room_setting-btn_cancel");
		m_strEnsure			= _T("panel_room_setting-btn_ensure");
		m_strClose			= _T("panel_room_setting-btn_close");

		// 初始化控件对象
		m_pOptRoomChat		= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strRoomChat));
		m_pOptRoomWisper	= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strRoomWisper));
		m_pOptGameChat		= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strGameChat));
		m_pOptEnterRoom		= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strEnterRoom));
		m_pOptEnterGame		= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strEnterGame));
		m_pOptCloseRoom		= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strCloseRoom));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void RoomSetting::InitData(TNotifyUI& msg)
	{
	}

	void RoomSetting::UpdateLastData()
	{
		// 更新上次设置
		m_pOptRoomChat->Selected(g_tMenuData.tRoomSetData.bRoomChat);
		m_pOptRoomWisper->Selected(g_tMenuData.tRoomSetData.bRoomWisper);
		m_pOptGameChat->Selected(g_tMenuData.tRoomSetData.bGameChat);
		m_pOptEnterRoom->Selected(g_tMenuData.tRoomSetData.bEnterRoom);
		m_pOptEnterGame->Selected(g_tMenuData.tRoomSetData.bEnterGame);
		m_pOptCloseRoom->Selected(g_tMenuData.tRoomSetData.bShallClose);
	}
	
	void RoomSetting::Notify(TNotifyUI& msg)
	{
		CDuiString& sType = msg.sType;
		CDuiString& sName = msg.pSender->GetName();

		if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		{
			this->InitData(msg);
		}
		else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
		{
			if (_tcscmp(sName, m_strClose.GetData()) == 0)
			{
				this->Close();
			}
			else if (_tcscmp(sName, m_strCancel.GetData()) == 0)
			{
				this->Close();
			}
			else if (_tcscmp(sName, m_strEnsure.GetData()) == 0)
			{
				this->Check();
			}
		}
	}
	
	void RoomSetting::Check()
	{
		if (g_pDelegate)
		{
			bool bRoomChat		= m_pOptRoomChat->IsSelected(); 
			bool bRoomWisper	= m_pOptRoomWisper->IsSelected();
			bool bGameChat		= m_pOptGameChat->IsSelected();
			bool bEnterRoom		= m_pOptEnterRoom->IsSelected();
			bool bEnterGame		= m_pOptEnterGame->IsSelected();
			bool bShallClose	= m_pOptCloseRoom->IsSelected();

			g_pDelegate->CallBackRoomSetting(bRoomChat, bRoomWisper, bGameChat
				, bEnterRoom, bEnterGame, bShallClose);
		}

		this->Close();
	}

	void RoomSetting::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE RoomSetting::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR RoomSetting::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString RoomSetting::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString RoomSetting::GetSkinFile()
	{
		return IDR_XML_PanelRoomSetting;
	}

	LRESULT RoomSetting::ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (m_PaintManager.GetRoot()->IsKeyboardEnabled())
		{
			if (wParam == VK_RETURN)
			{
				return FALSE;
			}
			else if (wParam == VK_ESCAPE)
			{
				this->Close();
				return TRUE;
			}
		}

		return FALSE;
	}
}
