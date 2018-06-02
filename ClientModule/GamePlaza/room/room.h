#ifndef __ROOM_H__
#define __ROOM_H__

#include "roomManager.h"
#include "tableManager.h"

enum ROOM_STATUS //房间状态
{
	ROOM_STATUS_NULL = 0,	//默认:无
	ROOM_STATUS_FREE,		//空闲

	ROOM_STATUS_EXTRA		//额外的预留项
};

enum ROOM_TYPE //房间类型
{
	ROOM_TYPE_NULL = 0,		//默认:无
	ROOM_TYPE_NORMAL,		//常规
	ROOM_TYPE_TIME_MATCH,	//定时赛

	ROOM_TYPE_EXTRA			//额外的预留项
};

// 道具面板结构
struct PropsPanelStruct
{
	DWORD						dwMaxCountsPerPage;		// 一页显示的最大物品数量
	DWORD						dwCurPage;				// 当前页
	DWORD						dwMinPage;				// 最小页
	DWORD						dwMaxPage;				// 最大页
	CButtonUI*					pBtnLeft;				// 左翻页按钮
	CButtonUI*					pBtnRight;				// 右翻页按钮
	CString						pzField;				// 控件公用字段
	std::vector<tagPropsInfo*>	vPropsMapRef;			// 道具容器引用

	void Reset()
	{
		dwMaxCountsPerPage	= 0;
		dwCurPage			= 0;
		dwMinPage			= 0;
		dwMaxPage			= 0;
		pBtnLeft			= nullptr;
		pBtnRight			= nullptr;
		pzField				= _T("");
		vPropsMapRef.clear();
	}
};

class MallBuyFrame;
class RoomManager;

class Room
{
public:

	Room(CPaintManagerUI* pManager, RoomManager* pRoomManagerRef);
	~Room();

	void InitPropsUI();																		// 初始化道具栏UI
	void ResetPropsUI();																	// 清除道具栏UI
	void PropsTurnPage(PropsPanelStruct& tTarget, bool bNext);								// 道具栏翻页
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// 道具消息

	void InitRoomData(tagGameServer* pData);												// 大厅登录时设置
	void InitRoomData(CMD_GR_ServerInfo* pData);											// 房间登录时设置
	void Notify(TNotifyUI& msg);															// 控件消息
	void CreatTables();																		// 创建桌子
	void RefreshTitleLabel(bool bShowOnline = true);										// 更新房间标题Lab
	void UpdateTableStatus(tagGS_TableStatus& tStruct);										// 更新桌子状态
	void UpdateTableStatus(WORD wTableID, tagTableStatus& tStruct);							// 更新桌子状态
	bool UpdateChairsStatus(UserData& uStruct);												// 更新椅子状态
	void SitDownOnTableReq(UserData& uStruct);												// 坐下请求
	void StandUpOnTableReq(UserData& uStruct);												// 起立请求
	void QuickJoin(bool bAnimation);														// 快速加入
	void QuickLocation(DWORD dwUserID);														// 快速定位玩家
	
	void AddOnLineCount(int iValue){ m_dwOnlineCount += iValue; }
	void SetServerID(WORD wServerID)	{ m_wServerID = wServerID; }
	void StartGames();																		// 启动游戏
	void CloseGameNotify(bool bEnforce = false);											// 关闭游戏(bEnforce:强制关闭)
	void ClearUserDataMap();																// 清除玩家数据表
	void ClearUserListUI();																	// 重置玩家列表UI
	void ResetChatUI();																		// 重置聊天UI
	void ClearAll();																		// 清除房间所有数据

	const bool IsHundredGame()			{ return m_bHundredGame; }
	const bool IsHundredLayout()		{ return m_bHundredLayout; }
	const WORD GetKindID()				{ return m_wKindID; }
	const WORD GetServerID()			{ return m_wServerID; }
	const WORD GetSortID()				{ return m_wSortID; }
	const WORD GetStationID()			{ return m_wStationID; }
	const WORD GetServerType()			{ return m_wRoomServiceType; }
	const DWORD GetOnlineCount()		{ return m_dwOnlineCount; }
	const LPCTSTR GetName()				{ return m_pzName; }
	const LPCTSTR GetGameName()			{ return m_pzGameName; }
	const LPCTSTR GetSeasonName()		{ return m_pzSeasonName; }

	const WORD GetTableCount()			{ return m_wTableCount; }
	const WORD GetChairCount()			{ return m_wChairCount; }
	const LONGLONG GetLessScore()		{ return m_lLessScore; }
	const LONGLONG GetMaxScore()		{ return m_lMaxScore; }
	const LONGLONG GetRestrictScore()	{ return m_lRestrictScore; }
	const WORD GetServerPort()			{ return m_wServerPort; }
	const DWORD GetServerAddr()			{ return m_dwServerAddr; }
	const DWORD GetVideoAddr()			{ return m_dwVideoAddr; }
	const BYTE GetHideUserInfo()		{ return m_cbHideUserInfo; }
	TableManager* GetTableManager()		{ return m_pTableManager; }

private:

	bool						m_bHundredGame;						// 是否是百人游戏
	bool						m_bHundredLayout;					// 是否百人游戏布局(百人布局:无椅子, 常规布局:有椅子)
	WORD						m_wKindID;							// 所属游戏I D
	WORD						m_wServerID;						// 房间ID
	WORD						m_wSortID;							// 排序号码
	WORD						m_wStationID;						// 站点号码
	WORD						m_wRoomServiceType;					// 房间/场次类型
	DWORD						m_dwOnlineCount;					// 在线人数
	CDuiString					m_pzName;							// 房间名
	CDuiString					m_pzSeasonName;						// 场次名
	CDuiString					m_pzGameName;						// 所属游戏名

	WORD						m_wTableCount;						// 桌子数目
	WORD						m_wChairCount;						// 椅子数目
	WORD						m_wTaxRate;							// 游戏税收
	LONGLONG					m_lLessScore;						// 最低积分
	LONGLONG					m_lMaxScore;						// 最高积分
	LONGLONG					m_lRestrictScore;					// 限制积分
	
	WORD						m_wServerPort;						// 房间端口
	DWORD						m_dwServerAddr;						// 房间地址
	DWORD						m_dwVideoAddr;						// 视频地址
	
	BYTE						m_cbHideUserInfo;					// 其他隐藏信息
	
	CPaintManagerUI*			m_pManager;							// 窗口绘图管理器
	RoomManager*				m_pRoomManagerRef;					// 房间管理器引用
	TableManager*				m_pTableManager;					// 桌子管理器

	TCHAR						m_pzBuffer[MAX_PATH];				// 字符串缓冲区
	PropsPanelStruct			m_tPropsPanel;						// 道具面板
	PropsPanelStruct			m_tGiftsPanel;						// 礼物面板
};

#endif //__ROOM_H__