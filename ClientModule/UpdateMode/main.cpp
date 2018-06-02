// 1979_Game.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "frame/mainFrame.h"

GameResource*	R = nullptr;
CStaticLogger	g_Logger;
ResInfo			g_tResInfo;

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

		g_Logger->TryInfo(_T("UpdateMode Start!"));
		g_Logger->TryInfo(_T("UpdateMode %s"), valStr);
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
	g_Logger->TryInfo(_T("UpdateMode End!"));
	::Sleep(10);
	g_Logger->UnInit();
}

// �����������
bool ParseCommandLine()
{
	g_tResInfo.uVersionHigh		= 0;
	g_tResInfo.uVersionLow		= 0;
	g_tResInfo.hResourceInst	= NULL;
	g_tResInfo.iResourceID		= _T("");
	g_tResInfo.iResourceType	= UILIB_RESOURCETYPE::UILIB_FILE;
	g_tResInfo.iSkinResID		= 0;

	// ��ȡ������������
	CDuiString pzCmdLine = ::GetCommandLine();
	if (-1 != pzCmdLine.Find(_T("/Transmittal_Begin")))
	{
		int ipos_1 = pzCmdLine.Find(_T("["));
		int ipos_2 = pzCmdLine.Find(_T("]"));
		int iLen = ipos_2 - ipos_1 - 1;
		pzCmdLine = pzCmdLine.Mid(ipos_1 + 1, iLen);
		
		ipos_1 = 0;
		ipos_2 = 0;
		CDuiString pzTmpStr;

		// �����汾�Ÿ�λ
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				g_tResInfo.uVersionHigh = _ttoi(pzTmpStr.GetData());
			}
		}

		// �����汾�ŵ�λ
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				g_tResInfo.uVersionLow = _ttoi(pzTmpStr.GetData());
			}
		}

		// ��Դ���
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				g_tResInfo.hResourceInst = (HINSTANCE)(_ttoi(pzTmpStr.GetData()));
			}
		}
		
		// ��ԴID
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				g_tResInfo.iResourceID = MAKEINTRESOURCE(_ttoi(pzTmpStr.GetData()));
			}
		}

		// ��Դ����
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				g_tResInfo.iResourceType = (UILIB_RESOURCETYPE)(_ttoi(pzTmpStr.GetData()));
			}
		}

		// Ƥ������
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				g_tResInfo.iSkinResID = _ttoi(pzTmpStr.GetData());
			}
		}

		// ��Դ·��
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				::StrToCharArray(g_tResInfo.pzResPath, pzTmpStr.GetData());
			}
		}

		// ��ԴDll·��
		if (ipos_1 >= 0 && pzCmdLine.GetLength() > ipos_1)
		{
			ipos_1 = pzCmdLine.Find(_T("="), ipos_1);
			if (-1 != ipos_1)
			{
				ipos_1 += 1;
				ipos_2 = pzCmdLine.Find(_T(","), ipos_1);
				pzTmpStr = pzCmdLine.Mid(ipos_1, ipos_2 - ipos_1);
				::StrToCharArray(g_tResInfo.pzResDllPath, pzTmpStr.GetData());
			}
		}

		return true;
	}
	else
	{
		g_Logger->TryInfo(_T("GetCommandLine Parama Error [eMsg = %s]"), pzCmdLine.GetData());
	}

	return false;
}

int FreeMain(int iRes)
{
	g_Logger->TryInfo(_T("EndCode [%d]"), iRes);

	// �ͷ�ȫ�ֵ���ָ��
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
	// ��ʼ���������ģ�ͣ�COM����
	HRESULT OleInitHr = ::OleInitialize(NULL);
	HRESULT CoInitHr = ::CoInitialize(NULL);
	if (FAILED(OleInitHr) || FAILED(CoInitHr)) return FreeMain(-1);
	
	// ��ʼ����־���
	InitLogger();

	// ��ʼ���������
	CLibcurlDownloader::InitGlobal();
	
	// �����������
	if (!ParseCommandLine()) return FreeMain(-2);
	g_Logger->TryInfo(_T("ParseCmdLine OK!"));

	// ��ʼ��Duilib ������Դ·��
	// ������dll��̬��,����Դ�ȶ��Ǿ�̬��������Դ���ǹ���ģ��˴������ڴ�����
	CPaintManagerUI::SetInstance(hInstance);
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	if (g_tResInfo.iResourceType == UILIB_RESOURCETYPE::UILIB_FILE)
	{
		CPaintManagerUI::SetResourcePath(g_tResInfo.pzResPath);
	}
	else if (g_tResInfo.iResourceType == UILIB_RESOURCETYPE::UILIB_ZIPRESOURCE)
	{
		g_tResInfo.hResourceInst = ::LoadLibrary(g_tResInfo.pzResDllPath);
		if (NULL == g_tResInfo.hResourceInst)
		{
			::FreeLibrary(g_tResInfo.hResourceInst);
			return FreeMain(-3);
		}

		CPaintManagerUI::SetResourcePath(g_tResInfo.pzResPath);
		CPaintManagerUI::SetResourceDll(g_tResInfo.hResourceInst);
	}
	
	// ���������ࡢ��Ϸ��Դ�� ʵ��
	R = GameResource::getInstance();

	// ������UI
	MainFrame* pFrame = MainFrame::getInstance();
	if (pFrame)
	{
		// ��������
		CDuiString pzURLCombine;
		pzURLCombine.Format(_T("GamePlaza_V_%u_%u"), g_tResInfo.uVersionHigh, g_tResInfo.uVersionLow);
		pFrame->CreateFrame(pzURLCombine);

		// ��ʼ��Ϣѭ��
		CPaintManagerUI::MessageLoop();
	}

	// �ͷ���Դ
	return FreeMain(0);
}
