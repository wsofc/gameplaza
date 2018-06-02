#include "Stdafx.h"
#include <afxdllx.h>

//���ͷ�ļ�
#include "BaseModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE BaseModuleDLL={NULL,NULL};

//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(BaseModuleDLL,hInstance)) return 0;
		new CDynLinkLibrary(BaseModuleDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(BaseModuleDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
