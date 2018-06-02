#ifndef __SERVER_BASE_H__
#define __SERVER_BASE_H__

#define IP_MAX_BUFF		20

class ServerDelegate
{
public:
	ServerDelegate();
	virtual ~ServerDelegate();

public:
	void SetPaintManager(CPaintManagerUI* pManager){ m_pManager = pManager; }						// ���û�ͼ������

	//@return ����true����Ϣ������, false���������OnConnectSuccessProc��OnConnectFailedProc
	virtual bool OnConnectProc(bool bSuccess)	= 0;												// �������ӻص�
	virtual bool OnConnectSuccessProc()			= 0;												// �������ӳɹ��ص�
	virtual bool OnConnectFailedProc()			= 0;												// ��������ʧ�ܻص�
	virtual bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient) = 0;		// Socket�Ͽ��ص�

private:
	CPaintManagerUI*	m_pManager;
	
};

class ServerBase : public ITCPSocketClientSink
{
public:
	ServerBase();
	virtual ~ServerBase();

	enum ServerBaseMsxBoxID
	{
		MSG_BOX_ID_CLIENT_DAMAGED,		// �ͻ�������
		MSG_BOX_ID_NET_COM_CREATFAILED,	// �����������ʧ��
		MSG_BOX_ID_NET_CONNECT_FAILED,	// ��������ʧ��
		MSG_BOX_ID_RECONNECT,			// ��������
		MSG_BOX_ID_NET_DISCONNECTION,	// ����Ͽ�
	};

public:
	// ����ί��
	void SetDelegate(ServerDelegate* pDelegate, CPaintManagerUI* pManager);
	ServerDelegate* GetDelegate(){ return m_pDelegate; }

	// ����
	void Connect(DWORD dwServerIP, LPCTSTR szServerIP, WORD wPort);
	void ReConnect(DLG_ID id, TNotifyUI& msg);
	bool IsConnect(){ return m_bConnect; }

	// ����
	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);

	// �ر�����
	void CloseSocket();

public:

	//�ӿڲ�ѯ
	void * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�ͷŶ���
	void __cdecl Release();

	// �����¼�
	bool __cdecl OnSocketConnect(INT nErrorCode, ITCPSocketClient* pITCPSocketClient);

	// �ر��¼�
	bool __cdecl OnSocketClose(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);

	// ��ȡ�¼�
	virtual bool __cdecl OnSocketRead(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient) = 0;
	
private:
	bool _SetupConnect();
	
protected:
	DWORD					m_dlgWaitID;
	CPaintManagerUI*		m_pManager;

private:
	bool					m_bConnect;
	ServerDelegate*			m_pDelegate;
	CTCPSocketClientHelper	m_ClientSocket;
	TCHAR					m_szServerIP[IP_MAX_BUFF];
	DWORD					m_dwServerIP;
	WORD					m_wPort;

};

#endif // !__SERVER_BASE_H__
