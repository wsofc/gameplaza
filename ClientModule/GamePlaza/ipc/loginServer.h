#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include "serverBase.h"

class LoginSeverDelegate : public ServerDelegate
{
public:
	LoginSeverDelegate(){}
	virtual ~LoginSeverDelegate(){}

public:
	virtual bool OnConnectProc(bool bSuccess){ return false; }														// �������ӻص�
	virtual bool OnConnectSuccessProc(){ return false; };															// �������ӳɹ��ص�
	virtual bool OnConnectFailedProc(){ return false; };															// ��������ʧ�ܻص�
	virtual bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient){ return true; }			// Socket�Ͽ�

public:
	virtual bool OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// ��¼��Ϣ
	virtual bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// ����
	virtual bool OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }	// �б���Ϣ
	virtual bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// ������Ϣ
	virtual bool OnNetwork_System(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }		// ϵͳ��Ϣ
	virtual bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// �û���Ϣ
	virtual bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// ������Ϣ
};

class LoginServer : public ServerBase
{
public:
	static LoginServer* GetInstance();
	static bool	IsInstanceExist(){ return g_instance != nullptr; }

public:
	// ����ί��
	void SetDelegate(LoginSeverDelegate* pLoginDelegate, CPaintManagerUI* pManager);
	ServerDelegate* GetDelegate(){ return m_pLoginDelegate; }

	// ��������
	void Connect();

	// ��ȡ�¼�
	bool __cdecl OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient);
	
private:
	LoginServer();
	~LoginServer();
		
private:
	static LoginServer*		g_instance;
	LoginSeverDelegate*		m_pLoginDelegate;

};

#endif // !__LOGIN_SERVER_H__
