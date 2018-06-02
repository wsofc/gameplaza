/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.0.3
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Project	: https://github.com/ldcsaa
 * Blog		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
Module:  Logger.h

Purpose: ��¼������־��
		1. ����־��Ϣ�����ָ���ļ�
		2. ���� GUI ���򣬿��԰���־��Ϣ���͵�ָ������
		3. ����ConsoleӦ�ó��򣬿��԰���־��Ϣ������׼��� (std::cout)

Desc:
		1�����ܣ�
		--------------------------------------------------------------------------------------
		a) ����־��Ϣ�����ָ���ļ�
		b) ÿ������һ����־�ļ�
		c) ���� GUI ���򣬿��԰���־��Ϣ���͵�ָ������
		d) ����ConsoleӦ�ó��򣬿��԰���־��Ϣ������׼��� (std::cout)
		e) ֧�� MBCS / UNICODE��Console / GUI��win32 / x64 ����
		f) ֧�ֶ�̬���غ;�̬������־��� DLL
		g) ֧�� DEBUG/TRACE/INFO/WARN/ERROR/FATAL �ȶ����־����
		
		2�������ԣ�
		--------------------------------------------------------------------------------------
		a) �򵥴������������κγ�������
		b) ʹ�ýӿڼ򵥣����踴�ӵ����û����ù���
		c) �ṩ CStaticLogger �� CDynamicLogger ��װ�����ھ�̬��̬�����Լ�������־������û������ע����ϸ��
		d) �������Ҫ��¼�����־�ļ�ֻ��Ϊÿ����־�ļ�������Ӧ�� CStaticLogger �� CDynamicLogger ����
		e) ֻ����� Log()/Debug()/Trace()/Info()/Warn()/Error()/Fatal() �ȷ�����¼��־
		f) ��־��¼����֧�ֿɱ����
		g) ��־�����ʽ��<ʱ��> <�߳�ID> <��־����> <��־����>
		
		3�����ܣ�
		--------------------------------------------------------------------------------------
		a) ֧�ֶ��߳�ͬʱ����д��־����
		b) ʹ�õ����߳��ں�̨д��־����Ӱ�칤���̵߳�����ִ��
		c) ����������ʽ������¼��־

Usage:
		����һ������̬���� Logger DLL��
		--------------------------------------------------------------------------------------
		0. Ӧ�ó������ StaticLogger.h ͷ�ļ�
		1. ���� CStaticLogger ����ͨ��Ϊȫ�ֶ���
		2. ���� CStaticLogger->Init(...) ��ʼ����־���
		3. ʹ�� CStaticLogger->Log()/Debug()/Trace()/Info()/Warn()/Error()/Fatal() �ȷ���д��־
		4. ���� CStaticLogger->UnInit(...) ������־�����CStaticLogger ��������ʱҲ���Զ�������־�����

		������������̬���� Logger DLL��
		--------------------------------------------------------------------------------------
		0. Ӧ�ó������ DynamicLogger.h ͷ�ļ�
		1. ���� CDynamicLogger ����ͨ��Ϊȫ�ֶ���
		2. ���� CDynamicLogger->Init(...) ��ʼ����־���
		3. ʹ�� CDynamicLogger->Log()/Debug()/Trace()/Info()/Warn()/Error()/Fatal() �ȷ���д��־
		4. ���� CDynamicLogger->UnInit(...) ������־�����CDynamicLogger ��������ʱҲ���Զ�������־�����

		����������ֱ���õ����������� Logger DLL��
		--------------------------------------------------------------------------------------
		0. Ӧ�ó������ Logger.h ͷ�ļ�
		1. �ֹ����� ILoger_Create() �� ILoger_Create() ������������������ ILogger ���� 
		��ע������Ƕ�̬���أ����ֹ����� ::LoadLibrary()/::FreeLibrary() ϵ�� API �������غ�ж�� Logger DLL��
		
		[
			***** ����ϣ��ͨ�����ڽ�����־��Ϣ�� GUI ���� *****

			A. ��־�����ʼ���ɹ������ SetGUIWindow(HWND) ��������־�Ĵ���
			B. ��������Ӧ���� LOG_MESSAGE ��Ϣ
			C. ������ LOG_MESSAGE ��Ϣ�󣬵��� ILogger::FreeLogMsg() ���ٽ��յ��� TLogMsg 
		]

