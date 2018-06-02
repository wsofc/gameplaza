#include "stdafx.h"
#include "dataManager.h"

DataManager* DataManager::g_instance = NULL;

DataManager::DataManager()
	:m_userSelf(nullptr)
{
}

DataManager::~DataManager()
{
	DataManager::g_instance = NULL;

	this->Clean(true);
}

DataManager* DataManager::GetInstance()
{
	if (NULL == DataManager::g_instance)
	{
		DataManager::g_instance = new (std::nothrow)DataManager();
	}

	return DataManager::g_instance;
}

// 清除数据(bDurable - 是否一并清除持久性的数据)
void DataManager::Clean(bool bDurable /* = false */)
{
	//清除用户列表
	this->CleanOtherUserMap();

	//清除等级配置
	this->CleanLevelConfig();

	//清除房间场次配置
	this->CleanRoomSeasonConfig();

	if (bDurable)
	{
		//清除经验值等级配置
		this->CleanXPLevelConfig();
	}
}

UserData& DataManager::GetSelfData()
{
	return *m_userSelf;
}

UserData* DataManager::CreatUserDataByID(DWORD wUserID, bool bIsSelf /* = false */)
{
	UserData* pUserData = nullptr;
	if (m_userDataMap.find(wUserID) != m_userDataMap.end())
	{
		pUserData = m_userDataMap[wUserID];
	}
	else
	{
		pUserData = new UserData();
		if (pUserData)
		{
			pUserData->SetUserID(wUserID);
			m_userDataMap[wUserID] = pUserData;
		}
	}

	if (pUserData && bIsSelf)
	{
		pUserData->SetSelf(bIsSelf);
		m_userSelf = pUserData;
	}

	return pUserData;
}

UserData* DataManager::GetUserDataByID(DWORD wUserID)
{
	UserData* pUserData = nullptr;
	if (m_userDataMap.find(wUserID) != m_userDataMap.end())
		pUserData = m_userDataMap[wUserID];
	return pUserData;
}

bool DataManager::RemoveUserDataByID(DWORD wUserID)
{
	bool bRes = false;
	if (m_userDataMap.find(wUserID) != m_userDataMap.end())
	{
		bRes = true;
		UserData* pUser = m_userDataMap[wUserID];
		m_userDataMap.erase(wUserID);
		SAFE_DELETE(pUser);
	}
	return bRes;
}

void DataManager::CleanOtherUserMap()
{
	std::map<DWORD, UserData*>::iterator& it = m_userDataMap.begin();
	for (; it != m_userDataMap.end();)
	{
		UserData* pUser = it->second;
		if (pUser->IsSelf())
		{
			++it;
			continue;
		}

		SAFE_DELETE(pUser);
		it = m_userDataMap.erase(it);
	}
}

UserData* DataManager::EnumUserDataMap(DWORD& iNextKey)
{
	UserData* pData = nullptr;
	if (0 == iNextKey)
		iNextKey = m_userDataMap.begin()->first;

	std::map<DWORD, UserData*>::iterator& it = m_userDataMap.find(iNextKey);
	if (it != m_userDataMap.end())
	{
		pData = it->second;

		// iNextKey往后偏移
		if (++it != m_userDataMap.end())
			iNextKey = it->first;
	}

	return pData;
}

void DataManager::InsertOnlineDescConfig(DWORD dwCount, std::string& pzDesc, int iStatus)
{
	if (m_onlineDescConfigMap.find(dwCount) == m_onlineDescConfigMap.end())
	{
		OnlineDescConfig* pOnlineDescInfo = new OnlineDescConfig();
		pOnlineDescInfo->SetOnlineCount(dwCount);
		pOnlineDescInfo->SetDescribe(pzDesc);
		pOnlineDescInfo->SetStatus(iStatus);
		m_onlineDescConfigMap[dwCount] = pOnlineDescInfo;
	}
}

void DataManager::CleanOnlineDescConfig()
{
	for (const auto& it : m_onlineDescConfigMap)
	{
		OnlineDescConfig* pOnlineDescInfo = it.second;
		SAFE_DELETE(pOnlineDescInfo);
	}
	m_onlineDescConfigMap.clear();
}

CDuiString DataManager::GetOnlineDescByCount(DWORD dwCount)
{
	CDuiString pzDesc;
	for (const auto& it : m_onlineDescConfigMap)
	{
		pzDesc = it.second->GetDescribe();

		if (dwCount <= it.second->GetOnlineCount())
		{
			break;
		}
	}

	return pzDesc;
}

