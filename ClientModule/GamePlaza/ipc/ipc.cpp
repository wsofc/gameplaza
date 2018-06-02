#include "stdafx.h"
#include "ipc.h"
#include "../room/roomManager.h"
#include "../room/tableManager.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

IPC* IPC::g_instance = nullptr;

IPC::IPC()
{
	this->Reset();
}

IPC::~IPC()
{
}

IPC* IPC::GetInstance()
{
	if (!IPC::g_instance)
	{
		IPC::g_instance = new (std::nothrow)IPC();
	}
	return IPC::g_instance;
}

void IPC::Reset()
{
	m_hWnd			= NULL;
	m_hTargetWnd	= NULL;
	m_bConnect		= false;
	m_bGameFinished = false;
	m_bHeartbeat	= false;
	m_dwHeartOut	= 0;
	m_dwMaxHeartOut = 0;
	m_pManager		= nullptr;

	::ZeroMemory(m_strBuffer,			MAX_PATH * sizeof(TCHAR));
	::ZeroMemory(&m_tTargetProcessInfo,	sizeof(m_tTargetProcessInfo));
	::ZeroMemory(&m_tNestedProcessInfo, sizeof(m_tNestedProcessInfo));
	::ZeroMemory(&m_IPCBuf,				IPC_BUFFER);
}

BOOL CALLBACK IPC::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessID = 0;
	::GetWindowThreadProcessId(hWnd, &dwProcessID);
	LPWNDINFO pInfo = (LPWNDINFO)lParam;
	
	if (dwProcessID == pInfo->dwProcessID)
	{
		pInfo->hWnd = hWnd;

		//退出枚举(无错误)
		::SetLastError(0);
		return FALSE;
	}

	//继续枚举
	return TRUE;
}

HWND IPC::GetWindowHandleByPID(DWORD dwProcessID)
{
	WNDINFO wi;
	wi.dwProcessID = dwProcessID;
	wi.hWnd = NULL;
	::EnumWindows(EnumWindowsProc, (LPARAM)&wi);
	return wi.hWnd;
}

