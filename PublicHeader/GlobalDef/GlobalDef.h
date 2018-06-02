
#pragma once

#include "../SafeDelete.h"
#include "GlobalSocket.h"
#include "GlobalIpc.h"

//////////////////////////////////////////////////////////////////////////
//公共定义
#define MACHINE_PC						0							// 电脑
#define MACHINE_IOS						1							// 苹果
#define MACHINE_ANDROID			2							// 安卓

#define MAX_CHAIR							100							//最大椅子
#define MAX_CHAIR_NORMAL			8								//最大人数

#define MAX_ROBOT						550								//单个房间最大机器数量
#define MAX_CHAT_LEN					200								//聊天长度
#define LIMIT_CHAT_TIMES				1200								//限时聊天
#define MAX_SYSMESSAGE_LEN		1024								//系统消息长度

/////////////////////////////////////////////////////////////////////////////////////////
//宏定义

//端口定义
#define PORT_VIDEO_SERVER					7600								//视频服务器
#define PORT_LOGON_SERVER				9001								//登陆服务器
#define PORT_CENTER_SERVER				9010								//中心服务器
#define PORT_HALL_SERVER_BASE			9020								//大厅服务器


//////////////////////////////////////////////////////////////////////////

//长度宏定义
#define TYPE_LEN							32									//种类长度
#define KIND_LEN						32									//类型长度
#define STATION_LEN					32									//站点长度
#define SERVER_LEN						32									//房间长度
#define MODULE_LEN					32									//进程长度
#define SET_NUMBER_LEN			64									//局号长度
#define RESULT_CARD_TYPE			128								//结算牌型描述长度

//性别定义
#define GENDER_NULL						0									//未知性别
#define GENDER_BOY						1									//男性性别
#define GENDER_GIRL						2									//女性性别

//房间服务类型
enum emRoomServiceType
{
	ROOM_SERVICE_TYPE_ERROR		= 0x0000,			//无效类型	
	ROOM_SERVICE_TYPE_SCORE		= 0x0001,			//点值类型	
	ROOM_SERVICE_TYPE_GOLD		= 0x0002,			//金币类型
	ROOM_SERVICE_TYPE_DAY_MATCH	= 0x0004,		//每日赛
};

//游戏类型结构
struct tagGameType
{
	tagGameType()
	{
		memset(this,0,sizeof(*this));
	}
	WORD			wSortID;									//排序号码
	WORD			wTypeID;									//种类号码
	TCHAR			szTypeName[TYPE_LEN];			//种类名字
};

//游戏名称结构
struct tagGameKind
{
	tagGameKind()
	{
		memset(this,0,sizeof(*this));
	}
	WORD			wSortID;							//排序号码
	WORD			wTypeID;							//类型号码
	WORD			wKindID;							//名称号码
	DWORD			dwMaxVersion;						//最新版本
	DWORD			dwOnlineCount;						//在线数目
	TCHAR			szKindName[KIND_LEN];				//游戏名字
	TCHAR			szClientModuleName[MODULE_LEN];		//客户端模块名字
};

//游戏站点结构
struct tagGameStation
{
	tagGameStation()
	{
		memset(this,0,sizeof(*this));
	}

	WORD			wSortID;							//排序号码
	WORD			wKindID;							//名称号码
	WORD			wJoinID;							//挂接号码
	WORD			wStationID;							//站点号码
	TCHAR			szStationName[STATION_LEN];			//站点名称
};

//每日赛
struct tagDayMatchOption
{
	tagDayMatchOption()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwMatchID;				//比赛房间ID

	BYTE		cbMatchStartTime;		//开始时间
	BYTE		cbMatchEndTime;			//结束时间
	WORD		wSetCnt;				//局数设置	

	DWORD		dwSignUpHappyBeanCnt;	//扣除欢乐豆数量
	DWORD		dwSignUpMatchQuanCnt;	//扣除参赛券数量
};

//游戏房间列表结构
struct tagGameServer
{
	tagGameServer()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wSortID;							//排序号码
	WORD		wKindID;							//名称号码
	WORD		wServerID;							//房间号码
	WORD		wStationID;							//站点号码
	WORD		wServerPort;						//房间端口
	WORD		wRoomServiceType;					//服务类型
	LONG		lCellScore;			
	DWORD		dwServerAddr;						//房间地址
	DWORD		dwOnlineCount;						//在线人数
	TCHAR		szServerName[SERVER_LEN];			//房间名称

	tagDayMatchOption DayMatchOption;				//比赛选项

	//积分限制
	WORD		wTaxRate;							//游戏税收
	LONGLONG	lLessScore;							//最低积分
	LONGLONG	lMaxScore;							//最高积分
	LONGLONG	lRestrictScore;						//限制积分
	bool				bMobileRoom;					// 手机房间
};

