/*

1 IP��ַʹ�������ֽ�˳��,��λ��ǰ

3 http://blog.csdn.net/icedmilk/article/details/5336296
4 http://blog.csdn.net/Sunboy_2050/article/details/6061528

5 inet_addr() ���������ֽ���

6 CIPAddressCtrl �ؼ� ʹ�� �����ֽ���

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
					//�ӿؼ��õ�IP��ַ
	static bool s_SetIPAddress(CIPAddressCtrl *pCtrl, DWORD dwIPAddress);
	static bool s_SetIPAddress(CIPAddressCtrl *pCtrl, CString & strIPAddress);
					//����IP��ַ���ؼ�
	static bool s_IsValidIPAddress(DWORD dwIPAddress);
					//�Ƿ�����Ч��IP��ַ
	static void s_DWORD2String(DWORD dwIPAddress, CString & str);
	static void s_DWORD2String(DWORD dwIPAddress, LPTSTR lpStr, DWORD dwLen);
	static void s_String2DWORD(const CString & str, DWORD & dwIPAddress);
					//IP��ַת��
	static bool s_GetLocalIPAddress(DWORD & dwIPAddress);
	static bool s_GetLocalIPAddress(std::vector<tagIPItem> & IPItem_);
					//�õ�����IP
};



//-----------------------------------------------
//					the end
//-----------------------------------------------