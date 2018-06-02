#include "StdAfx.h"
#include "CSLock.h"


//��������: ����������
//��������:
//��������:
CCSLock::CCSLock(void)
{
	m_pCS = new CRITICAL_SECTION;
	m_Mode = M_CREATE;		
	::InitializeCriticalSection(m_pCS); 
}

CCSLock::CCSLock(CRITICAL_SECTION *pCS)
{
	m_Mode = M_ERROR;
	m_pCS = pCS;
	this->Lock();
}

CCSLock::~CCSLock(void)
{
	this->Unlock();

	if(m_Mode == M_CREATE)
	{
		::DeleteCriticalSection(m_pCS);
		delete m_pCS;
		m_pCS = NULL;
	}
}

CRITICAL_SECTION * CCSLock::GetCS(void) const
{
	return m_pCS;
}

//��������: �Ƿ��Ǵ���ģʽ
//��������:
//��������:
bool	CCSLock::IsCreateMode(void)
{
	return (m_Mode==M_CREATE)?true:false;
}

//��������: ��
//��������:
//��������:
bool	CCSLock::Lock(void)
{
	if(m_Mode == M_CREATE)
	{
		if(m_pCS == NULL)
		{
			return false;	
		}
	}
	
	::EnterCriticalSection(m_pCS);
	return true;
}

//��������: ����
//��������:
//��������:
bool	CCSLock::Unlock(void)
{
	if(m_Mode == M_CREATE)
	{
		if(m_pCS == NULL)
		{
			return false;	
		}
	}

	::LeaveCriticalSection(m_pCS);
	return true;
}


//-----------------------------------------------
//					the end
//-----------------------------------------------
