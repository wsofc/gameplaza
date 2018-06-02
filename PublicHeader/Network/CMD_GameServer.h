
#pragma once

//-----------------------------------------------
//登录数据包定义
#define MAIN_GS_LOGON				0x3000								//房间登录

#define SUB_GS_LOGON_BY_USER_ID		0x3002								//I D 登录

#define SUB_GS_LOGON_SUCCESS		0x3003								//登录成功
#define SUB_GS_LOGON_ERROR			0x3004								//登录失败
#define SUB_GS_LOGON_FINISH			0x3005								//登录完成

//房间 ID 登录
struct tagGS_LogonByUserID
{
	tagGS_LogonByUserID()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwProcessVersion;					//进程版本
	DWORD							dwUserID;									//用户 I D
	TCHAR								szPwd[PWD_LEN];						//登录密码
};

//登录成功消息
struct tagGS_RT_LogonSuccess
{
	tagGS_RT_LogonSuccess()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD							dwUserID;							//用户 I D
};

//登录失败
struct tagGS_RT_LogonError
{
	tagGS_RT_LogonError()
	{
		memset(this,0,sizeof(*this));
	}
	LONG							lErrorCode;							//错误代码
	TCHAR							szErrorDescribe[128];				//错误消息
};



//-----------------------------------------------
//用户数据包定义

#define MDM_GR_USER					0x3100									//用户信息

#define SUB_GR_USER_SIT_REQ			1									//坐下请求
#define SUB_GR_USER_LOOKON_REQ		2									//旁观请求
#define SUB_GR_USER_STANDUP_REQ		3									//起立请求
#define SUB_GR_USER_LEFT_GAME_REQ	4									//离开游戏

#define SUB_GR_USER_COME			100									//用户进入
#define SUB_GR_USER_STATUS			101									//用户状态
#define SUB_GR_USER_SCORE			102									//用户分数
#define SUB_GR_USER_SIT_FAILED		103									//坐下失败
#define SUB_GR_USER_RIGHT			104									//用户权限
#define SUB_GR_USER_MEMBER_ORDER	105									//会员等级

#define SUB_GR_USER_CHAT			200									//聊天消息
#define SUB_GR_USER_WISPER			201									//私语消息
#define SUB_GR_USER_RULE			202									//用户规则

#define SUB_GR_USER_INVITE			300									//邀请消息
#define SUB_GR_USER_INVITE_REQ		301									//邀请请求

#define SUB_GR_USER_UPDATE_GOLD		401									//更新银行信息

//会员等级
struct CMD_GR_MemberOrder
{
	DWORD							dwUserID;							//数据库 ID
	BYTE							cbMemberOrder;						//会员等级
};

//用户权限
struct CMD_GR_UserRight
{
	DWORD							dwUserID;							//数据库 ID
	DWORD							dwUserRight;						//用户权限
};

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//数据库 ID
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbUserStatus;						//用户状态
};

//用户分数
struct CMD_GR_UserScore
{
	LONGLONG							lCharm;								//用户魅力
	//LONG							lBankScore;						//消费金币
	//LONG							lGameGold;							//游戏金币
	DWORD							dwUserID;							//用户 I D
	tagUserScore					UserScore;							//积分信息
};

//请求坐下
struct CMD_GR_UserSitReq
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbPassLen;							//密码长度
	TCHAR							szTablePass[PWD_LEN];				//桌子密码
};

//邀请用户 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//坐下失败
struct CMD_GR_SitFailed
{
	CMD_GR_SitFailed()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR			szFailedDescribe[256];				//错误描述
};

//聊天结构 
struct CMD_GR_UserChat
{
	WORD							wChatLength;						//信息长度
	COLORREF					crFontColor;						//信息颜色
	DWORD						dwSendUserID;						//发送用户
	DWORD						dwTargetUserID;						//目标用户
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//聊天信息
};

//私语结构 
struct CMD_GR_Wisper
{
	WORD							wChatLength;						//信息长度
	COLORREF						crFontColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//聊天信息
};

