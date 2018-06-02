/**********************************************************************
* Copyright (C) 2008 - 棋牌网络 - All Rights Reserved
*
* 文件名称:		NetPacketParserXorMap.h
* 摘    要:		网络数据包解析器 - 异或&映射表混合加密
*     
* 作    者:		yanglinbo, zcc
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#ifndef __NetModule_NetPacketParserXorMap_H__
#define __NetModule_NetPacketParserXorMap_H__


#include "NetModule.h"


class CXorCoding;
class CMapCoding;


//////////////////////////////////////////////////////////////////////////
/// 网络数据包解析器
//////////////////////////////////////////////////////////////////////////
class NET_MODULE_CLASS CNetPacketParserXorMap
{
public:
	CNetPacketParserXorMap(void);
	~CNetPacketParserXorMap(void);

public:
	virtual void Reset();

	virtual bool Pack(const uint8* pInData, int nInLen, uint8* pOutData, int& nOutLen);

	virtual bool Unpack(const uint8* pInData, int nInLen, uint8* pOutData, int& nOutLen);

	virtual void NeedPublicKey(const bool bNeed = true);

private:
	bool WritePublicKey( uint8* pOutData, int& nOutLen, uint32 dwXorKey );

	bool ReadPublicKey( uint8* pOutData, int& nOutLen );

	const bool& IsNeedPublicKey() const { return m_bNeedPublicKey; }

	const bool& IsFirstPack() const { return m_bFirstPack; }

	const bool& IsFirstUnpack() const { return m_bFirstUnpack; }

private:
	CXorCoding*	m_XorCoder;			///< 异或加密解密器
	bool		m_bEnableXORCodec;	///< 异或编解码器开关

	CMapCoding*	m_MapCoder;			///< 映射表加密解密器
	bool		m_bEnableMapCodec;	///< 映射表编解码器开关

	bool		m_bFirstPack;		///< 第一次封包
	bool		m_bFirstUnpack;		///< 第一次解包

	bool		m_bNeedPublicKey;	///< 需要公共密钥标志
};


#endif
