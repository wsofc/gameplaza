/**********************************************************************
* * Copyright (C) 2016 - 一九七九 - All Rights Reserved
*
* 文件名称:		Platform.h
* 摘    要:		定义了一些与平台有关的宏
*     
* 作    者:		zcc, 
* 修    改:		
* 
***********************************************************************/

#ifndef __Platform_H__
#define __Platform_H__


// 操作系统类型
#define __PLATFORM_WIN32		1
#define __PLATFORM_LINUX		2
#define __PLATFORM_APPLE		3
#define __PLATFORM_APPLE_IOS	4
#define __PLATFORM_ANDROID		5
#define __PLATFORM_NACL			6

// 编译器类型
#define __COMPILER_MSVC			1
#define __COMPILER_GNUC			2
#define __COMPILER_BORL			3
#define __COMPILER_WINSCW		4
#define __COMPILER_GCCE			5
#define __COMPILER_CLANG		6

// 大小边类型
#define __ENDIAN_LITTLE			1
#define __ENDIAN_BIG			2

// 
#define __ARCHITECTURE_32		1
#define __ARCHITECTURE_64		2


// 获取当前编译器的类型以及版本
#if defined( _MSC_VER )
#   define __COMPILER __COMPILER_MSVC
#   define __COMP_VER _MSC_VER

#elif defined( __GCCE__ )
#   define __COMPILER __COMPILER_GCCE
#   define __COMP_VER _MSC_VER

#elif defined( __WINSCW__ )
#   define __COMPILER __COMPILER_WINSCW
#   define __COMP_VER _MSC_VER

#elif defined( __clang__ )
#   define __COMPILER __COMPILER_CLANG
#   define __COMP_VER (((__clang_major__)*100) + \
        (__clang_minor__*10) + \
        __clang_patchlevel__)

#elif defined( __GNUC__ )
#   define __COMPILER __COMPILER_MSVC
#   define __COMP_VER (((__GNUC__)*100) + \
	(__GNUC_MINOR__*10) + \
	__GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#   define __COMPILER __COMPILER_BORL
#   define __COMP_VER __BCPLUSPLUS__

#else
#   pragma error "No known compiler. Abort! Abort!"

#endif


#if __COMPILER == __COMPILER_MSVC
#   if __COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif


// 获取当前操作系统
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define __PLATFORM __PLATFORM_WIN32
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define __PLATFORM __PLATFORM_APPLE_IOS
#   else
#       define __PLATFORM __PLATFORM_APPLE
#   endif
#elif defined(__ANDROID__)
#	define __PLATFORM __PLATFORM_ANDROID
#elif defined( __native_client__ )
#   define __PLATFORM __PLATFORM_NACL
#else
#   define __PLATFORM __PLATFORM_LINUX
#endif


// Detect compiler is for x86 or x64.
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#	define CPU_X86 1
#endif

// 
#if defined(__x86_64__) || defined(_M_X64)
#   define __ARCH_TYPE __ARCHITECTURE_64
#else
#   define __ARCH_TYPE __ARCHITECTURE_32
#endif


#endif