//用户规则
struct CMD_GR_UserRule
{
	bool							bPassword;							//设置密码
	bool							bLimitWin;							//限制胜率
	bool							bLimitFlee;							//限制逃跑
	bool							bLimitScore;							//限制分数
	bool							bCheckSameIP;						//效验地址
	WORD						wWinRate;							//限制胜率
	WORD						wFleeRate;							//限制逃跑
	LONGLONG 				lMaxScore;							//最高分数 
	LONGLONG 				lLessScore;							//最低分数
	TCHAR						szPassword[PWD_LEN];			//桌子密码
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};


//-----------------------------------------------
//配置信息数据包

#define MDM_GR_INFO					0x3200								//配置信息

#define SUB_GR_SERVER_INFO			100									//房间配置
#define SUB_GR_ORDER_INFO			101									//等级配置
#define SUB_GR_MEMBER_INFO			102									//会员配置
#define SUB_GR_COLUMN_INFO			103									//列表配置
#define SUB_GR_CONFIG_FINISH		104									//配置完成

//游戏房间信息
struct CMD_GR_ServerInfo
{
	//房间属性
	WORD							wKindID;							//类型 I D
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目
	DWORD							dwVideoAddr;						//视频地址

	//扩展配置
	WORD							wRoomServiceType;					//房间类型
	BYTE							cbHideUserInfo;						//隐藏信息
};

//分数描述信息
struct CMD_GR_ScoreInfo
{
	WORD							wDescribeCount;						//描述数目
	WORD							wDataDescribe[16];					//数据标志
};

//等级描述结构
struct tagOrderItem
{
	long 							lScore;								//等级积分
	WORD							wOrderDescribe[16];					//等级描述
};

//等级描述信息
struct CMD_GR_OrderInfo
{
	WORD							wOrderCount;						//等级数目
	tagOrderItem					OrderItem[128];						//等级描述
};

//列表项描述结构
struct tagColumnItem
{
	WORD							wColumnWidth;						//列表宽度
	WORD							wDataDescribe;						//字段类型
	TCHAR							szColumnName[16];					//列表名字
};

//列表描述信息
struct CMD_GR_ColumnInfo
{
	WORD							wColumnCount;						//列表数目
	tagColumnItem					ColumnItem[32];						//列表描述
};



//-----------------------------------------------
//房间状态数据包

#define MAIN_GS_STATUS				0x3300								//状态信息

#define SUB_GS_TABLE_INFO			0x3301								//桌子信息
#define SUB_GS_TABLE_STATUS			0x3302								//桌子状态

//桌子状态结构
struct tagTableStatus
{
	tagTableStatus()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE							bTableLock;							//锁定状态
	BYTE							bPlayStatus;						//游戏状态
};

//桌子状态数组
struct tagGS_TableInfo
{
	tagGS_TableInfo()
	{
		memset(this,0,sizeof(*this));
	}
	WORD							wTableCount;						//桌子数目
	tagTableStatus				TableStatus[512];					//状态数组
};

//桌子状态信息
struct tagGS_TableStatus
{
	tagGS_TableStatus()
	{
		memset(this,0,sizeof(*this));
	}
	WORD						wTableID;							//桌子号码
	BYTE							bTableLock;							//锁定状态
	BYTE							bPlayStatus;						//游戏状态
};



//-----------------------------------------------
//管理数据包

#define MDM_GR_MANAGER				0x3400								//管理命令

#define SUB_GR_SEND_WARNING		1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER					4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS		5									//设置禁用帐户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置
#define SUB_GR_OPTION_SERVER		7									//设置房间设置
#define SUB_GR_GMCONTROL				8									//GM控制
#define SUB_GR_GMGETINFO				9									//GM获取信息 

//发送警告
struct CMD_GR_SendWarning
{
	WORD							wChatLength;											//信息长度
	DWORD						dwTargetUserID;										//目标用户
	TCHAR							szWarningMessage[MAX_CHAT_LEN];		//警告消息
};

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//游戏消息
	BYTE							cbRoom;								//游戏消息
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[MAX_CHAT_LEN];		//系统消息
};

//查看地址
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//目标用户
};