void IPC::StartGames(CPaintManagerUI* pManager, LPCTSTR lpGameName, LPCTSTR lpModelName)
{
	bool bTestModel = false;
#ifdef GAME_TEST_SWITCH
	bTestModel = true;
#endif // GAME_TEST_SWITCH

	m_bConnect = _CreateGameProcess(pManager, lpGameName, lpModelName);
	g_Logger->TryInfo(_T("IPC - StartGames [Res = %d] [name = %s] [model = %s] [test = %d]"), m_bConnect, lpGameName, lpModelName, bTestModel);
	
	if (!m_bConnect)
	{
		g_Logger->TryInfo(_T("IPC - StartGames Failed"));

		DialogCallBack dlg_cb = CC_CALLBACK_2(IPC::MsgBoxProc, this);
		DialogFrame::MsgBox(pManager
			, _T("温馨提示")
			, _T("启动游戏失败")
			, DLG_ID(IPCMsgBoxID::MSG_BOX_ID_START_GAME_FAILED)
			, DLG_STYLE::DLG_OK_CANCEL
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else
	{
#ifdef GAME_TEST_SWITCH
		// 测试模拟启动完成
		CMD_Command cmd;
		cmd.wMainCmdID = IPC_MAIN_CONCTROL;
		cmd.wSubCmdID = IPC_SUB_START_FINISH;
		LocalRecvData(cmd, 0, 0);
#else
		// 正式启动完成
		// 大厅窗口最小化
		::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);

		// 开启心跳检测
		this->CloseHeartbeat();
		this->StartHeartbeat(TIME_HEART_MAX_OUT);
#endif // GAME_TEST_SWITCH
	}
}

void IPC::StartGamesInfo()
{
	// 上行服务器:游戏启动完毕(服务器会下发场景初始化消息)
	g_Logger->TryInfo(_T("IPC - Send to Server GameInfo"));

	CMD_GF_Info stuInfo;
	stuInfo.bAllowLookon = FALSE;

	RoomManager* roomManager = RoomManager::GetInstance();
	DWORD bRes = roomManager->SendData(MDM_GF_FRAME, SUB_GF_INFO, &stuInfo, sizeof(CMD_GF_Info));
	if (FALSE == bRes)
	{
		// DialogFrame::MsgBox(m_pManager, _T("温馨提示:"), _T("获取游戏数据失败!"));
		g_Logger->TryError(_T("IPC - Send to Server GameInfo - Failed"));
	}

#ifdef GAME_TEST_SWITCH
	//捕鱼类游戏自动模拟READY消息
	RoomManager* pRoomManager = RoomManager::GetInstance();
	Room* pCurRoom = pRoomManager->GetCurRoom();
	if (pCurRoom)
	{
		//捕鱼类游戏自动模拟READY消息
		const WORD wGameKind = pCurRoom->GetKindID();
		if (wGameKind == 30 || wGameKind == 31)
		{
			RoomManager* roomManager = RoomManager::GetInstance();
			DWORD bRes = roomManager->SendData(MDM_GF_FRAME, SUB_GF_USER_READY);
			if (FALSE == bRes)
			{
				DialogFrame::MsgBox(m_pManager, _T("温馨提示:"), _T("游戏准备请求失败!"));
			}
		}
	}
#endif // GAME_TEST_SWITCH
}

bool IPC::IsGameStarted()
{
	return m_bConnect;
}

bool IPC::IsGameFinished()
{
	return m_bGameFinished;
}

void IPC::MsgBoxProc(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case IPCMsgBoxID::MSG_BOX_ID_START_GAME_FAILED:
		{
			RoomManager* pRoomManager = RoomManager::GetInstance();
			Room* pCurRoom = pRoomManager->GetCurRoom();
			pCurRoom->CloseGameNotify();

			this->DisConnect(false);
		}break;

		default:
			break;
	}
}

