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
#define _CRT_SECURE_NO_WARNINGS			//  标记来忽略这些警告
#define _CRT_SECURE_NO_DEPRECATE		//  标记来忽略这些警告

#include "targetver.h"

// Windows 头文件: 
//#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

////////////////////////////////// MFC模块 //////////////////////////////////
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <atlimage.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
////////////////////////////////// MFC模块 //////////////////////////////////

// TODO:  在此处引用程序需要的其他头文件
#include<atlstr.h>
#include <fstream>
#include <regex>
#include <direct.h>
#include "resource.h"

//加载vc读取.rc版本信息库
#pragma comment(lib, "version.lib")

////////////////////////////////////////////////////////////////////////////
//Duilib

// 是否启用游戏调试模式
//#define GAME_TEST_SWITCH

// 是否启用Winform框架嵌套模式
#define WINFORM_NESTED_SWITCH

// 是否跳过版本检测(取消注释即可生效)
//#define SKIP_VERSION_CHECK

// 是否开启资源测试模式(开启测试模式则从磁盘读取,否则从Dll中读取,取消注释即可生效)
//#define SKIN_RES_TEST

#ifndef SKIN_RES_TEST
	#define SKINE_ZIPRE_ID	101		// 皮肤资源包ID(值在资源dll中定义)
#endif

// 大厅风格(同时只能存在一种,取消注释即可生效)
//#define GAMEPLAZA_STYLE_1		// 默认风格
#define GAMEPLAZA_STYLE_2		// 列表风格

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
*	tinyxml库*/
#include "..\\..\\ThirdParty\\tinyxml\\tinyXml.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\tinyxml_d.lib")
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\tinyxmlSTL_d.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\tinyxml.lib")
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\tinyxmlSTL.lib")
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

/*
*	Zlib库
*/
#include "..\\..\\ThirdParty\\zlib\\zip.h"
#include "..\\..\\ThirdParty\\zlib\\unzip.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\zlibwapi_d.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\zlibwapi.lib")
#endif

/*
*	Libcurl库基于URL下载
*/
#include "..\\..\\ThirdParty\\libcurl\\include\\curl\\curl.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\libcurld.lib")
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\libcurld_imp.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\libcurl.lib")
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\libcurl_imp.lib")
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
*	添加wke浏览器控件(Chrome内核)
*	暂时处于试用期，没有把wke嵌入Duilib库中,直接在项目中使用
*/
#include "..\\..\\ThirdParty\\wke\\wke.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\wke.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\wke.lib")
#endif

/*
*	RichEdit内嵌Gif表情 支持库
*/
#include "..\\..\\ThirdParty\\imageOle\\IImageOle.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\ImageOleCtrl.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\ImageOleCtrl.lib")
#endif

/*
*	网络模块
*/
#include "..\\..\\ShareModule\\BaseModule\\include\\BaseModule.h"
#include "..\\..\\ShareModule\\NetModule\\include\\NetModule.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\BaseModuleD.lib")
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\NetModuleD.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\BaseModule.lib")
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\NetModule.lib")
#endif

/*
*	短信接口
*/
#include "..\\..\\ThirdParty\\sms\\FYOpenApi.h"
#ifdef DEBUG || _DEBUG
#   ifdef UNICODE || _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\FYOpenApi_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\FYOpenApi_d.lib")
#   endif
#else
#   ifdef UNICODE || _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\FYOpenApi_u.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\FYOpenApi.lib")
#   endif
#endif

// 公用头文件(以后再整理)
#include <shellapi.h>
#include "..\\..\\PublicHeader\\BaseHeader.h"

/*	Duilib菜单控件(因为其中使用了stl，跨模块传递数据有bug)
*	因此单独提出来内置到具体的项目中,以后抽时间修改
*/
#include "..\\share\\define.h"							// 宏定义
#include "..\\share\\wndTimmer.h"						// 基于窗口的定时器
#include "..\\share\\stringConvertor.h"					// 字符串转换
#include "..\\share\\numberConvert.h"					// 字符串转换
#include "..\\share\\smsConfig.h"						// 短信配置
#include "..\\share\\jsonFile.h"						// Json文件读写
#include "..\\share\\unicodemsxml.h"					// Unicode字符集版本 xml解析
#include "..\\share\\libcurlDownloader.h"				// 封装libcurl
#include "..\\share\\zlibUtil.h"						// 封装zlib

#include "..\\share\\ui\\UIDialogFrame.h"				// 对话框
#include "..\\share\\ui\\UIWkeWebkit.h"					// 浏览器
#include "..\\share\\ui\\UIMenu.h"						// 菜单
#include "..\\share\\ui\\UIScreenshotBox.h"				// 裁剪框
#include "..\\share\\ui\\UIControlGdiplus.h"			// Container GDI+绘图

#include "util\\gameUtil.h"

#endif //__STDAFX_H__