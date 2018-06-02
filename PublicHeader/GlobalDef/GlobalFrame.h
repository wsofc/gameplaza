#ifndef GLOBAL_FRAME_HEAD_FILE
#define GLOBAL_FRAME_HEAD_FILE

#pragma once

#include "GlobalDef.h"
#include "GlobalProperty.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//游戏状态
#define GAME_SCENE_FREE								0							//空闲状态
#define GAME_SCENE_PLAYING						100							//游戏状态

//////////////////////////////////////////////////////////////////////////
//IPC 网络事件

#define IPC_MAIN_SOCKET					1								//网络消息

#define IPC_SUB_SOCKET_SEND				1								//网络发送
#define IPC_SUB_SOCKET_RECV				2								//网络接收

//IPC 网络包结构
struct IPC_SocketPackage
{
	CMD_Command							Command;						//命令信息
	BYTE								cbBuffer[TCP_PACKET_BUFFER_SIZE];		//数据缓冲
};

//////////////////////////////////////////////////////////////////////////
//IPC 配置信息

#define IPC_MAIN_CONFIG					2								//配置信息

#define IPC_SUB_SERVER_INFO				1								//房间信息
#define IPC_SUB_COLUMN_INFO				2								//列表信息

//游戏信息
struct IPC_GF_ServerInfo
{
	DWORD		dwUserID;						//用户 I D
	WORD		wTableID;						//桌子号码
	WORD		wChairID;						//椅子号码
	WORD		wKindID;						//类型标识
	WORD		wServerID;						//房间标识
	WORD		wRoomServiceType;				//游戏类型
	WORD		wChairCount;					//椅子数目
	BYTE		cbHideUserInfo;					//隐藏信息
	DWORD		dwVideoAddr;					//视频地址
	TCHAR		szKindName[KIND_LEN];			//类型名字
	TCHAR		szServerName[SERVER_LEN];		//房间名称
};

//////////////////////////////////////////////////////////////////////////
//IPC 用户信息

#define IPC_MAIN_USER					3								//用户信息

#define IPC_SUB_USER_CONFIG				0								//用户配置
#define IPC_SUB_USER_COME				1								//用户信息
#define IPC_SUB_USER_STATUS				2								//用户状态
#define IPC_SUB_USER_SCORE				3								//用户积分
#define IPC_SUB_GAME_START				4								//游戏开始
#define IPC_SUB_GAME_FINISH				5								//游戏结束
#define IPC_SUB_UPDATE_FACE				6								//更新头像
#define IPC_SUB_MEMBERORDER				7								//更新头像

#define IPC_SUB_USER_PWD				8								//用户信息


//用户状态
struct IPC_UserStatus
{
	DWORD								dwUserID;						//用户 I D
	WORD								wNetDelay;						//网络延时
	BYTE								cbUserStatus;					//用户状态
};

//用户分数
struct IPC_UserScore
{
	LONG								lCharm;							//用户魅力
	DWORD								dwUserID;						//用户 I D
	tagUserScore						UserScore;						//用户积分
};

//会员等级
struct IPC_MemberOrder
{
	BYTE								cbMember;					//会员等级
	DWORD								dwUserID;						//用户 I D
};

//用户分数
struct IPC_UpdateFace
{
	DWORD								dwCustomFace;					//用户 I D
};

//用户状态
struct IPC_UserPwd
{
	DWORD								dwUserID;						//用户 I D
	TCHAR								szPwd[PWD_LEN];					//密码
};

//用户扩展信息
struct tagFrameUserInfoEx
{
	tagFrameUserInfoEx()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD	dwUserID;
	TCHAR	szAccount[NAME_LEN];
	TCHAR	szMD5Pwd[PWD_LEN];
};
//////////////////////////////////////////////////////////////////////////
//IPC 控制信息

#define IPC_MAIN_CONCTROL				4								//控制信息

#define IPC_SUB_START_FINISH			1								//启动完成
#define IPC_SUB_CLOSE_FRAME				2								//关闭框架
#define IPC_SUB_JOIN_IN_GAME			3								//加入游戏
#define IPC_SUB_CLOSE_STATUSWND			4								//关闭状态提示

#define IPC_SUB_SWITCH_TABLE			10								//换桌

