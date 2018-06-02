#include "stdafx.h"
#include "bank.h"
#include "mall.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

BankFrame* BankFrame::g_instance = NULL;

BankFrame::BankFrame()
	: m_pParentManager(nullptr)
	, m_dwCurPageIndex(0)
	, m_pSelfData(nullptr)
	, m_pTabLaout(nullptr)
	, m_pSwitch1_EdtYXB(nullptr)
	, m_pSwitch1_EdtSaveNum(nullptr)
	, m_pSwitch1_EdtDeposit(nullptr)
	, m_pSwitch1_EdtTakeNum(nullptr)
	, m_pSwitch1_EdtTakePassword(nullptr)
	, m_pSwitch2_EdtSrcPw(nullptr)
	, m_pSwitch2_EdtNewPw(nullptr)
	, m_pSwitch2_EdtCfmPw(nullptr)
	, m_lMaxCharmOnce(0)
	, m_pSwitch3_EdtCharms(nullptr)
	, m_pSwitch3_EdtDeposit(nullptr)
	, m_pSwitch3_EdtExCount(nullptr)
	, m_pSwitch3_Opt(nullptr)
	, m_pSwitch3_EdtTakePassword(nullptr)
	, m_pSwitch4_WebCharmsDetail(nullptr)
	, m_pSwitch5_WebScoreRebate(nullptr)
	, m_iCurOperateStatus(BanckOperateStatus::BANCK_OS_NONE)
{
	memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
}

BankFrame::~BankFrame()
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

	g_Logger->TryInfo(_T("BankFrame - Destroy"));
}

BankFrame* BankFrame::GetInstance()
{
	if (g_instance == NULL)
		g_instance = new BankFrame();

	return g_instance;
}

bool BankFrame::IsInstanceExist()
{
	return g_instance != nullptr;
}

BOOL BankFrame::CreateFrame(CPaintManagerUI* pParentManager)
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

UILIB_RESOURCETYPE BankFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR BankFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString BankFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString BankFrame::GetSkinFile()
{
	return _T("hall/bank/bank.xml");
}

LRESULT BankFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_CHAR:
		case WM_IME_CHAR:
		{
			if ((m_pSwitch1_EdtSaveNum && m_pSwitch1_EdtSaveNum->IsFocused())
				|| (m_pSwitch1_EdtTakeNum && m_pSwitch1_EdtTakeNum->IsFocused())
				|| (m_pSwitch3_EdtExCount && m_pSwitch3_EdtExCount->IsFocused()))
			{
				CRichEditUI* pFocusEdt = static_cast<CRichEditUI*>(m_PaintManager.GetFocus());
				if (pFocusEdt)
				{
					// 只接收数字(且第一位不能是0)
					if (wParam < 48 || wParam > 57 || (pFocusEdt->GetText().GetLength() == 0 && wParam == 48))
					{
						bHandled = TRUE;
						return TRUE;
					}
				}
			}
		}break;
		
		case WM_ACTIVATEAPP:
		{
			BOOL bActivated = (BOOL)wParam;
			DWORD dwThread = (DWORD)lParam;
			if (bActivated)
			{
				::SetFocus(m_hWnd);
			}
		}break;

		default:
			break;
	}

	return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

LRESULT BankFrame::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWndFocus = ::GetFocus();

	if (hWndFocus == m_pParentManager->GetPaintWindow() && m_PaintManager.GetRoot()->IsMouseEnabled())
	{
		g_GameUtil->RunWndUnFocusAction(m_hWnd, 5);
	}

	return FALSE;
}

void BankFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("BankFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(BankFrame::g_instance);
}

CControlUI* BankFrame::CreateControl(LPCTSTR pstrClassName)
{
	CControlUI* pControl = nullptr;
	CDialogBuilder builder;

	if (_tcsicmp(pstrClassName, _T("bank_switch_1")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/bank/bank_switch_1.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("bank_switch_2")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/bank/bank_switch_2.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("bank_switch_3")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/bank/bank_switch_3.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("bank_switch_4")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/bank/bank_switch_4.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, _T("bank_switch_5")) == 0)
	{
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/bank/bank_switch_5.xml"), (UINT)0, this, &m_PaintManager));
	}
	else if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	{
		pControl = new CWkeWebkitUI();
	}
	return pControl;
}

void BankFrame::InitData(TNotifyUI& msg)
{
	g_Logger->TryInfo(_T("BankFrame - Open"));

	if (!m_pSelfData)
	{
		DataManager* pDataManager = DataManager::GetInstance();
		m_pSelfData = &pDataManager->GetSelfData();
	}
	
	// 初始化银行面板
	this->InitBankPanel();

	// 设置登录服委托对象
	m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_LOGIN;
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);
	if (!m_pLoginServer->IsConnect())
	{
		m_pLoginServer->Connect();
	}
	else
	{
		this->CheckLogin();
	}

	g_Logger->TryInfo(_T("BankFrame - Open Complete"));
}