bool IPC::_CreateGameProcess(CPaintManagerUI* pManager, LPCTSTR lpGameName, LPCTSTR lpModelName)
{
	m_pManager	= pManager;
	m_hWnd		= m_pManager->GetPaintWindow();
	if (!pManager || FALSE == ::IsWindow(m_hWnd)) return false;
	
	// 测试启动
#ifdef GAME_TEST_SWITCH
	// 是否开启Winform窗口嵌套模式
	#ifdef WINFORM_NESTED_SWITCH
		m_hTargetWnd = ::FindWindow(NULL, R->WinFormTestTitleName);
		if (TRUE == ::IsWindow(m_hTargetWnd))
		{
			int lpPid = 0;
			::GetWindowThreadProcessId(m_hTargetWnd, (LPDWORD)&lpPid);
			m_tTargetProcessInfo.dwProcessId = lpPid;
			m_tTargetProcessInfo.hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpPid);
			if (!m_tTargetProcessInfo.hProcess)
			{
				g_Logger->TryInfo(_T("IPC - Game Test Open Winform Process Failed [e_code = %d]"), ::GetLastError());
				return FALSE;
			}

			g_Logger->TryInfo(_T("IPC - Game Test Open Winform Successful"));
			return TRUE;
		}
	#else
		// 常规游戏测试模式
		CDuiString pzWindowName;
		CDuiString pzGameView;
		CDuiString pzGameName;
		TCHAR pzBuff[MAX_PATH] = { 0 };
		do
		{
			g_Logger->TryInfo(_T("IPC - Game Test Read Config"));
			_stprintf_s(pzBuff, _T("%s%s"), CPaintManagerUI::GetInstancePath().GetData(), _T("GAMETEST.config"));

			Json::Value vRoot;
			JsonFile* pFile = new JsonFile(pzBuff, true);
			bool bRes = pFile->initRootNode(vRoot, false);
			SAFE_DELETE(pFile);
			if (bRes)
			{
				if (vRoot["pzWindowName"].isString())
				{
					pzWindowName = vRoot["pzWindowName"].asCString();
				}

				if (vRoot["pzGameView"].isString())
				{
					pzGameView = vRoot["pzGameView"].asCString();
				}

				if (vRoot["bFixGameName"].isBool() && vRoot["bFixGameName"].asBool())
				{
					if (vRoot["pzGameName"].isString())
					{
						pzGameName = vRoot["pzGameName"].asCString();
					}
				}
			}

			g_Logger->TryInfo(_T("IPC - Game Test Read Config Complete [bRes = %u]"), bRes);
		} while (0);

		if (pzWindowName && pzGameView)
		{
			if (pzGameName.GetLength() <= 0)
			{
				pzGameName = lpGameName;
			}
			_stprintf_s(pzBuff, pzWindowName.GetData(), pzGameName.GetData());
		}

		g_Logger->TryInfo(_T("IPC - Game Test [WindowName = %s]"), pzBuff);
		HWND hEditParent = ::FindWindow(NULL, pzBuff);
		if (TRUE == ::IsWindow(hEditParent))
		{
			g_Logger->TryInfo(_T("IPC - Game Test [GameView = %s]"), pzGameView);
			m_hTargetWnd = ::FindWindowEx(hEditParent, NULL, NULL, pzGameView);

			if (TRUE == ::IsWindow(m_hTargetWnd))
			{
				int lpPid = 0;
				::GetWindowThreadProcessId(m_hTargetWnd, (LPDWORD)&lpPid);
				m_tTargetProcessInfo.dwProcessId = lpPid;
				m_tTargetProcessInfo.hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpPid);
				if (!m_tTargetProcessInfo.hProcess)
				{
					g_Logger->TryInfo(_T("IPC - Game Test OpenProcess Failed [e_code = %d]"), ::GetLastError());
					return FALSE;
				}

				g_Logger->TryInfo(_T("IPC - Game Test Open Successful"));
				return TRUE;
			}
		}
	#endif // WINFORM_NESTED_SWITCH
	g_Logger->TryInfo(_T("IPC - Game Test Open Failed"));
	return FALSE;
#endif // GAME_TEST_SWITCH

	// 正式启动
	if ((m_tTargetProcessInfo.hProcess != NULL) && (WaitForSingleObject(m_tTargetProcessInfo.hProcess, 0) == WAIT_TIMEOUT))
	{
		g_Logger->TryInfo(_T("IPC - process already exists"));

		// 激活进程
		return true;
	}
		
	// 格式化路径
	CDuiString szFilePath;
	CDuiString szModelName;

#ifdef WINFORM_NESTED_SWITCH
	szModelName = R->WinFormTestModelName;
#else
	szModelName = lpModelName;
#endif // WINFORM_NESTED_SWITCH

	::ZeroMemory(m_strBuffer, MAX_PATH * sizeof(TCHAR));
	if (m_pManager)
	{
		szFilePath += CPaintManagerUI::GetInstancePath();
	}
	szFilePath += szModelName;

	if (FALSE == ::PathCanonicalize(m_strBuffer, szFilePath.GetData()))
	{
		g_Logger->TryInfo(_T("IPC - Create Process Failed, Path Error [path = %s, real_path = %s, ecode = %lu]"), szFilePath.GetData(), m_strBuffer, ::GetLastError());
		return false;
	}
	szFilePath = m_strBuffer;

	// 启动进程
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOWMAXIMIZED;
	TCHAR szCommomLine[MAX_PATH] = { 0 };

#ifdef WINFORM_NESTED_SWITCH
	_stprintf_s(szCommomLine, _T("/Transmittal_Begin[%I64d,%s,%s]/Transmittal_End"), (int64)m_hWnd, lpGameName, lpModelName);
#else 
	_stprintf_s(szCommomLine, _T("/Transmittal_Begin[%I64d,%u,%u]/Transmittal_End"), (int64)m_hWnd, 1280, 720);
