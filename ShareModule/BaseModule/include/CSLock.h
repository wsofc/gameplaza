
#pragma once


#include "BaseModule.h"


//CS��
//
class BASE_MODULE_CLASS CCSLock
{
private:
	//ģʽ
	//
	enum emMode
	{
		M_ERROR		= 0,		//����ģʽ
		M_CREATE	= 10		//����ģʽ
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