//加入游戏
struct IPC_JoinInGame
{
	WORD								wTableID;						//桌子号码
	WORD								wChairID;						//椅子号码
};

//////////////////////////////////////////////////////////////////////////
//IPC 银行信息

#define IPC_MAIN_BANK					5								//银行信息

#define IPC_SUB_BANK_RESULT		1								//操作结果

//操作结果
struct IPC_BankResult
{
	BYTE                                cbGameAction;                   //游戏操作
	LONG								lResultType;						//结果类型
	LONG								lErrorCode;						//错误代码
	TCHAR								szErrorDescribe[128];			//错误消息
};

//////////////////////////////////////////////////////////////////////////
//网络命令码

#define MDM_GF_GAME						100								//游戏消息
#define MDM_GF_FRAME						101								//框架消息
#define	MDM_GF_PRESENT					102								//礼物消息
#define	MDM_GF_BANK						103								//银行消息
#define MDM_GF_MAIL						104								//邮件消息

#define SUB_GF_INFO								1								//游戏信息
#define SUB_GF_USER_READY					2								//用户同意
#define SUB_GF_LOOKON_CONTROL		3								//旁观控制
#define SUB_GF_KICK_TABLE_USER			4								//踢走用户

#define SUB_GF_OPTION							100								//游戏配置
#define SUB_GF_SCENE							101								//场景信息

#define SUB_GF_USER_CHAT					200								//用户聊天
#define SUB_GF_BET_RECORD_TIME			201								//下注记录时间请求

#define SUB_GF_MESSAGE						300								//系统消息

//#define SUB_GF_GIFT								400								//赠送消息

#define SUB_GF_BANK_SAVE					450							//银行存储
#define SUB_GF_BANK_WITHDRAW			451							//银行提取
#define SUB_GF_BANK_TRANSFER            452                          //银行转帐
#define SUB_GF_BANK_SECURITY				453                          //密码安全
#define SUB_GF_BANK_TRANSFERLOG         454                      //转帐记录查询
#define SUB_GF_TRANSFERLOG_RESULT		455						//转帐记录结果
#define SUB_GF_BANK_RESULT				456								//银行结果
#define SUB_GF_BANK_INFO				457								//银行信息
#define SUB_GF_BANK_GETINFO			458								//请求银行信息
#define SUB_GF_MAIL_RESULT				482								//邮件结果


#define SUB_GF_EXCHANGE_CHARM				502								//兑换魅力
#define SUB_GF_EXCHANGE_CHARM_OK			503								//兑换魅力成功
#define SUB_GF_EXCHANGE_CHARM_ERROR	504								//兑换魅力失败

#define SUB_GF_BUYPROPS_REQ				510								//购买道具 请求
#define SUB_GF_BUYPROPS_OK				511								//购买道具 成功
#define SUB_GF_BUYPROPS_ERROR			512								//购买道具 失败

#define SUB_GF_PROPERTY						550								//道具消息
#define SUB_GF_PROPERTY_RESULT			551								//道具结果
#define SUB_GF_RESIDUAL_PROPERTY		552								//剩余道具
#define SUB_GF_PROP_ATTRIBUTE			553								//道具属性
#define SUB_GF_PROP_BUGLE					554								//喇叭道具

//新增,只能加在后面
#define	SUB_GF_DAY_MATCH_WAIT_FOR_SIT	601					//等待分配座位
#define	SUB_GF_DAY_MATCH_SIT_FINISH		603					//分配结束
#define	SUB_GF_DAY_MATCH_SET_START		607					//一局开始
#define	SUB_GF_DAY_MATCH_SET_END		608					//一局结束
#define	SUB_GF_DAY_MATCH_WILL_CLOSE		609					//每日赛将结束

#define	SUB_GF_MATCH_CONTINUE			611					//继续						
#define	SUB_GF_MATCH_EXIT				613					//退出						
#define	SUB_GF_MATCH_SWITCH_TABLE		615					//换桌						

#define		SUB_GF_LOADPROPS				620					// 加载道具		


//版本信息
struct CMD_GF_Info
{
	BYTE								bAllowLookon;					//旁观标志
};

//游戏配置
struct CMD_GF_Option
{
	BYTE								bGameStatus;					//游戏状态
	BYTE								bAllowLookon;					//允许旁观
};

