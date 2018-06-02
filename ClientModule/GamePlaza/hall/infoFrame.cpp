#include "stdafx.h"
#include "infoFrame.h"
#include "util/gameUtil.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

InfoFrame* InfoFrame::g_instance = NULL;
InfoFrame::InfoFrame()
	: m_dwSwitchIndex(0)
	, m_llVertcodeSec(0)
	, m_pWaitDlg(nullptr)
	, m_pInfoSwitch(nullptr)
	, m_pSelfData(nullptr)
	, m_pParentManager(nullptr)
	, m_pLoginServer(nullptr)
	, m_iCurOperateStatus(InfoOperateStatus::INFO_OS_NONE)
	, m_pSwitch1_EdtAccount(nullptr)
	, m_pSwitch1_EdtRealName(nullptr)
	, m_pSwitch1_EdtTelphone(nullptr)
	, m_pSwitch1_EdtIDCard(nullptr)
	, m_pSwitch1_EdtNickName(nullptr)
	, m_pSwitch1_EdtGender(nullptr)
	, m_pSwitch1_EdtEmail(nullptr)
	, m_pSwitch1_EdtQQNumber(nullptr)
	, m_pSwitch1_ComboGender(nullptr)
	, m_pSwitch1_EdtUnderWrite(nullptr)
	, m_pSwitch1_LabRemainWords(nullptr)
	, m_pSwitch3_LabOldTel(nullptr)
	, m_pSwitch3_EdtNewTel(nullptr)
	, m_pSwitch3_EdtVCode(nullptr)
	, m_pSwitch3_BtnGetVCode(nullptr)
	, m_pSwitch3_BtnBind(nullptr)
	, m_pSwitch3_EdtOldPw(nullptr)
	, m_pSwitch3_EdtNewPw(nullptr)
	, m_pSwitch3_EdtCfmPw(nullptr)
	, m_pSwitch3_BtnEnsure(nullptr)
	, m_pSwitch4_OptWin(nullptr)
	, m_pSwitch4_EdtWin(nullptr)
	, m_pSwitch4_OptFlee(nullptr)
	, m_pSwitch4_EdtFlee(nullptr)
	, m_pSwitch4_OptPassword(nullptr)
	, m_pSwitch4_EdtPassword(nullptr)
	, m_pSwitch4_OptScore(nullptr)
	, m_pSwitch4_EdtScoreMin(nullptr)
	, m_pSwitch4_EdtScoreMax(nullptr)
	, m_pSwitch4_OptSameIP(nullptr)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer));
	memset(&m_tTempUnserInfo, 0, sizeof(tagLS_UpdateUserInfo));
}

InfoFrame::~InfoFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);
	g_GameUtil->ShowTopWindow(m_pParentManager->GetPaintWindow());

	if (LoginServer::IsInstanceExist())
	{
		m_pLoginServer->SetDelegate(nullptr, m_pParentManager);
		m_pLoginServer->CloseSocket();
	}

	g_Logger->TryInfo(_T("InfoFrame - Destroy"));
}

InfoFrame* InfoFrame::GetInstance()
{
	if (InfoFrame::g_instance == NULL)
	{
		InfoFrame::g_instance = new InfoFrame();
	}

	return InfoFrame::g_instance;
}

bool InfoFrame::IsInstanceExist()
{
	return InfoFrame::g_instance != nullptr;
}

BOOL InfoFrame::CreateFrame(CPaintManagerUI* pParentManager, int infoPanelIndex /* = 0 */)
{
	m_pParentManager = pParentManager;
	if (FALSE == IsWindow(m_hWnd))
	{
		//若有索引,先保存索引,创建完窗口,在ShowPanelByIndex
		m_dwSwitchIndex = infoPanelIndex;

		HWND hParent = m_pParentManager->GetPaintWindow();
		g_GameUtil->SetFrozenEnable(m_pParentManager, false);
		m_hWnd = this->Create(hParent, NULL, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
		this->ShowPanelByIndex(infoPanelIndex);
	}

	return TRUE;
}

UILIB_RESOURCETYPE InfoFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR InfoFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString InfoFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString InfoFrame::GetSkinFile()
{
	return _T("hall/info/info.xml");
}

LRESULT InfoFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == CMW_CLOSE_SURE)
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	g_GameUtil->MinimizeTray(*this);
	return 0;
}

void InfoFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("InfoFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(InfoFrame::g_instance);
}

CControlUI* InfoFrame::CreateControl(LPCTSTR pstrClassName)
{
	CControlUI* pControl = NULL;
	CDialogBuilder builder;

	if (_tcsicmp(pstrClassName, _T("info_switch_1")) == 0)
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/info/info_switch_1.xml"), (UINT)0, this, &m_PaintManager));
	else if (_tcsicmp(pstrClassName, _T("info_switch_2")) == 0)
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/info/info_switch_2.xml"), (UINT)0, this, &m_PaintManager));
	else if (_tcsicmp(pstrClassName, _T("info_switch_3")) == 0)
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/info/info_switch_3.xml"), (UINT)0, this, &m_PaintManager));
	else if (_tcsicmp(pstrClassName, _T("info_switch_4")) == 0)
		pControl = static_cast<CControlUI*>(builder.Create(_T("hall/info/info_switch_4.xml"), (UINT)0, this, &m_PaintManager));
	else if (_tcscmp(pstrClassName, DUI_CTR_SCREENSHOTBOX) == 0)
		pControl = new CScreenshotBoxUI(m_hWnd);

	return pControl;
}

