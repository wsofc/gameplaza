// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include "targetver.h"

// Windows ͷ�ļ�: 
//#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//////////////////////////////////// MFCģ�� //////////////////////////////////
//#include <afxwin.h>         // MFC ��������ͱ�׼���
//#include <afxext.h>         // MFC ��չ
//#include <atlimage.h>
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxole.h>         // MFC OLE ��
//#include <afxodlgs.h>       // MFC OLE �Ի�����
//#include <afxdisp.h>        // MFC �Զ�����
//#endif // _AFX_NO_OLE_SUPPORT
//
//#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>			// MFC ODBC ���ݿ���
//#endif // _AFX_NO_DB_SUPPORT
//
//#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO ���ݿ���
//#endif // _AFX_NO_DAO_SUPPORT
//
//#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
//#endif // _AFX_NO_AFXCMN_SUPPORT
//////////////////////////////////// MFCģ�� //////////////////////////////////

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
*	���wke������ؼ�(Chrome�ں�)
*	��ʱ���������ڣ�û�а�wkeǶ��Duilib����,ֱ������Ŀ��ʹ��
*/
#include "..\\..\\ThirdParty\\wke\\wke.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\wke.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\wke.lib")
#endif

/*
*	Json��
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

/*
*	Zlib��
*/
#include "..\\..\\ThirdParty\\zlib\\zip.h"
#include "..\\..\\ThirdParty\\zlib\\unzip.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\PublicLibrary\\debug\\zlibwapi_d.lib")
#else
	#pragma comment(lib, "..\\..\\PublicLibrary\\release\\zlibwapi.lib")
#endif

/*
*	Libcurl�����URL����
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
*	VC-Logge��(�����־)
*/
#include "..\\..\\ThirdParty\\vc-logge\\StaticLogger.h"
#include "..\\..\\ThirdParty\\vc-logge\\DynamicLogger.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\Logger_CD.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\Logger_C.lib")
#endif

// ����ͷ�ļ�
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

// �����궨��
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