//旁观控制
struct CMD_GF_LookonControl
{
	DWORD								dwUserID;						//用户标识
	BYTE								bAllowLookon;					//允许旁观
};

//踢走用户
struct CMD_GF_KickTableUser
{
	DWORD								dwUserID;						//用户 I D
};

//聊天结构
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//信息长度
	COLORREF						crFontColor;					//信息颜色
	DWORD							dwSendUserID;					//发送用户
	DWORD							dwTargetUserID;					//目标用户
	TCHAR								szChatMessage[MAX_CHAT_LEN];	//聊天信息
};

//下注记录时间
struct CMD_GF_GetServerTimes
{
	LONGLONG						   lServerMilliseconds;				//服务器返回的总微秒数
	TCHAR							   szKey[37];						//GUID key
	WORD							   wButtonFlag;						//按钮标志
};
//////////////////////////////////////////////////////////////////////////

//消息类型
#define SMT_INFO						0x0001							//信息消息
#define SMT_EJECT						0x0002							//弹出消息
#define SMT_GLOBAL						0x0004							//全局消息
#define SMT_CLOSE_GAME					0x1000							//关闭游戏

//消息数据包
struct CMD_GF_Message
{
	WORD								wMessageType;					//消息类型
	WORD								wMessageLength;					//消息长度
	TCHAR								szContent[MAX_SYSMESSAGE_LEN];				//消息内容
};

//等待坐下
struct CMD_GF_DayMatch_WaitForSit
{
	CMD_GF_DayMatch_WaitForSit()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wSecondCnt;			//秒数		
	BYTE	cbPlayerLocation;	//玩家所处位置
};

//坐下完成
struct CMD_GF_DayMatch_SitFinish
{
	CMD_GF_DayMatch_SitFinish()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD dwStuff;
};

//每日赛,每局开始
struct CMD_GF_DayMatch_SetStart
{
	CMD_GF_DayMatch_SetStart()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wKindID;
	WORD	wServerID;
	DWORD	dwMatchID;
};

//每日赛,每局结束
struct CMD_GF_DayMatch_SetEnd
{
	CMD_GF_DayMatch_SetEnd()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wKindID;
	WORD	wServerID;
	DWORD	dwMatchID;
};

//比赛将关闭
struct CMD_GF_DayMatch_WillClose
{
	CMD_GF_DayMatch_WillClose()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wSecondCnt;			//秒数		
	BYTE	cbMatchEndTime;				
	BYTE	cbPlayerLocation;	//玩家所处位置
};



//////////////////////////////////////////////////////////////////////////

//共享内存定义
struct tagShareMemory
{
	WORD								wDataSize;						//数据大小
	HWND								hWndGameFrame;			//框架句柄
	HWND								hWndGamePlaza;				//广场句柄
	HWND								hWndGameServer;			//房间句柄
};

//////////////////////////////////////////////////////////////////////////

//发送场所
#define	LOCATION_GAME_ROOM				1								//游戏房间
#define	LOCATION_PLAZA_ROOM				2								//大厅房间
#define	LOCATION_SHOPPINGMALL			3								//商城
//////////////////////////////////////////////////////////////////////////

//赠送结构
struct CMD_GF_Gift
{
	BYTE								cbSendLocation;				//发送场所
	DWORD						dwSendGameID;				//赠送者ID
	DWORD						dwRcvGameID;					//接受者ID
	WORD							wGiftID;								//礼物ID
	WORD							wFlowerCount;					//鲜花数目
};

//	购买道具
struct CMD_GF_BuyProps
{
	BYTE										cbSendLocation;					//发送场所
	WORD									wPropsID;								//道具ID
	DWORD								dwPachurseCount;				//购买数目
	DWORD								dwSourceGameID;				//购买玩家
	DWORD								dwTargetGameID;					//接收目标gameID玩家
	TCHAR									szPassword[PWD_LEN];		//银行密码
};

//喇叭结构
struct CMD_GF_BugleProperty
{
	BYTE								cbSendLocation;							//发送场所
	TCHAR							szUserName[32];							//玩家帐号
	COLORREF					crText;											//文字颜色
	TCHAR							szContext[BUGLE_MAX_CHAR];		//喇叭内容
};

