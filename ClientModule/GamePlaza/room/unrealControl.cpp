#include "stdafx.h"
#include "unrealControl.h"

extern GameResource* R;
extern GameUtil* g_GameUtil;
extern CStaticLogger g_Logger;

UnrealControl* UnrealControl::g_instance = nullptr;
UnrealControl::UnrealControl()
{ }

UnrealControl::~UnrealControl()
{ }

UnrealControl* UnrealControl::GetInstance()
{
	if (UnrealControl::g_instance == nullptr)
	{
		UnrealControl::g_instance = new UnrealControl();
	}
	return UnrealControl::g_instance;
}

bool UnrealControl::LoadUnrealDll()
{
	if (m_bLoadSuccess) return false;

	//¶¯Ì¬¼ÓÔØDll
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	CDuiString strInstancePath = CPaintManagerUI::GetInstancePath();
#ifdef DEBUG || _DEBUG
	_stprintf_s(m_pzBuffer, _T("%s%s"), strInstancePath.GetData(), _T("Unreal_d.dll"));
#else
	_stprintf_s(m_pzBuffer, _T("%s%s"), strInstancePath.GetData(), _T("Unreal.dll"));
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
		m_pFuncCreateUnreal			= (ProcCreateUnreal)GetProcAddress(m_hInst,			_T("CreateUnreal"));
		m_pFuncDeleteUnreal			= (ProcDeleteUnreal)GetProcAddress(m_hInst,			_T("DeleteUnreal"));

		m_pFuncLoadUnreal			= (ProcLoadUnreal)GetProcAddress(m_hInst,			_T("LoadUnreal"));
		m_pFuncInitUnreal			= (ProcInitUnreal)GetProcAddress(m_hInst,			_T("InitUnreal"));
		m_pFuncParseUnreal			= (ProcParseUnreal)GetProcAddress(m_hInst,			_T("ParseUnreal"));
		m_pFuncCleanUnreal			= (ProcCleanUnreal)GetProcAddress(m_hInst,			_T("CleanUnreal"));
		m_pFuncCheckUnrealRobot		= (ProcCheckUnrealRobot)GetProcAddress(m_hInst,		_T("CheckUnreal_Robot"));
		m_pFuncCheckUnrealUser		= (ProcCheckUnrealUser)GetProcAddress(m_hInst,		_T("CheckUnreal_User"));
		m_pFuncCheckUnrealReturn	= (ProcCheckUnrealReturn)GetProcAddress(m_hInst,	_T("CheckUnreal_Return"));

		m_pFuncIsMachineIDSwitchOn	= (ProcIsMachineIDSwitchOn)GetProcAddress(m_hInst,	_T("IsMachineIDSwitchOn"));
		m_pFuncGetMachineID			= (ProcGetMachineID)GetProcAddress(m_hInst,			_T("GetMachineID"));

		if (m_pFuncCreateUnreal && m_pFuncDeleteUnreal)
		{
			m_pFuncCreateUnreal(this, DataManager::GetInstance(), ServiceData::GetInstance(), GameResource::GetInstance());
		}
		else
		{
			int e_code = ::GetLastError();
			m_bLoadSuccess = false;
			::FreeLibrary(m_hInst);
		}
	}

	return m_bLoadSuccess;
}

void UnrealControl::UnLoadUnrealDll()
{
	if (m_bLoadSuccess && m_pFuncDeleteUnreal)
	{
		m_pFuncDeleteUnreal();
	}

	if (m_hInst)
	{
		::FreeLibrary(m_hInst);

		m_hInst = NULL;
		m_pFuncLoadUnreal = nullptr;
	}

	m_bLoadSuccess = false;
	SAFE_DELETE(UnrealControl::g_instance);
}

void UnrealControl::EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33])
{
	CMD5Encrypt::EncryptData(pszSrcData, szMD5Result);
}

DWORD UnrealControl::SendData(BYTE cbSendLocation, WORD wMainCmdID, WORD wSubCmdID)
{
	DWORD iRes = 0;
	switch (cbSendLocation)
	{
		case LOCATION_PLAZA_ROOM:
		{
			LoginServer* pLoginServer = LoginServer::GetInstance();
			if (pLoginServer->IsConnect())
			{
				iRes = pLoginServer->SendData(wMainCmdID, wSubCmdID);
			}
		}break;

		case LOCATION_GAME_ROOM:
		{
			GameServer* pGameServer = GameServer::GetInstance();
			if (pGameServer->IsConnect())
			{
				iRes = pGameServer->SendData(wMainCmdID, wSubCmdID);
			}
		}break;

		default:
			break;
	}

	return iRes;
}

DWORD UnrealControl::SendData(BYTE cbSendLocation, WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize)
{
	DWORD iRes = 0;
	switch (cbSendLocation)
	{
		case LOCATION_PLAZA_ROOM:
		{
			LoginServer* pLoginServer = LoginServer::GetInstance();
			if (pLoginServer->IsConnect())
			{
				iRes = pLoginServer->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
			}
		}break;

		case LOCATION_GAME_ROOM:
		{
			GameServer* pGameServer = GameServer::GetInstance();
			if (pGameServer->IsConnect())
			{
				iRes = pGameServer->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
			}
		}break;

		default:
			break;
	}

	return iRes;
}

void UnrealControl::LoadUnreal()
{
	if (m_bLoadSuccess && m_pFuncLoadUnreal)
	{
		m_pFuncLoadUnreal();
	}
}

void UnrealControl::InitUnreal(UserData* pUser)
{
	if (m_bLoadSuccess && m_pFuncInitUnreal)
	{
		m_pFuncInitUnreal(pUser);
	}
}

void UnrealControl::ParseUnreal(UserData* pUser)
{
	if (m_bLoadSuccess && m_pFuncParseUnreal)
	{
		m_pFuncParseUnreal(pUser);
	}
}

void UnrealControl::CleanUnreal(UserData* pUser)
{
	if (m_bLoadSuccess && m_pFuncCleanUnreal)
	{
		m_pFuncCleanUnreal(pUser);
	}
}

void UnrealControl::CheckUnreal_Robot()
{
	if (m_bLoadSuccess && m_pFuncCheckUnrealRobot)
	{
		bool bInRoom = g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_ROOM;
		m_pFuncCheckUnrealRobot(bInRoom);
	}
}

void UnrealControl::CheckUnreal_User(UnrealNSP::UnrealChargeType uStyle, LONGLONG lGolds, LPCTSTR lpEncryptPW, BYTE cbSendLocation)
{
	if (m_bLoadSuccess && m_pFuncCheckUnrealUser)
	{
		m_pFuncCheckUnrealUser(uStyle, lGolds, lpEncryptPW, cbSendLocation);
	}
}

bool UnrealControl::CheckUnreal_Return(DWORD dwRecvGameID, DWORD dwRecvID)
{
	bool bRes = false;
	if (m_bLoadSuccess && m_pFuncCheckUnrealReturn)
	{
		bRes = m_pFuncCheckUnrealReturn(dwRecvGameID, dwRecvID);
	}
	return bRes;
}

bool UnrealControl::IsMachineIDSwitchOn()
{
	bool bRes = false;
	if (m_bLoadSuccess && m_pFuncIsMachineIDSwitchOn)
	{
		bRes = m_pFuncIsMachineIDSwitchOn();
	}
	return bRes;
}

void UnrealControl::GetMachineID(TCHAR(&buffer)[COMPUTER_ID_LEN])
{
	if (m_bLoadSuccess && m_pFuncGetMachineID)
	{
		m_pFuncGetMachineID(buffer);
	}
}