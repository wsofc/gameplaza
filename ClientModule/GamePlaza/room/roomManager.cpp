#include "stdafx.h"
#include "roomManager.h"
#include "downloadFrame.h"
#include "..\\hall\\bank.h"
#include "..\\data\\serverList.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

RoomManager* RoomManager::g_instance = nullptr;
RoomManager::RoomManager()
	: m_bLoginSuccess(false)
	, m_bLoginFinished(false)
	, m_pManagerRef(nullptr)
	, m_pGameItem(nullptr)
	, m_wSeasonNodeHeight(36)
	, m_pWaittingMsgBoxDlg(nullptr)
	, m_pDisconnectMsgBoxDlg(nullptr)
	, m_wCurRoomID(0)
	, m_curRoom(nullptr)
	, m_pGameServer(nullptr)
	, m_cbReConnecting(FALSE)
	, m_cbReConnectCount(0)
	, m_llReConnectTotalCount(0)
	, m_iSelfComingCount(0)
	, m_uOffNetStatus(OFFNetStatus::ONS_EXCEPTION)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	memset(m_cbBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(BYTE));
	g_Logger->TryInfo(_T("RoomManager - Create"));
}

RoomManager::~RoomManager()
{
	g_Logger->TryInfo(_T("RoomManager - Destroy"));
}

RoomManager* RoomManager::GetInstance()
{
	if (RoomManager::g_instance == nullptr)
	{
		RoomManager::g_instance = new (std::nothrow)RoomManager();
	}
	return RoomManager::g_instance;
}

void RoomManager::Release()
{
	if (RoomManager::g_instance)
	{
		RoomManager::g_instance->ExitPanel(true);
	}
	SAFE_DELETE(RoomManager::g_instance);
}

void RoomManager::_InitGameLevelConfig()
{
	g_Logger->TryInfo(_T("RoomManager - Init GameLevel Config"));

	ASSERT(m_pManagerRef != nullptr);
	if (!m_pManagerRef) return;

	bool bExist = false;
	bool bAbsolutePath = false;
	CDuiString pzFile;
	LPCTSTR szClientModuleName = m_pGameItem->GetGameKind()->szClientModuleName;
	if (szClientModuleName && _tcscmp(szClientModuleName, _T("")) != 0)
	{
		_stprintf_s(m_pzBuffer, _T("%s"), szClientModuleName);
		::PathRemoveExtension(m_pzBuffer);
		pzFile.Empty();
		pzFile += CPaintManagerUI::GetResourcePath();
		pzFile += _T("..\\");
		pzFile += _T("..\\");
		pzFile += m_pzBuffer;
		pzFile += _T("_Data");
		pzFile += _T("\\Resources\\Config\\data\\LEVEL");
		pzFile += JsonFile::g_pzTarExtension;
		memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
		::PathCanonicalize(m_pzBuffer, pzFile.GetData());
		bExist = g_GameUtil->IsFileExist(m_pzBuffer);
		bAbsolutePath = true;
	}
	if (!bExist)
	{
		pzFile.Empty();
		pzFile += _T("LEVEL");
		bAbsolutePath = false;
	}
	Json::Value vRoot;
	JsonFile* pFile = new JsonFile(pzFile, bAbsolutePath);
	bool bResult = pFile->initRootNode(vRoot);
	SAFE_DELETE(pFile);
	if (bResult)
	{
		DataManager* pDataManager = DataManager::GetInstance();

		if (vRoot["GameLevel"].isArray())
		{
			for (size_t i = 0; i < vRoot["GameLevel"].size(); ++i)
			{
				Json::Value& node = vRoot["GameLevel"][i];
				DWORD dwLevel = node["level"].asInt();
				std::string& strDesc = node["desc"].asString();
				LONGLONG lScore = node["score"].asInt64();

				pDataManager->InsertLevelConfig(dwLevel, lScore, strDesc);
			}
		}
	}

	g_Logger->TryInfo(_T("RoomManager - Init GameLevel Config Complete"));
}

void RoomManager::_InitRoomSeasonConfig()
{
	g_Logger->TryInfo(_T("RoomManager - Init RoomSeason Config"));

	ASSERT(m_pManagerRef != nullptr);
	if (!m_pManagerRef) return;

	bool bExist = false;

	Json::Value vRoot;
	JsonFile* pFile = new JsonFile(_T("ROOM"));
	bool bResult = pFile->initRootNode(vRoot);
	SAFE_DELETE(pFile);
	if (bResult)
	{
		DataManager* pDataManager = DataManager::GetInstance();
		Json::Value& SeasonsNode = vRoot["SeasonType"];
		if (SeasonsNode.isArray())
		{
			for (size_t i = 0; i < SeasonsNode.size(); ++i)
			{
				Json::Value& node = SeasonsNode[i];

				DWORD dwSeasonID = node["id"].asInt();
				std::string& strName = node["name"].asString();
				std::string& strDesc = node["desc"].asString();

				pDataManager->InsertRoomSeasonConfig(dwSeasonID, strName, strDesc);
			}
		}
	}

	g_Logger->TryInfo(_T("RoomManager - Init RoomSeason Config Complete"));
}

void RoomManager::_InitRoomListUI_1()
{
	g_Logger->TryInfo(_T("RoomManager - Init RoomList Config"));

	// 先清除列表UI
	this->_CleanRoomListUI();
	if (!m_pGameItem)
	{
		g_Logger->TryInfo(_T("RoomManager - the GameItem is NULL"));
		return;
	}

	DWORD wGameKindID = m_pGameItem->GetGameKind()->wKindID;
	ServiceData* pServerListData = ServiceData::GetInstance();
	DWORD wGameRoomCount = pServerListData->GetGameServerCount();
	CTabLayoutUI* room_panle_switch = static_cast<CTabLayoutUI*>(m_pManagerRef->FindControl(R->kTlsRoomListPanel_Switch));
	if (!room_panle_switch)
	{
		g_Logger->TryInfo(_T("RoomManager - the Result of FindControl is NULL"));
		return;
	}

	DWORD iNextTypeKey = 0;
	for (size_t i = 0; i < wGameRoomCount; ++i)
	{
		tagGameServer* pStruct = pServerListData->EnumGameServer(iNextTypeKey);
		if (wGameKindID == pStruct->wKindID)
		{
			std::vector<tagGameServer*>& vRoomArr = m_RoomListMap[pStruct->wRoomServiceType];
			vRoomArr.push_back(pStruct);
		}
	}

	// 下标为0 表示"所有房间", 其他则表示其他场次
	// 添加"所有房间"场次
	int iIndex = 0;
	DataManager* pDataManager = DataManager::GetInstance();
	this->CreatRoomOptBtn(iIndex, _T("所有房间"));

	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	_stprintf_s(m_pzBuffer, _T("room_season_all_room_%d"), iIndex);
	CTreeViewUI* pViewAllRoomRoot = this->AddSeasonRoot(room_panle_switch, m_pzBuffer);
	if (pViewAllRoomRoot)
	{
		for (const auto& it : m_RoomListMap)
		{
			// 添加场次节点
			CDuiString pzSeasonDesc = pDataManager->GetRoomSeasonDescByID(it.first);
			CTreeNodeUI* pSeasonNode = this->AddSeasonNode(pViewAllRoomRoot, m_pzBuffer, pzSeasonDesc.GetData(), m_wSeasonNodeHeight);

			// 添加具体场次项
			for (size_t i = 0; i < it.second.size(); ++i)
			{
				tagGameServer* pStruct = it.second[i];
				this->AddSeasonItem(pSeasonNode, pStruct);
			}
		}
	}
	
	// 添加其他场次
	iIndex = 0;
	for (const auto& it : m_RoomListMap)
	{
		++iIndex;

		LPCTSTR pzSeasonName = pDataManager->GetRoomSeasonNameByID(it.first);
		this->CreatRoomOptBtn(iIndex, pzSeasonName);

		// 添加TreeView视图根节点
		_stprintf_s(m_pzBuffer, _T("room_season_%d"), iIndex);
		CTreeViewUI* pViewRoot = this->AddSeasonRoot(room_panle_switch, m_pzBuffer);
		if (pViewRoot)
		{
			// 添加场次节点
			_stprintf_s(m_pzBuffer, _T("room_season_%d-node"), iIndex);
			CDuiString pzSeasonDesc = pDataManager->GetRoomSeasonDescByID(it.first);
			CTreeNodeUI* pSeasonNode = this->AddSeasonNode(pViewRoot, m_pzBuffer, pzSeasonDesc.GetData(), m_wSeasonNodeHeight);

			// 添加具体场次项
			for (size_t i = 0; i < it.second.size(); ++i)
			{
				tagGameServer* pStruct = it.second[i];
				this->AddSeasonItem(pSeasonNode, pStruct);
			}
		}
	}

	g_Logger->TryInfo(_T("RoomManager - Init RoomList Config Complete"));
}

void RoomManager::_InitRoomListUI_2()
{
	g_Logger->TryInfo(_T("RoomManager - Init RoomList_2 Config"));

	// 先清除列表UI
	this->_CleanRoomListUI();
	if (!m_pGameItem)
	{
		g_Logger->TryInfo(_T("RoomManager - the GameItem is NULL"));
		return;
	}

	DWORD wGameKindID = m_pGameItem->GetGameKind()->wKindID;
	ServiceData* pServerListData = ServiceData::GetInstance();
	CTabLayoutUI* room_panle_switch = static_cast<CTabLayoutUI*>(m_pManagerRef->FindControl(R->kTlsRoomListPanel_Switch));

	// 初始化左侧单选按钮列表(同大厅选项一样)
	do
	{
		// 枚举
		DWORD iNextTypeKey = 0;
		std::vector<tagGameType*> vGameTypes;
		for (size_t i = 0; i < pServerListData->GetGameTypeCount(); ++i)
		{
			tagGameType* pGameType = pServerListData->EnumGameType(iNextTypeKey);
			DWORD wTypeID = pGameType->wTypeID;

			DWORD iNextKindKey = 0;
			std::vector<tagGameKind*> vGameKinds;
			for (size_t j = 0; j < pServerListData->GetGameKindCount(); ++j)
			{
				tagGameKind* pGameKind = pServerListData->EnumGameKind(iNextKindKey);
				if (wTypeID == pGameKind->wTypeID)
				{
					vGameKinds.push_back(pGameKind);
				}
			}

			if (vGameKinds.size() > 0)
			{
				vGameTypes.push_back(pGameType);
			}
		}
		std::sort(vGameTypes.begin(), vGameTypes.end(), CGameItemUI::less_sort_type_id);

		// 插入左侧单选按钮
		int iIndex = 0;
		this->CreatRoomOptBtn(iIndex, _T("所有游戏"));
		for (const auto&it : vGameTypes)
		{
			this->CreatRoomOptBtn(++iIndex, it->szTypeName);
		}
	} while (0);

	// 下标为0 表示"所有房间", 其他则表示其他场次
	// 添加"所有房间"场次
	do
	{
		DWORD iNextRoomKey = 0;
		DWORD wGameRoomCount = pServerListData->GetGameServerCount();
		for (size_t i = 0; i < wGameRoomCount; ++i)
		{
			tagGameServer* pGameServer = pServerListData->EnumGameServer(iNextRoomKey);
			if (wGameKindID == pGameServer->wKindID)
			{
				std::vector<tagGameServer*>& vRoomArr = m_RoomListMap[pGameServer->wRoomServiceType];
				vRoomArr.push_back(pGameServer);
			}
		}
	} while (0);

	int iIndex = 0;
	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	_stprintf_s(m_pzBuffer, _T("room_season_all_room_%d"), 0);
	CTreeViewUI* pViewAllRoomRoot = this->AddSeasonRoot(room_panle_switch, m_pzBuffer);
	if (pViewAllRoomRoot)
	{
		// 提出来只留一个场次
		// 添加场次节点
		CDuiString pzSeasonDesc = m_pGameItem->GetGameKind()->szKindName;
		pzSeasonDesc += _T("-房间");
		CTreeNodeUI* pSeasonNode = this->AddSeasonNode(pViewAllRoomRoot, m_pzBuffer, pzSeasonDesc.GetData(), m_wSeasonNodeHeight);
		if (pSeasonNode)
		{
			for (const auto& it : m_RoomListMap)
			{
				// 添加具体场次项
				for (size_t i = 0; i < it.second.size(); ++i)
				{
					tagGameServer* pStruct = it.second[i];
					this->AddSeasonItem(pSeasonNode, pStruct);
				}
			}
		}
	}
	
	g_Logger->TryInfo(_T("RoomManager - Init RoomList_2 Config Complete"));
}