//登陆服务器
struct tagLogonServer
{
	tagLogonServer(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	DWORD			dwStuff;
};

//大厅服务器
struct tagHallServer
{
	tagHallServer(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	WORD			wServerID;					//ID
	WORD			wServerPort;				//端口
	DWORD			dwServerAddr;				//地址
	DWORD			dwOnlineCount;				//在线人数
};


//游戏级别结构
struct tagLevelItem
{
	LONG								lLevelScore;						//级别积分
	WCHAR								szLevelName[16];					//级别描述
};
//////////////////////////////////////////////////////////////////////////

//用户状态定义
#define US_NULL							0x00							//没有状态
#define US_FREE							0x01							//站立状态
#define US_SIT								0x02							//坐下状态
#define US_READY						0x03							//同意状态
#define US_LOOKON					0x04							//旁观状态
#define US_PLAY							0x05							//游戏状态
#define US_OFFLINE						0x06							//断线状态

//长度宏定义
#define NAME_LEN						32								//名字长度
#define PWD_LEN							33								//密码长度
#define EMAIL_LEN						32								//邮箱长度
#define GROUP_LEN						32								//社团长度
#define COMPUTER_ID_LEN			33								//机器序列
#define UNDER_WRITE_LEN			32								//个性签名
#define PHONE_LEN						15								//手机号
#define IDCARD_LEN					20								//身份证
#define SMS_LEN							6								//短信认证码
#define INFO_DESC_LEN				20								//个人信息描述
#define PROPS_NAME					20								//道具名称

//用户积分信息
struct tagUserScore
{
	tagUserScore()
	{
		memset(this,0,sizeof(*this));
	}
	LONGLONG							lScore;								//用户分数
	LONGLONG							lGameGold;						//游戏金币
	LONGLONG							lBankScore;						//存储金币
	LONG								lWinCount;							//胜利盘数
	LONG								lLostCount;							//失败盘数
	LONG								lDrawCount;							//和局盘数
	LONG								lFleeCount;							//断线数目
	DWORD							dwExperience;						//用户经验
};

//用户状态信息
struct tagUserStatus
{
	WORD								wTableID;							//桌子号码
	WORD								wChairID;							//椅子位置
	BYTE								cbUserStatus;						//用户状态
};

//用户基本信息结构
struct tagUserInfoHead
{
	//用户属性
	WORD									wFaceID;							//头像索引
	DWORD								dwUserID;							//用户 I D
	DWORD								dwGameID;						//游戏 I D
	DWORD								dwGroupID;						//社团索引
	DWORD								dwUserRight;					//用户等级
	LONGLONG							lCharm;								//用户魅力
	DWORD								dwMasterRight;					//管理权限

	//用户属性
	BYTE								cbGender;						    	//用户性别
	BYTE								cbMemberOrder;					//会员等级
	BYTE								cbMasterOrder;						//管理等级
	BOOL							bRobot;								//机器人

	//用户状态
	WORD								wTableID;							//桌子号码
	WORD								wChairID;							//椅子位置
	BYTE								cbUserStatus;						//用户状态

	//用户积分
	tagUserScore						UserScoreInfo;						//积分信息

	//扩展信息
	//LONG								lInsureScore;						//消费金币
	//LONG								lGameGold;						//游戏金币
	DWORD								dwCustomFaceVer;			//上传头像
};

//用户基本信息结构
struct tagUserInfoHeadMatch
{
	tagUserInfoHeadMatch()
	{
		memset(this,0,sizeof(*this));
	}	
	//用户属性
	WORD					wFaceID;								//头像索引
	DWORD				dwUserID;								//用户 I D
	DWORD				dwGameID;							//游戏 I D
	DWORD				dwGroupID;							//社团索引
	DWORD				dwUserRight;						//用户等级
	LONG					lCharm;									//用户魅力
	DWORD				dwMasterRight;						//管理权限

	//用户属性
	BYTE				cbGender;									//用户性别
	BYTE				cbMemberOrder;						//会员等级
	BYTE				cbMasterOrder;							//管理等级

	//用户状态
	WORD				wTableID;							//桌子号码
	WORD				wChairID;							//椅子位置
	BYTE				cbUserStatus;						//用户状态

	//用户积分
	tagUserScore		UserScoreInfo;						//积分信息

	//扩展信息
	//LONG				lInsureScore;						//消费金币
	//LONG				lGameGold;							//游戏金币
	DWORD				dwCustomFaceVer;					//上传头像

	//比赛信息
	BYTE				cbHaveSignUp;						
	WORD				wSignUpTimes;	
	WORD				wSetCnt;	
};


//用户信息结构
struct tagUserData
{
	tagUserData()
	{
		memset(this,0,sizeof(*this));
	}
	//用户属性
	WORD					wFaceID;									//头像索引
	DWORD				dwCustomFaceVer;					//上传头像
	DWORD				dwUserID;									//用户 I D
	DWORD				dwGroupID;								//社团索引
	DWORD				dwGameID;								//游戏 I D
	DWORD				dwUserRight;							//用户等级
	LONGLONG			lCharm;										//用户魅力
	DWORD				dwMasterRight;							//管理权限
	TCHAR					szNickname[NAME_LEN];			//昵称
	TCHAR					szGroupName[GROUP_LEN];				//社团名字
	TCHAR					szUnderWrite[UNDER_WRITE_LEN];		//个性签名

	//用户属性
	BYTE				cbGender;									//用户性别
	BYTE				cbMemberOrder;						//会员等级
	BYTE				cbMasterOrder;							//管理等级
	BOOL			bRobot;									//机器人

	//用户积分
	LONGLONG			lInsureScore;						//消费金币
	LONGLONG			lGameGold;						//游戏金币
	LONGLONG			lScore;								//用户分数
	LONG					lWinCount;						//胜利盘数
	LONG					lLostCount;						//失败盘数
	LONG					lDrawCount;						//和局盘数
	LONG					lFleeCount;						//断线数目
	DWORD				dwExperience;					//用户经验

	//用户状态
	WORD				wTableID;							//桌子号码
	WORD				wChairID;							//椅子位置
	BYTE				cbUserStatus;						//用户状态

	//其他信息
	BYTE				cbCompanion;						//用户关系

	//比赛
	WORD				wSignUpTimes;						//报名次数			
	WORD				wSetCnt;							//局数				
	bool				cbHaveSignUp;						//是否已经报名
};

//////////////////////////////////////////////////////////////////////////

//配置缓冲结构
struct tagOptionBuffer
{
	BYTE								cbBufferLen;						//数据长度
	BYTE								cbOptionBuf[32];					//设置缓冲
};

//////////////////////////////////////////////////////////////////////////




//-----------------------------------------------
//					the end
//-----------------------------------------------
