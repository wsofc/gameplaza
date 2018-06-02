#include "Stdafx.h"
#include "TCPSocketClient.h"
#include "SafeDelete.h"


//宏定义
#define ID_SOCKET_WND				10								//SOCKET 窗口 ID
#define WM_SOCKET_NOTIFY			WM_USER+10						//SOCKET 消息

#define	ID_TMR_DETECT				100			//检测定时器

//-----------------------------------------------

BEGIN_MESSAGE_MAP(CTCPSocketClient, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY,OnSocketNotify)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CTCPSocketClient::CTCPSocketClient()
{
	m_wRecvSize=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;
	m_hSocket=INVALID_SOCKET;
	m_pITCPSocketSink=NULL;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;
	m_ClientTickDetect.Init();
}

CTCPSocketClient::~CTCPSocketClient()
{
	CloseSocket();
}

//接口查询
void * __cdecl CTCPSocketClient::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketClient,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketClient,Guid,dwQueryVer);
	return NULL;
}

//代理服务器
bool __cdecl CTCPSocketClient::SetProxyServerInfo(emProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)
{
	m_ProxyServerInfo=ProxyServerInfo;
	return true;
}

//设置接口
bool __cdecl CTCPSocketClient::SetTCPSocketSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pITCPSocketSink = (ITCPSocketClientSink*) pIUnknownEx->QueryInterface(IID_ITCPSocketClientSink, VER_ITCPSocketClientSink);
	ASSERT(m_pITCPSocketSink!=NULL);
	return (m_pITCPSocketSink!=NULL);
}

//获取接口
void * __cdecl CTCPSocketClient::GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pITCPSocketSink==NULL) return NULL;
	return m_pITCPSocketSink->QueryInterface(Guid,dwQueryVer);
}

//连接服务器
DWORD __cdecl CTCPSocketClient::Connect(DWORD dwServerIP, WORD wPort)
{
	//效验参数
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

	//效验状态
	if (m_hSocket!=INVALID_SOCKET) throw TEXT("连接 SOCKET 句柄已经存在");
	if (m_cbSocketStatus!=SOCKET_STATUS_IDLE) throw TEXT("连接状态不是等待连接状态");
	if (dwServerIP==INADDR_NONE) throw TEXT("目标服务器地址格式不正确，请检查后再次尝试！");

	//设置参数
	m_wRecvSize=0;
	m_dwSendTickCount=GetTickCount()/1000L;
	m_dwRecvTickCount=GetTickCount()/1000L;

	// 重置解析器
	m_pPacketParser.Reset();

	try
	{
		// MFC初始化
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
			throw TEXT("MFC初始化错误！");

		//建立 SOCKET
		m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket==INVALID_SOCKET) throw TEXT("SOCKET 创建失败");
		if ((m_hWnd==NULL)&&(!Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),ID_SOCKET_WND,NULL)))
			throw TEXT("SOCKET 内部窗口创建失败");

		//填写服务器地址
		SOCKADDR_IN SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr=dwServerIP;

		//连接服务器
		int iErrorCode=0;
		WSASetLastError(0);
		//if (m_ProxyInfo.cbProxyType!=PROXY_NONE)
		//{
		//	throw TEXT("未支持代理服务器");
		//}
		//else
		{
			//绑定窗口
			iErrorCode = WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,FD_READ|FD_CONNECT|FD_CLOSE);
			if (iErrorCode==SOCKET_ERROR) throw TEXT("绑定内部窗口错误");

			//连接服务器
			iErrorCode=connect(m_hSocket,(SOCKADDR *)&SocketAddr,sizeof(SocketAddr));
			if (iErrorCode==SOCKET_ERROR)
			{
				iErrorCode=WSAGetLastError();
				if (iErrorCode!=WSAEWOULDBLOCK)
				{
					static TCHAR szBuffer[64];
					wsprintf(szBuffer, _T("连接发生错误，错误代码 [ %d ]"), iErrorCode);

					throw szBuffer;
				}
			}

			//设置变量
			m_cbSocketStatus=SOCKET_STATUS_WAIT;
		}

		return CONNECT_SUCCESS;
	}
	catch (LPCTSTR pszError)
	{
		CloseSocket();
		throw pszError;
		return CONNECT_FAILURE;
	}
	catch (...)
	{
		CloseSocket();
		throw TEXT("连接产生未知异常错误");
		return CONNECT_EXCEPTION;
	}
}

