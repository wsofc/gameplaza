#include "stdafx.h"
#include "lockframe.h"
#include "util/gameUtil.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

// --------------------------------------------------------------------------------------------------------------------------------------
// SMSLonginCheckFrame

SMSLonginCheckFrame* SMSLonginCheckFrame::g_instance = nullptr;
SMSLonginCheckFrame::SMSLonginCheckFrame()
	: m_pParentManager(nullptr)
	, m_pLoginServer(nullptr)
	, m_pEdtVertCode(nullptr)
	, m_pBtnGetCode(nullptr)
	, m_pBtnLogin(nullptr)
	, m_llVertcodeSec(0)
	, m_iCurOperateStatus(OperateStatus::OS_NONE)
{

}

SMSLonginCheckFrame::~SMSLonginCheckFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);

	LoginFrame* pLoginFrame = LoginFrame::GetInstance();
	pLoginFrame->SetWaittingDlgManager(nullptr);
	m_pLoginServer->SetDelegate(pLoginFrame, m_pParentManager);
	
	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Destroy"));
}

SMSLonginCheckFrame* SMSLonginCheckFrame::GetInstance()
{
	if (SMSLonginCheckFrame::g_instance == nullptr)
	{
		SMSLonginCheckFrame::g_instance = new (std::nothrow)SMSLonginCheckFrame();
	}
	return SMSLonginCheckFrame::g_instance;
}

UILIB_RESOURCETYPE SMSLonginCheckFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR SMSLonginCheckFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString SMSLonginCheckFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString SMSLonginCheckFrame::GetSkinFile()
{
	return _T("hall\\lock\\sms_check.xml");
}

LRESULT SMSLonginCheckFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

BOOL SMSLonginCheckFrame::CreateFrame(CPaintManagerUI* pParentManager, LPCTSTR pzAccount)
{
	m_pParentManager = pParentManager;
	m_pzAccount = pzAccount;
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

void SMSLonginCheckFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(SMSLonginCheckFrame::g_instance);
}

void SMSLonginCheckFrame::InitWindow()
{
	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Open"));
	m_pEdtVertCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtSMSCheck_VCode));

	// 获取验证码按钮
	m_pBtnGetCode = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnSMSCheck_GetVertcode));
	if (m_pBtnGetCode)
	{
		m_pBtnGetCode->SetEnabled(true);
	}


	// 登录按钮
	m_pBtnLogin = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnSMSCheck_Login));
	if (m_pBtnLogin)
	{
		m_pBtnLogin->SetEnabled(false);
	}
}

void SMSLonginCheckFrame::InitData(TNotifyUI& msg)
{
	// 设置登录服委托对象
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);
	if (!m_pLoginServer->IsConnect())
	{
		m_iCurOperateStatus = OperateStatus::OS_NONE;
		m_pLoginServer->Connect();
	}

	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Open Complete"));
}

void SMSLonginCheckFrame::Check()
{
	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Check"));

	if (!m_pEdtVertCode) return;
	CDuiString& pzVcode = m_pEdtVertCode->GetText();
	if (pzVcode.GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(SMSLonginCheckFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("验证码不能为空!")
			, DLG_ID(MsgBoxID::MSG_BOX_ID_VERTCODE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (!g_GameUtil->MatchWordByType(pzVcode, MatchWords_Type::REG_TYPE_VERTCODE))
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(SMSLonginCheckFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("验证码输入不正确!")
			, DLG_ID(MsgBoxID::MSG_BOX_ID_VERTCODE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else
	{
		LoginFrame* pLoginFrame = LoginFrame::GetInstance();
		m_pLoginServer->SetDelegate(pLoginFrame, m_pParentManager);
		pLoginFrame->SetWaittingDlgManager(&m_PaintManager);
		pLoginFrame->SetLoginSMS(pzVcode);
		pLoginFrame->LoginCheck();
	}

	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Check Complete"));
}

void SMSLonginCheckFrame::SMSCheckFailed(LPCTSTR lpErrorMsg)
{
	if (!lpErrorMsg || _tcscmp(lpErrorMsg, _T("")) == 0) return;

	DialogCallBack dlg_cb = CC_CALLBACK_2(SMSLonginCheckFrame::MsgBoxProcFocus, this);
	DialogFrame::MsgBox(&m_PaintManager
		, _T("温馨提示:")
		, lpErrorMsg
		, DLG_ID(MsgBoxID::MSG_BOX_ID_VERTCODE)
		, DLG_STYLE::DLG_DEFAULT
		, dlg_cb
		, dlg_cb
		, dlg_cb);
}

void SMSLonginCheckFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case MsgBoxID::MSG_BOX_ID_NONE:
		case MsgBoxID::MSG_BOX_ID_DEFAULT:
		case MsgBoxID::MSG_BOX_ID_VERTCODE:
		default:
		{
			m_pEdtVertCode->SetFocus();
		}break;
	}
}

bool SMSLonginCheckFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case OperateStatus::OS_NONE:								break;
		case OperateStatus::OS_GETVCODE:	this->GetServerSMS();	break;
		case OperateStatus::OS_EXTRA:								break;
		default:													break;
	}
	m_iCurOperateStatus = OperateStatus::OS_NONE;

	return true;
}

bool SMSLonginCheckFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}

void SMSLonginCheckFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

bool SMSLonginCheckFrame::OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_SENDSMSSUCCESS:
		{
			g_Logger->TryInfo(_T("SMSLonginCheckFrame - Get SMS OK"));

			DialogCallBack dlg_cb = CC_CALLBACK_2(SMSLonginCheckFrame::MsgBoxProcFocus, this);
			DialogFrame::Toast(&m_PaintManager, _T("验证码发送成功!"), (DLG_ID)MsgBoxID::MSG_BOX_ID_VERTCODE, dlg_cb);

			// 验证码按钮开始间隔时间计时
			this->SetVertcodeTimer();

			// 解除"登录按钮"禁用状态
			if (!m_pBtnLogin)
			{
				m_pBtnLogin = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnSMSCheck_Login));
			}
			m_pBtnLogin->SetEnabled(true);
		}break;

		case SUB_LS2C_SMS_ERROR:
		{
			tag_S_ErrorMsg stu_ErrMSG;
			memset(&stu_ErrMSG, 0, sizeof(tag_S_ErrorMsg));
			memcpy(&stu_ErrMSG, pDataBuffer, wDataSize);

			g_Logger->TryInfo(_T("SMSLonginCheckFrame - Get SMS Failed [code = %u]"), stu_ErrMSG.btErrorCode);
			DialogCallBack dlg_cb = CC_CALLBACK_2(SMSLonginCheckFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, stu_ErrMSG.szMsg
				, DLG_ID(MsgBoxID::MSG_BOX_ID_VERTCODE)
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

void SMSLonginCheckFrame::GetServerSMS()
{
	//从服务端获取短信验证码
	tag_C_GetSMS stuSMS;
	stuSMS.btType = 8;
	::StrToCharArray(stuSMS.szAccount, m_pzAccount);
	
	//发送
	g_Logger->TryInfo(_T("SMSLonginCheckFrame - Get SMS Req"));
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_SMS, SUB_C2LS_GETSMS, &stuSMS, sizeof(tag_C_GetSMS));
	if (FALSE == iRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(SMSLonginCheckFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示")
			, _T("获取验证码失败，请检查您的网络!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void SMSLonginCheckFrame::SetVertcodeTimer()
{
	m_pBtnGetCode->SetEnabled(false);

	TimerCallBack timepro = CC_CALLBACK_2(SMSLonginCheckFrame::OnVertcodeTimer, this);
	g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_GET_VERTCODE, 1000);
	m_llVertcodeSec = TIME_GET_VERTCODE;

	//开启定时器时即可设置Text显示
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	_stprintf_s(pzBuffer, _T("%ld秒后重新获取"), m_llVertcodeSec);
	m_pBtnGetCode->SetText(pzBuffer);
}

void SMSLonginCheckFrame::OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime)
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

void SMSLonginCheckFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(type, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnSMSCheck_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnSMSCheck_GetVertcode) == 0)
		{
			m_pEdtVertCode->SetText(_T(""));
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
		else if (_tcscmp(sName, R->kBtnSMSCheck_Login) == 0)
		{
			this->Check();
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------
// LockLoginFrame
LockLoginFrame::LockLoginFrame()
	: m_pParentManager(nullptr)
	, m_pEdtAccount(nullptr)
	, m_pEdtTelphone(nullptr)
	, m_pCtlTelphone(nullptr)
	, m_pEdtVertCode(nullptr)
	, m_pCtlVertCode(nullptr)
	, m_pBtnGetCode(nullptr)
	, m_pLoginServer(nullptr)
	, m_llVertcodeSec(0)
	, m_iCurOperateStatus(OperateStatus::OS_NONE)
{
}

LockLoginFrame::~LockLoginFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);

	LoginFrame* pLoginFrame = LoginFrame::GetInstance();
	pLoginFrame->ShowWindow(true);
	m_pLoginServer->SetDelegate(pLoginFrame, m_pParentManager);

	g_Logger->TryInfo(_T("LockLoginFrame - Destroy"));
}

BOOL LockLoginFrame::CreateFrame(CPaintManagerUI* pParentManager)
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

UILIB_RESOURCETYPE LockLoginFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR LockLoginFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString LockLoginFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString LockLoginFrame::GetSkinFile()
{
	return _T("hall\\lock\\login_lock.xml");
}

LRESULT LockLoginFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == CMW_CLOSE_SURE)
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	g_GameUtil->MinimizeTray(*this);
	return 0;
}

void LockLoginFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("LockLoginFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
};

void LockLoginFrame::InitWindow()
{
	g_Logger->TryInfo(_T("LockLoginFrame - Open"));

	// 账号
	m_pEdtAccount = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtUnBind_Account));
	m_pCtlAccount = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlUnBind_Account));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_ACCOUNT));

	// 手机号
	m_pEdtTelphone = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtUnBind_Telphone));
	m_pCtlTelphone = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlUnBind_Telphone));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_TELPHONE));

	// 验证码
	m_pEdtVertCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtUnBind_VertCode));
	m_pCtlVertCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kCtlUnBind_VertCode));
	m_vCompleteFlag.push_back(stuMsgBoxFlag(false, MsgBoxID::MSG_BOX_ID_VERTCODE));

	// 获取验证码按钮
	m_pBtnGetCode = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnUnBind_GetVertcode));
	m_pBtnGetCode->SetEnabled(true);
}

void LockLoginFrame::InitData(TNotifyUI& msg)
{
	m_pEdtAccount->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlAccount, 0);

	m_pEdtTelphone->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlTelphone, 0);

	m_pEdtVertCode->SetText(_T(""));
	this->ChangeTipsImage(m_pCtlVertCode, 0);


	// 设置登录服委托对象
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);
	if (!m_pLoginServer->IsConnect())
	{
		m_iCurOperateStatus = OperateStatus::OS_NONE;
		m_pLoginServer->Connect();
	}

	g_Logger->TryInfo(_T("LockLoginFrame - Open Complete"));
}

void LockLoginFrame::ChangeTipsImage(CControlUI* pControl, DWORD dwStatus)
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

void LockLoginFrame::Check()
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
		tagLS_LockMachine stuLockMachine;
		stuLockMachine.bLock = false;
		::StrToCharArray(stuLockMachine.szAccount, m_pEdtAccount->GetText().GetData());
		::StrToCharArray(stuLockMachine.szSMS, m_pEdtVertCode->GetText().GetData());

		g_Logger->TryInfo(_T("LockLoginFrame - Unlock Machine Req"));
		DWORD iRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_LS_USER_UNLOCK_MACHINE, &stuLockMachine, sizeof(tagLS_LockMachine));
		if (FALSE == iRes)
		{
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("修改密码验证失败，请检查您的网络!"));
		}
	}
	else
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LockLoginFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("填写信息不正确!")
			, DLG_ID(uMsgID)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
}

void LockLoginFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case LockLoginFrame::MSG_BOX_ID_NONE:										break;
		case LockLoginFrame::MSG_BOX_ID_ACCOUNT:	m_pEdtAccount->SetFocus();		break;
		case LockLoginFrame::MSG_BOX_ID_TELPHONE:	m_pEdtTelphone->SetFocus();		break;
		case LockLoginFrame::MSG_BOX_ID_VERTCODE:	m_pEdtVertCode->SetFocus();		break;
		case LockLoginFrame::MSG_BOX_EXTRA:											break;
		default:																	break;
	}
}

void LockLoginFrame::SetVertcodeTimer()
{
	m_pBtnGetCode->SetEnabled(false);

	TimerCallBack timepro = CC_CALLBACK_2(LockLoginFrame::OnVertcodeTimer, this);
	g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_GET_VERTCODE, 1000);
	m_llVertcodeSec = TIME_GET_VERTCODE;

	//开启定时器时即可设置Text显示
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	_stprintf_s(pzBuffer, _T("%ld秒后重新获取"), m_llVertcodeSec);
	m_pBtnGetCode->SetText(pzBuffer);
}

