#include "stdafx.h"
#include "serverBase.h"

extern GameResource*	R;
extern CStaticLogger	g_Logger;

ServerDelegate::ServerDelegate()
	: m_pManager(nullptr)
{

}

ServerDelegate::~ServerDelegate()
{

}

ServerBase::ServerBase()
	: m_bConnect(false)
	, m_pManager(nullptr)
	, m_pDelegate(nullptr)
	, m_wPort(0)
	, m_dwServerIP(0)
{
	memset(m_szServerIP, 0, IP_MAX_BUFF);
}

ServerBase::~ServerBase()
{

}
void ServerBase::SetDelegate(ServerDelegate* pDelegate, CPaintManagerUI* pManager)
{
	m_pDelegate = pDelegate;
	m_pManager = pManager;

	if (m_pDelegate)
	{
		m_pDelegate->SetPaintManager(m_pManager);
	}
}

void ServerBase::Connect(DWORD dwServerIP, LPCTSTR szServerIP, WORD wPort)
{
	g_Logger->TryDebug(_T("ServerBase - Connect Server [dwip = %lu, strip = %s, port = %u]"), dwServerIP, szServerIP, wPort);

	m_wPort = wPort;
	m_dwServerIP = dwServerIP;
	memset(m_szServerIP, 0, IP_MAX_BUFF);
	if (szServerIP)
	{
		::StrToCharArray(m_szServerIP, szServerIP);
	}
		
	if (!this->_SetupConnect())
	{
		m_bConnect = false;
	}
}

void ServerBase::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	TCHAR szServerIP[IP_MAX_BUFF] = { 0 };
	::StrToCharArray(szServerIP, m_szServerIP);

	// 因为m_szServerIP在Connect内部有置空操作，所以要先拷贝一份作为形参传入
	this->Connect(m_dwServerIP, szServerIP, m_wPort);
}

bool ServerBase::_SetupConnect()
{
	//创建组件
	if (m_ClientSocket.GetInterface() == NULL)
	{
		try
		{
			IUnknownEx * pIUnknownEx = QUERY_ME_INTERFACE(ITCPSocketClientSink);
			if (m_ClientSocket.CreateInstance() == false)
			{
				throw _T("网络组件创建失败");
			}
			if (m_ClientSocket->SetTCPSocketSink(pIUnknownEx) == false)
			{
				throw _T("网络组件回调接口设置失败");
			}
		}
		catch (...)
		{
			DialogFrame::MsgBox(m_pManager, _T("温馨提示:"), _T("网络组件创建失败,请重新下载游戏大厅!"));
			return false;
		}
	}

	//连接服务器
	try
	{
		//连接服务器
		DWORD bRes = CONNECT_FAILURE;
		m_ClientSocket->CloseSocket();
		if (_tcscmp(m_szServerIP, _T("")) != 0)
		{
			bRes = m_ClientSocket->Connect(m_szServerIP, m_wPort);
		}
		else
		{
			bRes = m_ClientSocket->Connect(m_dwServerIP, m_wPort);
		}

		if (bRes != CONNECT_SUCCESS)
		{
			throw _T("服务器连接错误,可能是你的系统还没有成功连接上网络!");
		}
	}
	catch (...)
	{
		// 错误信息请查看 pszError 字符串
		// 对话框只显示和谐透明的字符串提示
		DialogCallBack dlg_cb = CC_CALLBACK_2(ServerBase::ReConnect, this);
		DialogFrame::MsgBox(m_pManager
			, _T("温馨提示:")
			, _T("服务器连接错误,可能是你的系统还没有成功连接上网络!")
			, DLG_ID(ServerBaseMsxBoxID::MSG_BOX_ID_NET_CONNECT_FAILED)
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);

		return false;
	}

	return true;
}

DWORD ServerBase::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	if (NULL != m_ClientSocket.GetInterface())
	{
		return m_ClientSocket->SendData(wMainCmdID, wSubCmdID);
	}

	return 0;
}

DWORD ServerBase::SendData(WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize)
{
	if (NULL != m_ClientSocket.GetInterface())
	{
		return  m_ClientSocket->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
	}

	return 0;
}

void ServerBase::CloseSocket()
{
	g_Logger->TryDebug(_T("ServerBase - CloseSocket"));

	if (NULL != m_ClientSocket.GetInterface())
	{
		m_ClientSocket->CloseSocket();
	}
}

void* ServerBase::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketClientSink, Guid, dwQueryVer);
	return NULL;
}

void ServerBase::Release()
{
	this->SetDelegate(nullptr, nullptr);
	this->CloseSocket();

	delete this;
}

bool ServerBase::OnSocketConnect(INT nErrorCode, ITCPSocketClient* pITCPSocketClient)
{
	if (ERROR_SUCCESS == nErrorCode)
	{
		m_bConnect = true;
		if (m_pDelegate)
		{
			if (!m_pDelegate->OnConnectProc(true))
			{
				m_pDelegate->OnConnectSuccessProc();
			}
		}
	}
	else
	{
		m_bConnect = false;
		bool bShowTips = !m_pDelegate;
		if (m_pDelegate)
		{
			if (!m_pDelegate->OnConnectProc(false))
			{
				if (!m_pDelegate->OnConnectFailedProc())
				{
					bShowTips = true;
				}
			}
		}

		if (bShowTips)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(ServerBase::ReConnect, this);
			DialogFrame::MsgBox(m_pManager
				, _T("温馨提示:")
				, _T("由于网络故障，与服务器断开连接,请稍后再试!")
				, DLG_ID(ServerBaseMsxBoxID::MSG_BOX_ID_NET_CONNECT_FAILED)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	}

	return true;
}

bool ServerBase::OnSocketClose(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	g_Logger->TryDebug(_T("ServerBase - OnSocketClose [ShutReason = %u]"), cbShutReason);

	m_bConnect = false;
	if (m_pDelegate)
	{
		return m_pDelegate->OnSocketCloseProc(cbShutReason, pITCPSocketClient);
	}

	return true;
}