#endif // WINFORM_NESTED_SWITCH
	
	BOOL bSuccess = ::CreateProcess(szFilePath, szCommomLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &m_tTargetProcessInfo);
	if (TRUE == bSuccess)
	{
		// 此时主程序等待子进程进行初始化  
		//::WaitForInputIdle(m_tTargetProcessInfo.hProcess, 1000);
		return true;
	}
	else
	{
		g_Logger->TryInfo(_T("IPC - Create Game Process Failed, Create Error [path = %s, ecode = %lu]"), szFilePath.GetData(), ::GetLastError());

		::ZeroMemory(&m_tTargetProcessInfo, sizeof(m_tTargetProcessInfo));
		return false;
	}
}

void IPC::_CloseGameProcess()
{
	// 关闭目标进程
#ifndef GAME_TEST_SWITCH
	if (m_tTargetProcessInfo.hProcess != NULL)
	{
		::TerminateProcess(m_tTargetProcessInfo.hProcess, 0);
		if (m_tTargetProcessInfo.hThread != NULL)
		{
			::CloseHandle(m_tTargetProcessInfo.hThread);
		}
		if (m_tTargetProcessInfo.hProcess != NULL)
		{
			::CloseHandle(m_tTargetProcessInfo.hProcess);
		}
	}
#endif // !GAME_TEST_SWITCH

	// Winform框架模式下:关闭游戏进程
#ifdef WINFORM_NESTED_SWITCH
	HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, m_tNestedProcessInfo.dwProcessId);
	::TerminateProcess(hProcess, 0);
	::CloseHandle(hProcess);
#endif // WINFORM_NESTED_SWITCH
}

void IPC::DisConnect(bool bShowTips /* = true */)
{
	g_Logger->TryInfo(_T("IPC - DisConnect [bShowTips = %u]"), bShowTips);

	// 关闭心跳定时器(若已关闭,不影响)
	this->CloseHeartbeat();
	
	// 关闭加载游戏步骤计时器
	this->CloseStepTimer();

	// 弹窗
	if (bShowTips && m_pManager)
	{
		CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(R->kEdtChatPanel_View));
		if (pViewEdit)
		{
			g_GameUtil->SendSystemMsgText(pViewEdit, _T("您已成功退出游戏!"));
		}
	}

	// 关闭游戏进程(置空连接状态)
	this->_CloseGameProcess();

	// 大厅窗口复原
	g_GameUtil->RestoreTopWindow(m_hWnd);

	// 重置数据状态等
	this->Reset();

	g_Logger->TryInfo(_T("IPC - DisConnect Complete"));
}

void IPC::StartHeartbeat(int iMaxHeartOutCount)
{
	g_Logger->TryInfo(_T("IPC - Start Heartbeat"));
	if (::IsWindow(m_hWnd))
	{
		m_bHeartbeat = true;
		m_dwMaxHeartOut = iMaxHeartOutCount > 0 ? iMaxHeartOutCount : TIME_HEART_MIN_OUT;

		TimerCallBack timepro = CC_CALLBACK_2(IPC::OnHeartbeat, this);
		g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_HEARTBEAT, TIME_HEARTBEAT);
	}

	CMD_Command cmd;
	cmd.wMainCmdID = IPC_MAIN_CONCTROL;
	cmd.wSubCmdID = IPC_SUB_HEARTBEAT;
	this->LocalSendData(cmd, nullptr, 0);
}

void IPC::CloseHeartbeat()
{
	g_Logger->TryInfo(_T("IPC - Close Heartbeat"));

	if (TRUE == ::IsWindow(m_hWnd))
	{
		g_GameUtil->KillTimer(m_hWnd, TIMER_ID_HEARTBEAT);
	}

	m_bHeartbeat = false;
	m_dwHeartOut = 0;
	m_dwMaxHeartOut = 0;
}

