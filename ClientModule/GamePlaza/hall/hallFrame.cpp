#include "stdafx.h"
#include "hallFrame.h"
#include "exitFrame.h"
#include "infoFrame.h"
#include "loginFrame.h"
#include "lockFrame.h"
#include "activity.h"
#include "signFrame.h"
#include "doleFrame.h"
#include "bank.h"
#include "mall.h"

extern GameResource*	R;
extern GameUtil*		g_GameUtil;
extern CStaticLogger	g_Logger;

HallFrame* HallFrame::g_instance = NULL;

HallFrame::HallFrame()
	: m_pViewEdit(nullptr)
	, m_pSendEdit(nullptr)
	, m_pNickName(nullptr)
	, m_pID(nullptr)
	, m_pRoomManager(nullptr)
	, m_pDataManager(nullptr)
	, m_bSwitchToLogin(false)
	, m_pGameWebBrowser(nullptr)
	, m_wMaxFrequentCount(3)
{
	m_rcLastWnd = { 0 };
	m_rcLastRollCtl = { 0 };
	m_pointChatROle = { 0 };
	m_pDataManager = DataManager::GetInstance();

	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));

	::srand((unsigned)time(NULL));
}

HallFrame::~HallFrame()
{
	do // 断开登录服务器
	{
		if (LoginServer::IsInstanceExist())
		{
			LoginServer* pLoginServer = LoginServer::GetInstance();
			pLoginServer->Release();
		}
	} while (0);

	do // 清除房间管理器
	{
		if (m_pRoomManager)
		{
			RoomManager::Release();
			m_pRoomManager = nullptr;
		}
	} while (0);

	do // 清除浏览器对象
	{
		if (m_pGameWebBrowser)
		{
#ifdef GAMEPLAZA_STYLE_1
#else if GAMEPLAZA_STYLE_2
			m_pGameWebBrowser->Release();
#endif
		}
	} while (0);

	do // 清除ImgOle组件
	{
		this->_UnRegisterImgOle();
	} while (0);

	do // 保存本地游戏数据
	{
		this->SaveLocalData();
	} while (0);

	// 是否切换到登录界面
	if (m_bSwitchToLogin)
	{
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.wShowWindow = SW_SHOWMAXIMIZED;
		PROCESS_INFORMATION	tProcessInfo;
		ZeroMemory(&tProcessInfo, 0, sizeof(tProcessInfo));

		int err_code = 0;
		TCHAR szModuleFileName[MAX_PATH]; // 全路径名
		memset(szModuleFileName, 0, MAX_PATH * sizeof(TCHAR));
		if (FALSE == ::GetModuleFileName(NULL, szModuleFileName, MAX_PATH))
		{
			//重启失败
			err_code = GetLastError();
		}
		else
		{
			BOOL bSuccess = ::CreateProcess(szModuleFileName, NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &tProcessInfo);
			if (TRUE == bSuccess)
			{
				//重启成功
			}
			else
			{
				//重启失败
				err_code = GetLastError();
			}
		}
	}

	g_Logger->TryInfo(_T("HallFrame - Destroy"));
}

HallFrame* HallFrame::GetInstance()
{
	if (g_instance == NULL)
		g_instance = new HallFrame();

	return g_instance;
}

BOOL HallFrame::CreateFrame()
{
	if (FALSE == IsWindow(m_hWnd))
	{
		m_hWnd = this->Create(NULL, R->MainWndTitleName, UI_CLASSSTYLE_DIALOG | WS_MAXIMIZEBOX, 0L, 0, 0, 1024, 738);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}

	return TRUE;
}

void HallFrame::SwitchToLogin()
{
	m_bSwitchToLogin = true;
	this->Close(CMW_CLOSE_SURE);
}

//菜单栏关闭在此处处理(点击按钮关闭在Notify处处理)
LRESULT HallFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (CMW_CLOSE_SURE == wParam)
	{
		return WindowImplBase::OnClose(uMsg, wParam, lParam, bHandled);
	}
	else
	{
		ExitFrame* pExitFrame = ExitFrame::GetInstance();
		pExitFrame->CreateFrame();
	}

	return FALSE;
}

void HallFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("HallFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(g_instance);

	// 退出大厅循环
	do
	{
		// 退出进程
		g_GameUtil->EndHall();
	} while (0);
};

