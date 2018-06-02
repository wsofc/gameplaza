#include "stdafx.h"
#include "SendSysMsg.h"

namespace UserMenuNSP
{
	extern NumberConvert 	g_NumberConvert;
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	SendSysMsg* SendSysMsg::g_instance = nullptr;
	SendSysMsg::SendSysMsg()
		: m_pEdtSysMsg(nullptr)
		, m_pOptSendToGame(nullptr)
		, m_pOptSendToRoom(nullptr)
		, m_pLabLeftWords(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
		memset(m_pzSysMsg, 0, ArrayCount(m_pzSysMsg) * sizeof(TCHAR));
	}

	SendSysMsg::~SendSysMsg()
	{
	}

	SendSysMsg* SendSysMsg::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new SendSysMsg();
		}

		return g_instance;
	}

	int SendSysMsg::CreatFrame(CPaintManagerUI* pParentManager)
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

	void SendSysMsg::InitWindow()
	{
		// 初始化控件名
		m_strSysMsg			= _T("panel_send_sys-edt_msg");
		m_strSendToGame		= _T("panel_send_sys-opt_game");
		m_strSendToRoom		= _T("panel_send_sys-opt_room");
		m_strLeftWords		= _T("panel_send_sys-lab_words");
		m_strCancel			= _T("panel_send_sys-btn_cancel");
		m_strEnsure			= _T("panel_send_sys-btn_ensure");
		m_strClose			= _T("panel_send_sys-btn_close");

		// 初始化控件对象
		m_pEdtSysMsg		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strSysMsg));
		m_pOptSendToGame	= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strSendToGame));
		m_pOptSendToRoom	= static_cast<COptionUI*>(m_PaintManager.FindControl(m_strSendToRoom));
		m_pLabLeftWords		= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strLeftWords));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void SendSysMsg::InitData(TNotifyUI& msg)
	{
		// 聚焦"系统消息"输入框
		m_PaintManager.SetFocusNeeded(m_pEdtSysMsg);

		// 显示剩余字数
		this->ShowLeftWords();

		// 默认勾选"发送到游戏"和"发送到房间"
		m_pOptSendToGame->Selected(true);
		m_pOptSendToRoom->Selected(true);
	}

	void SendSysMsg::Notify(TNotifyUI& msg)
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
			if (_tcscmp(sName,m_strSysMsg) == 0)
			{
				CDuiString& pzUnderWrite = m_pEdtSysMsg->GetText();
				WORD wCount = m_pEdtSysMsg->GetText().GetLength();
				if (wCount > SYS_MSG_LEN)
				{
					memset(m_pzSysMsg, 0, ArrayCount(m_pzSysMsg) * sizeof(TCHAR));
					memcpy(m_pzSysMsg, pzUnderWrite.GetData(), SYS_MSG_LEN * sizeof(TCHAR));
					m_pEdtSysMsg->SetText(m_pzSysMsg);
				}

				this->ShowLeftWords();
			}
		}
	}

	void SendSysMsg::ShowLeftWords()
	{
		if (!m_pEdtSysMsg) return;

		int iRealNum = SYS_MSG_LEN;
		int iLeftNum = iRealNum - m_pEdtSysMsg->GetText().GetLength();
		iLeftNum = iLeftNum < 0 ? 0 : iLeftNum;

		_stprintf_s(m_pzBuffer, _T("%d/%d"), iLeftNum, iRealNum);
		m_pLabLeftWords->SetText(m_pzBuffer);
	}

	void SendSysMsg::Check()
	{
		bool bInputError = false;
		int iMsgEventID = MsgEventID::MSGID_NONE;
		CDuiString& strMsg = m_pEdtSysMsg->GetText();
		if (strMsg.GetLength() <= 0)
		{
			iMsgEventID = MsgEventID::MSGID_SEND_SYSMSG;
			_stprintf_s(m_pzBuffer, _T("%s"), _T("输入不能为空!"));
			bInputError = true;
			goto lab_err;
		}
		else if (strMsg.GetLength() > SYS_MSG_LEN)
		{
			iMsgEventID = MsgEventID::MSGID_SEND_SYSMSG;
			_stprintf_s(m_pzBuffer, _T("%s"), _T("输入消息过长!"));
			bInputError = true;
			goto lab_err;
		}
		else if (!m_pOptSendToGame->IsSelected() && !m_pOptSendToRoom->IsSelected())
		{
			iMsgEventID = MsgEventID::MSGID_SEND_SYSMSG_RANG;
			_stprintf_s(m_pzBuffer, _T("%s"), _T("发送范围至少勾选一个!"));
			bInputError = true;
			goto lab_err;
		}
		else
		{
			if (g_pDelegate)
			{
				bool bSendToGame = m_pOptSendToGame ? m_pOptSendToGame->IsSelected() : false;
				bool bSendToRoom = m_pOptSendToRoom ? m_pOptSendToRoom->IsSelected() : false;
				g_pDelegate->CallBackSendSysMsg(strMsg.GetData(), bSendToGame, bSendToRoom);
			}
			this->Close();
		}

	lab_err:
		if (bInputError)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(SendSysMsg::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, m_pzBuffer
				, (DLG_ID)iMsgEventID
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}
	}
	
	void SendSysMsg::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		switch (id)
		{
			case MsgEventID::MSGID_SEND_SYSMSG:
			{
				m_pEdtSysMsg->SetFocus();
			}break;

			case MsgEventID::MSGID_SEND_SYSMSG_RANG:
			{
				m_pOptSendToGame->SetFocus();
				m_pOptSendToGame->SetFocusBorderOnDraw(true);
			}break;

			default:
				break;
		}
	}

	void SendSysMsg::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE SendSysMsg::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR SendSysMsg::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString SendSysMsg::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString SendSysMsg::GetSkinFile()
	{
		return IDR_XML_PanelSendSys;
	}

	LRESULT SendSysMsg::ResponseDefaultKeyEvent(WPARAM wParam)
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
