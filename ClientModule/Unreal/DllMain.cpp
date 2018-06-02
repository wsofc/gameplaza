#include "stdafx.h"

// 定义全局变量
CStaticLogger	g_Logger;
GameResource*	R				= nullptr;
DataManager*	g_pDataManager	= nullptr;
ServiceData*	g_pServiceData	= nullptr;

// 初始化日志组件
void InitLogger()
{
	LPCTSTR lf = NULL;
	ILogger::LogLevel ll = ILogger::DEFAULT_LOG_LEVEL;
	int pf = ILogger::PRINT_FLAG_FILE | ILogger::PRINT_FLAG_SCREEN;

	if (g_Logger->Init(lf, ll, pf))
	{
		g_Logger->TryDebug(_T("UnrealMode Start!"));
	}
	else
	{
		ILogger::ErrorCode eCode = g_Logger->GetLastError();
		LPCTSTR eDescMsg = ::ILogger_GetErrorDesc(eCode);
		g_Logger->UnInit();

		TCHAR pzBuff[MAX_PATH] = { 0 };
		_stprintf(pzBuff, _T("Load LogFile Failed (%u)\n%s!"), eCode, eDescMsg);
		::MessageBox(NULL, pzBuff, _T("温馨提示"), MB_OK);
	}
}

// 清理日志组件
void UnintLogger()
{
	g_Logger->TryDebug(_T("UnrealMode End!"));
	::Sleep(10);
	g_Logger->UnInit();
}

namespace UnrealNSP {
	
	tUnReal				g_tUnreal;
	NumberConvert		g_NumberConvert;
	IUnrealControl*		g_pUnrealDelegate	= nullptr;
	bool				g_bInitCompleted	= false;
}

// DllMain.cpp : 定义 DLL 应用程序的入口点。
BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
