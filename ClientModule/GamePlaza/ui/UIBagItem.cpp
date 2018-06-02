#include "stdafx.h"
#include "UIBagItem.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;

// ------------------------------------------------------------------------------------------------------
// CBagTipsWnd
CBagTipsWnd* CBagItemUI::g_pTipsFrame = nullptr;
CBagTipsWnd::CBagTipsWnd()
	: m_pTxtGoodsName(nullptr)
	, m_pLabGoodsIcon(nullptr)
	, m_pTxtGoodsDesc(nullptr)
	, m_pTxtGoodsPrice(nullptr)
{
	m_cxyFixed = { 0 };
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

CBagTipsWnd::~CBagTipsWnd()
{

}

void CBagTipsWnd::InitWindow()
{
	m_pTxtGoodsName = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMall_GoodsName));
	m_pLabGoodsIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabMall_GoodsIcon));
	m_pTxtGoodsDesc = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMall_GoodsDesc));
	m_pTxtGoodsPrice = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMall_GoodsPrice));
}

void CBagTipsWnd::FillGoodsData(tagPropsInfo* pGoods)
{
	ASSERT(pGoods);
	if (!pGoods) return;

	m_pTxtGoodsName->SetText(pGoods->szName);

	_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), pGoods->wPropsID);
	m_pLabGoodsIcon->SetBkImage(m_pzBuffer);
	m_pTxtGoodsDesc->SetText(pGoods->szDescribe);

	_stprintf_s(m_pzBuffer, _T("购买单价:%ld 金币"), pGoods->lPrice);
	m_pTxtGoodsPrice->SetText(m_pzBuffer);
}

BOOL CBagTipsWnd::CreateFrame(CPaintManagerUI* pManager)
{
	ASSERT(pManager);
	if (!pManager) return FALSE;

	if (FALSE == IsWindow(m_hWnd))
	{
		m_hWnd = this->Create(pManager->GetPaintWindow(), NULL, UI_CLASSSTYLE_DIALOG, 0L);
	}

	return TRUE;
}

SIZE CBagTipsWnd::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = { 0 };
	SIZE disXY = { 0 };
	SIZE fixed_xy = { 0 };
	CContainerUI* pRoot = static_cast<CContainerUI*>(m_PaintManager.GetRoot());
	for (int it = 0; it < pRoot->GetCount(); ++it)
	{
		CControlUI* pControl = static_cast<CControlUI*>(pRoot->GetItemAt(it));
		if (!pControl->IsVisible()) continue;

		// 若有扩充高度，则依次下移后面的控件
		if (disXY.cy > 0)
		{
			fixed_xy = pControl->GetFixedXY();
			fixed_xy.cy += disXY.cy;
			pControl->SetFixedXY(fixed_xy);
		}

		CLabelUI* pLab = (CLabelUI*)pControl->GetInterface(DUI_CTR_LABEL);
		if (pLab && pLab->GetText().GetLength() > 0)
		{
			RECT rcText = { 0, 0, max(szAvailable.cx, 9999), max(szAvailable.cy, 9999) };
			rcText.left += pLab->GetTextPadding().left;
			rcText.right -= pLab->GetTextPadding().right;

			CDuiString strText = pLab->GetText();
			DWORD iTextColor = pLab->GetTextColor();
			int nFont = pLab->GetFont();
			UINT uTextStyle = pLab->GetTextStyle();
			CRenderEngine::DrawText(m_PaintManager.GetPaintDC(), &m_PaintManager, rcText, strText.GetData(), iTextColor, nFont, DT_CALCRECT | uTextStyle);

			LONG lw = rcText.right - rcText.left + pLab->GetTextPadding().left + pLab->GetTextPadding().right;
			LONG lh = rcText.bottom - rcText.top + pLab->GetTextPadding().top + pLab->GetTextPadding().bottom;
			if (lw > pLab->GetFixedWidth())
			{
				LONG fixed_x = pLab->GetFixedWidth();
				LONG fixed_y = lh;
				LONG lCount = ceil((DOUBLE)lh / fixed_y);
				lCount *= ceil((DOUBLE)lw / fixed_x);
				LONG H = lCount * fixed_y - (lCount - 1) * (pLab->GetTextPadding().top + pLab->GetTextPadding().bottom);
				pLab->SetFixedHeight(H);
				disXY.cy += H - fixed_y;
			}
			else
			{
				pLab->SetFixedWidth(lw);
			}
		}
		SIZE sz = pControl->EstimateSize(szAvailable);
		cXY.cy += sz.cy;
		if (cXY.cx < sz.cx)
		{
			cXY.cx = sz.cx;
		}
	}

	if (cXY.cy < m_cxyFixed.cy)
	{
		cXY.cy = m_cxyFixed.cy;
	}
	if (cXY.cx < m_cxyFixed.cx)
	{
		cXY.cx = m_cxyFixed.cx;
	}

	m_cxyFixed.cy = cXY.cy;
	m_cxyFixed.cx = cXY.cx;
	return cXY;
}

