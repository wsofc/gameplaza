#include "stdafx.h"
#include "tableManager.h"
#include "roomManager.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

class RoomManager;

TableManager::TableManager(CPaintManagerUI* pManager)
	: m_pManager(pManager)
{}

TableManager::~TableManager()
{
	// 清除桌子面板
	CTileLayoutUI* panel_1 = static_cast<CTileLayoutUI*>(m_pManager->FindControl(R->kTilRoomPanel_Panel_1));
	if (panel_1)
	{
		panel_1->RemoveAll();
	}

	CContainerUI* panel_2 = static_cast<CContainerUI*>(m_pManager->FindControl(R->kCtlRoomPanel_Panel_2));
	if (panel_2)
	{
		// 清空背景
		CContainerUI* pRoomPanel = static_cast<CContainerUI*>(m_pManager->FindControl(R->kTlsHallMain_RoomPanel));
		if (pRoomPanel)
		{
			pRoomPanel->SetBkImage(_T(""));
		}

		CControlUI* pCtlRoomBgBlock = static_cast<CControlUI*>(m_pManager->FindControl(R->kCtlRoomPanel_BgBlock));
		if (pCtlRoomBgBlock)
		{
			pCtlRoomBgBlock->SetBkImage(_T(""));
		}

		CControlUI* pCtlRoomBg = static_cast<CControlUI*>(m_pManager->FindControl(R->kCtlRoomPanel_Bg));
		if (pCtlRoomBg)
		{
			pCtlRoomBg->SetBkImage(_T(""));
		}

		// 禁用面板
		panel_2->SetVisible(false);
		panel_2->SetMouseEnabled(false);
		panel_2->SetMouseChildEnabled(false);

		// 清理动态添加的控件等
		panel_2->RemoveAll();
	}

	// 清除桌子容器
	std::map<WORD, Table*>::iterator it;
	for (it = m_vTableMap.begin(); it != m_vTableMap.end();)
	{
		Table* pDesk = it->second;
		SAFE_DELETE(pDesk);

		it = m_vTableMap.erase(it);
	}

	g_Logger->TryInfo(_T("TableManager - Destroy"));
}

void TableManager::CreatDeskPanel(WORD wRoomID, WORD wKindID, WORD wTableCount, WORD wChairCount, bool bHundredGame, bool bHundredLayout)
{
	g_Logger->TryInfo(_T("TableManager - CreatDeskPanel"));

	if (!m_pManager) return;

	// 无椅子类型(面板2:百人游戏布局)
	if (bHundredLayout)
	{
		CContainerUI* panel = static_cast<CTileLayoutUI*>(m_pManager->FindControl(R->kCtlRoomPanel_Panel_2));
		if (!panel) return;
		
		for (int index = 0; index < wTableCount; ++index)
		{
			Table* pDesk = new Table(wRoomID, wKindID, index, index, m_pManager, wChairCount, bHundredGame, bHundredLayout);
			if (!pDesk) continue;

			CContainerUI* table_tile = pDesk->GetTableCtl();
			if (table_tile)
			{
				m_vTableMap[index] = pDesk;
				panel->SetVisible(true);
				panel->SetMouseEnabled(true);
				panel->SetMouseChildEnabled(true);
				panel->Add(pDesk->GetTableCtl());
			}
			else
			{
				SAFE_DELETE(pDesk);
			}
		}
	}
	// 有椅子类型(面板1:常规布局)
	else
	{
		CTileLayoutUI* panel = static_cast<CTileLayoutUI*>(m_pManager->FindControl(R->kTilRoomPanel_Panel_1));
		if (!panel) return;

		//在创建
		SIZE tileSize = { 0, 0 };
		TCHAR tableName[MAX_PATH] = { 0 };
		RECT rcPadding = { 0 };
		for (int index = 0; index < wTableCount; ++index)
		{
			Table* pDesk = new Table(wRoomID, wKindID, index, index, m_pManager, wChairCount, bHundredGame, bHundredLayout);
			if (!pDesk) continue;

			CContainerUI* table_tile = pDesk->GetTableCtl();
			if (table_tile)
			{
				_stprintf_s(tableName, _T("table_%d"), index);
				table_tile->SetName(tableName);
				m_vTableMap[index] = pDesk;

				int table_tile_w = table_tile->GetFixedWidth();
				int table_tile_h = table_tile->GetFixedHeight();
				tileSize.cx = table_tile_w;
				tileSize.cy = table_tile_h;

#ifdef GAMEPLAZA_STYLE_1
				rcPadding.left		= 0;
				rcPadding.top		= 0;
				rcPadding.right		= 0;
				rcPadding.bottom	= 0;
#else if GAMEPLAZA_STYLE_2
				rcPadding.left		= 0;
				rcPadding.top		= table_tile_h / 10;
				rcPadding.right		= 0;
				rcPadding.bottom	= 0;
#endif
				table_tile->SetPadding(rcPadding);
				panel->SetItemSize(tileSize);
				panel->Add(table_tile);
			}
			else
			{
				SAFE_DELETE(pDesk);
			}
		}
	}

	g_Logger->TryInfo(_T("TableManager - CreatDeskPanel Complete"));
}

