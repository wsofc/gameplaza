#ifndef ENCRYPT_HEAD_FILE
#define ENCRYPT_HEAD_FILE

#include "BaseModule.h"


//////////////////////////////////////////////////////////////////////////
//MD5 ������
class BASE_MODULE_CLASS CMD5Encrypt
{
	//��������
private:
	CMD5Encrypt() {}

	//���ܺ���
public:
	//��������
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
	//���ɴ�дʮ����������
	static void EncryptData_CapitalHex(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
};


//////////////////////////////////////////////////////////////////////////
//��������
class BASE_MODULE_CLASS CXOREncrypt
{
	//��������
private:
	CXOREncrypt() {}

	//���ܺ���
public:
	//��������
	static WORD EncryptData(LPCTSTR pszSrcData, LPTSTR pszEncrypData, WORD wSize);
	//�⿪����
	static WORD CrevasseData(LPCTSTR pszEncrypData, LPTSTR pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////

#endif