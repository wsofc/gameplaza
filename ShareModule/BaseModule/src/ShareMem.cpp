#include "StdAfx.h"
#include "ShareMem.h"


CShareMem::CShareMem(LPCTSTR lpName, int nSize)
{
	m_Handle = ::CreateFileMapping((HANDLE)0xFFFFFFFF,
									NULL,   
									PAGE_READWRITE,   
									0,   
									nSize,   
									lpName);

	const bool bCreated = (GetLastError()==0);
	if(m_Handle != 0)
	{
		m_pData	= (char*)MapViewOfFile(m_Handle,
						FILE_MAP_WRITE,   
						0,   
						0,   
						nSize);
	}

	if(m_pData!=NULL && bCreated)
	{
		ZeroMemory(m_pData, nSize);
	}
}

CShareMem::~CShareMem()
{
	if(m_pData != NULL)
	{
		::UnmapViewOfFile(m_pData);
	}
	if(m_Handle != NULL)
	{
		::CloseHandle(m_Handle);
	}
} 




//-----------------------------------------------
//					the end
//-----------------------------------------------
