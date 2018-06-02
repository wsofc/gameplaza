

#pragma once



const TCHAR	g_szProductName[]	= TEXT("wan1979");					//产品名字
const TCHAR g_szSystemRegKey[]	= TEXT("wan1979");					//注册主键
const TCHAR g_szStationPage[]	= TEXT("www.wan1979.com");			//游戏主页
const TCHAR g_szCookieUrl[]		= TEXT("http://www.wan1979.com");		///记录地址

#define PLAZA_MAIN_WND_CLASS_NAME	TEXT("hhw1688.GamePlaza")			//窗口类名


const TCHAR g_szExitPlazaInfo[]		= TEXT("6168有您更精彩！！！");		//离开大厅提示语
const TCHAR g_szLogonWindowText[]	= TEXT("6168");					//登陆框窗口标题



#define	FILE_LOG_FOLDER				_T("Log")
#define	AUTO_RUN					_T("AUTO")
#define	CONFIG_FOLDER				_T("Config")


#define	INVALID_MATCH_GROUP_ID		0xFF
#define TIMER_REPEAT_TIMER			DWORD(-1)							//重复次数

#define	DEFAULT_TAX_RATE			5		//千分比

#define	_W_							10000

#define MAX_AREA_CNT				11     //百人游戏最大区域数量
#define MAX_CARD_TYPE_CNT			5	   //百人游戏最多牌型数量


//下注明细总节点
#define NAME_NT_BET_DETAIL				_T("BetDetail")
//下注各节点
#define NAME_NT_AREA_BET_S				_T("Areas")
#define NAME_NT_AREA_BET				_T("Area")
#define NAME_NT_AREA_BET_ID				_T("ID")
#define NAME_NT_AREA_BET_NAME			_T("Name")
#define NAME_NT_AREA_BET_MULTIPLE		_T("Odds")
#define NAME_NT_AREA_BET_GOLD			_T("Bet")
#define NAME_NT_AREA_BET_PAYOUT			_T("Payout")

//总投注、总派彩
#define NAME_NT_AREA_ALL_BET			_T("TotalBet")
#define NAME_NT_AREA_ALL_PAYOUT			_T("TotalPayout")

//牌区域节点
#define NAME_NT_CARD_RESULT				_T("GameResult")	
#define NAME_NT_CARD_S					_T("Areas")
#define NAME_NT_CARD					_T("Area")
#define NAME_NT_CARD_ID					_T("ID")
#define NAME_NT_CARD_TYPE				_T("Name")
#define NAME_NT_CARD_VALUE				_T("Result")

//服务器类型
enum emServerType
{
	SERVER_TYPE_ERROR	,
	SERVER_TYPE_CENTER	,	//中心服务器
	SERVER_TYPE_LOGON	,	//登陆服务器
	SERVER_TYPE_HALL	,	//大厅服务器
	SERVER_TYPE_GAME	,	//游戏服务器
	SERVER_TYPE_DATEBASE,	//数据库服务器
};

////网络类型
//enum emNetType
//{
//	NET_TYPE_ERROR		,
//	NET_TYPE_DIAN_XIN	,	//电信
//	NET_TYPE_WANG_TONG	,	//网通
//};

//网络地址类型
enum emAddrType
{
	ADDR_TYPE_ERROR		,
	ADDR_TYPE_DOMAIN	,	//域名
	ADDR_TYPE_IP		,	//IP
};

//玩家所处位置
enum emPlayerLocation
{
	PLAYER_LOCATION_ERROR,
	PLAYER_LOCATION_ROOM = 0x01,
	PLAYER_LOCATION_GAME = 0x02,
};

//会员等级
enum emMemberOrder
{
	MEMBER_ORDER_NORMAL	= 0,	//普通会员
	MEMBER_ORDER_VIP    = 10,	//VIP会员
};

//密码类型
enum emPwdType
{
	PWD_TYPE_ERROR	,
	PWD_TYPE_LOGON	= 0x01,	//登录密码
	PWD_TYPE_BANK	= 0x02,	//银行密码
};

//比赛奖品
struct tagMatchPrize
{
	tagMatchPrize()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wExperienceCount;					//经验数目
	WORD		wMedalCount;						//奖牌数目
	LONG		lGoldCount;							//金币数目
};

//局号
struct tagSetNumber
{
	tagSetNumber(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	//游戏场所定位
	WORD	wKindID;
	WORD	wServerID;
	WORD	wTableID;

	//启动次数,如果一个房间重新启动多次
	//为保证局号唯一
	WORD	wGameServerStartTimes;

	//年月日定位
	WORD	wYear;
	WORD	wMonth;
	WORD	wDay;
	//系统时间毫秒数
	LONGLONG lMilliseconds;
	
	//序号
	WORD	wIndex;
};

//下注明细
struct tagBetDetail
{
	tagBetDetail(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	BYTE		cbAreaCnt;							//区域数量
	TCHAR		szAreaName[MAX_AREA_CNT][32];		//各区域名称
	TCHAR		szAreaMultiple[MAX_AREA_CNT][32];	//各区域倍率
	LONGLONG	lAreaBet[MAX_AREA_CNT];				//各区域下注额
	LONGLONG	lAreaPayout[MAX_AREA_CNT];			//各区域派彩额

};

//本局开牌牌型及牌值
struct tagCardResult
{
	tagCardResult(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	BYTE		cbCardTypeCnt;							//比牌数量
	TCHAR		szAreaCardType[MAX_CARD_TYPE_CNT][32];	//各区域牌型
	TCHAR		szAreaCardValue[MAX_CARD_TYPE_CNT][32];	//各区域牌值
	LONGLONG	lTotalBet;								//本局所有玩家下注合计
	LONGLONG	lTotalPayout;							//本局所有玩家派彩合计
};

//保证Sub ID唯一
#define	GAME_SUB_ID_SERVER_MIN			1000
#define	GAME_SUB_ID_CLIENT_MIN			1
#define	GAME_SUB_ID_CLIENT_MAX			999



//-----------------------------------------------
//					the end
//-----------------------------------------------