CControlUI* HallFrame::CreateControl(LPCTSTR pstrClassName)
{
	CDialogBuilder builder;
	CControlUI* pControl = nullptr;

	if (_tcsicmp(pstrClassName, _T("sidebar_hall")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/sidebar_hall.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("sidebar_table")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/sidebar_table.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	{
		pControl = new CWkeWebkitUI();
	}
	else if (_tcsicmp(pstrClassName, _T("hall_panel")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/hall_panel.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("room_list_panel")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/room/room_list_panel.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("room_panel")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/room/room_panel.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("prop_panel")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/prop/prop_panel.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcscmp(pstrClassName, DUI_CTR_CONTROLGDIPLUS) == 0)
	{
		pControl = new CControlGdiplusUI();
	}

	// 风格2
	else if (_tcscmp(pstrClassName, _T("table_panel")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/room/table_panel.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcscmp(pstrClassName, _T("sidebar_panel")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/room/sidebar_panel.xml"), (UINT)0, this, &m_PaintManager));
	}

	return pControl;
}

void HallFrame::InitLocalConfig()
{
	this->_InitExperinceConfig();
	this->_InitOnlineDescConfig();
	this->ReadFrequentGamesConfig();
}

void HallFrame::_InitExperinceConfig()
{
	g_Logger->TryInfo(_T("RoomManager - Init GameExperince Config"));
	
	bool bExist = false;
	Json::Value vRoot;
	JsonFile* pFile = new JsonFile(_T("EXPERINCE"));
	bool bResult = pFile->initRootNode(vRoot);
	SAFE_DELETE(pFile);
	if (bResult)
	{
		DataManager* pDataManager = DataManager::GetInstance();

		if (vRoot["GameXPLevel"].isArray())
		{
			for (size_t i = 0; i < vRoot["GameXPLevel"].size(); ++i)
			{
				Json::Value& node = vRoot["GameXPLevel"][i];
				DWORD dwLevel = node["level"].asInt();
				LONGLONG lScore = node["score"].asInt64();

				pDataManager->InsertXPLevelConfig(dwLevel, lScore);
			}
		}
	}

	g_Logger->TryInfo(_T("RoomManager - Init GameLevel GameExperince Complete"));
}

void HallFrame::_InitOnlineDescConfig()
{
	g_Logger->TryInfo(_T("HallFrame - Init Online Config"));

	Json::Value vRoot;
	JsonFile* pFile = new JsonFile(_T("ONLINE"));
	bool bResult = pFile->initRootNode(vRoot);
	SAFE_DELETE(pFile);
	if (bResult)
	{
		DataManager* pDataManager = DataManager::GetInstance();
		Json::Value& vOnlineDescNode = vRoot["OnlineDesc"];
		if (vOnlineDescNode.isArray())
		{
			for (size_t i = 0; i < vOnlineDescNode.size(); ++i)
			{
				Json::Value& node = vOnlineDescNode[i];
				DWORD dwCount = node["count"].asInt();
				std::string& strDesc = node["desc"].asString();
				int iStatus = node["status"].asInt();

				pDataManager->InsertOnlineDescConfig(dwCount, strDesc, iStatus);
			}
		}
	}
	g_Logger->TryInfo(_T("HallFrame - Init Online Config Complete"));
}

void HallFrame::InitGameList()
{
#ifdef GAMEPLAZA_STYLE_1
	this->_InitGameListStyle_1();
#else if GAMEPLAZA_STYLE_2
	this->_InitGameListStyle_2();
#endif
}

DWORD HallFrame::_ReCheckGameItemMap()
{
	ServiceData* pServerListData = ServiceData::GetInstance();
	DWORD typeCount = pServerListData->GetGameTypeCount();

	DWORD iNextTypeKey = 0;
	DWORD dwGameItemUITextcolor = 0xFFFFFFFF;
	GameFontColor* pFontColor = pServerListData->GetFontColorByID(2);
	if (pFontColor)
	{
		dwGameItemUITextcolor = pFontColor->dwTextcolor;
	}

	// 游戏类型分组
	std::vector<tagGameType*> vGameTypes;
	for (size_t i = 0; i < typeCount; ++i)
	{
		tagGameType* pGameType = pServerListData->EnumGameType(iNextTypeKey);
		DWORD wTypeID = pGameType->wTypeID;

		// 游戏子项分组
		DWORD iNextKindKey = 0;
		std::vector<CGameItemUI*> vTempGameItems;
		for (size_t j = 0; j < pServerListData->GetGameKindCount(); ++j)
		{
			tagGameKind* pGameKind = pServerListData->EnumGameKind(iNextKindKey);
			if (wTypeID == pGameKind->wTypeID)
			{
				CGameItemUI* pGameItemUI = new CGameItemUI();
				if (pGameItemUI->InitData(pGameKind, dwGameItemUITextcolor))
				{
					vTempGameItems.push_back(pGameItemUI);
				}
				else
				{
					SAFE_DELETE(pGameItemUI);
				}
			}
		}

		if (vTempGameItems.size() > 0)
		{
			vGameTypes.push_back(pGameType);
			this->m_vGameItemMap[pGameType->wTypeID] = vTempGameItems;
		}
	}

	// 检测游戏列表版本状态
	for (auto& it : this->m_vGameItemMap)
	{
		std::vector<CGameItemUI*>& vGameItems = it.second;
		for (int i = 0; i < vGameItems.size(); ++i)
		{
			CGameItemUI* pItem = vGameItems[i];
			if (!pItem) continue;

			CGameItemUI::GAME_STATE eGameVersionStatus = this->CheckGameVersion(pItem->GetGameKind(), pItem);
			pItem->SetGameStatus(eGameVersionStatus);
		}
	}

	return dwGameItemUITextcolor;
}

void HallFrame::_InitGameListStyle_1()
{
	g_Logger->TryDebug(_T("HallFrame - Init Game List Style_1 Config"));
	
	CTabLayoutUI* hallpanel_switch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsHallPanel_Switch));
	if (!hallpanel_switch) return;

	// 重新检测游戏Item容器
	DWORD dwGameItemUITextcolor = this->_ReCheckGameItemMap();

	// 游戏列表布局
	do
	{
		// 游戏类型排序
		size_t iIndex = 0;
		std::vector<tagGameType*> vGameTypes;
		ServiceData* pServerListData = ServiceData::GetInstance();
		for (auto& it : m_vGameItemMap)
		{
			tagGameType* pGameType = pServerListData->SearchGameType(it.first);
			if (!pGameType) continue;

			vGameTypes.push_back(pGameType);
		}
		std::sort(vGameTypes.begin(), vGameTypes.end(), CGameItemUI::less_sort_type_id);

		// 创建"所有游戏"列表
		do
		{
			// 创建游戏单选按钮
			this->CreatGameOptBtn(iIndex, _T("所有游戏"));

			// 创建页
			CTileLayoutUI* pTiled = new CTileLayoutUI();
			_stprintf_s(m_pzBuffer, _T("hallpanel-switch_%u"), iIndex);
			pTiled->SetName(m_pzBuffer);
			hallpanel_switch->Add(pTiled);

			// 排序布局
			std::vector<CGameItemUI*> vAllItems;
			for (const auto& it : vGameTypes)
			{
				if (m_vGameItemMap.find(it->wTypeID) != m_vGameItemMap.end())
				{
					std::vector<CGameItemUI*>& vTemp = m_vGameItemMap[it->wTypeID];
					std::sort(vTemp.begin(), vTemp.end(), CGameItemUI::less_sort_item_id);
					for (const auto& it : vTemp)
					{
						CGameItemUI* pGameItemUI = new CGameItemUI();
						const tagGameKind* pGameKind = it->GetGameKind();
						if (pGameItemUI->InitData(pGameKind, dwGameItemUITextcolor))
						{
							vAllItems.push_back(pGameItemUI);
						}
						else
						{
							SAFE_DELETE(pGameItemUI);
						}
					}
				}
			}
			this->LayoutGameItem(pTiled, vAllItems);
		} while (0);

		// 循环创建每个游戏列表
		do
		{
			for (const auto& it : vGameTypes)
			{
				// 创建游戏单选按钮
				++iIndex;
				this->CreatGameOptBtn(iIndex, it->szTypeName);

				// 创建页
				CTileLayoutUI* pTiled = new CTileLayoutUI();
				_stprintf_s(m_pzBuffer, _T("hallpanel-switch_%u"), iIndex);
				pTiled->SetName(m_pzBuffer);
				hallpanel_switch->Add(pTiled);

				// 排序布局
				if (m_vGameItemMap.find(it->wTypeID) != m_vGameItemMap.end())
				{
					this->LayoutGameItem(pTiled, m_vGameItemMap[it->wTypeID]);
				}
			}
		} while (0);

	} while (0);

	hallpanel_switch->SelectItem(0);

	g_Logger->TryDebug(_T("HallFrame - Init Game List Style_1 Config Complete"));
}

void HallFrame::_InitGameListStyle_2()
{
	g_Logger->TryDebug(_T("HallFrame - Init Game List Style_2 Config"));

	// 重新检测游戏Item容器
	this->_ReCheckGameItemMap();

	// 游戏列表布局
	do
	{
		// 游戏类型排序
		size_t iIndex = 0;
		std::vector<tagGameType*> vGameTypes;
		ServiceData* pServerListData = ServiceData::GetInstance();

		for (auto& it : m_vGameItemMap)
		{
			tagGameType* pGameType = pServerListData->SearchGameType(it.first);
			if (!pGameType) continue;

			vGameTypes.push_back(pGameType);
		}
		std::sort(vGameTypes.begin(), vGameTypes.end(), CGameItemUI::less_sort_type_id);

		// 创建游戏树状列表UI
		do 
		{
			m_pGameListRoot = static_cast<CContainerUI*>(m_PaintManager.FindControl(R->kStyle2CtnHallPanel_GameList));
			if (m_pGameListRoot)
			{
				// 创建列表视图根节点
				m_pGameListTreeView = this->AddSeasonRoot(m_pGameListRoot, R->kStyle2TvwHallPanel_TreeView);
				ASSERT(m_pGameListTreeView);
				if (!m_pGameListTreeView) return;

				// 创建"最近登录游戏"列表项
				this->_InitFrequentNode();

				// 创建游戏类型
				for (const auto& it : vGameTypes)
				{
					WORD wTypeID = it->wTypeID;
					CTreeNodeUI* pGameTypeNodeUI = this->AddGameTypeNode(m_pGameListTreeView, it, wTypeID);
					if (!pGameTypeNodeUI) continue;
					m_vGameTypeNodeUI.push_back(pGameTypeNodeUI);

					//  创建游戏子项
					if (m_vGameItemMap.find(wTypeID) != m_vGameItemMap.end())
					{
						// 游戏排序
						std::vector<CGameItemUI*>& vTemp = m_vGameItemMap[it->wTypeID];
						std::sort(vTemp.begin(), vTemp.end(), CGameItemUI::less_sort_item_id);

						for (int i = 0; i < vTemp.size(); ++i)
						{
							CGameItemUI* pGameItem = vTemp[i];
							CTreeNodeUI* pGameKindNodeUI = this->AddGameKindNode(pGameTypeNodeUI, pGameItem, wTypeID);
							if (!pGameKindNodeUI) continue;
							m_vGameKindNodeUI.push_back(pGameKindNodeUI);

							// 创建房间
							std::vector<tagGameServer*>& vGameRooms = pGameItem->GetGameRoomList();
							std::sort(vGameRooms.begin(), vGameRooms.end(), CGameItemUI::less_sort_room_id);
							for (int j = 0; j < vGameRooms.size(); ++j)
							{
								CTreeNodeUI* pGameRoomNodeUI = this->AddGameRoomNode(pGameKindNodeUI, vGameRooms[j], wTypeID);
								if (!pGameRoomNodeUI) continue;
								m_vGameRoomNodeUI.push_back(pGameRoomNodeUI);
							}
						}
					}
				}
			}
		} while (0);
	} while (0);

	// 展开无子项节点的折叠按钮
	do 
	{
		ASSERT(m_pGameListTreeView);
		if (!m_pGameListTreeView) return;
		for (int i = 0; i < m_pGameListTreeView->GetCount(); ++i)
		{
			CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(m_pGameListTreeView->GetItemAt(i));
			if (!pNode)continue;

			if (pNode->GetCountChild() <= 0)
			{
				pNode->GetFolderButton()->Selected(true);
			}
		}
	} while (0);

	// 展开游戏列表
	do 
	{
		if (m_vLastFolderFlagMap.size() <= 0)
		{
			for (int i = 0; i < m_vGameTypeNodeUI.size(); ++i)
			{
				CCheckBoxUI* pFolderBtn = m_vGameTypeNodeUI[i]->GetFolderButton();
				if (pFolderBtn) pFolderBtn->Selected(true);
			}
		}
		else
		{
			for (auto& it : m_vGameTypeNodeUI)
			{
				CDuiString& pzName = it->GetName();
				if (this->FindFolderFlagByName(pzName))
				{
					CCheckBoxUI* pFolderButton = it->GetFolderButton();
					if (!pFolderButton) continue;

					pFolderButton->Selected(true);
				}
			}

			for (auto& it : m_vGameKindNodeUI)
			{
				CDuiString& pzName = it->GetName();
				if (this->FindFolderFlagByName(pzName))
				{
					CCheckBoxUI* pFolderButton = it->GetFolderButton();
					if (!pFolderButton) continue;

					pFolderButton->Selected(true);
				}
			}

			for (auto& it : m_vGameRoomNodeUI)
			{
				CDuiString& pzName = it->GetName();
				if (this->FindFolderFlagByName(pzName))
				{
					CCheckBoxUI* pFolderButton = it->GetFolderButton();
					if (!pFolderButton) continue;

					pFolderButton->Selected(true);
				}

				// 选中上一次进入的房间node
				if (_tcscmp(pzName, m_sLastRoomNodeName) == 0)
				{
					it->Select(true);
				}
			}
			
			// 恢复滚动条位置
			if (m_pGameListRoot && m_pGameListTreeView)
			{
				m_pGameListTreeView->SetScrollRange(m_tLastScrollRange);
				m_pGameListTreeView->SetScrollPos(m_tLastScrollPos);
			}
		}
	} while (0);

	g_Logger->TryDebug(_T("HallFrame - Init Game List Style_2 Config Complete"));
}

// 检测游戏列表版本状态
CGameItemUI::GAME_STATE HallFrame::CheckGameVersion(const tagGameKind* pData, CGameItemUI* pGameItem)
{
	CGameItemUI::GAME_STATE eGameVersionStatus = CGameItemUI::GAME_STATE::GAME_STATE_VERSION_ERROE;
	if (pData && pGameItem)
	{
		DWORD wKindID = pData->wKindID;
		std::vector<tagGameServer*>& vGameRooms = pGameItem->GetGameRoomList();

		ServiceData* pServerListData = ServiceData::GetInstance();
		CDuiString pzExePath = m_PaintManager.GetInstancePath() + pGameItem->GetCombinedFileName();
		DWORD wServerVersion = pData->dwMaxVersion;
		DWORD wLocalVersion = pServerListData->ReadExeVersionInfo(wKindID, pzExePath);

		// 检查房间数量
		do
		{
			vGameRooms.clear();
			DWORD dwNextKey = 0;
			for (size_t i = 0; i < pServerListData->GetGameServerCount(); ++i)
			{
				tagGameServer* pGameServer = pServerListData->EnumGameServer(dwNextKey);
				if (wKindID == pGameServer->wKindID)
				{
					vGameRooms.push_back(pGameServer);
				}
			}
		} while (0);

#ifdef SKIP_VERSION_CHECK
		// 测试状态,跳过检测
		eGameVersionStatus = CGameItemUI::GAME_STATE::GAME_STATE_VERSION_NORMAL;
#else
		if (vGameRooms.size() <= 0)
		{
			// 无房间,暂未开放
			eGameVersionStatus = CGameItemUI::GAME_STATE::GAME_STATE_NOT_OPEN;
		}
		else if (!g_GameUtil->IsFileExist(pzExePath.GetData()))
		{
			// 游戏路径不存在
			eGameVersionStatus = CGameItemUI::GAME_STATE::GAME_STATE_NOT_EXIST;
		}
		else if (wLocalVersion == wServerVersion)
		{
			// 状态正常
			eGameVersionStatus = CGameItemUI::GAME_STATE::GAME_STATE_VERSION_NORMAL;
		}
		else
		{
			// 版本异常
			eGameVersionStatus = CGameItemUI::GAME_STATE::GAME_STATE_VERSION_ERROE;
		}
#endif // GAME_TEST_SWITCH
		}

	return eGameVersionStatus;
	}

void HallFrame::CleanGameList()
{
#ifdef GAMEPLAZA_STYLE_1
	CTabLayoutUI* hallpanel_switch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsHallPanel_Switch));
	if (hallpanel_switch)
	{
		hallpanel_switch->RemoveAll();
		m_vGameItemMap.clear();
	}

	CVerticalLayoutUI* pOptLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(R->kOptHallPanel_VLayout));
	if (pOptLayout)
	{
		pOptLayout->RemoveAll();
		m_vOptName.clear();
	}
#else if GAMEPLAZA_STYLE_2
	if (m_pGameListRoot && m_pGameListTreeView)
	{
		// 记录上一次游戏列表UI信息
		do 
		{
			// 记录游戏列表折叠标志
			m_vLastFolderFlagMap.clear();
			for (int i = 0; i < m_pGameListTreeView->GetCount(); ++i)
			{
				CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(m_pGameListTreeView->GetItemAt(i));
				if (!pNode) continue;

				CCheckBoxUI* pFolderButton = pNode->GetFolderButton();
				if (pFolderButton && pFolderButton->IsSelected())
				{
					this->AddFolderFlagByName(pNode->GetName());
				}
			}

			// 清除记录节点
			m_vGameTypeNodeUI.clear();
			m_vGameKindNodeUI.clear();
			m_vGameRoomNodeUI.clear();

			// 记录上一次的滚动条位置
			m_tLastScrollRange = m_pGameListTreeView->GetScrollRange();
			m_tLastScrollPos = m_pGameListTreeView->GetScrollPos();
		} while (0);

		// 清除列表UI
		m_pGameListRoot->RemoveAll();

		// 由于风格2模式下的CGameItemUI并没有Add到UI视图上,所以这里要手动释放内存
		do
		{
			for (auto& it : m_vGameItemMap)
			{
				std::vector<CGameItemUI*>& vGameItems = it.second;
				for (int i = 0; i < vGameItems.size(); ++i)
				{
					SAFE_DELETE(vGameItems[i]);
				}
			}
			m_vGameItemMap.clear();
		} while (0);
	}
#endif
}

bool HallFrame::_RegisterImgOle()
{
	HINSTANCE hLib = ::LoadLibrary(_T("ImageOleCtrl.dll"));
	if (!hLib) return false;

	::FARPROC lpDllEntryPoint = ::GetProcAddress(hLib, "DllRegisterServer");
	if (lpDllEntryPoint != NULL)
	{
		if (FAILED((*lpDllEntryPoint)()))
		{
			::FreeLibrary(hLib);
			return false;
		}
	}
	else
	{
		::FreeLibrary(hLib);
		return false;
	}

	::FreeLibrary(hLib);
	return true;
}

