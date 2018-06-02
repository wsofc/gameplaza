#include "stdafx.h"
#include "registFrame.h"
#include "loginFrame.h"
#include "util/gameUtil.h"
#include <regex>

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

RegistFrame::RegistFrame()
	: m_text_color_default(0xFFFFFFFF)
	, m_text_color_illegal(0xFFFF0000)
	, m_text_color_correct(0xFF008000)
	, m_cbGender(0)
	, m_llVertcodeSec(0)
	, m_pEdtErr(nullptr)
	, m_pBtnGetCode(nullptr)
	, m_pBtnRegist(nullptr)
{
	::srand((unsigned)time(NULL));
}

RegistFrame::~RegistFrame()
{
	//清除控件数组
	for (int i = 0; i < m_vRegistCtl.size(); ++i)
	{
		delete m_vRegistCtl[i];
	}
	m_vRegistCtl.clear();

	g_Logger->TryInfo(_T("RegistFrame - Destroy"));
}

BOOL RegistFrame::CreateFrame()
{
	if (FALSE == IsWindow(m_hWnd))
	{
		m_hWnd = this->Create(NULL, R->MainWndTitleName, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}

	return TRUE;
}

UILIB_RESOURCETYPE RegistFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR RegistFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString RegistFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString RegistFrame::GetSkinFile()
{
	return _T("regist.xml");
}

LRESULT RegistFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (CMW_CLOSE_SURE == wParam)
	{
		return WindowImplBase::OnClose(uMsg, wParam, lParam, bHandled);
	}

	return FALSE;
}

void RegistFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("RegistFrame - Close"));

	//先关闭定时器
	g_GameUtil->KillTimer(hWnd, TIME_GET_VERTCODE);

	WindowImplBase::OnFinalMessage(hWnd);

	// 析构
	delete this;
};