void IPC::OnHeartbeat(UINT_PTR idEvent, DWORD dwTime)
{
	if (m_bHeartbeat)
	{
		m_bHeartbeat = false;

		CMD_Command cmd;
		cmd.wMainCmdID = IPC_MAIN_CONCTROL;
		cmd.wSubCmdID = IPC_SUB_HEARTBEAT;
		this->LocalSendData(cmd, nullptr, 0);
	}
	else
	{
		// 超时则断开
		if (++m_dwHeartOut >= m_dwMaxHeartOut)
		{
			g_Logger->TryInfo(_T("IPC Connect Game Time Out"));

			m_dwHeartOut = 0;

			// 关闭心跳定时器
			g_GameUtil->KillTimer(m_hWnd, TIMER_ID_HEARTBEAT);

			// 提示断线对话框
			DialogFrame::MsgBox(m_pManager, _T("温馨提示!"), _T("游戏异常关闭!"));
						
			// 发送关闭游戏消息
			CMD_Command cmd;
			cmd.wMainCmdID = IPC_MAIN_CONCTROL;
			cmd.wSubCmdID = IPC_SUB_CLOSE_FRAME_EX;
			this->LocalRecvData(cmd, nullptr, 0);

			g_Logger->TryInfo(_T("Game Close_Frame_Ex Local Req"));
		}
		else
		{
			m_bHeartbeat = true;
		}
	}
}

void IPC::_SendListHeadData()
{
	CMD_Command cmd;
	cmd.wMainCmdID = IPC_MAIN_USER;
	cmd.wSubCmdID = IPC_SUB_USER_CONFIG;

	UserList* pList = UserList::GetInstance();
	DataManager* pDataMgr = DataManager::GetInstance();
	UserData& tSelf = pDataMgr->GetSelfData();

	int iHeadItemCount = pList->GetHeadItemCount();
	if (iHeadItemCount > 0)
	{
		DWORD dwNextKey = 0;
		tagColumnItem* pHeadItemArray = new tagColumnItem[iHeadItemCount];
		for (int i = 0; i < iHeadItemCount; ++i)
		{
			pHeadItemArray[i] = *pList->EnumHeadItem(dwNextKey);
		}
		this->LocalSendData(cmd, pHeadItemArray, iHeadItemCount * sizeof(tagColumnItem));
	}
}

void IPC::_SendListBodyData()
{
	UserList* pList = UserList::GetInstance();
	std::vector<UserData*> vUserArray = pList->GetListBodyArray();
	if (vUserArray.size() <= 0)
	{
		this->m_bGameFinished = false;
		return;
	}

	// 发送玩家数据
	CMD_Command cmd;
	cmd.wMainCmdID = IPC_MAIN_USER;
	cmd.wSubCmdID = IPC_SUB_USER_COME;

	// 追加隐藏的管理员用户等
	DWORD dwNextKey = 0;
	DataManager* pDataMgr = DataManager::GetInstance();
	UserData& tSelf = pDataMgr->GetSelfData();
	for (int i = 0; i < pDataMgr->GetUserDataCount(); ++i)
	{
		UserData* pUser = pDataMgr->EnumUserDataMap(dwNextKey);
		if (pUser->GetMasterOrder() > tSelf.GetMasterOrder())
		{
			vUserArray.push_back(pUser);
		}
	}

	// 批量单个发送
	BYTE cbBuffer[IPC_BUFFER] = { 0 };
	for (size_t i = 0; i < vUserArray.size(); ++i)
	{
		UserData* pUser = vUserArray[i];
		::memset(&cbBuffer, 0, ArrayCount(cbBuffer) * sizeof(BYTE));

		// 筛选同桌(且有效桌)玩家下发至游戏
		if (pUser->GetTableID() == tSelf.GetTableID() && pUser->GetTableID() != INVALID_TABLE)
		{
			UserStruct& uStruct = pUser->GetUserStruct();
			::memcpy(cbBuffer, &uStruct, sizeof(UserStruct));

			// 附加用户结构
			UserStructSuffix uStructSuffix = pUser->GetUserStructSuffix();
			CSendPacketHelper Packet(cbBuffer + sizeof(UserStruct), sizeof(cbBuffer) - sizeof(UserStruct));
			Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);

			// 下发玩家数据
			this->LocalSendData(cmd, cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
		}
	}
}

