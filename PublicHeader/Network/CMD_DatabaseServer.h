/*

REQ -- request 请求
ANW -- answer  应答

*/
#pragma once


//-----------------------------------------------
//服务器管理
//

#define MAIN_DS_SERVER_MANAGER			0x5000				//服务器管理

#define SUB_DS_REGISTER					0x5001				//注册
#define SUB_DS_UNREGISTER				0x5002				//注销

struct tagDS_Register
{
	tagDS_Register()
	{
		memset(this,0,sizeof(*this));
	}
	
	emServerType	ServerType;		//服务器类型
	WORD			wServerID;		//服务器ID
};



//-----------------------------------------------
//服务器管理
//

#define MAIN_DS_GS							0x5100		//游戏服务器

#define SUB_DS_GS_WRITE_SCORE				0x5101		//写分
#define SUB_DS_GS_WRITE_RECORD_BET			0x5103		//下注记录
#define SUB_DS_GS_WRITE_RECORD_RESULT		0x5105		//结果记录
#define SUB_DS_GS_WRITE_ROOM_STATUS			0x5107		//房间状态
#define SUB_DS_GS_WRITE_RECORD_BET_DETAIL	0x5109		//下注记录明细
#define	SUB_DS_GS_WRITE_RECORD_CARD_RESULT  0x5111		//开牌牌值及牌值记录 
#define SUB_DS_GS_WRITE_SETNUMBER			0x5113		//本局局号记录

struct tagDS_GS_WriteScore
{
	tagDS_GS_WriteScore()
	{
		memset(this,0,sizeof(*this));
	}

	WORD				wKindID;
	WORD				wServerID;
	WORD				wRoomServiceType;

	//用户属性
	DWORD				dwUserID;					//用户地址
	DWORD				dwClientIP;					//连接地址
	DWORD				dwPlayTimeCount;			//游戏时间
	DWORD				dwOnlineTimeCount;			//在线时间

	//积分信息
	LONG				lTax;						//游戏税收
	tagUserScore		ScoreModifyInfo;			//修改积分
};


//下注记录
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

//下注流水记录
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

//下注记录明细
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

//开牌牌型及牌值记录
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

//结算记录
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

//房间状态
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

	BYTE				cbGameScene;				//游戏场景		
	TCHAR				szGameSceneComment[64];
};


//每局局号
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