void InfoFrame::InitWindow()
{
	g_Logger->TryInfo(_T("InfoFrame - Open"));

	// 获取个人对象实例
	if (!m_pSelfData)
	{
		DataManager* pDataManager = DataManager::GetInstance();
		m_pSelfData = &pDataManager->GetSelfData();
	}

	// 公用控件
	do 
	{
		m_pInfoSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsInfo_Switch));
	} while (0);

	// 个人资料面板
	do
	{
		// 设置账号、昵称信息等固定信息
		do
		{
			m_pSwitch1_EdtAccount = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_Account));
			m_pSwitch1_EdtNickName = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_NickName));

			if (m_pSwitch1_EdtAccount)
			{
				m_pSwitch1_EdtAccount->SetReadOnly(true);
				m_pSwitch1_EdtAccount->SetReadOnlyShowCaret(true);
				m_pSwitch1_EdtAccount->SetText(m_pSelfData->GetAccount());
				m_PaintManager.SetFocusNeeded(m_pSwitch1_EdtAccount);
			}

			if (m_pSwitch1_EdtNickName)
			{
				m_pSwitch1_EdtNickName->SetText(m_pSelfData->GetNickName());
			}
		} while (0);

		// 初始化其他控件
		do
		{
			// 真实姓名
			m_pSwitch1_EdtRealName	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_Name));

			// 电话
			m_pSwitch1_EdtTelphone	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_Tel));

			// 身份证
			m_pSwitch1_EdtIDCard	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_ID));

			// 性别
			m_pSwitch1_EdtGender	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_Gender));
			m_pSwitch1_ComboGender	= static_cast<CComboUI*>(m_PaintManager.FindControl(R->kCmbInfoSwitch_1_Gender));

			// 邮箱
			m_pSwitch1_EdtEmail		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_Email));

			// QQ号
			m_pSwitch1_EdtQQNumber	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_QQ));
		} while (0);

		// 个人说明字数限制显示
		do
		{
			m_pSwitch1_EdtUnderWrite	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch1_UnderWrite));
			m_pSwitch1_LabRemainWords	= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kLabInfoSwitch1_RWords));

			m_pSwitch1_EdtUnderWrite->SetLimitText(UNDER_WRITE_LEN);
			int iLimitText = m_pSwitch1_EdtUnderWrite->GetLimitText();
			_stprintf_s(m_pzBuffer, _T("%d/%d"), iLimitText, iLimitText);
			m_pSwitch1_LabRemainWords->SetText(m_pzBuffer);
		} while (0);
	} while (0);

	// 修改安全设置面板
	do 
	{
		// 修改手机号面板控件
		do
		{
			m_pSwitch3_LabOldTel		= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabInfoSwitch3_OldTel));
			m_pSwitch3_EdtNewTel		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch3_NewTel));
			m_pSwitch3_EdtVCode			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch3_VCode));

			m_pSwitch3_BtnGetVCode		= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnInfoSwitch3_GetVCode));
			m_pzSwitch3_BtnGetVCodeText = m_pSwitch3_BtnGetVCode->GetText();
			m_pSwitch3_BtnBind			= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnInfoSwitch3_Bind));
		} while (0);

		// 修改密码面板控件
		do
		{
			m_pSwitch3_EdtOldPw			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch3_OldPw));
			m_pSwitch3_EdtNewPw			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch3_NewPw));
			m_pSwitch3_EdtCfmPw			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch3_CfmPW));
			m_pSwitch3_BtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnInfoSwitch3_Ensure));
		} while (0);
	} while (0);
	
	// 修改游戏设置面板
	do 
	{
		m_pSwitch4_OptWin			= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptInfoSwitch4_Win));
		m_pSwitch4_OptFlee			= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptInfoSwitch4_Flee));
		m_pSwitch4_OptPassword		= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptInfoSwitch4_Password));
		m_pSwitch4_OptScore			= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptInfoSwitch4_Score));
		m_pSwitch4_OptSameIP		= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptInfoSwitch4_SameIP));

		m_pSwitch4_EdtWin			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch4_Win));
		m_pSwitch4_EdtFlee			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch4_Flee));
		m_pSwitch4_EdtPassword		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch4_Password));
		m_pSwitch4_EdtScoreMin		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch4_ScoreMin));
		m_pSwitch4_EdtScoreMax		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtInfoSwitch4_ScoreMax));
		
		const CMD_GR_UserRule& tGameRule = m_pSelfData->GetGameSetting();
		m_pSwitch4_OptWin->Selected(tGameRule.bLimitWin);
		m_pSwitch4_OptFlee->Selected(tGameRule.bLimitFlee);
		m_pSwitch4_OptPassword->Selected(tGameRule.bPassword);
		m_pSwitch4_OptScore->Selected(tGameRule.bLimitScore);
		m_pSwitch4_OptSameIP->Selected(tGameRule.bCheckSameIP);

		_stprintf_s(m_pzBuffer, _T("%u"), tGameRule.wWinRate);
		m_pSwitch4_EdtWin->SetText(m_pzBuffer);

		_stprintf_s(m_pzBuffer, _T("%u"), tGameRule.wFleeRate);
		m_pSwitch4_EdtFlee->SetText(m_pzBuffer);
		
		m_pSwitch4_EdtPassword->SetText(tGameRule.szPassword);

		_stprintf_s(m_pzBuffer, _T("%lld"), tGameRule.lLessScore);
		m_pSwitch4_EdtScoreMin->SetText(m_pzBuffer);

		_stprintf_s(m_pzBuffer, _T("%lld"), tGameRule.lMaxScore);
		m_pSwitch4_EdtScoreMax->SetText(m_pzBuffer);
	} while (0);
}

void InfoFrame::InitData(TNotifyUI& msg)
{
	// 检测"选择头面板"是否开启自定义头像功能
	this->CheckCustomPicPanel();
	
	// 显示指定索引面板
	this->ShowPanelByIndex(m_dwSwitchIndex);
	
	// 设置登录服委托对象
	do
	{
		m_pLoginServer = LoginServer::GetInstance();
		m_pLoginServer->SetDelegate(this, &m_PaintManager);
		if (m_pLoginServer->IsConnect())
		{
			this->ReqGetInfo();
		}
		else
		{
			m_iCurOperateStatus = InfoOperateStatus::INFO_OS_GET_INFO;
			m_pLoginServer->Connect();
		}
	} while (0);

	g_Logger->TryInfo(_T("InfoFrame - Open Complete"));
}

void InfoFrame::ShowLeftWords()
{
	if (!m_pSwitch1_EdtUnderWrite) return;

	int iRealNum = UNDER_WRITE_LEN - 2;
	int iLeftNum = iRealNum - m_pSwitch1_EdtUnderWrite->GetText().GetLength();
	iLeftNum = iLeftNum < 0 ? 0 : iLeftNum;
	
	_stprintf_s(m_pzBuffer, _T("%d/%d"), iLeftNum, iRealNum);
	m_pSwitch1_LabRemainWords->SetText(m_pzBuffer);
}

void InfoFrame::ShowPanelByIndex(int infoPanelIndex /* = 0 */)
{
	m_dwSwitchIndex = infoPanelIndex;
	CTreeViewUI* treeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(R->KTewInfo_View));
	if (!treeView) return;

	int iCurSel = 0;
	int iCount = treeView->GetList()->GetCount();
	switch (m_dwSwitchIndex)
	{
		case 0:
			iCurSel = 0; break;
		case 1:
			iCurSel = 2; break;
		case 2:
			iCurSel = iCount - 4; break;
		case 3:
			iCurSel = iCount - 2; break;
		default:
			break;
	}

	treeView->SelectItem(iCurSel);
	if (m_pInfoSwitch)
	{
		m_pInfoSwitch->SelectItem(m_dwSwitchIndex);

		if (1 == m_dwSwitchIndex)
		{
			this->LayoutHeadPanel();
		}
	}
}

bool InfoFrame::CheckCustomPicPanel()
{
	CTreeViewUI* pView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(R->KTewInfo_View));
	CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(m_PaintManager.FindControl(R->kOptInfo_Op_2));
	if (pNode && pNode->GetCountChild() > 1)
	{
		CHorizontalLayoutUI* pHoriz = pNode->GetTreeNodeHoriznotal();
		CCheckBoxUI* pFol = new CCheckBoxUI();
		pFol->SetName(R->kOptInfo_HeadOpt);
		pFol->SetFixedWidth(20);
		pFol->SetMouseEnabled(true);
		pFol->SetAttribute(_T("normalimage"),		_T("file='common/list/tree_btn_01.png' aligncenter='true' source='0,16,16,32'"));
		pFol->SetAttribute(_T("hotimage"),			_T("file='common/list/tree_btn_01.png' aligncenter='true' source='0,0,16,16'"));
		pFol->SetAttribute(_T("selectedimage"),		_T("file='common/list/tree_btn_01.png' aligncenter='true' source='16,16,32,32'"));
		pFol->SetAttribute(_T("selectedhotimage"),	_T("file='common/list/tree_btn_01.png' aligncenter='true' source='16,0,32,16'"));
		pHoriz->Add(pFol);

		return true;
	}

	return false;
}

void InfoFrame::ShowWaittingText(bool bShow, CDuiString pzText /* = _T("") */)
{
	if (bShow)
	{
		if (!m_pWaitDlg)
		{
			m_pWaitDlg = DialogFrame::ToastWait(&m_PaintManager, pzText);
		}

		// 适配位置
		CControlUI* pSubControl = m_pWaitDlg->GetSubControl();
		if (pSubControl)
		{
			SIZE pos = pSubControl->GetFixedXY();
			pos.cx = m_pInfoSwitch->GetFixedXY().cx + (m_pInfoSwitch->GetFixedWidth() - pSubControl->GetFixedWidth()) / 2;
			pSubControl->SetFixedXY(pos);
		}
	}
	else
	{
		DialogFrame::EndToastBox(m_pWaitDlg);
		m_pWaitDlg = nullptr;
	}

	// 禁用面板与否
	CControlUI* pControl = m_PaintManager.FindControl(MsgBoxWnd::g_kCtl_FadeLayer);
	if (pControl)
	{
		pControl->SetVisible(bShow);
	}
}

