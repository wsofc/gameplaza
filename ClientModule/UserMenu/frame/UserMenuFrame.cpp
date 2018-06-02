#include "stdafx.h"
#include "UserMenuFrame.h"

namespace UserMenuNSP {
	
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	UserMenu* UserMenu::g_Instance = nullptr;
	UserMenu::UserMenu()
		: m_pParentManager(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	UserMenu::~UserMenu()
	{

	}

	UserMenu* UserMenu::GetInstance()
	{
		if (!g_Instance)
		{
			g_Instance = new UserMenu();
		}
		return g_Instance;
	}

	void UserMenu::Release()
	{
		// 卸载前一定要关闭所有窗口
		do
		{
			// 删除"设置输赢分数"面板
			if (PanelScore::IsExistInstance())
			{
				PanelScore* pFramePanelScore = PanelScore::GetInstance();
				pFramePanelScore->Close();
			}

			// 删除"设置赢几局"面板
			if (WinCount::IsExistInstance())
			{
				WinCount* pFrame = WinCount::GetInstance();
				pFrame->Close();
			}

			// 删除"设置输几局"面板
			if (LostCount::IsExistInstance())
			{
				LostCount* pFrame = LostCount::GetInstance();
				pFrame->Close();
			}

			// 删除"发送系统消息"面板
			if (SendSysMsg::IsExistInstance())
			{
				SendSysMsg* pFrame = SendSysMsg::GetInstance();
				pFrame->Close();
			}

			// 删除"房间设置"面板
			if (RoomSetting::IsExistInstance())
			{
				RoomSetting* pFrame = RoomSetting::GetInstance();
				pFrame->Close();
			}

			// 删除"设置当前库存"面板
			if (StokSetting::IsExistInstance())
			{
				StokSetting* pFrame = StokSetting::GetInstance();
				pFrame->Close();
			}

			// 删除"设置机器人数量"面板
			if (RobotCount::IsExistInstance())
			{
				RobotCount* pFrame = RobotCount::GetInstance();
				pFrame->Close();
			}

			// 删除"设置机器人分数范围"面板
			if (RobotScore::IsExistInstance())
			{
				RobotScore* pFrame = RobotScore::GetInstance();
				pFrame->Close();
			}

		} while (0);

		// 关闭菜单窗口
		do 
		{
			if (g_Instance)
			{
				g_Instance->Close();

				// 发送卸载Dll消息
				::PostMessage(g_Instance->m_pParentManager->GetPaintWindow(), WM_UNINSTALL_USERMENU, NULL, NULL);
			}
		} while (0);
	}
	
	void UserMenu::CreatFrame(CPaintManagerUI* pManager)
	{
		ASSERT(pManager);
		if (!pManager) return;

		m_pParentManager = pManager;

		HWND hParentWnd = m_pParentManager->GetPaintWindow();
		this->Create(hParentWnd, NULL, UI_WNDSTYLE_CONTAINER, 0L, 0, 0, 0, 0);
		//this->ShowWindow(false);
		
		this->CreatUserMenu(m_pParentManager);
	}

	void UserMenu::CreatUserMenu(CPaintManagerUI* pManager)
	{
		ASSERT(pManager);
		if (!pManager) return;
		m_pParentManager = pManager;

		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));

		POINT p;
		::GetCursorPos(&p);

		CMenuWnd* pMenuWnd = new CMenuWnd();
		::CPoint point(p.x, p.y);
		pMenuWnd->Init(NULL, IDR_XML_UserMenu, _T(""), point, &m_PaintManager, &m_MenuCheckInfo, nullptr, eMenuAlignment_Right | eMenuAlignment_Bottom);
		if (g_pDelegate)
		{
			// 昵称
			do
			{
				CMenuElementUI* pEleNickName = pMenuWnd->GetMenuItem(_T("usermenu-nickname"));
				if (pEleNickName)
				{
					_stprintf_s(m_pzBuffer, _T("%s%s"), pEleNickName->GetText().GetData(), g_tMenuData.szNickname);
					pEleNickName->SetText(m_pzBuffer);
				}
			} while (0);

			// 游戏ID
			do
			{
				CMenuElementUI* pEleUserID = pMenuWnd->GetMenuItem(_T("usermenu-gameid"));
				if (pEleUserID)
				{
					_stprintf_s(m_pzBuffer, _T("%s%u"), pEleUserID->GetText().GetData(), g_tMenuData.dwGameID);
					pEleUserID->SetText(m_pzBuffer);
				}
			} while (0);

			// 金币数
			do
			{
				CMenuElementUI* pEleGolds = pMenuWnd->GetMenuItem(_T("usermenu-golds"));
				if (pEleGolds)
				{
					_stprintf_s(m_pzBuffer, _T("%s%lld"), pEleGolds->GetText().GetData(), g_tMenuData.lGameGold);
					pEleGolds->SetText(m_pzBuffer);
				}
			} while (0);

			// 个性签名
			do
			{
				CMenuElementUI* pEleUnderWrite = pMenuWnd->GetMenuItem(_T("usermenu-underwrite"));
				if (pEleUnderWrite)
				{
					_stprintf_s(m_pzBuffer, _T("%s%s"), pEleUnderWrite->GetText().GetData(), g_tMenuData.szUnderWrite);
					pEleUnderWrite->SetText(m_pzBuffer);
				}
			} while (0);
		}


		// 如果是自己,则禁用一些功能(例如:踢除自己)
		if (g_tMenuData.bSelf)
		{
			pMenuWnd->SetMenuItemEnable(_T("usermenu-kick_room"), false);
		}

