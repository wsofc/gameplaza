#include "stdafx.h"
#include "RobotScore.h"

namespace UserMenuNSP
{
	extern NumberConvert 	g_NumberConvert;
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	RobotScore* RobotScore::g_instance = nullptr;
	RobotScore::RobotScore()
		: m_pEdtNewMax(nullptr)
		, m_pEdtNewMin(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	RobotScore::~RobotScore()
	{
	}

	RobotScore* RobotScore::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new RobotScore();
		}

		return g_instance;
	}

	int RobotScore::CreatFrame(CPaintManagerUI* pParentManager)
	{
		ASSERT(pParentManager);
		if (!pParentManager)
		{
			return FALSE;
		}

		if (FALSE == ::IsWindow(m_hWnd))
		{
			// �ָ�����������
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

	void RobotScore::InitWindow()
	{
		// ��ʼ���ؼ���
		m_strNewMax			= _T("panel_robot_score-edt_max");
		m_strNewMin			= _T("panel_robot_score-edt_min");
		m_strCancel			= _T("panel_robot_score-btn_cancel");
		m_strEnsure			= _T("panel_robot_score-btn_ensure");
		m_strClose			= _T("panel_robot_score-btn_close");

		// ��ʼ���ؼ�����
		m_pEdtNewMax		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strNewMax));
		m_pEdtNewMin		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strNewMin));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void RobotScore::InitData(TNotifyUI& msg)
	{
		// �����ϴ�����
		this->UpdateLastData();

		// �۽�"����������"�����
		m_PaintManager.SetFocusNeeded(m_pEdtNewMax);
	}

	void RobotScore::UpdateLastData()
	{
		// �����ϴ�����
		_stprintf_s(m_pzBuffer, _T("%lld"), g_tMenuData.lCurRobotMax);
		m_pEdtNewMax->SetText(m_pzBuffer);

		_stprintf_s(m_pzBuffer, _T("%lld"), g_tMenuData.lCurRobotMin);
		m_pEdtNewMin->SetText(m_pzBuffer);
	}

	void RobotScore::Notify(TNotifyUI& msg)
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

	void RobotScore::Check()
	{
		bool bInputError = false;
		bool bInputEmpty = false;
		int iMsgEventID = MsgEventID::MSGID_NONE;
		CDuiString& pzNewMax = m_pEdtNewMax->GetText();
		CDuiString& pzNewMin = m_pEdtNewMin->GetText();
		if (!MatchRegular(pzNewMax, _T("[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzNewMax.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_ROBOT_SCORE_MAX;
			goto lab_err;
		}
		else if (!MatchRegular(pzNewMin, _T("[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzNewMin.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_ROBOT_SCORE_MIN;
			goto lab_err;
		}
		else
		{
			if (g_pDelegate)
			{
				UINT64 lNewMaxValue = _ttoi64(m_pEdtNewMax->GetText());
				UINT64 lNewMinValue = _ttoi64(m_pEdtNewMin->GetText());
				if (lNewMaxValue >= lNewMinValue)
				{
					g_pDelegate->CallBackSetRobotScore(lNewMaxValue, lNewMinValue);
				}
				else
				{
					bInputError = true;
					bInputEmpty = false;
					iMsgEventID = MsgEventID::MSGID_ROBOT_SCORE_MAX;
					goto lab_err;
				}
			}
			this->Close();
		}

	lab_err:
		if (bInputError)
		{
			LPCTSTR lpDescStr = bInputEmpty ? _T("���벻��Ϊ��!") : _T("���벻��ȷ!");
			DialogCallBack dlg_cb = CC_CALLBACK_2(RobotScore::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("��ܰ��ʾ")
				, lpDescStr
				, (DLG_ID)iMsgEventID
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}
	}

	void RobotScore::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		switch (id)
		{
			case MsgEventID::MSGID_ROBOT_SCORE_MAX:	m_pEdtNewMax->SetFocus();	break;
			case MsgEventID::MSGID_ROBOT_SCORE_MIN:	m_pEdtNewMin->SetFocus();	break;
			default:															break;
		}
	}

	LRESULT RobotScore::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg)
		{
			case WM_CHAR:
			case WM_IME_CHAR:
			{
				if ((m_pEdtNewMax && m_pEdtNewMax->IsFocused())
					|| (m_pEdtNewMin && m_pEdtNewMin->IsFocused()))
				{
					// ֻ��������
					if (wParam < 48 || wParam > 57)
					{
						bHandled = TRUE;
						return TRUE;
					}
				}
			}break;

			case WM_KEYDOWN:
			{
				// ճ�� ctrl + v �Ĵ���
				if ((GetKeyState(VK_CONTROL) < 0) && (wParam == 'V'))
				{
					CRichEditUI* pEdt = nullptr;
					if (m_pEdtNewMax && m_pEdtNewMax->IsFocused())
					{
						pEdt = m_pEdtNewMax;
					}
					else if (m_pEdtNewMin && m_pEdtNewMin->IsFocused())
					{
						pEdt = m_pEdtNewMin;
					}

					if (pEdt)
					{
						// ��ȡճ������
						CDuiString pzCopyText;
						pEdt->GetClipboardText(pzCopyText);
						pzCopyText.Replace(_T(" "), _T(""));

						// ������������
						if (!UserMenuNSP::MatchRegular(pzCopyText, _T("[0-9]+")))
						{
							bHandled = TRUE;
							return TRUE;
						}
						else
						{
							// ��������,ȥ��ճ���ı��еĿո�
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

	void RobotScore::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE RobotScore::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR RobotScore::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString RobotScore::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString RobotScore::GetSkinFile()
	{
		return IDR_XML_PanelRobotScore;
	}

	LRESULT RobotScore::ResponseDefaultKeyEvent(WPARAM wParam)
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