void LockLoginFrame::OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime)
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

void LockLoginFrame::SetEdtCtlEnable(bool bEnable)
{
	m_pEdtAccount->SetEnabled(false);
	m_pEdtTelphone->SetEnabled(false);
}

bool LockLoginFrame::OnConnectSuccessProc()
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

void LockLoginFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

void LockLoginFrame::GetServerSMS()
{
	//从服务端获取短信验证码
	tag_C_GetSMS stuSMS;
	stuSMS.btType = 7;
	
	::StrToCharArray(stuSMS.szAccount, m_pEdtAccount->GetText().GetData());
	::StrToCharArray(stuSMS.szPhone, m_pEdtTelphone->GetText().GetData());

	//发送
	g_Logger->TryInfo(_T("LockLoginFrame - Get SMS Req"));
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_SMS, SUB_C2LS_GETSMS, &stuSMS, sizeof(tag_C_GetSMS));
	if (FALSE == iRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LockLoginFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示")
			, _T("获取验证码失败，请检查您的网络!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

bool LockLoginFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}

bool LockLoginFrame::OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_SENDSMSSUCCESS:
		{
			g_Logger->TryInfo(_T("LockLoginFrame - Get SMS OK"));

			DialogCallBack dlg_cb = CC_CALLBACK_2(LockLoginFrame::MsgBoxProcFocus, this);
			DialogFrame::Toast(&m_PaintManager, _T("验证码发送成功!"), (DLG_ID)MsgBoxID::MSG_BOX_ID_VERTCODE, dlg_cb);

			// 验证码按钮开始间隔时间计时
			this->SetVertcodeTimer();

			// 发送验证码成功后,禁用相关输入
			this->SetEdtCtlEnable(false);
		}break;

		case SUB_LS2C_SMS_ERROR:
		{
			tag_S_ErrorMsg stu_ErrMSG;
			memset(&stu_ErrMSG, 0, sizeof(tag_S_ErrorMsg));
			memcpy(&stu_ErrMSG, pDataBuffer, wDataSize);

			g_Logger->TryInfo(_T("LockLoginFrame - Get SMS Failed [code = %u]"), stu_ErrMSG.btErrorCode);

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
			
			DialogCallBack dlg_cb = CC_CALLBACK_2(LockLoginFrame::MsgBoxProcFocus, this);
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

bool LockLoginFrame::OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS_USER_LOCK_MACHINE_OK:
		{
		}break;

		case SUB_LS_USER_LOCK_MACHINE_ERROR:
		{
		}break;

		case SUB_LS_USER_UNLOCK_MACHINE_OK:
		{
			g_Logger->TryInfo(_T("LockLoginFrame - Unlock Machine OK"));

			DialogFrame::ToastBox(m_pParentManager, _T("解锁成功!"), true);
			this->Close(CMW_CLOSE_SURE);
		}break;

		case SUB_LS_USER_UNLOCK_MACHINE_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;

			g_Logger->TryInfo(_T("LockLoginFrame - Unlock Machine Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { _T("解锁失败") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrorMSG);
		}break;

		default:
			break;
	}
	return true;
}

void LockLoginFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(type, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnUnBind_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnUnBind_Cancel) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnUnBind_GetVertcode) == 0)
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
				DialogCallBack dlg_cb = CC_CALLBACK_2(LockLoginFrame::MsgBoxProcFocus, this);
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
				DialogCallBack dlg_cb = CC_CALLBACK_2(LockLoginFrame::MsgBoxProcFocus, this);
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
		else if (_tcscmp(sName, R->kBtnUnBind_Ensure) == 0)
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
		if (_tcscmp(sName, R->kEdtUnBind_Account) == 0)
		{
			this->ChangeTipsImage(m_pCtlAccount, 0);
		}
		else if (_tcscmp(sName, R->kEdtUnBind_Telphone) == 0)
		{
			this->ChangeTipsImage(m_pCtlTelphone, 0);
		}
		else if (_tcscmp(sName, R->kEdtUnBind_VertCode) == 0)
		{
			this->ChangeTipsImage(m_pCtlVertCode, 0);
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_KILLFOCUS) == 0)
	{
		if (_tcscmp(sName, R->kEdtUnBind_Account) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_ACCOUNT);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlAccount, dwStatus);

			m_vCompleteFlag[0].bRes = bRes;
		}
		else if (_tcscmp(sName, R->kEdtUnBind_Telphone) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_TELPHONE);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlTelphone, dwStatus);

			m_vCompleteFlag[1].bRes = bRes;
		}
		else if (_tcscmp(sName, R->kEdtUnBind_VertCode) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_VERTCODE);
			dwStatus = bRes ? 2 : 1;
			this->ChangeTipsImage(m_pCtlVertCode, dwStatus);

			m_vCompleteFlag[2].bRes = bRes;
		}
	}

	//__super::Notify(msg);
}