void RoomManager::InitStyle_1(CPaintManagerUI* pManager, CGameItemUI* pGameItem)
{
	ASSERT(pManager);
	ASSERT(pGameItem);
	if (!pManager || !pGameItem) return;

	// 绑定窗口绘图管理器
	m_pManagerRef = pManager;

	// 绑定游戏对象
	m_pGameItem = pGameItem;

	// 初始化等级配置
	this->_InitGameLevelConfig();

	// 初始化房间场次
	this->_InitRoomSeasonConfig();

	// 布局房间列表(风格2)
	this->RefreshRoomList(2);

	// 第一次进入房间时,打开所有折叠项
	this->SetRoomListExpand(true);
}

void RoomManager::InitStyle_2(CPaintManagerUI* pManager, CGameItemUI* pGameItem)
{
	ASSERT(pManager);
	ASSERT(pGameItem);
	if (!pManager || !pGameItem) return;

	// 绑定窗口绘图管理器
	m_pManagerRef = pManager;

	// 绑定游戏对象
	m_pGameItem = pGameItem;
}

void RoomManager::CreatRoomOptBtn(int iIndex, LPCTSTR pzText)
{
	CVerticalLayoutUI* pLayout = static_cast<CVerticalLayoutUI*>(m_pManagerRef->FindControl(R->kOptRoomListPanel_VLayout));
	if (pLayout)
	{
		DWORD dwTextcolor			= 0xFFFFFFFF;
		DWORD dwHotTextcolor		= 0xFFFFFFFF;
		DWORD dwSelectedTextcolor	= 0xFFFFFFFF;

		DWORD dwHotBkcolor			= 0xFFFFFFFF;
		DWORD dwFocusborderColor	= 0xFFFFFFFF;

		ServiceData* pServerData = ServiceData::GetInstance();
		GameFontColor* pFontColor = pServerData->GetFontColorByID(1);
		if (pFontColor)
		{
			dwTextcolor = pFontColor->dwTextcolor;
			dwHotTextcolor = pFontColor->dwHotTextcolor;
			dwSelectedTextcolor = pFontColor->dwSelectedTextcolor;

			dwHotBkcolor = pFontColor->dwHotBkcolor;
			dwFocusborderColor = pFontColor->dwFocusborderColor;
		}

		CDuiString strText = pzText != nullptr ? pzText : _T("");
		_stprintf_s(m_pzBuffer, _T("roompanel-opt_btn_%d"), iIndex);
		COptionUI* pOption = new COptionUI();
		pOption->SetName(m_pzBuffer);
		pOption->SetFixedHeight(30);
		pOption->SetGroup(_T("room_panel_opt"));
		pOption->SetText(strText);
		pOption->SetFont(4);
		pOption->SetTextColor(dwTextcolor);
		pOption->SetHotTextColor(dwHotTextcolor);
		pOption->SetSelectedTextColor(dwSelectedTextcolor);
		pOption->SetHotBkColor(dwHotBkcolor);
		pOption->SetSelectedImage(_T("file='common/button/op_btn_03_01.png' corner='20,20,20,20'"));
		pOption->SetBorderSize(1);
		pOption->SetFocusBorderColor(dwFocusborderColor);
		pOption->Selected(0 == iIndex);
		pOption->SetTabEnable(true);
		pLayout->Add(pOption);

		CControlUI* pControl = new CControlUI();
		pControl->SetFixedHeight(2);
		pControl->SetBkImage(_T("hall/split.png"));
		pLayout->Add(pControl);

		m_vOptName.push_back(m_pzBuffer);
	}
}

void RoomManager::RefreshRoomList(int iStyle /* = 0 */)
{
	switch (iStyle)
	{
		case 1:		this->_InitRoomListUI_1();		break;
		case 2:		this->_InitRoomListUI_2();		break;
		default:	this->_InitRoomListUI_2();		break;
	}
}

void RoomManager::SetRoomListExpand(bool bExpand)
{
	CTabLayoutUI* room_panle_switch = static_cast<CTabLayoutUI*>(m_pManagerRef->FindControl(R->kTlsRoomListPanel_Switch));
	if (!room_panle_switch) return;

	for (size_t i = 0; i < room_panle_switch->GetCount(); ++i)
	{
		CTreeViewUI* pTreeView = dynamic_cast<CTreeViewUI*>(room_panle_switch->GetItemAt(i));
		if (pTreeView)
		{
			pTreeView->SetItemExpand(true);
		}
	}
}

void RoomManager::PrintUserInOutRoom(UserData* pUser, bool bInOut)
{
	if (!m_pManagerRef || !pUser) return;
	//g_Logger->TryInfo(_T("RoomManager - User In Out Room [u_id = %lu, ug_id = %lu, bInOut = %d]"), pUser->GetUserID(), pUser->GetGameID(), bInOut);

	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();

	BYTE cbUserMOrder = pUser->GetMasterOrder();
	BYTE cbSelfMOrder = tSelf.GetMasterOrder();
	CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManagerRef->FindControl(R->kEdtChatPanel_View));

	if (bInOut) // 进入房间提示
	{
		if (pViewEdit && this->m_bLoginFinished && (cbSelfMOrder >= cbUserMOrder || tSelf.IsUnrealUser()))
		{
			g_GameUtil->SendUserLeaveText(pViewEdit, pUser->GetNickName(), _T("进来了"));
		}
	}
	else // 退出房间提示
	{
		if (pViewEdit && pUser->GetUserID() != tSelf.GetUserID() && (cbSelfMOrder >= cbUserMOrder || tSelf.IsUnrealUser()))
		{
			g_GameUtil->SendUserLeaveText(pViewEdit, pUser->GetNickName(), _T("离开了房间"));
		}
	}

	// 刷新房间标题人数显示
	if (m_curRoom)
	{
		int iCount = bInOut ? 1 : -1;
		m_curRoom->AddOnLineCount(iCount);
		m_curRoom->RefreshTitleLabel();
	}
}

void RoomManager::_EnterRoom()
{
	g_Logger->TryInfo(_T("RoomManager - Enter Room"));

	ASSERT(m_curRoom);
	if (!m_curRoom) return;

	m_bLoginFinished = true;
	m_uOffNetStatus = OFFNetStatus::ONS_EXCEPTION;
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& pSelf = pDataManager->GetSelfData();
	pSelf.SetRoomID(m_curRoom->GetServerID());

	// 切换界面
	g_GameUtil->SetCurPanel(m_pManagerRef, Tls_Panel::TLS_PANEL_ROOM, Tls_SideBar::TLS_SIDEBAR_ROOM);

	// 设置滚轮滚动初始值
	CTileLayoutUI* pTileLayout = static_cast<CTileLayoutUI*>(m_pManagerRef->FindControl(R->kTilRoomPanel_Panel_1));
	if (pTileLayout)
	{
		pTileLayout->SetScrollPos({ 0, 0 });	// 位置
		pTileLayout->SetScrollStepSize(100);	// 速度
	}

	// 进入房间UI完毕,关闭waitting提示
	ShowWaittingText(false);

	g_Logger->TryInfo(_T("RoomManager - Enter Room Complete [room_id = %u]"), m_curRoom->GetServerID());
}

void RoomManager::EnterRoomReq(WORD wRoomID)
{
	// 绑定房间ID
	this->m_wCurRoomID = wRoomID;

	// 加载玩家控制Dll(若加载失败,则管理员权限无效)
	bool bLoadControlDllSuccess = this->LoadUserControl(m_pManagerRef);
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& SelfData = pDataManager->GetSelfData();
	SelfData.SetMasterSwitchOn(bLoadControlDllSuccess);

	// 开始连接房间服务器
	this->CheckGameStatus();
}

void RoomManager::ExitPanel(bool bExitToHall)
{
	if (bExitToHall)
	{
		this->_ExitToHall();
	}
	else
	{
		this->_ExitRoom();
	}
}

void RoomManager::_ExitRoom()
{
	g_Logger->TryInfo(_T("RoomManager - Exit Room Normal"));

	m_cbReConnecting = FALSE;
	m_uOffNetStatus = OFFNetStatus::ONS_NORMAL;
	this->DisConnect();
	
	if (m_curRoom)
	{
		m_curRoom->ClearAll();
	}
	SAFE_DELETE(m_curRoom);
	m_wCurRoomID = 0;

	// 切换UI至房间列表面板
	if (m_pManagerRef)
	{
		g_GameUtil->SetCurPanel(m_pManagerRef, Tls_Panel::TLS_PANEL_ROOM_LIST, TLS_SIDEBAR_ROOM_LIST);

		// 清理断线重连提示Lab
		this->EndReTry();
	}

	m_bLoginFinished = false;

	g_Logger->TryInfo(_T("RoomManager - Exit Room Normal Complete"));
}

void RoomManager::_ExitToHall()
{
	g_Logger->TryInfo(_T("RoomManager - Exit To Hall"));

	this->_ExitRoom();
	this->Clean();

	// 切换UI至大厅
	if (m_pManagerRef)
	{
		g_GameUtil->SetCurPanel(m_pManagerRef, Tls_Panel::TLS_PANEL_HALL, TLS_SIDEBAR_HALL);
	}

	g_Logger->TryInfo(_T("RoomManager - Exit To Hall Complete"));
}

void RoomManager::Clean()
{
	g_Logger->TryInfo(_T("RoomManager - Clean"));

	this->_CleanRoomListUI();

	// 都是引用数据(不能delelte)
	m_pGameItem = nullptr;

	// 清除数据等
	DataManager* pDataManager = DataManager::GetInstance();
	pDataManager->Clean();

	g_Logger->TryInfo(_T("RoomManager - Clean Complete"));
}

bool RoomManager::LoadUserControl(CPaintManagerUI* pManager)
{
	g_Logger->TryInfo(_T("RoomManager - LoadUserControl"));

	bool bLoadSuccess = false;
	UserControl* pUserControl = UserControl::GetInstance();
	pUserControl->BindPaintManager(pManager);

	// 加载管理员菜单组件
	bLoadSuccess = pUserControl->IsLoadMenuDll();
	if (!bLoadSuccess)
	{
		bLoadSuccess = pUserControl->LoadUserMenuDll();
	}

	return bLoadSuccess;
}

void RoomManager::UnLoadUserControl()
{
	g_Logger->TryInfo(_T("RoomManager - UnLoadUserControl"));

	UserControl* pUserControl = UserControl::GetInstance();
	pUserControl->UnLoadUserMenuDll();
}

void RoomManager::CheckGameStatus()
{
	g_Logger->TryInfo(_T("RoomManager - CheckGameStatus"));

	CGameItemUI::GAME_STATE iGameState = m_pGameItem->GetGameStatus();
	switch (iGameState)
	{
		case DuiLib::CGameItemUI::GAME_STATE_NOT_OPEN:
		{
			DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示:"), m_pGameItem->GetGameStatusDesc());
		}break;

		case DuiLib::CGameItemUI::GAME_STATE_NOT_EXIST:
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(RoomManager::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(m_pManagerRef
				, _T("温馨提示:")
				, m_pGameItem->GetGameStatusDesc()
				, DLG_ID(RoomMsgBoxID::MSG_BOX_ID_GAME_STATUS_NONE)
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb);
		}break;

		case DuiLib::CGameItemUI::GAME_STATE_VERSION_ERROE:
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(RoomManager::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(m_pManagerRef
				, _T("温馨提示:")
				, m_pGameItem->GetGameStatusDesc()
				, DLG_ID(RoomMsgBoxID::MSG_BOX_ID_GAME_STATUS_LOW)
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb);
		}break;

		case DuiLib::CGameItemUI::GAME_STATE_VERSION_NORMAL:
		default:
		{
			this->GameServerConnect(m_wCurRoomID);
		}break;
	}

	g_Logger->TryInfo(_T("RoomManager - CheckGameStatus Complete"));
}