Environment:
		1. Windows 2000 or later (_WIN32_WINNT >= 0x0500)
		2. VC++ 2010 or later

Release:
		1. Logger_C.dll		- Console/MBCS/Release
		2. Logger_CD.dll	- Console/MBCS/Debug
		3. Logger_CU.dll	- Console/Unicode/Release
		4. Logger_CUD.dll	- Console/Unicode/Debug
		5. Logger.dll		- GUI/MBCS/Release
		6. Logger_D.dll		- GUI/MBCS/Debug
		7. Logger_U.dll		- GUI/Unicode/Release
		8. Logger_UD.dll	- GUI/Unicode/Debug

Examples:
		1. TestGUILogger		- GUI ����Գ���		����̬���أ�
		2. TestDynamicLogger	- GUI ����Գ���		����̬���أ�
		3. TestConsoleLogger	- Console ����Գ���	����̬���أ�

******************************************************************************/

#pragma once

/**************************************************/
/********** imports / exports Logger.dll **********/

#ifdef LOGGER_EXPORTS
	#define LOGGER_API __declspec(dllexport)
	//#define TRY_INLINE	inline
#else
	#define LOGGER_API __declspec(dllimport)
	//#define TRY_INLINE
#endif

/**************************************************/
/****************** ��־����ӿ� *******************/

class LOGGER_API ILogger
{
public:
	/***** ��־���� *****/
	enum LogLevel
	{
		LL_NONE		= 0XFF,
		LL_DEBUG	= 1,
		LL_TRACE	= 2,
		LL_INFO		= 3,
		LL_WARN		= 4,
		LL_ERROR	= 5,
		LL_FATAL	= 6
	};

	/***** ���������� *****/
	enum ErrorCode
	{
		// �޴���
		EC_OK	= NO_ERROR,
		// �ļ�������صĴ���
		EC_FILE_GENERIC,
		EC_FILE_FILENOTFOUND,
		EC_FILE_BADPATH,
		EC_FILE_TOMANYOPERFILES,
		EC_FILE_ACCESSDENIED,
		EC_FILE_INVALIDFILE,
		EC_FILE_REMOVECURRENTDIR,
		EC_FILE_DIRECTORYFULL,
		EC_FILE_BADSEEK,
		EC_FILE_HARDIO,
		EC_FILE_SHARINGVIOLATION,
		EC_FILE_LOCKVIOLATION,
		EC_FILE_DISKFULL,
		EC_FILE_ENDOFFILE,
		// ��������
		EC_INVALID_STATE,
		EC_INIT_LOGLEVEL,
		EC_INIT_PRINTFLAG,
		EC_INIT_CREATE_LOG_THREAD_FAIL
	};

	/******************************************
					��־��Ϣ�ṹ
	*******************************************/
	struct TLogMsg
	{
		DWORD		m_dwSize;		// �ṹ��С - ������Ϣ���ȶ�̬�仯
		LogLevel	m_logLevel;		// ��־����
		UINT		m_uiThreadID;	// �߳�ID
		SYSTEMTIME	m_stMsgTime;	// ��¼ʱ��
		TCHAR		m_psMsg[1];		// ��Ϣ����
	};

public:
	ILogger(void);
	virtual ~ILogger(void);
private:
	ILogger(const ILogger&);
	ILogger& operator = (const ILogger&);

public:
	// ��־�����ʼ������
	virtual BOOL Init(
						LPCTSTR logFile	= NULL					// ��־�ļ�. Ĭ��: {AppPath}/logs/{AppName}-YYYYMMDD.log
					  , LogLevel ll		= DEFAULT_LOG_LEVEL		// ��־����. Ĭ��: [Debug -> LL_DEBUG] / [Release -> LL_INFO]
					  , int printFlag	= DEFAULT_PRINT_FLAG	// �������. �Ƿ�������ļ���(��)��Ļ. Ĭ��: ֻ������ļ�
					 )		= 0;
	// ��־���������
	virtual BOOL UnInit()	= 0;

public:
	// д��־������������־�����ַ��������ڲ���Ҫ��ʽ������־�ı����ñ�����Ч����ߣ�
	virtual void Log_0	(LogLevel ll, LPCTSTR msg) = 0;
	virtual void Debug_0(LPCTSTR msg);
	virtual void Trace_0(LPCTSTR msg);
	virtual void Info_0	(LPCTSTR msg);
	virtual void Warn_0	(LPCTSTR msg);
	virtual void Error_0(LPCTSTR msg);
	virtual void Fatal_0(LPCTSTR msg);