void IPC::OnStepTimer(UINT_PTR idEvent, DWORD dwTime)
{
	switch (idEvent)
	{
		// 列表head(下发游戏:配置用户列表head信息(若开启WinForm框架嵌套,则先下发列表头初始化消息))
		case TIMER_ID_START_GAME_FINISH:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Send List Head"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH);

			this->_SendListHeadData();
		}break;

		// 列表body(下发游戏:用户comming)
		case TIMER_ID_START_GAME_FINISH + 1:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Send List Body"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH + 1);

#ifndef WINFORM_NESTED_SWITCH
			// 启动完成才能获取到游戏窗口句柄
			this->m_hTargetWnd = GetWindowHandleByPID(this->m_tTargetProcessInfo.dwProcessId);
			if (FALSE == ::IsWindow(this->m_hTargetWnd))
			{
				g_Logger->TryInfo(_T("IPC - StartGames Failed Not Find Window Handle [ecode = %lu]"), ::GetLastError());

				// 启动游戏失败,返回
				this->m_bGameFinished = false;
				return;
			}
#endif // WINFORM_NESTED_SWITCH

			this->_SendListBodyData();

			TimerCallBack timepro = CC_CALLBACK_2(IPC::OnStepTimer, this);
			g_GameUtil->SetTimer(m_pManager->GetPaintWindow(), timepro, TIMER_ID_START_GAME_FINISH + 2, TIME_START_GAME_FINISH);
		}break;

		// 列表menu(下发游戏:菜单配置)
		case TIMER_ID_START_GAME_FINISH + 2:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Send List Menu"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH + 2);

			CMD_Command cmd;
			cmd.wMainCmdID = IPC_MAIN_CONCTROL;
			cmd.wSubCmdID = IPC_SUB_WINGAME_MENU_INIT;
			this->LocalSendData(cmd, nullptr, 0);

			TimerCallBack timepro = CC_CALLBACK_2(IPC::OnStepTimer, this);
			g_GameUtil->SetTimer(m_pManager->GetPaintWindow(), timepro, TIMER_ID_START_GAME_FINISH + 3, TIME_START_GAME_FINISH);
		}break;

		// 启动完毕
		case TIMER_ID_START_GAME_FINISH + 3:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Finished"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH + 3);

#ifndef WINFORM_NESTED_SWITCH
			this->StartGamesInfo();
#endif //WINFORM_NESTED_SWITCH

			// 重置心跳设置
			this->CloseHeartbeat();
			this->StartHeartbeat(TIME_HEART_MIN_OUT);

			// 若游戏已启动完毕，减少超时重连次数
			// 便于及时检测嘛
			this->m_dwMaxHeartOut = TIME_HEART_MIN_OUT;

			// 游戏加载完毕
			this->m_bGameFinished = true;
		}break;

		default:
			break;
	}
}

void IPC::CloseStepTimer()
{
	if (m_pManager)
	{
		HWND hWnd = m_pManager->GetPaintWindow();
		g_GameUtil->KillTimer(hWnd, TIMER_ID_START_GAME_FINISH);
		g_GameUtil->KillTimer(hWnd, TIMER_ID_START_GAME_FINISH + 1);
		g_GameUtil->KillTimer(hWnd, TIMER_ID_START_GAME_FINISH + 2);
		g_GameUtil->KillTimer(hWnd, TIMER_ID_START_GAME_FINISH + 3);
	}
}

bool IPC::LocalSendData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	bool bRes = false;
#ifdef WINFORM_NESTED_SWITCH
	bRes = _LocalSendData(m_hTargetWnd, IPC_FLAG_TO_WINFROM, cmd, pDataBuffer, wDataSize);