LRESULT LockLoginFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

// --------------------------------------------------------------------------------------------------------------------------------------
// LockMachineFrame
LockMachineFrame* LockMachineFrame::g_instance = nullptr;
LockMachineFrame::LockMachineFrame()
	: m_pLoginServer(nullptr)
	, m_pEdtBankPwCtl(nullptr)
	, m_pParentManager(nullptr)
	, m_iCurOperateStatus(LockOperateStatus::LOCK_OS_NONE)
{
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);

	m_pDataManager = DataManager::GetInstance();
}

LockMachineFrame::~LockMachineFrame()
{
	if (m_pParentManager)
	{
		g_GameUtil->SetFrozenEnable(m_pParentManager, true);
		g_GameUtil->ShowTopWindow(m_pParentManager->GetPaintWindow());
	}

	if (LoginServer::IsInstanceExist())
	{
		m_pLoginServer->SetDelegate(nullptr, m_pParentManager);
		m_pLoginServer->CloseSocket();
	}

	g_Logger->TryInfo(_T("LockMachineFrame - Destroy"));
}

LockMachineFrame* LockMachineFrame::GetInstance()
{
	if (LockMachineFrame::g_instance == NULL)
		LockMachineFrame::g_instance = new (std::nothrow)LockMachineFrame();

	return LockMachineFrame::g_instance;
}

BOOL LockMachineFrame::CreateFrame(CPaintManagerUI* pParentManager)
{
	m_pParentManager = pParentManager;
	if (FALSE == IsWindow(m_hWnd))
	{
		HWND hParentWnd = NULL;
		if (m_pParentManager)
		{
			hParentWnd = m_pParentManager->GetPaintWindow();
			g_GameUtil->SetFrozenEnable(m_pParentManager, false);
		}

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

UILIB_RESOURCETYPE LockMachineFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR LockMachineFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString LockMachineFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString LockMachineFrame::GetSkinFile()
{
	return _T("hall\\lock\\lock.xml");
}

void LockMachineFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("LockMachineFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	
	SAFE_DELETE(LockMachineFrame::g_instance);
}

void LockMachineFrame::InitWindow()
{
	g_Logger->TryInfo(_T("LockMachineFrame - Open"));

	m_pEdtBankPwCtl = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtLock_BankPw));
}

void LockMachineFrame::InitData(TNotifyUI& msg)
{
	g_Logger->TryInfo(_T("LockMachineFrame - Open Complete"));
}

void LockMachineFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnLock_Close) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnLock_Cancel) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnLock_Lock) == 0)
		{
			if (m_pEdtBankPwCtl->GetText().GetLength() <= 0)
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(LockMachineFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("银行密码不能为空!")
					, DLG_ID(LockMsgBoxID::MSG_BOX_ID_LOCK_PW)
					, DLG_STYLE::DLG_OK
					, dlg_cb);
			}
			else
			{
				if (m_pLoginServer->IsConnect())
				{
					this->LockMachine(true);
				}
				else
				{
					m_iCurOperateStatus = LockOperateStatus::LOCK_OS_LOCKMACHINE;
					m_pLoginServer->Connect();
				}
			}
		}
		else if (_tcscmp(sName, R->kBtnLock_UnLock) == 0)
		{
			if (m_pEdtBankPwCtl->GetText().GetLength() <= 0)
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(LockMachineFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("银行密码不能为空!")
					, DLG_ID(LockMsgBoxID::MSG_BOX_ID_LOCK_PW)
					, DLG_STYLE::DLG_OK
					, dlg_cb);
			}
			else
			{
				if (m_pLoginServer->IsConnect())
				{
					this->LockMachine(false);
				}
				else
				{
					m_iCurOperateStatus = LockOperateStatus::LOCK_OS_UNLOCKMACHINE;
					m_pLoginServer->Connect();
				}
			}
		}
	}

	//__super::Notify(msg);
}

