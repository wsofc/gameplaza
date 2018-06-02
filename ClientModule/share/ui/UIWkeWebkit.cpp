#include "stdafx.h"
#include "UIWkeWebkit.h"

namespace DuiLib{

	///////////////////////////////////////////////////  
	//��ҳ����״̬����߳�
	DWORD WINAPI CheckThreadFun(LPVOID lpParam)
	{
		CWkeWebkitUI* pWebkitUI = (CWkeWebkitUI*)lpParam;
		wkeWebView  pWebView = pWebkitUI->GetWebView();
		if (NULL == pWebView)
			return 1;
		CWkeWebkitLoadCallback* pCallback = pWebkitUI->GetLoadCallback();
		if (NULL == pCallback)
			return 1;
		bool bOver = false;
		while (!pWebView->isLoaded())
		{
			if (!bOver && pWebView->isDocumentReady())
			{
				pCallback->OnWkeDocumentReady();
				bOver = true;
			}
			if (pWebView->isLoadFailed())
			{
				pCallback->OnWkeLoadFailed();
				return 1;
			}
			::Sleep(30);
		}
		if (pWebView->isLoadComplete())
			pCallback->OnWkeLoadComplete();
		return 0;
	}

	//////////////////////////////////////////////////////  
	int CWkeWebkitUI::m_bWebkitCount = 0;
	CWkeWebkitUI::CWkeWebkitUI(void)
		: m_pWebView(NULL)
		, m_bClickUrl(false)
		, m_hCheckThread(NULL)
		, m_pLoadCallback(NULL)
		, m_pClientCallback(NULL)
	{
		memset(&m_ClientHandler, 0, sizeof(m_ClientHandler));

		if (0 == m_bWebkitCount)
		{
			wkeInit();
		}
		m_pWebView = wkeCreateWebView();
		m_pWebView->setBufHandler(this);
		m_pWebView->setClientHandler(&m_ClientHandler);
		m_ClientHandler.onTitleChanged = CWkeWebkitUI::OnTitleChange;
		m_ClientHandler.onURLChanged = CWkeWebkitUI::OnUrlChange;
		m_ClientHandler.pDelegate = this;

		++m_bWebkitCount;
	}

	CWkeWebkitUI::~CWkeWebkitUI(void)
	{
		StopCheckThread();
		m_pManager->KillTimer(this);
		wkeDestroyWebView(m_pWebView);
		if (0 == --m_bWebkitCount)
			wkeShutdown();
	}

	LPCTSTR CWkeWebkitUI::GetClass() const
	{
		return _T("WkeWebkitUI");
	}

