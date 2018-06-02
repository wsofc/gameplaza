//-----------------------------------------------
//File		: FunctionTrace.h
//Comment	: ������ӡ
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
	
	bool		m_bPrintTimeDiff;	//�Ƿ��ӡ����ʱ���
	DWORD		m_dwStartTickCnt;
	int			m_nMyIndex;

	static	int s_nIndex;
};



//-----------------------------------------------
//					the end
//-----------------------------------------------
