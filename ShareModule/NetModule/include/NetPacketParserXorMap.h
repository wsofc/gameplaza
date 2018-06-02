/**********************************************************************
* Copyright (C) 2008 - �������� - All Rights Reserved
*
* �ļ�����:		NetPacketParserXorMap.h
* ժ    Ҫ:		�������ݰ������� - ���&ӳ����ϼ���
*     
* ��    ��:		yanglinbo, zcc
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#ifndef __NetModule_NetPacketParserXorMap_H__
#define __NetModule_NetPacketParserXorMap_H__


#include "NetModule.h"


class CXorCoding;
class CMapCoding;


//////////////////////////////////////////////////////////////////////////
/// �������ݰ�������
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
	CXorCoding*	m_XorCoder;			///< �����ܽ�����
	bool		m_bEnableXORCodec;	///< �������������

	CMapCoding*	m_MapCoder;			///< ӳ�����ܽ�����
	bool		m_bEnableMapCodec;	///< ӳ�������������

	bool		m_bFirstPack;		///< ��һ�η��
	bool		m_bFirstUnpack;		///< ��һ�ν��

	bool		m_bNeedPublicKey;	///< ��Ҫ������Կ��־
};


#endif
