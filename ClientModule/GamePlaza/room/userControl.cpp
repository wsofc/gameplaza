#include "stdafx.h"
#include "userControl.h"

extern CStaticLogger	g_Logger;
extern GameResource*	R;
extern GameUtil*		g_GameUtil;

class UserList;

UserControl* UserControl::g_instance = nullptr;
UserControl::UserControl()
	: m_pManager(nullptr)
	, m_pRoomManager(nullptr)
	, m_pTagUser(nullptr)
	, m_bLoadSuccess(false)
	, m_hInst(NULL)
	, m_pFuncCreateMenu(nullptr)
	, m_pFuncDeleteMenu(nullptr)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	m_pRoomManager = RoomManager::GetInstance();
}

UserControl::~UserControl()
{
	g_Logger->TryInfo(_T("UserControl - Destroy"));
}

UserControl* UserControl::GetInstance()
{
	if (UserControl::g_instance == nullptr)
	{
		UserControl::g_instance = new UserControl();
	}
	return UserControl::g_instance;
}

void UserControl::CreateGeneralMenu()
{
	g_Logger->TryInfo(_T("UserControl - Create General Menu"));

	// 创建一般的菜单
	POINT p;
	::GetCursorPos(&p);

	CMenuWnd* pMenu = new CMenuWnd();
	::CPoint point(p.x, p.y);
	pMenu->Init(NULL, _T("menu/userMenu.xml"), _T(""), point, m_pManager, &m_MenuCheckInfo, nullptr, eMenuAlignment_Right | eMenuAlignment_Bottom);

	// 昵称
	do
	{
		CMenuElementUI* pEleNickName = pMenu->GetMenuItem(_T("usermenu-nickname"));
		if (pEleNickName)
		{
			_stprintf_s(m_pzBuffer, _T("%s%s"), pEleNickName->GetText().GetData(), m_pTagUser->GetNickName());
			pEleNickName->SetText(m_pzBuffer);
		}
	} while (0);

	// 游戏ID
	do
	{
		CMenuElementUI* pEleUserID = pMenu->GetMenuItem(_T("usermenu-gameid"));
		if (pEleUserID)
		{
			_stprintf_s(m_pzBuffer, _T("%s%lu"), pEleUserID->GetText().GetData(), m_pTagUser->GetGameID());
			pEleUserID->SetText(m_pzBuffer);
		}
	} while (0);

	// 金币数
	do
	{
		CMenuElementUI* pEleGolds = pMenu->GetMenuItem(_T("usermenu-golds"));
		if (pEleGolds)
		{
			_stprintf_s(m_pzBuffer, _T("%s%lld"), pEleGolds->GetText().GetData(), m_pTagUser->GetGameGold());
			pEleGolds->SetText(m_pzBuffer);
		}
	} while (0);

	// 个性签名
	do
	{
		CMenuElementUI* pEleUnderWrite = pMenu->GetMenuItem(_T("usermenu-underwrite"));
		if (pEleUnderWrite)
		{
			_stprintf_s(m_pzBuffer, _T("%s%s"), pEleUnderWrite->GetText().GetData(), m_pTagUser->GetUnderWrite());
			pEleUnderWrite->SetText(m_pzBuffer);
		}
	} while (0);

	pMenu->Show();

	g_Logger->TryInfo(_T("UserControl - Create General Menu Complete"));
}

void UserControl::CreatSpecialMenu()
{
	g_Logger->TryInfo(_T("UserControl - Create Special Menu"));

	// 资源信息
	ResInfo tRes;
	tRes.uVersionHigh	= VER_PLAZA_HIGH;
	tRes.uVersionLow	= VER_PLAZA_LOW;
	tRes.hResourceInst	= GameResource::g_ResourceInst;
	tRes.iResourceType	= GameResource::g_ResourceType;
	tRes.iSkinResID		= GameResource::g_SkinResID;
	tRes.iResourceID	= GameResource::g_ResourceID;
	::StrToCharArray(tRes.pzResPath, GameResource::g_ResourcePath);
	::StrToCharArray(tRes.pzResDllPath, GameResource::g_ResourceDllPath);
	
	// 玩家数据
	UserMenuStruct tUMS;
	tUMS.bSelf			= m_pTagUser->IsSelf();
	tUMS.dwUserID		= m_pTagUser->GetUserID();
	tUMS.dwGameID		= m_pTagUser->GetGameID();
	tUMS.lGameGold		= m_pTagUser->GetGameGold();
	::StrToCharArray(tUMS.szNickname, m_pTagUser->GetNickName());
	::StrToCharArray(tUMS.szUnderWrite, m_pTagUser->GetUnderWrite());

	// 创建控制菜单实例
	m_pFuncCreateMenu(m_pManager, &tRes, &tUMS, this);

	g_Logger->TryInfo(_T("UserControl - Create Special Menu Complete"));
}

