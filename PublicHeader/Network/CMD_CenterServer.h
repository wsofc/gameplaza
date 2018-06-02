/*

REQ -- request 请求
ANW -- answer  应答

*/
#pragma once


//-----------------------------------------------
//游戏列表命令码
//

#define MAIN_CS_SERVER_LIST				0x1000							//列表信息

#define SUB_CS_GET_SERVER_LIST		0x1001							//请求列表

#define SUB_CS_LIST_BEGIN					0x1002							//列表信息
#define SUB_CS_LIST_TYPE					0x1003							//类型列表
#define SUB_CS_LIST_KIND					0x1004							//种类列表
#define SUB_CS_LIST_STATION				0x1005							//站点列表
#define SUB_CS_LIST_GAME_SERVER	0x1006							//房间服务器列表
#define SUB_CS_LIST_HALL_SERVER		0x1007							//大厅服务器列表
#define SUB_CS_LIST_FINISH					0x1010							//列表完成

//列表开始
struct tagCS_ListBegin
{
	tagCS_ListBegin()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwTypeCount;					//类型数目
	DWORD				dwKindCount;					//种类数目
	DWORD				dwGameServerCount;				//房间数目
	DWORD				dwHallServerCount;				//大厅服务器数目
	DWORD				dwStationCount;					//站点数目
};



//-----------------------------------------------
//房间管理
//

#define MAIN_CS_SERVER_MANAGER					0x1100				//房间管理

#define SUB_CS_LS_REGISTER								0x1101				//注册登陆服务器
#define SUB_CS_LS_UNREGISTER							0x1102				//注销

#define SUB_CS_HS_REGISTER								0x1110				//注册大厅服务器
#define SUB_CS_HS_UNREGISTER							0x1111				//注销
#define SUB_CS_HS_ADD_GAME_SERVER				0x1112				//增加房间
#define SUB_CS_HS_SUB_GAME_SERVER				0x1113				//减少房间

#define SUB_CS_GS_REGISTER								0x1120				//注册游戏服务器
#define SUB_CS_GS_UNREGISTER							0x1121				//注销
#define SUB_CS_GS_SERVER_ONLINE_COUNT		0x1122				//房间人数


//注册大厅服务器
struct tagCS_HS_RegisterHallServer
{
	tagCS_HS_RegisterHallServer()
	{
		memset(this,0,sizeof(*this));
	}
	tagHallServer		HallServer;						
};

//注销大厅服务器
struct tagCS_HS_UnregisterHallServer
{
	tagCS_HS_UnregisterHallServer()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wServerID;				
};


struct tagCS_HS_AddGameServer
{
	tagCS_HS_AddGameServer()
	{
		memset(this,0,sizeof(*this));
	}
	tagGameServer		GameServer;						
};
struct tagCS_HS_SubGameServer
{
	tagCS_HS_SubGameServer()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wKindID;
	WORD				wServerID;
};

//注册房间
struct tagCS_GS_RegisterServer
{
	tagCS_GS_RegisterServer()
	{
		memset(this,0,sizeof(*this));
	}
	tagGameServer		GameServer;						//房间信息
};

//注销房间
struct tagCS_GS_UnregisterServer
{
	tagCS_GS_UnregisterServer()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wKindID;						//名称号码
	WORD				wServerID;						//房间号码
};

//房间人数
struct tagCS_GS_ServerOnlineCount
{
	tagCS_GS_ServerOnlineCount()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wKindID;						//名称号码
	WORD				wServerID;						//房间号码
	DWORD				dwOnlineCount;					//在线人数
};



//-----------------------------------------------
//平台维护
//

#define MAIN_CS_PLATFORM_MAINTENANCE			0x1200				

#define SUB_CS_MAINTENANCE				0x1201			//维护


//维护
struct tagCS_Maintenance
{
	tagCS_Maintenance()
	{
		memset(this,0,sizeof(*this));
		wDelayCloseMinutes = 10;
	}
	bool	bMaintenanceFlag;				//是否将关闭
	WORD	wDelayCloseMinutes;	//延迟关闭的时间
};


// ----------------------------------------------
// 机器人

#define MAIN_CS_SERVER_ROBOT							0x1300				// 机器人命令
#define SUB_GC_ROBOT_LOADROBOT_REQ			0x1301				// 请求机器人账号
#define SUB_GC_ROBOT_OFFLINE_REQ					0x1302				// 机器人申请休息

#define SUB_CG_ROBOT_SENDACCOUNTS				0x1303				// 发送机器人账号

//机器用户
struct tagCS_RobotUser
{
	tagCS_RobotUser()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;			// 用户 I D
	DWORD							dwRoomID;		// 所在房间
	DWORD							dwWorkTime;	// 开始工作时间
	BOOL								bEnabled;			// 是否有效
};

// 请求增加机器人
struct tagCS_NeedRobot_Req
{
	tagCS_NeedRobot_Req()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;				// 房间 I D
	DWORD							dwRobotCounts;		// 机器人数量
};

// 发送机器人账号
struct tagCS_SendRobotAccounts
{
	tagCS_SendRobotAccounts()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;							// 房间 I D
	DWORD							dwUserID[MAX_ROBOT];		// 机器人userid
	DWORD							dwRobotCounts;					// 机器人数量
};

// 机器人请求下线
struct tagCS_OffLineReq
{
	tagCS_OffLineReq()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;		// 房间 I D
	DWORD							dwUserID;			// 机器人userid
};


// ----------------------------------------------
// 机器人

#define MAIN_CS_STOCKSCORE							0x2400				// 库存命令
#define SUB_CS_STOCKSCORE_SET_REQ				0x2401				// 请求设置库存
#define SUB_CS_STOCKSCORE_UPDATE_REQ		0x2402				// 请求更新库存
#define SUB_CS_STOCKSCORE_GET						0x2403				// 获取库存


struct tagCS_StockScore
{
	tagCS_StockScore()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;				// 房间 I D
	LONGLONG						lScore;						// 库存值/增加值
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