void RegistFrame::InitWindow()
{
	g_Logger->TryInfo(_T("RegistFrame - Open"));

	// 账号(5-18个有效字符)
	tRegist_Ctl* pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_Account;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Account));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Account));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Account));
	pRegistCtl->pTxtMust = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Account_Must));
	pRegistCtl->bMust = true;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = true;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 5;
	pRegistCtl->dwMaxLen = 18;
	pRegistCtl->pzError = _T("不能包含汉字或特殊符号!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_ACCOUNT;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 设置焦点
	m_PaintManager.SetFocusNeeded(pRegistCtl->pEdt);

	// 昵称(4-30个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_NickName;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_NickName));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_NickName));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_NickName));
	pRegistCtl->pTxtMust = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_NickName_Must));
	pRegistCtl->bMust = true;
	pRegistCtl->bComplete = false;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = true;
	pRegistCtl->bCheckMax = true;
	pRegistCtl->dwMinLen = 4;
	pRegistCtl->dwMaxLen = 30;
	pRegistCtl->pzError = _T("不能包含空格或特殊符号等");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_NICKNAME;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 密码(6-18个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_Password;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Password));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Password));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Password));
	pRegistCtl->pTxtMust = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Password_Must));
	pRegistCtl->bMust = true;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = true;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 6;
	pRegistCtl->dwMaxLen = 18;
	pRegistCtl->pzError = _T("不能包含汉字或特殊符号!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_PASSWORD;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 确认密码(6-18个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_PasswordEx;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_PasswordEx));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_PasswordEx));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_PasswordEx));
	pRegistCtl->pTxtMust = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_PasswordEx_Must));
	pRegistCtl->bMust = true;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = true;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 6;
	pRegistCtl->dwMaxLen = 18;
	pRegistCtl->pzError = _T("两次输入的密码不一致!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_NULL;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 手机号(11个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_Telphone;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Telphone));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Telphone));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Telphone));
	pRegistCtl->pTxtMust = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Telphone_Must));
	pRegistCtl->bMust = true;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = false;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 11;
	pRegistCtl->dwMaxLen = 11;
	pRegistCtl->pzError = _T("请填写正确的手机号码!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_TELPHONE;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 验证码(4个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_Vertcode;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Vertcode));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Vertcode));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Vertcode));
	pRegistCtl->pTxtMust = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Vertcode_Must));
	pRegistCtl->bMust = true;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = false;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 4;
	pRegistCtl->dwMaxLen = 4;
	pRegistCtl->pzError = _T("验证码填写不正确!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_VERTCODE;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 真实姓名(2-6个汉字)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_RealName;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_RealName));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_RealName));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_RealName));
	pRegistCtl->bMust = false;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = true;
	pRegistCtl->bCheckMax = true;
	pRegistCtl->dwMinLen = 2 * 2;
	pRegistCtl->dwMaxLen = 6 * 2;
	pRegistCtl->pzError = _T("不能包含除汉字以外的其它字符!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_REALNAME;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	// 身份证(15-18个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_IDCard;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_IDCard));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_IDCard));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_IDCard));
	pRegistCtl->bMust = false;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = false;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 15;
	pRegistCtl->dwMaxLen = 18;
	pRegistCtl->pzError = _T("请填写正确的身份证号码!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_IDCARD;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	//// 邮箱
	//pRegistCtl = new tRegist_Ctl();
	//pRegistCtl->pzName = R->kEdtRegist_Email;
	//pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Email));
	//pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Email));
	//pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Email));
	//pRegistCtl->bMust = false;
	//pRegistCtl->bComplete = false;
	//pRegistCtl->pzError = _T("请填写正确的邮箱地址!");
	//pRegistCtl->dwType = MatchWords_Type::REG_TYPE_EMAIL;
	//if (pRegistCtl->CheckPointer())
	//{
	//	m_vRegistCtl.push_back(pRegistCtl);
	//}

	// QQ号码(5-18个有效字符)
	pRegistCtl = new tRegist_Ctl();
	pRegistCtl->pzName = R->kEdtRegist_QQ;
	pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_QQ));
	pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_QQ));
	pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_QQ));
	pRegistCtl->bMust = false;
	pRegistCtl->bComplete = false;
	pRegistCtl->bCheckMin = false;
	pRegistCtl->bCheckMax = false;
	pRegistCtl->dwMinLen = 5;
	pRegistCtl->dwMaxLen = 18;
	pRegistCtl->pzError = _T("请填写正确的QQ号码!");
	pRegistCtl->dwType = MatchWords_Type::REG_TYPE_QQ_NUMBER;
	if (pRegistCtl->CheckPointer())
	{
		m_vRegistCtl.push_back(pRegistCtl);
	}

	//// 密保问题
	//pRegistCtl = new tRegist_Ctl();
	//pRegistCtl->pzName = R->kEdtRegist_Question;
	//pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Question));
	//pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Question));
	//pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Question));
	//pRegistCtl->bMust = false;
	//pRegistCtl->bComplete = false;
	//pRegistCtl->pzError = _T("不能包含空格或特殊符号等!");
	//pRegistCtl->dwType = MatchWords_Type::REG_TYPE_QUESTION;
	//if (pRegistCtl->CheckPointer())
	//{
	//	m_vRegistCtl.push_back(pRegistCtl);
	//}

	//// 密保答案
	//pRegistCtl = new tRegist_Ctl();
	//pRegistCtl->pzName = R->kEdtRegist_Answer;
	//pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Answer));
	//pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Answer));
	//pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Answer));
	//pRegistCtl->bMust = false;
	//pRegistCtl->bComplete = false;
	//pRegistCtl->pzError = _T("不能包含空格或特殊符号等!");
	//pRegistCtl->dwType = MatchWords_Type::REG_TYPE_ANSWER;
	//if (pRegistCtl->CheckPointer())
	//{
	//	m_vRegistCtl.push_back(pRegistCtl);
	//}

	//// 推荐人账号
	//pRegistCtl = new tRegist_Ctl();
	//pRegistCtl->pzName = R->kEdtRegist_Spreader;
	//pRegistCtl->pEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtRegist_Spreader));
	//pRegistCtl->pTxt = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtRegist_Spreader));
	//pRegistCtl->pTip = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kCtlRegist_Spreader));
	//pRegistCtl->bMust = false;
	//pRegistCtl->bComplete = false;
	//pRegistCtl->pzError = _T("不能包含汉字或特殊符号!");
	//pRegistCtl->dwType = MatchWords_Type::REG_TYPE_ACCOUNT;
	//if (pRegistCtl->CheckPointer())
	//{
	//	m_vRegistCtl.push_back(pRegistCtl);
	//}

	// 禁用获取验证码按钮
	m_pBtnGetCode = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnRegist_GetVertcode));

	// 默认提示颜色跟配xml置色一致
	m_text_color_default = pRegistCtl->pTxt->GetTextColor();
}

void RegistFrame::InitData(TNotifyUI& msg)
{
	// 设置窗口图标
	this->SetIcon(IDI_GAMEPLAZA_ICON);

	for (const auto& it : m_vRegistCtl)
	{
		it->pEdt->SetText(_T(""));
		it->pTxt->SetUserData(it->pTxt->GetText());
		this->ChangeTipsImage(it->pTip, 0);
	}

	// 设置登录服委托对象
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);
	if (!m_pLoginServer->IsConnect())
	{
		m_iCurOperateStatus = OperateStatus::OS_NONE;
		m_pLoginServer->Connect();
	}

	g_Logger->TryInfo(_T("RegistFrame - Open Complete"));
}

