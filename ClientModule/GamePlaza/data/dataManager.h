#pragma once

#ifndef __DATAMANAGER_H__
#define __DATAMANAGER_H__

#include "userData.h"
#include "serviceData.h"

class DataManager
{

public:
	enum eOnlineStatus
	{
		OLS_FREE = 0,		// 空闲
		OLS_NORMAL,			// 正常
		OLS_CROWDED,		// 拥挤
		OLS_BEPACKED		// 爆满
	};

public:

	~DataManager();
	static void Release(){ SAFE_DELETE(g_instance); }
	static DataManager* GetInstance();

	void Clean(bool bDurable = false);

	UserData& GetSelfData();
	UserData* CreatUserDataByID(DWORD wUserID, bool bIsSelf = false);
	UserData* GetUserDataByID(DWORD wUserID);
	bool RemoveUserDataByID(DWORD wUserID);
	void CleanOtherUserMap();
	DWORD GetUserDataCount(){ return m_userDataMap.size(); }
	UserData* EnumUserDataMap(DWORD& iNextKey);

	// 在线人数描述配置
	void InsertOnlineDescConfig(DWORD dwCount, std::string& pzDesc, int iStatus);
	void CleanOnlineDescConfig();
	CDuiString GetOnlineDescByCount(DWORD dwCount);
	int GetOnlineStatusByCount(DWORD dwCount);
	
	// 等级配置
	void InsertLevelConfig(DWORD dwLevel, LONGLONG lScore, std::string& pzDesc);
	void CleanLevelConfig();
	CDuiString GetLevelDescByScore(LONGLONG lScore);

	// 经验值等级配置
	void InsertXPLevelConfig(DWORD dwLevel, LONGLONG lScore);
	void CleanXPLevelConfig();
	DWORD GetXPLevelByScore(LONGLONG lScore);

	// 场次配置
	void InsertRoomSeasonConfig(DWORD dwSeasonID, std::string& pzName, std::string& pzDesc);
	void CleanRoomSeasonConfig();
	CDuiString GetRoomSeasonNameByID(DWORD dwSeasonID);
	CDuiString GetRoomSeasonDescByID(DWORD dwSeasonID);

private:
	UserData*									m_userSelf;					// 当前玩家
	std::map<DWORD,		UserData*>				m_userDataMap;				// 用户信息容器
	std::map<DWORD,		OnlineDescConfig*>		m_onlineDescConfigMap;		// 在线人数描述配置容器
	std::map<LONGLONG,	LevelConfig*>			m_levelConfigMap;			// 等级配置容器
	std::map<LONGLONG,	ExperinceConfig*>		m_experinceConfigMap;		// 经验值配置容器
	std::map<DWORD,		RoomSeasonConfig*>		m_roomSeasonConfigMap;		// 房间场次配置容器


	static DataManager* g_instance;
	DataManager();
};

#endif // __DATAMANAGER_H__