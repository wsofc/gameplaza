#include "stdafx.h"
#include "serviceData.h"

ServiceData* ServiceData::g_pServerListData = nullptr;
ServiceData::ServiceData()
	: m_dwAllOnLineCount(0)
	, m_pNormalTableConfig(nullptr)
	, m_pMemberOrderConfig(nullptr)
	, m_pChatColorConfig(nullptr)
{
	// 桌子、会员、聊天等文本色配置先创建对象,就算无配置文件,也可以读取默认值
	m_pNormalTableConfig	= new GameNormalTableConfig();
	m_pMemberOrderConfig	= new MemberOrderConfig();
	m_pChatColorConfig		= new ChatColorConfig();
}

ServiceData::~ServiceData()
{
	SAFE_DELETE(m_pNormalTableConfig);
	SAFE_DELETE(m_pMemberOrderConfig);
	SAFE_DELETE(m_pChatColorConfig);

	DeleteAllGameType();
	DeleteAllGameKind();
	DeleteAllGameStation();
	DeleteAllGameServer();
	DeleteAllGameProps();
	DeleteAllGameFontColor();
}

ServiceData* ServiceData::GetInstance()
{
	if (ServiceData::g_pServerListData == nullptr)
		ServiceData::g_pServerListData = new (std::nothrow)ServiceData();
	return ServiceData::g_pServerListData;
}

void ServiceData::SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount)
{
	GameKindItemMap::iterator& it = m_GameKindItemMap.find(wKindID);
	if (it != m_GameKindItemMap.end())
		it->second->dwOnlineCount = dwOnLineCount;
}

void ServiceData::SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount)
{
	GameServerItemMap::iterator& it = m_GameServerItemMap.find(wServerID);
	if (it != m_GameServerItemMap.end())
	{
		//更新总在线人数
		m_dwAllOnLineCount -= it->second->dwOnlineCount;
		m_dwAllOnLineCount += dwOnLineCount;

		//更新房间在线人数
		it->second->dwOnlineCount = dwOnLineCount;
	}
}

bool ServiceData::InsertGameType(tagGameType * pGameType)
{
	ASSERT(pGameType != nullptr);
	if (!pGameType) return false;
	
	tagGameType* pDestGameType = nullptr;
	for (const auto& it : m_GameTypeItemMap)
	{
		if (it.first == pGameType->wTypeID)
		{
			pDestGameType = it.second;
			break;
		}
	}

	if (!pDestGameType)
	{
		pDestGameType = new tagGameType();
	}

	::CopyMemory(pDestGameType, pGameType, sizeof(tagGameType));
	m_GameTypeItemMap[pDestGameType->wTypeID] = pDestGameType;

	return true;
}

bool ServiceData::InsertGameKind(tagGameKind * pGameKind)
{
	ASSERT(pGameKind != nullptr);
	if (!pGameKind) return false;

	tagGameKind* pDesGametKind = nullptr;
	for (const auto& it : m_GameKindItemMap)
	{
		if (it.first == pGameKind->wKindID)
		{
			pDesGametKind = it.second;
			break;
		}
	}

	if (!pDesGametKind)
	{
		pDesGametKind = new tagGameKind();
	}

	::CopyMemory(pDesGametKind, pGameKind, sizeof(tagGameKind));
	m_GameKindItemMap[pDesGametKind->wKindID] = pDesGametKind;

	return true;
}

bool ServiceData::InsertGameStation(tagGameStation* pGameStation)
{
	ASSERT(pGameStation != nullptr);
	if (!pGameStation) return false;

	tagGameStation* pDesGametStation = nullptr;
	for (const auto& it : m_GameStationItemMap)
	{
		if (it.first == pGameStation->wKindID)
		{
			pDesGametStation = it.second;
			break;
		}
	}

	if (!pDesGametStation)
	{
		pDesGametStation = new tagGameStation();
	}

	::CopyMemory(pDesGametStation, pGameStation, sizeof(tagGameStation));
	m_GameStationItemMap[pDesGametStation->wStationID] = pDesGametStation;

	return true;
}

bool ServiceData::InsertGameServer(tagGameServer * pGameServer)
{
	ASSERT(pGameServer != nullptr);
	if (!pGameServer) return false;

	tagGameServer* pDestGameServer = nullptr;
	for (const auto& it : m_GameServerItemMap)
	{
		if (it.first == pGameServer->wServerID)
		{
			pDestGameServer = it.second;
		}
	}

	if (!pDestGameServer)
	{
		pDestGameServer = new tagGameServer();
	}

	::CopyMemory(pDestGameServer, pGameServer, sizeof(tagGameServer));
	m_GameServerItemMap[pDestGameServer->wServerID] = pDestGameServer;

	return true;
}