void RegistFrame::ChangeTipsImage(CControlUI* pControl, DWORD dwStatus)
{
	if (!pControl) return;

	LPCTSTR pzStatus = _T("");
	switch (dwStatus)
	{
		// 无
		case 0:
		{
			pzStatus = _T("");
		}break;

		// 失败
		case 1:
		{
			pzStatus = _T("file='common/blocks/tooltip.png' source='0,0,16,16'");
		}break;

		// 成功
		case 2:
		{
			pzStatus = _T("file='common/blocks/tooltip.png' source='16,0,32,32'");
		}break;

		default:
			break;
	}

	pControl->SetBkImage(pzStatus);
}

void RegistFrame::ChangeTipsText(CTextUI* pText, LPCTSTR pzWord, DWORD dwTextColor)
{
	if (!pText) return;
	const DWORD dwMaxTextLen = 50;
	RECT rcTextPadding = { 2, 0, 2, 0 };

	if (_tcslen(pzWord) <= dwMaxTextLen / 2)
	{
		TFontInfo* pFontInfo = m_PaintManager.GetFontInfo(pText->GetFont());
		if (pFontInfo)
		{
			DWORD dwSize = (pText->GetHeight() - pFontInfo->iSize) / 2;
			rcTextPadding.top = dwSize;
			rcTextPadding.bottom = dwSize;
		}
	}

	pText->SetText(pzWord);
	pText->SetAlign(_T("left"));
	pText->SetTextColor(dwTextColor);
	pText->SetTextPadding(rcTextPadding);
}

void RegistFrame::SetTextMustVisible(CTextUI* pTextMust, bool bVisible)
{
	if (!pTextMust) return;
	pTextMust->SetVisible(bVisible);
}

void RegistFrame::SwitchToLoginFrame(tagLS_RegisterAccount* pRegistInfo /* = nullptr */)
{
	this->ShowWindow(false);

	LoginFrame* login_instance = LoginFrame::GetInstance();
	login_instance->CreateFrame();

	if (pRegistInfo)
	{
		login_instance->RefreshLoginData(pRegistInfo->szAccount, pRegistInfo->szPwd);
	}

	this->Close(CMW_CLOSE_SURE);
}