	// д��־�����������ʽ���ַ����Ͳ���ջָ�루ͨ��ֻ������ڲ�ʹ�ã�
	virtual void LogV	(LogLevel ll, LPCTSTR format, va_list arg_ptr);

	// д��־�����������ʽ���ַ����Ϳɱ�������ǳ�����㣩
	virtual void Log	(LogLevel ll, LPCTSTR format, ...);
	virtual void Debug	(LPCTSTR format, ...);
	virtual void Trace	(LPCTSTR format, ...);
	virtual void Info	(LPCTSTR format, ...);
	virtual void Warn	(LPCTSTR format, ...);
	virtual void Error	(LPCTSTR format, ...);
	virtual void Fatal	(LPCTSTR format, ...);

	// д��־�����������ʽ���ַ����Ϳɱ����������һ�鷽�����ƣ����ڽ����κβ���ǰ������־����
	virtual void TryLog		(LogLevel ll, LPCTSTR format, ...);
	virtual void TryDebug	(LPCTSTR format, ...);
	virtual void TryTrace	(LPCTSTR format, ...);
	virtual void TryInfo	(LPCTSTR format, ...);
	virtual void TryWarn	(LPCTSTR format, ...);
	virtual void TryError	(LPCTSTR format, ...);
	virtual void TryFatal	(LPCTSTR format, ...);

	// ͨ�ø�������
	virtual BOOL HasInited			()		const	= 0;		// �Ƿ��Ѿ���ʼ��							
	virtual BOOL IsPrint2File		()		const	= 0;		// �Ƿ����־������ļ�	
	virtual BOOL IsPrint2Screen		()		const	= 0;		// �Ƿ����־�������Ļ����	
	virtual int	GetPrintFlag		()		const	= 0;		// ��ӡ��־					
	virtual LogLevel	GetLogLevel	()		const	= 0;		// ��־����		
	virtual LPCTSTR		GetLogFile	()		const	= 0;		// ��־�ļ�
	virtual ErrorCode	GetLastError()		const	= 0;		// ��ǰ����������

/****************************** GUI ******************************/
#ifdef _WINDOWS
	public:
		// ���ý�����־��Ϣ�Ĵ���, hWndGUI == NULL ��ȡ������
		virtual void SetGUIWindow(HWND hWndGUI)	= 0;
		// ��ȡ������־��Ϣ�Ĵ���
		virtual HWND GetGUIWindow()				= 0;

		// �����ڷ��� LOG_MESSAGE ��Ϣʱ��̬������ TLogMsg ����
		virtual void FreeLogMsg(const TLogMsg* pLogMsg);

		// ���ⴰ�ھ�������룺������ GUI ���ڷ��� LOG_MESSAGE ��Ϣʱ��Ϊ����Դ��ʶ
		static const int LOGGER_FAKE_WINDOW_BASE = 0X80001111;
		// �Զ�����־��Ϣ��ͨ������Ϣ�� GUI ���ڷ�����־
		// ���У�WPARAM -> ILogger ����ָ�룬LPARAM -> TLogMsg �ṹ��ָ��
		static const int LOG_MESSAGE = WM_USER | (0x7FFF & LOGGER_FAKE_WINDOW_BASE);
#endif

public:
	static const int PRINT_FLAG_FILE			= 0x00000001;			// ��ӡ���ļ�
	static const int PRINT_FLAG_SCREEN			= 0x00000002;			// ��ӡ����Ļ
	static const int DEFAULT_PRINT_FLAG			= PRINT_FLAG_FILE;		// Ĭ����־����
	static const LogLevel DEFAULT_LOG_LEVEL		= 
#ifdef _DEBUG
				LL_DEBUG
#else
				LL_INFO
#endif
				;
};

/**************************************************/
/************** Logger DLL �������� ***************/

// ���� ILogger ����
EXTERN_C LOGGER_API ILogger* ILogger_Create();
// ���� ILogger ����
EXTERN_C LOGGER_API void ILogger_Destroy(ILogger* p);

// ��ȡ����־�������������
EXTERN_C LOGGER_API LPCTSTR	ILogger_GetLogLevelDesc	(ILogger::LogLevel ll);
// ��ȡ���������������������
EXTERN_C LOGGER_API LPCTSTR	ILogger_GetErrorDesc	(ILogger::ErrorCode ec);