bool ServiceData::InsertGameProps(tagPropsInfo* pGameProps)
{
	ASSERT(pGameProps != nullptr);
	if (!pGameProps) return false;

	tagPropsInfo* pDestGameProps = nullptr;
	for (const auto& it : m_GamePropsItemMap)
	{
		if (it.first == pGameProps->wPropsID)
		{
			pDestGameProps = it.second;
		}
	}

	if (!pDestGameProps)
	{
		pDestGameProps = new tagPropsInfo();
	}

	::CopyMemory(pDestGameProps, pGameProps, sizeof(tagPropsInfo));
	m_GamePropsItemMap[pDestGameProps->wPropsID] = pDestGameProps;

	return true;
}

bool ServiceData::DeleteGameType(WORD wTypeID)
{
	GameTypeItemMap::iterator it = m_GameTypeItemMap.find(wTypeID);
	if (it != m_GameTypeItemMap.end())
	{
		SAFE_DELETE(it->second);
		m_GameTypeItemMap.erase(it);
		return true;
	}
	else
		return false;
}

bool ServiceData::DeleteGameKind(WORD wKindID)
{
	GameKindItemMap::iterator it = m_GameKindItemMap.find(wKindID);
	if (it != m_GameKindItemMap.end())
	{
		SAFE_DELETE(it->second);
		m_GameKindItemMap.erase(it);
		return true;
	}
	else
		return false;
}

bool ServiceData::DeleteGameStation(WORD wStationID)
{
	GameStationItemMap::iterator it = m_GameStationItemMap.find(wStationID);
	if (it != m_GameStationItemMap.end())
	{
		SAFE_DELETE(it->second);
		m_GameStationItemMap.erase(it);
		return true;
	}
	else
		return false;
}

bool ServiceData::DeleteGameServer(WORD wServerID)
{
	GameServerItemMap::iterator it = m_GameServerItemMap.find(wServerID);
	if (it != m_GameServerItemMap.end())
	{
		SAFE_DELETE(it->second);
		m_GameServerItemMap.erase(it);
		return true;
	}
	else
		return false;
}

bool ServiceData::DeleteGameProps(WORD wPropsID)
{
	GamePropsItemMap::iterator it = m_GamePropsItemMap.find(wPropsID);
	if (it != m_GamePropsItemMap.end())
	{
		SAFE_DELETE(it->second);
		m_GamePropsItemMap.erase(it);
		return true;
	}
	else
		return false;
}

void ServiceData::DeleteAllGameType()
{
	for (const auto& it : m_GameTypeItemMap)
	{
		tagGameType* pGameType = it.second;
		SAFE_DELETE(pGameType);
	}
	m_GameTypeItemMap.clear();
}

void ServiceData::DeleteAllGameKind()
{
	for (const auto& it : m_GameKindItemMap)
	{
		tagGameKind* pGameKind = it.second;
		SAFE_DELETE(pGameKind);
	}
	m_GameKindItemMap.clear();
}

void ServiceData::DeleteAllGameStation()
{
	for (const auto& it : m_GameStationItemMap)
	{
		tagGameStation* pGameStation = it.second;
		SAFE_DELETE(pGameStation);
	}
	m_GameStationItemMap.clear();
}

void ServiceData::DeleteAllGameServer()
{
	for (const auto& it : m_GameServerItemMap)
	{
		tagGameServer* pGameServer = it.second;
		SAFE_DELETE(pGameServer);
	}
	m_GameServerItemMap.clear();
}

void ServiceData::DeleteAllGameProps()
{
	for (const auto& it : m_GamePropsItemMap)
	{
		tagPropsInfo* pPropsInfo = it.second;
		SAFE_DELETE(pPropsInfo);
	}
	m_GamePropsItemMap.clear();
}

tagGameType* ServiceData::SearchGameType(WORD wTypeID)
{
	GameTypeItemMap::mapped_type pGameType = nullptr;
	if (m_GameTypeItemMap.find(wTypeID) != m_GameTypeItemMap.end())
		pGameType = m_GameTypeItemMap[wTypeID];
	return pGameType;
}

tagGameKind* ServiceData::SearchGameKind(WORD wKindID)
{
	GameKindItemMap::mapped_type pGameKind = nullptr;
	if (m_GameKindItemMap.find(wKindID) != m_GameKindItemMap.end())
		pGameKind = m_GameKindItemMap[wKindID];
	return pGameKind;
}

