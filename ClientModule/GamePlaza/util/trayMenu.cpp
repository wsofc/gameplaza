#include "stdafx.h"
#include "trayMenu.h"
#include "..\\hall\\exitFrame.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;

TrayMenu* TrayMenu::g_instance = NULL;

TrayMenu::TrayMenu()
	: m_hParentWnd(NULL)
{

}

TrayMenu::~TrayMenu()
{
}

TrayMenu* TrayMenu::GetInstance()
{
	if (g_instance == NULL)
		g_instance = new TrayMenu();

	return g_instance;
}

CDuiString TrayMenu::GetSkinFolder()
{
	return _T("");
}

CDuiString TrayMenu::GetSkinFile()
{
	return _T("menu/menuWin.xml");
}

void TrayMenu::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);

	this->RemoveTrayIcon();
	SAFE_DELETE(g_instance);
}

BOOL TrayMenu::RemoveTrayIcon()
{
	return Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

BOOL TrayMenu::CreateFrame(CPaintManagerUI* pParentManager)
{
	if (!pParentManager) return FALSE;

	// 创建一个容器样式的窗口(即无任何样式)
	// 此处仅用于初始化m_hWnd,来创建托盘icon
	//UI_WNDSTYLE_CONTAINER | WS_OVERLAPPED | 0
	m_pParentManager	= pParentManager;
	m_hParentWnd		= m_pParentManager->GetPaintWindow();
	this->Create(m_hParentWnd, NULL, UI_WNDSTYLE_CONTAINER, 0L, 0, 0, 0, 0);
	//this->ShowWindow(false);

	HICON hicon = LoadIcon(m_PaintManager.GetInstance(), MAKEINTRESOURCE(IDI_GAMEPLAZA_ICON));
	TCHAR szTip[128] = { 0 };
	_tcscpy(szTip, R->MainWndTitleName);
	
	m_nid.cbSize			= sizeof(NOTIFYICONDATA);				//定义structure大小
	m_nid.hWnd				= m_hWnd;								//当前程序窗口句柄，可以在CreateWindowEx的返回值获得
	m_nid.uID				= NULL;									//托盘图标的ID,再有多个ico文件时使用，这里设成NULL(没有试过，以后可以尝试一下）
	m_nid.uFlags			= NIF_MESSAGE | NIF_ICON | NIF_TIP;		//选择哪几个参数有效
	m_nid.uCallbackMessage	= TRAY_MENU;							//回调消息ID，用户自定义事件(可以是任何整数）
	m_nid.hIcon				= hicon;								//托盘图标的句柄，可以是和window图标相同，也可以用LoadIcon来定义
	_tcscpy(m_nid.szTip, szTip);									//托盘图标的提示字符串(nid.szTip为指针常量)
	

	::Shell_NotifyIcon(NIM_ADD, &m_nid);

	return TRUE;
}

void TrayMenu::ResetControlSoon(CControlUI* control)
{
	if (control != NULL)
		m_aResetControl.Add(control);
}

void TrayMenu::ShowCurruntWindow(HWND hWnd, int nCmdShow)
{
	if (hWnd != NULL)
		m_aWnd.Add(hWnd);
	
	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);
}

void TrayMenu::_ShowAllWindow(int nCmdShow)
{
	for (int i = 0; i < m_aWnd.GetSize(); ++i) {
		HWND phWnd = static_cast<HWND>(m_aWnd[i]);

		::ShowWindow(phWnd, nCmdShow);
		::UpdateWindow(phWnd);
	}
	m_aWnd.Empty();
}

void TrayMenu::_ResetControl()
{
	for (int i = 0; i < m_aResetControl.GetSize(); ++i) {
		CControlUI* tempControl = static_cast<CControlUI*>(m_aResetControl[i]);
		tempControl->RestState();
	}
	m_aResetControl.Empty();
}

void TrayMenu::CreateMenu()
{
	POINT p;
	::GetCursorPos(&p);

	CMenuWnd* pMenu = new CMenuWnd();
	::CPoint point(p.x, p.y);
	pMenu->Init(NULL, _T("menu/trayMenu.xml"), _T(""), point, &m_PaintManager, &m_MenuCheckInfo, nullptr, eMenuAlignment_Right | eMenuAlignment_Bottom);

	BOOL bActive = FALSE;
	
	bActive = ::IsWindowEnabled(m_hParentWnd);
	pMenu->SetMenuItemEnable(R->kMenuTrayMenu_Restore, bActive);

	bActive = ::IsIconic(m_hParentWnd);
	pMenu->SetMenuItemEnable(R->kMenuTrayMenu_Min, !bActive);

	bActive = ::IsZoomed(m_hParentWnd);
	pMenu->SetMenuItemEnable(R->kMenuTrayMenu_Max, !bActive);

	bActive = TRUE;
	pMenu->SetMenuItemEnable(R->kMenuTrayMenu_Exit, bActive);

	pMenu->Show();
}

void TrayMenu::Notify(TNotifyUI& msg)
{
	CDuiString& sName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT);
	else if (msg.sType == DUI_MSGTYPE_CLICK) {
	}
}

LRESULT TrayMenu::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg) {

		case WM_LBUTTONDOWN:
		{
			int x = 0;
		}break;

		case TRAY_MENU:
		{
			switch (LOWORD(lParam))
			{
				case WM_MOUSEMOVE:
				{
				}break;
				case WM_LBUTTONDOWN:
				{
					this->_ShowAllWindow(SW_SHOW);
					this->_ResetControl();

					::SendMessage(m_hParentWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				}break;

				case WM_RBUTTONDOWN:
				{
					
				}break;

				case WM_RBUTTONUP:
				{
					this->CreateMenu();
				}break;

				default:
					break;
			}
		}break;

		case WM_MENUCLICK:
		{
			CControlUI* pParentCtl = (CControlUI*)(lParam);
			CDuiString* strMenuName = (CDuiString*)(wParam);

			if (_tcscmp(*strMenuName, R->kMenuTrayMenu_Exit) == 0)
			{
				ExitFrame* pExitFrame = ExitFrame::GetInstance();
				pExitFrame->CreateFrame();
			}
			else if (_tcscmp(*strMenuName, R->kMenuTrayMenu_Restore) == 0)
			{
				::SendMessage(m_hParentWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			}
			else if (_tcscmp(*strMenuName, R->kMenuTrayMenu_Min) == 0)
			{
				::SendMessage(m_hParentWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			else if (_tcscmp(*strMenuName, R->kMenuTrayMenu_Max) == 0)
			{
				::SendMessage(m_hParentWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}

			SAFE_DELETE(strMenuName);
		}break;

		default:
			break;
	}

	return 0;
}