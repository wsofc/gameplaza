#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN									// �� Windows ͷ���ų�����ʹ�õ�����
#endif

#ifndef WINVER											// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define WINVER 0x0400									// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define WINVER NTDDI_WINXP
#endif

#ifndef _WIN32_WINNT									// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINNT 0x0400							// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define  _WIN32_WINNT_WINXP
#endif						

#ifndef _WIN32_WINDOWS									// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINDOWS 0x0410							// ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_WINDOWS NTDDI_WINXP
#endif

#ifndef _WIN32_IE										// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
//#define _WIN32_IE 0x0400								// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_IE NTDDI_WINXP
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS				// ĳЩ CString ���캯����Ϊ��ʽ��

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN								//  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <regex>										// ������ʽ
#include <functional>									// ��������⺯��

/*
*	����vc��ȡ.rc�汾��Ϣ��
*/
#pragma comment(lib, "version.lib")

/*
*	Duilib��(�����־)
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
*	Json��
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

//////////////////////////////////////////////////////////////////////////
//���ó���

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)(0xFF))				// ��Ч��ֵ
#define INVALID_WORD				((WORD)(0xFFFF))			// ��Ч��ֵ
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))		// ��Ч��ֵ

//��Ч��ֵ
#define INVALID_TABLE				INVALID_WORD				// ��Ч����
#define INVALID_CHAIR				INVALID_WORD				// ��Ч����

#include "..\\..\\PublicHeader\\GlobalDef\\GlobalFrame.h"		// ȫ�ֶ����
#include "..\\..\\PublicHeader\\Network\\CMD_LogonServer.h"		// ��¼�����ݽṹ�����
#include "..\\..\\PublicHeader\\Network\\CMD_GameServer.h"		// ��Ϸ�����ݽṹ�����

#include "..\\share\\define.h"									// �궨��
#include "..\\share\\stringConvertor.h"							// �ַ���ת��
#include "..\\share\\numberConvert.h"							// ����ת����
#include "..\\share\\jsonFile.h"								// Json�ļ���д
#include "..\\share\\ui\\UIDialogFrame.h"						// �Ի���
#include "..\\share\\ui\\UIControlGdiplus.h"					// Container GDI+��ͼ
#include "..\\share\\control\\IUnrealControl.h"					// ���ƽӿڶ���

#include "..\\GamePlaza\\data\\dataManager.h"
#include "..\\GamePlaza\\util\\gameResource.h"

#include "Unreal.h"