#include "Stdafx.h"
#include "TCPSocketClient.h"
#include "SafeDelete.h"


//�궨��
#define ID_SOCKET_WND				10								//SOCKET ���� ID
#define WM_SOCKET_NOTIFY			WM_USER+10						//SOCKET ��Ϣ

#define	ID_TMR_DETECT				100			//��ⶨʱ��

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

//�ӿڲ�ѯ
void * __cdecl CTCPSocketClient::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketClient,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketClient,Guid,dwQueryVer);
	return NULL;
}

//���������
bool __cdecl CTCPSocketClient::SetProxyServerInfo(emProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)
{
	m_ProxyServerInfo=ProxyServerInfo;
	return true;
}

//���ýӿ�
bool __cdecl CTCPSocketClient::SetTCPSocketSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pITCPSocketSink = (ITCPSocketClientSink*) pIUnknownEx->QueryInterface(IID_ITCPSocketClientSink, VER_ITCPSocketClientSink);
	ASSERT(m_pITCPSocketSink!=NULL);
	return (m_pITCPSocketSink!=NULL);
}

//��ȡ�ӿ�
void * __cdecl CTCPSocketClient::GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pITCPSocketSink==NULL) return NULL;
	return m_pITCPSocketSink->QueryInterface(Guid,dwQueryVer);
}

//���ӷ�����
DWORD __cdecl CTCPSocketClient::Connect(DWORD dwServerIP, WORD wPort)
{
	//Ч�����
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

	//Ч��״̬
	if (m_hSocket!=INVALID_SOCKET) throw TEXT("���� SOCKET ����Ѿ�����");
	if (m_cbSocketStatus!=SOCKET_STATUS_IDLE) throw TEXT("����״̬���ǵȴ�����״̬");
	if (dwServerIP==INADDR_NONE) throw TEXT("Ŀ���������ַ��ʽ����ȷ��������ٴγ��ԣ�");

	//���ò���
	m_wRecvSize=0;
	m_dwSendTickCount=GetTickCount()/1000L;
	m_dwRecvTickCount=GetTickCount()/1000L;

	// ���ý�����
	m_pPacketParser.Reset();

	try
	{
		// MFC��ʼ��
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
			throw TEXT("MFC��ʼ������");

		//���� SOCKET
		m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket==INVALID_SOCKET) throw TEXT("SOCKET ����ʧ��");
		if ((m_hWnd==NULL)&&(!Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),ID_SOCKET_WND,NULL)))
			throw TEXT("SOCKET �ڲ����ڴ���ʧ��");

		//��д��������ַ
		SOCKADDR_IN SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr=dwServerIP;

		//���ӷ�����
		int iErrorCode=0;
		WSASetLastError(0);
		//if (m_ProxyInfo.cbProxyType!=PROXY_NONE)
		//{
		//	throw TEXT("δ֧�ִ��������");
		//}
		//else
		{
			//�󶨴���
			iErrorCode = WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,FD_READ|FD_CONNECT|FD_CLOSE);
			if (iErrorCode==SOCKET_ERROR) throw TEXT("���ڲ����ڴ���");

			//���ӷ�����
			iErrorCode=connect(m_hSocket,(SOCKADDR *)&SocketAddr,sizeof(SocketAddr));
			if (iErrorCode==SOCKET_ERROR)
			{
				iErrorCode=WSAGetLastError();
				if (iErrorCode!=WSAEWOULDBLOCK)
				{
					static TCHAR szBuffer[64];
					wsprintf(szBuffer, _T("���ӷ������󣬴������ [ %d ]"), iErrorCode);

					throw szBuffer;
				}
			}

			//���ñ���
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
		throw TEXT("���Ӳ���δ֪�쳣����");
		return CONNECT_EXCEPTION;
	}
}

