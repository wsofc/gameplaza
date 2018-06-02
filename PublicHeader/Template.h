#ifndef TEMPLATE_HEAD_FILE
#define TEMPLATE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//���ó���

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)(0xFF))						//��Ч��ֵ
#define INVALID_WORD				((WORD)(0xFFFF))					//��Ч��ֵ
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//��Ч��ֵ

//////////////////////////////////////////////////////////////////////////
//���ó���

//��Ч��ֵ
#define INVALID_TABLE				INVALID_WORD						//��Ч����
#define INVALID_CHAIR				INVALID_WORD						//��Ч����

//////////////////////////////////////////////////////////////////////////

//����ά��
#define	CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
#define	ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define CountString(String) ((UINT)((lstrlen(String)+1)*sizeof(TCHAR)))

//��Ч��ַ
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//////////////////////////////////////////////////////////////////////////

//�洢����
#ifdef _UNICODE
#	define CountStringBuffer CountStringBufferW
#else
#	define CountStringBuffer CountStringBufferA
#endif

//�洢����
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

//////////////////////////////////////////////////////////////////////////

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "Factory.h"

//////////////////////////////////////////////////////////////////////////

#endif