void BankFrame::InitBankPanel()
{
	m_pTabLaout					= static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsBank_Switch));

	// Switch1
	m_pSwitch1_EdtYXB			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch1_YXB));
	m_pSwitch1_EdtSaveNum		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch1_SaveNumber));
	m_pSwitch1_EdtDeposit		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch1_Deposit));
	m_pSwitch1_EdtTakeNum		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch1_TakeNumber ));
	m_pSwitch1_EdtTakePassword	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch1_TakePassword));
	m_pSwitch1_OptMarkPw		= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptBankSwitch1_MarkPW));
	m_PaintManager.SetFocusNeeded(m_pSwitch1_EdtSaveNum);

	// Switch2
	m_pSwitch2_EdtSrcPw			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch2_SrcPW));
	m_pSwitch2_CtlSrcPw			= static_cast<CControlUI*>(m_PaintManager.FindControl(R->kCtlBankSwitch2_SrcPW));
	this->ChangeTipsImage(m_pSwitch2_CtlSrcPw, 0);
	m_vPwComplete.push_back(false);

	m_pSwitch2_EdtNewPw			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch2_NewPW));
	m_pSwitch2_CtlNewPw			= static_cast<CControlUI*>(m_PaintManager.FindControl(R->kCtlBankSwitch2_NewPW));
	this->ChangeTipsImage(m_pSwitch2_CtlNewPw, 0);
	m_vPwComplete.push_back(false);

	m_pSwitch2_EdtCfmPw			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch2_CfmPW));
	m_pSwitch2_CtlCfmPw			= static_cast<CControlUI*>(m_PaintManager.FindControl(R->kCtlBankSwitch2_CfmPW));
	this->ChangeTipsImage(m_pSwitch2_CtlCfmPw, 0);
	m_vPwComplete.push_back(false);
	
	// Switch3
	m_lMaxCharmOnce				= 100;
	m_pSwitch3_EdtCharms		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch3_Charms));
	m_pSwitch3_EdtDeposit		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch3_Deposit));
	m_pSwitch3_EdtExCount		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch3_ExCount));
	m_pSwitch3_Opt				= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptBankSwitch3_Opt));
	m_pSwitch3_OptMarkPw		= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptBankSwitch3_MarkPW));
	m_pSwitch3_EdtTakePassword	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtBankSwitch3_TakePassword));

	// Switch4
	m_pSwitch4_WebCharmsDetail	= static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeBankSwitch4_WebChars));

	// Switch5
	m_pSwitch5_WebScoreRebate	= static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeBankSwitch5_WebRebate));

	// Switch5,只有紫钻或者管理员才能看到
	do
	{
		DataManager* pDataManager = DataManager::GetInstance();
		UserData& stuSelf = pDataManager->GetSelfData();
		bool bVisible = stuSelf.GetMemberOrder() >= 3 || stuSelf.GetMasterOrder() > 0 || stuSelf.IsUnrealUser();
		COptionUI* pBank_Opt_5 = static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptBank_Op_5));
		if (pBank_Opt_5)
		{
			pBank_Opt_5->SetVisible(bVisible);
		}

		CControlUI* pBank_Split_5 = static_cast<CControlUI*>(m_PaintManager.FindControl(R->kCtlBank_Split_5));
		if (pBank_Split_5)
		{
			pBank_Split_5->SetVisible(bVisible);
		}
	} while (0);

	// 更新面板数据
	this->UpdateBankPanel();

	// 读取保存的取款密码
	this->ReadBankSetting();
}

void BankFrame::UpdateBankPanel()
{
	if (!m_pTabLaout) return;
	int iCurSel = m_pTabLaout->GetCurSel();
	switch (iCurSel)
	{
		case 0:
		{
			// 更新游戏币
			do
			{
				// 更新玩家UI数据
				m_pSelfData->UpdateUI(m_pParentManager);

				LONGLONG lGameGold = m_pSelfData->GetGameGold();
				_stprintf_s(m_pzBuffer, _T("%lld"), lGameGold);
				m_pSwitch1_EdtYXB->SetText(m_pzBuffer);

			} while (0);

			// 更新银行存款
			do
			{
				LONGLONG lBankGold = m_pSelfData->GetBankScore();
				_stprintf_s(m_pzBuffer, _T("%lld"), lBankGold);
				m_pSwitch1_EdtDeposit->SetText(m_pzBuffer);
			} while (0);
		}break;

		case 1:
		{

		}break;

		case 2:
		{
			LONG lCharm = m_pSelfData->GetCharm();
			_stprintf_s(m_pzBuffer, _T("%ld"), lCharm);
			m_pSwitch3_EdtCharms->SetText(m_pzBuffer);

			LONGLONG lBankScore = m_pSelfData->GetBankScore();
			_stprintf_s(m_pzBuffer, _T("%lld"), lBankScore);
			m_pSwitch3_EdtDeposit->SetText(m_pzBuffer);
		}break;
		default:
			break;
	}
}

void BankFrame::OnWkeTitleChange(const CDuiString& strTitle)
{
}

void BankFrame::OnWkeUrlChange(const CDuiString& strUrl)
{
	if (m_pSwitch4_WebCharmsDetail->IsClickURL())
	{
		m_pSwitch4_WebCharmsDetail->ResetClickURL();
		wkeWebView pWebView = m_pSwitch4_WebCharmsDetail->GetWebView();
		pWebView->stopLoading();
		pWebView->loadURL(m_pSwitch4_WebCharmsDetail->GetUrl().GetData());
		g_GameUtil->OpenWebByURL(strUrl.GetData());
	}
	else if (m_pSwitch5_WebScoreRebate->IsClickURL())
	{
		m_pSwitch5_WebScoreRebate->ResetClickURL();
		wkeWebView pWebView = m_pSwitch5_WebScoreRebate->GetWebView();
		pWebView->stopLoading();
		pWebView->loadURL(m_pSwitch5_WebScoreRebate->GetUrl().GetData());
		g_GameUtil->OpenWebByURL(strUrl.GetData());
	}
}

void BankFrame::AddTempGold(CRichEditUI* pEdt, LONGLONG llNums, bool bGold, bool bSupp /* = false */)
{
	LONGLONG llTotals = 0;
	if (bGold)
	{
		llTotals = m_pSelfData->GetGameGold();
	}
	else
	{
		llTotals = m_pSelfData->GetBankScore();
	}

	if (!pEdt) return;
	llTotals = max(llTotals, 0);

	LONGLONG llTmpNum = llNums;
	if (bSupp)
	{
		llTmpNum += _ttoll(pEdt->GetText().GetData());
	}

	llTmpNum = min(llTmpNum, llTotals);
	memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	_stprintf_s(m_pzBuffer, _T("%lld"), llTmpNum);
	pEdt->SetText(m_pzBuffer);
}