//踢出用户
struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//禁用帐户
struct CMD_GR_LimitAccount
{
	DWORD							dwTargetUserID;						//目标用户
};

//权限设置
struct CMD_GR_SetUserRight
{
	//目标用户
	DWORD							dwTargetUserID;						//目标用户

	//绑定变量
	BYTE							cbGameRight;						//帐号权限
	BYTE							cbAccountRight;					//帐号权限

	//权限变化
	BYTE							cbLimitRoomChat;					//大厅聊天
	BYTE							cbLimitGameChat;					//游戏聊天
	BYTE							cbLimitPlayGame;					//游戏权限
	BYTE							cbLimitSendWisper;				//发送消息
	BYTE							cbLimitLookonGame;			//旁观权限
};

//房间设置
struct CMD_GR_OptionServer
{
	bool					bRoomChat;				//房间聊天
	bool					bGameChat;				//游戏聊天
	bool					bRoomWisper;			//房间私聊
	bool					bEnterRoom;				//进入房间
	bool					bEnterGame;				//进入游戏桌子
	bool					bShallClose;				//即将关闭
};

#define GM_OPERATION_WINORLOST				1					// 设置玩家输赢
#define GM_OPERATION_WININNINGS			2					// 赢几局
#define GM_OPERATION_LOSTINNINGS			3					// 输几局
#define GM_OPERATION_STORESCORE			4					// 设置库存
#define GM_OPERATION_ROBOTNUMS			5					// 设置机器人数量
#define GM_OPERATION_ROBOTSCORE			6					// 设置机器人分数范围
#define GM_OPERATION_ADDBLACKUSER		7					// 加入黑名单
#define GM_OPERATION_DELBLACKUSER		8					// 删除黑名单
#define GM_OPERATION_GETROOMINFO		9					// 获取房间设置

//GM设置（接收和发送都用这个）
struct CMD_GM_Option
{
	CMD_GM_Option()
	{
		memset(this, 0, sizeof(*this));
	}
	BYTE								btOperationID;		// GM操作ID
	DWORD						dwUserID;				//	用户ID
	LONGLONG					lValue;			// 参数一
	LONGLONG					lValue2;			// 参数二
	LONGLONG					lValue3;			// 参数三
};

//-----------------------------------------------
//系统数据包

#define MAIN_GS_SYSTEM				0x3500								//系统信息
#define SUB_GS_MESSAGE				0x3501								//系统消息

//消息类型
#define SMT_INFO					0x0001								//信息消息
#define SMT_EJECT					0x0002								//弹出消息
#define SMT_GLOBAL					0x0004							//全局消息
#define SMT_CLOSE_ROOM				0x1000						//关闭房间
#define SMT_INTERMIT_LINE			0x4000						//中断连接

//消息数据包
struct tagGS_Message
{
	tagGS_Message()
	{
		memset(this,0,sizeof(*this));
	}

	WORD							wMessageType;						//消息类型
	WORD							wMessageLength;						//消息长度
	char								szContent[MAX_SYSMESSAGE_LEN];						//消息内容
};



//-----------------------------------------------
//房间数据包

#define MAIN_GS_SERVER_INFO			0x3600								//房间信息

#define SUB_GS_ONLINE_COUNT_INFO	0x3601								//在线信息

//人数信息
struct tagOnlineCountInfo
{
	tagOnlineCountInfo()
	{
		memset(this,0,sizeof(*this));
	}

	WORD							wKindID;							//类型标识
	DWORD							dwOnlineCount;						//在线人数
};


/*-----------*/
struct tagRoomRobotInfo
{
	tagRoomRobotInfo()
	{
		memset(this, 0, sizeof(*this));
	}

	int								iRobotCounts;					// 机器人数量 
	LONGLONG				lRobotMinScore;				// 机器人最小分值
	LONGLONG				lRobotMaxScore;				// 机器人最大分值
	int								iArrayCounts;					// 机器人分数细分数量
	int								iArrayRate[10];					// 机器人分数细分 百分比
	LONGLONG				lArrayScore[10];					// 机器人分数细分 分数 
};
//-----------------------------------------------
//					the end
//-----------------------------------------------