		pMenuWnd->Show();
	}

	void UserMenu::InitWindow()
	{
	}

	void UserMenu::InitData(TNotifyUI& msg)
	{
	}

	void UserMenu::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		CDuiString& sType = msg.sType;
		CDuiString& sName = msg.pSender->GetName();

		switch (id)
		{
			case MsgEventID::MSGID_KICK_ROOM:
			{
				if (_tcscmp(sName, MsgBoxWnd::g_kBtnDialog_Ensure) == 0 && g_pDelegate)
				{
					g_pDelegate->CallBackKickRoom();
				}
			}break;

			case MsgEventID::MSGID_FORBID_ACCOUNT:
			{
				if (_tcscmp(sName, MsgBoxWnd::g_kBtnDialog_Ensure) == 0 && g_pDelegate)
				{
					g_pDelegate->CallBackForbidAccount();
				}
			}break;
			
			default:
				break;
		}
	}

	LRESULT UserMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_MENUCLICK:
			{
				CDuiString* strMenuName = (CDuiString*)(wParam);
				CControlUI* pControl = (CControlUI*)(lParam);
				this->OnMenuEvent(strMenuName, pControl);
				return 0;
			}break;

			default:
				break;
		}

		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	}

	void UserMenu::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_Instance;
		g_Instance = nullptr;
	}

	void UserMenu::OnMenuEvent(CDuiString* strMenuName, CControlUI* pControl)
	{
		if (!strMenuName) return;

		// 昵称
		if (_tcscmp(*strMenuName, _T("usermenu-nickname")) == 0)
		{
		}

		// 游戏ID
		else if (_tcscmp(*strMenuName, _T("usermenu-gameid")) == 0)
		{
		}

		// 金币
		else if (_tcscmp(*strMenuName, _T("usermenu-golds")) == 0)
		{
		}

		// 个性签名
		else if (_tcscmp(*strMenuName, _T("usermenu-underwrite")) == 0)
		{
		}

		// 定位玩家
		else if (_tcscmp(*strMenuName, _T("usermenu-location")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->CallBackLocation();
			}
		}

		// 设置输赢分数
		else if(_tcscmp(*strMenuName, _T("usermenu-set_score")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqSetScore();
			}
			PanelScore* pFrame = PanelScore::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}

		// 设置赢几局
		else if (_tcscmp(*strMenuName, _T("usermenu-set_win_count")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqWinCount();
			}
			WinCount* pFrame = WinCount::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}

		// 设置输几局
		else if (_tcscmp(*strMenuName, _T("usermenu-set_lost_count")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqLostCount();
			}
			LostCount* pFrame = LostCount::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}

		// 踢出房间
		else if (_tcscmp(*strMenuName, _T("usermenu-kick_room")) == 0)
		{
			_stprintf_s(m_pzBuffer, _T("是否确定把\"%s\"踢出房间?"), g_tMenuData.szNickname);
			DialogCallBack dlg_cb = CC_CALLBACK_2(UserMenu::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(m_pParentManager
				, _T("温馨提示")
				, m_pzBuffer
				, (DLG_ID)MsgEventID::MSGID_KICK_ROOM
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}
		
		// 发送系统消息
		else if (_tcscmp(*strMenuName, _T("usermenu-send_sys")) == 0)
		{
			SendSysMsg* pFrame = SendSysMsg::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}
		
		// 禁用用户账号
		else if (_tcscmp(*strMenuName, _T("usermenu-fobid_account")) == 0)
		{
			_stprintf_s(m_pzBuffer, _T("是否确定禁用用户\"%s\"?"), g_tMenuData.szNickname);
			DialogCallBack dlg_cb = CC_CALLBACK_2(UserMenu::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(m_pParentManager
				, _T("温馨提示")
				, m_pzBuffer
				, (DLG_ID)MsgEventID::MSGID_FORBID_ACCOUNT
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}

		// 查看用户IP
		else if (_tcscmp(*strMenuName, _T("usermenu-look_ip")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->CallBackLookIP();
			}
		}

		// 房间设置
		else if (_tcscmp(*strMenuName, _T("usermenu-set_room")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqRoomSetting();
			}
			RoomSetting* pFrame = RoomSetting::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}

		// 设置当前库存
		else if (_tcscmp(*strMenuName, _T("usermenu-set_stock")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqStokSetting();
			}
			StokSetting* pFrame = StokSetting::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}

		// 加入黑名单
		else if (_tcscmp(*strMenuName, _T("usermenu-add_blacklist")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->CallBackAddBlackList();
			}
		}

		// 删除黑名单
		else if (_tcscmp(*strMenuName, _T("usermenu-del_blacklist")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->CallBackDelBlackList();
			}
		}

		// 设置机器人数量
		else if (_tcscmp(*strMenuName, _T("usermenu-set_robot_count")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqRobotCount();
			}
			RobotCount* pFrame = RobotCount::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}

		// 设置机器人分数范围
		else if (_tcscmp(*strMenuName, _T("usermenu-set_robot_score")) == 0)
		{
			if (g_pDelegate)
			{
				g_pDelegate->ReqRobotScore();
			}
			RobotScore* pFrame = RobotScore::GetInstance();
			pFrame->CreatFrame(m_pParentManager);
		}
	}

	void UserMenu::Notify(TNotifyUI& msg)
	{
		CDuiString& sType = msg.sType;
		CDuiString& sName = msg.pSender->GetName();

		if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		{
			this->InitData(msg);
		}
		else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
		{
		}
	}

	UILIB_RESOURCETYPE UserMenu::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR UserMenu::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString UserMenu::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString UserMenu::GetSkinFile()
	{
		return IDR_XML_UserMenuWin;
	}
}