bool HallFrame::_UnRegisterImgOle()
{
	g_Logger->TryInfo(_T("HallFrame - UnRegister ImgOle"));

	HINSTANCE hLib = ::LoadLibrary(_T("ImageOleCtrl.dll"));
	if (!hLib) return false;

	::FARPROC lpDllUnInstall = ::GetProcAddress(hLib, "DllUnregisterServer");
	if (lpDllUnInstall != NULL)
	{
		if (FAILED((*lpDllUnInstall)()))
		{
			::FreeLibrary(hLib);
			return false;
		}
	}
	else
	{
		::FreeLibrary(hLib);
		return false;
	}

	::FreeLibrary(hLib);

	g_Logger->TryInfo(_T("HallFrame - UnRegister ImgOle Complete"));

	return true;
}

bool HallFrame::_RegisterImgOleCallBack()
{
	IRichEditOleCallback2* pRichEditOleCallback2 = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr) && m_pViewEdit)
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		CTxtWinHost* pTxtWinHost = m_pViewEdit->GetTxtWinHost();
		ITextServices * pTextServices = pTxtWinHost->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		m_pViewEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}
	else
		return false;
	
	pRichEditOleCallback2 = NULL;
	hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr) && m_pSendEdit)
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		CTxtWinHost* pTxtWinHost = m_pSendEdit->GetTxtWinHost();
		ITextServices * pTextServices = pTxtWinHost->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		m_pSendEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}
	else
		return false;

#ifdef UNICODE || _UNICODE
	// 注册文本拖放支持(可直接拖拽编辑框中的)
	IDropTarget *pdt = m_pSendEdit->GetTxDropTarget();
	hr = ::RegisterDragDrop(m_hWnd, pdt);
	pdt->Release();
#endif // UNICODE

	return true;
}

void HallFrame::InitFaceData()
{
	bool bRes = false;
	
	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	bRes = g_RichEdtUtil->LoadFaceFile(_T("face\\FaceConfig.xml"));
	if (!bRes)
	{
		_stprintf_s(m_pzBuffer, _T("%s"), _T("加载动态表情资源失败!"));
		goto lab_face_load_failed;
	}
	
	if (!this->_RegisterImgOle() || !this->_RegisterImgOleCallBack())
	{
		_stprintf_s(m_pzBuffer, _T("%s"), _T("注册动态表情库失败!"));
		goto lab_face_load_failed;
	}
	return;

// 加载表情失败
lab_face_load_failed:
	DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示!"), m_pzBuffer);
}

void HallFrame::InitHallManager()
{
	m_pRoomManager = RoomManager::GetInstance();
}

void HallFrame::InitWeb()
{
#ifdef GAMEPLAZA_STYLE_1
	//设置登录界面网页
	m_pGameWebBrowser = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeSidebarHall_Web));
	if (m_pGameWebBrowser)
	{
		LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("AdWeb_Hall_Right"));
		m_pGameWebBrowser->Navigate(lpUrl);
		m_pGameWebBrowser->SetClientCallback(this);
	}
#else if GAMEPLAZA_STYLE_2
	// 绑定游戏网页格式串
	m_pzUrlFormat = g_GameUtil->GetWebByName(_T("Hall_GameWeb"));
	m_pzUrlOfficial = g_GameUtil->GetWebByName(_T("Official"));

	// 修改注册表:设置IWebBrowser2内核ie版本为8.0以上
	/*
	10001	(0x2711)	Internet Explorer 10。网页以IE 10的标准模式展现，页面!DOCTYPE无效
	10000	(0x02710)	Internet Explorer 10。在IE 10标准模式中按照网页上!DOCTYPE指令来显示网页。Internet Explorer 10 默认值。
	9999	(0x270F)	Windows Internet Explorer 9. 强制IE9显示，忽略!DOCTYPE指令
	9000	(0x2328)	Internet Explorer 9. Internet Explorer 9默认值，在IE9标准模式中按照网页上!DOCTYPE指令来显示网页。
	8888	(0x22B8)	Internet Explorer 8，强制IE8标准模式显示，忽略!DOCTYPE指令
	8000	(0x1F40)	Internet Explorer 8默认设置，在IE8标准模式中按照网页上!DOCTYPE指令展示网页
	7000	(0x1B58)	使用WebBrowser Control控件的应用程序所使用的默认值，在IE7标准模式中按照网页上!DOCTYPE指令来展示网页。
	*/
	do
	{
		int iReturn = 0;
		tstring strOutInfo = _T("");
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_BROWSER_EMULATION", 8000, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_ACTIVEX_REPURPOSEDETECTION", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_IMG", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_OBJECT", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_SCRIPT", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_Cross_Domain_Redirect_Mitigation", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_ENABLE_SCRIPT_PASTE_URLACTION_IF_PROMPT", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_LOCALMACHINE_LOCKDOWN", 1, strOutInfo);
		if (iReturn == 0) iReturn = GameUtil::WriteWebBrowserRegKey("FEATURE_GPU_RENDERING", 1, strOutInfo);
		if (iReturn != 0)
		{
			CDuiString pzErrMsg;
			pzErrMsg.Format(_T("错误写入注册表键:%s\nRegSetValueEx失败:代码%d"), strOutInfo.c_str(), iReturn);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrMsg.GetData());
		}
	} while (0);

	CActiveXUI* pActiveX = static_cast<CActiveXUI*>(m_PaintManager.FindControl(R->kStyle2CtlHallPanel_GameWeb));
	if (pActiveX)
	{
		m_pGameWebBrowser = nullptr;
		pActiveX->GetControl(IID_IWebBrowser2, (void**)&m_pGameWebBrowser);
		if (m_pGameWebBrowser != nullptr)
		{
			// 禁止javascript警告。
			m_pGameWebBrowser->put_Silent(VARIANT_TRUE);

			// 禁止拖入文件
			m_pGameWebBrowser->put_RegisterAsDropTarget(VARIANT_FALSE);

			CButtonUI* pOffcialBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnHallPanel_GoOfficial));
			if (pOffcialBtn)
			{
				m_PaintManager.SendNotify(pOffcialBtn, DUI_MSGTYPE_CLICK);
			}
		}
	}
#endif
}

void HallFrame::InitHallUI()
{
	m_pViewEdit			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtChatPanel_View));
	m_pSendEdit			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtChatPanel_Input));
	m_pNickName			= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtHallMain_NickName));
	m_pID				= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtHallMain_ID));
	m_pRollCtl			= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlHallMain_RollLayout));

	// 滚动文本相关位置初始化
	if (m_pRollCtl)
	{
		::GetClientRect(m_hWnd, &m_rcLastWnd);
		m_rcLastRollCtl = m_pRollCtl->GetPos();

		// 开启底栏跑马灯定时器
		TimerCallBack timepro = CC_CALLBACK_2(HallFrame::TimerProFunc, this);
		g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_HALL_ROOL_TEXT, 20);
	}

	// 设置玩家基础信息UI
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& uSelf = pDataManager->GetSelfData();
	uSelf.UpdateUI(&m_PaintManager);

	// 更新本机锁定状态
	CButtonUI* pLockBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnHallMain_LockMachine));
	if (pLockBtn)
	{
		CDuiString pzIcon;
		if (uSelf.IsMoorMachine())
		{
			pzIcon = _T("hall\\icon_lock_pc.png");
		}
		else
		{
			pzIcon = _T("hall\\icon_unlock_pc.png");
		}
		pLockBtn->SetBkImage(pzIcon);
	}
}

CGameItemUI* HallFrame::FindGameItemByKindID(WORD wKindID)
{
	for (auto& it : m_vGameItemMap)
	{
		std::vector<CGameItemUI*>& vGameItems = it.second;
		for (int i = 0; i < vGameItems.size(); ++i)
		{
			CGameItemUI* pItemp = vGameItems[i];
			if (!pItemp) continue;

			if (wKindID == pItemp->GetGameKind()->wKindID)
			{
				return pItemp;
			}
		}
	}

	return nullptr;
}

CGameItemUI* HallFrame::FindGameItemByName(LPCTSTR lpName)
{
	for (auto& it : m_vGameItemMap)
	{
		for (int i = 0; i < it.second.size(); ++i)
		{
			CGameItemUI* pItemp = it.second[i];
			if (!pItemp) continue;

			if (_tcscmp(pItemp->GetName(), lpName) == 0)
			{
				return pItemp;
			}
		}
	}

	return nullptr;
}

void HallFrame::TimerProFunc(UINT_PTR idEvent, DWORD dwTime)
{
	switch (idEvent)
	{
		case TIMER_ID_HALL_ROOL_TEXT:
		{
			RECT rcWnd = { 0 };
			::GetClientRect(m_hWnd, &rcWnd);
			
			if (FALSE == ::EqualRect(&m_rcLastWnd, &rcWnd))
			{
				LONG lw_lastctl = m_rcLastRollCtl.right - m_rcLastRollCtl.left;
				LONG lw_lastwnd = m_rcLastWnd.right - m_rcLastWnd.left;
				LONG lw_currwnd = rcWnd.right - rcWnd.left;
				FLOAT fPercent = (FLOAT)m_rcLastRollCtl.left / lw_lastwnd;
				m_rcLastRollCtl.left = fPercent * lw_currwnd;
				m_rcLastRollCtl.right = m_rcLastRollCtl.left + lw_lastctl;
				m_rcLastWnd = rcWnd;

				RECT rcTemp = m_pRollCtl->GetPos();
				m_rcLastRollCtl.top = rcTemp.top;
				m_rcLastRollCtl.bottom = rcTemp.bottom;
				m_pRollCtl->SetPos(m_rcLastRollCtl);
			}

			RECT rcCurrRollCtl = m_pRollCtl->GetPos();
			LONG lw_currctl = rcCurrRollCtl.right - rcCurrRollCtl.left;

			--rcCurrRollCtl.left;
			--rcCurrRollCtl.right;

			if (rcCurrRollCtl.right < rcWnd.left)
			{
				rcCurrRollCtl.left = rcWnd.right;
				rcCurrRollCtl.right = rcCurrRollCtl.left + lw_currctl;
			}
			m_pRollCtl->SetPos(rcCurrRollCtl);
			m_rcLastRollCtl = rcCurrRollCtl;
		}break;

		default:
			break;
	}
}

void HallFrame::InitData()
{
	g_Logger->TryInfo(_T("HallFrame - Open"));

	// 设置窗口图标
	this->SetIcon(IDI_GAMEPLAZA_ICON);

	this->InitLocalConfig();			// 初始本地配置
	this->InitHallManager();			// 创建房间、桌子管理器
	this->InitHallUI();					// 初始化大厅UI
	this->InitGameList();				// 初始化游戏列表
	this->InitFaceData();				// 初始化表情库
	this->InitWeb();					// 初始化网页内容

	// 设置登录服委托对象
	LoginServer* pLoginServer = LoginServer::GetInstance();
	pLoginServer->SetDelegate(this, &m_PaintManager);

	// 设置当前面板为大厅面板
	g_GameUtil->SetCurPanel(&m_PaintManager, Tls_Panel::TLS_PANEL_HALL, TLS_SIDEBAR_HALL);

	g_Logger->TryInfo(_T("HallFrame - Open Complete"));
}