void RoomManager::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case RoomMsgBoxID::MSG_BOX_ID_GAME_STATUS_NONE:
		{
			this->Download();
		}break;

		case RoomMsgBoxID::MSG_BOX_ID_GAME_STATUS_LOW:
		{
			this->Download();
		}break;

		case ServerBase::ServerBaseMsxBoxID::MSG_BOX_ID_NET_DISCONNECTION:
		{
			this->ExitPanel(false);
		}break;

		default:
			break;
	}
}

void RoomManager::NotifyGameServerUpdateBank()
{
	g_Logger->TryInfo(_T("RoomManager - Notify GameServer UpdateBank Req"));

	if (m_pGameServer && m_pGameServer->IsConnect())
	{
		m_pGameServer->SendData(MDM_GR_USER, SUB_GR_USER_UPDATE_GOLD);
	}
}

void RoomManager::_CleanRoomListUI()
{
	if (m_pManagerRef)
	{
		CVerticalLayoutUI* pLayout = static_cast<CVerticalLayoutUI*>(m_pManagerRef->FindControl(R->kOptRoomListPanel_VLayout));
		if (pLayout)
		{
			pLayout->RemoveAll();
		}

		CTabLayoutUI* room_panle_switch = static_cast<CTabLayoutUI*>(m_pManagerRef->FindControl(R->kTlsRoomListPanel_Switch));
		if (room_panle_switch)
		{
			room_panle_switch->RemoveAll();
		}
	}

	// 都是引用数据(不能delelte)
	m_vOptName.clear();
	m_RoomListMap.clear();
}

void RoomManager::Notify(TNotifyUI& msg)
{
	if (TLS_PANEL_ROOM == g_GameUtil->GetCurPanel() && m_curRoom)
	{
		m_curRoom->Notify(msg);
		return;
	}

	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnRoomListPanel_Back) == 0)
		{
			this->ExitPanel(true);

			// 刷新房间列表
			ServerList* pServerList = ServerList::GetInstance();
			pServerList->RefreshList(m_pManagerRef, ServerList::RefreshType::RF_TYPE_GAMELIST);
		}
		else if (_tcscmp(sName, R->kBtnRoomListPanel_Enter) == 0)
		{
			CDuiString pzRoomID = msg.pSender->GetUserData();
			WORD wCurRoomID = _ttoi(pzRoomID.GetData());
			this->EnterRoomReq(wCurRoomID);
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_SELECTCHANGED) == 0)
	{
		for (size_t i = 0; i < m_vOptName.size(); ++i)
		{
			if (_tcscmp(sName, m_vOptName[i].GetData()) == 0)
			{
				// 风格1
				if (false)
				{
					CTabLayoutUI* room_panle_switch = static_cast<CTabLayoutUI*>(m_pManagerRef->FindControl(R->kTlsRoomListPanel_Switch));
					if (room_panle_switch)
					{
						room_panle_switch->SelectItem(i);
					}
				}
				// 风格2
				else
				{
					CTabLayoutUI* hall_panle_switch = static_cast<CTabLayoutUI*>(m_pManagerRef->FindControl(R->kTlsHallPanel_Switch));
					if (hall_panle_switch)
					{
						hall_panle_switch->SelectItem(i);
						CStdPtrArray* aOptionGroup = m_pManagerRef->GetOptionGroup(R->kStrHallPanel_OptGroupName);
						if (aOptionGroup)
						{
							COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
							if (pControl)
							{
								pControl->Selected(true);
							}
						}
					}
					this->ExitPanel(true);
				}
			}
		}
	}
}

CTreeViewUI* RoomManager::AddSeasonRoot(CTabLayoutUI* tabLayout, LPCTSTR pzName)
{
	CTreeViewUI* pTreeView = nullptr;
	if (tabLayout)
	{
		DWORD dwHotBkcolor		= 0xFFFFFFFF;
		DWORD dwSelectedBkcolor = 0xFFFFFFFF;
		ServiceData* pServiceData = ServiceData::GetInstance();
		GameFontColor* pFontColor = pServiceData->GetFontColorByID(3);
		if (pFontColor)
		{
			dwHotBkcolor = pFontColor->dwHotBkcolor;
			dwSelectedBkcolor = pFontColor->dwSelectedBkcolor;
		}

		CDuiString strName = pzName != nullptr ? pzName : _T("");
		pTreeView = new CTreeViewUI();
		pTreeView->SetName(strName);
		pTreeView->SetVisibleFolderBtn(true);
		pTreeView->EnableScrollBar(true, pTreeView->GetHorizontalScrollBar() != NULL);
		pTreeView->SetHotItemBkColor(dwHotBkcolor);
		pTreeView->SetSelectedItemBkColor(dwSelectedBkcolor);
		tabLayout->Add(pTreeView);
	}

	return pTreeView;
}

CTreeNodeUI* RoomManager::AddSeasonNode(CControlUI* pControl, LPCTSTR pzName, LPCTSTR pzText, DWORD height)
{
	CTreeViewUI* pTreeView = dynamic_cast<CTreeViewUI*>(pControl);
	if (!pTreeView) return nullptr;

	DWORD dwTextColor	= 0xFF000000;
	DWORD dwBkcolor		= 0x00FFFFFF;
	ServiceData* pServiceData = ServiceData::GetInstance();
	GameFontColor* pFontColor = pServiceData->GetFontColorByID(4);
	if (pFontColor)
	{
		dwTextColor = pFontColor->dwTextcolor;
		dwBkcolor = pFontColor->dwBkcolor;
	}

	// 创建场次
	CTreeNodeUI* pSeason = new CTreeNodeUI();
	CHorizontalLayoutUI* pHoriz = pSeason->GetTreeNodeHoriznotal();
	if (!pHoriz) return nullptr;

	// 图标
	do 
	{
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetFixedWidth(42);
		pLabel->SetBkImage(_T("file='hall/room/room_list_icon.png'"));
		pHoriz->Add(pLabel);
	} while (0);

	// 场次标题
	do
	{
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("left"));
		pLabel->SetText(pzText);
		pLabel->SetFont(3);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	} while (0);

	// 占位20
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(20);
		pHoriz->Add(pControl);
	} while (0);

	// 添加返回按钮
	do
	{
		CVerticalLayoutUI* pVLayout = new CVerticalLayoutUI();
		pVLayout->SetFixedWidth(43);

		// 纵向占位
		CControlUI* pCtl_1 = new CControlUI();
		pVLayout->Add(pCtl_1);

		CButtonUI* pBackBtn = new CButtonUI();
		pBackBtn->SetName(_T("roomlistpanel-back_btn"));
		pBackBtn->SetFixedHeight(20);
		pBackBtn->SetNormalImage(_T("hall/room/btn_back_1.png"));
		pBackBtn->SetHotImage(_T("hall/room/btn_back_2.png"));
		pBackBtn->SetPushedImage(_T("hall/room/btn_back_3.png"));
		pVLayout->Add(pBackBtn);

		// 纵向占位
		CControlUI* pCtl_2 = new CControlUI();
		pVLayout->Add(pCtl_2);

		pHoriz->Add(pVLayout);
	} while (0);

	//占位20
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(20);
		pHoriz->Add(pControl);
	} while (0);

	pTreeView->Add(pSeason);
	pSeason->SetFixedHeight(height);
	pSeason->SetName(pzName);
	pSeason->SetBkColor(dwBkcolor);
	
	//COptionUI* pOptItem = pSeason->GetItemButton();
	//if (pOptItem)
	//{
	//	pOptItem->SetText(pzText);
	//	pOptItem->SetTextColor(0xFF5175B1);
	//	pOptItem->SetHotTextColor(0xFF5175B1);
	//	pOptItem->SetSelectedTextColor(0xFF5175B1);
	//	pOptItem->SetFont(3);
	//	pOptItem->SetVisible(true);
	//}
	
	//pSeason->SetAttribute(_T("dotlineattr"),	_T("visible=\"true\" width=\"42\" bkimage=\"file='hall/room/room_list_icon.png'\""));
	//pSeason->SetAttribute(_T("itemattr"),		_T("visible=\"true\" font=\"3\" textcolor=\"#FF5175B1\""));

	//<!-- 暂时屏蔽折叠按钮,因为改变了房间布局 -->
	//pSeason->SetAttribute(_T("folderattr"),		_T("visible=\"true\" width=\"58\" normalimage=\"file='hall/room/treeview_a.png'\" hotimage=\"file='hall/room/treeview_a.png'\" selectedimage=\"file='hall/room/treeview_b.png'\" selectedhotimage=\"file='hall/room/treeview_b.png'\""));
	pSeason->SetAttribute(_T("checkboxattr"),	_T("visible=\"false\" width=\"1\""));


	//pSeason->SetAttribute(_T("height"), _T("25"));
	pSeason->SetAttribute(_T("dotlineattr"),	_T("visible=\"false\" width=\"1\""));
	pSeason->SetAttribute(_T("itemattr"),		_T("visible=\"false\" width=\"1\""));
	pSeason->SetAttribute(_T("folderattr"),		_T("visible=\"false\" width=\"1\""));
	pSeason->SetAttribute(_T("checkboxattr"),	_T("visible=\"false\" width=\"1\""));

	return pSeason;
}

CTreeNodeUI* RoomManager::AddSeasonItem(CTreeNodeUI* pSeason, tagGameServer* pRoomData)
{
	if (!pSeason || !pRoomData) return nullptr;
	
	DWORD dwTextColor = 0xFFFFFFFF;
	ServiceData* pServiceData = ServiceData::GetInstance();
	GameFontColor* pFontColor = pServiceData->GetFontColorByID(5);
	if (pFontColor)
	{
		dwTextColor = pFontColor->dwTextcolor;
	}

	// 创建node横向布局
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	CTreeNodeUI* node = new CTreeNodeUI();
	CHorizontalLayoutUI* pHoriz = node->GetTreeNodeHoriznotal();
	if (!pHoriz) return nullptr;

	//占位30
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(30);
		pHoriz->Add(pControl);
	}

	//标题Lab
	{
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetText(pRoomData->szServerName);
		pLabel->SetFixedWidth(150);
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("left"));
		pLabel->SetFont(2);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	}
	
	//入场积分Lab
	{
		if (pRoomData->lLessScore > 0 && pRoomData->lLessScore < 10000)
		{
			_stprintf_s(m_pzBuffer, _T("%lld"), pRoomData->lLessScore);
		}
		if (pRoomData->lLessScore >= 10000)
		{
			_stprintf_s(m_pzBuffer, _T("%lld万"), pRoomData->lLessScore / 10000);
		}
		else
		{
			_stprintf_s(m_pzBuffer, _T("%s"), "自由");
		}
		CDuiString pzScoreDesc = m_pzBuffer;
		_stprintf_s(m_pzBuffer, _T("%s进入"), pzScoreDesc.GetData());

		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("center"));
		pLabel->SetText(m_pzBuffer);
		pLabel->SetFont(2);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	}
	
	//场次在线人数Lab
	{
		DataManager* pDataManager = DataManager::GetInstance();
		CDuiString strDesc = pDataManager->GetOnlineDescByCount(pRoomData->dwOnlineCount);
		if (strDesc.GetLength() <= 0)
		{
			strDesc = _T("正常");
		}
		
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("center"));
		pLabel->SetText(strDesc);
		pLabel->SetFont(2);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	}

	//进入按钮
	{
		CButtonUI* pButton = new CButtonUI();
		_stprintf_s(m_pzBuffer, _T("%u"), pRoomData->wServerID);
		pButton->SetUserData(m_pzBuffer);
		pButton->SetAttribute(_T("name"),			R->kBtnRoomListPanel_Enter);
		pButton->SetAttribute(_T("width"),			_T("74"));
		pButton->SetAttribute(_T("normalimage"),	_T("file='common/button/btn_02_01.png' corner='10,0,10,0'"));
		pButton->SetAttribute(_T("hotimage"),		_T("file='common/button/btn_02_02.png' corner='10,0,10,0'"));
		pButton->SetAttribute(_T("pushedimage"),	_T("file='common/button/btn_02_03.png' corner='10,0,10,0'"));
		pButton->SetAttribute(_T("foreimage"),		_T("file='common/text/048.png' aligncenter='true'"));
		pHoriz->Add(pButton);
	}

	//占位10
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(10);
		pHoriz->Add(pControl);
	}

	//注意:要先附加到父节点,然后在设置该节点属性(否则节点属性部分无效)
	//因为Add过程中会更改属性,所以需在Add之后再重新SetAttribute(坑啊~)
	pSeason->Add(node);
	node->SetAttribute(_T("height"),		_T("25"));
	node->SetAttribute(_T("dotlineattr"),	_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("itemattr"),		_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("folderattr"),	_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("checkboxattr"),	_T("visible=\"false\" width=\"1\""));

	return node;
}