void TableManager::UpdateTableStatus(tagGS_TableStatus& tStruct)
{
	WORD wTableID = tStruct.wTableID;
	if (m_vTableMap.find(wTableID) != m_vTableMap.end())
	{
		m_vTableMap[wTableID]->SetTableLock(tStruct.bTableLock);
		m_vTableMap[wTableID]->SetPlayStatus(tStruct.bPlayStatus);
		m_vTableMap[wTableID]->UpdateTableStatus();
	}
}

void TableManager::UpdateTableStatus(WORD wTableID, tagTableStatus& tStruct)
{
	if (m_vTableMap.find(wTableID) != m_vTableMap.end())
	{
		m_vTableMap[wTableID]->SetTableLock(tStruct.bTableLock);
		m_vTableMap[wTableID]->SetPlayStatus(tStruct.bPlayStatus);
		m_vTableMap[wTableID]->UpdateTableStatus();
	}
}

Table* TableManager::GetTableByID(WORD wTableID)
{
	Table* pTable = nullptr;
	if (m_vTableMap.find(wTableID) != m_vTableMap.end())
	{
		pTable = m_vTableMap[wTableID];
	}
	return pTable;
}

Table* TableManager::GetTable(UserData& uStruct)
{
	//坐下状态可以通过id获取
	//起立时桌子id以更新为空,因此要获取上次桌子id

	Table* pTable = nullptr;
	pTable = GetTableByID(uStruct.GetTableID());
	if (!pTable)
	{
		pTable = GetTableByID(uStruct.GetLastTableID());
	}

	return pTable;
}

Table* TableManager::GetFreeTable(bool bEmpty /* = false */)
{
	Table* pTable = nullptr;
	for (const auto& it : m_vTableMap)
	{
		if (bEmpty)
		{
			if (it.second->IsEmptyTable())
			{
				pTable = it.second;
				break;
			}
		}
		else
		{
			if (it.second->IsFreeTable())
			{
				pTable = it.second;
				break;
			}
		}
	}

	return pTable;
}

void TableManager::SetFrozenEnable(bool bEnable)
{
	CButtonUI* pJoinBtn = static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnRoomPanel_Join));
	CButtonUI* pBackBtn = static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnRoomPanel_Back));
	if (pJoinBtn && pBackBtn)
	{
		pJoinBtn->SetEnabled(bEnable);
		pBackBtn->SetEnabled(bEnable);
	}

	CTileLayoutUI* pTileLayout = static_cast<CTileLayoutUI*>(m_pManager->FindControl(R->kTilRoomPanel_Panel_1));
	if (pTileLayout)
	{
		pTileLayout->SetMouseEnabled(bEnable);
		pTileLayout->SetMouseChildEnabled(bEnable);
		pTileLayout->SetKeyboardEnabled(bEnable);
	}
}

void TableManager::QuickJoin(Table* pTable, WORD wChairIndex, bool bSitReq /* = true */)
{
	if (!pTable) return;

	// 常规布局
	if (!pTable->IsHundredLayout())
	{
		// 滚动条定位
		CTileLayoutUI* pTileLayout = static_cast<CTileLayoutUI*>(m_pManager->FindControl(R->kTilRoomPanel_Panel_1));
		RECT rcLayout = pTileLayout->GetPos();

		CContainerUI* pTabCtl = pTable->GetTableCtl();
		RECT rcTable = pTabCtl->GetPos();

		if (rcTable.top < rcLayout.top || rcTable.bottom > rcLayout.bottom)
		{
			SIZE pos_size = pTileLayout->GetScrollPos();
			pos_size.cx += 0;
			pos_size.cy += rcTable.top - rcLayout.top;
			pTileLayout->SetScrollPos(pos_size);
		}

		if (!m_pZoomBox)
		{
			m_pZoomBox = new CZoomBoxUI();
		}

		// 先Add，才能调用CPaintManagerUI->SetTimmer
		CContainerUI* pRootCtl = static_cast<CContainerUI*>(m_pManager->GetRoot());
		if (!pRootCtl) return;
		pRootCtl->Add(m_pZoomBox);

		QuickJoinAnimationCallBack cbFunc = CC_CALLBACK_3(TableManager::QuickJoinCallBack, this);
		if (m_pZoomBox->PlayAnimation(cbFunc, pTable, wChairIndex, 40, bSitReq))
		{
			// 禁用桌子面板事件
			this->SetFrozenEnable(false);
		}
		else
		{
			pRootCtl->Remove(m_pZoomBox);
			m_pZoomBox = nullptr;
		}
	}
	// 百人布局
	else
	{
		pTable->SitDownReq(wChairIndex);
	}
}

