/**********************************************************************
* Copyright (C) 2008 - �������� - All Rights Reserved
*
* �ļ�����:		MapCoding.h
* ժ    Ҫ:		ӳ�����ܽ�����
*     
* ��    ��:		zcc, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#ifndef __SocketModule_MapCoding_H__
#define __SocketModule_MapCoding_H__


#include "NetModule.h"


//////////////////////////////////////////////////////////////////////////
/// ӳ�����ܽ�����
//////////////////////////////////////////////////////////////////////////
class CMapCoding
{
public:
	CMapCoding(void);
	~CMapCoding(void);

public:
	/// ���ü��ܽ�����
	void Reset();

public:
	/// ��������
	bool Encode(uint8* arrDataBuffer, 
		const int wDataSize, 
		uint8& cbVerifyCode);

	/// ��������
	bool Decode(uint8* arrDataBuffer, 
		const int wDataSize, 
		uint8& cbVerifyCode);

protected:
	/// ���ܵ�������
	uint8 EncodeByte(const uint8 cbData);

	/// ���ܵ�������
	uint8 DecodeByte(const uint8 cbData);

private:
	uint8					m_cbEncodeRound;			///< �����ֽ�ӳ��
	uint8					m_cbDecodeRound;			///< �����ֽ�ӳ��
};


#endif
