#pragma once


#include "BaseModule.h"


class BASE_MODULE_CLASS CFileSizeFormat
{
private:
	CFileSizeFormat(void);
	~CFileSizeFormat(void);

	static CString s_InsertSeparator (DWORD dwNumber);

public:
	static CString s_Format(DWORD dwFileSize);
};

