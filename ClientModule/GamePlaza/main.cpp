// GamePlaza.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "hall/loginFrame.h"

GameResource*		R				= NULL;								// 资源类指针
GameUtil*			g_GameUtil		= NULL;								// 工具类指针
HANDLE				g_hMutex		= INVALID_HANDLE_VALUE;				// 进程单例锁
CStaticLogger		g_Logger;											// 静态库日志对象

// 初始化日志组件
void InitLogger()
{
	LPCTSTR lf = NULL;
	ILogger::LogLevel ll = ILogger::DEFAULT_LOG_LEVEL;
	int pf = ILogger::PRINT_FLAG_FILE | ILogger::PRINT_FLAG_SCREEN;

	if (g_Logger->Init(lf, ll, pf))
	{
		//struct VS_VERSIONINFO { 

		//  WORD  wLength;			//2

		//  WORD  wValueLength;		//2 

		//  WORD  wType;			//2

		//  WCHAR szKey[];			// Contains the Unicode string "VS_VERSION_INFO". 32 bytes

		//  WORD  Padding1[];		//2 

		//  VS_FIXEDFILEINFO Value; // offset :2+2+2+32+2=40

		//  WORD  Padding2[]; 

		//  WORD  Children[]; 

		//};

		//This structure is not a true C - language structure because it contains variable - length members.
		//This structure was created solely to depict the organization of data in a version resource and does not appear in
		//any of the header files shipped with the Microsoft Windows Software Development Kit(SDK).

		HRSRC hsrc = FindResource(0, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		HGLOBAL hgbl = LoadResource(0, hsrc);
		BYTE *pBt = (BYTE *)LockResource(hgbl);
		VS_FIXEDFILEINFO* pFinfo = (VS_FIXEDFILEINFO*)(pBt + 40);

		CString valStr;
		valStr.Format(_T("Version: %u.%u.%u.%u")
			, (pFinfo->dwFileVersionMS >> 16) & 0xFF
			, (pFinfo->dwFileVersionMS) & 0xFF
			, (pFinfo->dwFileVersionLS >> 16) & 0xFF
			, (pFinfo->dwFileVersionLS) & 0xFF);

		g_Logger->TryInfo(_T("GamePlaza Start!"));
		g_Logger->TryInfo(_T("GamePlaza %s"), valStr);
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
	g_Logger->TryInfo(_T("GamePlaza End!"));
	::Sleep(10);
	g_Logger->UnInit();
}

// 是否允许客户端多开
bool CheckMoreopen()
{
	bool bRes = false;
	Json::Value vRoot;
	
	CDuiString pzModulePath = CPaintManagerUI::GetInstancePath();
	pzModulePath += _T("MoreOpen");
	JsonFile* pJsonFile = new JsonFile(pzModulePath, true);
	bRes = pJsonFile->initRootNode(vRoot);
	SAFE_DELETE(pJsonFile);
	bRes = vRoot["MoreOpen"].isBool() && vRoot["MoreOpen"].asBool();

	return bRes;
}

int FreeMain(int iRes)
{
	// 释放相关Dll
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	pUnrealControl->UnLoadUnrealDll();

	// 释放全局单例指针
	SAFE_DELETE(g_GameUtil);
	SAFE_DELETE(R);

	// 清理下载组件
	CLibcurlDownloader::CleanGlobal();

	// 清理日志组件
	UnintLogger();

	// 关闭当前线程的COM库,卸载线程加载的所有dll,释放任何其他的资源,关闭在线程上维护所有的RPC连接
	::CoUninitialize();
	::OleUninitialize();

	return iRes;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	HRESULT OleInitHr = ::OleInitialize(NULL);	// 初始化组件对象模型（COM）库
	HRESULT CoInitHr = ::CoInitialize(NULL);	// 用来告诉 Windows以单线程的方式创建com对象
	if (FAILED(OleInitHr) || FAILED(CoInitHr)) return FreeMain(-1);

	// 初始化日志组件
	InitLogger();

	// 初始化下载组件
	CLibcurlDownloader::InitGlobal();
	
	// 创建工具类、游戏资源类实例
	g_GameUtil = GameUtil::GetInstance();
	R = GameResource::GetInstance();
	if (g_GameUtil == NULL || R == NULL) return FreeMain(-2);

	// 初始化Duilib 设置资源路径
	bool bLoadResComplete = false;
	CDuiString pzFilePath = _T("");
	CDuiString pzResourcePath = _T("");
	CPaintManagerUI::SetInstance(hInstance);
	GameResource::g_SkinResID = g_GameUtil->ReadSkinConfig();
#ifdef SKIN_RES_TEST
	pzFilePath.Empty();
	pzResourcePath.Empty();
	pzFilePath.Format(_T("%sGamePlaza\\skin_res\\"), CPaintManagerUI::GetInstancePath().GetData());
	if (g_GameUtil->GetSkinResPath(pzFilePath, pzResourcePath))
	{
		bLoadResComplete = true;
		pzResourcePath.Format(_T("%s%s\\"), pzFilePath.GetData(), pzResourcePath.GetData());
		CPaintManagerUI::SetResourcePath(pzResourcePath);
		::StrToCharArray(GameResource::g_ResourcePath, CPaintManagerUI::GetResourcePath().GetData());
		::memset(GameResource::g_ResourceDllPath, 0, ArrayCount(GameResource::g_ResourceDllPath) * sizeof(TCHAR));
	}
#else
	pzFilePath.Empty();
	pzResourcePath.Empty();
	pzFilePath.Format(_T("%sGamePlaza\\skin\\"), CPaintManagerUI::GetInstancePath().GetData());
	if (g_GameUtil->GetSkinResPath(pzFilePath, pzResourcePath))
	{
		pzResourcePath.Format(_T("%s%s"), pzFilePath.GetData(), pzResourcePath.GetData());
		if (g_GameUtil->LoadSkinResFromDll(pzResourcePath))
		{
			bLoadResComplete = true;
			CPaintManagerUI::SetResourcePath(pzFilePath);
			CPaintManagerUI::SetResourceDll(GameResource::g_ResourceInst);
			::StrToCharArray(GameResource::g_ResourcePath, CPaintManagerUI::GetResourcePath().GetData());
			::StrToCharArray(GameResource::g_ResourceDllPath, pzResourcePath.GetData());
		}
	}
	
	if (!bLoadResComplete)
	{
		g_Logger->TryError(_T("Load Resource Failed![%s,%s]"), pzFilePath.GetData(), pzResourcePath.GetData());
		::MessageBox(NULL, _T("加载资源失败!"), _T("温馨提示"), MB_OK);
		return FreeMain(-3);
	}
#endif // SKIN_RES_TEST

#ifdef DEBUG || _DEBUG
	// DEBUG模式下，不做限制，方便调试
#else
	// 限制一台电脑只能启动一个客户端
	if (!CheckMoreopen())
	{
		g_hMutex = ::CreateMutex(NULL, true, R->MainWndTitleName);				// _T()里面的是程序的标题
		if (GetLastError() == ERROR_ALREADY_EXISTS)								// 程序已经运行
		{
			HWND oldHWnd = NULL;
			oldHWnd = ::FindWindow(NULL, R->MainWndTitleName);					// 查找已经运行的程序
			if (oldHWnd)
			{
				::ShowWindow(oldHWnd, SW_SHOWNORMAL);							// 激活显示找到的已运行的程序
				::SetForegroundWindow(oldHWnd);									// 将已运行的程序设置为当前窗口
			}
			// 如果已有互斥量存在则释放句柄并复位互斥量　
			::ReleaseMutex(g_hMutex);
			g_hMutex = NULL;
			return FreeMain(0);
		}
		::ReleaseMutex(g_hMutex);
	}
#endif // DEBUG || _DEBUG

	// 加载敏感词汇表
	g_GameUtil->LoadSensitiveVocabulary();

	// 登录UI
	LoginFrame* pFrame = LoginFrame::GetInstance();
	if (pFrame)
	{
		// 创建窗口
		pFrame->CreateFrame();

		// 开始消息循环
		CPaintManagerUI::MessageLoop();
	}

	// 释放资源
	return FreeMain(0);
}
