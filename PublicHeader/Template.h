#ifndef TEMPLATE_HEAD_FILE
#define TEMPLATE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD						//无效椅子

//////////////////////////////////////////////////////////////////////////

//数组维数
#define	CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
#define	ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define CountString(String) ((UINT)((lstrlen(String)+1)*sizeof(TCHAR)))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//////////////////////////////////////////////////////////////////////////

//存储长度
#ifdef _UNICODE
#	define CountStringBuffer CountStringBufferW
#else
#	define CountStringBuffer CountStringBufferA
#endif

//存储长度
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

//////////////////////////////////////////////////////////////////////////

//模板文件
#include "Array.h"
#include "Module.h"
#include "Factory.h"

//////////////////////////////////////////////////////////////////////////

#endif