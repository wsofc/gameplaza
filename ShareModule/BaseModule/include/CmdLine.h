#pragma once


#include "BaseModule.h"


class BASE_MODULE_CLASS CCmdLine
{
private:
	enum 
	{
		MAX_PARAM_COUNT = 25
	};

public:
	CCmdLine();
	CCmdLine(LPCTSTR szCmdLine);
	~CCmdLine();

	void	Set(LPCTSTR szCmdLine);
	int		GetParamCount(void) const;
	LPCTSTR GetParam(int nIndex);
	LPCTSTR GetMainDir(void);
	LPCTSTR GetAppName(void);

private:
	void	ProcessCmdLine();
	void	MainDir(void);
	void	AppName(void);

private:
	LPTSTR	m_szStrBuf;
	int		m_nBufSize;
	LPTSTR	m_szParams[MAX_PARAM_COUNT];
	int		m_nParamCount;

	TCHAR	m_szMainDir[256];
	TCHAR	m_szAppName[256];
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
