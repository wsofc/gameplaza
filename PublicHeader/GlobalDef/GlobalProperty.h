#pragma once


//-----------------------------------------------
//-----------------------------------------------
//鲜花定义
#define FLOWER_1					0
#define FLOWER_2					1
#define FLOWER_3					2
#define FLOWER_4					3
#define FLOWER_5					4
#define FLOWER_6					5
#define FLOWER_7					6
#define FLOWER_8					7
#define FLOWER_9					8
#define FLOWER_10					9
#define FLOWER_11					10

const BYTE g_FlowerTypeList[]=
{
	FLOWER_1	,
	FLOWER_2	,
	FLOWER_3	,
	FLOWER_4	,
	FLOWER_5	,
	FLOWER_6	,
	FLOWER_7	,
	FLOWER_8	,
	FLOWER_9	,
	FLOWER_10	,
	FLOWER_11	,
};

//鲜花数目
#define FLOWER_COUNT		(sizeof(g_FlowerTypeList)/sizeof(g_FlowerTypeList[0]))//礼物数目

//////////////////////////////////////////////////////////////////////////
//宏定义
#define CHARM_EXCHANGE_RATE			50									//兑换比率
#define MAX_FLOWER_COUNT				100								//最大数目

//鲜花结构
struct tagGiftInfo
{
	TCHAR		szName[64];							//鲜花名称
	TCHAR		szAction[128];						//动作描述
	TCHAR		szQuantifier1[20];					//数量词
	TCHAR		szResult[128];						//结果描述
	TCHAR		szQuantifier2[6];					//数量词
	TCHAR		szCharmResult[128];				//结果描述
};

//鲜花定义
tagGiftInfo const g_FlowerDescribe[FLOWER_COUNT]=
{
	{TEXT("掌声"),TEXT(" 深情地赠送 "),TEXT(" 掌声1次"),TEXT(",恭喜 "),TEXT("次 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("香吻"),TEXT(" 深情地赠送 "),TEXT(" 香吻1个"),TEXT(",恭喜 "),TEXT("个 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("鲜花"),TEXT(" 深情地赠送 "),TEXT(" 鲜花1朵"),TEXT(",恭喜 "),TEXT("朵 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("啤酒"),TEXT(" 深情地赠送 "),TEXT(" 啤酒1杯"),TEXT(",恭喜 "),TEXT("杯 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("香烟"),TEXT(" 深情地赠送 "),TEXT(" 香烟1包"),TEXT(",恭喜 "),TEXT("包 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("钻戒"),TEXT(" 深情地赠送 "),TEXT(" 钻戒1枚"),TEXT(",恭喜 "),TEXT("枚 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("轿车"),TEXT(" 深情地赠送 "),TEXT(" 轿车1辆"),TEXT(",恭喜 "),TEXT("辆 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("别墅"),TEXT(" 深情地赠送 "),TEXT(" 别墅1座"),TEXT(",恭喜 "),TEXT("座 "),TEXT(" 魅力值增加 %ld 点!")},
	{TEXT("臭蛋"),TEXT(" 狠狠地砸了 "),TEXT(" 臭蛋1个"),TEXT(",抱歉 "),TEXT("个 "),TEXT(" 魅力值减少 %ld 点!")},
	{TEXT("砖头"),TEXT(" 狠狠地砸了 "),TEXT(" 砖头1块"),TEXT(",抱歉 "),TEXT("块 "),TEXT(" 魅力值减少 %ld 点!")},
	{TEXT("炸弹"),TEXT(" 狠狠地扔了 "),TEXT(" 炸弹1枚"),TEXT(",抱歉 "),TEXT("枚 "),TEXT(" 魅力值减少 %ld 点!")}
};


 
 
//道具定义
#define MAX_PROPERTY_COUNT				100									//最大数目
#define BUGLE_MAX_CHAR						120									//喇叭个数
#define BULESSING_MAX_CHAR				100									//祝福个数
#define PROPERTY_COUNT				10									//数量
 

//道具信息结构
struct tagPropsInfo
{
	tagPropsInfo()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wPropsID;							//道具ID
	TCHAR				szName[20];						// 道具名称
	LONG				lPrice;								//鲜花价格
	LONG				lVIPPrice;							//会员价格
	WORD				wType;								// 道具类型
	LONGLONG		lSrcCharm;						//赠送魅力
	LONGLONG		lRcvCharm;						//接受魅力
	TCHAR				szDescribe[200];				// 道具说明
	LONG				lAvailableTime;					// 有效时长
	bool					bEnabled;							// 是否可用
};



//-----------------------------------------------
//					the end
//-----------------------------------------------