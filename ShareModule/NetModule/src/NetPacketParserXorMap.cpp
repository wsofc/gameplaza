/**********************************************************************
* Copyright (C) 2008 - �������� - All Rights Reserved
*
* �ļ�����:		NetPacketParserXorMap.cpp
* ժ    Ҫ:		�������ݰ������� - ���&ӳ����ϼ���
*     
* ��    ��:		zcc, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#include "StdAfx.h"
#include "NetPacketParserXorMap.h"
#include "SafeDelete.h"
#include "XorCoding.h"
#include "MapCoding.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int MESSAGE_INFO_LEN		= (int) sizeof(CMD_Info);
const int MESSAGE_CMD_LEN		= (int) sizeof(CMD_Command);
const int DWORD_LEN				= (int) sizeof(uint32);


CNetPacketParserXorMap::CNetPacketParserXorMap(void)
: m_bNeedPublicKey(false)
, m_bEnableXORCodec(true)
, m_bEnableMapCodec(true)
{
	m_XorCoder = new CXorCoding;
	m_MapCoder = new CMapCoding;
	Reset();
}

CNetPacketParserXorMap::~CNetPacketParserXorMap(void)
{
	SafeDelete(m_XorCoder);
	SafeDelete(m_MapCoder);
}

void CNetPacketParserXorMap::Reset()
{
	m_bFirstPack	= true;
	m_bFirstUnpack	= true;

	m_XorCoder->Reset();
	m_MapCoder->Reset();
}

bool CNetPacketParserXorMap::Pack(const unsigned char* pInData, int nInLen,
								  unsigned char* pOutData, int& nOutLen )
{
	//Ч�����
	ASSERT( nInLen >= TCP_PACKET_HEAD_SIZE );
	ASSERT (nOutLen >= (nInLen + 2 * DWORD_LEN) );
	ASSERT( nInLen <= (TCP_PACKET_HEAD_SIZE + TCP_PACKET_MAXSIZE) );

	nOutLen = nInLen;
	uint16 wEncryptSize	= nOutLen - (uint16)MESSAGE_CMD_LEN;

	// ��������
	uint16 wSnapCount		= 0;
	if ((wEncryptSize % DWORD_LEN) != 0)
	{
		wSnapCount = DWORD_LEN - wEncryptSize % DWORD_LEN;
		memset(pOutData+MESSAGE_INFO_LEN+wEncryptSize, 0, wSnapCount);
	}

	CMD_Head* pHead = (CMD_Head*) pOutData;

	// д����Ϣ�汾��
	pHead->CmdInfo.cbMessageVer	= TCP_PROTOCOL_VER;
	pHead->CmdInfo.wDataSize	= nOutLen;

	uint8 cbCheckCode = 0;
	uint32 dwPublicKey = 0;

	// ��������
	if (m_bEnableMapCodec)
	{
		m_MapCoder->Encode( pOutData+MESSAGE_INFO_LEN, nOutLen-MESSAGE_INFO_LEN, cbCheckCode);
	}

	// д����֤��
	pHead->CmdInfo.cbCheckCode	= ~cbCheckCode+1;

	if (m_bEnableXORCodec)
	{
		// ��ȡ�������
		if ( IsFirstPack() && !IsNeedPublicKey())
		{
			m_XorCoder->GenerateKey();
		}

		uint16* pwSeed		= (uint16*)(pOutData + MESSAGE_INFO_LEN);
		uint32* pdwXor		= (uint32*)(pOutData + MESSAGE_INFO_LEN);
		uint16 wEncrypCount	= (wEncryptSize + wSnapCount) / DWORD_LEN;

		// ��������
		dwPublicKey	= m_XorCoder->Encode(pwSeed, pdwXor, wEncrypCount );
	}

	// ��һ�η������������������Կ
	if ( IsFirstPack() && !IsNeedPublicKey())
	{
		WritePublicKey(pOutData, nOutLen, dwPublicKey);
	}

	return true;
}

bool CNetPacketParserXorMap::Unpack(const uint8* pInData, int nInLen, 
									uint8* pOutData, int& nOutLen )
{
	// Ч�����
	ASSERT(nInLen >= TCP_PACKET_HEAD_SIZE);
	ASSERT(( (CMD_Head*)pInData)->CmdInfo.wDataSize == nInLen);

	nOutLen = nInLen;

	uint16 wSnapCount = 0;
	if ( (nOutLen % DWORD_LEN) != 0)
	{
		wSnapCount = (uint16)DWORD_LEN - nOutLen % (uint16)DWORD_LEN;
		memset(pOutData+nOutLen, 0, wSnapCount);
	}

	// ��ȡ������Կ
	if (IsFirstUnpack() && IsNeedPublicKey())
	{
		if (ReadPublicKey(pOutData, nOutLen) == false)
		{
			goto FAILED;
		}
	}

	uint16* pwSeed		= (uint16*) (pOutData + MESSAGE_INFO_LEN);
	uint32* pdwXor		= (uint32*) (pOutData + MESSAGE_INFO_LEN);
	uint16 wEncrypCount	= (nOutLen + wSnapCount - (uint16)MESSAGE_INFO_LEN) / DWORD_LEN;

	// XOR����
	if (m_bEnableXORCodec)
	{
		m_XorCoder->Decode(pwSeed, pdwXor, wEncrypCount, wSnapCount, pOutData, nOutLen);
	}

	// ӳ������
	uint8 cbCheckCode = 0;
	if (m_bEnableMapCodec)
	{
		CMD_Head* pHead = (CMD_Head*)pOutData;

		cbCheckCode = pHead->CmdInfo.cbCheckCode;
		m_MapCoder->Decode( pOutData+MESSAGE_INFO_LEN, nOutLen-MESSAGE_INFO_LEN, cbCheckCode);

		// ���У����

		if (cbCheckCode != 0)
		{
			TRACE("���ʧ�ܣ�У�������[%d][%d]",
				cbCheckCode, pHead->CmdInfo.cbCheckCode );
			goto FAILED;
		}
	}

	return true;

FAILED:
	return false;
}

void CNetPacketParserXorMap::NeedPublicKey( const bool bNeed /*= true*/ )
{
	m_bNeedPublicKey = bNeed;
}

