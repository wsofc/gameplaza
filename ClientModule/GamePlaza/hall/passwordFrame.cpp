#include "stdafx.h"
#include "passwordFrame.h"
#include "loginFrame.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;
class HallFrame;

PasswordFrame::PasswordFrame()
	: m_pEdtAccount(nullptr)
	, m_pCtlAccount(nullptr)
	, m_pEdtTelphone(nullptr)
	, m_pCtlTelphone(nullptr)
	, m_pEdtVertCode(nullptr)
	, m_pCtlVertCode(nullptr)
	, m_pEdtPassword(nullptr)
	, m_pCtlPassword(nullptr)
	, m_pEdtPasswordEx(nullptr)
	, m_pCtlPasswordEx(nullptr)
	, m_pBtnGetCode(nullptr)
	, m_pLoginServer(nullptr)
	, m_pParentManager(nullptr)
	, m_llVertcodeSec(0)
	, m_iCurOperateStatus(OperateStatus::OS_NONE)
{
}

PasswordFrame::~PasswordFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);

	LoginFrame* pLoginFrame = LoginFrame::GetInstance();
	pLoginFrame->ShowWindow(true);
	m_pLoginServer->SetDelegate(pLoginFrame, m_pParentManager);

	g_Logger->TryInfo(_T("PasswordFrame - Destroy"));
}

BOOL PasswordFrame::CreateFrame(CPaintManagerUI* pParentManager)
{
	m_pParentManager = pParentManager;
	if (0 == IsWindow(m_hWnd))
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

UILIB_RESOURCETYPE PasswordFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR PasswordFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString PasswordFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString PasswordFrame::GetSkinFile()
{
	return _T("password.xml");
}

LRESULT PasswordFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == CMW_CLOSE_SURE)
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	g_GameUtil->MinimizeTray(*this);
	return 0;
}

void PasswordFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("PasswordFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
};

void PasswordFrame::InitWindow()
{
	g_Logger->TryInfo(_T("PasswordFrame - Open"));

	// 账号
	m_pEdtAccount = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtPassword_Account));
	m_pCtlAccount = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlPassword_Account));
	m_PaintManager.SetFocusNeeded(m_pEdtAccount);
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_ACCOUNT));

	// 手机号
	m_pEdtTelphone = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtPassword_Telphone));
	m_pCtlTelphone = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlPassword_Telphone));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_TELPHONE));

	// 验证码
	m_pEdtVertCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtPassword_VertCode));
	m_pCtlVertCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlPassword_VertCode));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_VERTCODE));

	// 新密码
	m_pEdtPassword = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtPassword_Password));
	m_pCtlPassword = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlPassword_Password));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_NEW_PW));

	// 确认密码
	m_pEdtPasswordEx = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtPassword_PasswordEx));
	m_pCtlPasswordEx = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlPassword_PasswordEx));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_CFM_PW));

	// 获取验证码按钮
	m_pBtnGetCode = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnPassword_GetVertcode));
	m_pBtnGetCode->SetEnabled(true);
}

void PasswordFrame::InitData(TNotifyUI& msg)
{
	m_pEdtAccount->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlAccount, 0);

	m_pEdtTelphone->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlTelphone, 0);

	m_pEdtVertCode->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlVertCode, 0);

	m_pEdtPassword->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlPassword, 0);

	m_pEdtPasswordEx->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlPasswordEx, 0);

	// 设置登录服委托对象
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);
	if (!m_pLoginServer->IsConnect())
	{
		m_iCurOperateStatus = OperateStatus::OS_NONE;
		m_pLoginServer->Connect();
	}

	g_Logger->TryInfo(_T("PasswordFrame - Open Complete"));
}

void PasswordFrame::ChangeTipsImage(CControlUI* pControl, DWORD dwStatus)
{
	if (!pControl) return;

	LPCTSTR pzStatus = _T("");
	switch (dwStatus)
	{
		case 0:
		{
			pzStatus = _T("");
		}break;

		case 1:
		{
			pzStatus = _T("file='common/blocks/tooltip.png' source='0,0,16,16'");
		}break;

		case 2:
		{
			pzStatus = _T("file='common/blocks/tooltip.png' source='16,0,32,32'");
		}break;

		default:
			break;
	}

	pControl->SetBkImage(pzStatus);
}

void PasswordFrame::Check()
{
	bool bRes = true;
	uint uMsgID = MsgBoxID::MSG_BOX_ID_NONE;
	for (const auto& it : m_vCompleteFlag)
	{
		bRes &= it.bRes;
		uMsgID = it.uMsgID;
		if (!bRes) break;
	}

	if (bRes)
	{
		//发送
		tagLS_GetPassword stuVetPassword;
		::StrToCharArray(stuVetPassword.szAccount, m_pEdtAccount->GetText().GetData());
		::StrToCharArray(stuVetPassword.szSMS, m_pEdtVertCode->GetText().GetData());
		::StrToCharArray(stuVetPassword.szNewPwd, m_pEdtPassword->GetText().GetData());
		CMD5Encrypt::EncryptData(stuVetPassword.szNewPwd, stuVetPassword.szNewPwd);

		g_Logger->TryInfo(_T("PasswordFrame - Modify Pw Req"));

		DWORD iRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_C2LS_GETPASSWORD, &stuVetPassword, sizeof(tagLS_GetPassword));
		if (FALSE == iRes)
		{
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("修改密码验证失败，请检查您的网络!"));
		}
	}
	else
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("重置密码")
			, _T("填写信息不正确!")
			, DLG_ID(uMsgID)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
}

void PasswordFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case MsgBoxID::MSG_BOX_ID_ACCOUNT:		m_pEdtAccount->SetFocus();		break;
		case MsgBoxID::MSG_BOX_ID_TELPHONE:		m_pEdtTelphone->SetFocus();		break;
		case MsgBoxID::MSG_BOX_ID_VERTCODE:		m_pEdtVertCode->SetFocus();		break;
		case MsgBoxID::MSG_BOX_ID_NEW_PW:		m_pEdtPassword->SetFocus();		break;
		case MsgBoxID::MSG_BOX_ID_CFM_PW:		m_pEdtPasswordEx->SetFocus();	break;
		case MsgBoxID::MSG_BOX_ID_SUCCESS:		this->TurnToLoginUI();			break;
		default:																break;
	}
}

void PasswordFrame::SetVertcodeTimer()
{
	m_pBtnGetCode->SetEnabled(false);

	TimerCallBack timepro = CC_CALLBACK_2(PasswordFrame::OnVertcodeTimer, this);
	g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_GET_VERTCODE, 1000);
	m_llVertcodeSec = TIME_GET_VERTCODE;

	//开启定时器时即可设置Text显示
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	_stprintf_s(pzBuffer, _T("%ld秒后重新获取"), m_llVertcodeSec);
	m_pBtnGetCode->SetText(pzBuffer);
}

void PasswordFrame::OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime)
{
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	if (--m_llVertcodeSec <= 0)
	{
		m_llVertcodeSec = TIME_GET_VERTCODE;
		m_pBtnGetCode->SetText(_T("点击获取验证码"));
		m_pBtnGetCode->SetEnabled(true);
		g_GameUtil->KillTimer(m_hWnd, TIMER_ID_GET_VERTCODE);
	}
	else
	{
		_stprintf_s(pzBuffer, _T("%ld秒后重新获取"), m_llVertcodeSec);
		m_pBtnGetCode->SetText(pzBuffer);
	}
}

void PasswordFrame::SetEdtCtlEnable(bool bEnable)
{
	m_pEdtAccount->SetEnabled(false);
	m_pEdtTelphone->SetEnabled(false);
}

void PasswordFrame::TurnToLoginUI()
{
	LoginFrame* pLoginFrame = LoginFrame::GetInstance();
	pLoginFrame->RefreshLoginData(m_pEdtAccount->GetText(), _T(""));

	this->Close(CMW_CLOSE_SURE);
}

bool PasswordFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case OperateStatus::OS_NONE:								break;
		case OperateStatus::OS_GETVCODE:	this->GetServerSMS();	break;
		case OperateStatus::OS_MODIFY:		this->Check();			break;
		case OperateStatus::OS_EXTRA:								break;
		default:													break;
	}
	m_iCurOperateStatus = OperateStatus::OS_NONE;

	return true;
}

void PasswordFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

void PasswordFrame::GetServerSMS()
{
	//从服务端获取短信验证码
	tag_C_GetSMS stuSMS;
	stuSMS.btType = 2;
	_stprintf_s(stuSMS.szAccount,	_T("%s"),	m_pEdtAccount->GetText());
	_stprintf_s(stuSMS.szPhone,		_T("%s"),	m_pEdtTelphone->GetText());

	//发送
	g_Logger->TryInfo(_T("PasswordFrame - Get SMS Req"));
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_SMS, SUB_C2LS_GETSMS, &stuSMS, sizeof(tag_C_GetSMS));
	if (FALSE == iRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示")
			, _T("获取验证码失败，请检查您的网络!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

bool PasswordFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}

bool PasswordFrame::OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_SENDSMSSUCCESS:
		{
			g_Logger->TryInfo(_T("PasswordFrame - Get SMS OK"));

			DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::MsgBoxProcFocus, this);
			DialogFrame::Toast(&m_PaintManager, _T("验证码发送成功!"), (DLG_ID)MsgBoxID::MSG_BOX_ID_VERTCODE, dlg_cb);

			// 验证码按钮开始间隔时间计时
			this->SetVertcodeTimer();

			// 发送验证码成功后,禁止账号、手机号修改
			this->SetEdtCtlEnable(false);
		}break;

		case SUB_LS2C_SMS_ERROR:
		{
			tag_S_ErrorMsg stu_ErrMSG;
			memset(&stu_ErrMSG, 0, sizeof(tag_S_ErrorMsg));
			memcpy(&stu_ErrMSG, pDataBuffer, wDataSize);

			g_Logger->TryInfo(_T("PasswordFrame - Get SMS Failed [code = %u]"), stu_ErrMSG.btErrorCode);

			int iMsgBoxID = MsgBoxID::MSG_BOX_ID_VERTCODE;
			switch (stu_ErrMSG.btErrorCode)
			{
				case 1:
				{
					iMsgBoxID = MsgBoxID::MSG_BOX_ID_ACCOUNT;
				}break;

				case 2:
				{
					iMsgBoxID = MsgBoxID::MSG_BOX_ID_TELPHONE;
				}break;

				default:
					break;
			}

			DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, stu_ErrMSG.szMsg
				, DLG_ID(iMsgBoxID)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}break;

		default:
			break;
	}
	return true;
}