LRESULT LockMachineFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return FALSE;
	}
	return FALSE;
}

void LockMachineFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case LockMsgBoxID::MSG_BOX_ID_LOCK_PW:
		{
			m_pEdtBankPwCtl->SetFocus();
		}break;

		default:
			break;
	}
}

void LockMachineFrame::TimerProFunc(UINT_PTR idEvent, DWORD dwTime)
{
	switch (idEvent)
	{
		case TIMER_ID_HALL_LOCK_MACHINE:
		{
			CButtonUI* pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnHallMain_LockMachine));
			if (pBtn)
			{
				pBtn->SetEnabled(true);
			}
		}break;

		default:
			break;
	}
}

bool LockMachineFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case LockOperateStatus::LOCK_OS_NONE:			break;
		case LockOperateStatus::LOCK_OS_LOCKMACHINE:	this->LockMachine(true);	break;
		case LockOperateStatus::LOCK_OS_UNLOCKMACHINE:	this->LockMachine(false);	break;
		case LockOperateStatus::LOCK_OS_EXTRA:			break;
		default:										break;
	}
	m_iCurOperateStatus = LockOperateStatus::LOCK_OS_NONE;

	return true;
}

bool LockMachineFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}

void LockMachineFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

void LockMachineFrame::LockMachine(BOOL lbLock)
{
	UserData& tSelf = m_pDataManager->GetSelfData();

	tagLS_LockMachine tLockMachine;
	memset(&tLockMachine, 0, sizeof(tagLS_LockMachine));
	tLockMachine.bLock = lbLock;

	CDuiString& pzBankPw = m_pEdtBankPwCtl->GetText();
	::StrToCharArray(tLockMachine.szAccount, tSelf.GetAccount());
	::StrToCharArray(tLockMachine.szBankPwd, pzBankPw.GetData());
	CMD5Encrypt::EncryptData(tLockMachine.szBankPwd, tLockMachine.szBankPwd);
	::StrToCharArray(tLockMachine.szComputerID, tSelf.GetMachineID());
	
	// 锁定
	DWORD dwSubCmd = 0;
	LPCTSTR pzErrStr = _T("");
	if (TRUE == lbLock)
	{
		dwSubCmd = SUB_LS_USER_LOCK_MACHINE;
		pzErrStr = _T("锁定失败!");

	}
	// 解锁	
	else
	{
		dwSubCmd = SUB_LS_USER_UNLOCK_MACHINE;
		pzErrStr = _T("解锁失败!");
	}

	g_Logger->TryInfo(_T("LockMachineFrame - Lock Machine Req"));
	DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, dwSubCmd, &tLockMachine, sizeof(tagLS_LockMachine));
	if (!bRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LockMachineFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, pzErrStr
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}

	// 锁定本机按钮,每次点击间隔一秒
	CButtonUI* pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnHallMain_LockMachine));
	if (pBtn)
	{
		pBtn->SetEnabled(false);
		TimerCallBack timepro = CC_CALLBACK_2(LockMachineFrame::TimerProFunc, this);
		g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_HALL_LOCK_MACHINE, TIME_LOCK_MACHINE);
	}
}