//连接服务器
DWORD __cdecl CTCPSocketClient::Connect(const LPCTSTR szServerIP, WORD wPort)
{
	//效验数据
	ASSERT(wPort!=0);
	ASSERT(szServerIP!=NULL);
	if ((szServerIP==NULL)||(wPort==0)) return false;
	return Connect(TranslateAddr(szServerIP),wPort);
}

//发送函数
DWORD __cdecl CTCPSocketClient::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	return SendData(wMainCmdID, wSubCmdID, NULL, 0);
}

//发送函数
DWORD __cdecl CTCPSocketClient::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效验状态
	if (m_hSocket==INVALID_SOCKET) return false;
	if (m_cbSocketStatus!=SOCKET_STATUS_CONNECT) return false;

	//效验大小
	ASSERT(wDataSize<=TCP_PACKET_BUFFER_SIZE);
	if (wDataSize>TCP_PACKET_BUFFER_SIZE) return false;

	//构造数据
	BYTE cbDataBuffer[TCP_PACKET_MAXSIZE];
	CMD_Head* pHead = (CMD_Head*) cbDataBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	if (pData != NULL && wDataSize > 0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(pHead+1,pData,wDataSize);
	}

	// 封包
	int nSize = (int)TCP_PACKET_BUFFER_SIZE;
	bool bPackOK = m_pPacketParser.Pack(cbDataBuffer, TCP_PACKET_HEAD_SIZE+wDataSize, cbDataBuffer, nSize);
	if (bPackOK == false)
	{
		TRACE("压包失败:[%d, %d]", wMainCmdID, wSubCmdID);
		return false;
	}

	//发送数据
	return SendDataBuffer(cbDataBuffer, nSize);
}

void __cdecl CTCPSocketClient::SetDebugOutputHelper(const tagDebugOutputHelper & DebugOutputHelper)
{
	m_DebugOutputHelper = DebugOutputHelper;
	m_DebugOutputHelper.bEnable = true;
}

//关闭连接
void __cdecl CTCPSocketClient::CloseSocket()
{
	if (m_DebugOutputHelper.bEnable)
	{
		CString strLog;
		strLog.Format(_T("%s,%s,%d,关闭socket,"), 
			m_DebugOutputHelper.szOutputPrefix,
			__FUNCTION__,
			__LINE__);
		::OutputDebugString(strLog);
	}

	//关闭连接
	bool bClose=(m_hSocket!=INVALID_SOCKET);
	m_cbSocketStatus=SOCKET_STATUS_IDLE;
	if (m_hSocket!=INVALID_SOCKET)
	{
		WSAAsyncSelect(m_hSocket,GetSafeHwnd(), WM_SOCKET_NOTIFY,0);
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
		m_cbSocketStatus=SOCKET_STATUS_IDLE;
	}
	if ((bClose==true) && (m_pITCPSocketSink!=NULL))
	{
		ASSERT(m_pITCPSocketSink!=NULL);
		try { m_pITCPSocketSink->OnSocketClose(SHUT_REASON_NORMAL, this); }
		catch (...) {}
	}

	//恢复数据
	m_wRecvSize=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;
	m_ClientTickDetect.Init();
	if (::IsWindow(m_hWnd))
	{
		this->KillTimer(ID_TMR_DETECT);
	}
	return;
}

//发送数据
DWORD CTCPSocketClient::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//效验参数
	ASSERT(wSendSize!=0);
	ASSERT(pBuffer!=NULL);

	//发送数据
	WORD wSended=0;
	while (wSended<wSendSize)
	{
		int iErrorCode=send(m_hSocket,(char *)pBuffer+wSended,wSendSize-wSended,0);
		if (iErrorCode==SOCKET_ERROR)
		{
			if (WSAGetLastError()==WSAEWOULDBLOCK)
			{
				m_dwSendTickCount=GetTickCount()/1000L;
				return true;
			}
			return false;
		}
		wSended+=iErrorCode;
	}
	m_dwSendTickCount=GetTickCount()/1000L;

	return true;
}

