
#pragma once

#include "BaseModule.h"

class BASE_MODULE_CLASS CMiniDump
{
private:

public:
	CMiniDump(LPCTSTR szAppName);
	~CMiniDump(void);

public:
	static LPCTSTR GetAppName() { return s_szAppName; }

public:
	static TCHAR				s_szAppName[128];

private:
	LPTOP_LEVEL_EXCEPTION_FILTER m_Filter;
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
