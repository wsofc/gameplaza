// GamePlaza.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "hall/loginFrame.h"

GameResource*		R				= NULL;								// ��Դ��ָ��
GameUtil*			g_GameUtil		= NULL;								// ������ָ��
HANDLE				g_hMutex		= INVALID_HANDLE_VALUE;				// ���̵�����
CStaticLogger		g_Logger;											// ��̬����־����

// ��ʼ����־���
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
		::MessageBox(NULL, pzBuff, _T("��ܰ��ʾ"), MB_OK);
	}
}

// ������־���
void UnintLogger()
{
	g_Logger->TryInfo(_T("GamePlaza End!"));
	::Sleep(10);
	g_Logger->UnInit();
}

// �Ƿ�����ͻ��˶࿪
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
	// �ͷ����Dll
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	pUnrealControl->UnLoadUnrealDll();

	// �ͷ�ȫ�ֵ���ָ��
	SAFE_DELETE(g_GameUtil);
	SAFE_DELETE(R);

	// �����������
	CLibcurlDownloader::CleanGlobal();

	// ������־���
	UnintLogger();

	// �رյ�ǰ�̵߳�COM��,ж���̼߳��ص�����dll,�ͷ��κ���������Դ,�ر����߳���ά�����е�RPC����
	::CoUninitialize();
	::OleUninitialize();

	return iRes;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	HRESULT OleInitHr = ::OleInitialize(NULL);	// ��ʼ���������ģ�ͣ�COM����
	HRESULT CoInitHr = ::CoInitialize(NULL);	// �������� Windows�Ե��̵߳ķ�ʽ����com����
	if (FAILED(OleInitHr) || FAILED(CoInitHr)) return FreeMain(-1);

	// ��ʼ����־���
	InitLogger();

	// ��ʼ���������
	CLibcurlDownloader::InitGlobal();
	
	// ���������ࡢ��Ϸ��Դ��ʵ��
	g_GameUtil = GameUtil::GetInstance();
	R = GameResource::GetInstance();
	if (g_GameUtil == NULL || R == NULL) return FreeMain(-2);

	// ��ʼ��Duilib ������Դ·��
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
		::MessageBox(NULL, _T("������Դʧ��!"), _T("��ܰ��ʾ"), MB_OK);
		return FreeMain(-3);
	}
#endif // SKIN_RES_TEST

#ifdef DEBUG || _DEBUG
	// DEBUGģʽ�£��������ƣ��������
#else
	// ����һ̨����ֻ������һ���ͻ���
	if (!CheckMoreopen())
	{
		g_hMutex = ::CreateMutex(NULL, true, R->MainWndTitleName);				// _T()������ǳ���ı���
		if (GetLastError() == ERROR_ALREADY_EXISTS)								// �����Ѿ�����
		{
			HWND oldHWnd = NULL;
			oldHWnd = ::FindWindow(NULL, R->MainWndTitleName);					// �����Ѿ����еĳ���
			if (oldHWnd)
			{
				::ShowWindow(oldHWnd, SW_SHOWNORMAL);							// ������ʾ�ҵ��������еĳ���
				::SetForegroundWindow(oldHWnd);									// �������еĳ�������Ϊ��ǰ����
			}
			// ������л������������ͷž������λ��������
			::ReleaseMutex(g_hMutex);
			g_hMutex = NULL;
			return FreeMain(0);
		}
		::ReleaseMutex(g_hMutex);
	}
#endif // DEBUG || _DEBUG

	// �������дʻ��
	g_GameUtil->LoadSensitiveVocabulary();

	// ��¼UI
	LoginFrame* pFrame = LoginFrame::GetInstance();
	if (pFrame)
	{
		// ��������
		pFrame->CreateFrame();

		// ��ʼ��Ϣѭ��
		CPaintManagerUI::MessageLoop();
	}

	// �ͷ���Դ
	return FreeMain(0);
}
