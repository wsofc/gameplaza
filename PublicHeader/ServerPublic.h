/*
中心服务器主ID  0x1000


登陆服务器主ID  0x2000


游戏服务器主ID  0x3000


大厅服务器主ID  0x4000


*/

/*

REQ -- request 请求
ANW -- answer  应答

*/

#pragma once

#include "GlobalDef\GlobalDef.h"


#define	DATABASE_DEFAULT_PORT		1433
#define	LESS_CONNECT_COUNT			50							//最少连接
#define	PORT_DATABASE_SERVER		9050

#define	CENTER_SERVER_SERVICE_ID	100
#define	DATABASE_SERVER_SERVICE_ID	200


//数据库名字
const TCHAR	 g_szUserDB[]			= TEXT("QPUserDB");			//用户数据库
const TCHAR	g_szServerInfoDB[]	= TEXT("QPServerInfoDB");	//房间数据库
const TCHAR	g_szTreasureDB[]		= TEXT("QPTreasureDB");		//财富数据库
const TCHAR	g_szScoreDB[]			= TEXT("QPScoreDB");		//积分数据库
const TCHAR	g_szMatchDB[]			= TEXT("QPMatchDB");		//比赛数据库



#define	LOGON_SERVER_MAINTANENCE_MESSAGE	_T("系统正在维护,请您稍候重试登陆.谢谢您的关心.")


//服务状态
enum emServiceStatus
{
	SERVICE_STATUS_STOP,		//停止状态
	SERVICE_STATUS_SERVICE,		//服务状态
};

//数据库信息
struct tagDatabaseInfo
{
	tagDatabaseInfo()
	{
		memset(this,0,sizeof(*this));
	}
	WORD		wPort;				//数据库端口
	TCHAR		szAddress[16];		//数据库地址
	TCHAR		szUser[32];			//数据库用户
	TCHAR		szPwd[32];			//数据库密码
	TCHAR		szName[32];			//数据库名字
	TCHAR		szDoMain[50];		//数据库域名
	bool		bPriorityIP;		//优先IP
};


//数据库信息
struct tagDatabaseInfoEx
{
	tagDatabaseInfoEx()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wPort;				//数据库端口
	DWORD		dwAddress;			//数据库地址
	TCHAR		szUser[32];			//数据库用户
	TCHAR		szPwd[32];			//数据库密码
	TCHAR		szName[32];			//数据库名字
};


//游戏服务属性
struct tagGameServiceAttrib
{
	tagGameServiceAttrib()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wKindID;							//名称号码
	WORD		wChairCount;						//椅子数目
	BYTE		cbJoinInGame;						//游戏加入
	TCHAR		szDatabaseName[32];					//游戏库名
	TCHAR		szDescription[128];					//模块描述
	TCHAR		szKindName[KIND_LEN];				//类型名字
	TCHAR		szServerModuleName[MODULE_LEN];		//模块名字
	TCHAR		szClientModuleName[MODULE_LEN];		//模块名字
};

//游戏服务配置
struct tagGameServiceOption
{
	tagGameServiceOption()
	{
		memset(this,0,sizeof(*this));
	}

	//房间属性
	WORD							wKindID;							//类型标识
	WORD							wSortID;							//排序标识
	WORD							wNodeID;							//站点号码
	WORD							wServerID;							//房间号码
	WORD							wTableCount;						//桌子数目
	WORD							wServerPort;						//房间端口
	WORD							wRoomServiceType;					//房间类型
	WORD							wMaxConnect;						//最大连接
	DWORD							dwServerAddr;						//房间地址
	DWORD							dwVideoServer;						//视频地址
	DWORD							dwDatabaseAddr;						//数据地址
	TCHAR							szDatabaseName[SERVER_LEN];			//数据名字
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称

	TCHAR							szDatabaseDomain[50];				//数据域名
	TCHAR							szServerDomain[50];					//服务域名
	bool							m_bDatabasePriorityIP;				//
	bool							m_bServerPriorityIP;				
	bool							bMobileRoom;						// 是否是手机房间

	//积分限制
	WORD							wTaxRate;							//游戏税收
	LONG							lCellScore;							//单位积分
	LONGLONG						lLessScore;							//最低积分
	LONGLONG						lMaxScore;							//最高积分
	LONGLONG						lRestrictScore;						//限制积分

	//扩展配置
	BYTE							cbHideUserInfo;						//隐藏信息
	BYTE							cbUnLookOnTag;						//限制旁观
	BYTE							cbUnSameIPPlay;						//限制同IP

	LONG							lMatchDraw;
	BYTE							cbControlStart;

	tagDayMatchOption				DayMatchOption;
};

//报名类型
enum emSignUpType
{
	SIGN_UP_TYPE_ERROR		,
	SIGN_UP_TYPE_HAPPY_BEAN	,	//欢乐豆
	SIGN_UP_TYPE_MATCH_QUAN	,	//参赛券
};


//日志选项
enum emLogOption
{
	LOG_OPTION_LOGON		= 0x00000001,		//登陆
	LOG_OPTION_SYS_MESSAGE	= 0x00000002,		//系统消息
	LOG_OPTION_ROBOT		= 0x00000004,		//机器人
	LOG_OPTION_GAME_SERVER	= 0x00000008,		//游戏服务器
	LOG_OPTION_DAY_MATCH	= 0x00000010,
};


//比赛用户状态
enum emMatchUserState
{
	MATCH_USER_STATE_ERROR		,
	MATCH_USER_STATE_SIGN_UP	,	//报名
	MATCH_USER_STATE_GAME_NOW	,	//正在游戏
	MATCH_USER_STATE_EXIT		,	//退出
	MATCH_USER_STATE_FINISH		,	//完成
};





//-----------------------------------------------
//					the end
//-----------------------------------------------
