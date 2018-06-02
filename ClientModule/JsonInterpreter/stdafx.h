// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include "resource.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
#define _CRT_SECURE_NO_DEPRECATE		//  标记来忽略这些警告

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件
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
*	Json库
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#ifdef DEBUG || _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

//#ifdef DEBUG || _DEBUG
	#include <shlwapi.h>					// Windos字符串处理库
	#pragma comment(lib, "shlwapi.lib")
//#endif

// 其他头文件
#include <iostream>							// 标准输入输出流
#include <commdlg.h>						// Windows 公用对话框
#include "..\\share\\define.h"				// 公用宏
#include "..\\share\\jsonFile.h"			// Json加解密
#include "..\\share\\ui\\UIDialogFrame.h"	// 对话框