int DataManager::GetOnlineStatusByCount(DWORD dwCount)
{
	int iStatus = -1;
	for (const auto& it : m_onlineDescConfigMap)
	{
		iStatus = it.second->GetStatus();

		if (dwCount <= it.second->GetOnlineCount())
		{
			break;
		}
	}
	return iStatus;
}

void DataManager::InsertLevelConfig(DWORD dwLevel, LONGLONG lScore, std::string& pzDesc)
{
	if (m_levelConfigMap.find(lScore) == m_levelConfigMap.end())
	{
		LevelConfig* pLevelInfo = new LevelConfig();
		pLevelInfo->SetScore(lScore);
		pLevelInfo->SetLevel(dwLevel);
		pLevelInfo->SetDescribe(pzDesc);
		m_levelConfigMap[lScore] = pLevelInfo;
	}
}

void DataManager::CleanLevelConfig()
{
	for (const auto& it : m_levelConfigMap)
	{
		LevelConfig* pLevelInfo = it.second;
		SAFE_DELETE(pLevelInfo);
	}
	m_levelConfigMap.clear();
}

CDuiString DataManager::GetLevelDescByScore(LONGLONG lScore)
{
	CDuiString pzDesc;
	for (const auto& it : m_levelConfigMap)
	{
		pzDesc = it.second->GetDescribe();

		if (lScore <= it.second->GetScore())
		{
			break;
		}
	}

	return pzDesc;
}

void DataManager::InsertXPLevelConfig(DWORD dwLevel, LONGLONG lScore)
{
	if (m_experinceConfigMap.find(lScore) == m_experinceConfigMap.end())
	{
		ExperinceConfig* pExperinceInfo = new ExperinceConfig();
		pExperinceInfo->SetScore(lScore);
		pExperinceInfo->SetExperince(dwLevel);
		m_experinceConfigMap[lScore] = pExperinceInfo;
	}
}

void DataManager::CleanXPLevelConfig()
{
	for (const auto& it : m_experinceConfigMap)
	{
		ExperinceConfig* pExperinceInfo = it.second;
		SAFE_DELETE(pExperinceInfo);
	}
	m_experinceConfigMap.clear();
}

DWORD DataManager::GetXPLevelByScore(LONGLONG lScore)
{
	DWORD dwExperince = 0;
	for (const auto& it : m_experinceConfigMap)
	{
		dwExperince = it.second->GetExperince();

		if (lScore <= it.second->GetScore())
		{
			break;
		}
	}

	return dwExperince;
}

void DataManager::InsertRoomSeasonConfig(DWORD dwSeasonID, std::string& pzName, std::string& pzDesc)
{
	if (m_roomSeasonConfigMap.find(dwSeasonID) == m_roomSeasonConfigMap.end())
	{
		RoomSeasonConfig* pRoomSeason = new RoomSeasonConfig();
		pRoomSeason->SetSeasonID(dwSeasonID);
		pRoomSeason->SetSeasonName(pzName);
		pRoomSeason->SetSeasonDescribe(pzDesc);
		m_roomSeasonConfigMap[dwSeasonID] = pRoomSeason;
	}
}

void DataManager::CleanRoomSeasonConfig()
{
	for (const auto& it : m_roomSeasonConfigMap)
	{
		RoomSeasonConfig* pRoomSeason = it.second;
		SAFE_DELETE(pRoomSeason);
	}
	m_roomSeasonConfigMap.clear();
}

CDuiString DataManager::GetRoomSeasonNameByID(DWORD dwSeasonID)
{
	CDuiString pzName;
	if (m_roomSeasonConfigMap.find(dwSeasonID) != m_roomSeasonConfigMap.end())
	{
		pzName = m_roomSeasonConfigMap[dwSeasonID]->GetSeasonName();
	}

	return pzName;
}

CDuiString DataManager::GetRoomSeasonDescByID(DWORD dwSeasonID)
{
	CDuiString pzDesc;
	if (m_roomSeasonConfigMap.find(dwSeasonID) != m_roomSeasonConfigMap.end())
	{
		pzDesc = m_roomSeasonConfigMap[dwSeasonID]->GetSeasonDescribe();
	}

	return pzDesc;
}