void TableManager::QuickJoinCallBack(Table* pTable, WORD wChairIndex, bool bSitReq)
{
	CContainerUI* pRootCtl = static_cast<CContainerUI*>(m_pManager->GetRoot());
	if (pRootCtl)
	{
		if (pRootCtl->Remove(m_pZoomBox))
		{
			m_pZoomBox = nullptr;

			// 恢复桌子面板事件
			this->SetFrozenEnable(true);

			if (bSitReq)
			{
				pTable->SitDownReq(wChairIndex);
			}
		}
	}
}

// --------------------------------------------------------------------------
// CZoomBoxUI

CZoomBoxUI::CZoomBoxUI()
	: m_wColor(0xFFFFFFFF)
	, m_dwLastElapse(0)
	, m_fDisSum(0)
	, m_fSpeed(0)
	, m_bSitReq(false)
	, m_pTableRef(nullptr)
	, m_wChairIndex(INVALID_CHAIR)
	, m_cbFunc(nullptr)
{
	::ZeroMemory(&m_srcRect, sizeof(RECT));
	::ZeroMemory(&m_tagRect, sizeof(RECT));
	::ZeroMemory(&m_tmpRect, sizeof(RECT));
}

CZoomBoxUI::~CZoomBoxUI()
{}

LPCTSTR	CZoomBoxUI::GetClass() const
{
	return _T("ZoomBox");
}

LPVOID CZoomBoxUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("ZoomBox")) == 0) return static_cast<CZoomBoxUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CZoomBoxUI::DoInit()
{
	this->SetMouseEnabled(false);
	this->SetKeyboardEnabled(false);
}

void CZoomBoxUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;

	CRenderEngine::DrawRect(hDC, m_tagRect, m_nBorderSize, 0xFFFFFFFF);
}

void CZoomBoxUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
	{
		this->_OnTimer((UINT_PTR)event.wParam, event.dwTimestamp);
	}
}

bool CZoomBoxUI::PlayAnimation(QuickJoinAnimationCallBack cbFunc, Table* pTable, WORD wChairIndex, LONG lOffset, bool bSitReq)
{
	if (!cbFunc || !pTable) return false;

	Chair* pChair = pTable->GetChairByIndex(wChairIndex);
	if (!pChair) return false;

	CButtonUI* pChairCtl = static_cast<CButtonUI*>(m_pManager->FindSubControlByName(pTable->GetTableCtl(), pChair->scName));
	if (!pChairCtl) return false;

	RECT rectPos = pChairCtl->GetPos();
	m_pTableRef = pTable;
	m_bSitReq = bSitReq;
	m_wChairIndex = wChairIndex;
	m_srcRect = rectPos;
	m_tagRect = m_srcRect;
	::InflateRect(&m_tagRect, lOffset, lOffset);
	m_tmpRect = m_tagRect;
	m_fSpeed = lOffset / 200.0f;

	m_cbFunc = cbFunc;

	m_dwLastElapse = ::GetTickCount() + 10;
	m_pManager->SetTimer(this, TIMER_ID_ZOOMBOX, 10);

	return true;
}

void CZoomBoxUI::_OnTimer(UINT_PTR idEvent, DWORD dwElapse)
{
	m_fDisSum += (dwElapse - m_dwLastElapse) * m_fSpeed;
	m_dwLastElapse = dwElapse;
	
	m_tagRect.left = m_tmpRect.left + m_fDisSum;
	m_tagRect.top = m_tmpRect.top + m_fDisSum;
	m_tagRect.right = m_tmpRect.right - m_fDisSum;
	m_tagRect.bottom = m_tmpRect.bottom - m_fDisSum;

	if (m_tagRect.left >= m_srcRect.left
		&& m_tagRect.right <= m_srcRect.right
		&& m_tagRect.top >= m_srcRect.top
		&& m_tagRect.bottom <= m_srcRect.bottom)
	{
		m_pManager->KillTimer(this);

		if (m_pTableRef)
		{
			m_cbFunc(m_pTableRef, m_wChairIndex, m_bSitReq);
		}
	}

	this->Invalidate();
}