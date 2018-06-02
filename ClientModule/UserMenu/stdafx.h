#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN							// �� Windows ͷ���ų�����ʹ�õ�����
#endif

#ifndef WINVER									// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define WINVER 0x0400							// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define WINVER NTDDI_WINXP
#endif

#ifndef _WIN32_WINNT							// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINNT 0x0400					// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define  _WIN32_WINNT_WINXP
#endif						

#ifndef _WIN32_WINDOWS							// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINDOWS 0x0410					// ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_WINDOWS NTDDI_WINXP
#endif

#ifndef _WIN32_IE								// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
//#define _WIN32_IE 0x0400						// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_IE NTDDI_WINXP
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS		// ĳЩ CString ���캯����Ϊ��ʽ��

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN						//  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

#include <regex>								// ������ʽ
#include <functional>							// ��������⺯��

#include "resource.h"

#include "..\\DuiLib\\UIlib.h"
using namespace DuiLib;

#ifdef DEBUG || _DEBUG
#   ifdef UNICODE || _UNICODE
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

#include "..\\..\\PublicHeader\\GlobalDef\\GlobalFrame.h"	// ȫ�ֶ����
#include "..\\share\\define.h"								// �궨��
#include "..\\share\\stringConvertor.h"						// �ַ���ת��
#include "..\\share\\numberConvert.h"						// ����ת����
#include "..\\share\\ui\\UIMenu.h"							// �˵�
#include "..\\share\\ui\\UIDialogFrame.h"					// �Ի�����
#include "..\\share\\control\\IUserControl.h"				// ���ƽӿڶ���

#include "UserMenu.h"