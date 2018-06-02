#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include "serverBase.h"

class LoginSeverDelegate : public ServerDelegate
{
public:
	LoginSeverDelegate(){}
	virtual ~LoginSeverDelegate(){}

public:
	virtual bool OnConnectProc(bool bSuccess){ return false; }														// 网络连接回调
	virtual bool OnConnectSuccessProc(){ return false; };															// 网络连接成功回调
	virtual bool OnConnectFailedProc(){ return false; };															// 网络连接失败回调
	virtual bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient){ return true; }			// Socket断开

public:
	virtual bool OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 登录消息
	virtual bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 短信
	virtual bool OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }	// 列表消息
	virtual bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 道具消息
	virtual bool OnNetwork_System(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }		// 系统消息
	virtual bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 用户消息
	virtual bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// 银行消息
};

class LoginServer : public ServerBase
{
public:
	static LoginServer* GetInstance();
	static bool	IsInstanceExist(){ return g_instance != nullptr; }

public:
	// 设置委托
	void SetDelegate(LoginSeverDelegate* pLoginDelegate, CPaintManagerUI* pManager);
	ServerDelegate* GetDelegate(){ return m_pLoginDelegate; }

	// 建立连接
	void Connect();

	// 读取事件
	bool __cdecl OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient);
	
private:
	LoginServer();
	~LoginServer();
		
private:
	static LoginServer*		g_instance;
	LoginSeverDelegate*		m_pLoginDelegate;

};

#endif // !__LOGIN_SERVER_H__