void RoomManager::MsgBoxProc(DLG_ID id, TNotifyUI& msg)
{
	RoomMsgBoxID msg_box_id = (RoomMsgBoxID)id;
	switch (msg_box_id)
	{
		// 取消登录房间
		case RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_CANCEL:
		{
			this->ShowWaittingText(false);
			this->ExitPanel(false);
		}break;

		// 房间网络断开
		case RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_DISCONNECT:
		{
			this->EndReTry();
			this->OffNetTips(0xFF);
		}break;

		default:
			break;
	}
}

void RoomManager::ShowWaittingText(bool bShow, CDuiString pzText /* = _T("") */)
{
	if (bShow)
	{
		if (!m_pWaittingMsgBoxDlg)
		{
			DialogCallBack cb = CC_CALLBACK_2(RoomManager::MsgBoxProc, this);
			m_pWaittingMsgBoxDlg = DialogFrame::ToastBox(m_pManagerRef, pzText, false, (DLG_ID)RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_CANCEL, cb);
		}
		m_pWaittingMsgBoxDlg->LayoutText(_T(""), pzText);
	}
	else
	{
		DialogFrame::CloseMsgBox(m_pWaittingMsgBoxDlg);
		m_pWaittingMsgBoxDlg = nullptr;
	}
}

void RoomManager::ShowDisconnectText(bool bShow, CDuiString pzText /* = _T("") */)
{
	if (bShow)
	{
		if (!m_pDisconnectMsgBoxDlg)
		{
			DialogCallBack cb = CC_CALLBACK_2(RoomManager::MsgBoxProc, this);
			m_pDisconnectMsgBoxDlg = DialogFrame::ToastBox(m_pManagerRef, pzText, false, (DLG_ID)RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_DISCONNECT, cb);

			if (m_pDisconnectMsgBoxDlg)
			{
				RECT rcWnd = { 0 };
				RECT rcDlg = { 0 };
				::GetWindowRect(m_pManagerRef->GetPaintWindow(), &rcWnd);

				HWND hWnd = m_pDisconnectMsgBoxDlg->GetHWND();
				::GetWindowRect(hWnd, &rcDlg);

				CControlUI* pTopBar = m_pManagerRef->FindControl(R->kCtlHallMain_TopBar);
				CControlUI* pBottomBar = m_pManagerRef->FindControl(R->kCtlHallMain_BottomBar);
				CControlUI* pSiderBar = m_pManagerRef->FindControl(R->kTlsHallMain_SiderBar);
				if (pTopBar && pBottomBar && pSiderBar)
				{
					int iLeftW = rcWnd.right - rcWnd.left - pSiderBar->GetFixedWidth();
					int iLeftH = rcWnd.bottom - rcWnd.top - pTopBar->GetFixedHeight() - pBottomBar->GetFixedHeight();

					int pos_x = rcWnd.left + (iLeftW - (rcDlg.right - rcDlg.left)) / 2;
					int pos_y = rcWnd.top + pTopBar->GetFixedHeight() + (iLeftH - (rcDlg.bottom - rcDlg.top)) / 2;

					::SetWindowPos(hWnd, NULL, pos_x, pos_y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
		}
		m_pDisconnectMsgBoxDlg->LayoutText(_T(""), pzText);
	}
	else
	{
		DialogFrame::CloseMsgBox(m_pDisconnectMsgBoxDlg);
		m_pDisconnectMsgBoxDlg = nullptr;
	}
}

// ------------------------------------------------------------------------------------------------
// 下载模块
void RoomManager::Download()
{
	g_Logger->TryInfo(_T("RoomManager - Download"));
	const tagGameKind* pGameKind = m_pGameItem->GetGameKind();
	DownloadFrame* pDownloadFrame = DownloadFrame::GetInstance();
	pDownloadFrame->CreateFrame(m_pManagerRef, pGameKind);
}

// ------------------------------------------------------------------------------------------------
// 网络模块

void RoomManager::GameServerConnect(WORD wRoomID)
{
	g_Logger->TryInfo(_T("RoomManager - Connect GameServer [room_id = %u]"), wRoomID);
	
	ServiceData* pServerListData = ServiceData::GetInstance();
	tagGameServer* pStruct = pServerListData->SearchGameServer(wRoomID);
	if (pStruct)
	{
		//连接服务器
		DWORD dwServerIP = pStruct->dwServerAddr;
		WORD wPort = pStruct->wServerPort;

		g_Logger->TryInfo(_T("RoomManager - Connect GameServer [ip = %lu, port = %u]"), dwServerIP, wPort);

		m_pGameServer = GameServer::GetInstance();
		m_pGameServer->SetDelegate(this, m_pManagerRef);
		m_pGameServer->Connect(dwServerIP, nullptr, wPort);
		
		if (m_cbReConnecting == FALSE)
		{
			ShowWaittingText(true, _T("正在进入房间,请稍等..."));
		}

		// 创建当前房间
		if (!m_curRoom)
		{
			m_curRoom = new Room(m_pManagerRef, this);
		}
		m_curRoom->InitRoomData(pStruct);
		m_curRoom->InitPropsUI();
	}
	else
	{
		_stprintf_s(m_pzBuffer, _T("房间配置错误,未找到对应的房间!(%u)"), wRoomID);
		DialogFrame::MsgBox(m_pManagerRef
			, _T("温馨提示:")
			, m_pzBuffer
			, DLG_ID(RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_FAILED)
			, DLG_STYLE::DLG_DEFAULT);
	}
}

void RoomManager::DisConnect()
{
	g_Logger->TryInfo(_T("RoomManager - DisConnect GameServer"));

	if (m_pGameServer && m_pGameServer->IsConnect())
	{
		m_pGameServer->CloseSocket();
	}
}

void RoomManager::OffNetTips(BYTE cbShutReason)
{
	if (m_bLoginSuccess)
	{
		m_bLoginSuccess = false;

		// 先强行关闭游戏
		IPC* ipc = IPC::GetInstance();
		ipc->DisConnect(false);

		// 然后弹出提示框
		_stprintf_s(m_pzBuffer, _T("%s(%u)"), _T("网络连接已断开,请重新进入房间!"), cbShutReason);
		DialogCallBack dlg_cb = CC_CALLBACK_2(RoomManager::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(m_pManagerRef
			, _T("温馨提示:")
			, m_pzBuffer
			, DLG_ID(ServerBase::ServerBaseMsxBoxID::MSG_BOX_ID_NET_DISCONNECTION)
			, DLG_STYLE::DLG_OK
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
}

void RoomManager::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	g_Logger->TryInfo(_T("RoomManager - ReConnect GameServer"));

	this->GameServerConnect(m_wCurRoomID);
}

bool RoomManager::OnConnectProc(bool bSuccess)
{
	g_Logger->TryInfo(_T("RoomManager - Connect GameServer Complete [bRes = %d]"), bSuccess);

	// 连接成功
	if (bSuccess && this->m_wCurRoomID != 0)
	{
		// 房间登录
		DataManager* pDataManager = DataManager::GetInstance();
		UserData& tSelf = pDataManager->GetSelfData();
		tagGS_LogonByUserID loginMsg;
		::ZeroMemory(&loginMsg, sizeof(loginMsg));
		loginMsg.dwPlazaVersion = VER_PLAZA_FRAME;
		loginMsg.dwProcessVersion = m_pGameItem->GetGameKind()->dwMaxVersion;
		loginMsg.dwUserID = tSelf.GetUserID();
		CMD5Encrypt::EncryptData(tSelf.GetPassword(), loginMsg.szPwd);
		
		DWORD iRes = FALSE;
		bool bAddMachineID = true;
		if (bAddMachineID)
		{
			// 追加机器码
			tagClientSerial ClientSerial;
			::ZeroMemory(&ClientSerial, sizeof(tagClientSerial));
			ClientSerial.dwSystemVer = 0;
			::StrToCharArray(ClientSerial.szComputerID, tSelf.GetMachineID());

			BYTE cbBuffer[TCP_PACKET_BUFFER_SIZE] = { 0 };
			::memcpy(cbBuffer, &loginMsg, sizeof(tagGS_LogonByUserID));
			CSendPacketHelper Packet(cbBuffer + sizeof(tagGS_LogonByUserID), sizeof(cbBuffer) - sizeof(tagGS_LogonByUserID));
			Packet.AddPacket(&ClientSerial, sizeof(ClientSerial), DTP_COMPUTER_ID);
			iRes = m_pGameServer->SendData(MAIN_GS_LOGON, SUB_GS_LOGON_BY_USER_ID, cbBuffer, sizeof(tagGS_LogonByUserID) + Packet.GetDataSize());
		}
		else
		{
			iRes = m_pGameServer->SendData(MAIN_GS_LOGON, SUB_GS_LOGON_BY_USER_ID, &loginMsg, sizeof(tagGS_LogonByUserID));
		}

		g_Logger->TryInfo(_T("RoomManager - Login Room Req [%u]"), iRes);
		if (FALSE == iRes && m_cbReConnecting == FALSE)
		{
			ShowWaittingText(false);
			DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示:"), _T("进入房间失败,请稍后再试!"));
		}
	}
	// 连接失败
	else
	{
		ShowWaittingText(false);

		if (m_cbReConnecting == FALSE)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(RoomManager::ReConnect, this);
			DialogFrame::MsgBox(m_pManagerRef
				, _T("温馨提示:")
				, _T("由于网络故障，与服务器断开连接,请稍后再试!")
				, DLG_ID(RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_FAILED)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	}

	return true;
}

bool RoomManager::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	g_Logger->TryInfo(_T("RoomManager - GameServer Close [uOffNetStatus = %d, ShutReason = %u]"), m_uOffNetStatus, cbShutReason);

	ShowWaittingText(false);
	
	switch (m_uOffNetStatus)
	{
		// 网络正常关闭
		case OFFNetStatus::ONS_NORMAL:
		{
			// 先强行关闭游戏
			IPC* ipc = IPC::GetInstance();
			ipc->DisConnect(false);

			// 若正在重连则关闭重连
			this->EndReTry();
		}break;
		
		// 网络异常关闭
		case OFFNetStatus::ONS_EXCEPTION:
		{
			// 若未登录房间 或 已超过重连最大次数,则断开连接,提示断网返回大厅
			if (!m_bLoginSuccess || m_cbReConnectCount >= RECONNECT_MAX_COUNT)
			{
				this->EndReTry();
				this->OffNetTips(cbShutReason);
			}
			else
			{
				// 开始重连检测(若未开始重连,则生效)
				this->StartReTry();
			}
		}break;

		// 网络被远程关闭
		case OFFNetStatus::ONS_INTERMIT:
		{
			this->OffNetTips(cbShutReason);
		}break;

		// 网络直接关闭
		case OFFNetStatus::ONS_FINALCLOSE:
		{
			this->OffNetTips(cbShutReason);
		}break;
		
		default:
			break;
	}

	return true;
}

void RoomManager::StartReTry()
{
	if (m_cbReConnecting == FALSE)
	{
		g_Logger->TryInfo(_T("RoomManager - Start ReConnect Req [count = %lld]"), ++m_llReConnectTotalCount);

		// 重连前 置空自己的coming计数器
		// 因为游戏状态时重连回来只会coming一次
		// 而非游戏状态时会coming两次
		// 不置空会导致需要发送两次登录才能成功(第一次被coming计数器返回了,从而定时器不能及时关闭,发送第二次登录请求)
		m_iSelfComingCount = 0;

		m_cbReConnecting = TRUE;
		TimerCallBack timepro = CC_CALLBACK_2(RoomManager::OnTimer, this);

		// 先重连一次
		this->OnRetry();

		// 然后每间隔一段时间重连一次
		g_GameUtil->SetTimer(m_pManagerRef->GetPaintWindow(), timepro, TIMER_ID_RECONNECT, RECONNECT_INTERVAL);

		// 下发至游戏开始断线重连
		CMD_Command cmd;
		memset(&cmd, 0, sizeof(CMD_Command));
		cmd.wMainCmdID = IPC_MAIN_CONCTROL;
		cmd.wSubCmdID = IPC_SUB_GF_RECONNECT;

		tagGameFrameReconnect tGReconnect;
		memset(&tGReconnect, 0, sizeof(tagGameFrameReconnect));
		tGReconnect.cbMaxCount = RECONNECT_MAX_COUNT;
		tGReconnect.cbInterval = RECONNECT_INTERVAL / 1000;
		tGReconnect.cbStatus = m_cbReConnecting;
		this->OnNetwork_TurnToGame(cmd, &tGReconnect, sizeof(tGReconnect));
	}
}

void RoomManager::OnRetry()
{
	if (m_cbReConnecting)
	{
		// 显示UI(重连次数计数)
		_stprintf_s(m_pzBuffer, _T("正在尝试第(%u/%u)次重新连接"), ++m_cbReConnectCount, RECONNECT_MAX_COUNT);
		ShowDisconnectText(true, m_pzBuffer);

		// 重连
		TNotifyUI msg;
		memset(&msg, 0, sizeof(msg));
		this->ReConnect(DLG_ID::DLG_ID_NONE, msg);
	}
	else
	{
		this->EndReTry();
	}
}

void RoomManager::EndReTry()
{
	g_Logger->TryInfo(_T("RoomManager - End ReConnect [status = %u, count = %u]"), m_cbReConnecting, m_cbReConnectCount);

	m_cbReConnectCount = 0;
	g_GameUtil->KillTimer(m_pManagerRef->GetPaintWindow(), TIMER_ID_RECONNECT);
	ShowDisconnectText(false);

	if (m_cbReConnecting == TRUE)
	{
		m_cbReConnecting = FALSE;

		// 清除重连之前的若干状态、UI等
		this->ReconnectClear();

		// 下发游戏重连结果
		g_Logger->TryInfo(_T("RoomManager - End ReConnect Send To Game Notify!"));
		CMD_Command cmd;
		memset(&cmd, 0, sizeof(CMD_Command));
		cmd.wMainCmdID = IPC_MAIN_CONCTROL;
		cmd.wSubCmdID = IPC_SUB_GF_RECONNECT;

		tagGameFrameReconnect tGReconnect;
		memset(&tGReconnect, 0, sizeof(tagGameFrameReconnect));
		tGReconnect.cbMaxCount	= RECONNECT_MAX_COUNT;
		tGReconnect.cbInterval	= RECONNECT_INTERVAL;
		tGReconnect.cbStatus	= m_cbReConnecting;
		this->OnNetwork_TurnToGame(cmd, &tGReconnect, sizeof(tGReconnect));
	}
}

void RoomManager::ReconnectClear()
{
	this->m_bLoginFinished = false;
	if (this->m_curRoom)
	{
		m_curRoom->ResetChatUI();
	}
}

void RoomManager::OnTimer(UINT_PTR idEvent, DWORD dwTime)
{
	switch (idEvent)
	{
		case TIMER_ID_RECONNECT:
		{
			this->OnRetry();
		}break;

		default:
			break;
	}
}

bool RoomManager::OnNetwork_Logon(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GS_LOGON_SUCCESS:
		{
			g_Logger->TryInfo(_T("RoomManager - Room Login OK"));

			// 登录房间成功
			m_bLoginSuccess = true;
			tagGS_RT_LogonSuccess* pStruct = static_cast<tagGS_RT_LogonSuccess*>(pDataBuffer);
			if (pStruct){}
				
			// 发送当前用户的游戏设置信息
			DataManager* pDataManager = DataManager::GetInstance();
			UserData& pSelf = pDataManager->GetSelfData();
			CMD_GR_UserRule tGameRule = pSelf.GetGameSetting();

			g_Logger->TryInfo(_T("RoomManager - User Rule Setting Req"));
			m_pGameServer->SendData(MDM_GR_USER, SUB_GR_USER_RULE, &tGameRule, sizeof(CMD_GR_UserRule));
		}break;

		case SUB_GS_LOGON_ERROR:
		{
			tagGS_RT_LogonError* pStruct = static_cast<tagGS_RT_LogonError*>(pDataBuffer);
			if (!pStruct) return true;

			g_Logger->TryInfo(_T("RoomManager - Room Login Failed [code = %ld]"), pStruct->lErrorCode);

			memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
			_stprintf_s(m_pzBuffer, _T("%s"), pStruct->szErrorDescribe);

			ShowWaittingText(false);
			DialogCallBack dlg_cb = CC_CALLBACK_2(RoomManager::ReConnect, this);
			DialogFrame::MsgBox(m_pManagerRef
				, _T("房间登录失败:")
				, m_pzBuffer
				, DLG_ID(RoomMsgBoxID::MSG_BOX_ID_LOGIN_ROOM_FAILED)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);

			// 登录失败,关闭网络连接
			this->DisConnect();
		}break;

		case SUB_GS_LOGON_FINISH:
		{
			//进入房间
			this->_EnterRoom();
		}break;

		default:
			break;
	}

	return true;
}

bool RoomManager::OnNetwork_Status(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 桌子信息
		case SUB_GS_TABLE_INFO:
		{
			tagGS_TableInfo* pStruct = static_cast<tagGS_TableInfo*>(pDataBuffer);
			if (m_curRoom && pStruct)
			{
				for (size_t i = 0; i < pStruct->wTableCount; ++i)
				{
					tagTableStatus& tableStruct = pStruct->TableStatus[i];
					m_curRoom->UpdateTableStatus(i, tableStruct);
				}
			}
		}break;

		// 桌子状态
		case SUB_GS_TABLE_STATUS:
		{
			tagGS_TableStatus* pStruct = static_cast<tagGS_TableStatus*>(pDataBuffer);
			if (m_curRoom && pStruct)
			{
				m_curRoom->UpdateTableStatus(*pStruct);
			}
		}break;

		default:
			break;
	}

	// 中转至游戏
	this->OnNetwork_TurnToGame(cmd, pDataBuffer, wDataSize);

	return true;
}

bool RoomManager::OnNetwork_HallMSG(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GS_MESSAGE:
		{
			tagGS_Message stu;
			memset(&stu, 0, sizeof(tagGS_Message));
			memcpy(&stu, pDataBuffer, wDataSize);
			this->_UpdateSystemMessage(stu);
		}break;

		default:
			break;
	}
	return true;
}

bool RoomManager::OnNetwork_ServerInfo(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GS_ONLINE_COUNT_INFO:
		{
			tagOnlineCountInfo* pStruct = static_cast<tagOnlineCountInfo*>(pDataBuffer);
			if (pStruct)
			{
				WORD wItemCount = wDataSize / sizeof(tagOnlineCountInfo);
				ServiceData* pServiceData = ServiceData::GetInstance();
				for (size_t i = 0; i < wItemCount; ++i)
				{
					tagGameKind* pGameKind = pServiceData->SearchGameKind(pStruct->wKindID);
					if (pGameKind)
					{
						pGameKind->dwOnlineCount = pStruct->dwOnlineCount;
					}
				}
			}
		}break;

		default:
			break;
	}
	return true;
}

bool RoomManager::OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GF_BUYPROPS_OK:
		{
			if (m_curRoom)
			{
				CMD_GF_PropsNotify* pStuOk = static_cast<CMD_GF_PropsNotify*>(pDataBuffer);
				if (pStuOk)
				{
					g_Logger->TryInfo(_T("RoomManager - BuyProps OK [location = %u, type = %u]"), pStuOk->cbSendLocation, pStuOk->cbPropType);

					// 通知游戏服更新银行金币等
					this->NotifyGameServerUpdateBank();

					// 插入购买道具消息提示(广播消息)
					DataManager* pDataMgr = DataManager::GetInstance();
					if (pStuOk->dwRcvGameID == pDataMgr->GetSelfData().GetGameID())
					{
						CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManagerRef->FindControl(R->kEdtChatPanel_View));
						if (pStuOk->dwSrcGameID == pStuOk->dwRcvGameID)
						{
							switch (pStuOk->cbPropType)
							{
								// 礼物
								case 0:
								{
									g_GameUtil->SendBuyPropsMsgText(pViewEdit, pStuOk);
								}break;

								// 会员卡
								case 1:
								{
									g_GameUtil->SendBuyPropsMsgText(pViewEdit, pStuOk);
								}break;

								default:
									break;
							}
						}
						else
						{
							switch (pStuOk->cbPropType)
							{
								// 礼物
								case 0:
								{
									g_GameUtil->SendGivePropsMsgText(pViewEdit, pStuOk);
								}break;

								// 会员卡
								case 1:
								{
									g_GameUtil->SendGivePropsMsgText(pViewEdit, pStuOk);
								}break;

								default:
									break;
							}
						}
					}
				}

				m_curRoom->OnNetwork_Props(cmd, pDataBuffer, wDataSize);
			}
		}break;

		case SUB_GF_BUYPROPS_ERROR:
		{
			if (m_curRoom)
			{
				m_curRoom->OnNetwork_Props(cmd, pDataBuffer, wDataSize);
			}
		}break;

		default:
			break;
	}

	return true;
}