void RegistFrame::GetServerSMS()
{
	tRegist_Ctl* pAccountCtl = FindRegistCtl(R->kEdtRegist_Account);
	tRegist_Ctl* pTelphoneCtl = FindRegistCtl(R->kEdtRegist_Telphone);
	if (!pAccountCtl || !pTelphoneCtl) return;

	//从服务端获取短信验证码
	tag_C_GetSMS stuSMS;
	stuSMS.btType = 1;

	CDuiString& pzAccount = pAccountCtl->pEdt->GetText();
	CDuiString& pzTelphone = pTelphoneCtl->pEdt->GetText();
	::StrToCharArray(stuSMS.szAccount, pzAccount.GetData());
	::StrToCharArray(stuSMS.szPhone, pzTelphone.GetData());

	//发送
	g_Logger->TryInfo(_T("RegistFrame - Get SMS Req"));
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_SMS, SUB_C2LS_GETSMS, &stuSMS, sizeof(tag_C_GetSMS));
	if (FALSE == iRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示")
			, _T("获取验证码失败，请检查您的网络!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void RegistFrame::CheckAccount(BYTE cbType)
{
	if (cbType >= m_vRegistCtl.size()) return;

	int iIndex = 0;
	switch (cbType)
	{
		case 1:		iIndex = 0;		break;
		case 2:		iIndex = 1;		break;
		default:					break;
	}

	tRegist_Ctl* pTagCtl = m_vRegistCtl[iIndex];
	if (!pTagCtl) return;
	CDuiString& pzEdtText = pTagCtl->pEdt->GetText();
	
	if (m_pLoginServer->IsConnect())
	{
		tagLS_CheckAccount tCheckAccount;
		tCheckAccount.btType = cbType;
		::StrToCharArray(tCheckAccount.szAccount, pzEdtText);
		g_Logger->TryInfo(_T("RegistFrame - Check Account Req"));
		DWORD dwRes = m_pLoginServer->SendData(MAIN_LS_LOGON, SUB_C2LS_CHECKACCOUNT, &tCheckAccount, sizeof(tagLS_CheckAccount));
		if (FALSE == dwRes)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示:")
				, _T("获取失败，请检查您的网络")
				, DLG_ID(ServerBase::ServerBaseMsxBoxID::MSG_BOX_ID_NET_CONNECT_FAILED)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	}
	else
	{
		switch (cbType)
		{
			case 1:		m_iCurOperateStatus = OperateStatus::OS_ACCOUNT;	break;	// 核查账号
			case 2:		m_iCurOperateStatus = OperateStatus::OS_NICKNAME;	break;	// 核查昵称
			default:														break;
		}

		m_pLoginServer->Connect();
	}
}

bool RegistFrame::CheckRegist()
{
	bool bRes = true;
	m_pEdtErr = nullptr;
	for (const auto& it : m_vRegistCtl)
	{
		if (it->bMust)
		{
			bRes &= it->bComplete;
			if (!bRes)
			{
				m_pEdtErr = it->pEdt;
				break;
			}
		}
	}

	if (bRes)
	{
		// 性别(0 - 男, 1 - 女)
		m_tRegistAccount.cbGender = m_cbGender;

		// 随机头像
		int iHalfCount = DEFAULT_LOCAL_HEAD_COUNT / 2;
		iHalfCount = iHalfCount <= 0 ? 1 : iHalfCount;
		srand((int)time(0));
		m_tRegistAccount.wFaceID = m_tRegistAccount.cbGender * iHalfCount + (rand() % iHalfCount);

		m_tRegistAccount.dwPlazaVersion = VER_PLAZA_FRAME;

		// 保存推广人账号
		do
		{
			tRegist_Ctl* pRCtlSpreader = this->FindRegistCtl(R->kEdtRegist_Spreader);
			if (pRCtlSpreader)
			{
				CDuiString& pzText = pRCtlSpreader->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szSpreader, pzText.GetData());
			}
		} while (0);

		// 保存登录账号
		do
		{
			tRegist_Ctl* pRCtlAccount = this->FindRegistCtl(R->kEdtRegist_Account);
			if (pRCtlAccount)
			{
				CDuiString& pzText = pRCtlAccount->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szAccount, pzText.GetData());
			}
		} while (0);

		// 保存昵称
		do
		{
			tRegist_Ctl* pRCtlNickName = this->FindRegistCtl(R->kEdtRegist_NickName);
			if (pRCtlNickName)
			{
				CDuiString& pzText = pRCtlNickName->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szNickname, pzText.GetData());
			}
		} while (0);

		// 保存密码
		do
		{
			tRegist_Ctl* pRCtlPassword = this->FindRegistCtl(R->kEdtRegist_Password);
			if (pRCtlPassword)
			{
				CDuiString& pzText = pRCtlPassword->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szPwd, pzText.GetData());
			}
		} while (0);

		// 保存手机号
		do
		{
			tRegist_Ctl* pRCtlTelphone = this->FindRegistCtl(R->kEdtRegist_Telphone);
			if (pRCtlTelphone)
			{
				CDuiString& pzText = pRCtlTelphone->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szPhoneNumber, pzText.GetData());
			}
		} while (0);

		// 短信验证码
		do
		{
			tRegist_Ctl* pRCtlVertCode = this->FindRegistCtl(R->kEdtRegist_Vertcode);
			if (pRCtlVertCode)
			{
				CDuiString& pzText = pRCtlVertCode->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szSMS, pzText.GetData());
			}
		} while (0);

		// 保存姓名
		do
		{
			tRegist_Ctl* pRCtlRealName = this->FindRegistCtl(R->kEdtRegist_RealName);
			if (pRCtlRealName)
			{
				CDuiString& pzText = pRCtlRealName->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szRealName, pzText.GetData());
			}
		} while (0);

		// 保存身份证
		do
		{
			tRegist_Ctl* pRCtlIDCard = this->FindRegistCtl(R->kEdtRegist_IDCard);
			if (pRCtlIDCard)
			{
				CDuiString& pzText = pRCtlIDCard->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szIDCard, pzText.GetData());
			}
		} while (0);

		// 保存邮件
		do
		{
			tRegist_Ctl* pRCtlEmail = this->FindRegistCtl(R->kEdtRegist_Email);
			if (pRCtlEmail)
			{
				CDuiString& pzText = pRCtlEmail->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szEmail, pzText.GetData());
			}
		} while (0);

		// 保存QQ号码
		do
		{
			tRegist_Ctl* pRCtlQQ = this->FindRegistCtl(R->kEdtRegist_QQ);
			if (pRCtlQQ)
			{
				CDuiString& pzText = pRCtlQQ->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szQQNumber, pzText.GetData());
			}
		} while (0);

		// 保存密保问题
		do
		{
			tRegist_Ctl* pRCtlQuestion = this->FindRegistCtl(R->kEdtRegist_Question);
			if (pRCtlQuestion)
			{
				CDuiString& pzText = pRCtlQuestion->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szPwdProtectQuestion, pzText.GetData());
			}
		} while (0);

		// 保存密保答案
		do
		{
			tRegist_Ctl* pRCtlAnswer = this->FindRegistCtl(R->kEdtRegist_Answer);
			if (pRCtlAnswer)
			{
				CDuiString& pzText = pRCtlAnswer->pEdt->GetText();
				::StrToCharArray(m_tRegistAccount.szPwdProtectAnswer, pzText.GetData());
			}
		} while (0);

		this->RegistReq();
	}
	else
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("请填写正确的资料!")
			, DLG_ID(MsgBoxID::MSG_BOX_ID_DEFAULT)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}

	return bRes;
}