#else
	bRes = _LocalSendData(m_hTargetWnd, IPC_FLAG_TO_GAME, cmd, pDataBuffer, wDataSize);
#endif
	return bRes;
}

bool IPC::_LocalSendData(HWND hWnd, DWORD dwFlag, const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	if (!m_bConnect || FALSE == ::IsWindow(hWnd)) return false;

	//效验数据
	ASSERT(wDataSize <= IPC_BUFFER);
	if (wDataSize > IPC_BUFFER)
	{
		g_Logger->TryInfo(_T("IPC - Local Data Send Failed [mcid = %u, scid = %u, data_size = %u, buf_size = %d]")
			, cmd.wMainCmdID, cmd.wSubCmdID, wDataSize, IPC_BUFFER);

		return false;
	}

	// 填充本机进程ipc数据
	m_IPCBuf.Head.wVersion = IPC_VER;
	m_IPCBuf.Head.wMainCmdID = cmd.wMainCmdID;
	m_IPCBuf.Head.wSubCmdID = cmd.wSubCmdID;
	m_IPCBuf.Head.wDataSize = wDataSize;
	memcpy(m_IPCBuf.cbBuffer, pDataBuffer, wDataSize);

	COPYDATASTRUCT cdStruct;
	cdStruct.dwData = dwFlag;
	cdStruct.cbData = IPC_BUFFER;
	cdStruct.lpData = (PVOID)&m_IPCBuf;

	::SendMessage(hWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&cdStruct);

	// 发送完毕,清空ipc.buf缓存
	memset(&m_IPCBuf, 0, IPC_BUFFER);

	return true;
}

