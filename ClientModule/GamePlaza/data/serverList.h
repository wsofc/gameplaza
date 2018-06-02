#ifndef __SERVERLIST_H__
#define __SERVERLIST_H__

#include "..\\ipc\\loginServer.h"
class ServerList : public LoginSeverDelegate
{
public:

	enum OperateStatus
	{
		OS_NONE				= 0			// 无状态
		, OS_REFRESH_LIST				// 刷新列表
		, OS_EXTRA						// 额外预留项
	};

	enum RefreshType
	{
		RF_TYPE_NONE		= 0			// 无状态
		, RF_TYPE_GAMELIST				// 刷新游戏列表
		, RF_TYPE_ROOMLIST				// 刷新房间列表
	};

	static ServerList* GetInstance();
	~ServerList();

	void RefreshList(CPaintManagerUI* pManagerRef, RefreshType uType);				// 刷新列表信息

public:
	bool OnConnectSuccessProc();
	bool OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);

private:
	void _RefreshListReq();															// 刷新请求
	void _InitGameConfigList(void* pData, WORD wDataSize);							// 游戏列表配置
	void _InitGameTypeList(void* pData, WORD wDataSize);							// 游戏类型列表
	void _InitGameKindList(void* pData, WORD wDataSize);							// 游戏种类列表
	void _initGameStationList(void* pData, WORD wDataSize);							// 游戏站点列表
	void _InitGameRoomList(void* pData, WORD wDataSize);							// 游戏房间列表
	void _InitGameListComplete();													// 游戏列表配置完成

private:

	ServerList();
	static ServerList*		g_instance;

	RefreshType				m_uRefreshType;						// 刷新列表类型(1 - 刷新游戏列表, 2 - 刷新房间列表 不能同时刷新两者,流程不兼容)
	CPaintManagerUI*		m_pManagerRef;						// 窗口管理器引用
	ServiceData*			m_pServiceData;						// 全局服务数据
	LoginServer*			m_pLoginServer;						// 登录服务器接口对象
	OperateStatus			m_iCurOperateStatus;				// 当前操作状态
};

#endif // __SERVERLIST_H__