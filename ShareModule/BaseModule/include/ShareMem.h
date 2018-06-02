#pragma once


#include "BaseModule.h"


class BASE_MODULE_CLASS CShareMem
{
public:
	CShareMem(LPCTSTR lpName, int nSize);
	~CShareMem(void);
						
public:
	char	*m_pData;

private:
      HANDLE   m_Handle;
};







//-----------------------------------------------
//					the end
//-----------------------------------------------