bool UserControl::LoadUserMenuDll()
{
	ASSERT(m_pManager);
	if (!m_pManager) return false;

	//动态加载Dll
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	CDuiString strInstancePath = m_pManager->GetInstancePath();
#ifdef DEBUG || _DEBUG
	_stprintf_s(m_pzBuffer, _T("%s%s"), strInstancePath.GetData(), _T("UserMenu_d.dll"));
#else
	_stprintf_s(m_pzBuffer, _T("%s%s"), strInstancePath.GetData(), _T("UserMenu.dll"));
#endif

	m_bLoadSuccess = true;
	m_hInst = ::LoadLibrary(m_pzBuffer);
	if (m_hInst == NULL)
	{
		int e_code = ::GetLastError();
		m_bLoadSuccess = false;
		::FreeLibrary(m_hInst);
	}
	else
	{
		m_pFuncCreateMenu			= (ProcCreatUserMenu)GetProcAddress(m_hInst,		_T("CreatUserMenu"));
		m_pFuncDeleteMenu			= (ProcDeleteUserMenu)GetProcAddress(m_hInst,		_T("DeleteUserMenu"));

		m_pFuncReciveWinScore		= (ProcReciveWinScore)GetProcAddress(m_hInst,		_T("ReciveWinScore"));
		m_pFuncReciveWinCount		= (ProcReciveWinCount)GetProcAddress(m_hInst,		_T("ReciveWinCount"));
		m_pFuncReciveLostCount		= (ProcReciveLostCount)GetProcAddress(m_hInst,		_T("ReciveLostCount"));
		m_pFuncReciveRoomSetting	= (ProcReciveRoomSetting)GetProcAddress(m_hInst,	_T("ReciveRoomSetting"));
		m_pFuncReciveStokSetting	= (ProcReciveStokSetting)GetProcAddress(m_hInst,	_T("ReciveStokSetting"));
		m_pFuncReciveRobotCount		= (ProcReciveRobotCount)GetProcAddress(m_hInst,		_T("ReciveRobotCount"));
		m_pFuncReciveRobotScore		= (ProcReciveRobotScore)GetProcAddress(m_hInst,		_T("ReciveRobotScore"));

		if (!m_pFuncCreateMenu || !m_pFuncDeleteMenu)
		{
			int e_code = ::GetLastError();
			m_bLoadSuccess = false;
			::FreeLibrary(m_hInst);
		}
	}

	return m_bLoadSuccess;
}

void UserControl::UnLoadUserMenuDll()
{
	if (m_hInst)
	{
		::FreeLibrary(m_hInst);

		m_hInst = NULL;
		m_pFuncCreateMenu = nullptr;
		m_pFuncDeleteMenu = nullptr;
	}

	m_bLoadSuccess = false;
	SAFE_DELETE(UserControl::g_instance);
}

void UserControl::PreUnLoadUserMenuDll()
{
	this->m_bLoadSuccess = false;
	if (m_pFuncDeleteMenu && this->m_pManager)
	{
		m_pFuncDeleteMenu(this->m_pManager->GetPaintWindow());
	}
}