void InfoFrame::ShowToastText(CDuiString pzText /* = _T("")  */
	, DLG_ID id /* = DLG_ID::DLG_ID_NONE  */
	, DialogCallBack pCallBackFunc /* = nullptr  */
	, bool bFadeOut /* = false */)
{
	CControlUI* pToastCtl = DialogFrame::Toast(&m_PaintManager, pzText, id, pCallBackFunc, bFadeOut);
	if (pToastCtl)
	{
		SIZE pos = pToastCtl->GetFixedXY();
		pos.cx = m_pInfoSwitch->GetFixedXY().cx + (m_pInfoSwitch->GetFixedWidth() - pToastCtl->GetFixedWidth()) / 2;
		pToastCtl->SetFixedXY(pos);
	}
}

void InfoFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (MSGBOX_EVENT_ID(id))
	{
		case InfoFrame::MB_ID_NONE:																break;
		case InfoFrame::MB_ID_SWITCH1_GENDER:													break;
		case InfoFrame::MB_ID_SWITCH1_NiCK_NAME:		m_pSwitch1_EdtNickName->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH1_REAL_NAME:		m_pSwitch1_EdtRealName->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH1_TEL:				m_pSwitch1_EdtTelphone->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH1_IDCARD:			m_pSwitch1_EdtIDCard->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH1_EMAIL:			m_pSwitch1_EdtEmail->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH1_QQ:				m_pSwitch1_EdtQQNumber->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH1_UNDERWRITE:		m_pSwitch1_EdtUnderWrite->SetFocus();	break;
		case InfoFrame::MB_ID_SWITCH3_NEW_TEL:			m_pSwitch3_EdtNewTel->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH3_VCODE:			m_pSwitch3_EdtVCode->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH3_OLD_PW:			m_pSwitch3_EdtOldPw->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH3_NEW_PW:			m_pSwitch3_EdtNewPw->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH3_CFM_PW:			m_pSwitch3_EdtCfmPw->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH4_PASSWORD:			m_pSwitch4_EdtPassword->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH4_LIMIT_SCORE_MIN:	m_pSwitch4_EdtScoreMin->SetFocus();		break;
		case InfoFrame::MB_ID_SWITCH4_LIMIT_SCORE_MAX:	m_pSwitch4_EdtScoreMax->SetFocus();		break;
		case InfoFrame::MB_ID_EXTRA:															break;
		default:																				break;
	}
}

bool InfoFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case InfoOperateStatus::INFO_OS_NONE:											break;
		case InfoOperateStatus::INFO_OS_GET_INFO:			this->ReqGetInfo();			break;
		case InfoOperateStatus::INFO_OS_MODIFY_INFO:		this->ModifyUserInfo();		break;
		case InfoOperateStatus::INFO_OS_MODIFY_HEAD_PIC:	this->ModifyHeadPic();		break;
		case InfoOperateStatus::INFO_OS_MODIFY_TEL:			this->ModifyTelphone();		break;
		case InfoOperateStatus::INFO_OS_MODIFY_PW:			this->ModifyPassword();		break;
		case InfoOperateStatus::INFO_OS_GET_VCODE:			this->GetServerSMS();		break;
		case InfoOperateStatus::INFO_OS_EXTRA:											break;
		default:																		break;
	}
	m_iCurOperateStatus = InfoOperateStatus::INFO_OS_NONE;

	return true;
}

bool InfoFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}
void InfoFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

bool InfoFrame::OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	ShowWaittingText(false);
	switch (cmd.wSubCmdID)
	{
		case SUB_LS_USERINFO_GET_OK:
		{
			g_Logger->TryInfo(_T("InfoFrame - Userinfo Get OK"));
			tagLS_UpdateUserInfo* pUserInfo = static_cast<tagLS_UpdateUserInfo*>(pDataBuffer);
			if (pUserInfo)
			{
				memcpy(&m_tTempUnserInfo, pUserInfo, sizeof(tagLS_UpdateUserInfo));
				this->UpdateInfo(&m_tTempUnserInfo);
				this->FrozenModify();
			}
			else
			{
				DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("获取个人信息数据失败!"));
			}
		}break;

		case SUB_LS_USERINFO_GET_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;
			
			g_Logger->TryInfo(_T("InfoFrame - Userinfo Get Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { _T("获取个人信息数据失败!") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrorMSG);
		}break;

		case SUB_LS_USERINFO_UPDATE_OK:
		{
			g_Logger->TryInfo(_T("InfoFrame - Userinfo Update OK"));

			ShowToastText(_T("修改成功"));
			tagLS_UpdateUserInfo* pUserInfo = static_cast<tagLS_UpdateUserInfo*>(pDataBuffer);
			if (pUserInfo)
			{
				memcpy(&m_tTempUnserInfo, pUserInfo, sizeof(tagLS_UpdateUserInfo));
				this->FrozenModify();
			}
			else
			{
				DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("获取个人信息数据失败!"));
			}

			this->UpdateInfo(&m_tTempUnserInfo);
		}break;

		case SUB_LS_USERINFO_UPDATE_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;

			g_Logger->TryInfo(_T("InfoFrame - Userinfo Update Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { _T("修改个人信息失败!") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrorMSG);
		}break;

		case SUB_LS_USERINFO_MODTEL_OK:
		{
			g_Logger->TryInfo(_T("InfoFrame - Userinfo Modtel OK"));

			ShowToastText(_T("手机号修改成功"));

			// 更新手机号显示
			do 
			{
				CDuiString& pzNewTel = m_pSwitch3_EdtNewTel->GetText();
				::StrToCharArray(m_tTempUnserInfo.szTelphone, pzNewTel);

				memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
				_stprintf_s(m_pzBuffer, _T("%s****%s"), pzNewTel.Left(3).GetData(), pzNewTel.Right(3).GetData());
				m_pSwitch1_EdtTelphone->SetText(pzNewTel.GetData());
				m_pSwitch3_LabOldTel->SetText(m_pzBuffer);

				m_pSwitch3_EdtVCode->SetText(_T(""));
				m_pSwitch3_EdtNewTel->SetText(_T(""));
			} while (0);
		}break;

		case SUB_LS_USERINFO_MODTEL_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;

			g_Logger->TryInfo(_T("InfoFrame - Userinfo Modtel Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { _T("手机号修改失败!") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrorMSG);
		}break;

		case SUB_LS_USERINFO_MODPW_OK:
		{
			g_Logger->TryInfo(_T("InfoFrame - Userinfo Modpw OK"));
			ShowToastText(_T("密码修改成功"));
		}break;
		
		case SUB_LS_USERINFO_MODPW_ERROR:
		{
			tag_S_ErrorMsg* stu_ErrMSG = static_cast<tag_S_ErrorMsg*>(pDataBuffer);
			if (!stu_ErrMSG) return true;

			g_Logger->TryInfo(_T("InfoFrame - Userinfo Modpw Failed [code = %u]"), stu_ErrMSG->btErrorCode);

			TCHAR pzErrorMSG[MAX_PATH] = { _T("密码修改失败!") };
			_stprintf_s(pzErrorMSG, _T("%s"), stu_ErrMSG->szMsg);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), pzErrorMSG);
		}break;

		default:
			break;
	}

	return true;
}

