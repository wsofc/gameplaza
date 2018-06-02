#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "serverBase.h"

class GameSeverDelegate: public ServerDelegate
{
public:
	GameSeverDelegate(){}
	~GameSeverDelegate(){}

public:
	virtual bool OnConnectProc(bool bSuccess){ return false; }														// �������ӻص�
	virtual bool OnConnectSuccessProc(){ return false; };															// �������ӳɹ��ص�
	virtual bool OnConnectFailedProc(){ return false; };															// ��������ʧ�ܻص�
	virtual bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient){ return true; }			// Socket�Ͽ��ص�

public:
	virtual bool OnNetwork_Logon(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// �����¼
	virtual bool OnNetwork_Status(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// ������Ϣ
	virtual bool OnNetwork_HallMSG(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// ϵͳ��Ϣ
	virtual bool OnNetwork_ServerInfo(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }	// ������Ϣ
	virtual bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// ������Ϣ
	virtual bool OnNetwork_User(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// �û���Ϣ
	virtual bool OnNetwork_Info(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// ������Ϣ
	virtual bool OnNetwork_Manager(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// ��������
	virtual bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize){ return true; }			// ������Ϣ
	virtual bool OnNetwork_Game(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }			// ��Ϸ�߼���Ϣ
	virtual bool OnNetwork_GameFrame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize){ return true; }		// ��Ϸ״̬��Ϣ
	
};

class GameServer : public ServerBase
{
public:
	static GameServer* GetInstance();
	static bool	IsInstanceExist(){ return g_instance != nullptr; }

public:
	// ����ί��
	void SetDelegate(GameSeverDelegate* pGameDelegate, CPaintManagerUI* pManager);
	ServerDelegate* GetDelegate(){ return m_pGameDelegate; }

	// ��ȡ�¼�
	bool __cdecl OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient);
	
private:
	GameServer();
	~GameServer();

private:
	static GameServer*	g_instance;
	GameSeverDelegate*	m_pGameDelegate;
};

#endif // !__GAME_SERVER_H__
