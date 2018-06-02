/*

1 IP地址使用网络字节顺序,高位在前

3 http://blog.csdn.net/icedmilk/article/details/5336296
4 http://blog.csdn.net/Sunboy_2050/article/details/6061528

5 inet_addr() 返回网络字节序

6 CIPAddressCtrl 控件 使用 主机字节序

*/

#pragma once

#include "BaseModule.h"
#include <vector>


struct tagIPItem
{
	tagIPItem(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}
	TCHAR	szAddress[32];
};

class BASE_MODULE_CLASS CIPAddress
{
protected:
	CIPAddress(void);
	~CIPAddress(void);

public:
	static bool s_GetIPAddress(CIPAddressCtrl *pCtrl, DWORD & dwIPAddress);
	static bool s_GetIPAddress(CIPAddressCtrl *pCtrl, CString & strIPAddress);
					//从控件得到IP地址
	static bool s_SetIPAddress(CIPAddressCtrl *pCtrl, DWORD dwIPAddress);
	static bool s_SetIPAddress(CIPAddressCtrl *pCtrl, CString & strIPAddress);
					//设置IP地址到控件
	static bool s_IsValidIPAddress(DWORD dwIPAddress);
					//是否是有效的IP地址
	static void s_DWORD2String(DWORD dwIPAddress, CString & str);
	static void s_DWORD2String(DWORD dwIPAddress, LPTSTR lpStr, DWORD dwLen);
	static void s_String2DWORD(const CString & str, DWORD & dwIPAddress);
					//IP地址转换
	static bool s_GetLocalIPAddress(DWORD & dwIPAddress);
	static bool s_GetLocalIPAddress(std::vector<tagIPItem> & IPItem_);
					//得到本地IP
};



//-----------------------------------------------
//					the end
//-----------------------------------------------