//兑换结构
struct CMD_GF_ExchangeCharm
{
	DWORD						dwUserID;							// USERID
	BYTE								cbSendLocation;				//发送场所
	LONGLONG					lCharm;								//魅力数值
	TCHAR							szPassword[PWD_LEN];		//银行密码
};

//兑换魅力成功
struct CMD_GF_ExchangeCharmOK
{
	DWORD						dwUserID;							// USERID
	LONGLONG					lBankScore;						//银行金币
	LONGLONG					lGameScore;						//游戏金币
	LONGLONG					lCharm;								//魅力数值

};

//购买、赠送道具通知
struct CMD_GF_PropsNotify
{
	BYTE								cbSendLocation;						//发送场所
	BYTE								cbPropType;								// 道具类型  0:礼物； 1：会员卡
	DWORD						dwSrcGameID;							//赠送者gameID
	TCHAR							szSrcNickname[NAME_LEN];		//赠送者昵称
	DWORD						dwRcvGameID;							//接受者gameID
	TCHAR							szRcvNickname[NAME_LEN];		//接收昵称
	WORD							wGiftID;										//道具ID
	TCHAR							szPropsName[PROPS_NAME];	//道具名称
	WORD							wCount;									//购买数目
	LONGLONG					lBankScore;								// 银行金币
	LONGLONG					lSrcCharm;								// 赠送者增加魅力
	LONGLONG					lRecCharm;								// 接收者增加魅力
};



//存储金币
struct CMD_GF_BankStorage
{
	LONGLONG 							lStorageValue;					//存储金币
	BYTE										cbGameAction;					//游戏操作
	TCHAR									szPassword[PWD_LEN];		//用户密码
};

//提取金币
struct CMD_GF_BankGet
{
	LONGLONG						lGetValue;							//提取金币
	BYTE									cbGameAction;					//游戏操作
	TCHAR								szPassword[PWD_LEN];		//用户密码
};

//转出金币
struct CMD_GF_BankTransfer
{
	LONGLONG                      lTransferValue;                 //转出金币
	BYTE								    cbGameAction;                   //游戏操作
	DWORD                           dwTagGameID;                   //目标USER I D
	TCHAR                             szTagAccounts[NAME_LEN];        //目标帐号
	TCHAR                             szPassword[PWD_LEN];
};

//密码安全
struct CMD_GF_BankSecurity
{
	TCHAR								szAccounts[NAME_LEN];			//帐号
	TCHAR								szPassword[PWD_LEN];           //旧的密码
	TCHAR								szNewPassword[PWD_LEN];    //新的密码
};

//银行错误结果
struct CMD_GF_BankError
{
	BYTE                                cbGameAction;              //游戏操作
	LONG								lResultType;					//结果类型
	LONG								lErrorCode;					//错误代码
	TCHAR								szErrorDescribe[128];		//错误消息
};

//银行信息
struct CMD_GF_BankInfo
{
	BYTE                                cbGameAction;					    //游戏操作  1->存； 2->取；3->转账;  0->登陆
	TCHAR                             szAccounts[NAME_LEN];		  // 帐号
	LONGLONG						lBankScore;							 //  当前最新银行金额
	LONGLONG						lGameScore;							 //  当前最新游戏金额
	LONGLONG						lCharm;									//   当前魅力
};

// 银行操作
struct CMD_GF_BankAction
{
	BYTE									btActionID;								// 操作ID			1->存； 2->取;  3->转账
	DWORD                           dwTagGameID;						   //目标USER I D
	LONGLONG						lScore;									  // 金额 
	TCHAR                             szAccounts[NAME_LEN];		  //帐号
	TCHAR                             szTagAccounts[NAME_LEN];   //目标帐号
	TCHAR                             szPassword[PWD_LEN];		     // 银行密码	
};

//邮件结果
struct CMD_GF_UserMailResult
{
	DWORD								dwUserID;
	LONG								lUnreadMailCnt;					//未读邮件数量
};

//剩余道具
struct CMD_GF_ResidualProperty
{
	DWORD								dwResidualTime[PROPERTY_COUNT];	//剩余时间
};

//  错误
struct tagError
{
	tagError()
	{
		memset(this, 0, sizeof(*this));
	}
	LONG								lErrorCode;						//错误代码
	TCHAR								szErrorDescribe[128];			//错误消息
};
//////////////////////////////////////////////////////////////////////////
#endif