tRegist_Ctl* RegistFrame::FindRegistCtl(LPCTSTR pzName)
{
	tRegist_Ctl* pRCtl = nullptr;
	for (const auto& it : m_vRegistCtl)
	{
		if (it->pzName == pzName)
		{
			pRCtl = it;
			break;
		}
	}
	return pRCtl;
}

void RegistFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	CDuiString sType = msg.sType;
	CDuiString sName;
	if (msg.pSender)
	{
		sName = msg.pSender->GetName();
	}

	switch (id)
	{
		case MsgBoxID::MSG_BOX_ID_DEFAULT:
		{
			if (m_pEdtErr)
			{
				m_pEdtErr->SetFocus();
			}
		}break;
		
		case MsgBoxID::MSG_BOX_ID_REGIST_OK:
		{
			if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
			{
				if (_tcscmp(sName, MsgBoxWnd::g_kBtnDialog_Ensure) == 0)
				{
					this->SwitchToLoginFrame(&m_tRegistAccount);
				}
			}
		}break;

		default:
			break;
	}
}

void RegistFrame::SetVertcodeTimer()
{
	m_pBtnGetCode->SetEnabled(false);

	TimerCallBack timepro = CC_CALLBACK_2(RegistFrame::OnVertcodeTimer, this);
	g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_GET_VERTCODE, 1000);
	m_llVertcodeSec = TIME_GET_VERTCODE;

	//开启定时器时即可设置Text显示
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	_stprintf_s(pzBuffer, _T("%ld秒后重新获取"), m_llVertcodeSec);
	m_pBtnGetCode->SetText(pzBuffer);
}

void RegistFrame::OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime)
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

void RegistFrame::ClearCtlContent()
{
	// 清除"密码"
	tRegist_Ctl* pRCtlPassword = this->FindRegistCtl(R->kEdtRegist_Password);
	if (pRCtlPassword)
	{
		pRCtlPassword->pEdt->SetText(_T(""));
		pRCtlPassword->bComplete = false;

		this->SetTextMustVisible(pRCtlPassword->pTxtMust, true);
		this->ChangeTipsText(pRCtlPassword->pTxt, pRCtlPassword->pTxt->GetUserData(), m_text_color_default);
		this->ChangeTipsImage(pRCtlPassword->pTip, 0);
	}

	// 清除"确认密码"
	tRegist_Ctl* pRCtlPasswordEx = this->FindRegistCtl(R->kEdtRegist_PasswordEx);
	if (pRCtlPasswordEx)
	{
		pRCtlPasswordEx->pEdt->SetText(_T(""));
		pRCtlPasswordEx->bComplete = false;

		this->SetTextMustVisible(pRCtlPasswordEx->pTxtMust, true);
		this->ChangeTipsText(pRCtlPasswordEx->pTxt, pRCtlPasswordEx->pTxt->GetUserData(), m_text_color_default);
		this->ChangeTipsImage(pRCtlPasswordEx->pTip, 0);
	}

	// 清除"验证码"
	tRegist_Ctl* pRCtlVertCode = this->FindRegistCtl(R->kEdtRegist_Vertcode);
	if (pRCtlVertCode)
	{
		pRCtlVertCode->pEdt->SetText(_T(""));
		pRCtlVertCode->bComplete = false;

		this->SetTextMustVisible(pRCtlVertCode->pTxtMust, true);
		this->ChangeTipsText(pRCtlVertCode->pTxt, pRCtlVertCode->pTxt->GetUserData(), m_text_color_default);
		this->ChangeTipsImage(pRCtlVertCode->pTip, 0);
	}
}