bool RoomManager::OnNetwork_User(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GR_USER_SIT_FAILED:
		{
			_stprintf_s(m_pzBuffer, _T("%s"), _T("坐下失败!"));
			CMD_GR_SitFailed* pStruct = static_cast<CMD_GR_SitFailed*>(pDataBuffer);
			if (pStruct)
			{
				_stprintf_s(m_pzBuffer, _T("%s"), pStruct->szFailedDescribe);
			}
			DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示"), m_pzBuffer);
		}break;

		case SUB_GR_USER_COME:
		{
			bool bRes = this->_UpdateUserComing(cmd, pDataBuffer, wDataSize);
			if (!bRes)
			{
				DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示:"), _T("读取玩家信息失败!"));
			}
		}break;

		case SUB_GR_USER_STATUS:
		{
			this->_UpdateUserStatus(cmd, pDataBuffer, wDataSize);
		}break;

		case SUB_GR_USER_SCORE:
		{
			this->_UpdateUserScore(cmd, pDataBuffer, wDataSize);
		}break;

		case SUB_GR_USER_RIGHT:
		{
			this->_UpdateUserRight(pDataBuffer, wDataSize);
		}break;

		case SUB_GR_USER_MEMBER_ORDER:
		{
			this->_UpdateUserMember(pDataBuffer, wDataSize);
		}break;

		case SUB_GR_USER_CHAT:
		{
			CMD_GR_UserChat* pStruct = static_cast<CMD_GR_UserChat*>(pDataBuffer);
			if (pStruct)
			{
				WORD wSizeCount = sizeof(CMD_GR_UserChat) - sizeof(pStruct->szChatMessage) + pStruct->wChatLength;
				WORD wItemCount = wDataSize / wSizeCount;
				for (size_t i = 0; i < wItemCount; ++i)
				{
					this->_UpdateUserMessage(*pStruct++);
				}
			}
		}break;

		case SUB_GR_USER_WISPER:
		{
		}break;

		case SUB_GR_USER_RULE:
		{
		}break;

		case SUB_GR_USER_INVITE:
		{
		}break;

		default:
			break;
	}
	return true;
}

