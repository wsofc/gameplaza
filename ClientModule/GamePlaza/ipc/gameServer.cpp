#include "stdafx.h"
#include "gameServer.h"

extern GameResource*	R;
extern CStaticLogger	g_Logger;

GameServer* GameServer::g_instance = nullptr;
GameServer::GameServer()
	: m_pGameDelegate(nullptr)
{

}

GameServer::~GameServer()
{
	GameServer::g_instance = nullptr;
}

GameServer* GameServer::GetInstance()
{
	if (GameServer::g_instance == nullptr)
		GameServer::g_instance = new (std::nothrow)GameServer();

	return GameServer::g_instance;
}

void GameServer::SetDelegate(GameSeverDelegate* pGameDelegate, CPaintManagerUI* pManager)
{
	m_pGameDelegate = pGameDelegate;
	ServerBase::SetDelegate(m_pGameDelegate, pManager);
}

bool GameServer::OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient)
{
	bool bRes = true;
	if (m_pGameDelegate)
	{
		switch (cmd.wMainCmdID)
		{
			// 房间登录
			case MAIN_GS_LOGON:
			{
				bRes = m_pGameDelegate->OnNetwork_Logon(cmd, pDataBuffer, wDataSize);
			}break;

			// 配置信息
			case MDM_GR_INFO:
			{
				bRes = m_pGameDelegate->OnNetwork_Info(cmd, pDataBuffer, wDataSize);
			}break;

			// 状态信息
			case MAIN_GS_STATUS:
			{
				bRes = m_pGameDelegate->OnNetwork_Status(cmd, pDataBuffer, wDataSize);
			}break;
			
			// 系统信息
			case MAIN_GS_SYSTEM:
			{
				bRes = m_pGameDelegate->OnNetwork_HallMSG(cmd, pDataBuffer, wDataSize);
			}break;
			
			// 房间信息
			case MAIN_GS_SERVER_INFO:
			{
				bRes = m_pGameDelegate->OnNetwork_ServerInfo(cmd, pDataBuffer, wDataSize);
			}break;
			
			//礼物消息
			case MDM_GF_PRESENT:
			{
				bRes = m_pGameDelegate->OnNetwork_Props(cmd, pDataBuffer, wDataSize);
			}break;

			// 用户信息
			case MDM_GR_USER:
			{
				bRes = m_pGameDelegate->OnNetwork_User(cmd, pDataBuffer, wDataSize);
			}break;
			
			// 管理命令
			case MDM_GR_MANAGER:
			{
				bRes = m_pGameDelegate->OnNetwork_Manager(cmd, pDataBuffer, wDataSize);
			}break;

			//银行消息
			case MDM_GF_BANK:
			{
				bRes = m_pGameDelegate->OnNetwork_Bank(cmd, pDataBuffer, wDataSize);
			}break;

			// 游戏消息(状态等外围消息)
			case MDM_GF_FRAME:
			{
				bRes = m_pGameDelegate->OnNetwork_GameFrame(cmd, pDataBuffer, wDataSize);
			}break;

			//游戏消息(逻辑消息)
			case MDM_GF_GAME:
			{
				bRes = m_pGameDelegate->OnNetwork_Game(cmd, pDataBuffer, wDataSize);
			}break;

			default:
				break;
		}
	}

	if (!bRes)
	{
		g_Logger->TryWarn(_T("GameServer - OnSocketRead False [cmid = %u, csid = %u, size = %u]"), cmd.wMainCmdID, cmd.wSubCmdID, wDataSize);
	}

	return bRes;
}
