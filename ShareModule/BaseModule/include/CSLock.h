
#pragma once


#include "BaseModule.h"


//CS锁
//
class BASE_MODULE_CLASS CCSLock
{
private:
	//模式
	//
	enum emMode
	{
		M_ERROR		= 0,		//外来模式
		M_CREATE	= 10		//创建模式
	};

public:
	CCSLock(void);
	CCSLock(CRITICAL_SECTION *pCS);
	~CCSLock(void);

	CRITICAL_SECTION * GetCS(void) const;
	bool	IsCreateMode(void);
	bool	Lock(void);
	bool	Unlock(void);

private:
	CRITICAL_SECTION	*m_pCS;
	emMode				m_Mode;
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