bool RoomManager::OnNetwork_Info(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 房间配置
		case SUB_GR_SERVER_INFO:
		{
			CMD_GR_ServerInfo* pStruct = static_cast<CMD_GR_ServerInfo*>(pDataBuffer);
			if (m_curRoom && pStruct)
			{
				m_curRoom->InitRoomData(pStruct);
				m_curRoom->CreatTables();
			}
		}break;

		// 会员配置
		case SUB_GR_MEMBER_INFO:
		{
		}break;

		// 列表配置
		case SUB_GR_COLUMN_INFO:
		{
			CMD_GR_ColumnInfo* pStruct = static_cast<CMD_GR_ColumnInfo*>(pDataBuffer);
			if (pStruct)
			{
				// 清除玩家列表UI
				UserList* pUserList = UserList::GetInstance();
				pUserList->Clean();

				// 清除玩家列表数据
				if (this->m_curRoom)
				{
					this->m_curRoom->ClearUserDataMap();
				}

				pUserList->InsertSelfDefineHeadItem();
				for (size_t i = 0; i < pStruct->wColumnCount; ++i)
				{
					tagColumnItem& pItem = pStruct->ColumnItem[i];
					pUserList->InsertHeadItem(&pItem);
				}

				//加载玩家列表控件UI
				CListUI* pListUI = static_cast<CListUI*>(m_pManagerRef->FindControl(R->kLstChatPanel_UserList));
				pUserList->LoadHeaderItem(pListUI);
			}
		}break;

		// 配置完成
		case SUB_GR_CONFIG_FINISH:
		{
			g_Logger->TryInfo(_T("RoomManager - Info Config Complete"));
		}break;

		default:
			break;
	}
	return true;
}

bool RoomManager::OnNetwork_Manager(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 其他设置
		case SUB_GR_GMCONTROL:
		{
			UserControl* pUserControl = UserControl::GetInstance();
			pUserControl->OnGMControData(pDataBuffer, wDataSize);
		}break;

		// 房间设置
		case SUB_GR_OPTION_SERVER:
		{
			UserControl* pUserControl = UserControl::GetInstance();
			pUserControl->OnGMRoomData(pDataBuffer, wDataSize);
		}break;

		default:
			break;
	}

	return true;
}

bool RoomManager::OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	IPC* ipc = IPC::GetInstance();
	ipc->LocalSendData(cmd, pDataBuffer, wDataSize);
	
	// 发给游戏的消息不能控制,更不能采纳其操作结果
	// 此处必须返回true
	return true;
}

bool RoomManager::OnNetwork_Game(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	IPC* ipc = IPC::GetInstance();
	ipc->LocalSendData(cmd, pDataBuffer, wDataSize);

	// 发给游戏的消息不能控制,更不能采纳其操作结果
	// 此处必须返回true
	return true;
}

bool RoomManager::OnNetwork_GameFrame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	IPC* ipc = IPC::GetInstance();
	ipc->LocalSendData(cmd, pDataBuffer, wDataSize);

	// 发给游戏的消息不能控制,更不能采纳其操作结果
	// 此处必须返回true
	return true;
}

DWORD RoomManager::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	DWORD bRes = FALSE;
	if (m_pGameServer)
	{
		bRes = m_pGameServer->SendData(wMainCmdID, wSubCmdID);
	}
	else
	{
		DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示"), _T("未连接到网络,请重新登录!"));
	}

	return bRes;
}

DWORD RoomManager::SendData(WORD wMainCmdID, WORD wSubCmdID, void* const pData, WORD wDataSize)
{
	DWORD bRes = FALSE;
	if (m_pGameServer)
	{
		bRes = m_pGameServer->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
	}
	else
	{
		DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示"), _T("未连接到网络,请重新登录!"));
	}

	return bRes;
}

bool RoomManager::OnNetwork_GameToServer(void* pData)
{
	COPYDATASTRUCT *cpdStruct = static_cast<COPYDATASTRUCT*>(pData);
	if (!cpdStruct) return false;

	IPC_Buffer* pIPC_Buffer = static_cast<IPC_Buffer*>(cpdStruct->lpData);
	if (!pIPC_Buffer) return false;

	WORD wVersion = pIPC_Buffer->Head.wVersion;
	WORD wDataSize = pIPC_Buffer->Head.wDataSize;
	WORD wMainCmdID = pIPC_Buffer->Head.wMainCmdID;
	WORD wSubCmdID = pIPC_Buffer->Head.wSubCmdID;
	void* pDataBuffer = pIPC_Buffer->cbBuffer;

	ASSERT(cpdStruct->cbData == IPC_BUFFER);
	if (cpdStruct->cbData != IPC_BUFFER) return false;

	switch (cpdStruct->dwData)
	{
		case IPC_FLAG_TO_SERVER:
		{
			m_pGameServer->SendData(wMainCmdID, wSubCmdID, pDataBuffer, wDataSize);

			// Unreal Check
			if (wMainCmdID == MDM_GF_BANK && wSubCmdID == SUB_GF_BANK_WITHDRAW)
			{
				CMD_GF_BankAction* pBankAct = static_cast<CMD_GF_BankAction*>(pDataBuffer);
				if (pBankAct)
				{
					UnrealControl* pUnrealControl = UnrealControl::GetInstance();
					pUnrealControl->CheckUnreal_User(UnrealNSP::UnrealChargeType::UCT_TRANSFER, pBankAct->lScore, pBankAct->szPassword, LOCATION_GAME_ROOM);
				}
			}
		}break;

		case IPC_FLAG_TO_GAMEPLAZA:
		{
			IPC* ipc = IPC::GetInstance();

			CMD_Command cmd;
			cmd.wMainCmdID = wMainCmdID;
			cmd.wSubCmdID = wSubCmdID;
			ipc->LocalRecvData(cmd, pDataBuffer, wDataSize);
		}break;

		case IPC_FLAG_TO_GAME:break;
		case IPC_FLAG_TO_WINFROM:break;
		default:break;
	}

	return true;
}

bool RoomManager::OnNetwork_TurnToGame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	IPC* ipc = IPC::GetInstance();
	ipc->LocalSendData(cmd, pDataBuffer, wDataSize);

	// 发给游戏的消息不能控制,更不能采纳其操作结果
	// 此处必须返回true
	return true;
}

void RoomManager::_UpdateSystemMessage(tagGS_Message& msg)
{
	g_Logger->TryInfo(_T("RoomManager - Update System Msg [msg_type = %u]"), msg.wMessageType);
	
	// 提示消息
	if (SMT_INFO == (msg.wMessageType & SMT_INFO))
	{
		CDuiString pzText;
		TCHAR pzTemp[MAX_SYSMESSAGE_LEN] = { 0 };
		::StrToCharArray(pzTemp, msg.szContent);
		pzText = pzTemp;
		CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManagerRef->FindControl(R->kEdtChatPanel_View));
		g_GameUtil->SendSystemMsgText(pViewEdit, pzText);
	}

	// 全局消息
	if (SMT_GLOBAL == (msg.wMessageType & SMT_GLOBAL))
	{
		// 聊天框打印
		do
		{
			CDuiString pzText;
			TCHAR pzTemp[MAX_SYSMESSAGE_LEN] = { 0 };
			::StrToCharArray(pzTemp, msg.szContent);
			pzText = pzTemp;
			CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManagerRef->FindControl(R->kEdtChatPanel_View));
			if (pViewEdit)
			{
				g_GameUtil->SendSystemMsgText(pViewEdit, pzText);
			}
		} while (0);

		// 跑马灯打印
		do
		{
			CLabelUI* pRollText = static_cast<CLabelUI*>(m_pManagerRef->FindControl(R->kLabHallMain_RollText));
			if (pRollText)
			{
				CDuiString pzText;
				TCHAR pzTemp[MAX_SYSMESSAGE_LEN] = { 0 };
				::StrToCharArray(pzTemp, msg.szContent);
				pzText = pzTemp;
				pRollText->SetText(pzText);
			}
		} while (0);
	}

	// 弹框消息
	if (SMT_EJECT == (msg.wMessageType & SMT_EJECT))
	{
		g_Logger->TryInfo(_T("RoomManager - Update System Msg SMT_EJECT"));

		CDuiString pzText;
		TCHAR pzTemp[MAX_SYSMESSAGE_LEN] = { 0 };
		::StrToCharArray(pzTemp, msg.szContent);
		pzText = pzTemp;
		DialogFrame::MsgBox(m_pManagerRef, _T("温馨提示"), pzText);
	}

	// 关闭房间
	if (SMT_CLOSE_ROOM == (msg.wMessageType & SMT_CLOSE_ROOM))
	{
		g_Logger->TryInfo(_T("RoomManager - Update System Msg SMT_CLOSE_ROOM"));

		this->ExitPanel(false);
	}

	// 中断连接
	if (SMT_INTERMIT_LINE == (msg.wMessageType & SMT_INTERMIT_LINE))
	{
		g_Logger->TryInfo(_T("RoomManager - Update System Msg SMT_INTERMIT_LINE"));
		m_uOffNetStatus = OFFNetStatus::ONS_INTERMIT;
		this->DisConnect();
	}

	g_Logger->TryInfo(_T("RoomManager - Update System Msg Complete [msg_type = %u]"), msg.wMessageType);
}

void RoomManager::_UpdateUserMessage(CMD_GR_UserChat& msg)
{
	g_Logger->TryInfo(_T("RoomManager - Update User Chat Msg"));

	CDuiString pzText(msg.szChatMessage, msg.wChatLength);
	CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManagerRef->FindControl(R->kEdtChatPanel_View));

	DataManager* pDataManager = DataManager::GetInstance();
	UserData* pUser = pDataManager->GetUserDataByID(msg.dwSendUserID);
	if (pUser)
	{
		g_GameUtil->SendChatMsgText(pViewEdit, *pUser, pzText);
	}

	g_Logger->TryInfo(_T("RoomManager - Update User Chat Msg Complete"));
}

