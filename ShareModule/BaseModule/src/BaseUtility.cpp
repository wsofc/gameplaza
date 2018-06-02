#pragma once

#include "stdafx.h"
#include "BaseUtility.h"
#include <windows.h>
#include <dbghelp.h>
#include <sys/stat.h>

#pragma comment(lib, "Dbghelp.lib")


LONG WINAPI BaseUtility::UnhandledFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	LONG  ret = EXCEPTION_CONTINUE_SEARCH;
	TCHAR szFileName[512] = { 0 };

	SYSTEMTIME st;
	::GetLocalTime(&st);
	_sntprintf(szFileName,
		sizeof(szFileName),
		TEXT("%%04d_%02d_%02d_%02d_%02d_%02d_%02d_%02d.dmp"),
		CMiniDump::GetAppName(),
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		::rand() % 100);

	HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = false;

		//write the dump
		BOOL bOK = ::MiniDumpWriteDump(::GetCurrentProcess(),
			::GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			&ExInfo,
			NULL,
			NULL);
		if (bOK)
		{
			//printf("Create Dump File Success!\n");
		}
		else
		{
			//printf("MiniDumpWriteDump Failed: %d\n", ::GetLastError());
		}

		ret = EXCEPTION_EXECUTE_HANDLER;
		::CloseHandle(hFile);
	}
	else
	{
		//printf("Create File %s Failed %d\n", szFileName, ::GetLastError());
	}

	return ret;
}

BOOL BaseUtility::CreateDeepDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	if (::PathFileExists(lpPathName))
	{
		return TRUE;
	}

	CString strPath = _T("");
	TCHAR pszSrc[MAX_PATH] = { 0 };
	PTCHAR ptoken = nullptr;

	_tcscpy(pszSrc, lpPathName);
	ptoken = _tcstok(pszSrc, _T("\\"));
	while (ptoken)
	{
		strPath += ptoken;
		strPath += _T("\\");
		if (!::PathFileExists(strPath))
		{
			if (!::CreateDirectory(strPath, lpSecurityAttributes))
			{
				//DWORD dw = ::GetLastError(); 
				//CString strErr;
				//strErr.Format("CreateDirectory Failed: %d", dw);
				//AfxMessageBox(strErr);
				return FALSE;
			}
		}
		ptoken = _tcstok(NULL, _T("\\"));
	}
	return TRUE;
}

LPCTSTR BaseUtility::GetCurrentWorkingDirectory(void)
{
	static TCHAR szDirWork[MAX_PATH] = { 0 };
	if (szDirWork[0] != '\0') return szDirWork;

	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	TCHAR szDir[_MAX_DIR];
	::GetModuleFileName(hInstance, szDirWork, CountArray(szDirWork));

	TCHAR szDrive[_MAX_DRIVE];
	_tsplitpath(szDirWork, szDrive, szDir, NULL, NULL);
	_tcscpy(szDirWork, szDrive);
	_tcscat(szDirWork, szDir);

	return szDirWork;
}

//设置焦点
bool BaseUtility::SetFocus(CDialog *pDlg, UINT uCtrlID)
{
	if ((!pDlg) || !pDlg->GetSafeHwnd())
	{
		ASSERT(false);
		return false;
	}
	if (uCtrlID == 0)
	{
		ASSERT(false);
		return false;
	}

	CWnd *pWnd = pDlg->GetDlgItem(uCtrlID);
	if (!pWnd)
	{
		ASSERT(false);
		return false;
	}

	pWnd->SetFocus();
	return true;
}

//读取数值
LONG BaseUtility::ReadStringValue(LPCTSTR & pszSring)
{
	//效验参数
	ASSERT((pszSring != NULL) && (pszSring[0] != 0));
	if ((pszSring == NULL) || (pszSring[0] == 0)) return 0L;

	//寻找开始
	while (((pszSring[0] > 0) && (pszSring[0] < TEXT('0'))) || (pszSring[0] > TEXT('9'))) pszSring++;

	//读取数值
	LONG lValue = 0L;
	while ((pszSring[0] >= TEXT('0')) && (pszSring[0] <= TEXT('9')))
	{
		lValue = lValue * 10L + pszSring[0] - TEXT('0');
		++pszSring;
	}

	return lValue;
}

//读取矩形
bool BaseUtility::ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64] = TEXT("");
	ZeroMemory(&ValueRect, sizeof(ValueRect));

	//读取字符
	GetPrivateProfileString(pszItem, pszSubItem, TEXT(""), szReadData, ArrayCount(szReadData), pszIniFile);
	if (szReadData[0] != 0)
	{
		//读取变量
		LPCTSTR pszString = szReadData;
		ValueRect.left = ReadStringValue(pszString);
		ValueRect.top = ReadStringValue(pszString);
		ValueRect.right = ReadStringValue(pszString);
		ValueRect.bottom = ReadStringValue(pszString);

		return true;
	}

	return false;
}

