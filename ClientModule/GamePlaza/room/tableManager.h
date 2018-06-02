#ifndef __DESK_MANAGER_H__
#define __DESK_MANAGER_H__

#include "table.h"

typedef std::function<void(Table* pTable, WORD wChairIndex, bool bSitReq)> QuickJoinAnimationCallBack;

class CZoomBoxUI : public CControlUI
{
public:
	CZoomBoxUI();
	~CZoomBoxUI();

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);
	void	DoInit() override;
	void	DoPaint(HDC hDC, const RECT& rcPaint) override;
	void	DoEvent(TEventUI& event) override;
	
	bool	PlayAnimation(QuickJoinAnimationCallBack cbFunc, Table* pTable, WORD wChairIndex, LONG lOffset, bool bSitReq);

private:
	void    _OnTimer(UINT_PTR idEvent, DWORD dwElapse);

private:
	DWORD						m_wColor;
	DWORD						m_dwLastElapse;
	FLOAT						m_fDisSum;
	FLOAT						m_fSpeed;
	RECT						m_srcRect;
	RECT						m_tagRect;
	RECT						m_tmpRect;
	Table*						m_pTableRef;
	WORD						m_wChairIndex;
	bool						m_bSitReq;
	QuickJoinAnimationCallBack	m_cbFunc;
};

class TableManager
{
public:
	TableManager(CPaintManagerUI* pManager);
	~TableManager();
	
	void CreatDeskPanel(WORD wRoomID, WORD wKindID, WORD wTableCount, WORD wChairCount, bool bHundredGame, bool bHundredLayout);

	void UpdateTableStatus(tagGS_TableStatus& tStruct);							// 更新桌子状态
	void UpdateTableStatus(WORD wTableID, tagTableStatus& tStruct);				// 更新桌子状态
	Table* GetTable(UserData& uStruct);											// 通过玩家结构查找桌子
	Table* GetTableByID(WORD wTableID);											// 通过桌子ID查找桌子
	Table* GetFreeTable(bool bEmpty = false);									// 查找有座位的桌子(bEmpty:查找空桌)
	
	void QuickJoin(Table* pTable, WORD wChairIndex, bool bSitReq = true);		// 快速入桌动画
	void QuickJoinCallBack(Table* pTable, WORD wChairIndex, bool bSitReq);		// 入桌动画回调
	void SetFrozenEnable(bool bEnable);											// 是否禁用房间桌子面板事件

private:
	std::map<WORD, Table*>	m_vTableMap;
	CPaintManagerUI*		m_pManager;
	CZoomBoxUI*				m_pZoomBox;											// 快速入桌动画框控件
};


#endif //__DESK_MANAGER_H__