bool LockMachineFrame::OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS_USER_LOCK_MACHINE_OK:
		{
			g_Logger->TryInfo(_T("LockMachineFrame - Lock Machine OK"));

			UserData& tSelf = m_pDataManager->GetSelfData();
			tSelf.SetMoorMachine(TRUE);
			this->Close();
			
			if (m_pParentManager)
			{
				DialogFrame::ToastBox(m_pParentManager, _T("锁定成功!"), true);
				CButtonUI* pBtn = static_cast<CButtonUI*>(m_pParentManager->FindControl(R->kBtnHallMain_LockMachine));
				if (pBtn)
				{
					pBtn->SetBkImage(_T("hall\\icon_lock_pc.png"));
				}
			}
		}break;

		case SUB_LS_USER_LOCK_MACHINE_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;
			
			g_Logger->TryInfo(_T("LockMachineFrame - Lock Machine Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { _T("锁定失败!") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogCallBack dlg_cb = CC_CALLBACK_2(LockMachineFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, pzErrorMSG
				, DLG_ID(LockMachineFrame::MSG_BOX_ID_LOCK_PW)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
		}break;
		
		case SUB_LS_USER_UNLOCK_MACHINE_OK:
		{
			g_Logger->TryInfo(_T("LockMachineFrame - UnLock Machine OK"));

			UserData& tSelf = m_pDataManager->GetSelfData();
			tSelf.SetMoorMachine(FALSE);
			this->Close();

			if (m_pParentManager)
			{
				DialogFrame::ToastBox(m_pParentManager, _T("解锁成功!"), true);
				CButtonUI* pBtn = static_cast<CButtonUI*>(m_pParentManager->FindControl(R->kBtnHallMain_LockMachine));
				if (pBtn)
				{
					pBtn->SetBkImage(_T("hall\\icon_unlock_pc.png"));
				}
			}
		}break;

		case SUB_LS_USER_UNLOCK_MACHINE_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG)

			g_Logger->TryInfo(_T("LockMachineFrame - UnLock Machine Failed [code = %u]"), stu_ErrMSG->btErrorCode);
		
			TCHAR pzErrorMSG[MAX_PATH] = { _T("解锁失败") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogCallBack dlg_cb = CC_CALLBACK_2(LockMachineFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, pzErrorMSG
				, DLG_ID(LockMachineFrame::MSG_BOX_ID_LOCK_PW)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
		}break;

		default:
			break;
	}

	return true;
}

// --------------------------------------------------------------------------------------------------------------------------------------
// LockTableFrame
LockTableFrame::LockTableFrame()
	: m_pEdtPassword(nullptr)
	, m_pTable(nullptr)
	, m_wChairIndex(INVALID_CHAIR)
{

}

LockTableFrame::~LockTableFrame()
{
	HWND hParentWnd = NULL;
	if (m_pTable && m_pTable->GetPaintManager())
	{
		CPaintManagerUI* pManager = m_pTable->GetPaintManager();
		g_GameUtil->SetFrozenEnable(pManager, true);
		g_GameUtil->ShowTopWindow(pManager->GetPaintWindow());
	}

	g_Logger->TryInfo(_T("LockTableFrame - Destroy"));
}

BOOL LockTableFrame::CreateFrame(Table* pTable, WORD wChairIndex)
{
	m_pTable = pTable;
	m_wChairIndex = wChairIndex;
	if (FALSE == IsWindow(m_hWnd))
	{
		HWND hParentWnd = NULL;
		
		if (m_pTable && m_pTable->GetPaintManager())
		{
			CPaintManagerUI* pManager = m_pTable->GetPaintManager();
			hParentWnd = pManager->GetPaintWindow();
			g_GameUtil->SetFrozenEnable(pManager, false);
		}

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

UILIB_RESOURCETYPE LockTableFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR LockTableFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString LockTableFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString LockTableFrame::GetSkinFile()
{
	return _T("hall\\lock\\table_lock.xml");
}

void LockTableFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("LockTableFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void LockTableFrame::InitWindow()
{
	g_Logger->TryInfo(_T("LockTableFrame - Open"));

	m_pEdtPassword = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtTableLock_Password));
}

void LockTableFrame::InitData(TNotifyUI& msg)
{
	g_Logger->TryInfo(_T("LockTableFrame - Open Complete"));
}

void LockTableFrame::Check()
{
	CDuiString pzPassword = m_pEdtPassword->GetText();
	if (pzPassword.GetLength() > 0)
	{
		if (m_pTable)
		{
			m_pTable->SitDownReq(m_wChairIndex, pzPassword.GetData());
		}
		this->Close();
	}
	else
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LockTableFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("密码不能为空!")
			, DLG_ID(LockTableMsgBoxID::MSG_BOX_ID_PASSWORD)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb);
	}
}

void LockTableFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	switch (id)
	{
		case LockTableMsgBoxID::MSG_BOX_ID_NONE:
			break;
		case LockTableMsgBoxID::MSG_BOX_ID_DEFAULT:
			break;

		case LockTableMsgBoxID::MSG_BOX_ID_PASSWORD:
		{
			m_pEdtPassword->SetFocus();
		}break;

		default:
			break;
	}
}

void LockTableFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnTableLock_Cancel) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnTableLock_Close) == 0)
		{
			this->Close();
		}
		if (_tcscmp(sName, R->kBtnTableLock_Ensure) == 0)
		{
			this->Check();
		}
	}
}

LRESULT LockTableFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return FALSE;
	}
	return FALSE;
}