bool InfoFrame::OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_SENDSMSSUCCESS:
		{
			g_Logger->TryInfo(_T("InfoFrame - Send SMS OK"));

			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			ShowToastText(_T("验证码发送成功!"), DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_VCODE), dlg_cb);

			// 验证码按钮开始间隔时间计时
			this->SetVertcodeTimer();
		}break;

		case SUB_LS2C_SMS_ERROR:
		{
			tag_S_ErrorMsg stu_ErrMSG;
			memset(&stu_ErrMSG, 0, sizeof(tag_S_ErrorMsg));
			memcpy(&stu_ErrMSG, pDataBuffer, wDataSize);

			g_Logger->TryInfo(_T("InfoFrame - Send SMS Failed [code = %u]"), stu_ErrMSG.btErrorCode);
			
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, stu_ErrMSG.szMsg
				, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_VCODE)
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

void InfoFrame::LayoutHeadPanel()
{
	CTileLayoutUI* panel = NULL;	//选中的面板节点
	LONG offset_x = 20;				//每个icon的间隔宽度
	LONG offset_y = 5;				//每个icon的间隔高度
	::CSize icon_size(80, 80);		//每个icon的原始大小

	LPCTSTR lpHeadFolderPath = _T("common\\headPic\\");
	panel = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(R->kTilInfoSwitch2_SysHeadPanel));
	if (!panel) return;

	panel->RemoveAll();
	if (panel->GetCount() > 0) return;

	RECT paddingRect = { 0, 0, 0, offset_y };
	::CSize tileSize = { 0, 0 };
	tileSize.cx = icon_size.cx + offset_x;
	tileSize.cy = icon_size.cy;
	panel->SetItemSize(tileSize);
	
	std::vector<FILE_STRUCT> fileStruct;
#ifdef SKIN_RES_TEST
	CString folderPath = CPaintManagerUI::GetResourcePath();
	folderPath += lpHeadFolderPath;
	g_GameUtil->traverseFolder(folderPath, fileStruct);
#else
	int iZipFileIndex = CRenderEngine::GetZipItemIndex(lpHeadFolderPath);
	if (iZipFileIndex != 0)
	{
		DWORD dwFileSize = 0;
		CDuiString pzFileName;
		while (CRenderEngine::TraverseZipFolder(lpHeadFolderPath, ++iZipFileIndex, pzFileName, dwFileSize))
		{
			FILE_STRUCT stu;
			stu.fileName = g_GameUtil->GetFileName(pzFileName).c_str();
			stu.name = g_GameUtil->GetFileNameWithoutExtension(pzFileName).c_str();
			stu.exName = g_GameUtil->GetExtension(pzFileName).c_str();
			stu.size = dwFileSize;
			fileStruct.push_back(stu);
		}
	}
