/**********************************************************************
* Copyright (C) 2008 - 棋牌网络 - All Rights Reserved
*
* 文件名称:		XORCoding.h
* 摘    要:		异或加密解密器
*     
* 作    者:		zcc, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#ifndef __SocketModule_XorCoding_H__
#define __SocketModule_XorCoding_H__


#include "NetModule.h"


//////////////////////////////////////////////////////////////////////////
/// 异或加密解密器
//////////////////////////////////////////////////////////////////////////
class CXorCoding
{
public:
	CXorCoding(void);
	~CXorCoding(void);

public:
	/// 初始化
	void Reset();

	/// 生成密钥
	void GenerateKey();

	/// 设置公共密钥
	/// @param [in] dwEncodeKey 加密密钥
	/// @param [in] dwDecodeKey 解密密钥
	void SetPublicKey(uint32 dwEncodeKey, uint32 dwDecodeKey);

	uint32 GetEncodeKey() const { return m_dwEncodeKey; }
	uint32 GetDecodeKey() const { return m_dwDecodeKey; }

public:
	/// 加密数据-异或
	uint32 Encode(uint16* pwSeed,
		uint32* pdwXor,
		const uint16& wEncrypCount );

	/// 解密数据-异或
	uint32 Decode(uint16* pwSeed,
		uint32* pdwXor,
		const uint16& wEncrypCount,
		const uint16& wSnapCount,
		uint8* pcbDataBuffer,
		const uint16& wDataSize);

protected:
	/// 随机映射
	uint16 SeedRandMap(uint16 wSeed);

protected:
	uint32							m_dwEncodeKey;			///< 加密密钥
	uint32							m_dwDecodeKey;			///< 解密密钥
};


#endif
