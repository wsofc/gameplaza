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

	void UpdateTableStatus(tagGS_TableStatus& tStruct);							// ��������״̬
	void UpdateTableStatus(WORD wTableID, tagTableStatus& tStruct);				// ��������״̬
	Table* GetTable(UserData& uStruct);											// ͨ����ҽṹ��������
	Table* GetTableByID(WORD wTableID);											// ͨ������ID��������
	Table* GetFreeTable(bool bEmpty = false);									// ��������λ������(bEmpty:���ҿ���)
	
	void QuickJoin(Table* pTable, WORD wChairIndex, bool bSitReq = true);		// ������������
	void QuickJoinCallBack(Table* pTable, WORD wChairIndex, bool bSitReq);		// ���������ص�
	void SetFrozenEnable(bool bEnable);											// �Ƿ���÷�����������¼�

private:
	std::map<WORD, Table*>	m_vTableMap;
	CPaintManagerUI*		m_pManager;
	CZoomBoxUI*				m_pZoomBox;											// ��������������ؼ�
};


#endif //__DESK_MANAGER_H__