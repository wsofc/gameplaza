#ifndef _IUNREAL_H
#define _IUNREAL_H

// 借用Windows.h里面interface的定义
#ifndef __STRUCT__
	#define __STRUCT__ struct
#endif // __STRUCT__

#ifndef interface
	#define interface __STRUCT__
#endif // interface

#include <atlstr.h>

namespace UnrealNSP
{
	enum UnrealChargeType
	{
		UCT_NONE = 0
		, UCT_CHARM
		, UCT_TRANSFER
		, UCT_PLAZA_WITHDRAW
		, UCT_GAMES_WITHDRAW
		, UCT_EXTRA
	};
}

// 回调接口
interface IUnrealControl
{
	virtual void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33])													= 0;
	virtual DWORD SendData(BYTE cbSendLocation, WORD wMainCmdID, WORD wSubCmdID)										= 0;
	virtual DWORD SendData(BYTE cbSendLocation, WORD wMainCmdID, WORD wSubCmdID, void* const pData, WORD wDataSize)		= 0;
};

#endif // _IUNREAL_H