bool CNetPacketParserXorMap::WritePublicKey( uint8* pOutData, int& nOutLen, uint32 dwXorKey )
{
	uint8* pDataBuffer = pOutData + TCP_PACKET_HEAD_SIZE;
	CMD_Head* pHead = (CMD_Head*) pOutData;

	MoveMemory(pDataBuffer+DWORD_LEN, pDataBuffer, nOutLen);

	*(uint32*) pDataBuffer = dwXorKey;

	pHead->CmdInfo.wDataSize += DWORD_LEN;	
	nOutLen += DWORD_LEN;

	m_bFirstPack = false;

	return true;
}

bool CNetPacketParserXorMap::ReadPublicKey( uint8* pOutData, int& nOutLen )
{
	ASSERT(nOutLen >= (TCP_PACKET_HEAD_SIZE+DWORD_LEN));
	if (nOutLen < (TCP_PACKET_HEAD_SIZE+DWORD_LEN))
	{
		TRACE("���ݰ����ܳ��ȴ���[%d]",
			nOutLen );
		return false;
	}

	uint32 dwKey = *(uint32*) (pOutData + TCP_PACKET_HEAD_SIZE);
	m_XorCoder->SetPublicKey(dwKey, dwKey);

	MoveMemory(pOutData + TCP_PACKET_HEAD_SIZE,
		pOutData + TCP_PACKET_HEAD_SIZE + DWORD_LEN,
		nOutLen - TCP_PACKET_HEAD_SIZE - DWORD_LEN);

	nOutLen -= DWORD_LEN;
	CMD_Head* pHead = (CMD_Head*) pOutData;
	pHead->CmdInfo.wDataSize = nOutLen;

	m_bFirstUnpack = false;

	return true;
}