void HallFrame::OnFaceCtrlSel(int iSelID, int iSelIndex, LPCTSTR lpSelName)
{
	if (!m_pSendEdit || !lpSelName) return;

	m_pSendEdit->SetFocus();
	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	g_RichEdtUtil->InsertFace(m_pSendEdit, lpSelName, iSelID, iSelIndex);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 聊天面板右键菜单 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void HallFrame::OnMenuEvent(CDuiString* strMenuName, CControlUI* pControl)
{
	if (!strMenuName || !pControl) return;

	if (_tcscmp(*strMenuName, _T("Menu_Cut")) == 0)
		ChatEdit_RCut(pControl);
	else if (_tcscmp(*strMenuName, _T("Menu_Copy")) == 0)
		ChatEdit_RCopy(pControl);
	else if (_tcscmp(*strMenuName, _T("Menu_Paste")) == 0)
		ChatEdit_RPaste(pControl);
	else if (_tcscmp(*strMenuName, _T("Menu_SelAll")) == 0)
		ChatEdit_RSelAll(pControl);
	else if (_tcscmp(*strMenuName, _T("Menu_Clear")) == 0)
		ChatEdit_RClear(pControl);
	else if (_tcscmp(*strMenuName, _T("Menu_ZoomRatio_200")) == 0
		|| (_tcscmp(*strMenuName, _T("Menu_ZoomRatio_150")) == 0)
		|| (_tcscmp(*strMenuName, _T("Menu_ZoomRatio_125")) == 0)
		|| (_tcscmp(*strMenuName, _T("Menu_ZoomRatio_100")) == 0)
		|| (_tcscmp(*strMenuName, _T("Menu_ZoomRatio_75")) == 0))
		ChatEdit_RZoomRatio(pControl, *strMenuName);
	else if (_tcscmp(*strMenuName, _T("Menu_SaveAs")) == 0)
		ChatEdit_RSaveAs(pControl);
	else
	{

	}

	SAFE_DELETE(strMenuName);
}

void HallFrame::ChatEdit_RButtonDown(TNotifyUI& msg)
{
	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	POINT client_point = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
	POINT screen_point = client_point;
	m_pointChatROle = client_point;
	CControlUI* pControl = msg.pSender;
	if (pControl)
	{
		if (pControl == m_pViewEdit)
		{
			CMenuWnd* pMenu = new CMenuWnd();
			::ClientToScreen(m_hWnd, &screen_point);
			pMenu->Init(NULL, _T("menu/chatEditViewMenu.xml"), _T(""), screen_point, &m_PaintManager, &m_chatEditViewMenuCheckInfo, pControl);

			BOOL bSel = (m_pViewEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->SetMenuItemEnable(_T("Menu_Copy"), bSel);

			IImageOle * pImageOle = NULL;
			BOOL bRet = g_RichEdtUtil->GetImageOle(m_pViewEdit, client_point, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->SetMenuItemEnable(_T("Menu_SaveAs"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pMenu->Show();
		}
		else if (pControl == m_pSendEdit)
		{
			CMenuWnd* pMenu = new CMenuWnd();
			::ClientToScreen(m_hWnd, &screen_point);
			pMenu->Init(NULL, _T("menu/chatEditInputMenu.xml"), _T(""), screen_point, &m_PaintManager, &m_chatEditInputMenuCheckInfo, pControl, eMenuAlignment_Right | eMenuAlignment_Bottom);

			BOOL bSel = (m_pSendEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->SetMenuItemEnable(_T("Menu_Cut"), bSel);
			pMenu->SetMenuItemEnable(_T("Menu_Copy"), bSel);

			BOOL bPaste = m_pSendEdit->CanPaste();
			pMenu->SetMenuItemEnable(_T("Menu_Paste"), bPaste);
			
			IImageOle * pImageOle = NULL;
			BOOL bRet = g_RichEdtUtil->GetImageOle(m_pSendEdit, client_point, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->SetMenuItemEnable(_T("Menu_SaveAs"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pMenu->Show();
		}
	}
}

void HallFrame::ChatEdit_RCut(CControlUI* pParentControl)
{
	// "输入框" 才可以剪切
	m_pSendEdit->Cut();
}

void HallFrame::ChatEdit_RCopy(CControlUI* pParentControl)
{
	if (pParentControl == m_pSendEdit)
		m_pSendEdit->Copy();
	else if (pParentControl == m_pViewEdit)
		m_pViewEdit->Copy();
}

void HallFrame::ChatEdit_RPaste(CControlUI* pParentControl)
{
	//m_pSendEdit->PasteSpecial(CF_TEXT);
	m_pSendEdit->Paste();
}

void HallFrame::ChatEdit_RSelAll(CControlUI* pParentControl)
{
	if (pParentControl == m_pSendEdit)
		m_pSendEdit->SetSel(0, -1);
	else if (pParentControl == m_pViewEdit)
		m_pViewEdit->SetSel(0, -1);
}

void HallFrame::ChatEdit_RClear(CControlUI* pParentControl)
{
	m_pViewEdit->SetText(_T(""));
	m_pViewEdit->Clear();
}

void HallFrame::ChatEdit_RZoomRatio(CControlUI* pParentControl, LPCTSTR lpMenuName)
{
	if (_tcscmp(lpMenuName, _T("Menu_ZoomRatio_200")) == 0)
		m_pViewEdit->SetZoom(8, 4);
	else if (_tcscmp(lpMenuName, _T("Menu_ZoomRatio_150")) == 0)
		m_pViewEdit->SetZoom(6, 4);
	else if (_tcscmp(lpMenuName, _T("Menu_ZoomRatio_125")) == 0)
		m_pViewEdit->SetZoom(5, 4);
	else if (_tcscmp(lpMenuName, _T("Menu_ZoomRatio_100")) == 0)
		m_pViewEdit->SetZoom(4, 4);
	else if (_tcscmp(lpMenuName, _T("Menu_ZoomRatio_75")) == 0)
		m_pViewEdit->SetZoom(3, 4);
	else
		return;
}

void HallFrame::ChatEdit_RSaveAs(CControlUI* pParentControl)
{
	if (pParentControl != m_pSendEdit && pParentControl != m_pViewEdit)
		return;

	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(pParentControl);
	IImageOle * pImageOle = nullptr;

	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	BOOL bRet = g_RichEdtUtil->GetImageOle(pRichEdit, m_pointChatROle, &pImageOle);
	if (!bRet || nullptr == pImageOle) return;

	std::wstring strFileName;
	BSTR bstrFileName = nullptr;
	HRESULT hr = pImageOle->GetFileName(&bstrFileName);
	if (SUCCEEDED(hr))
		strFileName = bstrFileName;
	if (bstrFileName != nullptr)
		::SysFreeString(bstrFileName);

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT;

	LPTSTR lpszFilter;
	LPTSTR lpszDefExt;
	LPTSTR lpszTitle = _T("保存图片");
	HWND hWndParent = m_hWnd;

	GUID guid = { 0 };
	hr = pImageOle->GetRawFormat(&guid);

	if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatJPEG))
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatPNG))
	{
		lpszDefExt = _T(".png");
		lpszFilter = _T("图像文件(*.png)\0*.png\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatGIF))
	{
		lpszDefExt = _T(".gif");
		lpszFilter = _T("图像文件(*.gif)\0*.gif\0图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}
	else
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}

	BOOL bRes = FALSE;
	TCHAR lpOutFilePath[MAX_PATH];
	memset(lpOutFilePath, 0, sizeof(TCHAR) * MAX_PATH);
	bRes = g_GameUtil->saveFileDialog(m_hWnd, lpszFilter, lpszDefExt, lpszTitle, lpOutFilePath);
	if (bRes)
	{
		ASSERT(_tcscmp(lpOutFilePath, _T("")) != 0);
		tstring strExtName = _T(".") + g_GameUtil->GetExtension(lpOutFilePath);
		GUID exGuid = g_GameUtil->GetFileExtensionGuid(strExtName.c_str());
		if (InlineIsEqualGUID(guid, exGuid))
		{
			OLECHAR* wstr = nullptr;
		#ifdef UNICODE
			wstr = StringConvertor::WideStrDup(lpOutFilePath);
		#else
			wstr = StringConvertor::AnsiToWide(lpOutFilePath);
		#endif // UNICODE

			::CopyFileW(strFileName.c_str(), wstr, FALSE);
			StringConvertor::StringFree(wstr);
		}
		else
		{
			OLECHAR* wstr = nullptr;
		#ifdef UNICODE
			wstr = StringConvertor::WideStrDup(lpOutFilePath);
		#else
			wstr = StringConvertor::AnsiToWide(lpOutFilePath);
		#endif // UNICODE

			BSTR bstrSavePath = ::SysAllocString(wstr);
			if (bstrSavePath != NULL)
			{
				pImageOle->SaveAsFile(bstrSavePath);
				::SysFreeString(bstrSavePath);
			}
			StringConvertor::StringFree(wstr);
		}
	}

	if (pImageOle != NULL)
		pImageOle->Release();
}

void HallFrame::ChatEdit_SendMsg(CDuiString& strText)
{
	std::wstring wstrText;
	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	g_RichEdtUtil->GetOleText(m_pSendEdit, wstrText);
	LPSTR strTemp = StringConvertor::WideToAnsi(wstrText.c_str());
	strText += strTemp;
	StringConvertor::StringFree(strTemp);
	
	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->Clear();
	m_pSendEdit->SetFocus();
	if (strText.GetLength() <= 0) return;
	
	g_GameUtil->CheckSensitiveVocabulary(strText, true);
	DataManager* pDataManager = DataManager::GetInstance();
	UserData uSelf = pDataManager->GetSelfData();
	CMD_GR_UserChat stuChar;
	::ZeroMemory(&stuChar, sizeof(CMD_GR_UserChat));
	stuChar.crFontColor = RGB(255, 255, 255);
	stuChar.dwSendUserID = uSelf.GetUserID();
	stuChar.dwTargetUserID = 0;
	WORD wStrLength = ::StrToCharArray(stuChar.szChatMessage, strText.GetData());
	stuChar.wChatLength = sizeof(TCHAR) * wStrLength;

	RoomManager* pRoomManager = RoomManager::GetInstance();
	WORD wSizeData = sizeof(CMD_GR_UserChat) - sizeof(stuChar.szChatMessage) + stuChar.wChatLength;
	DWORD bRes = pRoomManager->SendData(MDM_GR_USER, SUB_GR_USER_CHAT, &stuChar, wSizeData);
}

// 换肤
void HallFrame::OnChangeSkin()
{
	CDuiString pzResPath;
#ifdef SKIN_RES_TEST
	pzResPath = CPaintManagerUI::GetResourcePath() + _T("..\\");
#else
	pzResPath = CPaintManagerUI::GetResourcePath();
#endif //SKIN_RES_TEST
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;

	std::vector<FILE_STRUCT> vFiles;
	LPCTSTR lpExName = _T("");
#ifndef SKIN_RES_TEST
	lpExName = _T("dll");
#endif
	g_GameUtil->traverseFolder(pzResPath.GetData(), vFiles, FALSE, lpExName);
	if (vFiles.size() <= 1) return;
	std::vector<FILE_STRUCT> tmpFiles(vFiles);

	// 剩余文件随机
	int iTmpSkinIndex = 0;
	for (std::vector<FILE_STRUCT>::iterator& it = tmpFiles.begin(); it != tmpFiles.end(); ++it)
	{
		if (iTmpSkinIndex == GameResource::g_SkinResID)
		{
			it = tmpFiles.erase(it);
			break;
		}
		++iTmpSkinIndex;
	}

	// 保存资源路径
	int iLeftIndex = rand() % tmpFiles.size();
	pzResPath.Format(_T("%s%s"), pzResPath.GetData(), tmpFiles[iLeftIndex].fileName.c_str());
	
	// 保存临时资源文件下标
	iTmpSkinIndex = GameResource::g_SkinResID;
	for (int i = 0; i < vFiles.size(); ++i)
	{
		if (_tcscmp(vFiles[i].fileName.c_str(), tmpFiles[iLeftIndex].fileName.c_str()) == 0)
		{
			iTmpSkinIndex = i;
		}
	}

#ifdef SKIN_RES_TEST
	CPaintManagerUI::SetResourcePath(pzResPath + _T("\\"));
	CPaintManagerUI::ReloadSkin();
	GameResource::g_SkinResID = iTmpSkinIndex;
#else
	HINSTANCE hLastInst = CPaintManagerUI::GetResourceDll();
	if (g_GameUtil->LoadSkinResFromDll(pzResPath))
	{
		if (hLastInst != GameResource::g_ResourceInst)
		{
			// 释放原有dll库
			::FreeLibrary(hLastInst);

			// 换肤
			CPaintManagerUI::SetResourceDll(GameResource::g_ResourceInst);
			CPaintManagerUI::ReloadZipResource(SKINE_ZIPRE_ID);
			CPaintManagerUI::ReloadSkin();

			// 保存皮肤索引下标
			GameResource::g_SkinResID = iTmpSkinIndex;
		}
	}
#endif // SKIN_RES_TEST
}
// --------------------------------------------------------- 聊天面板右键菜单 ---------------------------------------------------------

void HallFrame::OnWkeTitleChange(const CDuiString& strTitle)
{
}

void HallFrame::OnWkeUrlChange(const CDuiString& strUrl)
{
#ifdef GAMEPLAZA_STYLE_1
	if (m_pGameWebBrowser->IsClickURL())
	{
		m_pGameWebBrowser->ResetClickURL();
		wkeWebView pWebView = m_pGameWebBrowser->GetWebView();
		pWebView->stopLoading();
		pWebView->loadURL(m_pGameWebBrowser->GetUrl().GetData());
		g_GameUtil->OpenWebByURL(strUrl.GetData());
	}
#else if GAMEPLAZA_STYLE_2
#endif
}

void HallFrame::CreatGameOptBtn(int iIndex, LPCTSTR pzText)
{
	CVerticalLayoutUI* pLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(R->kOptHallPanel_VLayout));
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
		_stprintf_s(m_pzBuffer, _T("hallpanel-opt_btn_%d"), iIndex);
		COptionUI* pOption = new COptionUI();
		pOption->SetName(m_pzBuffer);
		pOption->SetFixedHeight(30);
		pOption->SetGroup(R->kStrHallPanel_OptGroupName);
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

int HallFrame::GetGameOptSelectIndex()
{
	int iIndex = -1;
	CStdPtrArray* aOptionGroup = m_PaintManager.GetOptionGroup(R->kStrHallPanel_OptGroupName);
	if (aOptionGroup)
	{
		for (int i = 0; i < aOptionGroup->GetSize(); ++i)
		{
			COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
			if (pControl->IsSelected())
			{
				iIndex = i;
				break;
			}
		}
	}

	return iIndex;
}

void HallFrame::SelectGameOptByIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_vOptName.size()) return;
	CTabLayoutUI* hall_panle_switch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsHallPanel_Switch));
	if (hall_panle_switch)
	{
		COptionUI* pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(m_vOptName[iIndex]));
		if (pOpt)
		{
			pOpt->Selected(true);
			hall_panle_switch->SelectItem(iIndex);
		}
	}
}

void HallFrame::LayoutGameItem(CTileLayoutUI* tPanel, std::vector<CGameItemUI*>& vtGameItemArr)
{
	if (!tPanel) return;
		
	LONG offset_x = 60;					//每个icon的间隔宽度
	LONG offset_y = 50;					//每个icon的间隔高度
	::CSize icon_size(128, 128);		//每个icon的原始大小

	RECT paddingRect = { 0, 0, 0, offset_y };
	::CSize tileSize = { 0, 0 };
	tileSize.cx = icon_size.cx;
	tileSize.cy = icon_size.cy;
	tPanel->SetItemSize(tileSize);

	// 每页按wSortID排序添加
	//std::sort(vtGameItemArr.begin(), vtGameItemArr.end(), CGameItemUI::less_sort_item_id);
	for (const auto& it : vtGameItemArr)
	{
		it->SetFixedWidth(icon_size.cx);
		it->SetFixedHeight(icon_size.cy);
		it->SetPadding(paddingRect);
		tPanel->Add(it);
	}
}

LRESULT HallFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_NOTIFY:
		{
			if (EN_PASTE == ((LPNMHDR)lParam)->code)
			{
				CTxtWinHost* pTxtWinHost = m_pSendEdit->GetTxtWinHost();
				if (pTxtWinHost)
				{
					ITextServices * pTextServices = pTxtWinHost->GetTextServices();
					if ((UINT)pTextServices == ((LPNMHDR)lParam)->idFrom)
					{
						RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
						g_RichEdtUtil->AddMsgToInputEdit(m_pSendEdit, ((NMRICHEDITOLECALLBACK *)lParam)->lpszText);
					}
				}
			}

			return 0;
		}break;

		case WM_MENUCLICK:
		{
			CDuiString* strMenuName = (CDuiString*)(wParam);
			CControlUI* pControl = (CControlUI*)(lParam);
			this->OnMenuEvent(strMenuName, pControl);
			return 0;
		}break;

		case WM_UNINSTALL_USERMENU:
		{
			if (m_pRoomManager)
			{
				// 卸载玩家控制Dll
				m_pRoomManager->UnLoadUserControl();
			}
		}break;

		case DOWNLOAD_ENTER_ROOM:
		{
#ifdef GAMEPLAZA_STYLE_1
			bool bRes = true;
			IPC* ipc = IPC::GetInstance();
			bRes &= ipc->IsGameStarted() == false;
			bRes &= TLS_PANEL_ROOM_LIST == g_GameUtil->GetCurPanel();
			bRes &= m_pRoomManager != nullptr;
			bRes &= m_pRoomManager->GetCurRoomID() != 0;
			if (bRes)
			{
				m_pRoomManager->CheckGameStatus();
			}
#else if GAMEPLAZA_STYLE_2
			WORD wGameKindID = (WORD)wParam;
			this->OnGameDownloadComplete(wGameKindID);
#endif
		}break;

		case WM_COPYDATA:
		{
			this->OnCopyData((void*)lParam);
			return 0;
		}break;
		
		default:
			break;
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void HallFrame::OnCopyData(void* pData)
{
	Tls_Panel cur_panel = g_GameUtil->GetCurPanel();
	if (m_pRoomManager)
	{
		m_pRoomManager->OnNetwork_GameToServer(pData);
	}
}

void HallFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnHallMain_Close) == 0)
		{
			ExitFrame* pExitFrame = ExitFrame::GetInstance();
			pExitFrame->CreateFrame();
		}
		else if (_tcscmp(sName, R->kBtnHallMain_Skin) == 0)
		{
			this->OnChangeSkin();
		}
		else if (_tcscmp(sName, R->kBtnHallMain_Min) == 0)
		{
			this->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		else if (_tcscmp(sName, R->kBtnHallMain_Max) == 0)
		{
			this->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			return;
		}
		else if (_tcscmp(sName, R->kBtnHallMain_Restore) == 0)
		{
			this->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			return;
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_HeadIcon) == 0))
		{
			int iInfoPanel_Index = 1;
			InfoFrame* pInfoFrame = InfoFrame::GetInstance();
			pInfoFrame->CreateFrame(&m_PaintManager, iInfoPanel_Index);
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_UserInfo) == 0))
		{
			InfoFrame* pInfoFrame = InfoFrame::GetInstance();
			pInfoFrame->CreateFrame(&m_PaintManager);
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_CopyInfo) == 0))
		{
			CDuiString strInfo = _T("");
			strInfo.Format(_T("我的ID:%s,昵称:%s,请确认,谢谢!"), m_pID->GetText().GetData(), m_pNickName->GetText().GetData());

			::EmptyClipboard();
			bool bRes = g_GameUtil->SetClipBoardText(strInfo.GetData(), m_hWnd);

			if (bRes)
			{
				DialogFrame::ToastBox(&m_PaintManager, _T("复制成功!"), true);
			}
			else
			{
				DialogFrame::ToastBox(&m_PaintManager, _T("复制失败!"), true);
			}
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_OfficialWeb) == 0))
		{
			CDuiString pzUrl = g_GameUtil->GetWebByName(_T("Official"));
#ifdef GAMEPLAZA_STYLE_1
			g_GameUtil->OpenWebByURL(pzUrl.GetData());
#else if GAMEPLAZA_STYLE_2
			if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_HALL)
			{
				LPWSTR wstrURL = nullptr;
				wstrURL = StringConvertor::AnsiToWide(pzUrl.GetData());
				m_pGameWebBrowser->Navigate(wstrURL, NULL, NULL, NULL, NULL);
				StringConvertor::StringFree(wstrURL);
			}
			else
			{
				g_GameUtil->OpenWebByURL(pzUrl.GetData());
			}