void BankFrame::TickingMaxCharms(bool bTicked)
{
	if (!m_pSwitch3_EdtExCount) return;
	if (bTicked)
	{
		_stprintf_s(m_pzBuffer, _T("%ld"), m_lMaxCharmOnce);
		m_pSwitch3_EdtExCount->SetText(m_pzBuffer);
		m_pSwitch3_EdtExCount->SetReadOnly(true);
		m_pSwitch3_EdtExCount->SetReadOnlyShowCaret(true);
		m_pSwitch3_EdtExCount->SetTabEnable(false);
		m_pSwitch3_EdtExCount->SetTextColor(0xFF969696);
	}
	else
	{
		m_pSwitch3_EdtExCount->SetReadOnly(false);
		m_pSwitch3_EdtExCount->SetTabEnable(true);
		m_pSwitch3_EdtExCount->SetTextColor(0xFFFF0000);
		m_PaintManager.SetFocusNeeded(m_pSwitch3_EdtExCount);
	}
}

void BankFrame::SaveBankSetting(int iType, bool bSaveToLocalFile /* = false */)
{
	g_Logger->TryInfo(_T("BankFrame - Save Bank Setting [type = %d, bFile = %d]"), iType, bSaveToLocalFile);

	// iType
	// 0 - 银行取款时的密码
	// 1 - 兑换魅力时的密码

	if (!m_pSelfData) return;
	
	bool bSaveBankTakePw = m_pSwitch1_OptMarkPw->IsSelected();
	bool bSaveExCharmsPw = m_pSwitch3_OptMarkPw->IsSelected();
	CDuiString& pzBankTakePw = m_pSwitch1_EdtTakePassword->GetText();
	CDuiString& pzExCharmsPw = m_pSwitch3_EdtTakePassword->GetText();

	if (bSaveToLocalFile)
	{
		CDuiString pzPath;
		pzPath.Empty();
		pzPath.Format(_T("%s..\\data\\user\\%s\\%s")
			, CPaintManagerUI::GetResourcePath().GetData()
			, m_pSelfData->GetAccount(), _T("BANKSETTING"));

		Json::Value vRoot;
		JsonFile* pFile = new JsonFile(pzPath);
		switch (iType)
		{
			case 0:
			{
				if (m_pSwitch1_OptMarkPw && m_pSwitch1_EdtTakePassword)
				{
					if (pzBankTakePw.GetLength() > 0)
					{
						vRoot["bSaveBankTakePw"] = bSaveBankTakePw;
						vRoot["pzBankTakePw"] = pzBankTakePw.GetData();
					}
				}

				pFile->SaveFile(vRoot);
			}break;

			case 1:
			{
				if (m_pSwitch3_OptMarkPw && m_pSwitch3_EdtTakePassword)
				{
					if(pzExCharmsPw.GetLength() > 0)
					{
						vRoot["bSaveExCharmsPw"] = bSaveExCharmsPw;
						vRoot["pzExCharmsPw"] = pzExCharmsPw.GetData();
					}
				}

				pFile->SaveFile(vRoot);
			}break;

			default:
				break;
		}
		SAFE_DELETE(pFile);
	}
	else
	{
		ServiceData* pServerData = ServiceData::GetInstance();
		GameCacheData& tGameCacheData = pServerData->GetGameCacheData();

		switch (iType)
		{
			case 0:
			{
				if (m_pSwitch1_OptMarkPw && m_pSwitch1_EdtTakePassword)
				{
					tGameCacheData.bSaveBankTakePw = m_pSwitch1_OptMarkPw->IsSelected();
					if (tGameCacheData.bSaveBankTakePw)
					{
						tGameCacheData.pzBankTakePw = m_pSwitch1_EdtTakePassword->GetText();
					}
				}
			}break;

			case 1:
			{
				if (m_pSwitch3_OptMarkPw && m_pSwitch3_EdtTakePassword)
				{
					tGameCacheData.bSaveExCharmsPw = m_pSwitch3_OptMarkPw->IsSelected();
					if (tGameCacheData.bSaveExCharmsPw)
					{
						tGameCacheData.pzExCharmsPw = m_pSwitch3_EdtTakePassword->GetText();
					}
				}
			};

			default:
				break;
		}
	}

	g_Logger->TryInfo(_T("BankFrame - Save Bank Setting Complete"));
}

