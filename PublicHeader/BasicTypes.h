/**********************************************************************
* Copyright (C) 2016 -一九七九网络 - All Rights Reserved
*
* 文件名称:		BasicTypes.h
* 摘    要:		基本类型定义
*     
* 作    者:		zcc
* 修    改:		 
* 
***********************************************************************/

#ifndef __BasicTypes_H__
#define __BasicTypes_H__


#include "StdHeaders.h"

#include <stddef.h>  // for NULL, size_t

#if !(defined(_MSC_VER) && (_MSC_VER < 1600))
#include <stdint.h>  // for uintptr_t
#endif


#if defined(_MSC_VER) && _MSC_VER <= 1200 // 1200 == VC++ 6.0
	#pragma warning(disable:4786)
#endif

#include <sys/types.h>

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;

#if defined(_MSC_VER)
	//
	// Windows/Visual C++
	//
	typedef signed __int8           int8;
	typedef unsigned __int8         uint8;
	typedef signed __int16          int16;
	typedef unsigned __int16        uint16;
	typedef signed __int32          int32;
	typedef unsigned __int32        uint32;
	typedef signed __int64          int64;
	typedef unsigned __int64        uint64;

	typedef int8					int8_t;
	typedef uint8					uint8_t;
	typedef int16					int16_t;
	typedef uint16					uint16_t;
	typedef int32					int32_t;
	typedef uint32					uint32_t;
	typedef int64					int64_t;
	typedef uint64					uint64_t;

#if defined(_WIN64)
#define PTR_IS_64_BIT 1
	typedef signed __int64			IntPtr;
	typedef unsigned __int64		UIntPtr;
#else
	typedef signed long				IntPtr;
	typedef unsigned long			UIntPtr;
#endif

#define HAVE_INT64 1

#endif


/////////////////////////////////////////////////////////////////////////////
// Workaround declaration problem with defaults
/////////////////////////////////////////////////////////////////////////////
//#ifdef _STLP_HASH_MAP
//	#define HashMap ::std::hash_map
//#else
//	#define HashMap ::stdext::hash_map
//#endif

#ifdef _UNICODE
	typedef std::wstring String;
//#	define String std::wstring
#else
	typedef std::string String;
//#	define String std::string
#endif


#endif
