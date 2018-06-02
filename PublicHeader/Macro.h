#pragma once


//msgbx
//
#define		MSGBX(hwnd, title)	\
	::MessageBox(hwnd,title, "MessageBox", MB_OK);

#define		MSGBX_ERROR(hwnd, title)	\
	::MessageBox(hwnd,title, "MessageBox", MB_ICONERROR|MB_OK);

#define		MSGBX_(title)		MSGBX(NULL, title)
#define		MSGBX_ERROR_(title)	MSGBX_ERROR(NULL, title)


//delete gdi handle
//eg: hBrush,hFont,and so on
#define	DELETE_GDIHANDLE(hHandle)\
		if (hHandle)\
		{\
			::DeleteObject(hHandle);\
			hHandle = NULL;\
		}

//delete gdi object
//eg: CBrush object,CFont object,and so on
#define	DELETE_GDIOBJECT(Object)\
		if (Object.GetSafeHandle())\
		{\
			Object.DeleteObject();\
		}

//close kernel handle
//eg: hEvent,hThread,and so on
#define	CLOSE_KERNELHANDLE(hHandle)\
		if (hHandle)\
		{\
			::CloseHandle(hHandle);\
			hHandle = NULL;\
		}

#define	DELETE_CWND_OBJECT(obj)\
	if (obj && ::IsWindow(obj->m_hWnd))\
		{\
		obj->DestroyWindow();\
		}\
		if (obj)\
		{\
		delete obj;\
		obj = NULL;\
		}


//////////////////////////////////////////////////////////////////////////////////

//接口释放
#define SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//删除指针
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//关闭句柄
#define SafeCloseHandle(hHandle) { if (hHandle!=NULL) { CloseHandle(hHandle); hHandle=NULL; } }

//删除数组
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//////////////////////////////////////////////////////////////////////////////////




//
//Trace
//
#define	MYTRACE			TRACE("    %s\r\n", __FUNCTION__)
#define	MYTRACE_ERROR	TRACE("    ---Error.    %s\r\n", __FUNCTION__)
#define	MYTRACE_N(N)	TRACE("    ---Error.    %s,%d\r\n", __FUNCTION__, N)
#define	MYASSERT		ASSERT(0)
#define	MYASSERT_(f)	ASSERT(f)

#ifdef _DEBUG
	#define REPORT_ME(hWnd) \
		char szBuf[256] = {0};\
		_snprintf(szBuf, sizeof(szBuf),"%s,%d,", __FUNCTION__,__LINE__);\
		MSGBX(hWnd, szBuf);
#else
	#define REPORTME
#endif




//-----------------------------------------------
//					the end
//-----------------------------------------------
