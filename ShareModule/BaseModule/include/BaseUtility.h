#pragma once


#include "BaseModule.h"
#include "Public.h"


class BASE_MODULE_CLASS BaseUtility
{
public:
	static LONG WINAPI UnhandledFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);

	//���ý���
	static bool SetFocus(CDialog *pDlg, UINT uCtrlID);

	//��ȡ��ֵ
	static LONG ReadStringValue(LPCTSTR & pszSring);

	//��ȡ����
	static bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//��ȡ����
	static bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//��ȡ��ɫ
	static bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);

	//�ַ����Ӷ���
	static bool	NumberString2ViewString(const CString & strNumber, CString & strView);

	//�ַ���ȥ������
	static bool	ViewString2NumberString(const CString & strViewNumber, CString & strNumber);

	static void FormatSetNumber(const tagSetNumber & SetNumber, OUT CString & strOut);

	/// �ļ��Ƿ����
	static bool file_exists(const LPCTSTR name);

	static BOOL CreateDeepDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	static LPCTSTR GetCurrentWorkingDirectory(void);
};
