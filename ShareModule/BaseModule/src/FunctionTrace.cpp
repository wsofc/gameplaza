//-----------------------------------------------
//File		: FunctionTrace.cpp
//Comment	: ������ӡ
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
	strTmp.Format(_T("Fun:%s ����,�������� %d,"), m_szFunName, m_nMyIndex);
	if(m_pFileLog)
	{
		m_pFileLog->_Write(_T(""));
		m_pFileLog->_Write(strTmp);
	}

	//��ӡ����ʱ���
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
		strTmp.Format(_T("Fun:%s �˳�,�������� %d,"), m_szFunName,m_nMyIndex);

		//��ӡ����ʱ���
		if(m_bPrintTimeDiff)
		{
			//����
			const DWORD dwTimeDiff = ::GetTickCount()-m_dwStartTickCnt;
			strTmp.Format(_T("Fun:%s �˳�,�������� %d,����ʱ�� %.3f ��"), 
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