bool IPC::LocalRecvData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	if (!m_bConnect) return false;

	//效验数据
	ASSERT(wDataSize <= IPC_BUFFER);
	if (wDataSize > IPC_BUFFER)
	{
		g_Logger->TryInfo(_T("IPC - Local Data Recv Failed [mcid = %u, scid = %u, data_size = %u, buf_size = %d]")
			, cmd.wMainCmdID, cmd.wSubCmdID, wDataSize, IPC_BUFFER);

		return false;
	}

	switch (cmd.wMainCmdID)
	{
		case IPC_MAIN_CONCTROL:
		{
			switch (cmd.wSubCmdID)
			{
				// 游戏-大厅间的心跳消息
				case IPC_SUB_HEARTBEAT:
				{
					m_bHeartbeat = true;
				}break;

			// Winform启动
			#ifdef WINFORM_NESTED_SWITCH
				case IPC_SUB_WINGAME_START_OK:
				{
					g_Logger->TryInfo(_T("IPC - Winform Start Finish"));

					// 启动完成才能获取到游戏窗口句柄
					m_hTargetWnd = GetWindowHandleByPID(m_tTargetProcessInfo.dwProcessId);
					if (FALSE == ::IsWindow(m_hTargetWnd))
					{
						g_Logger->TryInfo(_T("IPC - StartGames Failed Not Find Window Handle [ecode = %lu]"), ::GetLastError());
						return false;
					}

					this->CloseStepTimer();
					TimerCallBack timepro = CC_CALLBACK_2(IPC::OnStepTimer, this);
					g_GameUtil->SetTimer(m_pManager->GetPaintWindow(), timepro, TIMER_ID_START_GAME_FINISH, TIME_START_GAME_FINISH);
				}break;
				
				case IPC_SUB_WINGAME_START_NO:
				{
					g_Logger->TryError(_T("IPC - Winform Start Connect Failed"));
					DialogFrame::MsgBox(m_pManager, _T("温馨提示:"), _T("游戏启动失败!"));
					//this->DisConnect(false);
				}break;

				case IPC_SUB_WINGAME_GAMEPROCESS:
				{
					PROCESS_INFORMATION* pProcessInfo = static_cast<PROCESS_INFORMATION*>(pDataBuffer);
					if (pProcessInfo)
					{
						memcpy(&m_tNestedProcessInfo, pProcessInfo, sizeof(PROCESS_INFORMATION));
					}
				}break;
			#endif // WINFORM_NESTED_SWITCH

				// 游戏启动完成
				case IPC_SUB_START_FINISH:
				{
					TimerCallBack timepro = CC_CALLBACK_2(IPC::OnStepTimer, this);
					g_GameUtil->SetTimer(m_pManager->GetPaintWindow(), timepro, TIMER_ID_START_GAME_FINISH + 1, TIME_START_GAME_FINISH);
				}break;

				// 游戏启动完成,请求初始化游戏场景
				case IPC_SUB_WINGAME_START_INFO:
				{
					this->CloseStepTimer();
					this->StartGamesInfo();
				}break;

				// 启动失败
				case IPC_SUB_START_FAILED:
				{
					g_Logger->TryError(_T("IPC - Game Start Connect Failed"));

					//this->DisConnect();
					DialogFrame::MsgBox(m_pManager, _T("温馨提示:"), _T("游戏连接失败!"));
				}break;

				// 游戏用户同意
				case IPC_SUB_GF_USER_READY:
				{
					g_Logger->TryInfo(_T("IPC - User Ready Req"));
					RoomManager* roomManager = RoomManager::GetInstance();
					DWORD bRes = roomManager->SendData(MDM_GF_FRAME, SUB_GF_USER_READY);
					if (FALSE == bRes)
					{
						//DialogFrame::MsgBox(m_pManager, _T("温馨提示:"), _T("游戏准备请求失败!"));
						g_Logger->TryError(_T("IPC - User Ready Req Failed"));
					}
				}break;

				// 踢出用户
				case IPC_SUB_KICK_USER:
				{
					g_Logger->TryInfo(_T("IPC - KICK USER Req"));

					CMD_GR_KillUser* pKillUser = static_cast<CMD_GR_KillUser*>(pDataBuffer);
					if (pKillUser)
					{
						g_Logger->TryInfo(_T("IPC - KICK USER Req-OK [uid = %u]"), pKillUser->dwTargetUserID);
						RoomManager* pRoomManager = RoomManager::GetInstance();
						pRoomManager->SendData(MDM_GR_MANAGER, SUB_GR_KILL_USER, pKillUser, sizeof(CMD_GR_KillUser));
					}
				}break;

				// 发送聊天消息
				case IPC_SUB_SEND_CHAT:
				{
					CMD_GR_UserChat* pStuChar = static_cast<CMD_GR_UserChat*>(pDataBuffer);
					if (pStuChar)
					{
						RoomManager* pRoomManager = RoomManager::GetInstance();

						WORD wSendSize = sizeof(CMD_GR_UserChat) - sizeof(pStuChar->szChatMessage) + pStuChar->wChatLength;
						pRoomManager->SendData(MDM_GR_USER, SUB_GR_USER_CHAT, pStuChar, wSendSize);
					}
				}break;

				// 关闭框架
				case IPC_SUB_CLOSE_FRAME:
				{
					g_Logger->TryInfo(_T("IPC - Close Game"));

					bool bRes = false;
					RoomManager* pRoomManager = RoomManager::GetInstance();
					Room* pCurRoom = pRoomManager->GetCurRoom();
					pCurRoom->CloseGameNotify();
					
					this->DisConnect();

				}break;

				// 游戏强行关闭
				case IPC_SUB_CLOSE_FRAME_EX:
				{
					g_Logger->TryInfo(_T("IPC - Close Game Ex"));

					bool bRes = false;
					RoomManager* pRoomManager = RoomManager::GetInstance();
					Room* pCurRoom = pRoomManager->GetCurRoom();
					pCurRoom->CloseGameNotify(true);

					this->DisConnect();
				}

				default:
					break;
			}
		}break;
		default:
			break;
	}

	return true;
}
