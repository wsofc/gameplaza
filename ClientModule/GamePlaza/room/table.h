#ifndef __TABLE_H__
#define __TABLE_H__

#include "..\\data\\userData.h"

class UserData;
class CZoomBoxUI;

struct Chair
{
	bool bRead		= false;	// 是否准备好
	DWORD iIndex	= 0;		// 座位编号索引(下标从0开始)
	UserData* pUser	= nullptr;	// 用户指针

	CDuiString sBg;				// 座位背景图
	CDuiString sBoxBg;			// 座位边框图
	CDuiString scName;			// 座位控件名
	CDuiString stName;			// 座位文本控件名
	CDuiString srName;			// 座位准备控件名
};

class Table
{
public:
	Table();
	Table(WORD wRoomID, WORD wKindID, WORD wTableID, WORD wIndex, CPaintManagerUI* paintManager, WORD wChairCount, bool bHundredGame, bool bHundredLayout);
	~Table();

	CContainerUI* GetTableCtl(){ return m_pContainer; }
	CPaintManagerUI* GetPaintManager(){ return m_paintManager; }

	// --------------------------------------------------------------------------
	// 桌子属性

	// 是否为百人游戏
	bool IsHundredGame(){ return m_bHundredGame; }

	// 是否有椅子视图
	bool IsHundredLayout(){ return m_bHundredLayout; }

	//桌子 I D
	const WORD GetID(){ return m_wTableID; }
	const WORD GetIndex(){ return m_wIndex; }

	//是否为空桌
	bool IsEmptyTable();

	//是否为空闲桌(有座位,无锁,未开始游戏)
	bool IsFreeTable();

	//座位是否空闲
	bool IsEmptyChair(WORD wChairIndex);

	//获取空闲座位(索引值从小到大获取)
	WORD GetFreeChairIndex();

	//获取椅子通过索引
	Chair* GetChairByIndex(WORD wChairIndex);

	//锁定状态
	void SetTableLock(BYTE bTableLock){ m_bTableLock = bTableLock; }
	const BYTE GetTableLock(){ return m_bTableLock; }

	//游戏状态
	void SetPlayStatus(BYTE bPlayStatus){ m_bPlayStatus = bPlayStatus; }
	const BYTE GetPlayStatus(){ return m_bPlayStatus; }

	// --------------------------------------------------------------------------
	// 座位属性

	void SetChairBg(WORD wChairIndex, CDuiString sBg);
	const CDuiString GetChairBg(WORD wChairIndex);
	
	void SetChairIcon(WORD wChairIndex, CDuiString sIcon);
	const CDuiString GetChairIcon(WORD wChairIndex);

	void SetChairReady(WORD wChairIndex, bool bReady);
	const bool GetChairReady(WORD wChairIndex);

	void SetChairText(WORD wChairIndex, CDuiString sText);
	const CDuiString GetChairText(WORD wChairIndex);

	// --------------------------------------------------------------------------
	// 消息处理

	void Notify(TNotifyUI& msg);																// 控件消息
	void SitDownReq(WORD wChairIndex, CDuiString pzPassword = _T(""));							// 坐下请求
	void StandUpReq(WORD wChairIndex);															// 起立请求
	void UpdateTableStatus();																	// 更新桌子状态
	bool UpdateChairsUI(UserData& uStruct);														// 更新椅子视图
	bool LeftGameReq();																			// 强行退出
	void StartGames(WORD wKindID, UserData& uStruct);											// 启动游戏

	//// --------------------------------------------------------------------------
	//// 动画
	//void QuickJoin(WORD wChairIndex);															// 快速入桌
	//void QuickJoinCallBack(WORD wChairIndex);													// 入桌回调


	// 对话框事件回调
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

private:
	void _Reset();																				// 重置成员数据
	void _CreateTile();																			// 创建桌子列表
	void _InitControlData();																	// 初始化桌子控件
	void _SetPlayVisible(bool bPlay);															// 设置桌子游戏状态
	void _SetLockVisible(bool bVisible);														// 设置桌子锁定状态

private:
	WORD					m_wRoomID;															// 房间ID
	WORD					m_wKindID;															// 游戏ID
	WORD					m_wTableID;															// 桌子 I D
	WORD					m_wIndex;															// 桌子编号索引(下标从0开始)
	WORD					m_wChairCount;														// 椅子数
	CDuiString				m_path;																// 桌子资源相对路径
	BYTE					m_bTableLock;														// 锁定状态
	BYTE					m_bPlayStatus;														// 游戏状态
	bool					m_bHundredLayout;													// 是否百人游戏布局(百人布局:无椅子, 常规布局:有椅子)
	bool					m_bHundredGame;														// 是否是百人游戏

	CContainerUI*			m_pContainer;														// 桌子控件
	TCHAR					m_buffer[MAX_PATH];													// 字符串缓冲数组
	CPaintManagerUI*		m_paintManager;														// 绘图UI管理器
	std::vector<Chair*>		m_ChairsArr;														// 座位数组
};

#endif //__TABLE_H__