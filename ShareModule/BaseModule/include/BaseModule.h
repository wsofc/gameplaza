#pragma once

//ƽ̨ͷ�ļ�
#include "../../../PublicHeader/Template.h"
#include "../../../PublicHeader/SafeDelete.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef BASE_MODULE_CLASS
#	ifdef  BASE_MODULE_DLL
#		define BASE_MODULE_CLASS _declspec(dllexport)
#	else
#		define BASE_MODULE_CLASS _declspec(dllimport)
#	endif
#endif

//ģ�鶨��
#ifdef _DEBUG
#	define BASE_MODULE_DLL_NAME	TEXT("BaseModuleD.dll")			//��� DLL ����
#else
#	define BASE_MODULE_DLL_NAME	TEXT("BaseModule.dll")			//��� DLL ����
#endif

//
// �Զ����ӿ�.
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


//������ͷ�ļ�
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