void UserControl::ShowMenu(UserData* pUser)
{
	ASSERT(pUser);
	if (!pUser) return;
	m_pTagUser = pUser;
	
	g_Logger->TryInfo(_T("UserControl - ShowMenu [user_id = %lu, user_gameid = %lu]"), pUser->GetUserID(), pUser->GetGameID());

	// 此处检测一次,若之前加载失败,则再加载一次
	// 如果还是失败,则创建普通菜单
	//if (!m_bLoadSuccess)
	//{
	//	m_bLoadSuccess = this->LoadUserMenuDll();
	//}

	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();
	if (m_bLoadSuccess && (tSelf.GetMasterOrder() > 0 || tSelf.IsUnrealUser()))
	{
		this->CreatSpecialMenu();
	}
	else
	{
		this->CreateGeneralMenu();
	}
}

void UserControl::OnGMControData(void* pDataBuffer, WORD wDataSize)
{
	CMD_GM_Option* pGM_Opt = static_cast<CMD_GM_Option*>(pDataBuffer);
	if (!pGM_Opt) return;

	g_Logger->TryInfo(_T("UserControl - OnGMControData [user_id = %lu, op_id = %u, v1 = %lld, v2 = %lld, v3 = %lld]")
		, pGM_Opt->dwUserID, pGM_Opt->btOperationID, pGM_Opt->lValue, pGM_Opt->lValue2, pGM_Opt->lValue3);

	switch (pGM_Opt->btOperationID)
	{
		// 玩家输赢
		case GM_OPERATION_WINORLOST:
		{
			if (m_pFuncReciveWinScore)
			{
				m_pFuncReciveWinScore(pGM_Opt->lValue);
			}
		}break;

		// 赢几局
		case GM_OPERATION_WININNINGS:
		{
			if (m_pFuncReciveWinCount)
			{
				m_pFuncReciveWinCount(pGM_Opt->lValue);
			}
		}break;

		// 输几局
		case GM_OPERATION_LOSTINNINGS:
		{
			if (m_pFuncReciveLostCount)
			{
				m_pFuncReciveLostCount(pGM_Opt->lValue);
			}
		}break;

		// 设置库存
		case GM_OPERATION_STORESCORE:
		{
			if (m_pFuncReciveStokSetting)
			{
				m_pFuncReciveStokSetting(pGM_Opt->lValue);
			}
		}break;

		// 设置机器人数量
		case GM_OPERATION_ROBOTNUMS:
		{
			if (m_pFuncReciveRobotCount)
			{
				m_pFuncReciveRobotCount(pGM_Opt->lValue);
			}
		}break;

		// 设置机器人分数范围
		case GM_OPERATION_ROBOTSCORE:
		{
			if (m_pFuncReciveRobotScore)
			{
				m_pFuncReciveRobotScore(pGM_Opt->lValue, pGM_Opt->lValue2);
			}
		}break;

		default:
			break;
	}
}

void UserControl::OnGMRoomData(void* pDataBuffer, WORD wDataSize)
{
	g_Logger->TryInfo(_T("UserControl - OnGMRoomData"));

	CMD_GR_OptionServer* pOptServer = static_cast<CMD_GR_OptionServer*>(pDataBuffer);
	if (!pOptServer) return;

	if (m_pFuncReciveRoomSetting)
	{
		RoomSetData tRoomSetData;
		memset(&tRoomSetData, 0, sizeof(RoomSetData));

		tRoomSetData.bRoomChat		= pOptServer->bRoomChat;
		tRoomSetData.bRoomWisper	= pOptServer->bRoomWisper;
		tRoomSetData.bGameChat		= pOptServer->bGameChat;
		tRoomSetData.bEnterRoom		= pOptServer->bEnterRoom;
		tRoomSetData.bEnterGame		= pOptServer->bEnterGame;
		tRoomSetData.bShallClose	= pOptServer->bShallClose;

		m_pFuncReciveRoomSetting(tRoomSetData);
	}

	g_Logger->TryInfo(_T("UserControl - OnGMRoomData Complete"));
}

// -----------------------------------------------------------------------------------------
// 控制接口

void UserControl::ReqSetScore()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_WINORLOST;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::ReqWinCount()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_WININNINGS;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::ReqLostCount()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_LOSTINNINGS;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::ReqRoomSetting()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_GETROOMINFO;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::ReqStokSetting()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_STORESCORE;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::ReqRobotCount()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_ROBOTNUMS;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}
void UserControl::ReqRobotScore()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_ROBOTSCORE;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMGETINFO, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackLocation()
{
	Room* pCurRoom = m_pRoomManager->GetCurRoom();
	if (pCurRoom)
	{
		DWORD dwUserID = m_pTagUser->GetUserID();
		pCurRoom->QuickLocation(dwUserID);

		UserList* pUserList = UserList::GetInstance();
		pUserList->SelectByUserID(dwUserID);
	}
}