void RegistFrame::Notify(TNotifyUI& msg)
{
	DWORD dwStatus = 0;
	DWORD dwColor = m_text_color_default;
	LPCTSTR pzText = _T("");

	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(type, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnRegist_Close) == 0)
		{
			this->SwitchToLoginFrame();
		}
		else if (_tcscmp(sName, R->kBtnRegist_Cancel) == 0)
		{
			this->SwitchToLoginFrame();
		}
		else if (_tcscmp(sName, R->kOptRegist_GenderMan) == 0)
		{
			m_cbGender = 0;
		}
		else if (_tcscmp(sName, R->kOptRegist_GenderWoMan) == 0)
		{
			m_cbGender = 1;
		}
		else if (_tcscmp(sName, R->kBtnRegist_GetVertcode) == 0)
		{
			tRegist_Ctl* pAccountCtl = FindRegistCtl(R->kEdtRegist_Account);
			tRegist_Ctl* pTelphoneCtl = FindRegistCtl(R->kEdtRegist_Telphone);
			if (!pAccountCtl || !pTelphoneCtl) return;

			if (!pAccountCtl->bComplete)
			{
				m_pEdtErr = pAccountCtl->pEdt;
				DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("关联账号输入不正确!")
					, DLG_ID(MsgBoxID::MSG_BOX_ID_DEFAULT)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
			else if (!pTelphoneCtl->bComplete)
			{
				m_pEdtErr = pTelphoneCtl->pEdt;
				DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("关联手机号输入不正确!")
					, DLG_ID(MsgBoxID::MSG_BOX_ID_DEFAULT)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
			else
			{
				tRegist_Ctl* pVertCodeCtl = FindRegistCtl(R->kEdtRegist_Vertcode);
				if (!pVertCodeCtl) return;

				pVertCodeCtl->pEdt->SetText(_T(""));
				pVertCodeCtl->pEdt->SetFocus();

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
		}
		else if (_tcscmp(sName, R->kBtnRegist_Regist) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->CheckRegist();
			}
			else
			{
				m_iCurOperateStatus = OperateStatus::OS_REGIST;

				TNotifyUI msg;
				memset(&msg, 0, sizeof(TNotifyUI));
				this->ReConnect(DLG_ID::DLG_ID_NONE, msg);
			}
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_SETFOCUS) == 0)
	{
		for (const auto& it : m_vRegistCtl)
		{
			if (sName == it->pzName)
			{
				// 显示必选"*"文本
				this->SetTextMustVisible(it->pTxtMust, true);

				this->ChangeTipsText(it->pTxt, it->pTxt->GetUserData(), m_text_color_default);
				this->ChangeTipsImage(it->pTip, 0);
			}
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_KILLFOCUS) == 0)
	{
		for (const auto& it : m_vRegistCtl)
		{
			if (sName == it->pzName)
			{
				// 检测"确认密码"有效性
				if (_tcscmp(sName, R->kEdtRegist_PasswordEx) == 0)
				{
					tRegist_Ctl* pPasswordCtl = this->FindRegistCtl(R->kEdtRegist_Password);
					if (pPasswordCtl)
					{
						it->bComplete = it->pEdt->GetText().GetLength() > 0;
						it->bComplete &= it->pEdt->GetText() == pPasswordCtl->pEdt->GetText();
					}

					if (it->bComplete)
					{
						dwStatus = 2;
						dwColor = m_text_color_correct;
						pzText = _T("");
					}
					else
					{
						dwStatus = 1;
						dwColor = m_text_color_illegal;
						pzText = it->pzError;
					}

					this->ChangeTipsText(it->pTxt, pzText, dwColor);
					this->ChangeTipsImage(it->pTip, dwStatus);

					// 隐藏必选"*"文本
					this->SetTextMustVisible(it->pTxtMust, false);
				}
				
				// 检测其他选项有效性
				else
				{
					bool bSensitiveVocabulary = g_GameUtil->CheckSensitiveVocabulary(it->pEdt->GetText());
					bool bRes = true;
					bRes &= !bSensitiveVocabulary;
					bRes &= it->pEdt->GetText().GetLength() >= it->dwMinLen;
					bRes &= it->pEdt->GetText().GetLength() <= it->dwMaxLen;
					bRes &= g_GameUtil->MatchWordByType(it->pEdt->GetText(), it->dwType);
					if (bRes)
					{
						if (_tcscmp(sName, R->kEdtRegist_Account) == 0)
						{
							this->CheckAccount(1);
							continue;
						}
						else if (_tcscmp(sName, R->kEdtRegist_NickName) == 0)
						{
							this->CheckAccount(2);
							continue;
						}

						dwStatus = 2;
						pzText = _T("");
						dwColor = m_text_color_correct;
						it->bComplete = true;
						this->ChangeTipsText(it->pTxt, pzText, dwColor);
						this->ChangeTipsImage(it->pTip, dwStatus);
					}
					else
					{
						// 必填选项
						if (it->bMust)
						{
							if (it->pEdt->GetText().GetLength() <= 0)
							{
								pzText = _T("输入不能为空!");
							}
							else if (g_GameUtil->MatchRegular(it->pEdt->GetText(), _T(".*[\\s]+.*")))
							{
								pzText = _T("不能包含空白字符!");
							}
							else if (bSensitiveVocabulary)
							{
								pzText = _T("不能包含敏感字符!");
							}
							else if (it->bCheckMin && it->pEdt->GetText().GetLength() < it->dwMinLen)
							{
								pzText = _T("输入长度过小!");
							}
							else if (it->bCheckMax && it->pEdt->GetText().GetLength() > it->dwMaxLen)
							{
								pzText = _T("输入长度过大!");
							}
							else
							{
								pzText = it->pzError;
							}

							dwStatus = 1;
							dwColor = m_text_color_illegal;
							it->bComplete = false;
							this->ChangeTipsText(it->pTxt, pzText, dwColor);
							this->ChangeTipsImage(it->pTip, dwStatus);
						}

						// 非必填选项
						else
						{
							if (it->pEdt->GetText().GetLength() <= 0)
							{
								dwStatus = 2;
								pzText = _T("");
								dwColor = m_text_color_correct;
								it->bComplete = true;
								this->ChangeTipsText(it->pTxt, pzText, dwColor);
								this->ChangeTipsImage(it->pTip, dwStatus);
							}
							else
							{
								if (g_GameUtil->MatchRegular(it->pEdt->GetText(), _T(".*[\\s]+.*")))
								{
									pzText = _T("不能包含空白字符!");
								}
								else if (it->bCheckMin && it->pEdt->GetText().GetLength() < it->dwMinLen)
								{
									pzText = _T("输入长度过小!");
								}
								else if (it->bCheckMax && it->pEdt->GetText().GetLength() > it->dwMaxLen)
								{
									pzText = _T("输入长度过大!");
								}
								else
								{
									pzText = it->pzError;
								}

								dwStatus = 1;
								it->bComplete = false;
								dwColor = m_text_color_illegal;
								this->ChangeTipsText(it->pTxt, pzText, dwColor);
								this->ChangeTipsImage(it->pTip, dwStatus);
							}
						}
					}

					// 隐藏必选"*"文本
					this->SetTextMustVisible(it->pTxtMust, false);
				}
			}
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_TEXTCHANGED) == 0)
	{
	}

	//__super::Notify(msg);
}

