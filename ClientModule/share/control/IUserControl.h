#ifndef _CONTROL_H_
#define _CONTROL_H_

// 借用Windows.h里面interface的定义
#ifndef __STRUCT__
	#define __STRUCT__ struct
#endif // __STRUCT__

#ifndef interface
	#define interface __STRUCT__
#endif // interface

#include <atlstr.h>

#ifndef NAME_LEN
	#define NAME_LEN	32
#endif // !NAME_LEN

#ifndef UNDER_WRITE_LEN
	#define UNDER_WRITE_LEN 32
#endif // !UNDER_WRITE_LEN

// 默认的显示的玩家数据
struct UserMenuStruct
{
	UserMenuStruct(){ memset(this, 0, sizeof(UserMenuStruct)); }

	bool			bSelf;								// 是否是自己
	DWORD			dwUserID;							// 用户I D
	DWORD			dwGameID;							// 游戏I D
	LONGLONG		lGameGold;							// 金币
	LONGLONG		lScore;								// 用户分数
	TCHAR			szNickname[NAME_LEN];				// 昵称
	TCHAR			szUnderWrite[UNDER_WRITE_LEN];		// 个性签名
};

// 房间设置
struct RoomSetData
{
	RoomSetData(){ memset(this, 0, sizeof(RoomSetData)); }

	bool			bRoomChat;							// 允许房间聊天
	bool			bRoomWisper;						// 允许房间私聊
	bool			bGameChat;							// 允许游戏聊天
	bool			bEnterRoom;							// 允许进入房间
	bool			bEnterGame;							// 允许进入游戏
	bool			bShallClose;						// 关闭房间
};

// 资源信息
struct ResInfo
{
	ResInfo(){ memset(this, 0, sizeof(ResInfo)); }

	unsigned short		uVersionHigh;					//版本号高位
	unsigned short		uVersionLow;					//版本号低位
	UILIB_RESOURCETYPE	iResourceType;					//资源类型
	HINSTANCE			hResourceInst;					//资源句柄
	LPCTSTR				iResourceID;					//资源ID
	int					iSkinResID;						//皮肤ID
	TCHAR				pzResPath[MAX_PATH];			//资源路径
	TCHAR				pzResDllPath[MAX_PATH];			//资源Dll路径
};

// 回调接口
interface IUserControl
{
	// ------------------------------------------------------------------------------------------------
	// 请求数据
	
	// 请求当前玩家设置的输赢分数
	virtual void ReqSetScore()																= 0;

	// 请求当前玩家赢几局设置
	virtual void ReqWinCount()																= 0;

	// 请求当前玩家输几局设置
	virtual void ReqLostCount()																= 0;

	// 请求当前房间设置
	virtual void ReqRoomSetting()															= 0;

	// 请求当前库存设置
	virtual void ReqStokSetting()															= 0;

	// 请求当前机器人数量设置
	virtual void ReqRobotCount()															= 0;

	// 请求当前机器人分数设置
	virtual void ReqRobotScore()															= 0;

	// ------------------------------------------------------------------------------------------------
	// 操作回调

	// 定位玩家
	virtual void CallBackLocation()															= 0;

	// 设置玩家输赢分数
	virtual void CallBackSetScore(LONGLONG lGameID, LONGLONG lNewValue)						= 0;

	// 设置赢几局
	virtual void CallBackWinCount(LONGLONG lGameID, LONGLONG lCount)						= 0;

	// 设置输几局
	virtual void CallBackLostCount(LONGLONG lGameID, LONGLONG lCount)						= 0;

	// 踢出房间
	virtual void CallBackKickRoom()															= 0;

	// 禁用账号
	virtual void CallBackForbidAccount()													= 0;

	// 查看IP
	virtual void CallBackLookIP()															= 0;

	// 发送系统消息
	virtual void CallBackSendSysMsg(LPCTSTR lpMsg, bool bSendToGame, bool bSendToRoom)		= 0;

	// 房间设置
	virtual void CallBackRoomSetting(bool bRoomChat, bool bRoomWisper, bool bGameChat
		, bool bEnterRoom, bool bEnterGame, bool bShallClose)								= 0;

	// 库存设置
	virtual void CallBackStokSetting(LONGLONG lNewValue)									= 0;

	// 添加黑名单
	virtual void CallBackAddBlackList()														= 0;

	// 删除黑名单
	virtual void CallBackDelBlackList()														= 0;

	// 设置机器人数量
	virtual void CallBackSetRobotCount(DWORD dwRobotCount)									= 0;

	// 设置机器人分数范围
	virtual void CallBackSetRobotScore(LONGLONG lMax, LONGLONG lMin)						= 0;
};

#endif // _CONTROL_H_