void CBagTipsWnd::ResizeShow(RECT rc)
{
#if defined(WIN32) && !defined(UNDER_CE)
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
#else
	CDuiRect rcWork;
	::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif

	// 计算绘制所占总大小
	m_cxyFixed = { 0 };
	SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = this->EstimateSize(szAvailable);

	// 这里加上布局时候，控件之间的间距
	// 得出最终窗口总大小
	szAvailable.cx += 18;
	szAvailable.cy += 14;

	// 适配窗口位置
	// 获得工作区大小
	RECT rcDeskTop = { 0 };
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDeskTop, 0);
	LONG dis = 5;
	RECT rcPos = rc;
	LONG w = rc.right - rc.left;
	LONG h = rc.bottom - rc.top;
	if (rcPos.top - szAvailable.cy - dis < rcDeskTop.top)
	{
		rcPos.top = rc.bottom + dis;
		rcPos.bottom = rcPos.top + h;
	}
	else
	{
		rcPos.top = rc.top - szAvailable.cy - dis;
	}

	if (rcPos.left + szAvailable.cx > rcDeskTop.right)
	{
		rcPos.left = rc.left - szAvailable.cx;
		rcPos.left += w;
	}
	else if (rcPos.left < rcDeskTop.left)
	{
		rcPos.left = rc.right + dis;
		rcPos.right = rcPos.left + w;

		rcPos.top = rc.top;
		rcPos.bottom = rcPos.top + h;
	}

	if (rcPos.right > rcDeskTop.right)
	{
		rcPos.left = rc.left - szAvailable.cx - dis;
		rcPos.right = rcPos.left + w;

		rcPos.top = rc.top;
		rcPos.bottom = rcPos.top + h;
	}

	::SetWindowPos(m_hWnd, HWND_TOPMOST, rcPos.left, rcPos.top, szAvailable.cx, szAvailable.cy, SWP_SHOWWINDOW);
	this->ShowWindow(true);
}

UILIB_RESOURCETYPE CBagTipsWnd::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR CBagTipsWnd::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString CBagTipsWnd::GetSkinFolder()
{
	return _T("");
}

CDuiString CBagTipsWnd::GetSkinFile()
{
	return _T("hall\\mall\\tips.xml");
}

LPCTSTR CBagTipsWnd::GetWindowClassName(void) const
{
	return _T("CBagTipsWnd");
}

// ------------------------------------------------------------------------------------------------------
// CBagTileLayout
CBagTileLayout::CBagTileLayout()
{

}

CBagTileLayout::~CBagTileLayout()
{
}

// ------------------------------------------------------------------------------------------------------
// CBagItemUI
CBagItemUI::CBagItemUI()
{
}

CBagItemUI::~CBagItemUI()
{
}

LPCTSTR CBagItemUI::GetClass() const
{
	return _T("BagItemUI");
}

LPVOID CBagItemUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("BagItem")) == 0) return static_cast<CBagItemUI*>(this);
	return CButtonUI::GetInterface(pstrName);
}

UINT CBagItemUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

LPCTSTR CBagItemUI::GetBackgroundImg()
{
	return m_sBackgroundImage;
}

void CBagItemUI::SetBackgroundImg(LPCTSTR pStrImage)
{
	if (m_sBackgroundImage == pStrImage) return;

	m_sBackgroundImage = pStrImage;
	
	Invalidate();
}

