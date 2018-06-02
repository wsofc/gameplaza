#include "stdafx.h"
#include "UIControlGdiplus.h"

namespace DuiLib {

	CControlGdiplusUI::CControlGdiplusUI()
		: m_uButtonState(0)
	{ }

	CControlGdiplusUI::~CControlGdiplusUI()
	{ }

	LPCTSTR CControlGdiplusUI::GetClass() const
	{
		return DUI_CTR_CONTROLGDIPLUS;
	}

	LPVOID CControlGdiplusUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_CONTROLGDIPLUS) == 0) return static_cast<CControlGdiplusUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	UINT CControlGdiplusUI::GetControlFlags() const
	{
		return UIFLAG_SETCURSOR;
	}

	bool CControlGdiplusUI::Activate()
	{
		if (!CControlUI::Activate()) return false;
		if (m_pManager != NULL)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		}
		return true;
	}

	bool CControlGdiplusUI::RActivate()
	{
		if (!CControlUI::Activate()) return false;
		if (m_pManager != NULL)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_RCLICK);
		}
		return true;
	}

	void CControlGdiplusUI::DoEvent(TEventUI& event)
	{
		if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
			if (m_pParent != NULL) m_pParent->DoEvent(event);
			else CControlUI::DoEvent(event);
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
		if (event.Type == UIEVENT_KEYDOWN)
		{
			if (IsKeyboardEnabled())
			{
				if ( /*event.chKey == VK_SPACE ||*/ event.chKey == VK_RETURN)
				{
					Activate();
					return;
				}
			}
		}
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN)
		{
			if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled())
			{
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0)
			{
				if (::PtInRect(&m_rcItem, event.ptMouse)) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_BUTTONUP)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0)
			{
				if (::PtInRect(&m_rcItem, event.ptMouse)) Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_RBUTTONUP)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0)
			{
				if (::PtInRect(&m_rcItem, event.ptMouse)) RActivate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_CONTEXTMENU)
		{
			if (IsContextMenuUsed())
			{
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
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
			// return;
		}
		if (event.Type == UIEVENT_MOUSELEAVE)
		{
			if (IsEnabled())
			{
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			// return;
		}
		if (event.Type == UIEVENT_SETCURSOR)
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		CControlUI::DoEvent(event);
	}

	void CControlGdiplusUI::PaintBkImage(HDC hDC)
	{
		if (m_sBkImage.IsEmpty()) return;
		//return CContainerUI::PaintBkImage(hDC);

		const TImageInfo* data = NULL;
		if (m_sBkImage.IsEmpty())
		{
			data = m_pManager->GetImageEx((LPCTSTR)m_sBkImage, NULL, 0);
		}
		else
		{
			data = m_pManager->GetImageEx((LPCTSTR)m_sBkImage, (LPCTSTR)0, 0);
		}
		if (!data) return;

		int iw = m_rcItem.right - m_rcItem.left;
		int ih = m_rcItem.bottom - m_rcItem.top;

		Gdiplus::Bitmap cur_bitmap(data->hBitmap, NULL);
		Gdiplus::Graphics graphics(hDC);

		Gdiplus::RectF rcSrcF(0, 0, data->nX, data->nY);
		Gdiplus::RectF rcDestF(m_rcItem.left, m_rcItem.top, iw, ih);

		// ������С�ȱ�����
		if (true)
		{
			float fScaleW = rcDestF.Width / (float)rcSrcF.Width;
			float fScaleH = rcDestF.Height / (float)rcSrcF.Height;
			float fScale = min(fScaleW, fScaleH);

			rcDestF.Width = rcSrcF.Width * fScale;
			rcDestF.Height = rcSrcF.Height * fScale;
		}

		// ����
		rcDestF.X = m_rcItem.left + (iw - rcDestF.Width) / 2;
		rcDestF.Y = m_rcItem.top + (ih - rcDestF.Height) / 2;

		// ����λͼ��ͼԪ�ļ���ɫ����
		Gdiplus::ImageAttributes imAtt;

		// ˮƽ�ʹ�ֱ��ת����򽥱�,Ȼ��ƽ�̸�����򽥱�
		imAtt.SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY);

		// ȥ��ָ������ɫ
		imAtt.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0), Gdiplus::ColorAdjustTypeBitmap);

		// ͸������
		//Gdiplus::ColorMatrix colorMatrix = {
		//	0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
		//	0.59f, 0.59f, 0.59f, 0.0f, 0.0f,
		//	0.11f, 0.11f, 0.11f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 0.0f, 0.3f, 0.0f,
		//	0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		//imAtt.SetColorMatrix(&colorMatrix);

		// ����������
		graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);

		// �����
		graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);

		Gdiplus::GraphicsState transState = graphics.Save();

		graphics.DrawImage(&cur_bitmap
			, rcDestF
			, rcSrcF.X
			, rcSrcF.Y
			, rcSrcF.Width
			, rcSrcF.Height
			, Gdiplus::UnitPixel
			, &imAtt
			);

		graphics.Restore(transState);
	}

	void CControlGdiplusUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if (!IsEnabled()) {
			m_uButtonState = 0;
		}
	}

	void CControlGdiplusUI::RestState(UINT state /* = 0 */)
	{
		m_uButtonState = state;

		Invalidate();
	}

} //namespace DuiLib