#endif
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_LockMachine) == 0))
		{
			LockMachineFrame* pLockMachineFrame = LockMachineFrame::GetInstance();
			pLockMachineFrame->CreateFrame(&m_PaintManager);
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_Recharge) == 0))
		{
			CDuiString pzUrl = g_GameUtil->GetWebByName(_T("Recharge"));
			UserData& tSelf = m_pDataManager->GetSelfData();
			pzUrl.Format(_T("%s%lu"), pzUrl.GetData(), tSelf.GetGameID());

#ifdef GAMEPLAZA_STYLE_1
			g_GameUtil->OpenWebByURL(pzUrl.GetData());
#else if GAMEPLAZA_STYLE_2
			if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_HALL)
			{
				LPWSTR wstrURL = nullptr;
				wstrURL = StringConvertor::AnsiToWide(pzUrl.GetData());
				m_pGameWebBrowser->Navigate(wstrURL, NULL, NULL, NULL, NULL);
				StringConvertor::StringFree(wstrURL);
			}
			else
			{
				g_GameUtil->OpenWebByURL(pzUrl.GetData());
			}
#endif
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_UserBank) == 0))
		{
			BankFrame* bankFrame = BankFrame::GetInstance();
			bankFrame->CreateFrame(&m_PaintManager);
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_Mall) == 0))
		{
			MallFrame* mallFrame = MallFrame::GetInstance();
			mallFrame->CreateFrame();
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_CustomService) == 0))
		{
			CDuiString pzUrl = g_GameUtil->GetWebByName(_T("CustomService"));
#ifdef GAMEPLAZA_STYLE_1
			g_GameUtil->OpenWebByURL(pzUrl.GetData());
#else if GAMEPLAZA_STYLE_2
			if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_HALL)
			{
				LPWSTR wstrURL = nullptr;
				wstrURL = StringConvertor::AnsiToWide(pzUrl.GetData());
				m_pGameWebBrowser->Navigate(wstrURL, NULL, NULL, NULL, NULL);
				StringConvertor::StringFree(wstrURL);
			}
			else
			{
				g_GameUtil->OpenWebByURL(pzUrl.GetData());
			}
