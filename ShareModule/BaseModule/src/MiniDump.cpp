#include "StdAfx.h"
#include "MiniDump.h"
#include "BaseUtility.h"

TCHAR		CMiniDump::s_szAppName[128];


CMiniDump::CMiniDump(LPCTSTR szAppName)
{
	if(szAppName && lstrlen(szAppName) >0)
	{
		lstrcpyn(s_szAppName, szAppName, sizeof(s_szAppName));
	}

	m_Filter = ::SetUnhandledExceptionFilter(BaseUtility::UnhandledFilter);
}

CMiniDump::~CMiniDump(void)
{
	::SetUnhandledExceptionFilter(m_Filter);
}



//-----------------------------------------------
//					the end
//-----------------------------------------------
