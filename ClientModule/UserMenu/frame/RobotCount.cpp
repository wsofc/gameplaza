#include "stdafx.h"
#include "RobotCount.h"

namespace UserMenuNSP
{
	extern NumberConvert 	g_NumberConvert;
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	RobotCount* RobotCount::g_instance = nullptr;
	RobotCount::RobotCount()
		: m_pEdtCount(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	RobotCount::~RobotCount()
	{
	}

	RobotCount* RobotCount::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new RobotCount();
		}

		return g_instance;
	}

	int RobotCount::CreatFrame(CPaintManagerUI* pParentManager)
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

	void RobotCount::InitWindow()
	{
		// 初始化控件名
		m_strCount			= _T("panel_robot_count-edt_count");
		m_strCancel			= _T("panel_robot_count-btn_cancel");
		m_strEnsure			= _T("panel_robot_count-btn_ensure");
		m_strClose			= _T("panel_robot_count-btn_close");

		// 初始化控件对象
		m_pEdtCount			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strCount));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void RobotCount::InitData(TNotifyUI& msg)
	{
		// 设置上次机器人数量
		this->UpdateLastData();

		// 聚焦"设置机器人数量"编辑框
		m_PaintManager.SetFocusNeeded(m_pEdtCount);
	}

	void RobotCount::UpdateLastData()
	{
		// 更新上次设置
		_stprintf_s(m_pzBuffer, _T("%lld"), g_tMenuData.lCurRobotCount);
		m_pEdtCount->SetText(m_pzBuffer);
	}

	void RobotCount::Notify(TNotifyUI& msg)
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

	void RobotCount::Check()
	{
		bool bInputError = false;
		bool bInputEmpty = false;
		int iMsgEventID = MsgEventID::MSGID_NONE;
		CDuiString& pzCount = m_pEdtCount->GetText();
		if (!MatchRegular(pzCount, _T("[0-9]*")))
		{
			bInputError = true;
			bInputEmpty = pzCount.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_ROBOT_COUNT;
			goto lab_err;
		}
		else
		{
			if (g_pDelegate)
			{
				DWORD dwCount = _ttol(m_pEdtCount->GetText());
				g_pDelegate->CallBackSetRobotCount(dwCount);
			}
			this->Close();
		}

	lab_err:
		if (bInputError)
		{
			LPCTSTR lpDescStr = bInputEmpty ? _T("输入不能为空!") : _T("输入不正确!");
			DialogCallBack dlg_cb = CC_CALLBACK_2(RobotCount::MsgBoxProcFocus, this);
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

	void RobotCount::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		switch (id)
		{
			case MsgEventID::MSGID_ROBOT_COUNT:	m_pEdtCount->SetFocus();	break;
			default:														break;
		}
	}

	LRESULT RobotCount::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg)
		{
			case WM_CHAR:
			case WM_IME_CHAR:
			{
				if ((m_pEdtCount && m_pEdtCount->IsFocused()))
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
					if (m_pEdtCount && m_pEdtCount->IsFocused())
					{
						// 获取粘贴内容
						CDuiString pzCopyText;
						m_pEdtCount->GetClipboardText(pzCopyText);
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
							m_pEdtCount->SetClipboardText(pzCopyText);
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

	void RobotCount::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE RobotCount::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR RobotCount::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString RobotCount::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString RobotCount::GetSkinFile()
	{
		return IDR_XML_PanelRobotCount;
	}

	LRESULT RobotCount::ResponseDefaultKeyEvent(WPARAM wParam)
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