void BankFrame::ReadBankSetting(bool bReadFromLocalFile /* = false */)
{
	g_Logger->TryInfo(_T("BankFrame - Read Bank Setting [bFile = %u]"), bReadFromLocalFile);

	if (!m_pSelfData) return;

	bool bSaveBankTakePw = false;
	bool bSaveExCharmsPw = false;
	CDuiString pzBankTakePw;
	CDuiString pzExCharmsPw;

	if (bReadFromLocalFile)
	{
		CDuiString pzPath;
		pzPath.Empty();
		pzPath.Format(_T("user\\%s\\%s"), m_pSelfData->GetAccount(), _T("BANKSETTING"));

		Json::Value vRoot;
		JsonFile* pFile = new JsonFile(pzPath);
		bool bResult = pFile->initRootNode(vRoot);
		SAFE_DELETE(pFile);
		if (bResult)
		{
			if (vRoot["bSaveBankTakePw"].isBool())
			{
				bSaveBankTakePw = vRoot["bSaveBankTakePw"].asBool();
				if (bSaveBankTakePw && vRoot["pzBankTakePw"].isString())
				{
					pzBankTakePw = vRoot["pzBankTakePw"].asString().c_str();
				}
			}

			if (vRoot["bSaveExCharmsPw"].isBool())
			{
				bSaveExCharmsPw = vRoot["bSaveExCharmsPw"].asBool();
				if (bSaveExCharmsPw && vRoot["pzExCharmsPw"].isString())
				{
					pzExCharmsPw = vRoot["pzExCharmsPw"].asString().c_str();
				}
			}
		}
	}
	else
	{
		ServiceData* pServerData = ServiceData::GetInstance();
		GameCacheData& tGameCacheData = pServerData->GetGameCacheData();
		
		bSaveBankTakePw = tGameCacheData.bSaveBankTakePw;
		pzBankTakePw = tGameCacheData.pzBankTakePw;

		bSaveExCharmsPw = tGameCacheData.bSaveExCharmsPw;
		pzExCharmsPw = tGameCacheData.pzExCharmsPw;
	}

	m_pSwitch1_OptMarkPw->Selected(bSaveBankTakePw);
	if (bSaveBankTakePw)
	{
		m_pSwitch1_EdtTakePassword->SetText(pzBankTakePw);
		m_pSwitch1_EdtTakePassword->SetPassword(true);
	}

	m_pSwitch3_OptMarkPw->Selected(bSaveExCharmsPw);
	if (bSaveExCharmsPw)
	{
		m_pSwitch3_EdtTakePassword->SetText(pzExCharmsPw);
		m_pSwitch3_EdtTakePassword->SetPassword(true);
	}

	g_Logger->TryInfo(_T("BankFrame - Read Bank Setting Complete"));
}

void BankFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	
	BankMsgBoxID msg_box_id = BankMsgBoxID(id);
	switch (msg_box_id)
	{
		case BankMsgBoxID::MSG_BOX_ID_SAVE:				m_pSwitch1_EdtSaveNum->SetFocus();			break;
		case BankMsgBoxID::MSG_BOX_ID_TAKE:				m_pSwitch1_EdtTakeNum->SetFocus();			break;
		case BankMsgBoxID::MSG_BOX_ID_TAKE_PW:			m_pSwitch1_EdtTakePassword->SetFocus();		break;
		case BankMsgBoxID::MSG_BOX_ID_SRC_PW:			m_pSwitch2_EdtSrcPw->SetFocus();			break;
		case BankMsgBoxID::MSG_BOX_ID_NEW_PW:			m_pSwitch2_EdtNewPw->SetFocus();			break;
		case BankMsgBoxID::MSG_BOX_ID_CFM_PW:			m_pSwitch2_EdtCfmPw->SetFocus();			break;
		case BankMsgBoxID::MSG_BOX_ID_CHARM_COUNT:		m_pSwitch3_EdtExCount->SetFocus();			break;
		case BankMsgBoxID::MSG_BOX_ID_CHARM_PW:			m_pSwitch3_EdtTakePassword->SetFocus();		break;
		default:																					break;
	}
}

bool BankFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case BanckOperateStatus::BANCK_OS_NONE:														break;
		case BanckOperateStatus::BANCK_OS_LOGIN:		this->CheckLogin();							break;
		case BanckOperateStatus::BANCK_OS_SAVE:			this->CheckSave();							break;
		case BanckOperateStatus::BANCK_OS_TAKE:			this->CheckTake();							break;
		case BanckOperateStatus::BANCK_OS_MODIFY_PW:	this->CheckModifyPW();						break;
		case BanckOperateStatus::BANCK_OS_EXCHANGE:		this->CheckExchange();						break;
		case BanckOperateStatus::BANCK_OS_REFRESH:		this->CheckRefresh();						break;
		case BanckOperateStatus::BANCK_OS_EXTRA:													break;
		default:																					break;
	}
	m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_NONE;

	return true;
}

void BankFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

bool BankFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}

