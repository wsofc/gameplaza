#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN									// 从 Windows 头中排除极少使用的资料
#endif

#ifndef WINVER											// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
//#define WINVER 0x0400									// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#define WINVER NTDDI_WINXP
#endif

#ifndef _WIN32_WINNT									// 允许使用特定于 Windows NT 4 或更高版本的功能。
//#define _WIN32_WINNT 0x0400							// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#define  _WIN32_WINNT_WINXP
#endif						

#ifndef _WIN32_WINDOWS									// 允许使用特定于 Windows 98 或更高版本的功能。
//#define _WIN32_WINDOWS 0x0410							// 将此更改为针对于 Windows Me 或更高版本的合适的值。
#define _WIN32_WINDOWS NTDDI_WINXP
#endif

#ifndef _WIN32_IE										// 允许使用特定于 IE 4.0 或更高版本的功能。
//#define _WIN32_IE 0x0400								// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#define _WIN32_IE NTDDI_WINXP
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS				// 某些 CString 构造函数将为显式的

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN								//  从 Windows 头文件中排除极少使用的信息

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <regex>										// 正则表达式
#include <functional>									// 函数对象库函数

/*
*	加载vc读取.rc版本信息库
*/
#pragma comment(lib, "version.lib")

/*
*	Duilib库(输出日志)
*/
#include "..\\DuiLib\\UIlib.h"
using namespace DuiLib;
#ifdef DEBUG || _DEBUG
#   ifdef UNICODE || _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\DuiLib_d.lib")
#   endif
#else
#   ifdef UNICODE || _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\DuiLib.lib")
#   endif
#endif

/*
*	VC-Logge库(输出日志)
*/
#include "..\\..\\ThirdParty\\vc-logge\\StaticLogger.h"
#include "..\\..\\ThirdParty\\vc-logge\\DynamicLogger.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\Logger_CD.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\Logger_C.lib")
#endif

/*
*	Json库
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))				// 无效数值
#define INVALID_WORD				((WORD)(0xFFFF))			// 无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))		// 无效数值

//无效数值
#define INVALID_TABLE				INVALID_WORD				// 无效桌子
#define INVALID_CHAIR				INVALID_WORD				// 无效椅子

#include "..\\..\\PublicHeader\\GlobalDef\\GlobalFrame.h"		// 全局定义等
#include "..\\..\\PublicHeader\\Network\\CMD_LogonServer.h"		// 登录服数据结构定义等
#include "..\\..\\PublicHeader\\Network\\CMD_GameServer.h"		// 游戏服数据结构定义等

#include "..\\share\\define.h"									// 宏定义
#include "..\\share\\stringConvertor.h"							// 字符串转换
#include "..\\share\\numberConvert.h"							// 数字转汉字
#include "..\\share\\jsonFile.h"								// Json文件读写
#include "..\\share\\ui\\UIDialogFrame.h"						// 对话框
#include "..\\share\\ui\\UIControlGdiplus.h"					// Container GDI+绘图
#include "..\\share\\control\\IUnrealControl.h"					// 控制接口定义

#include "..\\GamePlaza\\data\\dataManager.h"
#include "..\\GamePlaza\\util\\gameResource.h"

#include "Unreal.h"