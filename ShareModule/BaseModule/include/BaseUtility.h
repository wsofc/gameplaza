#pragma once


#include "BaseModule.h"
#include "Public.h"


class BASE_MODULE_CLASS BaseUtility
{
public:
	static LONG WINAPI UnhandledFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);

	//设置焦点
	static bool SetFocus(CDialog *pDlg, UINT uCtrlID);

	//读取数值
	static LONG ReadStringValue(LPCTSTR & pszSring);

	//读取矩形
	static bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//读取坐标
	static bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//读取颜色
	static bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//字符串加逗号
	static bool	NumberString2ViewString(const CString & strNumber, CString & strView);

	//字符串去掉逗号
	static bool	ViewString2NumberString(const CString & strViewNumber, CString & strNumber);

	static void FormatSetNumber(const tagSetNumber & SetNumber, OUT CString & strOut);

	/// 文件是否存在
	static bool file_exists(const LPCTSTR name);

	static BOOL CreateDeepDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	static LPCTSTR GetCurrentWorkingDirectory(void);
};
