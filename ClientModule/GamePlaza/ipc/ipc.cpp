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

		//�˳�ö��(�޴���)
		::SetLastError(0);
		return FALSE;
	}

	//����ö��
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
			, _T("��ܰ��ʾ")
			, _T("������Ϸʧ��")
			, DLG_ID(IPCMsgBoxID::MSG_BOX_ID_START_GAME_FAILED)
			, DLG_STYLE::DLG_OK_CANCEL
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else
	{
#ifdef GAME_TEST_SWITCH
		// ����ģ���������
		CMD_Command cmd;
		cmd.wMainCmdID = IPC_MAIN_CONCTROL;
		cmd.wSubCmdID = IPC_SUB_START_FINISH;
		LocalRecvData(cmd, 0, 0);
#else
		// ��ʽ�������
		// ����������С��
		::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);

		// �����������
		this->CloseHeartbeat();
		this->StartHeartbeat(TIME_HEART_MAX_OUT);
#endif // GAME_TEST_SWITCH
	}
}

void IPC::StartGamesInfo()
{
	// ���з�����:��Ϸ�������(���������·�������ʼ����Ϣ)
	g_Logger->TryInfo(_T("IPC - Send to Server GameInfo"));

	CMD_GF_Info stuInfo;
	stuInfo.bAllowLookon = FALSE;

	RoomManager* roomManager = RoomManager::GetInstance();
	DWORD bRes = roomManager->SendData(MDM_GF_FRAME, SUB_GF_INFO, &stuInfo, sizeof(CMD_GF_Info));
	if (FALSE == bRes)
	{
		// DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ:"), _T("��ȡ��Ϸ����ʧ��!"));
		g_Logger->TryError(_T("IPC - Send to Server GameInfo - Failed"));
	}

#ifdef GAME_TEST_SWITCH
	//��������Ϸ�Զ�ģ��READY��Ϣ
	RoomManager* pRoomManager = RoomManager::GetInstance();
	Room* pCurRoom = pRoomManager->GetCurRoom();
	if (pCurRoom)
	{
		//��������Ϸ�Զ�ģ��READY��Ϣ
		const WORD wGameKind = pCurRoom->GetKindID();
		if (wGameKind == 30 || wGameKind == 31)
		{
			RoomManager* roomManager = RoomManager::GetInstance();
			DWORD bRes = roomManager->SendData(MDM_GF_FRAME, SUB_GF_USER_READY);
			if (FALSE == bRes)
			{
				DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ:"), _T("��Ϸ׼������ʧ��!"));
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
	
	// ��������
#ifdef GAME_TEST_SWITCH
	// �Ƿ���Winform����Ƕ��ģʽ
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
		// ������Ϸ����ģʽ
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

	// ��ʽ����
	if ((m_tTargetProcessInfo.hProcess != NULL) && (WaitForSingleObject(m_tTargetProcessInfo.hProcess, 0) == WAIT_TIMEOUT))
	{
		g_Logger->TryInfo(_T("IPC - process already exists"));

		// �������
		return true;
	}
		
	// ��ʽ��·��
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

	// ��������
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
		// ��ʱ������ȴ��ӽ��̽��г�ʼ��  
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
	// �ر�Ŀ�����
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

	// Winform���ģʽ��:�ر���Ϸ����
#ifdef WINFORM_NESTED_SWITCH
	HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, m_tNestedProcessInfo.dwProcessId);
	::TerminateProcess(hProcess, 0);
	::CloseHandle(hProcess);
#endif // WINFORM_NESTED_SWITCH
}

void IPC::DisConnect(bool bShowTips /* = true */)
{
	g_Logger->TryInfo(_T("IPC - DisConnect [bShowTips = %u]"), bShowTips);

	// �ر�������ʱ��(���ѹر�,��Ӱ��)
	this->CloseHeartbeat();
	
	// �رռ�����Ϸ�����ʱ��
	this->CloseStepTimer();

	// ����
	if (bShowTips && m_pManager)
	{
		CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(R->kEdtChatPanel_View));
		if (pViewEdit)
		{
			g_GameUtil->SendSystemMsgText(pViewEdit, _T("���ѳɹ��˳���Ϸ!"));
		}
	}

	// �ر���Ϸ����(�ÿ�����״̬)
	this->_CloseGameProcess();

	// �������ڸ�ԭ
	g_GameUtil->RestoreTopWindow(m_hWnd);

	// ��������״̬��
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
		// ��ʱ��Ͽ�
		if (++m_dwHeartOut >= m_dwMaxHeartOut)
		{
			g_Logger->TryInfo(_T("IPC Connect Game Time Out"));

			m_dwHeartOut = 0;

			// �ر�������ʱ��
			g_GameUtil->KillTimer(m_hWnd, TIMER_ID_HEARTBEAT);

			// ��ʾ���߶Ի���
			DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ!"), _T("��Ϸ�쳣�ر�!"));
						
			// ���͹ر���Ϸ��Ϣ
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

	// �����������
	CMD_Command cmd;
	cmd.wMainCmdID = IPC_MAIN_USER;
	cmd.wSubCmdID = IPC_SUB_USER_COME;

	// ׷�����صĹ���Ա�û���
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

	// ������������
	BYTE cbBuffer[IPC_BUFFER] = { 0 };
	for (size_t i = 0; i < vUserArray.size(); ++i)
	{
		UserData* pUser = vUserArray[i];
		::memset(&cbBuffer, 0, ArrayCount(cbBuffer) * sizeof(BYTE));

		// ɸѡͬ��(����Ч��)����·�����Ϸ
		if (pUser->GetTableID() == tSelf.GetTableID() && pUser->GetTableID() != INVALID_TABLE)
		{
			UserStruct& uStruct = pUser->GetUserStruct();
			::memcpy(cbBuffer, &uStruct, sizeof(UserStruct));

			// �����û��ṹ
			UserStructSuffix uStructSuffix = pUser->GetUserStructSuffix();
			CSendPacketHelper Packet(cbBuffer + sizeof(UserStruct), sizeof(cbBuffer) - sizeof(UserStruct));
			Packet.AddPacket(&uStructSuffix, sizeof(UserStructSuffix), IPC_USER_SUFFIX);

			// �·��������
			this->LocalSendData(cmd, cbBuffer, sizeof(UserStruct) + Packet.GetDataSize());
		}
	}
}

void IPC::OnStepTimer(UINT_PTR idEvent, DWORD dwTime)
{
	switch (idEvent)
	{
		// �б�head(�·���Ϸ:�����û��б�head��Ϣ(������WinForm���Ƕ��,�����·��б�ͷ��ʼ����Ϣ))
		case TIMER_ID_START_GAME_FINISH:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Send List Head"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH);

			this->_SendListHeadData();
		}break;

		// �б�body(�·���Ϸ:�û�comming)
		case TIMER_ID_START_GAME_FINISH + 1:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Send List Body"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH + 1);

#ifndef WINFORM_NESTED_SWITCH
			// ������ɲ��ܻ�ȡ����Ϸ���ھ��
			this->m_hTargetWnd = GetWindowHandleByPID(this->m_tTargetProcessInfo.dwProcessId);
			if (FALSE == ::IsWindow(this->m_hTargetWnd))
			{
				g_Logger->TryInfo(_T("IPC - StartGames Failed Not Find Window Handle [ecode = %lu]"), ::GetLastError());

				// ������Ϸʧ��,����
				this->m_bGameFinished = false;
				return;
			}
#endif // WINFORM_NESTED_SWITCH

			this->_SendListBodyData();

			TimerCallBack timepro = CC_CALLBACK_2(IPC::OnStepTimer, this);
			g_GameUtil->SetTimer(m_pManager->GetPaintWindow(), timepro, TIMER_ID_START_GAME_FINISH + 2, TIME_START_GAME_FINISH);
		}break;

		// �б�menu(�·���Ϸ:�˵�����)
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

		// �������
		case TIMER_ID_START_GAME_FINISH + 3:
		{
			g_Logger->TryInfo(_T("IPC - Game Start Finish OnStepTimer Step - Finished"));
			g_GameUtil->KillTimer(m_pManager->GetPaintWindow(), TIMER_ID_START_GAME_FINISH + 3);

#ifndef WINFORM_NESTED_SWITCH
			this->StartGamesInfo();
#endif //WINFORM_NESTED_SWITCH

			// ������������
			this->CloseHeartbeat();
			this->StartHeartbeat(TIME_HEART_MIN_OUT);

			// ����Ϸ��������ϣ����ٳ�ʱ��������
			// ���ڼ�ʱ�����
			this->m_dwMaxHeartOut = TIME_HEART_MIN_OUT;

			// ��Ϸ�������
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

	//Ч������
	ASSERT(wDataSize <= IPC_BUFFER);
	if (wDataSize > IPC_BUFFER)
	{
		g_Logger->TryInfo(_T("IPC - Local Data Send Failed [mcid = %u, scid = %u, data_size = %u, buf_size = %d]")
			, cmd.wMainCmdID, cmd.wSubCmdID, wDataSize, IPC_BUFFER);

		return false;
	}

	// ��䱾������ipc����
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

	// �������,���ipc.buf����
	memset(&m_IPCBuf, 0, IPC_BUFFER);

	return true;
}

bool IPC::LocalRecvData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize)
{
	if (!m_bConnect) return false;

	//Ч������
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
				// ��Ϸ-�������������Ϣ
				case IPC_SUB_HEARTBEAT:
				{
					m_bHeartbeat = true;
				}break;

			// Winform����
			#ifdef WINFORM_NESTED_SWITCH
				case IPC_SUB_WINGAME_START_OK:
				{
					g_Logger->TryInfo(_T("IPC - Winform Start Finish"));

					// ������ɲ��ܻ�ȡ����Ϸ���ھ��
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
					DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ:"), _T("��Ϸ����ʧ��!"));
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

				// ��Ϸ�������
				case IPC_SUB_START_FINISH:
				{
					TimerCallBack timepro = CC_CALLBACK_2(IPC::OnStepTimer, this);
					g_GameUtil->SetTimer(m_pManager->GetPaintWindow(), timepro, TIMER_ID_START_GAME_FINISH + 1, TIME_START_GAME_FINISH);
				}break;

				// ��Ϸ�������,�����ʼ����Ϸ����
				case IPC_SUB_WINGAME_START_INFO:
				{
					this->CloseStepTimer();
					this->StartGamesInfo();
				}break;

				// ����ʧ��
				case IPC_SUB_START_FAILED:
				{
					g_Logger->TryError(_T("IPC - Game Start Connect Failed"));

					//this->DisConnect();
					DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ:"), _T("��Ϸ����ʧ��!"));
				}break;

				// ��Ϸ�û�ͬ��
				case IPC_SUB_GF_USER_READY:
				{
					g_Logger->TryInfo(_T("IPC - User Ready Req"));
					RoomManager* roomManager = RoomManager::GetInstance();
					DWORD bRes = roomManager->SendData(MDM_GF_FRAME, SUB_GF_USER_READY);
					if (FALSE == bRes)
					{
						//DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ:"), _T("��Ϸ׼������ʧ��!"));
						g_Logger->TryError(_T("IPC - User Ready Req Failed"));
					}
				}break;

				// �߳��û�
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

				// ����������Ϣ
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

				// �رտ��
				case IPC_SUB_CLOSE_FRAME:
				{
					g_Logger->TryInfo(_T("IPC - Close Game"));

					bool bRes = false;
					RoomManager* pRoomManager = RoomManager::GetInstance();
					Room* pCurRoom = pRoomManager->GetCurRoom();
					pCurRoom->CloseGameNotify();
					
					this->DisConnect();

				}break;

				// ��Ϸǿ�йر�
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
