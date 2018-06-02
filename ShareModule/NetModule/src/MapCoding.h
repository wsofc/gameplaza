/**********************************************************************
* Copyright (C) 2008 - 棋牌网络 - All Rights Reserved
*
* 文件名称:		MapCoding.h
* 摘    要:		映射表加密解密器
*     
* 作    者:		zcc, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#ifndef __SocketModule_MapCoding_H__
#define __SocketModule_MapCoding_H__


#include "NetModule.h"


//////////////////////////////////////////////////////////////////////////
/// 映射表加密解密器
//////////////////////////////////////////////////////////////////////////
class CMapCoding
{
public:
	CMapCoding(void);
	~CMapCoding(void);

public:
	/// 重置加密解密器
	void Reset();

public:
	/// 加密数据
	bool Encode(uint8* arrDataBuffer, 
		const int wDataSize, 
		uint8& cbVerifyCode);

	/// 解密数据
	bool Decode(uint8* arrDataBuffer, 
		const int wDataSize, 
		uint8& cbVerifyCode);

protected:
	/// 加密单个数据
	uint8 EncodeByte(const uint8 cbData);

	/// 解密单个数据
	uint8 DecodeByte(const uint8 cbData);

private:
	uint8					m_cbEncodeRound;			///< 加密字节映射
	uint8					m_cbDecodeRound;			///< 解密字节映射
};


#endif
