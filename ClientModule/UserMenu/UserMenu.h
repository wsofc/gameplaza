#pragma once

#ifdef  USERMENU_EXPORTS
	#define USER_MENU_API _declspec(dllexport)
	#define USER_MENU_API_C extern "C" _declspec(dllexport)
#else
	#define USER_MENU_API _declspec(dllimport)
	#define USER_MENU_API_C extern "C" _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
#	define USER_MENU_CLASS_DLL_NAME	TEXT("UserMenu_d.dll")			// 组件 DLL 名字
#else
#	define USER_MENU_CLASS_DLL_NAME	TEXT("UserMenu.dll")			// 组件 DLL 名字
#endif

namespace UserMenuNSP {
	
	// 对话框ID枚举
	enum MsgEventID
	{
		MSGID_NONE	= 0,									// 无
		MSGID_USER_GAMEID,									// 玩家的游戏ID
		MSGID_SCORE_SETTING,								// 设置输赢分数
		MSGID_WIN_COUNT,									// 赢的局数
		MSGID_LOST_COUNT,									// 输的局数
		MSGID_KICK_ROOM,									// 踢出房间
		MSGID_SEND_SYSMSG,									// 发送系统消息
		MSGID_FORBID_ACCOUNT,								// 踢出房间
		MSGID_SEND_SYSMSG_RANG,								// 发送系统消息勾选范围
		MSGID_ROOM_SETTING,									// 房间设置
		MSGID_STOKE_SETTING,								// 库存设置
		MSGID_ROBOT_COUNT,									// 机器人数量
		MSGID_ROBOT_SCORE_MAX,								// 机器人最大分数范围
		MSGID_ROBOT_SCORE_MIN,								// 机器人最小分数范围
		MSGID_EXTRA											// 额外预留项
	};
	
	// 数据结构
	struct MenuDataStruct
	{
		bool			bSelf;								// 是否是自己
		DWORD			dwUserID;							// 用户I D
		DWORD			dwGameID;							// 游戏I D
		LONGLONG		lGameGold;							// 金币
		LONGLONG		lScore;								// 用户分数
		TCHAR			szNickname[NAME_LEN];				// 昵称
		TCHAR			szUnderWrite[UNDER_WRITE_LEN];		// 个性签名
		
		LONGLONG		lCurWinScore;						// 当前设置的分数
		LONGLONG		lCurWinCount;						// 当前赢几局
		LONGLONG		lCurLostCount;						// 当前输几局

		LONGLONG		lCurStokValue;						// 当前库存值

		LONGLONG		lCurRobotCount;						// 当前机器人数量
		LONGLONG		lCurRobotMax;						// 当前机器人最大分数
		LONGLONG		lCurRobotMin;						// 当前机器人最小分数

		RoomSetData		tRoomSetData;						// 房间设置
		ResInfo			tResInfo;							// 资源信息

		MenuDataStruct()
		{
			memset(this, 0, sizeof(this));
		}
	};
	
	// 正则匹配
	bool MatchRegular(CDuiString& strIn, LPCTSTR lpReg);

	// 创建菜单
	USER_MENU_API_C void CreatUserMenu(CPaintManagerUI* pManager, ResInfo* pResInfo, UserMenuStruct* pUserStruct, IUserControl* pDelegate);
	
	// 卸载菜单
	USER_MENU_API_C void DeleteUserMenu(HWND hWnd);

	// 接收当前输赢分数
	USER_MENU_API_C void ReciveWinScore(LONGLONG lCurWinScore);

	// 接收当前玩家赢几局
	USER_MENU_API_C void ReciveWinCount(LONGLONG lCurWinCount);
	
	// 接收当前玩家输几局
	USER_MENU_API_C void ReciveLostCount(LONGLONG lCurLostCount);

	// 接收当前房间设置
	USER_MENU_API_C void ReciveRoomSetting(RoomSetData& tRoomSetData);

	// 接收当前库存设置
	USER_MENU_API_C void ReciveStokSetting(LONGLONG lCurStokValue);
	
	// 接收当前机器人数量
	USER_MENU_API_C void ReciveRobotCount(LONGLONG lCurRobotCount);
	
	// 接收当前机器人分数
	USER_MENU_API_C void ReciveRobotScore(LONGLONG lCurRobotMax, LONGLONG lCurRobotMin);
}

#include "frame\\PanelScore.h"
#include "frame\\WinCount.h"
#include "frame\\LostCount.h"
#include "frame\\SendSysMsg.h"
#include "frame\\RoomSetting.h"
#include "frame\\StokSetting.h"
#include "frame\\RobotCount.h"
#include "frame\\RobotScore.h"

#include "frame\\UserMenuFrame.h"