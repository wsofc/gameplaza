#include "stdafx.h"
#include "PanelScore.h"

namespace UserMenuNSP
{
	extern NumberConvert		g_NumberConvert;
	extern MenuDataStruct		g_tMenuData;
	extern IUserControl*		g_pDelegate;

	PanelScore* PanelScore::g_instance = nullptr;
	PanelScore::PanelScore()
		: m_pEdtGameID(nullptr)
		, m_pLabCurValue(nullptr)
		, m_pLabCurCapital(nullptr)
		, m_pEdtNewValue(nullptr)
		, m_pLabNewCapital(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	PanelScore::~PanelScore()
	{
	}

	PanelScore* PanelScore::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new PanelScore();
		}

		return g_instance;
	}

	int PanelScore::CreatFrame(CPaintManagerUI* pParentManager)
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

	void PanelScore::InitWindow()
	{
		// 初始化控件名
		m_strGameID			= _T("panel_score-edt_game_id");
		m_strCurValue		= _T("panel_score-lab_cur_value");
		m_strCurCapital		= _T("panel_score-lab_cur_capital");
		m_strNewValue		= _T("panel_score-edt_new_value");
		m_strNewCapital		= _T("panel_score-lab_new_capital");
		m_strCancel			= _T("panel_score-btn_cancel");
		m_strEnsure			= _T("panel_score-btn_ensure");
		m_strClose			= _T("panel_score-btn_close");

		// 初始化控件对象
		m_pEdtGameID		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strGameID));
		m_pLabCurValue		= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strCurValue));
		m_pLabCurCapital	= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strCurCapital));
		m_pEdtNewValue		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strNewValue));
		m_pLabNewCapital	= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strNewCapital));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void PanelScore::InitData(TNotifyUI& msg)
	{
		// 当前玩家ID
		_stprintf_s(m_pzBuffer, _T("%lu"), g_tMenuData.dwGameID);
		m_pEdtGameID->SetText(m_pzBuffer);

		// 更新当前设置的分数、大小写
		this->UpdateLastData();

		// 聚焦新值输入
		m_PaintManager.SetFocusNeeded(m_pEdtNewValue);
	}

	void PanelScore::UpdateLastData()
	{
		// 更新当前设置的分数
		_stprintf_s(m_pzBuffer, _T("%lld"), g_tMenuData.lCurWinScore);
		m_pLabCurValue->SetText(m_pzBuffer);

		std::string strNum = g_NumberConvert.NumToChineseStr(g_tMenuData.lCurWinScore);
		m_pLabCurCapital->SetText(strNum.c_str());
		m_pLabCurCapital->SetToolTip(m_pLabCurCapital->GetText());
		m_pLabCurCapital->SetEndEllipsis(true);
	}

	void PanelScore::Notify(TNotifyUI& msg)
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
		else if (_tcscmp(sType, DUI_MSGTYPE_TEXTCHANGED) == 0)
		{
			// 更新新值输入的大写描述
			if (m_pEdtNewValue && m_pEdtNewValue->IsFocused())
			{
				LONGLONG lNewValue = _ttoll(m_pEdtNewValue->GetText());
				std::string strNum = g_NumberConvert.NumToChineseStr(lNewValue);
				m_pLabNewCapital->SetText(strNum.c_str());
				m_pLabNewCapital->SetToolTip(m_pLabNewCapital->GetText());
				m_pLabNewCapital->SetEndEllipsis(true);
			}
		}
	}

	void PanelScore::Check()
	{
		bool bInputError = false;
		bool bInputEmpty = false;
		int iMsgEventID = MsgEventID::MSGID_NONE;
		CDuiString& pzGameID = m_pEdtGameID->GetText();
		CDuiString& pzNewValue = m_pEdtNewValue->GetText();
		if (!MatchRegular(pzGameID, _T("[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzGameID.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_USER_GAMEID;
			goto lab_err;
		}
		else if (!MatchRegular(pzNewValue, _T("(-)?[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzNewValue.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_SCORE_SETTING;
			goto lab_err;
		}
		else
		{
			if (g_pDelegate)
			{
				LONGLONG lGameID = _ttoll(m_pEdtGameID->GetText());
				LONGLONG lNewValue = _ttoll(m_pEdtNewValue->GetText());
				if (lNewValue < pow(10, 16))
				{
					g_pDelegate->CallBackSetScore(lGameID, lNewValue);
				}
				else
				{
					bInputError = true;
					iMsgEventID = MsgEventID::MSGID_SCORE_SETTING;
					goto lab_err;
				}
			}
			this->Close();
		}

	lab_err:
		if (bInputError)
		{
			LPCTSTR lpDescStr = bInputEmpty ? _T("输入不能为空!") : _T("输入不正确!");
			DialogCallBack dlg_cb = CC_CALLBACK_2(PanelScore::MsgBoxProcFocus, this);
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

	void PanelScore::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		switch (id)
		{
			case MsgEventID::MSGID_USER_GAMEID:		m_pEdtGameID->SetFocus();	break;
			case MsgEventID::MSGID_SCORE_SETTING:	m_pEdtNewValue->SetFocus();	break;
			default:															break;
		}
	}

	LRESULT PanelScore::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg)
		{
			case WM_CHAR:
			case WM_IME_CHAR:
			{
				if (m_pEdtGameID && m_pEdtGameID->IsFocused())
				{
					// 只接收数字
					if (wParam < 48 || wParam > 57)
					{
						bHandled = TRUE;
						return TRUE;
					}
				}
				else if (m_pEdtNewValue && m_pEdtNewValue->IsFocused())
				{
					// 只接收数字
					// 第一位可为"-"(负号)
					if ((m_pEdtNewValue->GetText().GetLength() == 0 && wParam == 45) || (wParam >= 48 && wParam <= 57))
					{
						bHandled = FALSE;
						return FALSE;
					}
					else
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
					else if (m_pEdtNewValue && m_pEdtNewValue->IsFocused())
					{
						pEdt = m_pEdtNewValue;
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

	void PanelScore::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE PanelScore::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR PanelScore::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString PanelScore::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString PanelScore::GetSkinFile()
	{
		return IDR_XML_PanelScore;
	}

	LRESULT PanelScore::ResponseDefaultKeyEvent(WPARAM wParam)
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