//解释服务器地址
DWORD CTCPSocketClient::TranslateAddr(LPCTSTR pszServerAddr)
{


	//转化地址
	DWORD dwServerIP = 0;
	LPHOSTENT lpHost = nullptr;
	#ifdef _UNICODE
		char* cstr_ip = nullptr;
		DWORD dwMinSize = 0;
		dwMinSize = WideCharToMultiByte(CP_ACP, NULL, pszServerAddr, -1, NULL, 0, NULL, FALSE); //计算长度
		cstr_ip = new char[dwMinSize];
		WideCharToMultiByte(CP_OEMCP, NULL, pszServerAddr, -1, cstr_ip, dwMinSize, NULL, FALSE);
		dwServerIP = inet_addr(cstr_ip);
		lpHost = gethostbyname(cstr_ip);
		SAFE_DELETE(cstr_ip);
	#else
		dwServerIP = inet_addr(pszServerAddr);
		lpHost = gethostbyname(pszServerAddr);
	#endif

	if (dwServerIP == INADDR_NONE)
	{
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

//代理测试
DWORD  CTCPSocketClient::ProxyServerTesting()
{
	return 0;
}

//网络连接
LRESULT CTCPSocketClient::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//判断状态
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0) m_cbSocketStatus = SOCKET_STATUS_CONNECT;
	else CloseSocket();

	//发送通知
	//TCHAR szErrorDesc[128]=TEXT("");
	//GetConnectError(iErrorCode,szErrorDesc,sizeof(szErrorDesc));
	m_pITCPSocketSink->OnSocketConnect(iErrorCode, this);

	return 1;
}

//网络读取
LRESULT CTCPSocketClient::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//读取数据
		int iRetCode = recv(m_hSocket, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("网络连接关闭，读取数据失败");
		//ASSERT(m_dwSendPacketCount>0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//变量定义
		uint16 wPacketSize = 0;
		BYTE cbDataBuffer[PACKET_BUFFER_SIZE];
		CMD_Head * pHead = (CMD_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(CMD_Head))
		{
			//效验参数
			wPacketSize = pHead->CmdInfo.wDataSize;
			ASSERT(pHead->CmdInfo.cbMessageVer == TCP_PROTOCOL_VER);
			ASSERT(wPacketSize <= PACKET_BUFFER_SIZE);
			if (pHead->CmdInfo.cbMessageVer != TCP_PROTOCOL_VER) throw TEXT("数据包版本错误");
			if (wPacketSize > PACKET_BUFFER_SIZE) throw TEXT("数据包太大");
			if (m_wRecvSize < wPacketSize) return 1;

			//拷贝数据
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);


			// 解密数据
			int wRealySize = PACKET_BUFFER_SIZE;
			try
			{
				bool bUnPackOK = m_pPacketParser.Unpack(cbDataBuffer, wPacketSize, cbDataBuffer, wRealySize);

				ASSERT(bUnPackOK && wRealySize >= TCP_PACKET_HEAD_SIZE);
				if ((wRealySize <= 0) || (bUnPackOK == false))
				{
					TRACE("消息处理：解包失败！");
					return false;
				}
			}
			catch (...)
			{
				TRACE("解包失败，发生未知错误！");
			}

			//解释数据
			uint16 wDataSize = wRealySize - sizeof(CMD_Head);
			void* pDataBuffer = cbDataBuffer + sizeof(CMD_Head);
			CMD_Command Command = ((CMD_Head*)cbDataBuffer)->CommandInfo;

			//内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//网络检测
					{
						//发送数据
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);

						//处理心跳检测
						this->OnTickDetect();
						break;
					}
				}
				continue;
			}

			//处理数据
			const bool bSuccess = m_pITCPSocketSink->OnSocketRead(Command, pDataBuffer, wDataSize, this);
			if (bSuccess == false)
			{
				throw TEXT("网络数据包处理失败");
			}
		};
	}
	catch (char* str)
	{
		char buf[1024] = { 0 };
		_stprintf_s(buf, "网络层错误: %s(%d)\n", str, GetLastError());
		OutputDebugString(buf);

		CloseSocket();
	}

	return 1;
}

