#include "stdafx.h"
#include "UIGameItem.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;

CGameItemUI::CGameItemUI()
	: m_iOffset(5)
	, m_uButtonState(0)
	, m_bFirstPadding(true)
	, m_uGameState(GAME_STATE::GAME_STATE_NOT_OPEN)
{
	memset(&m_tStruct, 0, sizeof(tagGameKind));
}

CGameItemUI::~CGameItemUI()
{
}

bool CGameItemUI::less_sort_item_id(const CGameItemUI* a, const CGameItemUI* b)
{
	return a->GetGameKind()->wSortID < b->GetGameKind()->wSortID;
}

bool CGameItemUI::less_sort_type_id(const tagGameType* a, const tagGameType* b)
{
	return a->wSortID < b->wSortID;
}

bool CGameItemUI::less_sort_kind_id(const tagGameKind* a, const tagGameKind* b)
{
	return a->wSortID < b->wSortID;
}

bool CGameItemUI::less_sort_room_id(const tagGameServer* a, const tagGameServer* b)
{
	return a->wSortID < b->wSortID;
}

LPCTSTR CGameItemUI::GetClass() const
{
	return _T("GameItemUI");
}

LPVOID CGameItemUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("GameItem")) == 0) return static_cast<CGameItemUI*>(this);
	return __super::GetInterface(pstrName);
}

UINT CGameItemUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

bool CGameItemUI::Activate()
{
	if (!CControlUI::Activate()) return false;
	if (m_pManager != NULL)
		m_pManager->SendNotify(this, DUI_MSGTYPE_GAMEITEM_CLICK);

	return true;
}

void CGameItemUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
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

	if (event.Type == UIEVENT_SETFOCUS)
	{
		Invalidate();
	}
	if (event.Type == UIEVENT_KILLFOCUS)
	{
		Invalidate();
	}
	if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
	{
		if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) {
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			Invalidate();
		}
		return;
	}
	if (event.Type == UIEVENT_MOUSEMOVE)
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
			if (::PtInRect(&m_rcItem, event.ptMouse)) m_uButtonState |= UISTATE_PUSHED;
			else m_uButtonState &= ~UISTATE_PUSHED;
			Invalidate();
		}
		return;
	}
	if (event.Type == UIEVENT_BUTTONUP)
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
			if (::PtInRect(&m_rcItem, event.ptMouse)) Activate();
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
		}
		return;
	}
	if (event.Type == UIEVENT_MOUSEENTER)
	{
		RECT pos = GetPos();
		DWORD iw = pos.right - pos.left;
		DWORD ih = pos.bottom - pos.top;

		m_rcTextPadding.top -= m_iOffset;
		m_rcTextPadding.bottom += m_iOffset;

		::ZeroMemory(m_pzModify, sizeof(TCHAR));
		_stprintf_s(m_pzModify, _T("source='0,0,0,0' dest='%d,%d,%d,%d'"), 0, 0 - m_iOffset, iw, ih - m_iOffset);
		Invalidate();
	}
	if (event.Type == UIEVENT_MOUSELEAVE)
	{
		m_rcTextPadding.top += m_iOffset;
		m_rcTextPadding.bottom -= m_iOffset;

		::ZeroMemory(m_pzModify, MAX_PATH * sizeof(TCHAR));
		Invalidate();
	}
	if (event.Type == UIEVENT_SETCURSOR) {
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return;
	}

	__super::DoEvent(event);
}

void CGameItemUI::PaintStatusImage(HDC hDC)
{
	this->DrawImage(hDC, m_pzImg.GetData(), m_pzModify);
}

void CGameItemUI::PaintText(HDC hDC)
{
	if (m_sText.IsEmpty()) return;

	if (m_bFirstPadding)
	{
		m_bFirstPadding = false;
		m_rcTextPadding = { 0, 105, 0, 0 };
	}
	
	if (m_dwTextColor == 0) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if (m_dwDisabledTextColor == 0) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
	
	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
		m_iFont, DT_SINGLELINE | m_uTextStyle);
}

bool CGameItemUI::InitData(const tagGameKind* pStruct, DWORD dwTextcolor)
{
	ASSERT(pStruct != nullptr);
	if (!pStruct) return false;

	memcpy(&m_tStruct, pStruct, sizeof(tagGameKind));
	m_pzNoExtension = g_GameUtil->GetFileNameWithoutExtension(m_tStruct.szClientModuleName).c_str();
	m_pzConbinedFileName.Format(_T("%s\\%s"), m_pzNoExtension.GetData(), m_tStruct.szClientModuleName);
	
	CDuiString pzName;
	pzName.Format(_T("GameItem_%u"), m_tStruct.wKindID);
	SetName(pzName);

	CDuiString fileStr = CPaintManagerUI::GetResourcePath();
	m_pzImg.Format(_T("hall\\gameitem\\%s.png"), GetName().GetData());
#ifdef SKIN_RES_TEST
	if (FALSE == g_GameUtil->IsFileExist(fileStr + m_pzImg))
#else
	if(!CRenderEngine::IsZipItemExist(m_pzImg))
#endif //SKIN_RES_TEST
	{
		// 若不存在,则读取默认图标
		m_pzImg.Format(_T("hall\\gameitem\\GameItem_0.png"));
	}
	
	//字体颜色
	this->SetText(m_tStruct.szKindName);
	this->SetTextColor(dwTextcolor);
	this->SetFont(3);
	this->SetAlign(_T("center"));

	//开启文件检测(确保图片文件存在)
	bool bRes = false;
#ifdef GAMEPLAZA_STYLE_1
	#ifdef SKIN_RES_TEST
		bRes = TRUE == g_GameUtil->IsFileExist(fileStr + m_pzImg);
	#else
		bRes = CRenderEngine::IsZipItemExist(m_pzImg);
	#endif
#else if GAMEPLAZA_STYLE_2
	bRes = true;
#endif
	return bRes;
}

void CGameItemUI::SetGameStatus(GAME_STATE uGameState)
{
	m_uGameState = uGameState;
	switch (m_uGameState)
	{
		case DuiLib::CGameItemUI::GAME_STATE_NOT_OPEN:
		{
			m_pzStatusDesc = _T("暂未开放，敬请期待!");
		}break;

		case DuiLib::CGameItemUI::GAME_STATE_NOT_EXIST:
		{
			m_pzStatusDesc.Format(_T("【%s%s】还没有安装,现在是否下载?"), _T(""), m_tStruct.szKindName);
		}break;

		case DuiLib::CGameItemUI::GAME_STATE_VERSION_ERROE:
		{
			CDuiString cstrTemp;
			CDuiString cstrVersion;
			cstrTemp.Format(_T("%d"), m_tStruct.dwMaxVersion);
			for (int i = 0; i < cstrTemp.GetLength(); ++i)
			{
				cstrVersion += cstrTemp[i];
				if (i < cstrTemp.GetLength() - 1)
				{
					cstrVersion += _T(".");
				}
			}

			m_pzStatusDesc.Format(_T("【%s】已更新至v%s版本,您的版本不能继续使用,现在是否下载?"), m_tStruct.szKindName, cstrVersion);
		}break;

		case DuiLib::CGameItemUI::GAME_STATE_VERSION_NORMAL:
		{
			m_pzStatusDesc = _T("");
		}break;

		default:
			break;
	}
}