tagGameStation* ServiceData::SearchGameStation(WORD wStationID)
{
	GameStationItemMap::mapped_type pGameStation = nullptr;
	if (m_GameStationItemMap.find(wStationID) != m_GameStationItemMap.end())
		pGameStation = m_GameStationItemMap[wStationID];
	return pGameStation;
}

tagGameServer* ServiceData::SearchGameServer(WORD ServerID)
{
	GameServerItemMap::mapped_type pGameServer = nullptr;
	if (m_GameServerItemMap.find(ServerID) != m_GameServerItemMap.end())
		pGameServer = m_GameServerItemMap[ServerID];
	return pGameServer;
}

tagPropsInfo* ServiceData::SearchGameProps(WORD wPropsID)
{
	GamePropsItemMap::mapped_type pGameProps = nullptr;
	if (m_GamePropsItemMap.find(wPropsID) != m_GamePropsItemMap.end())
		pGameProps = m_GamePropsItemMap[wPropsID];
	return pGameProps;
}

tagGameType* ServiceData::EnumGameType(DWORD& iNextKey)
{
	tagGameType* pStruct = nullptr;
	if (0 == iNextKey)
		iNextKey = m_GameTypeItemMap.begin()->first;

	GameTypeItemMap::iterator& it = m_GameTypeItemMap.find(iNextKey);
	if (it != m_GameTypeItemMap.end())
	{
		pStruct = it->second;

		// iNextKey往后偏移
		if (++it != m_GameTypeItemMap.end())
			iNextKey = it->first;
	}

	return pStruct;
}

tagGameKind* ServiceData::EnumGameKind(DWORD& iNextKey)
{
	tagGameKind* pStruct = nullptr;
	if (0 == iNextKey)
		iNextKey = m_GameKindItemMap.begin()->first;

	GameKindItemMap::iterator& it = m_GameKindItemMap.find(iNextKey);
	if (it != m_GameKindItemMap.end())
	{
		pStruct = it->second;

		// iNextKey往后偏移
		if (++it != m_GameKindItemMap.end())
			iNextKey = it->first;
	}

	return pStruct;
}

tagGameStation* ServiceData::EnumGameStation(DWORD& iNextKey)
{
	tagGameStation* pStruct = nullptr;
	if (0 == iNextKey)
		iNextKey = m_GameStationItemMap.begin()->first;

	GameStationItemMap::iterator& it = m_GameStationItemMap.find(iNextKey);
	if (it != m_GameStationItemMap.end())
	{
		pStruct = it->second;

		// iNextKey往后偏移
		if (++it != m_GameStationItemMap.end())
			iNextKey = it->first;
	}

	return pStruct;
}

tagGameServer* ServiceData::EnumGameServer(DWORD& iNextKey)
{
	tagGameServer* pStruct = nullptr;
	if (0 == iNextKey)
		iNextKey = m_GameServerItemMap.begin()->first;

	GameServerItemMap::iterator& it = m_GameServerItemMap.find(iNextKey);
	if (it != m_GameServerItemMap.end())
	{
		pStruct = it->second;

		// iNextKey往后偏移
		if (++it != m_GameServerItemMap.end())
			iNextKey = it->first;
	}

	return pStruct;
}

tagPropsInfo* ServiceData::EnumGameProps(DWORD& iNextKey)
{
	tagPropsInfo* pStruct = nullptr;
	if (0 == iNextKey)
		iNextKey = m_GamePropsItemMap.begin()->first;

	GamePropsItemMap::iterator& it = m_GamePropsItemMap.find(iNextKey);
	if (it != m_GamePropsItemMap.end())
	{
		pStruct = it->second;

		// iNextKey往后偏移
		if (++it != m_GamePropsItemMap.end())
			iNextKey = it->first;
	}

	return pStruct;
}

bool ServiceData::InsertGameFontColor(GameFontColor* pFontColor)
{
	ASSERT(pFontColor != nullptr);
	if (!pFontColor) return false;

	GameFontColor* pDestFontColor = nullptr;
	for (const auto& it : m_GameFontColorMap)
	{
		if (it.first == pFontColor->id)
		{
			pDestFontColor = it.second;
			break;
		}
	}

	if (!pDestFontColor)
	{
		pDestFontColor = new GameFontColor();
	}

	::CopyMemory(pDestFontColor, pFontColor, sizeof(GameFontColor));
	m_GameFontColorMap[pDestFontColor->id] = pDestFontColor;

	return true;
}

GameFontColor* ServiceData::GetFontColorByID(int id)
{
	GameFontColor* pFontColor = nullptr;
	if (m_GameFontColorMap.find(id) != m_GameFontColorMap.end())
	{
		pFontColor = m_GameFontColorMap[id];
	}

	return pFontColor;
}