#endif
		}
		else if ((_tcscmp(sName, R->kBtnHallMain_Activity) == 0))
		{
			ActivityFrame* pFrame = ActivityFrame::GetInstance();
			pFrame->CreateFrame(&m_PaintManager);
		}
		else if (_tcscmp(sName, R->kBtnHallMain_Task) == 0)
		{
			//g_GameUtil->OpenWebByName(_T("Task"));
		}
		else if (_tcscmp(sName, R->kBtnSidebarHall_Sign) == 0)
		{
			SignFrame* pSignFrame = SignFrame::GetInstance();
			pSignFrame->CreateFrame(&m_PaintManager);
		}
		else if (_tcscmp(sName, R->kBtnSidebarHall_Dole) == 0)
		{
			DoleFrame* pDoleFrame = DoleFrame::GetInstance();
			pDoleFrame->CreateFrame(&m_PaintManager);
		}
		else if (_tcscmp(sName, R->kBtnChatPanel_Send) == 0)
		{
			CDuiString strText;
			ChatEdit_SendMsg(strText);
		}
		else if (_tcscmp(sName, R->kBtnChatPanel_Face) == 0)
		{
			RECT rcPos = msg.pSender->GetPos();
			RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();

			ProcFaceCtrlSel proCtrlSel = CC_CALLBACK_3(HallFrame::OnFaceCtrlSel, this);
			g_RichEdtUtil->CreatFaceFrame(m_hWnd, rcPos, proCtrlSel);
		}
		else if (_tcscmp(sName, R->kBtnHallPanel_GoBack) == 0)
		{
			if (m_pGameWebBrowser)
			{
				m_pGameWebBrowser->GoBack();
			}
		}
		else if (_tcscmp(sName, R->kBtnHallPanel_GoForward) == 0)
		{
			if (m_pGameWebBrowser)
			{
				m_pGameWebBrowser->GoForward();
			}
		}
		else if (_tcscmp(sName, R->kBtnHallPanel_GoStop) == 0)
		{
			if (m_pGameWebBrowser)
			{
#ifdef GAMEPLAZA_STYLE_1
				m_pGameWebBrowser->StopLoad();
#else if GAMEPLAZA_STYLE_2
				m_pGameWebBrowser->Stop();
#endif
			}
		}
		else if (_tcscmp(sName, R->kBtnHallPanel_GoRefresh) == 0)
		{
			if (m_pGameWebBrowser)
			{
				m_pGameWebBrowser->Refresh();
			}
		}
		else if (_tcscmp(sName, R->kBtnHallPanel_GoOfficial) == 0)
		{
			if (m_pGameWebBrowser)
			{
#ifdef GAMEPLAZA_STYLE_1
				m_pGameWebBrowser->Navigate(m_pzUrlOfficial);
#else if GAMEPLAZA_STYLE_2
				LPWSTR wstrURL = nullptr;
				wstrURL = StringConvertor::AnsiToWide(m_pzUrlOfficial.GetData());
				m_pGameWebBrowser->Navigate(wstrURL, NULL, NULL, NULL, NULL);
				StringConvertor::StringFree(wstrURL);
#endif
			}
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_GAMEITEM_CLICK) == 0)
	{
		CGameItemUI* game_item = FindGameItemByName(sName);
		if (game_item)
		{
			if (game_item->GetGameStatus() != CGameItemUI::GAME_STATE::GAME_STATE_NOT_OPEN)
			{
				g_GameUtil->SetCurPanel(&m_PaintManager, Tls_Panel::TLS_PANEL_ROOM_LIST);
				m_pRoomManager->InitStyle_1(&m_PaintManager, game_item);
			}
			else
			{
				DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示:"), game_item->GetGameStatusDesc());
			}
		}
		else
		{
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示:"), _T("进入房间错误!"));
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_RDOWN_RICHEDIT) == 0)
	{
		if (_tcscmp(sName, R->kEdtChatPanel_View) == 0)
		{
			ChatEdit_RButtonDown(msg);
		}
		else if (_tcscmp(sName, R->kEdtChatPanel_Input) == 0)
		{
			ChatEdit_RButtonDown(msg);
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_SELECTCHANGED) == 0)
	{
		CTabLayoutUI* hall_panle_switch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsHallPanel_Switch));
		for (size_t i = 0; i < m_vOptName.size(); ++i)
		{
			if (_tcscmp(sName, m_vOptName[i].GetData()) == 0 && hall_panle_switch)
			{
				hall_panle_switch->SelectItem(i);
			}
		}
	}
	
	// 消息传递房间等面板
	Tls_Panel cur_panel = g_GameUtil->GetCurPanel();
	switch (cur_panel)
	{
		case TLS_NONE:
			break;
		case TLS_PANEL_HALL:
			break;

		case TLS_PANEL_ROOM_LIST:
		case TLS_PANEL_ROOM:
		{
			m_pRoomManager->Notify(msg);
			return;
		}break;

		case TLS_PANEL_EXTRA:
			break;

		default:
			break;
	}
	

	//WindowImplBase::Notify(msg);
}

LRESULT HallFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		if (m_pSendEdit->IsFocused() && m_pSendEdit->GetText().GetLength() > 0)
		{
			CButtonUI* pSendBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnChatPanel_Send));
			if (pSendBtn)
			{
				m_PaintManager.SendNotify(pSendBtn, DUI_MSGTYPE_CLICK);
			}
		}
		return TRUE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return FALSE;
	}

	return FALSE;
}

LRESULT HallFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}

#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed)
	{
		CControlUI* pbtnMax = static_cast<CControlUI*>(m_PaintManager.FindControl(R->kBtnHallMain_Max));			// 最大化按钮
		CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(R->kBtnHallMain_Restore));	// 还原按钮

		// 切换最大化按钮和还原按钮的状态
		if (pbtnMax && pbtnRestore)
		{
			pbtnMax->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
			pbtnRestore->SetVisible(FALSE == bZoomed);
		}
	}
#else
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
	if (SC_RESTORE == (wParam & 0xfff0))
	{
		bHandled = FALSE;
	}
	return lRes;
}

UILIB_RESOURCETYPE HallFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR HallFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString HallFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString HallFrame::GetSkinFile()
{
	return _T("hall/hall_main.xml");
}

LPCTSTR HallFrame::GetWindowClassName() const
{
	return R->MainWndClassName;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 风格2

LPCTSTR HallFrame::_GetGameTypeIconPath(tagGameType* pData)
{
	ASSERT(pData);
	if (!pData) return _T("");

	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	_stprintf_s(m_pzBuffer, _T("hall\\gameicon\\type\\%u.png"), pData->wTypeID);

	CDuiString pzResPath;
	CDuiString pzTmpPath = m_pzBuffer;
#ifdef SKIN_RES_TEST
	pzResPath = CPaintManagerUI::GetResourcePath();
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;
	if (!g_GameUtil->IsFileExist(pzResPath + pzTmpPath))
#else
	if (!CRenderEngine::IsZipItemExist(pzTmpPath.GetData()))
#endif // SKIN_RES_TEST
	{
		pzTmpPath = _T("hall\\gameicon\\type\\default.png");
	}

	_stprintf_s(m_pzBuffer, _T("file='%s%s' aligncenter='true'"), pzResPath.GetData(), pzTmpPath.GetData());
	return m_pzBuffer;
}

LPCTSTR HallFrame::_GetGameKindIconPath(CGameItemUI* pData)
{
	ASSERT(pData);
	if (!pData) return _T("");

	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	if (pData->GetGameStatus() == CGameItemUI::GAME_STATE_VERSION_NORMAL || pData->GetGameStatus() == CGameItemUI::GAME_STATE_VERSION_ERROE)
	{
		_stprintf_s(m_pzBuffer, _T("hall\\gameicon\\kind\\%u.png"), pData->GetGameKind()->wKindID);
	}
	else
	{
		_stprintf_s(m_pzBuffer, _T("hall\\gameicon\\kind\\0.png"));
	}

	CDuiString pzResPath;
	CDuiString pzTmpPath = m_pzBuffer;
#ifdef SKIN_RES_TEST
	pzResPath = CPaintManagerUI::GetResourcePath();
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;
	if (!g_GameUtil->IsFileExist(pzResPath + pzTmpPath))
#else
	if (!CRenderEngine::IsZipItemExist(pzTmpPath.GetData()))
#endif // SKIN_RES_TEST
	{
		pzTmpPath = _T("hall\\gameicon\\type\\default.png");
	}
	_stprintf_s(m_pzBuffer, _T("file='%s%s' aligncenter='true'"), pzResPath.GetData(), pzTmpPath.GetData());

	return m_pzBuffer;
}

LPCTSTR HallFrame::_GetGameRoomIconPath(tagGameServer* pData)
{
	ASSERT(pData);
	if (!pData) return _T("");

	DataManager* pDataManager = DataManager::GetInstance();
	int iOnlineStatus = pDataManager->GetOnlineStatusByCount(pData->dwOnlineCount);

	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	_stprintf_s(m_pzBuffer, _T("hall\\gameicon\\online\\%u.png"), iOnlineStatus);

	CDuiString pzResPath;
	CDuiString pzTmpPath = m_pzBuffer;
#ifdef SKIN_RES_TEST
	pzResPath = CPaintManagerUI::GetResourcePath();
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;
	if (!g_GameUtil->IsFileExist(pzResPath + pzTmpPath))
#else
	if (!CRenderEngine::IsZipItemExist(pzTmpPath))
#endif // SKIN_RES_TEST
	{
		pzTmpPath = _T("hall\\gameicon\\online\\default.png");
	}
	_stprintf_s(m_pzBuffer, _T("file='%s%s' aligncenter='true'"), pzResPath.GetData(), pzTmpPath.GetData());
	return m_pzBuffer;
}

void HallFrame::_InitFrequentNode()
{
	if (m_pGameListRoot && m_pGameListTreeView)
	{
		memset(&m_pFrequentType, 0, sizeof(m_pFrequentType));
		m_pFrequentType.wSortID = 0;
		m_pFrequentType.wTypeID = 0;
		_stprintf_s(m_pFrequentType.szTypeName, _T("%s"), _T("最近常登陆游戏"));

		WORD wTypeID = m_pFrequentType.wTypeID;
		CTreeNodeUI* pFrequentNodeUI = this->AddGameTypeNode(m_pGameListTreeView, &m_pFrequentType, wTypeID);
		if (pFrequentNodeUI)
		{
			m_vGameTypeNodeUI.push_back(pFrequentNodeUI);
			for (int i = 0; i < m_vFrequentGameKinds.size(); ++i)
			{
				// 创建游戏
				WORD wKindID = m_vFrequentGameKinds[i];
				CGameItemUI* pGameItem = this->FindGameItemByKindID(wKindID);
				CTreeNodeUI* pGameKindNodeUI = this->AddGameKindNode(pFrequentNodeUI, pGameItem, wTypeID);
				if (!pGameKindNodeUI) continue;
				m_vGameKindNodeUI.push_back(pGameKindNodeUI);

				// 创建房间
				std::vector<tagGameServer*>& vGameRooms = pGameItem->GetGameRoomList();
				std::sort(vGameRooms.begin(), vGameRooms.end(), CGameItemUI::less_sort_room_id);
				for (int j = 0; j < vGameRooms.size(); ++j)
				{
					CTreeNodeUI* pGameRoomNodeUI = this->AddGameRoomNode(pGameKindNodeUI, vGameRooms[j], wTypeID);
					if (!pGameRoomNodeUI) continue;
					m_vGameRoomNodeUI.push_back(pGameRoomNodeUI);
				}
			}
		}
	} while (0);
}

bool HallFrame::FindFolderFlagByName(CDuiString pzName)
{
	for (auto& it : m_vLastFolderFlagMap)
	{
		if (_tcscmp(it, pzName) == 0)
		{
			return true;
		}
	}

	return false;
}

void HallFrame::AddFolderFlagByName(CDuiString pzName)
{
	for (auto& it : m_vLastFolderFlagMap)
	{
		if (_tcscmp(it, pzName) == 0) return;
	}

	m_vLastFolderFlagMap.push_back(pzName);
}

bool HallFrame::OnGameTypeNotify(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	return true;
}

bool HallFrame::OnGameKindNotify(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	CTreeNodeUI* pTreeNode = static_cast<CTreeNodeUI*>(pMsg->pSender);

	ASSERT(pTreeNode);
	if (!pTreeNode) return false;

	if (_tcscmp(pMsg->sType, DUI_MSGTYPE_ITEMCLICK) == 0)
	{
		CGameItemUI* pGameItem = (CGameItemUI*)(pTreeNode->GetVoidData());
		if (pGameItem && m_pGameWebBrowser)
		{
			_stprintf_s(m_pzBuffer, m_pzUrlFormat, pGameItem->GetGameKind()->wKindID);

#ifdef GAMEPLAZA_STYLE_1
			m_pGameWebBrowser->Navigate(m_pzBuffer);
			m_pGameWebBrowser->SetClientCallback(NULL);
#else if GAMEPLAZA_STYLE_2
			LPWSTR wstrURL = nullptr;
			wstrURL = StringConvertor::AnsiToWide(m_pzBuffer);
			m_pGameWebBrowser->Navigate(wstrURL, NULL, NULL, NULL, NULL);
			StringConvertor::StringFree(wstrURL);
#endif
		}
	}
	else if (_tcscmp(pMsg->sType, DUI_MSGTYPE_ITEMDBCLICK) == 0)
	{
		CGameItemUI* pGameItem = (CGameItemUI*)(pTreeNode->GetVoidData());
		if (pGameItem)
		{
			m_pRoomManager->InitStyle_2(&m_PaintManager, pGameItem);
			return this->CheckGameUpdate(pTreeNode, pGameItem, INVALID_WORD);
		}
	}

	return false;
}

bool HallFrame::OnGameRoomNotify(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	CTreeNodeUI* pTreeNode = static_cast<CTreeNodeUI*>(pMsg->pSender);
	ASSERT(pTreeNode);
	if (!pTreeNode) return false;

	if (_tcscmp(pMsg->sType, DUI_MSGTYPE_ITEMCLICK) == 0)
	{
		tagGameServer* pGameServer = (tagGameServer*)(pTreeNode->GetVoidData());
		if (pGameServer)
		{
			CGameItemUI* pGameItem = this->FindGameItemByKindID(pGameServer->wKindID);
			if (pGameItem)
			{
				WORD wKindID = pGameServer->wKindID;
				WORD wRoomID = pGameServer->wServerID;

				m_pRoomManager->InitStyle_2(&m_PaintManager, pGameItem);
				bool bRes = this->CheckGameUpdate(pTreeNode->GetParentNode(), pGameItem, wRoomID);
				if (bRes)
				{
					// 连接游戏服务器
					m_pRoomManager->EnterRoomReq(wRoomID);

					// 记录选中的房间节点名
					m_sLastRoomNodeName = pTreeNode->GetName();

					// 添加至经常登录游戏列表中
					this->AddFrequentGames(wKindID, true);
				}
			}
		}
	}

	return false;
}

bool HallFrame::CheckGameUpdate(CControlUI* pNode, CGameItemUI* pGameItem, WORD wRoomID /* = INVALID_WORD */)
{
	ASSERT(pGameItem);
	if (!pGameItem) return false;

	switch (pGameItem->GetGameStatus())
	{
		case DuiLib::CGameItemUI::GAME_STATE_NOT_OPEN:
		{
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示:"), pGameItem->GetGameStatusDesc());
		}break;
		case DuiLib::CGameItemUI::GAME_STATE_NOT_EXIST:
		{
			// 开始下载
			m_pRoomManager->Download();

			// 标记当前下载的游戏信息
			m_tCurTaggedGameInfo.wKindID	= pGameItem->GetGameKind()->wKindID;
			m_tCurTaggedGameInfo.wTypeID	= pGameItem->GetGameKind()->wTypeID;
			m_tCurTaggedGameInfo.wRoomID	= wRoomID;
			m_tCurTaggedGameInfo.pTreeNode	= static_cast<CTreeNodeUI*>(pNode);
		} break;
		case DuiLib::CGameItemUI::GAME_STATE_VERSION_ERROE:
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(RoomManager::MsgBoxProcFocus, m_pRoomManager);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示:")
				, pGameItem->GetGameStatusDesc()
				, DLG_ID(RoomManager::RoomMsgBoxID::MSG_BOX_ID_GAME_STATUS_LOW)
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb);
		} break;
		case DuiLib::CGameItemUI::GAME_STATE_VERSION_NORMAL:
		{
			return true;
		}break;

		default:
			break;
	}

	return false;
}