void CBagItemUI::SetGoodsData(tagPropsInfo* pGoods)
{
	m_pProps = pGoods;
}

void CBagItemUI::ShowTips(bool bVisible /* = true */)
{
	if (!m_sBkImage.IsEmpty())
	{
		if (bVisible)
		{
			POINT pt_lt = { m_rcItem.left, m_rcItem.top };
			POINT pt_rb = { m_rcItem.right, m_rcItem.bottom };
			::ClientToScreen(m_pManager->GetPaintWindow(), &pt_lt);
			::ClientToScreen(m_pManager->GetPaintWindow(), &pt_rb);

			if (!g_pTipsFrame)
			{
				g_pTipsFrame = new CBagTipsWnd();
			}

			g_pTipsFrame->CreateFrame(this->GetManager());
			g_pTipsFrame->FillGoodsData(m_pProps);
			g_pTipsFrame->ResizeShow({ pt_lt.x, pt_lt.y, pt_rb.x, pt_rb.y });
		}
		else
		{
			if (g_pTipsFrame && ::IsWindow(g_pTipsFrame->GetHWND()))
			{
				g_pTipsFrame->Close();
			}
		}
	}
}

tagPropsInfo* CBagItemUI::GetGoods()
{
	return m_pProps;
}

void CBagItemUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
		{
			m_pParent->DoEvent(event);
		}
		else
		{
			__super::DoEvent(event);
		}
		return;
	}
	
	if (event.Type == UIEVENT_MOUSEENTER)
	{
		this->ShowTips(true);
	}
	
	if (event.Type == UIEVENT_MOUSELEAVE)
	{
		this->ShowTips(false);
	}

	if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
	{
		this->ShowTips(false);
	}

	if (event.Type == UIEVENT_SETCURSOR)
	{
		//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return;
	}

	__super::DoEvent(event);
}

void CBagItemUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;

	PaintBackground(hDC);
	CButtonUI::DoPaint(hDC, rcPaint);
}

void CBagItemUI::PaintBackground(HDC hDC)
{
	if (m_sBackgroundImage.IsEmpty()) return;
	if (!DrawImage(hDC, (LPCTSTR)m_sBackgroundImage)) m_sBackgroundImage.Empty();
}

// ------------------------------------------------------------------------------------------------------
// CBagBoxUI
CBagBoxUI::CBagBoxUI()
{
}

CBagBoxUI::~CBagBoxUI()
{
}

LPCTSTR CBagBoxUI::GetClass() const
{
	return _T("BagBoxUI");
}

LPVOID CBagBoxUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("BagBox")) == 0) return static_cast<CBagBoxUI*>(this);
	return CButtonUI::GetInterface(pstrName);
}

UINT CBagBoxUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CBagBoxUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
		{
			m_pParent->DoEvent(event);
		}
		else
		{
			__super::DoEvent(event);
		}
		return;
	}

	if (event.Type == UIEVENT_MOUSEENTER)
	{
		if (IsEnabled())
		{
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}

	if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
	{
		this->ShowTips(true);

		if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled())
		{
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			Invalidate();
		}
		return;
	}

	if (event.Type == UIEVENT_RBUTTONDOWN)
	{
		this->ShowTips(false);
		return;
	}

	if (event.Type == UIEVENT_RBUTTONUP)
	{
		this->ShowMenu();
		return;
	}

	if (event.Type == UIEVENT_SETCURSOR)
	{
		//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return;
	}

	__super::DoEvent(event);
}

void CBagBoxUI::ShowMenu()
{
	if (!m_sBkImage.IsEmpty())
	{
		POINT p;
		::GetCursorPos(&p);
		::CPoint point(p.x, p.y);

		CMenuWnd* pMenuWnd = new CMenuWnd();
		pMenuWnd->Init(NULL, _T("hall\\mall\\useMenu.xml"), _T(""), point, m_pManager, &m_MenuCheckInfo, this, eMenuAlignment_Right | eMenuAlignment_Bottom);
		CMenuUI* pMenuUI = pMenuWnd->GetMenuUI();
		for (size_t i = 0; i < pMenuUI->GetCount(); ++i)
		{
			pMenuUI->GetItemAt(i)->SetFixedWidth(50);
		}
		pMenuWnd->Show();
	}
}