bool RoomManager::_UpdateUserComing(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	//g_Logger->TryDebug(_T("RoomManager - User Coming Start"));

	tagUserInfoHead * pUserInfoHead = static_cast<tagUserInfoHead*>(pDataBuffer);
	if (!pUserInfoHead) return false;
	
	// 是否是刚进来的用户(断线重连时检测用到)
	bool bJustComming = false;
	DataManager* pDataManager = DataManager::GetInstance();
	bJustComming = !pDataManager->GetUserDataByID(pUserInfoHead->dwUserID);

	//创建用户(若已存在则直接获取)
	UserData* pUserData = pDataManager->CreatUserDataByID(pUserInfoHead->dwUserID);
	if (!pUserData) return false;

	// 会广播自己两次,这里只更新一次,第二次跳过
	if (pUserData->IsSelf() && ++m_iSelfComingCount > 1)
	{
		m_iSelfComingCount = 0;
		return true;
	}
	
	//读取信息
	tagUserData udStruct;
	memset(&udStruct, 0, sizeof(udStruct));

	udStruct.dwUserID			= pUserInfoHead->dwUserID;
	udStruct.wTableID			= pUserInfoHead->wTableID;
	udStruct.wChairID			= pUserInfoHead->wChairID;
	udStruct.cbUserStatus		= pUserInfoHead->cbUserStatus;
	udStruct.dwUserRight		= pUserInfoHead->dwUserRight;
	udStruct.dwMasterRight		= pUserInfoHead->dwMasterRight;
	udStruct.bRobot				= pUserInfoHead->bRobot;
	udStruct.wFaceID			= pUserInfoHead->wFaceID;
	udStruct.dwCustomFaceVer	= pUserInfoHead->dwCustomFaceVer;
	udStruct.cbGender			= pUserInfoHead->cbGender;
	udStruct.cbMemberOrder		= pUserInfoHead->cbMemberOrder;
	udStruct.cbMasterOrder		= pUserInfoHead->cbMasterOrder;
	udStruct.dwGameID			= pUserInfoHead->dwGameID;
	udStruct.dwGroupID			= pUserInfoHead->dwGroupID;
	udStruct.lCharm				= pUserInfoHead->lCharm;
	udStruct.lScore				= pUserInfoHead->UserScoreInfo.lScore;
	udStruct.lGameGold			= pUserInfoHead->UserScoreInfo.lGameGold;
	udStruct.lInsureScore		= pUserInfoHead->UserScoreInfo.lBankScore;
	udStruct.lWinCount			= pUserInfoHead->UserScoreInfo.lWinCount;
	udStruct.lLostCount			= pUserInfoHead->UserScoreInfo.lLostCount;
	udStruct.lDrawCount			= pUserInfoHead->UserScoreInfo.lDrawCount;
	udStruct.lFleeCount			= pUserInfoHead->UserScoreInfo.lFleeCount;
	udStruct.dwExperience		= pUserInfoHead->UserScoreInfo.dwExperience;
	
	//读取扩展信息
	void * pTempBuffer = NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead + 1, wDataSize - sizeof(tagUserInfoHead));
	while (true)
	{
		pTempBuffer = RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe == DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
			case DTP_NICKNAME:		//用户昵称
			{
				ASSERT(pTempBuffer != NULL);
				ASSERT(DataDescribe.wDataSize <= sizeof(udStruct.szNickname));
				if (DataDescribe.wDataSize <= sizeof(udStruct.szNickname))
				{
					CopyMemory(&udStruct.szNickname, pTempBuffer, DataDescribe.wDataSize);
					udStruct.szNickname[ArrayCount(udStruct.szNickname) - 1] = 0;
				}

				break;
			}
			case DTP_UNDER_WRITE:		//个性签名
			{
				ASSERT(pTempBuffer != NULL);
				ASSERT(DataDescribe.wDataSize <= sizeof(udStruct.szUnderWrite));
				if (DataDescribe.wDataSize <= sizeof(udStruct.szUnderWrite))
				{
					CopyMemory(&udStruct.szUnderWrite, pTempBuffer, DataDescribe.wDataSize);
					udStruct.szUnderWrite[ArrayCount(udStruct.szUnderWrite) - 1] = 0;
				}

				break;
			}
			case DTP_USER_GROUP_NAME:	//用户社团
			{
				ASSERT(pTempBuffer != NULL);
				ASSERT(DataDescribe.wDataSize <= sizeof(udStruct.szGroupName));
				if (DataDescribe.wDataSize <= sizeof(udStruct.szGroupName))
				{
					CopyMemory(&udStruct.szGroupName, pTempBuffer, DataDescribe.wDataSize);
					udStruct.szGroupName[ArrayCount(udStruct.szGroupName) - 1] = 0;
				}

				break;
			}
		}
	}

	//填充数据结构
	pUserData->SetUserID(udStruct.dwUserID);
	pUserData->SetTableID(udStruct.wTableID);
	pUserData->SetChairID(udStruct.wChairID);
	pUserData->SetUserStatus(udStruct.cbUserStatus);
	pUserData->SetLastUserStatus(udStruct.cbUserStatus);
	pUserData->SetUserRight(udStruct.dwUserRight);
	pUserData->SetMasterRight(udStruct.dwMasterRight);
	pUserData->SetRobot(udStruct.bRobot);

	pUserData->SetFaceID(udStruct.wFaceID);
	pUserData->SetCustomFaceVer(udStruct.dwCustomFaceVer);
	pUserData->SetGender(udStruct.cbGender);
	pUserData->SetMemberOrder(udStruct.cbMemberOrder);
	pUserData->SetMasterOrder(udStruct.cbMasterOrder);
	pUserData->SetGameID(udStruct.dwGameID);
	pUserData->SetGroupID(udStruct.dwGroupID);
	pUserData->SetCharm(udStruct.lCharm);
	pUserData->SetScore(udStruct.lScore);
	pUserData->SetGameGold(udStruct.lGameGold);
	pUserData->SetInsureScore(udStruct.lInsureScore);
	pUserData->SetWinCount(udStruct.lWinCount);
	pUserData->SetLostCount(udStruct.lLostCount);
	pUserData->SetDrawCount(udStruct.lDrawCount);
	pUserData->SetFleeCount(udStruct.lFleeCount);
	pUserData->SetExperience(udStruct.dwExperience);

	pUserData->SetNickName(udStruct.szNickname);
	pUserData->SetUnderWrite(udStruct.szUnderWrite);
	pUserData->SetGroupName(udStruct.szGroupName);
	
	// Init Unreal
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	pUnrealControl->ParseUnreal(pUserData);
	
	// 添加数据到列表
	UserData& pSelfData = pDataManager->GetSelfData();
	UserList* pUserList = UserList::GetInstance();
	pUserList->InsertBodyItem(25, pUserData, &pSelfData);
	
	// 更新大厅主面板玩家UI
	if (pUserData->GetUserID() == pSelfData.GetUserID())
	{
		pSelfData.UpdateUI(m_pManagerRef);
	}
	else
	{
		BYTE cbUserMOrder = pUserData->GetMasterOrder();
		BYTE cbSelfMOrder = pSelfData.GetMasterOrder();
		
		// 管理员权限下机器人昵称加入前缀
		if (pUserData->IsRobot())
		{
			if ((cbSelfMOrder > 0 || pSelfData.IsUnrealUser()))
			{
				CDuiString pzNickName;
				pzNickName.Format(_T("%s%s"), pUserData->GetRobotPrefix(), udStruct.szNickname);
				pUserData->SetNickName(pzNickName);
			}
		}
		// 管理员分等级查看
		else
		{
			if (cbUserMOrder > 0 && (cbSelfMOrder > cbUserMOrder || pSelfData.IsUnrealUser()))
			{
				CDuiString pzNickName;
				pzNickName.Format(_T("<%s%u>%s"), _T("M"), cbUserMOrder, udStruct.szNickname);
				pUserData->SetNickName(pzNickName);
			}
		}

		//发送进入消息至聊天框
		if (bJustComming)
		{
			this->PrintUserInOutRoom(pUserData, true);
		}
	}

	// 筛选断线重连玩家进入的状态等
	do
	{
		// 特殊处理自己断线重连
		if (pUserData->IsSelf())
		{
			IPC* ipc = IPC::GetInstance();

			// 若是游戏状态
			if (pUserData->GetUserStatus() == US_PLAY)
			{
				this->EndReTry();

				// 若游戏窗口已关闭则,则上行服务器离开游戏
				if (!ipc->IsGameStarted())
				{
					this->SendData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ);
				}
			}
			// 若非游戏状态(重连时需要模拟进入房间、找位置、入座)
			else
			{
				bool bSuccess = false;
				if (ipc->IsGameStarted())
				{
					// 若游戏窗口已存在且为百人游戏布局类型(则自动模拟查找入座)
					if (m_curRoom && m_curRoom->IsHundredLayout())
					{
						TableManager* pTableManager = m_curRoom->GetTableManager();
						Table* pTable = pTableManager->GetFreeTable(false);
						if (!pTable)pTable = pTableManager->GetFreeTable(true);
						if (pTable)
						{
							WORD wChairIndex = pTable->GetFreeChairIndex();
							if (wChairIndex != INVALID_CHAIR)
							{
								bSuccess = true;
								pTable->SitDownReq(wChairIndex);
							}
						}
					}
				}
				else
				{
					this->EndReTry();
					this->SendData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ);
				}

				// 若检测失败,则直接关闭游戏窗口,同时上行服务器离开游戏
				if (!bSuccess)
				{
					ipc->DisConnect(false);
					this->SendData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ);
				}
			}
		}
	
		// 筛选同桌(且有效桌)玩家下发至游戏
		if (pUserData->IsSelf() || (pUserData->GetTableID() == pSelfData.GetTableID() && pUserData->GetTableID() != INVALID_TABLE))
		{
			CMD_Command tempCMD;
			tempCMD.wMainCmdID = IPC_MAIN_USER;
			tempCMD.wSubCmdID = IPC_SUB_USER_COME;

			UserStruct& uStruct = pUserData->GetUserStruct();
			memset(m_cbBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(BYTE));
			::memcpy(m_cbBuffer, &uStruct, sizeof(UserStruct));

			// 附加用户结构
			UserStructSuffix uStructSuffix = pUserData->GetUserStructSuffix();
			CSendPacketHelper Packet(m_cbBuffer + sizeof(UserStruct), sizeof(m_cbBuffer) - sizeof(UserStruct));
			Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);
			
			this->OnNetwork_TurnToGame(tempCMD, m_cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
		}
	} while (0);

	// 更新UI、下发游戏数据等
	this->_UpdateUserStatusUI(cmd, *pUserData);

	//g_Logger->TryDebug(_T("RoomManager - User Coming Complete [user_id = %lu, user_gameid = %lu]"), pUserData->GetUserID(), pUserData->GetGameID());

	return true;
}

void RoomManager::_UpdateUserStatus(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	CMD_GR_UserStatus* pStruct = static_cast<CMD_GR_UserStatus*>(pDataBuffer);
	if (pStruct)
	{
		WORD wItemCount = wDataSize / sizeof(CMD_GR_UserStatus);
		for (size_t i = 0; i < wItemCount; ++i, ++pStruct)
		{
			DataManager* pDataManager = DataManager::GetInstance();
			UserData* pUserData = pDataManager->GetUserDataByID(pStruct->dwUserID);
			if (pUserData)
			{
				pUserData->SetUserStatus(pStruct->cbUserStatus);
				pUserData->SetTableID(pStruct->wTableID);
				pUserData->SetChairID(pStruct->wChairID);

				this->_UpdateUserStatusUI(cmd, *pUserData);
			}
		}
	}
}

void RoomManager::_UpdateUserScore(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	CMD_GR_UserScore* pStruct = static_cast<CMD_GR_UserScore*>(pDataBuffer);
	if (pStruct)
	{
		WORD wItemCount = wDataSize / sizeof(CMD_GR_UserScore);
		for (size_t i = 0; i < wItemCount; ++i, ++pStruct)
		{
			DataManager* pDataManager = DataManager::GetInstance();
			UserData* pUserData = pDataManager->GetUserDataByID(pStruct->dwUserID);
			if (pUserData)
			{
				//魅力值
				pUserData->SetCharm(pStruct->lCharm);

				//积分信息
				tagUserScore& tagStruct = pStruct->UserScore;
				pUserData->SetScore(tagStruct.lScore);
				pUserData->SetGameGold(tagStruct.lGameGold);
				pUserData->SetBankScore(tagStruct.lBankScore);
				pUserData->SetWinCount(tagStruct.lWinCount);
				pUserData->SetLostCount(tagStruct.lLostCount);
				pUserData->SetDrawCount(tagStruct.lDrawCount);
				pUserData->SetFleeCount(tagStruct.lFleeCount);
				pUserData->SetExperience(tagStruct.dwExperience);
				
				// 更新玩家自身UI
				if (pUserData->IsSelf())
				{
					pUserData->UpdateUI(m_pManagerRef);

					// 如果打开了银行,则更新银行数据等
					if (BankFrame::IsInstanceExist())
					{
						BankFrame* pBankFrame = BankFrame::GetInstance();
						pBankFrame->UpdateBankPanel();
					}
				}

				// 更新用户列表UI
				UserList* pUserlist = UserList::GetInstance();
				pUserlist->Update();

				// 同桌的前提下才下发到游戏
				UserData& pSelfData = pDataManager->GetSelfData();
				if (pUserData->GetTableID() == pSelfData.GetTableID())
				{
					memset(m_cbBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(BYTE));
					UserStruct& uStruct = pUserData->GetUserStruct();
					::memcpy(m_cbBuffer, &uStruct, sizeof(UserStruct));

					// 附加用户结构
					UserStructSuffix uStructSuffix = pUserData->GetUserStructSuffix();
					CSendPacketHelper Packet(m_cbBuffer + sizeof(UserStruct), sizeof(m_cbBuffer) - sizeof(UserStruct));
					Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);

					this->OnNetwork_TurnToGame(cmd, m_cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
				}
			}
		}
	}
}