void UserControl::CallBackSetScore(LONGLONG lGameID, LONGLONG lNewValue)
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_WINORLOST;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	tGM_Opt.lValue = lGameID;
	tGM_Opt.lValue2 = lNewValue;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackWinCount(LONGLONG lGameID, LONGLONG lCount)
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_WININNINGS;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	tGM_Opt.lValue = lGameID;
	tGM_Opt.lValue2 = lCount;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackLostCount(LONGLONG lGameID, LONGLONG lCount)
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_LOSTINNINGS;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	tGM_Opt.lValue = lGameID;
	tGM_Opt.lValue2 = lCount;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackKickRoom()
{
	CMD_GR_KillUser tKillUser;
	tKillUser.dwTargetUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_KILL_USER, &tKillUser, sizeof(CMD_GR_KillUser));
}

void UserControl::CallBackSendSysMsg(LPCTSTR lpMsg, bool bSendToGame, bool bSendToRoom)
{
	CMD_GR_SendMessage tSysMsg;
	tSysMsg.cbGame = bSendToGame;
	tSysMsg.cbRoom = bSendToRoom;
	tSysMsg.wChatLength = min(_tcslen(lpMsg) + 1, ArrayCount(tSysMsg.szSystemMessage) - 1);
	memcpy(tSysMsg.szSystemMessage, lpMsg, tSysMsg.wChatLength * sizeof(TCHAR));
	WORD wDataSize = sizeof(CMD_GR_SendMessage) - sizeof(tSysMsg.szSystemMessage) + tSysMsg.wChatLength * sizeof(TCHAR);
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_SEND_MESSAGE, &tSysMsg, wDataSize);
}

void UserControl::CallBackForbidAccount()
{
	CMD_GR_LimitAccount tLimit;
	tLimit.dwTargetUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_LIMIT_ACCOUNS, &tLimit, sizeof(CMD_GR_LimitAccount));
}

void UserControl::CallBackLookIP()
{
	CMD_GR_LookUserIP tLookIP;
	tLookIP.dwTargetUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_LOOK_USER_IP, &tLookIP, sizeof(CMD_GR_LookUserIP));
}

void UserControl::CallBackRoomSetting(bool bRoomChat, bool bRoomWisper, bool bGameChat
	, bool bEnterRoom, bool bEnterGame, bool bShallClose)
{
	CMD_GR_OptionServer tOptServer;
	tOptServer.bRoomChat = bRoomChat;
	tOptServer.bGameChat = bGameChat;
	tOptServer.bRoomWisper = bRoomWisper;
	tOptServer.bEnterRoom = bEnterRoom;
	tOptServer.bEnterGame = bEnterGame;
	tOptServer.bShallClose = bShallClose;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_OPTION_SERVER, &tOptServer, sizeof(CMD_GR_OptionServer));
}

void UserControl::CallBackStokSetting(LONGLONG lNewValue)
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_STORESCORE;
	tGM_Opt.lValue = lNewValue;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackAddBlackList()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_ADDBLACKUSER;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackDelBlackList()
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_DELBLACKUSER;
	tGM_Opt.dwUserID = m_pTagUser->GetUserID();
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackSetRobotCount(DWORD dwRobotCount)
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_ROBOTNUMS;
	tGM_Opt.lValue = dwRobotCount;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}

void UserControl::CallBackSetRobotScore(LONGLONG lMax, LONGLONG lMin)
{
	CMD_GM_Option tGM_Opt;
	memset(&tGM_Opt, 0, sizeof(CMD_GM_Option));
	tGM_Opt.btOperationID = GM_OPERATION_ROBOTSCORE;
	tGM_Opt.lValue = lMax;
	tGM_Opt.lValue2 = lMin;
	m_pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_GMCONTROL, &tGM_Opt, sizeof(CMD_GM_Option));
}