//���ӷ�����
DWORD __cdecl CTCPSocketClient::Connect(const LPCTSTR szServerIP, WORD wPort)
{
	//Ч������
	ASSERT(wPort!=0);
	ASSERT(szServerIP!=NULL);
	if ((szServerIP==NULL)||(wPort==0)) return false;
	return Connect(TranslateAddr(szServerIP),wPort);
}

//���ͺ���
DWORD __cdecl CTCPSocketClient::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	return SendData(wMainCmdID, wSubCmdID, NULL, 0);
}

//���ͺ���
DWORD __cdecl CTCPSocketClient::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_hSocket==INVALID_SOCKET) return false;
	if (m_cbSocketStatus!=SOCKET_STATUS_CONNECT) return false;

	//Ч���С
	ASSERT(wDataSize<=TCP_PACKET_BUFFER_SIZE);
	if (wDataSize>TCP_PACKET_BUFFER_SIZE) return false;

	//��������
	BYTE cbDataBuffer[TCP_PACKET_MAXSIZE];
	CMD_Head* pHead = (CMD_Head*) cbDataBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	if (pData != NULL && wDataSize > 0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(pHead+1,pData,wDataSize);
	}

	// ���
	int nSize = (int)TCP_PACKET_BUFFER_SIZE;
	bool bPackOK = m_pPacketParser.Pack(cbDataBuffer, TCP_PACKET_HEAD_SIZE+wDataSize, cbDataBuffer, nSize);
	if (bPackOK == false)
	{
		TRACE("ѹ��ʧ��:[%d, %d]", wMainCmdID, wSubCmdID);
		return false;
	}

	//��������
	return SendDataBuffer(cbDataBuffer, nSize);
}

void __cdecl CTCPSocketClient::SetDebugOutputHelper(const tagDebugOutputHelper & DebugOutputHelper)
{
	m_DebugOutputHelper = DebugOutputHelper;
	m_DebugOutputHelper.bEnable = true;
}

//�ر�����
void __cdecl CTCPSocketClient::CloseSocket()
{
	if (m_DebugOutputHelper.bEnable)
	{
		CString strLog;
		strLog.Format(_T("%s,%s,%d,�ر�socket,"), 
			m_DebugOutputHelper.szOutputPrefix,
			__FUNCTION__,
			__LINE__);
		::OutputDebugString(strLog);
	}

	//�ر�����
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

	//�ָ�����
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

//��������
DWORD CTCPSocketClient::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//Ч�����
	ASSERT(wSendSize!=0);
	ASSERT(pBuffer!=NULL);

	//��������
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

//���ͷ�������ַ
DWORD CTCPSocketClient::TranslateAddr(LPCTSTR pszServerAddr)
{


	//ת����ַ
	DWORD dwServerIP = 0;
	LPHOSTENT lpHost = nullptr;
	#ifdef _UNICODE
		char* cstr_ip = nullptr;
		DWORD dwMinSize = 0;
		dwMinSize = WideCharToMultiByte(CP_ACP, NULL, pszServerAddr, -1, NULL, 0, NULL, FALSE); //���㳤��
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

//�������
DWORD  CTCPSocketClient::ProxyServerTesting()
{
	return 0;
}

//��������
LRESULT CTCPSocketClient::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//�ж�״̬
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0) m_cbSocketStatus = SOCKET_STATUS_CONNECT;
	else CloseSocket();

	//����֪ͨ
	//TCHAR szErrorDesc[128]=TEXT("");
	//GetConnectError(iErrorCode,szErrorDesc,sizeof(szErrorDesc));
	m_pITCPSocketSink->OnSocketConnect(iErrorCode, this);

	return 1;
}