//读取坐标
bool BaseUtility::ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64] = TEXT("");
	ZeroMemory(&ValuePoint, sizeof(ValuePoint));

	//读取字符
	GetPrivateProfileString(pszItem, pszSubItem, TEXT(""), szReadData, CountArray(szReadData), pszIniFile);
	if (szReadData[0] != 0)
	{
		//读取变量
		LPCTSTR pszString = szReadData;
		ValuePoint.x = ReadStringValue(pszString);
		ValuePoint.y = ReadStringValue(pszString);

		return true;
	}

	return false;
}

//读取颜色
bool BaseUtility::ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64] = TEXT("");
	ZeroMemory(&ValueColor, sizeof(ValueColor));

	//读取字符
	GetPrivateProfileString(pszItem, pszSubItem, TEXT(""), szReadData, CountArray(szReadData), pszIniFile);
	if (szReadData[0] != 0)
	{
		//读取变量
		LPCTSTR pszString = szReadData;
		ValueColor = RGB(ReadStringValue(pszString), ReadStringValue(pszString), ReadStringValue(pszString));

		return true;
	}

	return false;
}

bool BaseUtility::NumberString2ViewString(const CString & strNumber, CString & strView)
{
	//分割位数
	//如果以万为单位分割,就是4
	const int nWeiShu = 3;

	if (strNumber.GetLength() <= 0)
	{
		return false;
	}
	if (strNumber.GetLength() <= nWeiShu)
	{
		strView = strNumber;
		return true;
	}

	//记录负号
	const char c = '-';
	CString strNumberTmp = strNumber;
	const bool bFindMinus = strNumberTmp.Find(c) != -1;
	strNumberTmp.Remove(c);
	const int nLen = strNumberTmp.GetLength();

	CString strTmp[10];
	int nStartIndex = 0;

	const int m = nLen / nWeiShu;
	const int n = nLen%nWeiShu;

	//分割号个数
	const int nCnt = m + (n > 0 ? 1 : 0);
	int nCharIndex = 0;
	for (int i = 0; i < nCnt; i++)
	{
		if (i == 0 && n > 0)
		{
			strTmp[i] = strNumberTmp.Mid(nStartIndex, n);
			nStartIndex += n;
			continue;
		}
		strTmp[i] = strNumberTmp.Mid(nStartIndex, nWeiShu);
		nStartIndex += nWeiShu;
	}

	const CString str = _T(",");
	CString strViewTmp;
	for (int i = 0; i < nCnt; i++)
	{
		if (i == (nCnt - 1))
		{
			strViewTmp += strTmp[i];
			TRACE(_T("%s \r\n"), strTmp[i]);
			continue;
		}
		strViewTmp += strTmp[i] + str;
		TRACE(_T("%s \r\n"), strTmp[i]);
	}

	if (bFindMinus)
	{
		strView.Format(_T("-%s"), strViewTmp);
	}
	else
	{
		strView.Format(_T("%s"), strViewTmp);
	}
	return true;
}

bool BaseUtility::ViewString2NumberString(const CString & strViewNumber, CString & strNumber)
{
	if (strViewNumber.GetLength() <= 0)
	{
		return false;
	}

	strNumber = strViewNumber;

	//去掉分割符
	strNumber.Remove(_T(','));
	return true;
}

void BaseUtility::FormatSetNumber(const tagSetNumber & SetNumber, OUT CString & strOut)
{
	//局号格式为2位ServerID+12位时间毫秒数+2位流水
	strOut.Format(TEXT("%02d%I64d%02d"),
		SetNumber.wServerID, SetNumber.lMilliseconds, SetNumber.wIndex);

	return;
}

bool BaseUtility::file_exists(const LPCTSTR name)
{
	bool bRes = false;
	struct stat buffer;

#ifdef _UNICODE
	char* cstr = nullptr;
	int len = ::WideCharToMultiByte(CP_ACP, 0, name, -1, NULL, 0, NULL, NULL);
	cstr = new char[len + 1];
	::WideCharToMultiByte(CP_ACP, 0, name, -1, cstr, len, NULL, NULL);

	bRes = stat(cstr, &buffer) == 0;
	SAFE_DELETE(cstr);
#else
	bRes = stat(name, &buffer) == 0;
#endif // _UNICODE

	return bRes;
}
