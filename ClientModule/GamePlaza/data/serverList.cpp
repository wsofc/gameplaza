#include "stdafx.h"
#include "serverList.h"
#include "..\\hall\\hallFrame.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

ServerList* ServerList::g_instance = nullptr;
ServerList::ServerList()
	: m_pManagerRef(nullptr)
	, m_uRefreshType(RefreshType::RF_TYPE_NONE)
	, m_pServiceData(nullptr)
	, m_pLoginServer(nullptr)
	, m_iCurOperateStatus(OperateStatus::OS_NONE)
{
	m_pServiceData = ServiceData::GetInstance();
	m_pLoginServer = LoginServer::GetInstance();
}

ServerList::~ServerList()
{
	if (LoginServer::IsInstanceExist())
	{
		m_pLoginServer->SetDelegate(nullptr, m_pManagerRef);
		m_pLoginServer->CloseSocket();
	}
}

ServerList* ServerList::GetInstance()
{
	if (ServerList::g_instance == nullptr)
	{
		ServerList::g_instance = new (std::nothrow)ServerList();
	}
	return ServerList::g_instance;
}

void ServerList::RefreshList(CPaintManagerUI* pManagerRef, RefreshType uType)
{
	m_pManagerRef = pManagerRef;
	m_uRefreshType = uType;

	// 若非正常调用,则一定要检查实例是否为野指针
	if (!LoginServer::IsInstanceExist()) return;

	if (m_pLoginServer->IsConnect())
	{
		this->_RefreshListReq();
	}
	else
	{
		m_iCurOperateStatus = OperateStatus::OS_REFRESH_LIST;
		m_pLoginServer->SetDelegate(this, m_pManagerRef);
		m_pLoginServer->Connect();
	}
}

void ServerList::_RefreshListReq()
{
	tagLS_GetServerInfo stu;
	stu.btPlatform = MACHINE_PC;
	m_pLoginServer->SendData(MAIN_LS_LOGON, SUB_C2LS_GETSERVERINFO_REQ, &stu, sizeof(tagLS_GetServerInfo));
}

void ServerList::_InitGameConfigList(void* pData, WORD wDataSize)
{
	tagLS_ListConfig* pStruct = static_cast<tagLS_ListConfig*>(pData);
	if (pStruct)
	{
	}
}

void ServerList::_InitGameTypeList(void* pData, WORD wDataSize)
{
	tagGameType* pStruct = static_cast<tagGameType*>(pData);
	if (!pStruct || !m_pServiceData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameType);
	if (wItemCount > 0)
	{
		m_pServiceData->DeleteAllGameType();
	}

	for (size_t i = 0; i < wItemCount; ++i)
	{
		m_pServiceData->InsertGameType(pStruct++);
	}
}

void ServerList::_InitGameKindList(void* pData, WORD wDataSize)
{
	tagGameKind* pStruct = static_cast<tagGameKind*>(pData);
	if (!pStruct || !m_pServiceData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameKind);
	if (wItemCount > 0)
	{
		m_pServiceData->DeleteAllGameKind();
	}

	for (size_t i = 0; i < wItemCount; ++i)
	{
		m_pServiceData->InsertGameKind(pStruct++);
	}
}

void ServerList::_initGameStationList(void* pData, WORD wDataSize)
{
	tagGameStation* pStruct = static_cast<tagGameStation*>(pData);
	if (!pStruct || !m_pServiceData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameStation);
	if (wItemCount > 0)
	{
		m_pServiceData->DeleteAllGameStation();
	}

	for (size_t i = 0; i < wItemCount; ++i)
	{
		m_pServiceData->InsertGameStation(pStruct++);
	}
}

void ServerList::_InitGameRoomList(void* pData, WORD wDataSize)
{
	DataManager* pDataMgr = DataManager::GetInstance();
	UserData& tSelf = pDataMgr->GetSelfData();
	tagGameServer* pStruct = static_cast<tagGameServer*>(pData);
	if (!pStruct || !m_pServiceData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameServer);
	if (wItemCount > 0)
	{
		m_pServiceData->DeleteAllGameServer();
	}

	for (size_t i = 0; i < wItemCount; ++i)
	{
		tagGameServer* pTmp = pStruct++;
		if (pTmp->bMobileRoom && (pTmp->wKindID == 30 || pTmp->wKindID == 31)
			&& (tSelf.GetMasterOrder() <= 0 && !tSelf.IsUnrealUser()))
		{
			continue;
		}

		m_pServiceData->InsertGameServer(pTmp);
	}
}

void ServerList::_InitGameListComplete()
{
	m_pLoginServer->SetDelegate(nullptr, nullptr);
	m_pLoginServer->CloseSocket();

	switch (m_uRefreshType)
	{
		case RefreshType::RF_TYPE_NONE:
			break;

		case RefreshType::RF_TYPE_GAMELIST:
		{
			HallFrame* pHallFrame = HallFrame::GetInstance();
			int iOptSelectIndex = pHallFrame->GetGameOptSelectIndex();
			pHallFrame->CleanGameList();
			pHallFrame->InitGameList();
			pHallFrame->SelectGameOptByIndex(iOptSelectIndex);
		}break;

		case RefreshType::RF_TYPE_ROOMLIST:
		{
			RoomManager* pRoomManager = RoomManager::GetInstance();
			pRoomManager->RefreshRoomList();
			pRoomManager->SetRoomListExpand(true);
		}break;

		default:
			break;
	}

	m_uRefreshType = RefreshType::RF_TYPE_NONE;
}

bool ServerList::OnConnectSuccessProc()
{
	g_Logger->TryInfo(_T("LoginFrame - ConnectServer OK"));
	switch (m_iCurOperateStatus)
	{
		case OperateStatus::OS_NONE:										break;
		case OperateStatus::OS_REFRESH_LIST:	this->_RefreshListReq();	break;
		case OperateStatus::OS_EXTRA:										break;
		default:															break;
	}
	m_iCurOperateStatus = OperateStatus::OS_NONE;

	return true;
}

bool ServerList::OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS_LIST_CONFIG:
		{
			g_Logger->TryInfo(_T("ServerList - Login List Config"));
			this->_InitGameConfigList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_TYPE:
		{
			g_Logger->TryInfo(_T("ServerList - Login List Type"));
			this->_InitGameTypeList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_KIND:
		{
			g_Logger->TryInfo(_T("ServerList - Login List Kind"));
			this->_InitGameKindList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_STATION:
		{
			g_Logger->TryInfo(_T("ServerList - Login List Station"));
			this->_initGameStationList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_GAME_SERVER:
		{
			g_Logger->TryInfo(_T("ServerList - Login List GameServer"));
			this->_InitGameRoomList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_HALL_SERVER:
		{
			g_Logger->TryInfo(_T("ServerList - Login List HallServer"));
			tagGameStation* pStruct = static_cast<tagGameStation*>(pDataBuffer);
		}break;

		case SUB_LS_LIST_FINISH:
		{
			g_Logger->TryInfo(_T("ServerList - Login List Finish"));
			this->_InitGameListComplete();
		}break;

		default:
			break;
	}

	return true;
}