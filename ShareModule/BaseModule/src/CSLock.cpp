#include "StdAfx.h"
#include "CSLock.h"


//函数功能: 构造与析构
//函数参数:
//函数返回:
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

//函数功能: 是否是创建模式
//函数参数:
//函数返回:
bool	CCSLock::IsCreateMode(void)
{
	return (m_Mode==M_CREATE)?true:false;
}

//函数功能: 锁
//函数参数:
//函数返回:
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

//函数功能: 解锁
//函数参数:
//函数返回:
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