bool BankFrame::OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 温馨提示成功
		case SUB_GF_BANK_INFO:
		{
			CMD_GF_BankInfo* pStuBankInfo = static_cast<CMD_GF_BankInfo*>(pDataBuffer);
			if (pStuBankInfo)
			{
				g_Logger->TryInfo(_T("BankFrame - Bank Req OK [Action = %u]"), pStuBankInfo->cbGameAction);

				m_pSelfData->SetGameGold(pStuBankInfo->lGameScore);
				m_pSelfData->SetBankScore(pStuBankInfo->lBankScore);
				m_pSelfData->SetCharm(pStuBankInfo->lCharm);
				this->UpdateBankPanel();

				switch (pStuBankInfo->cbGameAction)
				{
					// 银行登录
					case 0:
						break;
					// 银行存款
					case 1:
					{
						m_pSwitch1_EdtSaveNum->SetFocus();
						DialogFrame::Toast(&m_PaintManager, _T("存款成功"));
					}break;

					// 银行取款
					case 2:
					{
						m_pSwitch1_EdtTakeNum->SetFocus();
						DialogFrame::Toast(&m_PaintManager, _T("取款成功"));

						// 成功操作，保存下相关设置
						this->SaveBankSetting(0);

						// 如果连接了游戏服,通知游戏服更新银行金币等
						do
						{
							if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_ROOM)
							{
								RoomManager* pRoomManager = RoomManager::GetInstance();
								pRoomManager->NotifyGameServerUpdateBank();
							}
						} while (0);
					}break;

					// 银行转账
					case 3:
					{
					}break;

					// 银行修改密码
					case 4:
					{
						DialogFrame::ToastBox(&m_PaintManager, _T("修改成功"), true);
						
						// 清除本地保存的银行密码等
						if (m_pSwitch1_OptMarkPw && m_pSwitch1_EdtTakePassword && m_pSwitch3_OptMarkPw && m_pSwitch3_EdtTakePassword)
						{
							m_pSwitch1_OptMarkPw->Selected(false);
							m_pSwitch1_EdtTakePassword->SetText(_T(""));

							m_pSwitch3_OptMarkPw->Selected(false);
							m_pSwitch3_EdtTakePassword->SetText(_T(""));

							ServiceData* pServerData = ServiceData::GetInstance();
							GameCacheData& tGameCacheData = pServerData->GetGameCacheData();
							memset(&tGameCacheData, 0, sizeof(tGameCacheData));
						}
					}break;

					default:
					{
						DialogFrame::Toast(&m_PaintManager, _T("操作成功"));
					}break;
				}
			}
		}break;

		// 温馨提示失败
		case SUB_GF_BANK_RESULT:
		{
			BankMsgBoxID msg_box_id = BankMsgBoxID::MSG_BOX_ID_NONE;
			CMD_GF_BankError* pStuBankError = static_cast<CMD_GF_BankError*>(pDataBuffer);
			if (pStuBankError)
			{
				g_Logger->TryInfo(_T("BankFrame - Bank Req Failed [Action = %u]"), pStuBankError->cbGameAction);
				switch (pStuBankError->cbGameAction)
				{
					// 银行登录
					case 0:														break;

					// 银行存款
					case 1:	msg_box_id = BankMsgBoxID::MSG_BOX_ID_SAVE;			break;

					// 银行取款
					case 2:	msg_box_id = BankMsgBoxID::MSG_BOX_ID_TAKE_PW;		break;

					// 银行转账
					case 3:														break;

					// 银行修改密码
					case 4:	msg_box_id = BankMsgBoxID::MSG_BOX_ID_SRC_PW;		break;

					default:													break;
				}
				
				DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, pStuBankError->szErrorDescribe
					, (DLG_ID)msg_box_id
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
		}break;
		
		default:
			break;
	}

	return true;
}

