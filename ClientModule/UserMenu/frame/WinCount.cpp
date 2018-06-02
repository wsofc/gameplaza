#include "stdafx.h"
#include "WinCount.h"

namespace UserMenuNSP
{
	extern NumberConvert 	g_NumberConvert;
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	WinCount* WinCount::g_instance = nullptr;
	WinCount::WinCount()
		: m_pEdtGameID(nullptr)
		, m_pEdtCount(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	WinCount::~WinCount()
	{
	}

	WinCount* WinCount::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new WinCount();
		}

		return g_instance;
	}

	int WinCount::CreatFrame(CPaintManagerUI* pParentManager)
	{
		ASSERT(pParentManager);
		if (!pParentManager)
		{
			return FALSE;
		}

		if (FALSE == ::IsWindow(m_hWnd))
		{
			// 恢复大厅主窗口
			HWND hParentWnd = pParentManager->GetPaintWindow();
			m_hWnd = this->Create(hParentWnd, NULL, UI_CLASSSTYLE_DIALOG, 0L);
			this->CenterWindow();
			this->ShowWindow(true);
		}
		else
		{
			this->CenterWindow();
			this->ShowWindow(true);
		}

		return TRUE;
	}

	void WinCount::InitWindow()
	{
		// 初始化控件名
		m_strGameID			= _T("panel_win_count-edt_game_id");
		m_strCount			= _T("panel_win_count-edt_count");
		m_strCancel			= _T("panel_win_count-btn_cancel");
		m_strEnsure			= _T("panel_win_count-btn_ensure");
		m_strClose			= _T("panel_win_count-btn_close");

		// 初始化控件对象
		m_pEdtGameID		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strGameID));
		m_pEdtCount			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strCount));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void WinCount::InitData(TNotifyUI& msg)
	{
		// 当前玩家ID
		_stprintf_s(m_pzBuffer, _T("%lu"), g_tMenuData.dwGameID);
		m_pEdtGameID->SetText(m_pzBuffer);

		// 设置上次数据
		this->UpdateLastData();

		// 聚焦"局数"输入框
		m_PaintManager.SetFocusNeeded(m_pEdtCount);
	}

	void WinCount::UpdateLastData()
	{
		// 更新上次设置数据
		_stprintf_s(m_pzBuffer, _T("%lld"), g_tMenuData.lCurWinCount);
		m_pEdtCount->SetText(m_pzBuffer);
	}

	void WinCount::Notify(TNotifyUI& msg)
	{
		CDuiString& sType = msg.sType;
		CDuiString& sName = msg.pSender->GetName();

		if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		{
			this->InitData(msg);
		}
		else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
		{
			if (_tcscmp(sName, m_strClose.GetData()) == 0)
			{
				this->Close();
			}
			else if (_tcscmp(sName, m_strCancel.GetData()) == 0)
			{
				this->Close();
			}
			else if (_tcscmp(sName, m_strEnsure.GetData()) == 0)
			{
				this->Check();
			}
		}
	}

	void WinCount::Check()
	{
		bool bInputError = false;
		bool bInputEmpty = false;
		int iMsgEventID = MsgEventID::MSGID_NONE;
		CDuiString& pzGameID = m_pEdtGameID->GetText();
		CDuiString& pzCount = m_pEdtCount->GetText();
		if (!MatchRegular(pzGameID, _T("[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzGameID.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_USER_GAMEID;
			goto lab_err;
		}
		else if (!MatchRegular(pzCount, _T("[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzCount.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_WIN_COUNT;
			goto lab_err;
		}
		else
		{
			if (g_pDelegate)
			{
				LONGLONG lGameID = _ttoll(m_pEdtGameID->GetText());
				UINT64 lCount = _ttoll(m_pEdtCount->GetText());
				g_pDelegate->CallBackWinCount(lGameID, lCount);
			}
			this->Close();
		}

	lab_err:
		if (bInputError)
		{
			LPCTSTR lpDescStr = bInputEmpty ? _T("输入不能为空!") : _T("输入不正确!");
			DialogCallBack dlg_cb = CC_CALLBACK_2(WinCount::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, lpDescStr
				, (DLG_ID)iMsgEventID
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}
	}

	void WinCount::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		switch (id)
		{
			case MsgEventID::MSGID_USER_GAMEID:	m_pEdtGameID->SetFocus();	break;
			case MsgEventID::MSGID_WIN_COUNT:	m_pEdtCount->SetFocus();	break;
			default:														break;
		}
	}

	LRESULT WinCount::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg)
		{
			case WM_CHAR:
			case WM_IME_CHAR:
			{
				if ((m_pEdtGameID && m_pEdtGameID->IsFocused())
					|| (m_pEdtCount && m_pEdtCount->IsFocused()))
				{
					// 只接收数字
					if (wParam < 48 || wParam > 57)
					{
						bHandled = TRUE;
						return TRUE;
					}
				}
			}break;

			case WM_KEYDOWN:
			{
				// 粘贴 ctrl + v 的处理
				if ((GetKeyState(VK_CONTROL) < 0) && (wParam == 'V'))
				{
					CRichEditUI* pEdt = nullptr;
					if (m_pEdtGameID && m_pEdtGameID->IsFocused())
					{
						pEdt = m_pEdtGameID;
					}
					else if (m_pEdtCount && m_pEdtCount->IsFocused())
					{
						pEdt = m_pEdtCount;
					}

					if (pEdt)
					{
						// 获取粘贴内容
						CDuiString pzCopyText;
						pEdt->GetClipboardText(pzCopyText);
						pzCopyText.Replace(_T(" "), _T(""));

						// 非数字则跳过
						if (!UserMenuNSP::MatchRegular(pzCopyText, _T("[0-9]+")))
						{
							bHandled = TRUE;
							return TRUE;
						}
						else
						{
							// 满足条件,去除粘贴文本中的空格
							pEdt->SetClipboardText(pzCopyText);
						}
					}
				}
			}break;

			default:
				break;
		}
		
		bHandled = FALSE;
		return FALSE;
	}

	void WinCount::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE WinCount::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR WinCount::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString WinCount::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString WinCount::GetSkinFile()
	{
		return IDR_XML_PanelWinCount;
	}

	LRESULT WinCount::ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (m_PaintManager.GetRoot()->IsKeyboardEnabled())
		{
			if (wParam == VK_RETURN)
			{
				return FALSE;
			}
			else if (wParam == VK_ESCAPE)
			{
				this->Close();
				return TRUE;
			}
		}

		return FALSE;
	}
}
