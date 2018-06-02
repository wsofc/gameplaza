#pragma once

//平台头文件
#include "../../../PublicHeader/Template.h"
#include "../../../PublicHeader/SafeDelete.h"

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifndef BASE_MODULE_CLASS
#	ifdef  BASE_MODULE_DLL
#		define BASE_MODULE_CLASS _declspec(dllexport)
#	else
#		define BASE_MODULE_CLASS _declspec(dllimport)
#	endif
#endif

//模块定义
#ifdef _DEBUG
#	define BASE_MODULE_DLL_NAME	TEXT("BaseModuleD.dll")			//组件 DLL 名字
#else
#	define BASE_MODULE_DLL_NAME	TEXT("BaseModule.dll")			//组件 DLL 名字
#endif

//
// 自动链接库.
//
#if defined(_MSC_VER)
#	if !defined(NO_AUTOMATIC_LIBS) && !defined(BASE_MODULE_DLL)
#		if defined(_DEBUG)
#			pragma comment(lib, "BaseModuleD.lib")
#		else
#			pragma comment(lib, "BaseModule.lib")
#		endif
#	endif
#endif


//////////////////////////////////////////////////////////////////////////


//导出类头文件
#include "CmdLine.h"
#include "CSLock.h"
#include "ShareMem.h"
#include "IPAddress.h"
#include "Encrypt.h"
#include "DataQueue.h"
#include "WinFileInfo.h"
#include "ServiceThread.h"
#include "IniFile.h"
#include "BaseUtility.h"
#include "MiniDump.h"

