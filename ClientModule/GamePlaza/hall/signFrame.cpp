#include "stdafx.h"
#include "signFrame.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

SignFrame* SignFrame::g_instance = nullptr;
SignFrame::SignFrame()
	: m_pParentManager(nullptr)
	, m_pWkeCtl(nullptr)
{
}

SignFrame::~SignFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);
	g_GameUtil->ShowTopWindow(m_pParentManager->GetPaintWindow(), true, true);

	g_Logger->TryInfo(_T("SignFrame - Destroy"));
}

SignFrame* SignFrame::GetInstance()
{
	if (SignFrame::g_instance == nullptr)
	{
		SignFrame::g_instance = new SignFrame();
	}

	return SignFrame::g_instance;
}

BOOL SignFrame::CreateFrame(CPaintManagerUI* pParentManager)
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

CDuiString SignFrame::GetSkinFile()
{
	return _T("hall\\sign\\sign.xml");
}

CDuiString SignFrame::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE SignFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR SignFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

LRESULT SignFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == CMW_CLOSE_SURE)
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	g_GameUtil->MinimizeTray(*this);
	return 0;
}

void SignFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("SignFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(SignFrame::g_instance);
};

CControlUI* SignFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	{
		return new CWkeWebkitUI();
	}

	return NULL;
}

void SignFrame::InitWindow()
{
	g_Logger->TryInfo(_T("SignFrame - Open"));
	m_pWkeCtl = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeSign_Web));
}

void SignFrame::InitData(TNotifyUI& msg)
{
	//ÉèÖÃµÇÂ¼½çÃæÍøÒ³
	if (m_pWkeCtl)
	{
		TCHAR pzBuffer[MAX_PATH] = { 0 };
		DataManager* pDataManager = DataManager::GetInstance();
		UserData& tSelf = pDataManager->GetSelfData();
		LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("Sign"));
		_stprintf_s(pzBuffer, _T("%s%lu"), lpUrl, tSelf.GetUserID());
		m_pWkeCtl->Navigate(pzBuffer);
	}

	g_Logger->TryInfo(_T("SignFrame - Open Complete"));
}

void SignFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		this->InitData(msg);
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnSign_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
	}

	//__super::Notify(msg);
}

LRESULT SignFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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