#endif //SKIN_RES_TEST

	TCHAR pzBuffer[MAX_PATH] = { 0 };
	::ZeroMemory(pzBuffer, MAX_PATH * sizeof(TCHAR));
	for (auto it : fileStruct)
	{
		COptionUI* option = new COptionUI();
		option->SetPadding(paddingRect);
		option->SetFixedWidth(icon_size.cx);
		option->SetFixedHeight(icon_size.cy);

		_stprintf_s(pzBuffer, _T("info_switch_2-sys_head_%s"), it.name.c_str());
		option->SetName(pzBuffer);
		option->SetUserData(it.name.c_str());

		_stprintf_s(pzBuffer, _T("file='%s%s' aligncenter='true'"), lpHeadFolderPath, it.fileName.c_str());
		option->SetBkImage(pzBuffer);

		option->SetHotImage(_T("common/blocks/head_box.png"));
		option->SetSelectedImage(_T("common/blocks/head_icon_sel.png"));
		option->SetGroup(R->kTexInfoSysIcon);
		panel->Add(option);
	}
	fileStruct.clear();

	// 选中当前头像
	WORD wFaceID = m_tTempUnserInfo.wFaceID;
	_stprintf_s(pzBuffer, _T("info_switch_2-sys_head_%u"), wFaceID);
	COptionUI* option = static_cast<COptionUI*>(m_PaintManager.FindControl(pzBuffer));
	if (option)
	{
		option->Selected(true);
	}
}
void InfoFrame::ReqGetInfo()
{
	tagLS_UpdateUserInfo tUserInfo;
	memset(&tUserInfo, 0, sizeof(tagLS_UpdateUserInfo));

	// 账号
	::StrToCharArray(tUserInfo.szAccount, m_pSelfData->GetAccount());

	// 密码
	CMD5Encrypt::EncryptData(m_pSelfData->GetPassword(), tUserInfo.szPwd);
	
	ShowWaittingText(true, _T("正在获取数据,请稍后..."));
	
	g_Logger->TryInfo(_T("InfoFrame - Userinfo Get Req"));
	DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_LS_USERINFO_GET, &tUserInfo, sizeof(tagLS_UpdateUserInfo));
	if (FALSE == bRes)
	{
		ShowWaittingText(false);
		m_iCurOperateStatus = InfoOperateStatus::INFO_OS_GET_INFO;
		DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("获取个人信息数据失败!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void InfoFrame::UpdateInfo(tagLS_UpdateUserInfo* pUserInfo)
{
	if (!pUserInfo) return;
	
	// 更新玩家数据
	do 
	{
		// 更新温馨提示面板
		do
		{
			// 账号
			m_pSelfData->SetAccount(pUserInfo->szAccount);
			m_pSwitch1_EdtAccount->SetText(pUserInfo->szAccount);

			// 昵称
			m_pSelfData->SetNickName(pUserInfo->szNickname);
			m_pSwitch1_EdtNickName->SetText(pUserInfo->szNickname);

			// 头像ID
			m_pSelfData->SetFaceID(pUserInfo->wFaceID);
			do 
			{
				// 选中当前头像
				TCHAR pzBuffer[MAX_PATH] = { 0 };
				WORD wFaceID = m_tTempUnserInfo.wFaceID;
				_stprintf_s(pzBuffer, _T("info_switch_2-sys_head_%u"), wFaceID);
				COptionUI* option = static_cast<COptionUI*>(m_PaintManager.FindControl(pzBuffer));
				if (option)
				{
					option->Selected(true);
				}
			} while (0);

			// 真实姓名
			m_pSwitch1_EdtRealName->SetText(pUserInfo->szRealname);

			// 性别
			m_pSelfData->SetGender(pUserInfo->cbGender);
			m_pSwitch1_EdtGender->SetText(m_pSelfData->GetGenderStr());
			m_pSwitch1_ComboGender->SelectItem(pUserInfo->cbGender, true);

			// 电话号码
			CDuiString pzTel = pUserInfo->szTelphone;
			if (g_GameUtil->MatchWordByType(pzTel, MatchWords_Type::REG_TYPE_TELPHONE))
			{
				_stprintf_s(m_pzBuffer, _T("%s****%s"), pzTel.Left(3).GetData(), pzTel.Right(3).GetData());
				m_pSwitch1_EdtTelphone->SetText(pzTel.GetData());
				m_pSwitch3_LabOldTel->SetText(m_pzBuffer);
				if (m_llVertcodeSec <= 0)
				{
					m_pSwitch3_BtnGetVCode->SetEnabled(true);
				}
			}

			// 邮箱
			m_pSwitch1_EdtEmail->SetText(pUserInfo->szEmail);

			// 身份证
			m_pSwitch1_EdtIDCard->SetText(pUserInfo->szIDCard);

			// QQ号码
			m_pSwitch1_EdtQQNumber->SetText(pUserInfo->szQQNumber);

			// 个人说明
			m_pSelfData->SetUnderWrite(pUserInfo->szUnderWrite);
			m_pSwitch1_EdtUnderWrite->SetText(pUserInfo->szUnderWrite);
			this->ShowLeftWords();

		} while (0);

		// 更新大厅主面板玩家UI
		do
		{
			m_pSelfData->UpdateUI(m_pParentManager);
		} while (0);

		// 更新房间面板玩家UI
		do
		{
			Tls_Panel iCurPanel = g_GameUtil->GetCurPanel();
			if (iCurPanel == Tls_Panel::TLS_PANEL_ROOM_LIST || iCurPanel == Tls_Panel::TLS_PANEL_ROOM)
			{
				UserList* pListUI = UserList::GetInstance();
				pListUI->Update();
			}
		} while (0);
	} while (0);
}

void InfoFrame::ModifyUserInfo()
{
	bool bRes = false;

	// 检测"昵称"
	do
	{
		if (m_pSwitch1_EdtNickName)
		{
			CDuiString& pzBuff = m_pSwitch1_EdtNickName->GetText();
			if (pzBuff.GetLength() > 0)
			{
				if (g_GameUtil->CheckSensitiveVocabulary(pzBuff))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("不能包含敏感字符!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_NiCK_NAME)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
				else if (!g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_NICKNAME))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("昵称格式填写不正确!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_NiCK_NAME)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
			else
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("昵称不能为空!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_NiCK_NAME)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);

				return;
			}
		}
	} while (0);

	// 检测"真实姓名"
	do
	{
		if (m_pSwitch1_EdtRealName && !m_pSwitch1_EdtRealName->IsReadOnly())
		{
			CDuiString& pzBuff = m_pSwitch1_EdtRealName->GetText();
			if (pzBuff.GetLength() > 0)
			{
				if (g_GameUtil->CheckSensitiveVocabulary(pzBuff))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("不能包含敏感字符!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_REAL_NAME)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
				else if (!g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_REALNAME))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("真实姓名格式填写不正确!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_REAL_NAME)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
		}
	} while (0);

	// 检测"电话号码"
	do
	{
		if (m_pSwitch1_EdtTelphone && !m_pSwitch1_EdtTelphone->IsReadOnly())
		{
			CDuiString& pzBuff = m_pSwitch1_EdtTelphone->GetText();
			if (pzBuff.GetLength() > 0)
			{
				bRes = g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_TELPHONE);
				if (!bRes)
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("电话号码格式填写不正确!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_TEL)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
		}
	} while (0);

	// 检测"邮箱"
	do
	{
		if (m_pSwitch1_EdtEmail && !m_pSwitch1_EdtEmail->IsReadOnly())
		{
			CDuiString& pzBuff = m_pSwitch1_EdtEmail->GetText();
			if (pzBuff.GetLength() > 0)
			{
				if (g_GameUtil->CheckSensitiveVocabulary(pzBuff))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("不能包含敏感字符!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_EMAIL)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
				else if (!g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_EMAIL))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("邮箱格式填写不正确!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_EMAIL)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
		}
	} while (0);

	// 检测"身份证"
	do
	{
		if (m_pSwitch1_EdtIDCard && !m_pSwitch1_EdtIDCard->IsReadOnly())
		{
			CDuiString& pzBuff = m_pSwitch1_EdtIDCard->GetText();
			if (pzBuff.GetLength() > 0)
			{
				bRes = g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_IDCARD);
				if (!bRes)
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("身份证格式填写不正确!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_IDCARD)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
		}
	} while (0);

	// 检测"QQ号码"
	do
	{
		if (m_pSwitch1_EdtQQNumber && !m_pSwitch1_EdtQQNumber->IsReadOnly())
		{
			CDuiString& pzBuff = m_pSwitch1_EdtQQNumber->GetText();
			if (pzBuff.GetLength() > 0)
			{
				bRes = g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_QQ_NUMBER);
				if (!bRes)
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("QQ格式填写不正确!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_QQ)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
		}
	} while (0);

	// 检测个性签名
	do 
	{
		if (m_pSwitch1_EdtUnderWrite)
		{
			CDuiString& pzBuff = m_pSwitch1_EdtUnderWrite->GetText();
			if (g_GameUtil->CheckSensitiveVocabulary(pzBuff))
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("不能包含敏感字符!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH1_UNDERWRITE)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);

				return;
			}
		}
	} while (0);

	// 开始请求修改
	do
	{
		// 清空缓存信息
		memset(&m_tTempUnserInfo, 0, sizeof(tagLS_UpdateUserInfo));

		// 头像
		m_tTempUnserInfo.wFaceID = m_pSelfData->GetFaceID();

		// 性别
		m_tTempUnserInfo.cbGender = m_pSwitch1_ComboGender->GetCurSel();

		// 账号
		::StrToCharArray(m_tTempUnserInfo.szAccount, m_pSelfData->GetAccount());

		// 密码
		CMD5Encrypt::EncryptData(m_pSelfData->GetPassword(), m_tTempUnserInfo.szPwd);
		
		// 昵称
		::StrToCharArray(m_tTempUnserInfo.szNickname, m_pSwitch1_EdtNickName->GetText());

		// 真实姓名
		::StrToCharArray(m_tTempUnserInfo.szRealname, m_pSwitch1_EdtRealName->GetText());

		// 电话号码
		::StrToCharArray(m_tTempUnserInfo.szTelphone, m_pSwitch1_EdtTelphone->GetText());

		// 邮箱
		::StrToCharArray(m_tTempUnserInfo.szEmail, m_pSwitch1_EdtEmail->GetText());

		// 身份证
		::StrToCharArray(m_tTempUnserInfo.szIDCard, m_pSwitch1_EdtIDCard->GetText());

		// QQ
		::StrToCharArray(m_tTempUnserInfo.szQQNumber, m_pSwitch1_EdtQQNumber->GetText());

		// 个人签名
		::StrToCharArray(m_tTempUnserInfo.szUnderWrite, m_pSwitch1_EdtUnderWrite->GetText());

		// 发送
		g_Logger->TryInfo(_T("InfoFrame - Userinfo Update Req"));
		ShowWaittingText(true, _T("正在生效中,请稍后..."));
		DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_LS_USERINFO_UPDATE, &m_tTempUnserInfo, sizeof(tagLS_UpdateUserInfo));
		if (FALSE == bRes)
		{
			ShowWaittingText(false);
			m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_INFO;
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, _T("操作失败，请稍后再试!")
				, DLG_ID::DLG_ID_NONE
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	} while (0);
}

void InfoFrame::ModifyHeadPic()
{
	CTabLayoutUI* pHeadPicSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsInfoSwitch2_Switch));
	if (!pHeadPicSwitch) return;

	int iCurSel = pHeadPicSwitch->GetCurSel();
	// 系统头像
	if (0 == iCurSel)
	{
		// 发送
		g_Logger->TryInfo(_T("InfoFrame - Userinfo Modify Pic Req"));
		ShowWaittingText(true, _T("正在生效中,请稍后..."));
		DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_LS_USERINFO_UPDATE, &m_tTempUnserInfo, sizeof(tagLS_UpdateUserInfo));
		if (FALSE == bRes)
		{
			ShowWaittingText(false);
			m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_HEAD_PIC;
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::ReConnect, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, _T("操作失败，请稍后再试!")
				, DLG_ID::DLG_ID_NONE
				, DLG_STYLE::DLG_RETRY_CANCEL
				, dlg_cb);
		}
	}

	// 自定义头像
	else if (1 == iCurSel)
	{
	}
}

void InfoFrame::SetVertcodeTimer()
{
	m_pSwitch3_BtnGetVCode->SetEnabled(false);

	TimerCallBack timepro = CC_CALLBACK_2(InfoFrame::OnVertcodeTimer, this);
	g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_GET_VERTCODE, 1000);
	m_llVertcodeSec = TIME_GET_VERTCODE;

	//开启定时器时即可设置Text显示
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	_stprintf_s(pzBuffer, _T("%lds获取"), m_llVertcodeSec);
	m_pSwitch3_BtnGetVCode->SetText(pzBuffer);
	m_pSwitch3_BtnGetVCode->SetUserData(m_pSwitch3_BtnGetVCode->GetForeImage());
	m_pSwitch3_BtnGetVCode->SetForeImage(_T(""));
}

void InfoFrame::OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime)
{
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	if (--m_llVertcodeSec <= 0)
	{
		g_GameUtil->KillTimer(m_hWnd, TIMER_ID_GET_VERTCODE);
		m_llVertcodeSec = TIME_GET_VERTCODE;

		m_pSwitch3_BtnGetVCode->SetText(m_pzSwitch3_BtnGetVCodeText);
		m_pSwitch3_BtnGetVCode->SetEnabled(true);
	}
	else
	{
		_stprintf_s(pzBuffer, _T("%lds获取"), m_llVertcodeSec);
		m_pSwitch3_BtnGetVCode->SetText(pzBuffer);
	}
}

void InfoFrame::GetServerSMS()
{
	//从服务端获取短信验证码
	tag_C_GetSMS stuSMS;
	memset(&stuSMS, 0, sizeof(tag_C_GetSMS));

	// 操作类型
	stuSMS.btType = 3;

	// 账号
	::StrToCharArray(stuSMS.szAccount, m_pSelfData->GetAccount());

	// 手机号
	::StrToCharArray(stuSMS.szPhone, m_pSwitch1_EdtTelphone->GetText());

	//发送
	g_Logger->TryInfo(_T("InfoFrame - Userinfo Get SMS Req"));
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_SMS, SUB_C2LS_GETSMS, &stuSMS, sizeof(tag_C_GetSMS));
	if (FALSE == iRes)
	{
		m_iCurOperateStatus = InfoOperateStatus::INFO_OS_GET_VCODE;
		DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示")
			, _T("获取验证码失败，请检查您的网络!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void InfoFrame::ModifyTelphone()
{
	// 检测输入是否有误
	do
	{
		// 验证码
		do
		{
			CDuiString& pzBuff = m_pSwitch3_EdtVCode->GetText();
			if (pzBuff.GetLength() > 0)
			{
				if (!g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_VERTCODE))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("验证码输入有误!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_VCODE)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
			else
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("验证码不能为空!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_VCODE)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);

				return;
			}
		} while (0);

		// 新手机号
		do
		{
			CDuiString& pzBuff = m_pSwitch3_EdtNewTel->GetText();
			if (pzBuff.GetLength() > 0)
			{
				if (!g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_TELPHONE))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("新手机号输入有误!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_NEW_TEL)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
			else
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("新手机号不能为空!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_NEW_TEL)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);

				return;
			}
		} while (0);
	} while (0);

	tagLS_ModTelphone tModTel;
	memset(&tModTel, 0, sizeof(tagLS_ModTelphone));

	// 账号
	::StrToCharArray(tModTel.szAccount, m_pSelfData->GetAccount());

	// 密码
	CMD5Encrypt::EncryptData(m_pSelfData->GetPassword(), tModTel.szPwd);
	
	// 短信验证码
	::StrToCharArray(tModTel.szSMS, m_pSwitch3_EdtVCode->GetText());

	// 旧手机号
	::StrToCharArray(tModTel.szOldTel, m_tTempUnserInfo.szTelphone);

	// 新手机号
	::StrToCharArray(tModTel.szNewTel, m_pSwitch3_EdtNewTel->GetText());

	// 发送
	g_Logger->TryInfo(_T("InfoFrame - Userinfo Modify Tel Req"));
	ShowWaittingText(true, _T("正在生效中,请稍后..."));
	DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_LS_USERINFO_MODTEL, &tModTel, sizeof(tagLS_ModTelphone));
	if (FALSE == bRes)
	{
		ShowWaittingText(false);
		m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_TEL;
		DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("绑定失败，请稍后再试!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void InfoFrame::ModifyPassword()
{
	// 检测输入是否有误
	do
	{
		// 原密码
		do
		{
			CDuiString& pzBuff = m_pSwitch3_EdtOldPw->GetText();
			if (pzBuff.GetLength() > 0)
			{
				if (!g_GameUtil->MatchWordByType(pzBuff, MatchWords_Type::REG_TYPE_PASSWORD))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("原密码格式有误!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_OLD_PW)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
			else
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("原密码不能为空!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_OLD_PW)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);

				return;
			}
		} while (0);

		// 新密码\确认密码
		do
		{
			// 新密码
			CDuiString& pzNewPw = m_pSwitch3_EdtNewPw->GetText();
			if (pzNewPw.GetLength() > 0)
			{
				if (!g_GameUtil->MatchWordByType(pzNewPw, MatchWords_Type::REG_TYPE_PASSWORD))
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, _T("新密码输入有误!")
						, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_NEW_PW)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);

					return;
				}
			}
			else
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("新密码不能为空!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_NEW_PW)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);

				return;
			}

			// 确认密码
			CDuiString& pzCfmPw = m_pSwitch3_EdtCfmPw->GetText();
			if (pzCfmPw != pzNewPw)
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(&m_PaintManager
					, _T("温馨提示")
					, _T("两次输入不一致!")
					, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH3_CFM_PW)
					, DLG_STYLE::DLG_DEFAULT
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
		} while (0);
	} while (0);

	tagLS_ModPassword tModPw;
	memset(&tModPw, 0, sizeof(tagLS_ModPassword));

	// 账号
	::StrToCharArray(tModPw.szAccount, m_pSelfData->GetAccount());

	// 原密码
	TCHAR pzBuff[PWD_LEN] = { 0 };
	CMD5Encrypt::EncryptData(m_pSwitch3_EdtOldPw->GetText(), pzBuff);
	::StrToCharArray(tModPw.szOldPwd, pzBuff);

	// 新密码
	CMD5Encrypt::EncryptData(m_pSwitch3_EdtNewPw->GetText(), pzBuff);
	::StrToCharArray(tModPw.szNewPwd, pzBuff);

	// 发送
	g_Logger->TryInfo(_T("InfoFrame - Userinfo Modify PW Req"));
	ShowWaittingText(true, _T("正在生效中,请稍后..."));
	DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_LS_USERINFO_MODPW, &tModPw, sizeof(tagLS_ModPassword));
	if (FALSE == bRes)
	{
		ShowWaittingText(false);
		m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_PW;
		DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("修改密码失败，请稍后再试!")
			, DLG_ID::DLG_ID_NONE
			, DLG_STYLE::DLG_RETRY_CANCEL
			, dlg_cb);
	}
}

void InfoFrame::FrozenModify()
{
	// 以下控件第一次修改后，以后都禁止修改，为只读
	
	// 账号
	if (_tcscmp(m_tTempUnserInfo.szAccount, _T("")) != 0)
	{
		m_pSwitch1_EdtAccount->SetReadOnly(true);
		m_pSwitch1_EdtAccount->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtAccount->SetTabEnable(false);
		m_pSwitch1_EdtAccount->SetTextColor(0xFF696969);
	}
	// 昵称
	if (_tcscmp(m_tTempUnserInfo.szNickname, _T("")) != 0)
	{
		m_pSwitch1_EdtNickName->SetReadOnly(true);
		m_pSwitch1_EdtNickName->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtNickName->SetTabEnable(false);
		m_pSwitch1_EdtNickName->SetTextColor(0xFF696969);
	}
	// 真实姓名
	if (_tcscmp(m_tTempUnserInfo.szRealname, _T("")) != 0)
	{
		m_pSwitch1_EdtRealName->SetReadOnly(true);
		m_pSwitch1_EdtRealName->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtRealName->SetTabEnable(false);
		m_pSwitch1_EdtRealName->SetTextColor(0xFF696969);
	}
	// 性别
	if (true)
	{
		m_pSwitch1_ComboGender->SetEnabled(false);
		m_pSwitch1_ComboGender->SetTabEnable(false);
		m_pSwitch1_EdtGender->SetTextColor(0xFF696969);
	}
	// 电话号码
	if (_tcscmp(m_tTempUnserInfo.szTelphone, _T("")) != 0)
	{
		m_pSwitch1_EdtTelphone->SetReadOnly(true);
		m_pSwitch1_EdtTelphone->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtTelphone->SetTabEnable(false);
		m_pSwitch1_EdtTelphone->SetTextColor(0xFF696969);
	}
	// 邮箱
	if (_tcscmp(m_tTempUnserInfo.szEmail, _T("")) != 0)
	{
		m_pSwitch1_EdtEmail->SetReadOnly(true);
		m_pSwitch1_EdtEmail->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtEmail->SetTabEnable(false);
		m_pSwitch1_EdtEmail->SetTextColor(0xFF696969);
	}
	// 身份证
	if (_tcscmp(m_tTempUnserInfo.szIDCard, _T("")) != 0)
	{
		m_pSwitch1_EdtIDCard->SetReadOnly(true);
		m_pSwitch1_EdtIDCard->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtIDCard->SetTabEnable(false);
		m_pSwitch1_EdtIDCard->SetTextColor(0xFF696969);
	}
	// QQ
	if (_tcscmp(m_tTempUnserInfo.szQQNumber, _T("")) != 0)
	{
		m_pSwitch1_EdtQQNumber->SetReadOnly(true);
		m_pSwitch1_EdtQQNumber->SetReadOnlyShowCaret(false);
		m_pSwitch1_EdtQQNumber->SetTabEnable(false);
		m_pSwitch1_EdtQQNumber->SetTextColor(0xFF696969);
	}
}

void InfoFrame::ModifyGameSetting()
{
	if (!m_pSelfData)
	{
		DataManager* pDataManager = DataManager::GetInstance();
		m_pSelfData = &pDataManager->GetSelfData();
	}

	CMD_GR_UserRule tGameRule;
	memset(&tGameRule, 0, sizeof(CMD_GR_UserRule));

	tGameRule.bLimitWin		= m_pSwitch4_OptWin->IsSelected();
	tGameRule.bLimitFlee	= m_pSwitch4_OptFlee->IsSelected();;
	tGameRule.bPassword		= m_pSwitch4_OptPassword->IsSelected();;
	tGameRule.bLimitScore	= m_pSwitch4_OptScore->IsSelected();;
	tGameRule.bCheckSameIP	= m_pSwitch4_OptSameIP->IsSelected();;

	tGameRule.wWinRate		= _ttoi(m_pSwitch4_EdtWin->GetText().GetData());
	tGameRule.wFleeRate		= _ttoi(m_pSwitch4_EdtFlee->GetText().GetData());
	tGameRule.lLessScore	= _ttoll(m_pSwitch4_EdtScoreMin->GetText().GetData());
	tGameRule.lMaxScore		= _ttoll(m_pSwitch4_EdtScoreMax->GetText().GetData());
	
	::StrToCharArray(tGameRule.szPassword, m_pSwitch4_EdtPassword->GetText().GetData());

	// 保存游戏设置
	// 检测桌子密码
	if (tGameRule.bPassword)
	{
		CDuiString pzPassword = tGameRule.szPassword;
		if (pzPassword.GetLength() <= 0)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("密码输入不能为空!")
				, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH4_PASSWORD)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
			return;
		}
		else if (pzPassword.GetLength() > 0 && !g_GameUtil->MatchRegular(pzPassword, _T("[\\S]+")))
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("密码格式填写错误!")
				, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH4_PASSWORD)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
			return;
		}
	}

	// 检测限制分数
	if (tGameRule.bLimitScore)
	{
		if (m_pSwitch4_EdtScoreMin->GetText().GetLength() <= 0)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("限制最低积分不能为空!")
				, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH4_LIMIT_SCORE_MIN)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
			return;
		}
		else if (m_pSwitch4_EdtScoreMax->GetText().GetLength() <= 0)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("限制最高积分不能为空!")
				, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH4_LIMIT_SCORE_MAX)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
			return;
		}
		else if (tGameRule.lLessScore > tGameRule.lMaxScore)
		{
			DialogCallBack dlg_cb = CC_CALLBACK_2(InfoFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("最低积分不能大于最高积分!")
				, DLG_ID(MSGBOX_EVENT_ID::MB_ID_SWITCH4_LIMIT_SCORE_MIN)
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb);
			return;
		}
	}

	// 本地保存
	m_pSelfData->SetGameSetting(tGameRule);
	m_pSelfData->SaveGameSetting();

	// 请求设置生效
	if (g_GameUtil->GetCurPanel() == Tls_Panel::TLS_PANEL_ROOM)
	{
		g_Logger->TryInfo(_T("InfoFrame - Modify GameSetting Req"));
		RoomManager* pRoomManager = RoomManager::GetInstance();
		DWORD bRes = pRoomManager->SendData(MDM_GR_USER, SUB_GR_USER_RULE, &tGameRule, sizeof(CMD_GR_UserRule));
		if (bRes == TRUE)
		{
			ShowToastText(_T("设置成功!"));
		}
		else
		{
			ShowToastText(_T("设置失败，请检查您的网络!"));
		}
	}
	else
	{
		ShowToastText(_T("设置成功!"));
	}
}

void InfoFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		this->InitData(msg);
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (_tcscmp(sName, R->kBtnInfo_Close) == 0)
		{
			Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch1_Cancel) == 0)
		{
			Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch2_Cancel) == 0)
		{
			Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch4_Cancel) == 0)
		{
			Close(CMW_CLOSE_SURE);
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch1_Ensure) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->ModifyUserInfo();
			}
			else
			{
				m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_INFO;
				m_pLoginServer->Connect();
			}
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch2_Ensure) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->ModifyHeadPic();
			}
			else
			{
				m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_INFO;
				m_pLoginServer->Connect();
			}
		}
		else if (_tcscmp(sName, R->kOptInfo_HeadOpt) == 0)
		{
			CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(m_PaintManager.FindControl(R->kOptInfo_Op_2));
			if (pNode)
			{
				pNode->Select(true);
				CCheckBoxUI* pFolderBtn = pNode->GetFolderButton();
				if (pFolderBtn)
				{
					pNode->GetFolderButton()->Selected(!pFolderBtn->IsSelected());
				}
			}
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch2_ChoosePic) == 0)
		{
			bool isRes = false;
			std::vector<CDuiString> names;
			isRes = g_GameUtil->openFileDialog(_T("图像文件(*.bmp;*.png;*.jpg)\0*.BMP;*.PNG;*.JPG\0\0"), m_hWnd, names, false);
			if (isRes)
			{
				CScreenshotBoxUI* head_shot_box = static_cast<CScreenshotBoxUI*>(m_PaintManager.FindControl(R->kBoxInfoSwitch2_HeadBox));
				head_shot_box->SetPicture(names[0]);
				head_shot_box->SetVisible(true);

				//创建缩略图位图
				CVerticalLayoutUI* thumbnail_01 = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(R->kLabInfoSwitch2_Thumbnail_01));
				head_shot_box->CreateThumbnailBitmap(R->kLabInfoSwitch2_Thumbnail_01, R->kLabInfoSwitch2_Thumbnail_01);

				CVerticalLayoutUI* thumbnail_02 = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(R->kLabInfoSwitch2_Thumbnail_02));
				head_shot_box->CreateThumbnailBitmap(R->kLabInfoSwitch2_Thumbnail_02, R->kLabInfoSwitch2_Thumbnail_02);
			}
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch3_GetVCode) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->GetServerSMS();
			}
			else
			{
				m_iCurOperateStatus = InfoOperateStatus::INFO_OS_GET_VCODE;
				m_pLoginServer->Connect();
			}
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch3_Bind) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->ModifyTelphone();
			}
			else
			{
				m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_INFO;
				m_pLoginServer->Connect();
			}
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch3_Ensure) == 0)
		{
			if (m_pLoginServer->IsConnect())
			{
				this->ModifyPassword();
			}
			else
			{
				m_iCurOperateStatus = InfoOperateStatus::INFO_OS_MODIFY_INFO;
				m_pLoginServer->Connect();
			}
		}
		else if (_tcscmp(sName, R->kBtnInfoSwitch4_Ensure) == 0)
		{
			this->ModifyGameSetting();
		}
	} 
	else if (_tcscmp(sType, DUI_MSGTYPE_SELECTCHANGED) == 0)
	{
		switch (m_dwSwitchIndex)
		{
			// 个人资料
			case 0:
				break;

			// 头像选择
			case 1:
			{
				// 保存当前选中的系统头像索引
				CTileLayoutUI* panel = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(R->kTilInfoSwitch2_SysHeadPanel));
				if (panel)
				{
					if (-1 != panel->GetItemIndex(msg.pSender))
					{
						CDuiString pzTemp = msg.pSender->GetUserData();
						m_tTempUnserInfo.wFaceID = _tstol(pzTemp.GetData());
					}
				}
			}break;

			// 安全设置
			case 2:
				break;

			// 游戏设置
			case 3:
				break;

			default:
				break;
		}

	}
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
		CControlUI* pControl = msg.pSender;
		
		if (_tcscmp(sName, R->kCmbInfoSwitch_1_Gender) == 0)
			m_pSwitch1_EdtGender->SetText(pControl->GetText());
		else if (_tcscmp(sName, R->KTewInfo_View) == 0)
		{
			CTreeViewUI* treeView = static_cast<CTreeViewUI*>(msg.pSender);
			int iCurSel = treeView->GetCurSel();
			CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(treeView->GetItemAt(iCurSel));
			sName = pNode->GetName();
			CTabLayoutUI* pHeadPicSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTlsInfoSwitch2_Switch));
			if (_tcscmp(sName, R->kOptInfo_Op_1) == 0)
			{
				m_dwSwitchIndex = 0;
				m_pInfoSwitch->SelectItem(m_dwSwitchIndex);
			}
			else if (_tcscmp(sName, R->kOptInfo_Op_2) == 0)
			{
				m_dwSwitchIndex = 1;
				m_pInfoSwitch->SelectItem(m_dwSwitchIndex);

				pHeadPicSwitch->SelectItem(0);
				this->LayoutHeadPanel();
			}
			else if (_tcscmp(sName, R->kOptInfo_HeadOp_1) == 0)
			{
				m_dwSwitchIndex = 1;
				m_pInfoSwitch->SelectItem(m_dwSwitchIndex);

				pHeadPicSwitch->SelectItem(0);
				this->LayoutHeadPanel();
			}
			else if (_tcscmp(sName, R->kOptInfo_HeadOp_2) == 0)
			{
				m_dwSwitchIndex = 1;
				m_pInfoSwitch->SelectItem(m_dwSwitchIndex);

				pHeadPicSwitch->SelectItem(1);
			}
			else if (_tcscmp(sName, R->kOptInfo_Op_3) == 0)
			{
				m_dwSwitchIndex = 2;
				m_pInfoSwitch->SelectItem(m_dwSwitchIndex);
			}
			else if (_tcscmp(sName, R->kOptInfo_Op_4) == 0)
			{
				m_dwSwitchIndex = 3;
				m_pInfoSwitch->SelectItem(m_dwSwitchIndex);
			}
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_TEXTCHANGED) == 0)
	{
		if (_tcscmp(sName, R->kEdtInfoSwitch1_NickName) == 0)
		{
			CDuiString pzNickName = m_pSwitch1_EdtNickName->GetText();
			if (pzNickName.GetLength() > NAME_LEN - 2)
			{
				int iBufArryLen = ArrayCount(m_pzBuffer);
				memset(m_pzBuffer, 0, iBufArryLen * sizeof(TCHAR));

				int iMinLen = min(pzNickName.GetLength() + 1, UNDER_WRITE_LEN - 2);
				iMinLen = min(iMinLen, iBufArryLen);

				memcpy(m_pzBuffer, pzNickName.GetData(), iMinLen * sizeof(TCHAR));
				m_pSwitch1_EdtNickName->SetText(m_pzBuffer);
			}
		}
		else if (_tcscmp(sName, R->kEdtInfoSwitch1_UnderWrite) == 0)
		{
			CDuiString pzUnderWrite = m_pSwitch1_EdtUnderWrite->GetText();
			if (pzUnderWrite.GetLength() > UNDER_WRITE_LEN - 2)
			{
				int iBufArryLen = ArrayCount(m_pzBuffer);
				memset(m_pzBuffer, 0, iBufArryLen * sizeof(TCHAR));

				int iMinLen = min(pzUnderWrite.GetLength() + 1, UNDER_WRITE_LEN - 2);
				iMinLen = min(iMinLen, iBufArryLen);

				memcpy(m_pzBuffer, pzUnderWrite.GetData(), iMinLen * sizeof(TCHAR));
				m_pSwitch1_EdtUnderWrite->SetText(m_pzBuffer);
			}

			this->ShowLeftWords();
		}
	}
}

