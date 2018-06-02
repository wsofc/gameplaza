#include "stdafx.h"
#include "gameResource.h"

GameResource*		GameResource::g_instance					= NULL;									//����

UILIB_RESOURCETYPE	GameResource::g_ResourceType				= UILIB_RESOURCETYPE::UILIB_FILE;		//��Դ����
HINSTANCE			GameResource::g_ResourceInst				= NULL;									//��Դ���
LPCTSTR				GameResource::g_ResourceID					= _T("");								//��ԴID
int					GameResource::g_SkinResID					= 0;									//Ƥ��ID
TCHAR				GameResource::g_ResourcePath[MAX_PATH]		= { 0 };								//��Դ·��
TCHAR				GameResource::g_ResourceDllPath[MAX_PATH]	= { 0 };								//��ԴDll·��

GameResource* GameResource::GetInstance()
{
	if (GameResource::g_instance)
		return GameResource::g_instance;

	GameResource::g_instance = new GameResource();
	return GameResource::g_instance;
}

GameResource::GameResource()
{
	memset(LoginServer_IP, 0, PWD_LEN * sizeof(TCHAR));
}

GameResource::~GameResource()
{

}