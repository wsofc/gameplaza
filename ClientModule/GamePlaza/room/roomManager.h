#ifndef __ROOM_MANAGER_H__
#define __ROOM_MANAGER_H__
#include "room.h"
#include "ui/UIGameItem.h"
#include "ipc/gameServer.h"

class Room;
class RoomManager : public GameSeverDelegate
{
public:

	// 断网状态
	typedef enum Broken_Network_Status
	{
		ONS_NORMAL	= 0,					// 正常断开
		ONS_INTERMIT,						// 被远程中断
		ONS_EXCEPTION,						// 网络异常
		ONS_FINALCLOSE						// 断网关闭(最终提示)
	}OFFNetStatus;

	enum RoomMsgBoxID
	{
		MSG_BOX_ID_NONE = 0,				// 无
		MSG_BOX_ID_GAME_STATUS_NONE,		// 游戏不存在
		MSG_BOX_ID_GAME_STATUS_LOW,			// 游戏版本过低
		MSG_BOX_ID_LOGIN_ROOM_CANCEL,		// 取消登录房间
		MSG_BOX_ID_LOGIN_ROOM_DISCONNECT,	// 房间网络断开
		MSG_BOX_ID_LOGIN_ROOM_FAILED,		// 登录房间失败
	};

	static RoomManager* GetInstance();
	static void Release();

	void InitStyle_1(CPaintManagerUI* pManager, CGameItemUI* pGameItem);
	void InitStyle_2(CPaintManagerUI* pManager, CGameItemUI* pGameItem);

	// 创建房间类型单选按钮
	void CreatRoomOptBtn(int iIndex, LPCTSTR pzText);

	// 刷新房间列表
	void RefreshRoomList(int iStyle = 0);

	// 设置房间列表折叠与否
	void SetRoomListExpand(bool bExpand);

	// 打印玩家进出房间提示至聊天框
	void PrintUserInOutRoom(UserData* pUser, bool bInOut);

	// 进入房间
	void EnterRoomReq(WORD wRoomID);

	// 退出房间
	void ExitPanel(bool bExitToHall);

	// 清除数据
	void Clean();

	// 获取当前窗口管理器
	CPaintManagerUI* GetPaintManager() { return m_pManagerRef; }

	// 获取当前房间ID
	WORD GetCurRoomID() { return m_wCurRoomID; }

	// 获取当前房间
	Room* GetCurRoom(){ return m_curRoom; }

	// 加载玩家控制组件
	bool LoadUserControl(CPaintManagerUI* pManager);

	// 卸载玩家控制组件
	void UnLoadUserControl();

	// 检查游戏状态
	void CheckGameStatus();

	// 对话框事件回调
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	// 通知游戏服更新银行数据等
	void NotifyGameServerUpdateBank();

	void Notify(TNotifyUI& msg);

	/*@breif: 添加视图根节点
	*@param	tabLayout		- 挂接的父节点
	*@param pzName			- 场次根节点名
	*/
	CTreeViewUI* AddSeasonRoot(CTabLayoutUI* tabLayout, LPCTSTR pzName);

	/*@breif: 添加场次节点
	*@param	pControl		- 必须是CTreeViewUI类型(该函数内部已做dynamic_cast检测)
	*@param name			- 场次节点名
	*@param text			- 场次显示文本
	*@param height			- 场次节点高度值
	*/
	CTreeNodeUI* AddSeasonNode(CControlUI* pControl, LPCTSTR pzName, LPCTSTR pzText, DWORD height);

	/*@breif: 添加场次子项
	*@param	pSeason			- 场次节点
	*@param pRoomData		- 房间结构
	*/
	CTreeNodeUI* AddSeasonItem(CTreeNodeUI* pSeason, tagGameServer* pRoomData);

	void MsgBoxProc(DLG_ID id, TNotifyUI& msg);
	void ShowWaittingText(bool bShow, CDuiString pzText = _T(""));							// 显示连接等待提示语
	void ShowDisconnectText(bool bShow, CDuiString pzText = _T(""));						// 显示断线重连提示语

	// ----------------------------------------------------------------------------------------
	// 下载模块
	void Download();																		// 下载

