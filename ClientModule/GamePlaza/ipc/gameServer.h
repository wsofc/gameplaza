#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "serverBase.h"

class GameSeverDelegate: public ServerDelegate
{
public:
	GameSeverDelegate(){}
	~GameSeverDelegate(){}

public:
	virtual bool OnConnectProc(bool bSuccess){ return false; }														// 网络连接回调
	virtual bool OnConnectSuccessProc(){ return false; };															// 网络连接成功回调
	virtual bool OnConnectFailedProc(){ return false; };															// 网络连接失败回调
	virtual bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient){ return true; }			// Socket断开回调

public:
	virtual bool OnNetwork_Logon(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// 房间登录
	virtual bool OnNetwork_Status(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// 配置信息
	virtual bool OnNetwork_HallMSG(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// 系统信息
	virtual bool OnNetwork_ServerInfo(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }	// 房间信息
	virtual bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 道具消息
	virtual bool OnNetwork_User(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// 用户信息
	virtual bool OnNetwork_Info(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// 配置信息
	virtual bool OnNetwork_Manager(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// 管理命令
	virtual bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 银行消息
	virtual bool OnNetwork_Game(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// 游戏逻辑信息
	virtual bool OnNetwork_GameFrame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// 游戏状态消息
	
};

class GameServer : public ServerBase
{
public:
	static GameServer* GetInstance();
	static bool	IsInstanceExist(){ return g_instance != nullptr; }

public:
	// 设置委托
	void SetDelegate(GameSeverDelegate* pGameDelegate, CPaintManagerUI* pManager);
	ServerDelegate* GetDelegate(){ return m_pGameDelegate; }

	// 读取事件
	bool __cdecl OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient);
	
private:
	GameServer();
	~GameServer();

private:
	static GameServer*	g_instance;
	GameSeverDelegate*	m_pGameDelegate;
};

#endif // !__GAME_SERVER_H__