	LPVOID CWkeWebkitUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_WKEWEBKIT) == 0)
			return static_cast<CWkeWebkitUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CWkeWebkitUI::DoEvent(TEventUI& event)
	{
		switch (event.Type)
		{
			case UIEVENT_SETFOCUS:
				if (m_pWebView) m_pWebView->focus(); break;
			case UIEVENT_KILLFOCUS:
				if (m_pWebView) m_pWebView->unfocus(); break;
			case UIEVENT_WINDOWSIZE:
				if (m_pWebView) m_pWebView->resize(GET_X_LPARAM(event.lParam), GET_Y_LPARAM(event.lParam)); break;
			case UIEVENT_CHAR:
			{
				if (NULL == m_pWebView) break;
				unsigned int charCode = event.wParam;
				unsigned int flags = 0;
				if (HIWORD(event.lParam) & KF_REPEAT)
					flags |= WKE_REPEAT;
				if (HIWORD(event.lParam) & KF_EXTENDED)
					flags |= WKE_EXTENDED;
				bool bHandled = m_pWebView->keyPress(charCode, flags, false);
				if (bHandled)
					return;
				break;
			}
			case UIEVENT_KEYDOWN:
			{
				if (NULL == m_pWebView) break;
				unsigned int flags = 0;
				if (HIWORD(event.lParam) & KF_REPEAT)
					flags |= WKE_REPEAT;
				if (HIWORD(event.lParam) & KF_EXTENDED)
					flags |= WKE_EXTENDED;
				bool bHandled = m_pWebView->keyDown(event.wParam, flags, false);
				if (event.wParam == VK_F5)
					Refresh();
				if (bHandled)
					return;
				break;
			}
			case UIEVENT_KEYUP:
			{
				if (NULL == m_pWebView) break;
				unsigned int flags = 0;
				if (HIWORD(event.lParam) & KF_REPEAT)
					flags |= WKE_REPEAT;
				if (HIWORD(event.lParam) & KF_EXTENDED)
					flags |= WKE_EXTENDED;
				bool bHandled = m_pWebView->keyUp(event.wParam, flags, false);
				if (bHandled)
					return;
				break;
			}
			case UIEVENT_CONTEXTMENU:
			{
				if (NULL == m_pWebView) break;
				unsigned int flags = 0;
				if (event.wParam & MK_CONTROL)
					flags |= WKE_CONTROL;
				if (event.wParam & MK_SHIFT)
					flags |= WKE_SHIFT;
				if (event.wParam & MK_LBUTTON)
					flags |= WKE_LBUTTON;
				if (event.wParam & MK_MBUTTON)
					flags |= WKE_MBUTTON;
				if (event.wParam & MK_RBUTTON)
					flags |= WKE_RBUTTON;
				bool handled = m_pWebView->contextMenuEvent(event.ptMouse.x, event.ptMouse.y, flags);
				if (handled)
					return;
				break;
			}
			case UIEVENT_MOUSEMOVE:
			case UIEVENT_BUTTONDOWN:
			case UIEVENT_BUTTONUP:
			case UIEVENT_RBUTTONDOWN:
			case UIEVENT_DBLCLICK:
			{
				HWND hWnd = m_pManager->GetPaintWindow();
				if (event.Type == UIEVENT_BUTTONDOWN)
				{
					::SetFocus(hWnd);
					SetCapture(hWnd);
				}
				else if (event.Type == UIEVENT_BUTTONUP)
				{
					ReleaseCapture();
					m_bClickUrl = true;
				}
				unsigned int flags = 0;
				if (event.wParam & MK_CONTROL)
					flags |= WKE_CONTROL;
				if (event.wParam & MK_SHIFT)
					flags |= WKE_SHIFT;

				if (event.wParam & MK_LBUTTON)
					flags |= WKE_LBUTTON;
				if (event.wParam & MK_MBUTTON)
					flags |= WKE_MBUTTON;
				if (event.wParam & MK_RBUTTON)
					flags |= WKE_RBUTTON;
				UINT uMsg = 0;
				switch (event.Type)
				{
					case UIEVENT_BUTTONDOWN:    uMsg = WM_LBUTTONDOWN; break;
					case UIEVENT_BUTTONUP:      uMsg = WM_LBUTTONUP; break;
					case UIEVENT_RBUTTONDOWN:   uMsg = WM_RBUTTONDOWN; break;
					case UIEVENT_DBLCLICK:      uMsg = WM_LBUTTONDBLCLK; break;
					case UIEVENT_MOUSEMOVE:     uMsg = WM_MOUSEMOVE; break;
				}
				bool bHandled = m_pWebView->mouseEvent(uMsg, event.ptMouse.x - m_rcItem.left, \
					event.ptMouse.y - m_rcItem.top, flags);
				if (bHandled)
					return;
				break;
			}
			case UIEVENT_TIMER:
				if (m_pWebView)
					m_pWebView->tick();
				break;
			case UIEVENT_SCROLLWHEEL:
			{
				POINT pt;
				pt.x = LOWORD(event.lParam);
				pt.y = HIWORD(event.lParam);
				int nFlag = GET_X_LPARAM(event.wParam);
				int delta = (nFlag == SB_LINEDOWN) ? -120 : 120;
				unsigned int flags = 0;
				if (event.wParam & MK_CONTROL)
					flags |= WKE_CONTROL;
				if (event.wParam & MK_SHIFT)
					flags |= WKE_SHIFT;
				if (event.wParam & MK_LBUTTON)
					flags |= WKE_LBUTTON;
				if (event.wParam & MK_MBUTTON)
					flags |= WKE_MBUTTON;
				if (event.wParam & MK_RBUTTON)
					flags |= WKE_RBUTTON;
				bool handled = m_pWebView->mouseWheel(pt.x, pt.y, delta, flags);
				if (handled)
					return;
				break;
			}
			default:
				CControlUI::DoEvent(event); break;
		}
	}

	void CWkeWebkitUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if (m_pWebView)
		{
			RECT rcInsert;
			IntersectRect(&rcInsert, &m_rcItem, &rcPaint);
			m_pWebView->paint(hDC, rcInsert.left, rcInsert.top, \
				rcInsert.right - rcInsert.left, rcInsert.bottom - rcInsert.top, \
				rcInsert.left - m_rcItem.left, rcInsert.top - m_rcItem.top, true);
		}
	}

	void CWkeWebkitUI::onBufUpdated(const HDC hdc, int x, int y, int cx, int cy)
	{
		RECT rcValide = { x, y, x + cx, y + cy };
		::OffsetRect(&rcValide, m_rcItem.left, m_rcItem.top);
		HWND hWnd = m_pManager->GetPaintWindow();
		::InvalidateRect(hWnd, &rcValide, TRUE);
	}

	void CWkeWebkitUI::Navigate(LPCTSTR lpUrl)
	{
		if (m_pWebView)
		{
			m_strUrl = lpUrl;
			m_pWebView->loadURL(lpUrl);
			StartCheckThread();
		}
	}

	void CWkeWebkitUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		if (m_pWebView)
			m_pWebView->resize(rc.right - rc.left, rc.bottom - rc.top);
	}

	void CWkeWebkitUI::DoInit()
	{
		if (!m_strUrl.IsEmpty())
			Navigate(m_strUrl.GetData());
		m_pManager->SetTimer(this, 100, 100);
	}

	void CWkeWebkitUI::StartCheckThread()
	{
		StopCheckThread();
		m_hCheckThread = ::CreateThread(NULL, 0, CheckThreadFun, this, 0, NULL);
	}

	void CWkeWebkitUI::StopCheckThread()
	{
		if (m_hCheckThread)
		{
			if (::WaitForSingleObject(m_hCheckThread, 10) != WAIT_OBJECT_0)
				::TerminateThread(m_hCheckThread, 0);
			::CloseHandle(m_hCheckThread);
			m_hCheckThread = NULL;
		}
	}

	// ����wke�޷����ص����ת��ҳǰ���¼�
	// ��˴˴����������
	bool CWkeWebkitUI::IsClickURL() const
	{
		return m_bClickUrl;
	}

	// �Զ�����תurl�����Ǻ����ñ��
	void CWkeWebkitUI::ResetClickURL()
	{
		m_bClickUrl = false;
	}

	bool CWkeWebkitUI::CanGoBack() const
	{
		return m_pWebView ? m_pWebView->canGoBack() : false;
	}

	bool CWkeWebkitUI::GoBack()
	{
		return m_pWebView ? m_pWebView->goBack() : false;
	}

	bool CWkeWebkitUI::CanGoForward() const
	{
		return m_pWebView ? m_pWebView->canGoForward() : false;
	}

	bool CWkeWebkitUI::GoForward()
	{
		return m_pWebView ? m_pWebView->goForward() : false;
	}

	void CWkeWebkitUI::StopLoad()
	{
		if (m_pWebView)
			m_pWebView->stopLoading();
	}

	void CWkeWebkitUI::Refresh()
	{
		if (m_pWebView)
		{
			StopCheckThread();
			m_pWebView->reload();
			StartCheckThread();
		}
	}

	wkeWebView CWkeWebkitUI::GetWebView()
	{
		return m_pWebView;
	}

	void CWkeWebkitUI::SetLoadCallback(CWkeWebkitLoadCallback* pCallback)
	{
		m_pLoadCallback = pCallback;
	}

	CWkeWebkitLoadCallback* CWkeWebkitUI::GetLoadCallback()
	{
		return m_pLoadCallback;
	}

	void CWkeWebkitUI::SetClientCallback(CWkeWebkitClientCallback* pCallback)
	{
		m_pClientCallback = pCallback;
	}

	CWkeWebkitClientCallback* CWkeWebkitUI::GetClientCallback()
	{
		return m_pClientCallback;
	}

	void CWkeWebkitUI::OnTitleChange(const struct _wkeClientHandler* clientHandler, const wkeString title)
	{
		CWkeWebkitUI* pDelegate = static_cast<CWkeWebkitUI*>(clientHandler->pDelegate);
		if (pDelegate && pDelegate->m_pClientCallback)
		{
			CDuiString strTitle;
			const wchar_t* w_title = wkeToStringW(title);
		#ifdef _UNICODE || UNICODE
			strURL = w_url;
		#else
			DWORD dwLen = WideCharToMultiByte(CP_OEMCP, NULL, w_title, -1, NULL, 0, NULL, FALSE);
			char* c_title = new char[dwLen];
			WideCharToMultiByte(CP_OEMCP, NULL, w_title, -1, c_title, dwLen, NULL, FALSE);
			strTitle = c_title;
			delete c_title;
			c_title = NULL;
		#endif // _UNICODE

			pDelegate->m_pClientCallback->OnWkeTitleChange(strTitle);
		}
	}

	void CWkeWebkitUI::OnUrlChange(const struct _wkeClientHandler* clientHandler, const wkeString url)
	{
		CWkeWebkitUI* pDelegate = static_cast<CWkeWebkitUI*>(clientHandler->pDelegate);
		if (pDelegate && pDelegate->m_pClientCallback)
		{
			CDuiString strURL;
			const wchar_t* w_url = wkeToStringW(url);
		#ifdef _UNICODE || UNICODE
			strURL = w_url;
		#else
			DWORD dwLen = WideCharToMultiByte(CP_OEMCP, NULL, w_url, -1, NULL, 0, NULL, FALSE);
			char* c_url = new char[dwLen];
			WideCharToMultiByte(CP_OEMCP, NULL, w_url, -1, c_url, dwLen, NULL, FALSE);
			strURL = c_url;
			delete c_url;
			c_url = NULL;
		#endif // _UNICODE

			pDelegate->m_pClientCallback->OnWkeUrlChange(strURL);
		}
	}

	void CWkeWebkitUI::LoadFile(LPCTSTR lpFile)
	{
		if (m_pWebView)
			m_pWebView->loadFile(lpFile);
	}

	void CWkeWebkitUI::LoadHtml(LPCTSTR lpHtml)
	{
		if (m_pWebView)
			m_pWebView->loadHTML(lpHtml);
	}

	const CDuiString& CWkeWebkitUI::GetUrl() const
	{
		return m_strUrl;
	}

	void CWkeWebkitUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("url")) == 0)
			m_strUrl = pstrValue;
		else
			CControlUI::SetAttribute(pstrName, pstrValue);
	}

}