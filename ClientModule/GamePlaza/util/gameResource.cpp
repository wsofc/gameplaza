#include "stdafx.h"
#include "gameResource.h"

GameResource*		GameResource::g_instance					= NULL;									//单例

UILIB_RESOURCETYPE	GameResource::g_ResourceType				= UILIB_RESOURCETYPE::UILIB_FILE;		//资源类型
HINSTANCE			GameResource::g_ResourceInst				= NULL;									//资源句柄
LPCTSTR				GameResource::g_ResourceID					= _T("");								//资源ID
int					GameResource::g_SkinResID					= 0;									//皮肤ID
TCHAR				GameResource::g_ResourcePath[MAX_PATH]		= { 0 };								//资源路径
TCHAR				GameResource::g_ResourceDllPath[MAX_PATH]	= { 0 };								//资源Dll路径

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