	// 网络模块
	void GameServerConnect(WORD wRoomID);													// 登录链接
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// 重连
	void DisConnect();																		// 断开连接
	void OffNetTips(BYTE cbShutReason);														// 断网提示
	bool OnConnectProc(bool bSuccess);														// 连接结果回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// 网络断开通知

	void StartReTry();																		// 断网自动重试
	void OnRetry();																			// 开始重连中
	void EndReTry();																		// 结束自动重试
	void ReconnectClear();																	// 清除重连之前的若干状态、UI等
	void OnTimer(UINT_PTR idEvent, DWORD dwTime);

	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID);										// 发送
	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID, void* const pData, WORD wDataSize);		// 发送
	bool OnNetwork_GameToServer(void* pData);												// 游戏上行主消息
	bool OnNetwork_TurnToGame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);	// 游戏下行主消息

private:
	bool OnNetwork_Logon(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 房间登录
	bool OnNetwork_Status(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 配置信息
	bool OnNetwork_HallMSG(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 系统信息
	bool OnNetwork_ServerInfo(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);	// 房间信息
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// 道具消息
	bool OnNetwork_User(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);			// 用户信息
	bool OnNetwork_Info(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);			// 配置信息
	bool OnNetwork_Manager(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 管理命令
	bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 银行消息
	bool OnNetwork_Game(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);			// 游戏逻辑信息
	bool OnNetwork_GameFrame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);	// 游戏状态消息
	
	void _InitGameLevelConfig();															// 初始化等级配置
	void _InitRoomSeasonConfig();															// 初始化房间场次配置
	void _InitRoomListUI_1();																// 初始化房间列表风格1(暂时注释)
	void _InitRoomListUI_2();																// 初始化房间列表风格2
	void _CleanRoomListUI();																// 清除房间面板UI

	void _UpdateSystemMessage(tagGS_Message& msg);											// 更新系统消息
	void _UpdateUserMessage(CMD_GR_UserChat& msg);											// 更新用户消息
	bool _UpdateUserComing(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 更新用户信息
	void _UpdateUserStatus(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 更新用户状态
	void _UpdateUserScore(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 更新用户分数
	void _UpdateUserRight(void* pDataBuffer, WORD wDataSize);								// 更新用户权限
	void _UpdateUserMember(void* pDataBuffer, WORD wDataSize);								// 更新用户会员等级

	void _UpdateUserStatusUI(const CMD_Command& cmd, UserData& stuTagUser);					// 更新玩家状态UI
	void _EnterRoom();																		// 进入房间
	void _ExitRoom();																		// 仅退出房间
	void _ExitToHall();																		// 退出到大厅

private:
	static RoomManager* g_instance;
	RoomManager();
	~RoomManager();
	
	bool				m_bLoginSuccess;													// 是否登录房间成功
	bool				m_bLoginFinished;													// 是否登录房间流程完毕
	int					m_iSelfComingCount;													// 自己的coming广播计数(因为会广播自己两次)
	TCHAR				m_pzBuffer[MAX_PATH];												// 字符串缓冲区
	BYTE				m_cbBuffer[IPC_BUFFER];												// 数据缓存区

	CPaintManagerUI*	m_pManagerRef;
	DWORD				m_wSeasonNodeHeight;												// 房间场次表头UI高度
	CGameItemUI*		m_pGameItem;														// 所属游戏
	Room*				m_curRoom;															// 玩家当前房间
	MsgBoxWnd*			m_pWaittingMsgBoxDlg;												// 等待对话框
	MsgBoxWnd*			m_pDisconnectMsgBoxDlg;												// 断线重连对话框
	WORD				m_wCurRoomID;														// 当前房间ID
	OFFNetStatus		m_uOffNetStatus;													// 网络断开状态(用于断网提示,重连等)
	GameServer*			m_pGameServer;														// 游戏服务器接口对象
	BYTE				m_cbReConnectCount;													// 网络异常重连次数
	BYTE				m_cbReConnecting;													// 断线重连状态(1 - 开始, 0 - 结束)
	LONGLONG			m_llReConnectTotalCount;											// 本次运行期间断线重连总次数
	
	std::vector<CDuiString>							m_vOptName;								// 单选按钮名
	std::map<DWORD, std::vector<tagGameServer*> >	m_RoomListMap;							// 房间列表
};

#endif //__ROOM_MANAGER_H__