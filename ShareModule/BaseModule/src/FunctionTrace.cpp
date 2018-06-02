//-----------------------------------------------
//File		: FunctionTrace.cpp
//Comment	: 函数打印
//History	: 	
//-----------------------------------------------

#include "StdAfx.h"
#include "FunctionTrace.h"

int CFunctionTrace::s_nIndex = 0;

CFunctionTrace::CFunctionTrace(CFileLog *pFileLog, 
							   LPCTSTR lpFunName,
							    bool bPrintTimeDiff)
{
	ASSERT(pFileLog);
	ASSERT(lpFunName);

	m_nMyIndex = s_nIndex++;

	lstrcpyn(m_szFunName, lpFunName,sizeof(m_szFunName));
	m_pFileLog = pFileLog;

	CString strTmp;
	strTmp.Format(_T("Fun:%s 进入,辅助索引 %d,"), m_szFunName, m_nMyIndex);
	if(m_pFileLog)
	{
		m_pFileLog->_Write(_T(""));
		m_pFileLog->_Write(strTmp);
	}

	//打印函数时间差
	m_bPrintTimeDiff = bPrintTimeDiff;
	m_dwStartTickCnt = 0;
	if(bPrintTimeDiff)
	{
		m_dwStartTickCnt = ::GetTickCount();
	}
}

CFunctionTrace::~CFunctionTrace(void)
{
	if(lstrlen(m_szFunName) > 0)
	{
		CString strTmp;
		strTmp.Format(_T("Fun:%s 退出,辅助索引 %d,"), m_szFunName,m_nMyIndex);

		//打印函数时间差
		if(m_bPrintTimeDiff)
		{
			//毫秒
			const DWORD dwTimeDiff = ::GetTickCount()-m_dwStartTickCnt;
			strTmp.Format(_T("Fun:%s 退出,辅助索引 %d,函数时间 %.3f 秒"), 
							m_szFunName,
							m_nMyIndex,
							dwTimeDiff/1000.0);
		}

		if(m_pFileLog)
		{
			m_pFileLog->_Write(strTmp);
		}
	}
}



//-----------------------------------------------
//					the end
//-----------------------------------------------