void HallFrame::OnGameDownloadComplete(WORD wGameKindID)
{
	CTreeNodeUI* pGameKindNode = m_tCurTaggedGameInfo.pTreeNode;
	if (pGameKindNode)
	{
		CGameItemUI* pGameItem = this->FindGameItemByKindID(m_tCurTaggedGameInfo.wKindID);
		this->RefreshGameKindNode(pGameKindNode, pGameItem);
	}
}

CTreeViewUI* HallFrame::AddSeasonRoot(CContainerUI* pParentCtl, LPCTSTR lpName)
{
	CTreeViewUI* pTreeView = nullptr;
	if (pParentCtl)
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

		pTreeView = new CTreeViewUI();
		pTreeView->SetName(lpName);
		pTreeView->SetVisibleFolderBtn(true);
		//pTreeView->SetHotItemBkColor(dwHotBkcolor);
		//pTreeView->SetSelectedItemBkColor(dwSelectedBkcolor);
		pTreeView->SetHotItemImage(_T("file='common/list/hot.png' cornor='6,6,6,6'"));
		pTreeView->SetSelectedItemImage(_T("file='common/list/select.png' cornor='6,6,6,6'"));
		pTreeView->SetSepImage(_T("common/list/sep_line.png"));
		pParentCtl->Add(pTreeView);

		// add之后,滚动条才生效
		pTreeView->EnableScrollBar(true, pTreeView->GetHorizontalScrollBar() != NULL);
	}

	return pTreeView;
}

CTreeNodeUI* HallFrame::AddGameTypeNode(CTreeViewUI* pParentView, tagGameType* pData, WORD wTypeID)
{
	CTreeViewUI* pTreeViewCtl = dynamic_cast<CTreeViewUI*>(pParentView);
	if (!pTreeViewCtl || !pData) return nullptr;

	DWORD dwTextColor	= 0xFF000000;
	DWORD dwBkcolor		= 0x00FFFFFF;
	ServiceData* pServiceData = ServiceData::GetInstance();
	GameFontColor* pFontColor = pServiceData->GetFontColorByID(4);

	if (pFontColor)
	{
		dwTextColor = pFontColor->dwTextcolor;
		dwBkcolor = pFontColor->dwBkcolor;
	}

	// 格式化资源路径
	CDuiString pzResPath = CPaintManagerUI::GetResourcePath();
	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;

	// 创建node横向布局
	CTreeNodeUI* node = new CTreeNodeUI();
	CHorizontalLayoutUI* pHoriz = node->GetTreeNodeHoriznotal();
	node->OnNotify += MakeDelegate(this, &HallFrame::OnGameTypeNotify);
	node->SetVoidData(pData);
	node->SetFixedHeight(HallFrame::g_iTypeNodeHeight);

	// 绑定名称
	_stprintf_s(m_pzBuffer, _T("game_type_%u"), wTypeID);
	node->SetName(m_pzBuffer);

	// 占位5
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 图标
	do
	{
		CDuiString pzIconPath = this->_GetGameTypeIconPath(pData);
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetName(_T("lab_icon"));
		pLabel->SetFixedWidth(22);
		pLabel->SetBkImage(pzIconPath);
		pHoriz->Add(pLabel);
	} while (0);

	// 占位5
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 标题
	do
	{
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetName(_T("lab_text"));
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("left"));
		pLabel->SetText(pData->szTypeName);
		pLabel->SetFont(1);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	} while (0);

	// 占位10
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 添加节点至父控件
	pTreeViewCtl->Add(node);

	// 设置默认控件属性
	node->SetAttribute(_T("dotlineattr"),	_T("visible=\"true\" width=\"20\""));
	node->SetAttribute(_T("itemattr"),		_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("folderattr"),	_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("checkboxattr"),	_T("visible=\"false\" width=\"1\""));
	
	// 设置折叠按钮
	CCheckBoxUI* pFolderButton = node->GetFolderButton();
	pFolderButton->SetVisible(true);
	pFolderButton->SetFixedWidth(14);

	_stprintf_s(m_pzBuffer, _T("file='%s' aligncenter='true'"), _T("hall\\gameicon\\folder_1.png"));
	pFolderButton->SetNormalImage(m_pzBuffer);

	_stprintf_s(m_pzBuffer, _T("file='%s' aligncenter='true'"), _T("hall\\gameicon\\folder_2.png"));
	pFolderButton->SetSelectedImage(m_pzBuffer);

	return node;
}

CTreeNodeUI* HallFrame::AddGameKindNode(CTreeNodeUI* pParentNode, CGameItemUI* pData, WORD wTypeID)
{
	if (!pParentNode || !pData) return nullptr;

	DWORD dwTextColor = 0xFFFFFFFF;
	ServiceData* pServiceData = ServiceData::GetInstance();
	GameFontColor* pFontColor = pServiceData->GetFontColorByID(5);

	// 格式化资源路径
	CDuiString pzResPath = CPaintManagerUI::GetResourcePath();
	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;

	if (pFontColor)
	{
		dwTextColor = pFontColor->dwTextcolor;
	}

	// 创建node节点
	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	CTreeNodeUI* node = new CTreeNodeUI();
	CHorizontalLayoutUI* pHoriz = node->GetTreeNodeHoriznotal();
	node->OnNotify += MakeDelegate(this, &HallFrame::OnGameKindNotify);
	node->SetVoidData(pData);
	node->SetFixedHeight(HallFrame::g_iGameNodeHeight);

	// 绑定名称
	_stprintf_s(m_pzBuffer, _T("game_t_%u_k_%u"), wTypeID, pData->GetGameKind()->wKindID);
	node->SetName(m_pzBuffer);

	// 占位5
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 图标
	do
	{
		CDuiString pzIconPath = this->_GetGameKindIconPath(pData);
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetName(_T("lab_icon"));
		pLabel->SetFixedWidth(22);
		pLabel->SetBkImage(pzIconPath);
		pHoriz->Add(pLabel);
	} while (0);

	// 占位5
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 游戏状态
	do
	{
		CDuiString pzGameStatus;
		switch (pData->GetGameStatus())
		{
			case CGameItemUI::GAME_STATE::GAME_STATE_NOT_OPEN:		pzGameStatus = _T(" (暂未开放)");	break;
			case CGameItemUI::GAME_STATE::GAME_STATE_NOT_EXIST:		pzGameStatus = _T(" (双击下载)");	break;
			case CGameItemUI::GAME_STATE::GAME_STATE_VERSION_ERROE:										break;
			case CGameItemUI::GAME_STATE::GAME_STATE_VERSION_NORMAL:									break;
			default:																					break;
		}
		_stprintf_s(m_pzBuffer, _T("%s%s"), pData->GetGameKind()->szKindName, pzGameStatus.GetData());
		
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetName(_T("lab_status"));
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("left"));
		pLabel->SetText(m_pzBuffer);
		pLabel->SetFont(1);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	} while (0);

	// 添加节点至父控件
	pParentNode->Add(node);

	// 设置默认控件属性
	node->SetAttribute(_T("dotlineattr"),	_T("visible=\"true\" width=\"44\""));
	node->SetAttribute(_T("itemattr"),		_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("folderattr"),	_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("checkboxattr"),	_T("visible=\"false\" width=\"1\""));
	
	// 设置折叠按钮
	CCheckBoxUI* pFolderButton = node->GetFolderButton();
	pFolderButton->SetVisible(true);
	pFolderButton->SetFixedWidth(14);

	_stprintf_s(m_pzBuffer, _T("file='%s' aligncenter='true'"), _T("hall\\gameicon\\folder_1.png"));
	pFolderButton->SetNormalImage(m_pzBuffer);

	_stprintf_s(m_pzBuffer, _T("file='%s' aligncenter='true'"), _T("hall\\gameicon\\folder_2.png"));
	pFolderButton->SetSelectedImage(m_pzBuffer);

	return node;
}