LRESULT RegistFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (m_PaintManager.GetRoot()->IsKeyboardEnabled())
	{
		if (wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			this->SwitchToLoginFrame();
			return TRUE;
		}
	}

	return FALSE;
}

void RegistFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

bool RegistFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case OperateStatus::OS_NONE:									break;
		case OperateStatus::OS_ACCOUNT:		this->CheckAccount(1);		break;
		case OperateStatus::OS_NICKNAME:	this->CheckAccount(2);		break;
		case OperateStatus::OS_GETVCODE:	this->GetServerSMS();		break;
		case OperateStatus::OS_REGIST:		this->CheckRegist();		break;
		case OperateStatus::OS_EXTRA:									break;
		default:														break;
	}
	m_iCurOperateStatus = OperateStatus::OS_NONE;

	return true;
}

void RegistFrame::RegistReq()
{
	tagLS_RegisterAccount tagRegistInfo;
	memcpy(&tagRegistInfo, &m_tRegistAccount, sizeof(tagLS_RegisterAccount));
	CMD5Encrypt::EncryptData(m_tRegistAccount.szPwd, tagRegistInfo.szPwd);

	g_Logger->TryInfo(_T("RegistFrame - Regist Account Req"));
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_LOGON, SUB_LS_REGISTER_ACCOUNT, &tagRegistInfo, sizeof(tagLS_RegisterAccount));
	if (FALSE == iRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示:")
			, _T("注册失败，请检查您的网络")
			, DLG_ID(ServerBase::ServerBaseMsxBoxID::MSG_BOX_ID_NET_CONNECT_FAILED)
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

bool RegistFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	//CString strLog;

	//strLog.Format("%s%s,%d,wSocketID:%d,cbShutReason:%d,",
	//	PREFIX_PROJECT,
	//	__FUNCTION__, __LINE__,
	//	wSocketID,
	//	cbShutReason);
	//g_FileLog.Write(strLog);

	//if (!m_bLogonSuccess)
	//{
	//	if (cbShutReason != SHUT_REASON_NORMAL)
	//	{
	//		g_GlobalAttemper.DestroyStatusWnd(this);
	//		this->ShowMessageBox(TEXT("登录服务器连接失败,请稍后再试或留意网站公告!"), MB_ICONINFORMATION);
	//		this->LogonDlgShow();
	//	}
	//}

	return true;
}

