#include "stdafx.h"
#include "activity.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

ActivityFrame* ActivityFrame::g_instance = nullptr;
ActivityFrame::ActivityFrame()
	: m_pParentManager(nullptr)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

ActivityFrame::~ActivityFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);
	g_GameUtil->ShowTopWindow(m_pParentManager->GetPaintWindow(), true, true);

	g_Logger->TryInfo(_T("ActivityFrame - Destroy"));
}

ActivityFrame* ActivityFrame::GetInstance()
{
	if (ActivityFrame::g_instance == nullptr)
	{
		ActivityFrame::g_instance = new ActivityFrame();
	}

	return ActivityFrame::g_instance;
}

BOOL ActivityFrame::CreateFrame(CPaintManagerUI* pParentManager)
{
	ASSERT(pParentManager);
	if (!pParentManager) return FALSE;

	m_pParentManager = pParentManager;
	if (FALSE == ::IsWindow(m_hWnd))
	{
		HWND hParentWnd = m_pParentManager->GetPaintWindow();
		g_GameUtil->SetFrozenEnable(m_pParentManager, false);
		m_hWnd = this->Create(hParentWnd, NULL, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}

	return TRUE;
}

CDuiString ActivityFrame::GetSkinFile()
{
	return _T("hall\\activity\\activity.xml");
}

CDuiString ActivityFrame::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE ActivityFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR ActivityFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

LRESULT ActivityFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == CMW_CLOSE_SURE)
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	g_GameUtil->MinimizeTray(*this);
	return 0;
}

void ActivityFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("ActivityFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(ActivityFrame::g_instance);
};

CControlUI* ActivityFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	{
		return new CWkeWebkitUI();
	}

	return NULL;
}

void ActivityFrame::InitWindow()
{
	g_Logger->TryInfo(_T("ActivityFrame - Open"));
	m_pTabLaout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTabActivity_Switch));
}

void ActivityFrame::InitData(TNotifyUI& msg)
{
	COptionUI* pOpt1 = static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptActivity_Op_1));
	if (pOpt1)
	{
		pOpt1->Selected(true);
	}
}

void ActivityFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnActivity_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_SELECTCHANGED) == 0)
	{
		if (_tcscmp(sName, R->kOptActivity_Op_1) == 0 && m_pTabLaout)
		{
			m_pTabLaout->SelectItem(0);

			// º”‘ÿÕ¯“≥
			CWkeWebkitUI* pWeb = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeActivity_Web1));
			if (pWeb)
			{
				DataManager* pDataManager = DataManager::GetInstance();
				UserData& tSelf = pDataManager->GetSelfData();
				LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("Sign"));
				_stprintf_s(m_pzBuffer, _T("%s%lu"), lpUrl, tSelf.GetUserID());
				pWeb->Navigate(m_pzBuffer);
			}
		}
		else if (_tcscmp(sName, R->kOptActivity_Op_2) == 0 && m_pTabLaout)
		{
			m_pTabLaout->SelectItem(1);

			// º”‘ÿÕ¯“≥
			CWkeWebkitUI* pWeb = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeActivity_Web2));
			if (pWeb)
			{
				DataManager* pDataManager = DataManager::GetInstance();
				UserData& tSelf = pDataManager->GetSelfData();
				LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("Dole"));
				_stprintf_s(m_pzBuffer, _T("%s%lu"), lpUrl, tSelf.GetUserID());
				pWeb->Navigate(m_pzBuffer);
			}
		}
	}
	//__super::Notify(msg);
}

LRESULT ActivityFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (m_PaintManager.GetRoot()->IsKeyboardEnabled())
	{
		if (wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			this->Close(CMW_CLOSE_SURE);
			return TRUE;
		}
	}
	return FALSE;
}