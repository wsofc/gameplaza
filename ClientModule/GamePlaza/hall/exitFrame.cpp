#include "stdafx.h"
#include "exitFrame.h"
#include "loginFrame.h"
#include "hallFrame.h"
#include "util/gameUtil.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

ExitFrame* ExitFrame::g_instance = nullptr;

ExitFrame::ExitFrame()
	: m_iCloseEvent(CLOSE_EVENT::CLOSE_EVENT_NONE)
	, m_pWkeCtl(nullptr)
{
}

ExitFrame::~ExitFrame()
{
	HallFrame* pHallFrame = HallFrame::GetInstance();
	CPaintManagerUI* pHallPaintManager = pHallFrame->GetPaintManager();
	g_GameUtil->SetFrozenEnable(pHallPaintManager, true);
	g_GameUtil->ShowTopWindow(pHallPaintManager->GetPaintWindow());

	switch (m_iCloseEvent)
	{
		case ExitFrame::CLOSE_EVENT_NONE:
			break;
		case ExitFrame::CLOSE_EVENT_SWITCH:
		{
			g_Logger->TryInfo(_T("ExitFrame - Switch to Login"));
			pHallFrame->SwitchToLogin();
		}break;
		case ExitFrame::CLOSE_EVENT_CANCEL:
			break;
		case ExitFrame::CLOSE_EVENT_EXIT:
		{
			pHallFrame->Close(CMW_CLOSE_SURE);
		}break;
		case ExitFrame::CLOSE_EVENT_EXTRA:
			break;
		default:
			break;
	}

	g_Logger->TryInfo(_T("ExitFrame - Destroy"));
}

ExitFrame* ExitFrame::GetInstance()
{
	if (ExitFrame::g_instance == NULL)
		ExitFrame::g_instance = new (std::nothrow)ExitFrame();

	return ExitFrame::g_instance;
}

BOOL ExitFrame::CreateFrame()
{
	if (FALSE == IsWindow(m_hWnd))
	{
		// 恢复大厅主窗口
		HallFrame* pHallFrame = HallFrame::GetInstance();
		CPaintManagerUI* pManager = pHallFrame->GetPaintManager();
		HWND hParentWnd = pManager->GetPaintWindow();
		if (TRUE == ::IsIconic(hParentWnd))
		{
			::SendMessage(hParentWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		pHallFrame->ShowWindow(true);

		// 创建退出窗口
		g_GameUtil->SetFrozenEnable(pManager, false);
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

UILIB_RESOURCETYPE ExitFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR ExitFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString ExitFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString ExitFrame::GetSkinFile()
{
	return _T("exit.xml");
}

void ExitFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("ExitFrame - Close"));
	__super::OnFinalMessage(hWnd);
	
	SAFE_DELETE(ExitFrame::g_instance);
};

CControlUI* ExitFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
		return  new CWkeWebkitUI();

	return NULL;
}

void ExitFrame::InitWindow()
{
	m_pWkeCtl = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWebExit_Ad));
	if (m_pWkeCtl)
	{
		LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("AdWeb_Exit"));
		m_pWkeCtl->Navigate(lpUrl);

		m_pWkeCtl->SetClientCallback(this);
	}
}

void ExitFrame::InitData(TNotifyUI& msg)
{
	//do sth(eg: load data)

}

void ExitFrame::OnWkeTitleChange(const CDuiString& strTitle)
{
}

void ExitFrame::OnWkeUrlChange(const CDuiString& strUrl)
{
	if (m_pWkeCtl->IsClickURL())
	{
		m_pWkeCtl->ResetClickURL();
		wkeWebView pWebView = m_pWkeCtl->GetWebView();
		pWebView->stopLoading();
		pWebView->loadURL(m_pWkeCtl->GetUrl().GetData());
		g_GameUtil->OpenWebByURL(strUrl.GetData());
	}
}

void ExitFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnExit_Switch) == 0)
		{
			m_iCloseEvent = CLOSE_EVENT::CLOSE_EVENT_SWITCH;
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnExit_Cancel) == 0)
		{
			m_iCloseEvent = CLOSE_EVENT::CLOSE_EVENT_CANCEL;
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnExit_Exit) == 0)
		{
			m_iCloseEvent = CLOSE_EVENT::CLOSE_EVENT_EXIT;
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnExit_Close) == 0)
		{
			m_iCloseEvent = CLOSE_EVENT::CLOSE_EVENT_CANCEL;
			this->Close();
		}
	}

	//__super::Notify(msg);
}

LRESULT ExitFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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
