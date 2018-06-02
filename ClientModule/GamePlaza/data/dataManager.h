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
		OLS_FREE = 0,		// ����
		OLS_NORMAL,			// ����
		OLS_CROWDED,		// ӵ��
		OLS_BEPACKED		// ����
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

	// ����������������
	void InsertOnlineDescConfig(DWORD dwCount, std::string& pzDesc, int iStatus);
	void CleanOnlineDescConfig();
	CDuiString GetOnlineDescByCount(DWORD dwCount);
	int GetOnlineStatusByCount(DWORD dwCount);
	
	// �ȼ�����
	void InsertLevelConfig(DWORD dwLevel, LONGLONG lScore, std::string& pzDesc);
	void CleanLevelConfig();
	CDuiString GetLevelDescByScore(LONGLONG lScore);

	// ����ֵ�ȼ�����
	void InsertXPLevelConfig(DWORD dwLevel, LONGLONG lScore);
	void CleanXPLevelConfig();
	DWORD GetXPLevelByScore(LONGLONG lScore);

	// ��������
	void InsertRoomSeasonConfig(DWORD dwSeasonID, std::string& pzName, std::string& pzDesc);
	void CleanRoomSeasonConfig();
	CDuiString GetRoomSeasonNameByID(DWORD dwSeasonID);
	CDuiString GetRoomSeasonDescByID(DWORD dwSeasonID);

private:
	UserData*									m_userSelf;					// ��ǰ���
	std::map<DWORD,		UserData*>				m_userDataMap;				// �û���Ϣ����
	std::map<DWORD,		OnlineDescConfig*>		m_onlineDescConfigMap;		// ��������������������
	std::map<LONGLONG,	LevelConfig*>			m_levelConfigMap;			// �ȼ���������
	std::map<LONGLONG,	ExperinceConfig*>		m_experinceConfigMap;		// ����ֵ��������
	std::map<DWORD,		RoomSeasonConfig*>		m_roomSeasonConfigMap;		// ���䳡����������


	static DataManager* g_instance;
	DataManager();
};

#endif // __DATAMANAGER_H__