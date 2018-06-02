/**********************************************************************
* Copyright (C) 2008 - 棋牌网络 - All Rights Reserved
*
* 文件名称:		XORCoding.cpp
* 摘    要:		异或加密解密器
*     
* 作    者:		zcc, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#include "StdAfx.h"
#include "XORCoding.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/// 公共密钥
//////////////////////////////////////////////////////////////////////////
static const uint32 XOR_PUBLIC_KEY = 0x20121014;


CXorCoding::CXorCoding(void)
{
	Reset();
}

CXorCoding::~CXorCoding(void)
{
}

void CXorCoding::Reset()
{
	m_dwEncodeKey		= 0x12345678;
	m_dwDecodeKey		= 0x12345678;
}

void CXorCoding::GenerateKey()
{
	uint32 dwXorKey = m_dwEncodeKey;

	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		dwXorKey	= GetTickCount() * GetTickCount();
		dwXorKey	^= guid.Data1;
		dwXorKey	^= guid.Data2;
		dwXorKey	^= guid.Data3;
		dwXorKey	^= *((uint32*)guid.Data4);
	}

	//随机映射种子
	dwXorKey = SeedRandMap((uint16)dwXorKey);

	dwXorKey	|= ((uint32)SeedRandMap((uint16)(dwXorKey >> 16))) << 16;
	dwXorKey	^= XOR_PUBLIC_KEY;

	m_dwEncodeKey	= dwXorKey;
	m_dwDecodeKey	= dwXorKey;
}

uint16 CXorCoding::SeedRandMap( uint16 wSeed )
{
	uint32 dwHold = wSeed;
	return (uint16)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

uint32 CXorCoding::Encode(uint16* pwSeed,
						  uint32* pdwXor,
						  const uint16& wEncrypCount )
{
	//TRACE("CXorCoding::Encode[%u]\n", m_dwEncodeKey );
	uint32 dwNewXorKey = m_dwEncodeKey;
	const uint32 dwOldXorKey = m_dwEncodeKey;

	for (int i=0; i<wEncrypCount; ++i)
	{
		*pdwXor++ ^= dwNewXorKey;
		dwNewXorKey = SeedRandMap(*pwSeed++);
		dwNewXorKey |= ((uint32)SeedRandMap(*pwSeed++)) << 16;
		dwNewXorKey ^= XOR_PUBLIC_KEY;
	}

	m_dwEncodeKey = dwNewXorKey;

	return dwOldXorKey;
}

uint32 CXorCoding::Decode(uint16* pwSeed,
						  uint32* pdwXor,
						  const uint16& wEncrypCount,
						  const uint16& wSnapCount,
						  uint8* pcbDataBuffer,
						  const uint16& wDataSize)
{
	//TRACE("CXorCoding::Encode[%u]\n", m_dwEncodeKey );
	uint32 dwNewXorKey = m_dwDecodeKey;

	for (int i=0; i<(int)wEncrypCount; ++i)
	{
		if ((i==(wEncrypCount-1)) && (wSnapCount>0))
		{
			//TRACE("CXorCoding::Encode wEncrypCount[%u] wSnapCount[%u]\n", wEncrypCount, wSnapCount );
			uint8* pcbKey = ((uint8*)&m_dwDecodeKey) + sizeof(uint32) - wSnapCount;
			CopyMemory(pcbDataBuffer+wDataSize, pcbKey, wSnapCount);
		}

		dwNewXorKey	= SeedRandMap(*pwSeed++);
		dwNewXorKey	|= ((uint32)SeedRandMap(*pwSeed++)) << 16;
		dwNewXorKey	^= XOR_PUBLIC_KEY;
		*pdwXor++	^= m_dwDecodeKey;
		m_dwDecodeKey = dwNewXorKey;
	}

	return m_dwDecodeKey;
}

void CXorCoding::SetPublicKey( uint32 dwEncodeKey, uint32 dwDecodeKey )
{
	m_dwEncodeKey = dwEncodeKey;
	m_dwDecodeKey = dwDecodeKey;
}
