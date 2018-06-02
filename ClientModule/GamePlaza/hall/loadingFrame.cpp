#include "stdafx.h"
#include "loadingFrame.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

LoadingFrame::LoadingFrame()
	: m_progress(nullptr)
	, m_iStep(0)
	, m_iRatio(0)
{
	if (0 == IsWindow(m_hWnd))
	{
		int style = UI_CLASSSTYLE_DIALOG | WS_MINIMIZEBOX;
		m_hWnd = this->Create(NULL, R->MainWndTitleName, style, 0L);

		this->CenterWindow();
		::ShowWindow(*this, SW_NORMAL);
	}
	else
	{
		this->ShowWindow(true);
	}
}

LoadingFrame::~LoadingFrame()
{
	g_Logger->TryInfo(_T("LoadingFrame - Destroy"));
}

CDuiString LoadingFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString LoadingFrame::GetSkinFile()
{
	return _T("loading.xml");
}

UILIB_RESOURCETYPE LoadingFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR LoadingFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

LRESULT LoadingFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return FALSE;
	}
	return FALSE;
}

void LoadingFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("LoadingFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void LoadingFrame::InitWindow()
{
	g_Logger->TryInfo(_T("LoadingFrame - Open"));

	m_progress = static_cast<CProgressUI*>(m_PaintManager.FindControl(R->kProLoading_Progress));
	m_progress->SetValue(20);

	g_Logger->TryInfo(_T("LoadingFrame - Open Complete"));
}

void LoadingFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0);
	else if (_tcscmp(type, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnLoading_Close) == 0)
		{
			g_GameUtil->EndHall();
		}
	}
}

void LoadingFrame::AddPercent()
{
	int iValue = m_progress->GetValue();
	iValue += m_iRatio;
	m_progress->SetValue(iValue);
}

void LoadingFrame::SetPercent(int iPercent)
{
	m_progress->SetValue(iPercent);
}