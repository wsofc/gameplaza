#ifndef __SERVER_BASE_H__
#define __SERVER_BASE_H__

#define IP_MAX_BUFF		20

class ServerDelegate
{
public:
	ServerDelegate();
	virtual ~ServerDelegate();

public:
	void SetPaintManager(CPaintManagerUI* pManager){ m_pManager = pManager; }						// 设置绘图管理器

	//@return 返回true则消息被拦截, false则继续传递OnConnectSuccessProc或OnConnectFailedProc
	virtual bool OnConnectProc(bool bSuccess)	= 0;												// 网络连接回调
	virtual bool OnConnectSuccessProc()			= 0;												// 网络连接成功回调
	virtual bool OnConnectFailedProc()			= 0;												// 网络连接失败回调
	virtual bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient) = 0;		// Socket断开回调

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
		MSG_BOX_ID_CLIENT_DAMAGED,		// 客户端已损坏
		MSG_BOX_ID_NET_COM_CREATFAILED,	// 网络组件创建失败
		MSG_BOX_ID_NET_CONNECT_FAILED,	// 网络连接失败
		MSG_BOX_ID_RECONNECT,			// 网络重连
		MSG_BOX_ID_NET_DISCONNECTION,	// 网络断开
	};

public:
	// 设置委托
	void SetDelegate(ServerDelegate* pDelegate, CPaintManagerUI* pManager);
	ServerDelegate* GetDelegate(){ return m_pDelegate; }

	// 连接
	void Connect(DWORD dwServerIP, LPCTSTR szServerIP, WORD wPort);
	void ReConnect(DLG_ID id, TNotifyUI& msg);
	bool IsConnect(){ return m_bConnect; }

	// 发送
	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);

	// 关闭连接
	void CloseSocket();

public:

	//接口查询
	void * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//释放对象
	void __cdecl Release();

	// 连接事件
	bool __cdecl OnSocketConnect(INT nErrorCode, ITCPSocketClient* pITCPSocketClient);

	// 关闭事件
	bool __cdecl OnSocketClose(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);

	// 读取事件
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