CTreeNodeUI* HallFrame::AddGameRoomNode(CTreeNodeUI* pParentNode, tagGameServer* pData, WORD wTypeID)
{
	if (!pParentNode || !pData) return nullptr;

	DWORD dwTextColor = 0xFFFFFFFF;
	ServiceData* pServiceData = ServiceData::GetInstance();
	GameFontColor* pFontColor = pServiceData->GetFontColorByID(5);

	// 格式化资源路径
	CDuiString pzResPath = CPaintManagerUI::GetResourcePath();
	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	::PathCanonicalize(m_pzBuffer, pzResPath.GetData());
	pzResPath = m_pzBuffer;

	if (pFontColor)
	{
		dwTextColor = pFontColor->dwTextcolor;
	}

	// 创建node节点
	::memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	CTreeNodeUI* node = new CTreeNodeUI();
	CHorizontalLayoutUI* pHoriz = node->GetTreeNodeHoriznotal();
	node->OnNotify += MakeDelegate(this, &HallFrame::OnGameRoomNotify);
	node->SetVoidData(pData);
	node->SetFixedHeight(HallFrame::g_iRoomNodeHeight);

	// 绑定名称
	_stprintf_s(m_pzBuffer, _T("game_t_%u_k_%u_r_%u"), wTypeID, pData->wKindID, pData->wServerID);
	node->SetName(m_pzBuffer);

	// 占位5
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 图标
	do
	{
		CDuiString pzIconPath = this->_GetGameRoomIconPath(pData);
		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetFixedWidth(22);
		pLabel->SetBkImage(pzIconPath);
		pHoriz->Add(pLabel);
	} while (0);

	// 占位5
	do
	{
		CControlUI* pControl = new CControlUI();
		pControl->SetFixedWidth(5);
		pHoriz->Add(pControl);
	} while (0);

	// 房间名(人数状态)
	do
	{
		DataManager* pDataManager = DataManager::GetInstance();
		CDuiString strDesc = pDataManager->GetOnlineDescByCount(pData->dwOnlineCount);
		if (strDesc.GetLength() <= 0)
		{
			strDesc = _T("正常");
		}
		_stprintf_s(m_pzBuffer, _T("%s(%s)"), pData->szServerName, strDesc);

		CLabelUI* pLabel = new CLabelUI();
		pLabel->SetEndEllipsis(true);
		pLabel->SetAlign(_T("left"));
		pLabel->SetText(m_pzBuffer);
		pLabel->SetFont(1);
		pLabel->SetTextColor(dwTextColor);
		pHoriz->Add(pLabel);
	} while (0);

	// 添加节点至父控件
	pParentNode->Add(node);

	// 设置默认控件属性
	node->SetAttribute(_T("dotlineattr"),	_T("visible=\"true\" width=\"58\""));
	node->SetAttribute(_T("itemattr"),		_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("folderattr"),	_T("visible=\"false\" width=\"1\""));
	node->SetAttribute(_T("checkboxattr"),	_T("visible=\"false\" width=\"1\""));

	return node;
}

void HallFrame::RefreshGameTypeNode(CTreeNodeUI* pNode, tagGameType* pData)
{
	ASSERT(pNode);
	if (!pNode) return;

	// 图标
	do
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(pNode->FindSubControl(_T("lab_icon")));
		if (pLabel)
		{
			CDuiString pzIconPath = this->_GetGameTypeIconPath(pData);
			pLabel->SetBkImage(pzIconPath);
		}
	} while (0);


	// 文本描述
	do
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(pNode->FindSubControl(_T("lab_text")));
		if (pLabel)
		{
			pLabel->SetText(pData->szTypeName);
		}
	} while (0);
}

void HallFrame::RefreshGameKindNode(CTreeNodeUI* pNode, CGameItemUI* pData)
{
	ASSERT(pNode);
	if (!pNode) return;

	// 图标
	do
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(pNode->FindSubControl(_T("lab_icon")));
		if (pLabel)
		{
			CDuiString pzIconPath = this->_GetGameKindIconPath(pData);
			pLabel->SetBkImage(pzIconPath);
		}
	} while (0);


	// 文本描述
	do
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(pNode->FindSubControl(_T("lab_status")));
		if (pLabel)
		{
			CDuiString pzGameStatus;
			switch (pData->GetGameStatus())
			{
				case CGameItemUI::GAME_STATE::GAME_STATE_NOT_OPEN:		pzGameStatus = _T(" (暂未开放)");	break;
				case CGameItemUI::GAME_STATE::GAME_STATE_NOT_EXIST:		pzGameStatus = _T(" (双击下载)");	break;
				case CGameItemUI::GAME_STATE::GAME_STATE_VERSION_ERROE:										break;
				case CGameItemUI::GAME_STATE::GAME_STATE_VERSION_NORMAL:									break;
				default:																					break;
			}
			_stprintf_s(m_pzBuffer, _T("%s%s"), pData->GetGameKind()->szKindName, pzGameStatus.GetData());
			pLabel->SetText(m_pzBuffer);
		}
	} while (0);
}

void HallFrame::RefreshGameRoomNode(CTreeNodeUI* pNode, tagGameServer* pData)
{
	ASSERT(pNode);
	if (!pNode) return;

	// 图标
	do
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(pNode->FindSubControl(_T("lab_icon")));
		if (pLabel)
		{
			CDuiString pzIconPath = this->_GetGameRoomIconPath(pData);
			pLabel->SetBkImage(pzIconPath);
		}
	} while (0);

	// 文本描述
	do
	{
		CLabelUI* pLabel = static_cast<CLabelUI*>(pNode->FindSubControl(_T("lab_text")));
		if (pLabel)
		{
			DataManager* pDataManager = DataManager::GetInstance();
			CDuiString strDesc = pDataManager->GetOnlineDescByCount(pData->dwOnlineCount);
			if (strDesc.GetLength() <= 0)
			{
				strDesc = _T("正常");
			}
			_stprintf_s(m_pzBuffer, _T("%s(%s)"), pData->szServerName, strDesc);

			pLabel->SetText(m_pzBuffer);
		}
	} while (0);
}

void HallFrame::AddFrequentGames(WORD wKindID, bool bAddToFirst /* = false */)
{
	RemoveFrequentGames(wKindID);

	// 添加id
	if (bAddToFirst)
	{
		m_vFrequentGameKinds.insert(m_vFrequentGameKinds.begin(), wKindID);
	}
	else
	{
		m_vFrequentGameKinds.push_back(wKindID);
	}

	// 删除多余id
	std::vector<WORD>::iterator it = m_vFrequentGameKinds.begin();
	for (int i = 0; it != m_vFrequentGameKinds.end(); ++it, ++i)
	{
		if (i == m_wMaxFrequentCount) break;
	}
	m_vFrequentGameKinds.erase(it, m_vFrequentGameKinds.end());
}

bool HallFrame::RemoveFrequentGames(WORD wKindID)
{
	std::vector<WORD>::iterator& it = m_vFrequentGameKinds.begin();
	for (; it != m_vFrequentGameKinds.end();)
	{
		if (wKindID == *it)
		{
			it = m_vFrequentGameKinds.erase(it);
			return true;
		}

		++it;
	}

	return false;
}

void HallFrame::ClearFrequentGames()
{
	m_vFrequentGameKinds.clear();
}

void HallFrame::ReadFrequentGamesConfig()
{
	DataManager* pDataMgr = DataManager::GetInstance();
	UserData& tSelfData = pDataMgr->GetSelfData();
	_stprintf_s(m_pzBuffer, _T("user\\%s\\GAMESETTING"), tSelfData.GetAccount());
	JsonFile* pJsonFile = new JsonFile(m_pzBuffer);

	Json::Value vRoot;
	bool bRes = pJsonFile->initRootNode(vRoot);
	SAFE_DELETE(pJsonFile);

	if (bRes)
	{
		if (!vRoot["FrequentGames"].empty() && vRoot["FrequentGames"].isArray())
		{
			this->ClearFrequentGames();
			for (int i = 0; i < vRoot["FrequentGames"].size(); ++i)
			{
				Json::Value vNode = vRoot["FrequentGames"][i];
				if (!vNode["wKindID"].empty() && vNode["wKindID"].isUInt())
				{
					WORD wKindID = vNode["wKindID"].asUInt();
					this->AddFrequentGames(wKindID);
				}
			}
		}
	}
}

void HallFrame::SaveFrequentGamesConfig()
{
	g_Logger->TryInfo(_T("HallFrame - Save FrequentGames Config"));

	DataManager* pDataMgr = DataManager::GetInstance();
	UserData& tSelfData = pDataMgr->GetSelfData();
	_stprintf_s(m_pzBuffer, _T("user\\%s\\GAMESETTING"), tSelfData.GetAccount());
	JsonFile* pJsonFile = new JsonFile(m_pzBuffer);

	Json::Value vRoot;
	bool bRes = pJsonFile->initRootNode(vRoot);
	if (bRes)
	{
		if (!vRoot["FrequentGames"].isArray())
		{
			vRoot["FrequentGames"].clear();
			//vRoot.removeMember("FrequentGames");
		}
	}

	// 添加本次常登陆游戏id
	Json::Value vFrequentGamesNode;
	for (auto& it : m_vFrequentGameKinds)
	{
		Json::Value vNode;
		vNode["wKindID"] = it;
		vFrequentGamesNode["FrequentGames"].append(vNode);

		// 清除重复id
		for (int i = 0; i < vRoot["FrequentGames"].size();)
		{
			Json::Value& vTmpNode = vRoot["FrequentGames"][i];
			if (!vTmpNode["wKindID"].isUInt()) continue;
			DWORD wKindID = vTmpNode["wKindID"].asUInt();
			if (wKindID == it)
			{
				vRoot["FrequentGames"].removeIndex(i, &vTmpNode);
			}
			else
			{
				++i;
			}
		}
	}

	// 添加所有常登陆游戏id
	for (int i = 0; i < vRoot["FrequentGames"].size(); ++i)
	{
		Json::Value& vNode = vRoot["FrequentGames"][i];
		vFrequentGamesNode["FrequentGames"].append(vNode);
	}

	// 只保留规定数量的常登陆游戏id(去除多余的)
	for (int i = m_wMaxFrequentCount; i < vFrequentGamesNode["FrequentGames"].size();)
	{
		Json::Value& vNode = vRoot["FrequentGames"][i];
		vFrequentGamesNode["FrequentGames"].removeIndex(i, &vNode);
	}

	pJsonFile->SaveFile(vFrequentGamesNode);
	SAFE_DELETE(pJsonFile);

	g_Logger->TryInfo(_T("HallFrame - Save FrequentGames Config Complete"));
}

void HallFrame::SaveSkinConfig()
{
	DataManager* pDataMgr = DataManager::GetInstance();
	UserData& tSelf = pDataMgr->GetSelfData();
	LPCTSTR pzAccount = tSelf.GetAccount();
	g_GameUtil->SaveSkinConfig(pzAccount);
}

void HallFrame::SaveLocalData()
{
	this->SaveSkinConfig();
	this->SaveFrequentGamesConfig();
}