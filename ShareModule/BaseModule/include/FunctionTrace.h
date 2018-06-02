//-----------------------------------------------
//File		: FunctionTrace.h
//Comment	: 函数打印
//History	: 	
//-----------------------------------------------

#pragma once

#include "FileLog.h"

class CFunctionTrace
{
public:
	CFunctionTrace(CFileLog *pFileLog, LPCTSTR lpFunName, bool bPrintTimeDiff=true);
	~CFunctionTrace(void);

private:
	TCHAR		m_szFunName[256];
	CFileLog	*m_pFileLog;
	
	bool		m_bPrintTimeDiff;	//是否打印函数时间差
	DWORD		m_dwStartTickCnt;
	int			m_nMyIndex;

	static	int s_nIndex;
};



//-----------------------------------------------
//					the end
//-----------------------------------------------
