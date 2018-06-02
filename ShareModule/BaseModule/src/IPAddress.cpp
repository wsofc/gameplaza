#include "StdAfx.h"
#include "IPAddress.h"
#include <Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

CIPAddress::CIPAddress(void)
{
}

CIPAddress::~CIPAddress(void)
{
}

//从控件得到IP地址
bool CIPAddress::s_GetIPAddress(CIPAddressCtrl *pCtrl, DWORD & dwIPAddress)
{
	if(!pCtrl || !pCtrl->GetSafeHwnd())
	{
		return false;
	}
	pCtrl->GetAddress(dwIPAddress);
	dwIPAddress = ::htonl(dwIPAddress);
	return true;
}
bool CIPAddress::s_GetIPAddress(CIPAddressCtrl *pCtrl, CString & strIPAddress)
{
	if(!pCtrl || !pCtrl->GetSafeHwnd())
	{
		return false;
	}
	DWORD dwIPAddress;
	if(!CIPAddress::s_GetIPAddress(pCtrl, dwIPAddress))
	{
		return false;
	}
	strIPAddress.Format(TEXT("%d.%d.%d.%d"),
				*(((BYTE *)&dwIPAddress)+0),
				*(((BYTE *)&dwIPAddress)+1),
				*(((BYTE *)&dwIPAddress)+2),
				*((BYTE *)&dwIPAddress)+3);
	return true;
}

//设置IP地址到控件,控件 使用 主机字节序
bool CIPAddress::s_SetIPAddress(CIPAddressCtrl *pCtrl, DWORD dwIPAddress)
{
	if(!pCtrl || !pCtrl->GetSafeHwnd())
	{
		return false;
	}
	dwIPAddress = ::ntohl(dwIPAddress);
	pCtrl->SetAddress(dwIPAddress);
	return true;
}
bool CIPAddress::s_SetIPAddress(CIPAddressCtrl *pCtrl, CString & strIPAddress)
{
	if(!pCtrl || !pCtrl->GetSafeHwnd())
	{
		return false;
	}
	
	DWORD dwIPAddress = INADDR_NONE;
	CIPAddress::s_String2DWORD(strIPAddress, dwIPAddress);

	CIPAddress::s_SetIPAddress(pCtrl, dwIPAddress);
	return true;
}
					
//是否是有效的IP地址
bool CIPAddress::s_IsValidIPAddress(DWORD dwIPAddress)
{
	if(dwIPAddress==0 || dwIPAddress==INADDR_NONE)
	{
		return false;
	}
	const BYTE *pAddrByte = (BYTE *)&dwIPAddress;
	if(pAddrByte[0] == 0)
	{
		return false;
	}

	return true;
}
		
//IP地址转换
void CIPAddress::s_DWORD2String(DWORD dwIPAddress, CString & str)
{
	const BYTE *pAddrByte = (BYTE *)&dwIPAddress;
	str.Format(TEXT("%d.%d.%d.%d"),
				pAddrByte[0],pAddrByte[1],pAddrByte[2],pAddrByte[3]);
}
void CIPAddress::s_DWORD2String(DWORD dwIPAddress,LPTSTR lpStr,DWORD dwLen)
{
	if(!lpStr || dwLen<16)
	{
		ASSERT(false);
		return ;
	}
	const BYTE *pAddrByte = (BYTE *)&dwIPAddress;
	wsprintf(lpStr, _T("%d.%d.%d.%d"), pAddrByte[0], pAddrByte[1], pAddrByte[2], pAddrByte[3]);
}

//地址使用网络字节顺序
void CIPAddress::s_String2DWORD(const CString & str, DWORD & dwIPAddress)
{

#ifdef _UNICODE
	char* cstr = nullptr;
	int len = ::WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	cstr = new char[len + 1];
	::WideCharToMultiByte(CP_ACP, 0, str, -1, cstr, len, NULL, NULL);
	dwIPAddress = ::inet_addr(cstr);
	SAFE_DELETE(cstr);
#else
	dwIPAddress = ::inet_addr(str);
#endif // _UNICODE

}

//得到本地IP
bool CIPAddress::s_GetLocalIPAddress(DWORD & dwIPAddress)
{
	dwIPAddress = 0;
	CString strIP;

	char local[255] = {0};
	::gethostname(local, sizeof(local));
	LPHOSTENT lpHost = ::gethostbyname(local);
	if(lpHost == NULL)
	{
		return false;
	}
	strIP = inet_ntoa (*(struct in_addr *)*lpHost->h_addr_list);
	CIPAddress::s_String2DWORD(strIP, dwIPAddress);
	return true;
}
					
bool CIPAddress::s_GetLocalIPAddress(std::vector<tagIPItem> & IPItem_)
{
	IPItem_.clear();

	//服务地址
	char szHostName[64];
	gethostname(szHostName,sizeof(szHostName));
	HOSTENT * pHostent=gethostbyname(szHostName);
	LPIN_ADDR * lpAddrList=(LPIN_ADDR *)pHostent->h_addr_list;
    while (*lpAddrList!=NULL)
	{
		tagIPItem Item;
		TCHAR* strAddr = nullptr;
#ifdef _UNICODE
		char* cstr = inet_ntoa(**lpAddrList);
		size_t iLen = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
		strAddr = new TCHAR[iLen];
		::MultiByteToWideChar(CP_ACP, 0, cstr, -1, strAddr, iLen);
		lstrcpyn(Item.szAddress, strAddr, ArrayCount(Item.szAddress));
		SAFE_DELETE(strAddr);
#else
		strAddr = inet_ntoa(**lpAddrList);
		lstrcpyn(Item.szAddress, strAddr, ArrayCount(Item.szAddress));
#endif // _UNICODE
		
		IPItem_.push_back(Item);

		lpAddrList++;
	}
	return IPItem_.size()>0;
}


//-----------------------------------------------
//					the end
//-----------------------------------------------