bool BankFrame::OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 兑换魅力成功
		case SUB_GF_EXCHANGE_CHARM_OK:
		{
			g_Logger->TryInfo(_T("BankFrame - Exchange Charms OK"));

			// 成功操作，保存下相关设置
			this->SaveBankSetting(1);

			m_pSwitch3_EdtExCount->SetFocus();
			DialogFrame::Toast(&m_PaintManager, _T("兑换成功"));

			CMD_GF_ExchangeCharmOK* pCharmOk = static_cast<CMD_GF_ExchangeCharmOK*>(pDataBuffer);
			if (pCharmOk)
			{
				m_pSelfData->SetBankScore(pCharmOk->lBankScore);
				m_pSelfData->SetGameGold(pCharmOk->lGameScore);
				m_pSelfData->SetCharm(pCharmOk->lCharm);
			}

			// 更新银行面板
			this->UpdateBankPanel();

			// 如果连接了游戏服,通知游戏服更新银行金币等
			do
			{
				if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_ROOM)
				{
					RoomManager* pRoomManager = RoomManager::GetInstance();
					pRoomManager->NotifyGameServerUpdateBank();
				}
			} while (0);
		}break;

		// 兑换魅力失败
		case SUB_GF_EXCHANGE_CHARM_ERROR:
		{
			CDuiString pzErrDesc = _T("兑换失败");
			tagError* pStuError = static_cast<tagError*>(pDataBuffer);
			if (!pStuError) return true;

			g_Logger->TryInfo(_T("BankFrame - Exchange Charms Failed [code = %ld]"), pStuError->lErrorCode);

			pzErrDesc = pStuError->szErrorDescribe;
			BankMsgBoxID msg_box_id = BankMsgBoxID::MSG_BOX_ID_NONE;
			switch (pStuError->lErrorCode)
			{
				case 1:		msg_box_id = BankMsgBoxID::MSG_BOX_ID_CHARM_PW;			break;
				case 2:		msg_box_id = BankMsgBoxID::MSG_BOX_ID_CHARM_COUNT;		break;
				default:	msg_box_id = BankMsgBoxID::MSG_BOX_ID_CHARM_COUNT;		break;
			}

			DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, pzErrDesc.GetData()
				, (DLG_ID)msg_box_id
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

void BankFrame::CheckLogin()
{
	CMD_GF_BankInfo tBankInfo;
	memset(&tBankInfo, 0, sizeof(CMD_GF_BankInfo));
	tBankInfo.cbGameAction = 0;
	tBankInfo.lBankScore = m_pSelfData->GetBankScore();
	tBankInfo.lGameScore = m_pSelfData->GetGameGold();
	::StrToCharArray(tBankInfo.szAccounts, m_pSelfData->GetAccount());

	g_Logger->TryInfo(_T("BankFrame - Bank Info Req"));
	DWORD bRes = m_pLoginServer->SendData(MDM_GF_BANK, SUB_GF_BANK_INFO, &tBankInfo, sizeof(CMD_GF_BankInfo));
	if (FALSE == bRes)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("获取银行数据失败!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_NONE)
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void BankFrame::CheckSave()
{
	CDuiString& pzSave = m_pSwitch1_EdtSaveNum->GetText();
	DWORD dwSaveNum = _ttoll(pzSave.GetData());
	if (pzSave.GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("存入金额不能为空!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_SAVE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (dwSaveNum <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("存入金额不能为0!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_SAVE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (dwSaveNum > m_pSelfData->GetGameGold())
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("输入金额过大!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_SAVE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else
	{
		CMD_GF_BankAction tBankAction;
		memset(&tBankAction, 0, sizeof(CMD_GF_BankStorage));
		tBankAction.btActionID = 1;
		tBankAction.lScore = dwSaveNum;
		::StrToCharArray(tBankAction.szAccounts, m_pSelfData->GetAccount());

		g_Logger->TryInfo(_T("BankFrame - Bank Save Req"));
		DWORD bRes = m_pLoginServer->SendData(MDM_GF_BANK, SUB_GF_BANK_SAVE, &tBankAction, sizeof(CMD_GF_BankAction));
		if (FALSE == bRes)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, _T("存入金额失败，请稍后再试!")
				, DLG_ID(BankMsgBoxID::MSG_BOX_ID_NONE)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	}
}

void BankFrame::CheckTake()
{
	CDuiString& pzTakeNum = m_pSwitch1_EdtTakeNum->GetText();
	CDuiString& pzTakePassword = m_pSwitch1_EdtTakePassword->GetText();
	DWORD dwTakeNum = _ttoll(pzTakeNum.GetData());
	if (pzTakeNum.GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("取款金额不能为空!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_TAKE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (pzTakePassword.GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("取款密码不能为空!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_TAKE_PW)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (dwTakeNum <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("取款金额不能为0!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_TAKE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (dwTakeNum > m_pSelfData->GetBankScore())
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("输入金额过大!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_TAKE)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else
	{
		// 发送
		CMD_GF_BankAction tBankAction;
		memset(&tBankAction, 0, sizeof(CMD_GF_BankStorage));
		tBankAction.btActionID = 2;
		tBankAction.lScore = _ttoll(pzTakeNum.GetData());
		::StrToCharArray(tBankAction.szAccounts, m_pSelfData->GetAccount());
		CMD5Encrypt::EncryptData(pzTakePassword.GetData(), tBankAction.szPassword);

		g_Logger->TryInfo(_T("BankFrame - Bank Withdraw Req"));
		DWORD bRes = m_pLoginServer->SendData(MDM_GF_BANK, SUB_GF_BANK_WITHDRAW, &tBankAction, sizeof(CMD_GF_BankAction));
		if (FALSE == bRes)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, _T("取款操作失败，请稍后再试!")
				, DLG_ID(BankMsgBoxID::MSG_BOX_ID_NONE)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}

		// Unreal Check
		UnrealControl* pUnrealControl = UnrealControl::GetInstance();
		pUnrealControl->CheckUnreal_User(UnrealNSP::UnrealChargeType::UCT_PLAZA_WITHDRAW, tBankAction.lScore, tBankAction.szPassword, LOCATION_PLAZA_ROOM);
	}
}

void BankFrame::CheckExchange()
{
	CDuiString& pzCount = m_pSwitch3_EdtExCount->GetText();
	CDuiString& pzPassword = m_pSwitch3_EdtTakePassword->GetText();
	if (pzCount.GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("单次兑换数量不能为空!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_CHARM_COUNT)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (!g_GameUtil->MatchRegular(pzCount, _T("[0-9]+")))
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("输入不合法!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_CHARM_COUNT)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (_ttoll(pzCount.GetData()) > pow(10, 4) - 1)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("单次兑换数量太大!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_CHARM_COUNT)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (!g_GameUtil->MatchWordByType(pzPassword, MatchWords_Type::REG_TYPE_PASSWORD))
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("密码输入不正确!")
			, DLG_ID(BankMsgBoxID::MSG_BOX_ID_CHARM_PW)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else
	{
		LONG lCharm = _ttol(m_pSwitch3_EdtExCount->GetText());
		BYTE cbSendLocation = LOCATION_PLAZA_ROOM;
		// 操作场所(大厅、房间)
		if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_ROOM)
		{
			cbSendLocation = LOCATION_GAME_ROOM;
		}

		CMD_GF_ExchangeCharm tExCharm;
		memset(&tExCharm, 0, sizeof(CMD_GF_ExchangeCharm));
		tExCharm.cbSendLocation = cbSendLocation;
		tExCharm.dwUserID = m_pSelfData->GetUserID();
		tExCharm.lCharm = lCharm;
		CMD5Encrypt::EncryptData(pzPassword.GetData(), tExCharm.szPassword);

		g_Logger->TryInfo(_T("BankFrame - Exchange Charms Req"));
		DWORD bRes = m_pLoginServer->SendData(MDM_GF_PRESENT, SUB_GF_EXCHANGE_CHARM, &tExCharm, sizeof(CMD_GF_ExchangeCharm));
		if (FALSE == bRes)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, _T("兑换魅力失败!")
				, DLG_ID(BankMsgBoxID::MSG_BOX_ID_NONE)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}

		// Unreal Check
		UnrealControl* pUnrealControl = UnrealControl::GetInstance();
		pUnrealControl->CheckUnreal_User(UnrealNSP::UnrealChargeType::UCT_CHARM, tExCharm.lCharm, tExCharm.szPassword, LOCATION_PLAZA_ROOM);
	}
}

void BankFrame::CheckRefresh()
{
	int iCurSel = m_pTabLaout->GetCurSel();
	switch (iCurSel)
	{
		// 存取款面板
		case 0: break;

		// 修改密码面板
		case 1: break;

		// 魅力兑换面板
		case 2:
		{
			this->CheckLogin();

			// Unreal Check
			UnrealControl* pUnrealControl = UnrealControl::GetInstance();
			pUnrealControl->CheckUnreal_Robot();
		}break;

		default:
			break;
	}
}

void BankFrame::CheckModifyPW()
{
	CDuiString& pzSrcPW = m_pSwitch2_EdtSrcPw->GetText();
	CDuiString& pzNewPW = m_pSwitch2_EdtNewPw->GetText();
	CDuiString& pzCfmPW = m_pSwitch2_EdtCfmPw->GetText();
	bool bCheck = true;
	BankMsgBoxID iErr_FoucsID = BankMsgBoxID::MSG_BOX_ID_NONE;
	LPCTSTR	szTipsTitle = _T("温馨提示");
	LPCTSTR	szTipsContent = _T("输入格式不正确");
	for (size_t i = 0; i < m_vPwComplete.size(); ++i)
	{
		bCheck &= m_vPwComplete[i];
		if (!bCheck)
		{
			switch (i)
			{
				case 0:
				{
					iErr_FoucsID = BankMsgBoxID::MSG_BOX_ID_SRC_PW;
					if (m_pSwitch2_EdtSrcPw->GetText().GetLength() <= 0)
					{
						szTipsContent = _T("原密码不能为空!");
					}
				}break;

				case 1:
				{
					iErr_FoucsID = BankMsgBoxID::MSG_BOX_ID_NEW_PW;
					if (m_pSwitch2_EdtNewPw->GetText().GetLength() <= 0)
					{
						szTipsContent = _T("新密码不能为空!");
					}
				}break;

				case 2:
				{
					iErr_FoucsID = BankMsgBoxID::MSG_BOX_ID_CFM_PW;
					szTipsContent = _T("两次输入不一致!");
				}break;

				default:
				{
					iErr_FoucsID = BankMsgBoxID::MSG_BOX_ID_SRC_PW;;
				}break;
			}

			DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, szTipsTitle
				, szTipsContent
				, (DLG_ID)iErr_FoucsID
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb
				, dlg_cb
				, dlg_cb);

			return;
		}
	}
	
	if (bCheck)
	{
		TCHAR pzBuff[MAX_PATH] = { 0 };
		CMD_GF_BankSecurity tBankSecurity;
		memset(&tBankSecurity, 0, sizeof(CMD_GF_BankSecurity));
		::StrToCharArray(tBankSecurity.szAccounts, m_pSelfData->GetAccount());
		::StrToCharArray(tBankSecurity.szPassword, pzSrcPW.GetData());
		CMD5Encrypt::EncryptData(tBankSecurity.szPassword, tBankSecurity.szPassword);
		::StrToCharArray(tBankSecurity.szNewPassword, pzNewPW.GetData());
		CMD5Encrypt::EncryptData(tBankSecurity.szNewPassword, tBankSecurity.szNewPassword);

		g_Logger->TryInfo(_T("BankFrame - Bank Security Req"));
		DWORD bRes = m_pLoginServer->SendData(MDM_GF_BANK, SUB_GF_BANK_SECURITY, &tBankSecurity, sizeof(CMD_GF_BankSecurity));
		if (FALSE == bRes)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(BankFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, _T("修改密码失败，请稍后再试!")
				, DLG_ID(BankMsgBoxID::MSG_BOX_ID_NONE)
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	}
}

void BankFrame::ChangeTipsImage(CControlUI* pControl, DWORD dwStatus)
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

void BankFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (type == DUI_MSGTYPE_CLICK)
	{
		if (_tcscmp(sName, R->kBtnBank_Close) == 0)
		{
			this->Close(CMW_CLOSE_SURE);
		}

		// 存入
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn0) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtSaveNum, 10 * 10000, true);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn1) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtSaveNum, 100 * 10000, true);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn2) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtSaveNum, 500 * 10000, true);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn3) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtSaveNum, 1000 * 10000, true);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn4) == 0)
		{
			DWORD dwNums = m_pSelfData->GetGameGold();
			this->AddTempGold(m_pSwitch1_EdtSaveNum, dwNums, true);
		}

		// 取出
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn5) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtTakeNum, 10 * 10000, false);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn6) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtTakeNum, 100 * 10000, false);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn7) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtTakeNum, 500 * 10000, false);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn8) == 0)
		{
			this->AddTempGold(m_pSwitch1_EdtTakeNum, 1000 * 10000, false);
		}
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Btn9) == 0)
		{
			DWORD dwNums = m_pSelfData->GetBankScore();
			this->AddTempGold(m_pSwitch1_EdtTakeNum, dwNums, false);
		}
		
		// 存款按钮
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Save) == 0)
		{
			IPC* ipc = IPC::GetInstance();
			if (ipc->IsGameStarted() || m_pSelfData->IsGaming())
			{
				DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("正在游戏中不能进行此操作!"));
			}
			else if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_ROOM)
			{
				DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("房间中不能进行此操作!"));
			}
			else
			{
				if (m_pLoginServer->IsConnect())
				{
					this->CheckSave();
				}
				else
				{
					m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_SAVE;
					m_pLoginServer->Connect();
				}
			}
		}
		
		// 取款按钮
		else if (_tcscmp(sName, R->kBtnBankSwitch1_Take) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->CheckTake();
			}
			else
			{
				m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_TAKE;

				TNotifyUI msg;
				memset(&msg, 0, sizeof(TNotifyUI));
				this->ReConnect(DLG_ID::DLG_ID_NONE, msg);
			}
		}
		
		// 确认修改密码
		else if (_tcscmp(sName, R->kBtnBankSwitch2_Ensure) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->CheckModifyPW();
			}
			else
			{
				m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_MODIFY_PW;

				TNotifyUI msg;
				memset(&msg, 0, sizeof(TNotifyUI));
				this->ReConnect(DLG_ID::DLG_ID_NONE, msg);
			}
		}

		// 取款时勾选(记住取款密码)
		else if (_tcscmp(sName, R->kOptBankSwitch1_MarkPW) == 0)
		{
			// 统一关闭窗口时保存设置
		}

		// 兑换勾选(限制最大值)
		else if (_tcscmp(sName, R->kOptBankSwitch3_Opt) == 0)
		{
			COptionUI* pOpt = static_cast<COptionUI*>(msg.pSender);
			if (pOpt)
			{
				this->TickingMaxCharms(!pOpt->IsSelected());
			}
		}
		
		// 兑换时勾选(记住取款密码)
		else if (_tcscmp(sName, R->kOptBankSwitch3_MarkPW) == 0)
		{
			// 统一关闭窗口时保存设置
		}

		// 兑换确认
		else if (_tcscmp(sName, R->kBtnBankSwitch3_Ensure) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->CheckExchange();
			}
			else
			{
				m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_EXCHANGE;
				m_pLoginServer->Connect();
			}
		}

		// 兑换刷新
		else if (_tcscmp(sName, R->kBtnBankSwitch3_Refresh) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->CheckRefresh();
			}
			else
			{
				m_iCurOperateStatus = BanckOperateStatus::BANCK_OS_REFRESH;
				this->CheckRefresh();
			}
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_SELECTCHANGED) == 0)
	{
		if (_tcscmp(sName, R->kOptBank_Op_1) == 0 && m_pTabLaout)
		{
			m_dwCurPageIndex = 0;
			m_pTabLaout->SelectItem(m_dwCurPageIndex);
			m_PaintManager.SetFocusNeeded(m_pSwitch1_EdtSaveNum);
			this->UpdateBankPanel();
		}
		else if (_tcscmp(sName, R->kOptBank_Op_2) == 0 && m_pTabLaout)
		{
			m_dwCurPageIndex = 1;
			m_pTabLaout->SelectItem(m_dwCurPageIndex);
			m_PaintManager.SetFocusNeeded(m_pSwitch2_EdtSrcPw);
			this->UpdateBankPanel();
		}
		else if (_tcscmp(sName, R->kOptBank_Op_3) == 0 && m_pTabLaout)
		{
			m_dwCurPageIndex = 2;
			m_pTabLaout->SelectItem(m_dwCurPageIndex);
			m_PaintManager.SetFocusNeeded(m_pSwitch3_EdtExCount);
			this->UpdateBankPanel();
		}
		else if (_tcscmp(sName, R->kOptBank_Op_4) == 0 && m_pTabLaout)
		{
			// 加载网页
			if (m_pSwitch4_WebCharmsDetail)
			{
				LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("CharmsDetail"));
				_stprintf_s(m_pzBuffer, _T("%s%lu"), lpUrl, m_pSelfData->GetUserID());
				m_pSwitch4_WebCharmsDetail->Navigate(m_pzBuffer);
			}

			m_dwCurPageIndex = 3;
			m_pTabLaout->SelectItem(m_dwCurPageIndex);
			this->UpdateBankPanel();
		}
		else if (_tcscmp(sName, R->kOptBank_Op_5) == 0 && m_pTabLaout)
		{
			// 加载网页
			if (m_pSwitch5_WebScoreRebate)
			{
				LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("ScoreRebate"));
				_stprintf_s(m_pzBuffer, _T("%s%lu"), lpUrl, m_pSelfData->GetUserID());
				m_pSwitch5_WebScoreRebate->Navigate(m_pzBuffer);
			}

			m_dwCurPageIndex = 4;
			m_pTabLaout->SelectItem(m_dwCurPageIndex);
			this->UpdateBankPanel();
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_SETFOCUS) == 0)
	{
		if (_tcscmp(sName, R->kEdtBankSwitch2_SrcPW) == 0)
		{
			this->ChangeTipsImage(m_pSwitch2_CtlSrcPw, 0);
		}
		else if (_tcscmp(sName, R->kEdtBankSwitch2_NewPW) == 0)
		{
			this->ChangeTipsImage(m_pSwitch2_CtlNewPw, 0);
		}
		else if (_tcscmp(sName, R->kEdtBankSwitch2_CfmPW) == 0)
		{
			this->ChangeTipsImage(m_pSwitch2_CtlCfmPw, 0);
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_KILLFOCUS) == 0)
	{
		if (_tcscmp(sName, R->kEdtBankSwitch2_SrcPW) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_PASSWORD);
			if (bRes)
			{
				dwStatus = 2;
			}
			else
			{
				dwStatus = 1;
			}

			m_vPwComplete[0] = bRes;
			this->ChangeTipsImage(m_pSwitch2_CtlSrcPw, dwStatus);
		}
		else if (_tcscmp(sName, R->kEdtBankSwitch2_NewPW) == 0)
		{
			DWORD dwStatus = 1;
			bool bRes = g_GameUtil->MatchWordByType(msg.pSender->GetText(), MatchWords_Type::REG_TYPE_PASSWORD);
			if (bRes)
			{
				dwStatus = 2;
			}
			else
			{
				dwStatus = 1;
			}

			m_vPwComplete[1] = bRes;
			this->ChangeTipsImage(m_pSwitch2_CtlNewPw, dwStatus);
		}
		else if (_tcscmp(sName, R->kEdtBankSwitch2_CfmPW) == 0)
		{
			this->ChangeTipsImage(m_pSwitch2_CtlCfmPw, 0);
			DWORD dwStatus = 1;
			bool bRes = m_pSwitch2_EdtNewPw->GetText() == m_pSwitch2_EdtCfmPw->GetText();
			bRes &= m_pSwitch2_EdtCfmPw->GetText().GetLength() > 0;
			if (bRes)
			{
				dwStatus = 2;
			}
			else
			{
				dwStatus = 1;
			}

			m_vPwComplete[2] = bRes;
			this->ChangeTipsImage(m_pSwitch2_CtlCfmPw, dwStatus);
		}
	}
}

LRESULT BankFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (m_PaintManager.GetRoot()->IsKeyboardEnabled())
	{
		if (wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			Close(CMW_CLOSE_SURE);
			return TRUE;
		}
	}
	return FALSE;
}