bool RegistFrame::OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_SENDSMSSUCCESS:
		{
			g_Logger->TryInfo(_T("RegistFrame - Get SMS OK"));

			tRegist_Ctl* pVertCodeCtl = FindRegistCtl(R->kEdtRegist_Vertcode);
			if (!pVertCodeCtl) return true;

			m_pEdtErr = pVertCodeCtl->pEdt;
			DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::MsgBoxProcFocus, this);
			DialogFrame::Toast(&m_PaintManager, _T("验证码发送成功!"), (DLG_ID)MsgBoxID::MSG_BOX_ID_DEFAULT, dlg_cb);

			// 验证码按钮开始间隔时间计时
			this->SetVertcodeTimer();
		}break;

		case SUB_LS2C_SMS_ERROR:
		{
			tag_S_ErrorMsg stu_ErrMSG;
			memset(&stu_ErrMSG, 0, sizeof(tag_S_ErrorMsg));
			memcpy(&stu_ErrMSG, pDataBuffer, wDataSize);

			g_Logger->TryInfo(_T("RegistFrame - Get SMS Failed [code = %u]"), stu_ErrMSG.btErrorCode);
			
			DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, stu_ErrMSG.szMsg
				, DLG_ID(MsgBoxID::MSG_BOX_ID_VCODE)
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

bool RegistFrame::OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 申请核查账号是否可用成功
		case SUB_C2LS_CHECKACCOUNT_SSUCCESS:
		{
			tagLS_CheckAccount* pCheckAccount = static_cast<tagLS_CheckAccount*>(pDataBuffer);
			if (pCheckAccount)
			{
				g_Logger->TryInfo(_T("RegistFrame - Check Account OK [type = %u]"), pCheckAccount->btType);

				switch (pCheckAccount->btType)
				{
					// 账号审查成功
					case 1:
					{
						tRegist_Ctl* pAccountCtl = m_vRegistCtl[0];
						if (pAccountCtl)
						{
							pAccountCtl->bComplete = true;
							this->ChangeTipsImage(pAccountCtl->pTip, 2);
							this->ChangeTipsText(pAccountCtl->pTxt, _T(""), m_text_color_correct);

							// 隐藏必选"*"文本
							this->SetTextMustVisible(pAccountCtl->pTxtMust, false);
						}
					}break;

					// 昵称审查成功
					case 2:
					{
						tRegist_Ctl* pNickNameCtl = m_vRegistCtl[1];
						if (pNickNameCtl)
						{
							pNickNameCtl->bComplete = true;
							this->ChangeTipsImage(pNickNameCtl->pTip, 2);
							this->ChangeTipsText(pNickNameCtl->pTxt, _T(""), m_text_color_correct);

							// 隐藏必选"*"文本
							this->SetTextMustVisible(pNickNameCtl->pTxtMust, false);
						}
					}break;

					default:
						break;
				}
			}
		}break;

		// 申请核查账号是否可用失败
		case SUB_C2LS_CHECKACCOUNT_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (stu_ErrMSG)
			{
				g_Logger->TryInfo(_T("RegistFrame - Check Account Failed [type = %u]"), stu_ErrMSG->btErrorCode);

				switch (stu_ErrMSG->btErrorCode)
				{
					// 账号审查失败
					case 1:
					{
						tRegist_Ctl* pAccountCtl = m_vRegistCtl[0];
						if (pAccountCtl)
						{
							pAccountCtl->bComplete = false;
							this->ChangeTipsImage(pAccountCtl->pTip, 1);
							this->ChangeTipsText(pAccountCtl->pTxt, stu_ErrMSG->szMsg, m_text_color_illegal);

							// 隐藏必选"*"文本
							this->SetTextMustVisible(pAccountCtl->pTxtMust, false);
						}
					}break;

					// 昵称审查失败
					case 2:
					{
						tRegist_Ctl* pNickNameCtl = m_vRegistCtl[1];
						if (pNickNameCtl)
						{
							pNickNameCtl->bComplete = false;
							this->ChangeTipsImage(pNickNameCtl->pTip, 1);
							this->ChangeTipsText(pNickNameCtl->pTxt, stu_ErrMSG->szMsg, m_text_color_illegal);

							// 隐藏必选"*"文本
							this->SetTextMustVisible(pNickNameCtl->pTxtMust, false);
						}
					}break;

					default:
						break;
				}
			}
		}break;

		case SUB_LS_REGISTER_SUCCESS:
		{
			g_Logger->TryInfo(_T("RegistFrame - Regist Account OK"));

			DialogCallBack dlg_cb = CC_CALLBACK_2(RegistFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示:")
				, _T("恭喜您,注册成功!")
				, DLG_ID(MsgBoxID::MSG_BOX_ID_REGIST_OK)
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}break;

		case SUB_LS_REGISTER_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;

			g_Logger->TryInfo(_T("RegistFrame - Regist Account Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			CDuiString pzErrorMSG = stu_ErrMSG->szMsg;
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示:"), pzErrorMSG.GetData());

			// 注册失败:清除相关信息
			this->ClearCtlContent();
		}break;

		default:
			break;
	}

	return true;
}