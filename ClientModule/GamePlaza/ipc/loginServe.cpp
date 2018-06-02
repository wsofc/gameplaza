#include "stdafx.h"
#include "loginServer.h"

extern GameResource*	R;
extern CStaticLogger	g_Logger;

LoginServer* LoginServer::g_instance = nullptr;
LoginServer::LoginServer()
	: m_pLoginDelegate(nullptr)
{

}

LoginServer::~LoginServer()
{
	LoginServer::g_instance = nullptr;
}

LoginServer* LoginServer::GetInstance()
{
	if (LoginServer::g_instance == nullptr)
		LoginServer::g_instance = new (std::nothrow)LoginServer();

	return LoginServer::g_instance;
}

void LoginServer::SetDelegate(LoginSeverDelegate* pLoginDelegate, CPaintManagerUI* pManager)
{
	m_pLoginDelegate = pLoginDelegate;
	ServerBase::SetDelegate(m_pLoginDelegate, pManager);
}

void LoginServer::Connect()
{
	ServerBase::Connect(0, R->LoginServer_IP, PORT_LOGON_SERVER);
}

bool LoginServer::OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient)
{
	bool bRes = true;
	if (m_pLoginDelegate)
	{
		switch (cmd.wMainCmdID)
		{
			//登录消息
			case MAIN_LS_LOGON:
			{
				bRes = m_pLoginDelegate->OnNetwork_Logon(cmd, pDataBuffer, wDataSize);
			}break;

			//短信
			case MAIN_LS_SMS:
			{
				bRes = m_pLoginDelegate->OnNetwork_SMS(cmd, pDataBuffer, wDataSize);
			}break;

			//列表消息
			case MAIN_LS_SERVER_LIST:
			{
				bRes = m_pLoginDelegate->OnNetwork_ServerList(cmd, pDataBuffer, wDataSize);
			}break;

			//礼物消息
			case MDM_GF_PRESENT:
			{
				bRes = m_pLoginDelegate->OnNetwork_Props(cmd, pDataBuffer, wDataSize);
			}break;

			//系统消息
			case MAIN_LS_SYSTEM:
			{
				bRes = m_pLoginDelegate->OnNetwork_System(cmd, pDataBuffer, wDataSize);
			}break;

			//用户消息
			case MAIN_LS_USER:
			{
				bRes = m_pLoginDelegate->OnNetwork_User(cmd, pDataBuffer, wDataSize);
			}break;

			//银行消息
			case MDM_GF_BANK:
			{
				bRes = m_pLoginDelegate->OnNetwork_Bank(cmd, pDataBuffer, wDataSize);
			}break;

			default:
				break;
		}
	}

	if (!bRes)
	{
		g_Logger->TryWarn(_T("LoginServer - OnSocketRead False [cmid = %u, csid = %u, size = %u]"), cmd.wMainCmdID, cmd.wSubCmdID, wDataSize);
	}

	return bRes;
}
