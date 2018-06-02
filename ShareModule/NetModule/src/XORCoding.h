/**********************************************************************
* Copyright (C) 2008 - �������� - All Rights Reserved
*
* �ļ�����:		XORCoding.h
* ժ    Ҫ:		�����ܽ�����
*     
* ��    ��:		zcc, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#ifndef __SocketModule_XorCoding_H__
#define __SocketModule_XorCoding_H__


#include "NetModule.h"


//////////////////////////////////////////////////////////////////////////
/// �����ܽ�����
//////////////////////////////////////////////////////////////////////////
class CXorCoding
{
public:
	CXorCoding(void);
	~CXorCoding(void);

public:
	/// ��ʼ��
	void Reset();

	/// ������Կ
	void GenerateKey();

	/// ���ù�����Կ
	/// @param [in] dwEncodeKey ������Կ
	/// @param [in] dwDecodeKey ������Կ
	void SetPublicKey(uint32 dwEncodeKey, uint32 dwDecodeKey);

	uint32 GetEncodeKey() const { return m_dwEncodeKey; }
	uint32 GetDecodeKey() const { return m_dwDecodeKey; }

public:
	/// ��������-���
	uint32 Encode(uint16* pwSeed,
		uint32* pdwXor,
		const uint16& wEncrypCount );

	/// ��������-���
	uint32 Decode(uint16* pwSeed,
		uint32* pdwXor,
		const uint16& wEncrypCount,
		const uint16& wSnapCount,
		uint8* pcbDataBuffer,
		const uint16& wDataSize);

protected:
	/// ���ӳ��
	uint16 SeedRandMap(uint16 wSeed);

protected:
	uint32							m_dwEncodeKey;			///< ������Կ
	uint32							m_dwDecodeKey;			///< ������Կ
};


#endif