bool PasswordFrame::OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_GETPASSWORD_SSUCCESS:
		{
			g_Logger->TryInfo(_T("PasswordFrame - Modify Pw OK"));
			DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示")
				, _T("重置成功!")
				, DLG_ID(MsgBoxID::MSG_BOX_ID_SUCCESS)
				, DLG_STYLE::DLG_OK
				, dlg_cb
				, nullptr
				, dlg_cb);
		}break;

		case SUB_LS2C_GETPASSWORD_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;

			g_Logger->TryInfo(_T("PasswordFrame - Modify Pw Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { 0 };
			_stprintf_s(pzErrorMSG, _T("重置密码失败：%s"), stu_ErrMSG->szMsg);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrorMSG);
		}break;

		default:
			break;
	}
	return true;
}

void PasswordFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
		this->InitData(msg);
	else if (_tcscmp(type, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnPassword_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnPassword_Cancel) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnPassword_GetVertcode) == 0)
		{
			m_pEdtVertCode->SetText(_T(""));
			if (m_vCompleteFlag[0].bRes && m_vCompleteFlag[1].bRes)
			{
				if (m_pLoginServer->IsConnect())
				{
					this->GetServerSMS();
				}
				else
				{
					m_iCurOperateStatus = OperateStatus::OS_GETVCODE;
					m_pLoginServer->Connect();
				}
			}
			else if (!m_vCompleteFlag[0].bRes)
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("关联账号输入不正确!")
					, DLG_ID(MsgBoxID::MSG_BOX_ID_ACCOUNT)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
			else if (!m_vCompleteFlag[1].bRes)
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(PasswordFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("关联手机号输入不正确!")
					, DLG_ID(MsgBoxID::MSG_BOX_ID_TELPHONE)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
		}
		else if (_tcscmp(sName, R->kBtnPassword_Ensure) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->Check();
			}
			else
			{
				m_iCurOperateStatus = OperateStatus::OS_MODIFY;
				m_pLoginServer->Connect();
			}
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_SETFOCUS) == 0)
	{
		if (_tcscmp(sName, R->kEdtPassword_Account) == 0)
		{
			this->ChangeTipsImage(m_pCtlAccount, 0);
		}
		else if (_tcscmp(sName, R->kEdtPassword_Telphone) == 0)
		{
			this->ChangeTipsImage(m_pCtlTelphone, 0);
		}
		else if (_tcscmp(sName, R->kEdtPassword_VertCode) == 0)
		{
			this->ChangeTipsImage(m_pCtlVertCode, 0);
		}
		else if (_tcscmp(sName, R->kEdtPassword_Password) == 0)
		{
			this->ChangeTipsImage(m_pCtlPassword, 0);
		}
		else if (_tcscmp(sName, R->kEdtPassword_PasswordEx) == 0)
		{
			this->ChangeTipsImage(m_pCtlPasswordEx, 0);
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_KILLFOCUS) == 0)
	{
		if (_tcscmp(sName, R->kEdtPassword_Account) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_ACCOUNT);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlAccount, dwStatus);

			m_vCompleteFlag[0].bRes = bRes;
		}
		else if (_tcscmp(sName, R->kEdtPassword_Telphone) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_TELPHONE);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlTelphone, dwStatus);

			m_vCompleteFlag[1].bRes = bRes;
		}
		else if (_tcscmp(sName, R->kEdtPassword_VertCode) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_VERTCODE);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlVertCode, dwStatus);

			m_vCompleteFlag[2].bRes = bRes;
		}
		else if (_tcscmp(sName, R->kEdtPassword_Password) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_PASSWORD);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlPassword, dwStatus);

			m_vCompleteFlag[3].bRes = bRes;
		}
		else if (_tcscmp(sName, R->kEdtPassword_PasswordEx) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = false;
			bRes = m_pEdtPasswordEx->GetText().GetLength() > 0;
			bRes &= m_pEdtPasswordEx->GetText() == m_pEdtPassword->GetText();
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlPasswordEx, dwStatus);

			m_vCompleteFlag[4].bRes = bRes;
		}
	}

	//__super::Notify(msg);
}

LRESULT PasswordFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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