#include "stdafx.h"
#include "doleFrame.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

DoleFrame* DoleFrame::g_instance = nullptr;
DoleFrame::DoleFrame()
	: m_pParentManager(nullptr)
	, m_pWkeCtl(nullptr)
{
}

DoleFrame::~DoleFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);
	g_GameUtil->ShowTopWindow(m_pParentManager->GetPaintWindow(), true, true);

	g_Logger->TryInfo(_T("DoleFrame - Destroy"));
}

DoleFrame* DoleFrame::GetInstance()
{
	if (DoleFrame::g_instance == nullptr)
	{
		DoleFrame::g_instance = new DoleFrame();
	}

	return DoleFrame::g_instance;
}

BOOL DoleFrame::CreateFrame(CPaintManagerUI* pParentManager)
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

CDuiString DoleFrame::GetSkinFile()
{
	return _T("hall\\sign\\dole.xml");
}

CDuiString DoleFrame::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE DoleFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR DoleFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

LRESULT DoleFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == CMW_CLOSE_SURE)
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	g_GameUtil->MinimizeTray(*this);
	return 0;
}

void DoleFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("DoleFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(DoleFrame::g_instance);
};

CControlUI* DoleFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	{
		return new CWkeWebkitUI();
	}

	return NULL;
}

void DoleFrame::InitWindow()
{
	g_Logger->TryInfo(_T("DoleFrame - Open"));
	m_pWkeCtl = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeDole_Web));
}

void DoleFrame::InitData(TNotifyUI& msg)
{
	//ÉèÖÃµÇÂ¼½çÃæÍøÒ³
	if (m_pWkeCtl)
	{
		TCHAR pzBuffer[MAX_PATH] = { 0 };
		DataManager* pDataManager = DataManager::GetInstance();
		UserData& tSelf = pDataManager->GetSelfData();
		LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("Dole"));
		_stprintf_s(pzBuffer, _T("%s%lu"), lpUrl, tSelf.GetUserID());
		m_pWkeCtl->Navigate(pzBuffer);
	}

	g_Logger->TryInfo(_T("DoleFrame - Open Complete"));
}

void DoleFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		this->InitData(msg);
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnDole_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
	}

	//__super::Notify(msg);
}

LRESULT DoleFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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