LRESULT InfoFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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
	return FALSE;
}

LRESULT InfoFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_CHAR:
		case WM_IME_CHAR:
		{
			// 手机号
			// 身份证号
			// QQ号
			// 都只能输入数字(且第一位不能是0)
			if (m_pSwitch1_EdtTelphone && m_pSwitch1_EdtTelphone->IsFocused()
				|| m_pSwitch1_EdtIDCard && m_pSwitch1_EdtIDCard->IsFocused()
				|| m_pSwitch1_EdtQQNumber && m_pSwitch1_EdtQQNumber->IsFocused()
				|| m_pSwitch3_EdtVCode && m_pSwitch3_EdtVCode->IsFocused()
				|| m_pSwitch3_EdtNewTel && m_pSwitch3_EdtNewTel->IsFocused()
				|| m_pSwitch4_EdtWin && m_pSwitch4_EdtWin->IsFocused()
				|| m_pSwitch4_EdtFlee && m_pSwitch4_EdtFlee->IsFocused()
				|| m_pSwitch4_EdtScoreMin && m_pSwitch4_EdtScoreMin->IsFocused()
				|| m_pSwitch4_EdtScoreMax && m_pSwitch4_EdtScoreMax->IsFocused())
			{
				if (wParam < 48 || wParam > 57)// || (m_pSwitch1_EdtTelphone->GetText().GetLength() == 0 && wParam == 48))
				{
					bHandled = TRUE;
					return TRUE;
				}
			}
		}break;
	}


	bHandled = FALSE;
	return 0;
}