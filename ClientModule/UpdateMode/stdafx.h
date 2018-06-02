// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER						// Allow use of features specific to Windows XP or later.
#define WINVER NTDDI_WINXP			// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT				// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT NTDDI_WINXP	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS				// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS NTDDI_WINXP	// Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE					// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE NTDDI_WINXP		// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include "targetver.h"

// Windows 头文件: 
//#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//////////////////////////////////// MFC模块 //////////////////////////////////
//#include <afxwin.h>         // MFC 核心组件和标准组件
//#include <afxext.h>         // MFC 扩展
//#include <atlimage.h>
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxole.h>         // MFC OLE 类
//#include <afxodlgs.h>       // MFC OLE 对话框类
//#include <afxdisp.h>        // MFC 自动化类
//#endif // _AFX_NO_OLE_SUPPORT
//
//#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>			// MFC ODBC 数据库类
//#endif // _AFX_NO_DB_SUPPORT
//
//#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO 数据库类
//#endif // _AFX_NO_DAO_SUPPORT
//
//#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
//#endif // _AFX_NO_AFXCMN_SUPPORT
//////////////////////////////////// MFC模块 //////////////////////////////////

// TODO:  在此处引用程序需要的其他头文件
#include<atlstr.h>
#include <fstream>
#include <regex>
#include "resource.h"
#include <functional>

////////////////////////////////////////////////////////////////////////////
//Duilib

#include "..\\DuiLib\\UIlib.h"
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\DuiLib.lib")
#   endif
#endif

/*
*	添加wke浏览器控件(Chrome内核)
*	暂时处于试用期，没有把wke嵌入Duilib库中,直接在项目中使用
*/
#include "..\\..\\ThirdParty\\wke\\wke.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\wke.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\wke.lib")
#endif

/*
*	Json库
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

/*
*	Zlib库
*/
#include "..\\..\\ThirdParty\\zlib\\zip.h"
#include "..\\..\\ThirdParty\\zlib\\unzip.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\zlibwapi_d.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\zlibwapi.lib")
#endif

/*
*	Libcurl库基于URL下载
*/
#include "..\\..\\ThirdParty\\libcurl\\include\\curl\\curl.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\libcurld.lib")
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\libcurld_imp.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\libcurl.lib")
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\libcurl_imp.lib")
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

// 其他头文件
#include <shellapi.h>
#include "..\\share\\define.h"
#include "..\\share\\wndTimmer.h"
#include "..\\share\\jsonFile.h"
#include "..\\share\\stringConvertor.h"
#include "..\\share\\libcurlDownloader.h"
#include "..\\share\\zlibUtil.h"

#include "..\\share\\ui\\UIWkeWebkit.h"
#include "..\\share\\ui\\UIDialogFrame.h"
#include "..\\share\\control\\IUserControl.h"

// 其他宏定义
/** safe delete macro
*/
#ifndef SAFE_DELETE
#	define SAFE_DELETE(p) {if ( (p) ) { delete (p); (p) = 0;} }
#endif // SAFE_DELETE

/** safe release macro
*/
#ifndef SAFE_RELEASE
#	define SAFE_RELEASE(p) {if ( (p) ) { (p)->Release(); (p) = 0;} }
#endif // SAFE_RELEASE

/** safe delete array macro
*/
#ifndef SAFE_DELETE_ARRAY
#	define SAFE_DELETE_ARRAY(p) { if (p) { delete [] (p);  (p) = 0; } }
#endif //SAFE_DELETE_ARRAY

#endif //__STDAFX_H__