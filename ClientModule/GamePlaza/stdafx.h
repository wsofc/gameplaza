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
#define _CRT_SECURE_NO_WARNINGS			//  �����������Щ����
#define _CRT_SECURE_NO_DEPRECATE		//  �����������Щ����

#include "targetver.h"

// Windows ͷ�ļ�: 
//#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

////////////////////////////////// MFCģ�� //////////////////////////////////
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <atlimage.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
////////////////////////////////// MFCģ�� //////////////////////////////////

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include<atlstr.h>
#include <fstream>
#include <regex>
#include <direct.h>
#include "resource.h"

//����vc��ȡ.rc�汾��Ϣ��
#pragma comment(lib, "version.lib")

////////////////////////////////////////////////////////////////////////////
//Duilib

// �Ƿ�������Ϸ����ģʽ
//#define GAME_TEST_SWITCH

// �Ƿ�����Winform���Ƕ��ģʽ
#define WINFORM_NESTED_SWITCH

// �Ƿ������汾���(ȡ��ע�ͼ�����Ч)
//#define SKIP_VERSION_CHECK

// �Ƿ�����Դ����ģʽ(��������ģʽ��Ӵ��̶�ȡ,�����Dll�ж�ȡ,ȡ��ע�ͼ�����Ч)
//#define SKIN_RES_TEST

#ifndef SKIN_RES_TEST
	#define SKINE_ZIPRE_ID	101		// Ƥ����Դ��ID(ֵ����Դdll�ж���)
#endif

// �������(ͬʱֻ�ܴ���һ��,ȡ��ע�ͼ�����Ч)
//#define GAMEPLAZA_STYLE_1		// Ĭ�Ϸ��
#define GAMEPLAZA_STYLE_2		// �б���

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
*	tinyxml��*/
#include "..\\..\\ThirdParty\\tinyxml\\tinyXml.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\tinyxml_d.lib")
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\tinyxmlSTL_d.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\tinyxml.lib")
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\tinyxmlSTL.lib")
#endif

/*
*	Json��
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

/*
*	Zlib��
*/
#include "..\\..\\ThirdParty\\zlib\\zip.h"
#include "..\\..\\ThirdParty\\zlib\\unzip.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\zlibwapi_d.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\zlibwapi.lib")
#endif

/*
*	Libcurl�����URL����
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
*	VC-Logge��(�����־)
*/
#include "..\\..\\ThirdParty\\vc-logge\\StaticLogger.h"
#include "..\\..\\ThirdParty\\vc-logge\\DynamicLogger.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\Logger_CD.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\Logger_C.lib")
#endif

/*
*	���wke������ؼ�(Chrome�ں�)
*	��ʱ���������ڣ�û�а�wkeǶ��Duilib����,ֱ������Ŀ��ʹ��
*/
#include "..\\..\\ThirdParty\\wke\\wke.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\wke.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\wke.lib")
#endif

/*
*	RichEdit��ǶGif���� ֧�ֿ�
*/
#include "..\\..\\ThirdParty\\imageOle\\IImageOle.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\ImageOleCtrl.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\ImageOleCtrl.lib")
#endif

/*
*	����ģ��
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
*	���Žӿ�
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

// ����ͷ�ļ�(�Ժ�������)
#include <shellapi.h>
#include "..\\..\\PublicHeader\\BaseHeader.h"

/*	Duilib�˵��ؼ�(��Ϊ����ʹ����stl����ģ�鴫��������bug)
*	��˵�����������õ��������Ŀ��,�Ժ��ʱ���޸�
*/
#include "..\\share\\define.h"							// �궨��
#include "..\\share\\wndTimmer.h"						// ���ڴ��ڵĶ�ʱ��
#include "..\\share\\stringConvertor.h"					// �ַ���ת��
#include "..\\share\\numberConvert.h"					// �ַ���ת��
#include "..\\share\\smsConfig.h"						// ��������
#include "..\\share\\jsonFile.h"						// Json�ļ���д
#include "..\\share\\unicodemsxml.h"					// Unicode�ַ����汾 xml����
#include "..\\share\\libcurlDownloader.h"				// ��װlibcurl
#include "..\\share\\zlibUtil.h"						// ��װzlib

#include "..\\share\\ui\\UIDialogFrame.h"				// �Ի���
#include "..\\share\\ui\\UIWkeWebkit.h"					// �����
#include "..\\share\\ui\\UIMenu.h"						// �˵�
#include "..\\share\\ui\\UIScreenshotBox.h"				// �ü���
#include "..\\share\\ui\\UIControlGdiplus.h"			// Container GDI+��ͼ

#include "util\\gameUtil.h"

#endif //__STDAFX_H__