//网络关闭
LRESULT CTCPSocketClient::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket();
	return 1;
}

void CTCPSocketClient::OnTimer(UINT nIDEvent)
{
	//心跳检测
	if (nIDEvent == ID_TMR_DETECT)
	{
		if (m_ClientTickDetect.dwTickTimeSpace <= 0)
		{
			return ;
		}

		m_ClientTickDetect.dwNewTickCount = ::GetTickCount();
		m_ClientTickDetect.dwDisconnectTickTimes = (m_ClientTickDetect.dwNewTickCount - m_ClientTickDetect.dwOldTickCount)/m_ClientTickDetect.dwTickTimeSpace;

		if (m_DebugOutputHelper.bEnable)
		{
			CString strLog;
			strLog.Format(_T("%s,%s,%d,心跳定时器,dwOldTickCount:%d,dwNewTickCount:%d,dwDisconnectTickTimes:%d,"), 
				m_DebugOutputHelper.szOutputPrefix,
				__FUNCTION__,
				__LINE__,
				m_ClientTickDetect.dwOldTickCount,
				m_ClientTickDetect.dwNewTickCount,
				m_ClientTickDetect.dwDisconnectTickTimes);
			::OutputDebugString(strLog);
		}

		if (m_ClientTickDetect.dwDisconnectTickTimes > m_ClientTickDetect.dwMaxDisconnectTickTimes)
		{
			if (m_DebugOutputHelper.bEnable)
			{
				CString strLog;
				strLog.Format(_T("%s,%s,%d,心跳定时器,断线,"), 
					m_DebugOutputHelper.szOutputPrefix,
					__FUNCTION__,
					__LINE__);
				::OutputDebugString(strLog);
			}

			//认为断线
			m_pITCPSocketSink->OnSocketClose(SHUT_REASON_TIME_OUT, this);
			this->KillTimer(ID_TMR_DETECT);
		}
	}
}

//SOCKET 消息处理程序
LRESULT	CTCPSocketClient::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	//网络连接
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
		break;

	case FD_READ:		//数据读取
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
		break;

	case FD_CLOSE:		//网络关闭
		{ 
			return OnSocketNotifyClose(wParam,lParam);	
		}
		break;

	default:
		{
			;
		}
		break;
	}

	return 0;
}

//处理心跳检测
void CTCPSocketClient::OnTickDetect(void)
{
	//debug测试
	if (m_DebugOutputHelper.bEnable)
	{
		CString strLog;
		strLog.Format(_T("%s,%s,%d,心跳检测,dwOldTickCount:%d,dwNewTickCount:%d,dwTickTimeSpace:%d,"), 
			m_DebugOutputHelper.szOutputPrefix,
			__FUNCTION__,
			__LINE__,
			m_ClientTickDetect.dwOldTickCount,
			m_ClientTickDetect.dwNewTickCount,
			m_ClientTickDetect.dwTickTimeSpace);
		::OutputDebugString(strLog);
	}

	if (m_ClientTickDetect.dwOldTickCount == 0)
	{
		m_ClientTickDetect.dwOldTickCount = ::GetTickCount();
		return;
	}

	if (m_ClientTickDetect.dwNewTickCount == 0)
	{
		m_ClientTickDetect.dwNewTickCount = ::GetTickCount();
		m_ClientTickDetect.dwTickTimeSpace = m_ClientTickDetect.dwNewTickCount - m_ClientTickDetect.dwOldTickCount;

		m_ClientTickDetect.dwOldTickCount = m_ClientTickDetect.dwNewTickCount;

		//设置定时器
		this->SetTimer(ID_TMR_DETECT, 
			m_ClientTickDetect.dwTickTimeSpace,
			NULL);
		return ;
	}

	m_ClientTickDetect.dwOldTickCount = m_ClientTickDetect.dwNewTickCount;
	m_ClientTickDetect.dwNewTickCount = ::GetTickCount();
}


//-----------------------------------------------
//建立对象函数
DECLARE_CREATE_MODULE(TCPSocketClient)