void RoomManager::_UpdateUserRight(void* pDataBuffer, WORD wDataSize)
{
	CMD_GR_UserRight* pStruct = static_cast<CMD_GR_UserRight*>(pDataBuffer);
	if (pStruct)
	{
		g_Logger->TryInfo(_T("RoomManager - Update User Right [uid = %lu, uright = %lu]"), pStruct->dwUserID, pStruct->dwUserRight);
		WORD wItemCount = wDataSize / sizeof(CMD_GR_UserScore);
		for (size_t i = 0; i < wItemCount; ++i, ++pStruct)
		{
			DataManager* pDataManager = DataManager::GetInstance();
			UserData* pUserData = pDataManager->GetUserDataByID(pStruct->dwUserID);
			if (pUserData)
			{
				pUserData->SetUserRight(pStruct->dwUserRight);
			}
		}
	}
}

void RoomManager::_UpdateUserMember(void* pDataBuffer, WORD wDataSize)
{
	CMD_GR_MemberOrder* pStruct = static_cast<CMD_GR_MemberOrder*>(pDataBuffer);
	if (pStruct)
	{
		g_Logger->TryInfo(_T("RoomManager - Update User Member [uid = %lu, morder = %u]"), pStruct->dwUserID, pStruct->cbMemberOrder);
		WORD wItemCount = wDataSize / sizeof(CMD_GR_UserScore);
		for (size_t i = 0; i < wItemCount; ++i, ++pStruct)
		{
			DataManager* pDataManager = DataManager::GetInstance();
			UserData* pUserData = pDataManager->GetUserDataByID(pStruct->dwUserID);
			if (pUserData)
			{
				pUserData->SetMemberOrder(pStruct->cbMemberOrder);
			}
		}
	}
}

void RoomManager::_UpdateUserStatusUI(const CMD_Command& cmd, UserData& stuTagUser)
{
	//g_Logger->TryDebug(_T("RoomManager - Update User Status +++++++++++++++++++++++++++++"));
	//g_Logger->TryDebug(_T("RoomManager - Update User Status [uid = %lu, ugid = %lu, us = %u, tid = %u, cid = %u, ltid = %u, lcid = %u]")
	//	, stuTagUser.GetUserID(), stuTagUser.GetGameID(), stuTagUser.GetUserStatus(), stuTagUser.GetTableID(), stuTagUser.GetChairID()
	//	, stuTagUser.GetLastUserStatus(), stuTagUser.GetLastChairID());
	
	bool bSendToGame = true;
	UserData& tSelf = DataManager::GetInstance()->GetSelfData();

	const BYTE cbUserStatus = stuTagUser.GetUserStatus();
	switch (cbUserStatus)
	{
		case US_NULL:		//没有状态(用户离开房间)
		{
			//clean unreal
			UnrealControl* pUnrealControl = UnrealControl::GetInstance();
			pUnrealControl->CleanUnreal(&stuTagUser);

			//发送离开消息至聊天框
			this->PrintUserInOutRoom(&stuTagUser, false);

			// 先把该消息下发到游戏中，然后在依据状态处理析构玩家数据
		}break;

		case US_READY:		//同意状态
		{
			if (m_curRoom)
			{
				m_curRoom->UpdateChairsStatus(stuTagUser);
			}
		}break;

		case US_PLAY:		//游戏状态
		{
			if (m_curRoom)
			{
				m_curRoom->UpdateChairsStatus(stuTagUser);
			}

			// 若是从离线状态恢复的,则移除离线列表
			if (stuTagUser.IsSelf() == FALSE)
			{
				UserList* pUserlist = UserList::GetInstance();
				pUserlist->RecoverOffLineMap(stuTagUser.GetUserID());
			}
		}break;

		case US_LOOKON:		//旁观状态
			break;

		case US_OFFLINE:	//断线状态
		{
			// 更新椅子状态
			if (m_curRoom)
			{
				m_curRoom->UpdateChairsStatus(stuTagUser);
			}

			// 添加至离线列表
			if (stuTagUser.IsSelf() == FALSE)
			{
				UserList* pUserlist = UserList::GetInstance();
				pUserlist->AddToOffLineMap(stuTagUser.GetUserID());
			}
		}break;

		case US_FREE:		//站立状态
		{
			// 更新椅子状态
			if (m_curRoom)
			{
				m_curRoom->UpdateChairsStatus(stuTagUser);
			}

			// 如果服务器强踢离开桌子,则断开链接
			bool bRes = true;
			bRes &= cmd.wMainCmdID == MDM_GR_USER;				// 主消息为用户消息
			bRes &= cmd.wSubCmdID == SUB_GR_USER_STATUS;		// 子消息为用户状态消息
			bRes &= stuTagUser.IsSelf();						// 且为自己
			if (bRes)
			{
				// 游戏窗口已存在,且自己不是重连状态,则关闭游戏窗口
				IPC* ipc = IPC::GetInstance();
				if (ipc->IsGameStarted() && m_cbReConnecting == FALSE)
				{
					ipc->DisConnect();
				}
			}

			// 下发到游戏中
			bool bLeaveGame1 = false;
			bool bLeaveGame2 = false;

			bLeaveGame1 = stuTagUser.IsSelf();
			bLeaveGame1 &= stuTagUser.GetLastTableID() != INVALID_TABLE;

			bLeaveGame2 = stuTagUser.GetLastTableID() == tSelf.GetTableID();
			bLeaveGame2 &= stuTagUser.GetLastTableID() != INVALID_TABLE;

			if (bLeaveGame1 || bLeaveGame2)
			{
				UserStruct& uStruct = stuTagUser.GetUserStruct();
				uStruct.dwTableID = stuTagUser.GetLastTableID();
				uStruct.dwChairID = stuTagUser.GetLastChairID();
				
				memset(m_cbBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(BYTE));
				::memcpy(m_cbBuffer, &uStruct, sizeof(UserStruct));
				
				// 附加用户结构
				UserStructSuffix uStructSuffix = stuTagUser.GetUserStructSuffix();
				CSendPacketHelper Packet(m_cbBuffer + sizeof(UserStruct), sizeof(m_cbBuffer) - sizeof(UserStruct));
				Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);

				this->OnNetwork_TurnToGame(cmd, m_cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
			}

			// 清除桌子、椅子ID引用等
			stuTagUser.SetLastTableID(INVALID_TABLE);
			stuTagUser.SetLastChairID(INVALID_CHAIR);

			bSendToGame = false;
		}break;

		case US_SIT:		//坐下状态
		{
			if (stuTagUser.IsSelf())
			{
				// 若为自己，则启动游戏
				bool bRes1 = stuTagUser.GetChairID() == INVALID_TABLE;
				bool bRes2 = stuTagUser.GetChairID() != INVALID_TABLE && stuTagUser.GetChairID() != stuTagUser.GetLastChairID();
				if (bRes1 || bRes2)
				{
					bSendToGame = false;

					// 若游戏已启动,且自己是重连状态,则发送重连消息
					IPC* ipc = IPC::GetInstance();
					if (ipc->IsGameStarted() && m_cbReConnecting == TRUE)
					{
						this->EndReTry();
					}
					// 若游戏未启动,则启动游戏
					else
					{
						m_curRoom->StartGames();
					}
				}
			}
			else
			{
				// 筛选同桌(且有效桌)玩家下发至游戏
				bool bRes1 = true;
				bRes1 &= stuTagUser.GetTableID() == tSelf.GetTableID();
				bRes1 &= stuTagUser.GetTableID() != INVALID_TABLE;

				bool bRes2 = stuTagUser.GetTableID() != stuTagUser.GetLastTableID();
				bool bRes3 = stuTagUser.GetTableID() == stuTagUser.GetLastTableID() && stuTagUser.GetChairID() != stuTagUser.GetLastChairID();
				
				if (bRes1 && (bRes2 || bRes3))
				{
					CMD_Command tempCMD;
					tempCMD.wMainCmdID = IPC_MAIN_USER;
					tempCMD.wSubCmdID = IPC_SUB_USER_COME;

					UserStruct& uStruct = stuTagUser.GetUserStruct();

					memset(m_cbBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(BYTE));
					::memcpy(m_cbBuffer, &uStruct, sizeof(UserStruct));

					// 附加用户结构
					UserStructSuffix uStructSuffix = stuTagUser.GetUserStructSuffix();
					CSendPacketHelper Packet(m_cbBuffer + sizeof(UserStruct), sizeof(m_cbBuffer) - sizeof(UserStruct));
					Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);

					this->OnNetwork_TurnToGame(tempCMD, m_cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
				}
			}

			// 更新椅子状态
			if (m_curRoom)
			{
				m_curRoom->UpdateChairsStatus(stuTagUser);
			}
		}break;

		default:
			break;
	}
	
	// 更新完状态,设置玩家上次状态
	stuTagUser.SetLastUserStatus(cbUserStatus);

	// 更新玩家UI
	if (stuTagUser.IsSelf())
	{
		stuTagUser.UpdateUI(m_pManagerRef);
	}

	// 更新玩家列表UI
	UserList* pUserlist = UserList::GetInstance();
	pUserlist->Update();

	//	同桌的前提下才下发到游戏
	IPC* ipc = IPC::GetInstance();
	bSendToGame &= ipc->IsGameFinished();
	if (bSendToGame && (stuTagUser.GetTableID() == tSelf.GetTableID()))
	{
		UserStruct& uStruct = stuTagUser.GetUserStruct();

		memset(m_cbBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(BYTE));
		::memcpy(m_cbBuffer, &uStruct, sizeof(UserStruct));

		// 附加用户结构
		UserStructSuffix uStructSuffix = stuTagUser.GetUserStructSuffix();
		CSendPacketHelper Packet(m_cbBuffer + sizeof(UserStruct), sizeof(m_cbBuffer) - sizeof(UserStruct));
		Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);

		this->OnNetwork_TurnToGame(cmd, m_cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
	}

	// 处理玩家离开消息
	// 放在最后检测处理
	if (US_NULL == cbUserStatus)
	{
		//g_Logger->TryInfo(_T("RoomManager - User Leave Room[user_id = %lu, user_gameid = %lu]"), stuTagUser.GetUserID(), stuTagUser.GetGameID());
		
		// 先从列表UI移除该玩家(顺便清理一下离线列表)
		DWORD wUserID = stuTagUser.GetUserID();
		UserList* pUserlist = UserList::GetInstance();
		pUserlist->RemoveBodyItem(wUserID);
		pUserlist->RemoveOffLineMap(wUserID);

		// 然后才能移除玩家数据
		DataManager* pDatamanaget = DataManager::GetInstance();
		pDatamanaget->RemoveUserDataByID(wUserID);

		//g_Logger->TryInfo(_T("RoomManager - User Leave Room Complete"));
	}
	
	//g_Logger->TryDebug(_T("RoomManager - Update User End --------------------------------"));
}
