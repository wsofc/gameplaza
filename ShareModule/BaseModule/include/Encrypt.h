#ifndef ENCRYPT_HEAD_FILE
#define ENCRYPT_HEAD_FILE

#include "BaseModule.h"


//////////////////////////////////////////////////////////////////////////
//MD5 加密类
class BASE_MODULE_CLASS CMD5Encrypt
{
	//函数定义
private:
	CMD5Encrypt() {}

	//功能函数
public:
	//生成密文
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
	//生成大写十六进制密文
	static void EncryptData_CapitalHex(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
};


//////////////////////////////////////////////////////////////////////////
//异或加密类
class BASE_MODULE_CLASS CXOREncrypt
{
	//函数定义
private:
	CXOREncrypt() {}

	//功能函数
public:
	//生成密文
	static WORD EncryptData(LPCTSTR pszSrcData, LPTSTR pszEncrypData, WORD wSize);
	//解开密文
	static WORD CrevasseData(LPCTSTR pszEncrypData, LPTSTR pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////

#endif