void ServiceData::DeleteAllGameFontColor()
{
	for (const auto& it : m_GameFontColorMap)
	{
		GameFontColor* pFontColor = it.second;
		SAFE_DELETE(pFontColor);
	}
	m_GameFontColorMap.clear();
}

void ServiceData::SetNormalTableConfig(GameNormalTableConfig* pNormalTable)
{
	ASSERT(pNormalTable != nullptr);
	if (!pNormalTable) return;

	if (!m_pNormalTableConfig)
	{
		m_pNormalTableConfig = new GameNormalTableConfig();
	}

	::CopyMemory(m_pNormalTableConfig, pNormalTable, sizeof(GameNormalTableConfig));
}

void ServiceData::SetMemberOrderConfig(MemberOrderConfig* pMemberOrderConfig)
{
	ASSERT(pMemberOrderConfig != nullptr);
	if (!pMemberOrderConfig) return;

	if (!m_pMemberOrderConfig)
	{
		m_pMemberOrderConfig = new MemberOrderConfig();
	}

	::CopyMemory(m_pMemberOrderConfig, pMemberOrderConfig, sizeof(MemberOrderConfig));
}

void ServiceData::SetChatColorConfig(ChatColorConfig* pChatColorConfig)
{
	ASSERT(pChatColorConfig != nullptr);
	if (!pChatColorConfig) return;

	if (!m_pChatColorConfig)
	{
		m_pChatColorConfig = new ChatColorConfig();
	}

	::CopyMemory(m_pChatColorConfig, pChatColorConfig, sizeof(ChatColorConfig));
}

void ServiceData::AddUrlWeb(LPCTSTR lpName, LPCTSTR lpUrl)
{
	if (nullptr == lpName || lpUrl == nullptr) return;

	m_urlWebMap[lpName] = lpUrl;
}

LPCTSTR ServiceData::GetUrlWeb(LPCTSTR lpName)
{
	LPCTSTR lpUrl = nullptr;

	if (m_urlWebMap.find(lpName) != m_urlWebMap.end())
	{
		lpUrl = m_urlWebMap[lpName].c_str();
	}

	return lpUrl;
}

DWORD ServiceData::GetExeVersion(LPCTSTR strFile)
{
	DWORD u32_Version = 0;
	DWORD dwVerSize = 0;
	DWORD dwHandle = 0;
	BOOL bRes = FALSE;
	dwVerSize = ::GetFileVersionInfoSize(strFile, &dwHandle);
	if (dwVerSize > 0)
	{
		TCHAR* szVersionBuffer = new TCHAR[dwVerSize];
		memset(szVersionBuffer, 0, dwVerSize * sizeof(TCHAR));
		if (::GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
		{
			VS_FIXEDFILEINFO* pInfo = nullptr;
			unsigned int nInfoLen;

			if (::VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))
			{
				WORD w_v1 = HIWORD(pInfo->dwFileVersionMS);
				WORD w_v2 = LOWORD(pInfo->dwFileVersionMS);
				WORD w_v3 = HIWORD(pInfo->dwFileVersionLS);
				WORD w_v4 = LOWORD(pInfo->dwFileVersionLS);

				CString strVersion;
				strVersion.Format(_T("%d%d%d%d"), w_v1, w_v2, w_v3, w_v4);
				u32_Version = _ttol(strVersion);
			}
		}

		delete szVersionBuffer;
		szVersionBuffer = nullptr;
	}

	return u32_Version;
}

void ServiceData::GetExeVersion(LPCTSTR strFile, CDuiString& strVersion)
{
	DWORD u32_Version = this->GetExeVersion(strFile);

	strVersion.Format(_T("%4d"), u32_Version);
	strVersion.Format(_T("%c.%c.%c.%c"), strVersion.GetAt(0), strVersion.GetAt(1), strVersion.GetAt(2), strVersion.GetAt(3));
}

void ServiceData::SetLocalGameVersion(UINT wKindID, UINT wVersion)
{
	m_dwLocalGameVersionMap[wKindID] = wVersion;
}

UINT ServiceData::GetLocalGameVersion(UINT wKindID)
{
	if (m_dwLocalGameVersionMap.end() != m_dwLocalGameVersionMap.find(wKindID))
	{
		return m_dwLocalGameVersionMap[wKindID];
	}
	return 0;
}

DWORD ServiceData::ReadExeVersionInfo(UINT wKindID, CDuiString& pzExePath)
{
	TCHAR pzPath[MAX_PATH] = { 0 };
	::PathCanonicalize(pzPath, pzExePath);

	DWORD wVersion = this->GetExeVersion(pzPath);
	this->SetLocalGameVersion(wKindID, wVersion);

	return wVersion;
}
