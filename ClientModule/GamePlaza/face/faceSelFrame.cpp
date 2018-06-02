#include "stdafx.h"
#include "faceSelFrame.h"

FaceSelFrame::FaceSelFrame()
	: m_pFaceCtrl(nullptr)
	, m_lpFaceList(nullptr)
	, m_nSelFaceId(-1)
	, m_nSelFaceIndex(-1)
	, m_strSelFaceFileName(_T(""))
	, m_pFuncCtrlSel(nullptr)
{
}

FaceSelFrame::~FaceSelFrame(void)
{
}

void FaceSelFrame::CreateFrame(HWND parenWnd, RECT& rcPos, CFaceSelList * lpFaceList, ProcFaceCtrlSel proCtrlSel)
{
	m_lpFaceList = lpFaceList;
	m_pFuncCtrlSel = proCtrlSel;

	if (FALSE == ::IsWindow(m_hWnd))
	{
		this->Create(parenWnd, _T("FaceSelFrame"), WS_CHILD | WS_POPUP, WS_EX_TOOLWINDOW, 0L);
		::ClientToScreen(parenWnd, (LPPOINT)&rcPos);

		int cx = 432;
		int cy = 236;
		int x = rcPos.left - cx / 2;
		int y = rcPos.top - cy;

		::SetWindowPos(m_hWnd, NULL, x, y, cx, cy, NULL);
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}
}

UILIB_RESOURCETYPE FaceSelFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR FaceSelFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString FaceSelFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString FaceSelFrame::GetSkinFile()
{
	return _T("face/FaceSelFrame.xml");
}


void FaceSelFrame::InitWindow()
{
	m_pFaceCtrl = static_cast<CFaceSelUI*>(m_PaintManager.FindControl(_T("FaceCtrl1")));

	//m_pFaceCtrl->SetBgColor(RGB(255, 255, 255));
	m_pFaceCtrl->SetLineColor(RGB(223, 230, 246));
	m_pFaceCtrl->SetFocusBorderColor(RGB(0, 0, 255));
	m_pFaceCtrl->SetZoomBorderColor(RGB(0, 138, 255));
	m_pFaceCtrl->SetRowAndCol(8, 15);
	m_pFaceCtrl->SetItemSize(28, 28);
	m_pFaceCtrl->SetZoomSize(84, 84);
	m_pFaceCtrl->SetFaceList(m_lpFaceList);
	m_pFaceCtrl->SetCurPage(0);

	m_nSelFaceId = -1;
	m_nSelFaceIndex = -1;
	m_strSelFaceFileName = _T("");
}

CControlUI* FaceSelFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FaceCtrl")) == 0)
	{
		return new CFaceSelUI();
	}
	
	return NULL;
}

LRESULT FaceSelFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void FaceSelFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT FaceSelFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ACTIVATE)
	{
		if (WA_INACTIVE == (UINT)LOWORD(wParam))
		{
			// 先发送 WM_CLOSE 消息,因为 WA_INACTIVE 会同时触发多条
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
		}
	}
	else if (uMsg == WM_CLOSE)
	{
		// WM_CLOSE 消息只有一条,这里只调用一次
		if (this->m_pFuncCtrlSel)
		{
			this->m_pFuncCtrlSel(this->GetSelFaceId(), this->GetSelFaceIndex(), this->GetSelFaceFileName().c_str());
		}
	}
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT FaceSelFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

int FaceSelFrame::GetSelFaceId()
{
	return m_nSelFaceId;
}

int FaceSelFrame::GetSelFaceIndex()
{
	return m_nSelFaceIndex;
}

tstring FaceSelFrame::GetSelFaceFileName()
{
	return m_strSelFaceFileName;
}

void FaceSelFrame::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (msg.pSender == m_pFaceCtrl)
		{
			int nSelIndex = (int)msg.lParam;
			CFaceInfo * lpFaceInfo = m_pFaceCtrl->GetFaceInfo(nSelIndex);
			if (lpFaceInfo != NULL)
			{
				m_nSelFaceId = lpFaceInfo->m_nId;
				m_nSelFaceIndex = lpFaceInfo->m_nIndex;
				m_strSelFaceFileName = lpFaceInfo->m_strFileName;
			}
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
		}
	}
}