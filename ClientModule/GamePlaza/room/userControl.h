#ifndef _USER_CONTROL_
#define _USER_CONTROL_

#include "..\\share\\control\\IUserControl.h"

typedef void(*ProcCreatUserMenu)(CPaintManagerUI*, ResInfo*, UserMenuStruct*, IUserControl*);
typedef void(*ProcDeleteUserMenu)(HWND hWnd);
typedef void(*ProcReciveWinScore)(LONGLONG lCurWinScore);
typedef void(*ProcReciveWinCount)(LONGLONG lCurWinCount);
typedef void(*ProcReciveLostCount)(LONGLONG lCurLostCount);
typedef void(*ProcReciveRoomSetting)(RoomSetData& tRoomSetData);
typedef void(*ProcReciveStokSetting)(LONGLONG lCurStokValue);
typedef void(*ProcReciveRobotCount)(LONGLONG lCurRobotCount);
typedef void(*ProcReciveRobotScore)(LONGLONG lCurRobotMax, LONGLONG lCurRobotMin);

class UserControl : public IUserControl
{
public:
	static UserControl* GetInstance();

	// 设置关联绘图管理器
	void BindPaintManager(CPaintManagerUI* pManager){ m_pManager = pManager; }

	// 显示菜单
	void ShowMenu(UserData* pTagUser);

	// 创建一般的菜单
	void CreateGeneralMenu();

	// 创建特殊的菜单
	void CreatSpecialMenu();

	// 获取玩家对象
	UserData* GetUserData(){ return m_pTagUser; }

	// 是否已经加载了DLL
	bool IsLoadMenuDll(){ return m_bLoadSuccess; }

	// 加载DLL
	bool LoadUserMenuDll();

	// 卸载DLL
	void UnLoadUserMenuDll();

	// 发送卸载预处理
	void PreUnLoadUserMenuDll();
	
	// 接收服务器回调的控制数据
	void OnGMControData(void* pDataBuffer, WORD wDataSize);

	// 接收房间设置回调数据
	void OnGMRoomData(void* pDataBuffer, WORD wDataSize);

// -----------------------------------------------------------------------
// 菜单回调接口
public:
	
	void ReqSetScore();																// 请求当前玩家设置的输赢分数
	void ReqWinCount();																// 请求当前玩家赢几局设置
	void ReqLostCount();															// 请求当前玩家输几局设置
	void ReqRoomSetting();															// 请求当前房间设置
	void ReqStokSetting();															// 请求当前库存设置
	void ReqRobotCount();															// 请求当前机器人数量设置
	void ReqRobotScore();															// 请求当前机器人分数设置

	void CallBackLocation();														// 定位玩家
	void CallBackSetScore(LONGLONG lGameID, LONGLONG lNewValue);					// 设置输赢分数
	void CallBackWinCount(LONGLONG lGameID, LONGLONG lCount);						// 设置赢几局
	void CallBackLostCount(LONGLONG lGameID, LONGLONG lCount);						// 设置输几局
	void CallBackKickRoom();														// 踢出房间
	void CallBackSendSysMsg(LPCTSTR lpMsg, bool bSendToGame, bool bSendToRoom);		// 发送系统消息
	void CallBackForbidAccount();													// 禁用用户账号
	void CallBackLookIP();															// 查看用户IP
	void CallBackRoomSetting(bool bRoomChat, bool bRoomWisper, bool bGameChat
		, bool bEnterRoom, bool bEnterGame, bool bShallClose);						// 房间设置
	void CallBackStokSetting(LONGLONG lNewValue);									// 设置当前库存
	void CallBackAddBlackList();													// 加入黑名单
	void CallBackDelBlackList();													// 删除黑名单
	void CallBackSetRobotCount(DWORD dwRobotCount);									// 设置机器人数量
	void CallBackSetRobotScore(LONGLONG lMax, LONGLONG lMin);						// 设置机器人分数范围

private:
	UserControl();
	~UserControl();
	static UserControl*				g_instance;					// 单例

	bool							m_bLoadSuccess;				// 是否加载成功
	TCHAR							m_pzBuffer[MAX_PATH];		// 字符串缓冲区
	std::map<CDuiString, bool>		m_MenuCheckInfo;			// 保存菜单的单选复选信息
	HINSTANCE						m_hInst;					// 动态库句柄
	CPaintManagerUI*				m_pManager;					// 窗口绘制器
	RoomManager*					m_pRoomManager;				// 房间管理器
	UserData*						m_pTagUser;					// 用户对象指针

	ProcCreatUserMenu				m_pFuncCreateMenu;			// 创建菜单入口
	ProcDeleteUserMenu				m_pFuncDeleteMenu;			// 卸载菜单入口
	ProcReciveWinScore				m_pFuncReciveWinScore;		// 接收设置输赢分数
	ProcReciveWinCount				m_pFuncReciveWinCount;		// 接收赢几局
	ProcReciveLostCount				m_pFuncReciveLostCount;		// 接收输几局
	ProcReciveRoomSetting			m_pFuncReciveRoomSetting;	// 接收房间设置
	ProcReciveStokSetting			m_pFuncReciveStokSetting;	// 接收库存设置
	ProcReciveRobotCount			m_pFuncReciveRobotCount;	// 接收设置机器人数量
	ProcReciveRobotScore			m_pFuncReciveRobotScore;	// 接收设置机器人分数范围
};

#endif // _USER_CONTROL_