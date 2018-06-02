/*

REQ -- request ����
ANW -- answer  Ӧ��

*/
#pragma once


//-----------------------------------------------
//����������
//

#define MAIN_DS_SERVER_MANAGER			0x5000				//����������

#define SUB_DS_REGISTER					0x5001				//ע��
#define SUB_DS_UNREGISTER				0x5002				//ע��

struct tagDS_Register
{
	tagDS_Register()
	{
		memset(this,0,sizeof(*this));
	}
	
	emServerType	ServerType;		//����������
	WORD			wServerID;		//������ID
};



//-----------------------------------------------
//����������
//

#define MAIN_DS_GS							0x5100		//��Ϸ������

#define SUB_DS_GS_WRITE_SCORE				0x5101		//д��
#define SUB_DS_GS_WRITE_RECORD_BET			0x5103		//��ע��¼
#define SUB_DS_GS_WRITE_RECORD_RESULT		0x5105		//�����¼
#define SUB_DS_GS_WRITE_ROOM_STATUS			0x5107		//����״̬
#define SUB_DS_GS_WRITE_RECORD_BET_DETAIL	0x5109		//��ע��¼��ϸ
#define	SUB_DS_GS_WRITE_RECORD_CARD_RESULT  0x5111		//������ֵ����ֵ��¼ 
#define SUB_DS_GS_WRITE_SETNUMBER			0x5113		//���־ֺż�¼

struct tagDS_GS_WriteScore
{
	tagDS_GS_WriteScore()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;

	//�û�����
	DWORD				dwUserID;					//�û���ַ
	DWORD				dwClientIP;					//���ӵ�ַ
	DWORD				dwPlayTimeCount;			//��Ϸʱ��
	DWORD				dwOnlineTimeCount;			//����ʱ��

	//������Ϣ
	LONG				lTax;						//��Ϸ˰��
	tagUserScore		ScoreModifyInfo;			//�޸Ļ���
};


//��ע��¼
struct tagDS_GS_WriteRecordBet
{
	tagDS_GS_WriteRecordBet()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;
	WORD				wTableID;

	TCHAR				szServerName[SERVER_LEN];
	TCHAR				szSetNumber[SET_NUMBER_LEN];

	DWORD				dwUserID;	
	LONGLONG			lStartGold;
	LONGLONG			lBet;
	LONGLONG			lBankGold;
};

//��ע��ˮ��¼
struct tagDS_GS_WriteRecordBetOnWater
{
	tagDS_GS_WriteRecordBetOnWater()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wRoomServiceType;
	TCHAR				szSetNumber[SET_NUMBER_LEN];
	DWORD				dwUserID;	
	TCHAR				szAreaName[32];
	LONGLONG			lBet;
	tagBetDetail		BetDetail;

};

//��ע��¼��ϸ
struct tagDS_GS_WriteRecordBetDetail
{
	tagDS_GS_WriteRecordBetDetail()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;

	TCHAR				szServerName[SERVER_LEN];
	TCHAR				szSetNumber[SET_NUMBER_LEN];

	DWORD				dwUserID;	
	tagBetDetail		BetDetail;
};

//�������ͼ���ֵ��¼
struct tagDS_GS_WriteRecordCardResult
{
	tagDS_GS_WriteRecordCardResult()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;

	TCHAR				szServerName[SERVER_LEN];
	TCHAR				szSetNumber[SET_NUMBER_LEN];

	tagCardResult		CardResult;
};

//�����¼
struct tagDS_GS_WriteRecordResult
{
	tagDS_GS_WriteRecordResult()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;
	WORD				wTableID;

	TCHAR				szServerName[SERVER_LEN];
	TCHAR				szSetNumber[SET_NUMBER_LEN];

	DWORD				dwUserID;		
	LONGLONG			lStartGold;
	LONGLONG			lBet;
	LONGLONG			lWinLoseGold;
	LONGLONG			lWinLoseGoldExcludeTax;
	LONGLONG			lTax;
	LONGLONG			lBalanceGold;
	LONGLONG			lValidBet;
	LONGLONG			lPeaceBet;
	LONGLONG			lBankGold;
};

//����״̬
struct tagDS_GS_WriteRoomStatus
{
	tagDS_GS_WriteRoomStatus()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;

	TCHAR				szServerName[SERVER_LEN];
	TCHAR				szSetNumber[SET_NUMBER_LEN];

	BYTE				cbGameScene;				//��Ϸ����		
	TCHAR				szGameSceneComment[64];
};


//ÿ�־ֺ�
struct tagDS_GS_WriteSetNumber
{
	tagDS_GS_WriteSetNumber()
	{
		memset(this,0,sizeof(*this));
	}

	TCHAR				szSetNumber[SET_NUMBER_LEN];

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;

	WORD				wTableID;
	DWORD				dwBankerUserID;
};
//-----------------------------------------------
//					the end
//-----------------------------------------------