//�����ȡ
LRESULT CTCPSocketClient::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//��ȡ����
		int iRetCode = recv(m_hSocket, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");
		//ASSERT(m_dwSendPacketCount>0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//��������
		uint16 wPacketSize = 0;
		BYTE cbDataBuffer[PACKET_BUFFER_SIZE];
		CMD_Head * pHead = (CMD_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(CMD_Head))
		{
			//Ч�����
			wPacketSize = pHead->CmdInfo.wDataSize;
			ASSERT(pHead->CmdInfo.cbMessageVer == TCP_PROTOCOL_VER);
			ASSERT(wPacketSize <= PACKET_BUFFER_SIZE);
			if (pHead->CmdInfo.cbMessageVer != TCP_PROTOCOL_VER) throw TEXT("���ݰ��汾����");
			if (wPacketSize > PACKET_BUFFER_SIZE) throw TEXT("���ݰ�̫��");
			if (m_wRecvSize < wPacketSize) return 1;

			//��������
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);


			// ��������
			int wRealySize = PACKET_BUFFER_SIZE;
			try
			{
				bool bUnPackOK = m_pPacketParser.Unpack(cbDataBuffer, wPacketSize, cbDataBuffer, wRealySize);

				ASSERT(bUnPackOK && wRealySize >= TCP_PACKET_HEAD_SIZE);
				if ((wRealySize <= 0) || (bUnPackOK == false))
				{
					TRACE("��Ϣ�������ʧ�ܣ�");
					return false;
				}
			}
			catch (...)
			{
				TRACE("���ʧ�ܣ�����δ֪����");
			}

			//��������
			uint16 wDataSize = wRealySize - sizeof(CMD_Head);
			void* pDataBuffer = cbDataBuffer + sizeof(CMD_Head);
			CMD_Command Command = ((CMD_Head*)cbDataBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//������
					{
						//��������
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);

						//�����������
						this->OnTickDetect();
						break;
					}
				}
				continue;
			}

			//��������
			const bool bSuccess = m_pITCPSocketSink->OnSocketRead(Command, pDataBuffer, wDataSize, this);
			if (bSuccess == false)
			{
				throw TEXT("�������ݰ�����ʧ��");
			}
		};
	}
	catch (char* str)
	{
		char buf[1024] = { 0 };
		_stprintf_s(buf, "��������: %s(%d)\n", str, GetLastError());
		OutputDebugString(buf);

		CloseSocket();
	}

	return 1;
}

//����ر�
LRESULT CTCPSocketClient::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket();
	return 1;
}

void CTCPSocketClient::OnTimer(UINT nIDEvent)
{
	//�������
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
			strLog.Format(_T("%s,%s,%d,������ʱ��,dwOldTickCount:%d,dwNewTickCount:%d,dwDisconnectTickTimes:%d,"), 
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
				strLog.Format(_T("%s,%s,%d,������ʱ��,����,"), 
					m_DebugOutputHelper.szOutputPrefix,
					__FUNCTION__,
					__LINE__);
				::OutputDebugString(strLog);
			}

			//��Ϊ����
			m_pITCPSocketSink->OnSocketClose(SHUT_REASON_TIME_OUT, this);
			this->KillTimer(ID_TMR_DETECT);
		}
	}
}

//SOCKET ��Ϣ�������
LRESULT	CTCPSocketClient::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	//��������
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
		break;

	case FD_READ:		//���ݶ�ȡ
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
		break;

	case FD_CLOSE:		//����ر�
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

//�����������
void CTCPSocketClient::OnTickDetect(void)
{
	//debug����
	if (m_DebugOutputHelper.bEnable)
	{
		CString strLog;
		strLog.Format(_T("%s,%s,%d,�������,dwOldTickCount:%d,dwNewTickCount:%d,dwTickTimeSpace:%d,"), 
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

		//���ö�ʱ��
		this->SetTimer(ID_TMR_DETECT, 
			m_ClientTickDetect.dwTickTimeSpace,
			NULL);
		return ;
	}

	m_ClientTickDetect.dwOldTickCount = m_ClientTickDetect.dwNewTickCount;
	m_ClientTickDetect.dwNewTickCount = ::GetTickCount();
}


//-----------------------------------------------
//����������
DECLARE_CREATE_MODULE(TCPSocketClient)
