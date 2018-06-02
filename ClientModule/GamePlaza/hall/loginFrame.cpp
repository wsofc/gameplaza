#include "stdafx.h"
#include "loginFrame.h"
#include "registFrame.h"
#include "passwordFrame.h"
#include "lockFrame.h"
#include "hallFrame.h"
#include "..\\room\\unrealControl.h"

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib,"iphlpapi.lib")

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

LoginFrame* LoginFrame::g_instance = nullptr;

LoginFrame::LoginFrame()
	: m_bAllowConnect(false)
	, m_bRefreshAcconut(false)
	, m_pConfigFile(nullptr)
	, m_pEdtAccount(nullptr)
	, m_pEdtPassword(nullptr)
	, m_pOptCheckPW(nullptr)
	, m_pComboAccount(nullptr)
	, m_pWkeCtl(nullptr)
	, m_pBtnLogin(nullptr)
	, m_pLoadingFrame(nullptr)
	, m_pLoginServer(nullptr)
	, m_pWaitDlgManager(nullptr)
	, m_dwWatingEventID(INVALID_DWORD)
	, m_iCurOperateStatus(OperateStatus::OS_NONE)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

LoginFrame::~LoginFrame()
{
	// 删除配置文件指针
	SAFE_DELETE(m_pConfigFile);

	// 删除账号候选数组
	this->_ClearAccount();

	g_Logger->TryInfo(_T("LoginFrame - Destroy"));
}

LoginFrame* LoginFrame::GetInstance()
{
	if (LoginFrame::g_instance == nullptr)
		LoginFrame::g_instance = new (std::nothrow)LoginFrame();

	return LoginFrame::g_instance;
}

BOOL LoginFrame::CreateFrame()
{
	if (FALSE == IsWindow(m_hWnd))
	{
		int style = UI_CLASSSTYLE_DIALOG | WS_MINIMIZEBOX;
		m_hWnd = this->Create(NULL, R->MainWndTitleName, style, 0L);

		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);

		// 服务器对象绑定窗口
		if (!m_pLoginServer)m_pLoginServer = LoginServer::GetInstance();
		m_pLoginServer->SetDelegate(this, &m_PaintManager);
	}

	return TRUE;
}

LRESULT LoginFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (CMW_CLOSE_SURE == wParam)
	{
		return WindowImplBase::OnClose(uMsg, wParam, lParam, bHandled);
	}
	else
	{
		g_GameUtil->EndHall();
	}

	return FALSE;
}

void LoginFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("LoginFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	if (m_pLoadingFrame)
	{
		m_pLoadingFrame->Close(CMW_CLOSE_SURE);	// 关闭加载界面
	}
	
	SAFE_DELETE(g_instance);
}

LRESULT LoginFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_CHAR:
		case WM_IME_CHAR:
		{
			if (m_pEdtAccount->IsFocused())
			{
				bool boolHandled = bHandled;
				m_pEdtAccount->MessageHandler(uMsg, wParam, lParam, boolHandled);
				this->_MatchAccount();
				m_PaintManager.SetFocusNeeded(m_pEdtAccount);

				bHandled = TRUE;
				return TRUE;
			}
		}break;

		case WM_KEYDOWN:
		{
			if (wParam == VK_DELETE || wParam == VK_BACK)
			{
				if (m_pEdtAccount->IsFocused())
				{
					bool boolHandled = bHandled;
					m_pEdtAccount->MessageHandler(uMsg, wParam, lParam, boolHandled);
					this->_MatchAccount();
					m_PaintManager.SetFocusNeeded(m_pEdtAccount);
					
					bHandled = TRUE;
					return TRUE;
				}
			}
		}break;

		default:
			break;
	}
	
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

CDuiString LoginFrame::GetSkinFile()
{
	return _T("login.xml");
}

CDuiString LoginFrame::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE LoginFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR LoginFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CControlUI* LoginFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	{
		return new CWkeWebkitUI();
	}

	return NULL;
}

void LoginFrame::InitWindow()
{
	g_Logger->TryInfo(_T("LoginFrame - Open"));

	m_pLabWaitting	= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabLogin_Watting));
	m_pLabVersion	= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabLogin_Version));
	m_pEdtAccount	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtLogin_Account));
	m_pEdtPassword	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtLogin_Passaword));
	m_pOptCheckPW	= static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptLogin_Check));
	m_pBtnLogin		= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnLogin_Login));
	m_pComboAccount = static_cast<CComboUI*>(m_PaintManager.FindControl(R->kCmbLogin_Account));

	// init status
	m_PaintManager.SetFocusNeeded(m_pEdtAccount);
	m_pBtnLogin->SetEnabled(true);
	m_pLabWaitting->SetVisible(false);
}

void LoginFrame::InitData(TNotifyUI& msg)
{
	// 设置窗口图标
	this->SetIcon(IDI_GAMEPLAZA_ICON);
	
	// 设置版本号UI显示
	do
	{
		if (m_pLabVersion)
		{
			CDuiString str_version;
			//str_version.Format(_T("%02d%02d"), VER_PLAZA_HIGH, VER_PLAZA_LOW);
			str_version.Format(_T("%d%d%d%d")
				, GAMEPLAZA_VERSION_1(VER_PLAZA_HIGH)
				, GAMEPLAZA_VERSION_2(VER_PLAZA_HIGH)
				, GAMEPLAZA_VERSION_3(VER_PLAZA_LOW)
				, GAMEPLAZA_VERSION_4(VER_PLAZA_LOW));
			str_version.Format(_T("版本号: v%c.%c.%c.%c"), str_version.GetAt(0), str_version.GetAt(1), str_version.GetAt(2), str_version.GetAt(3));
			m_pLabVersion->SetText(str_version.GetData());
		}
	} while (0);

	// 清除本地临时下载的缓存文件
	do 
	{
		TCHAR pzBuff[MAX_PATH] = { 0 };
		::PathCanonicalize(pzBuff, m_PaintManager.GetInstancePath().GetData());
		_stprintf_s(pzBuff, _T("%s%s"), pzBuff, _T("Update\\Temp"));
		if (TRUE == ::PathFileExists(pzBuff))
		{
			g_GameUtil->DeleteDirectory(pzBuff);
		}
	} while (0);
	
	// 读取换肤风格配置支持
	this->_ReadStyleFont();
	
	// 加载本地账号配置
	this->_ReadAccountFile();

	// 加载本地网页配置
	this->_ReadWebConfig();

	// 加载本地Unreal配置
	this->_ReadUnrealConfig();

	//设置登录界面网页
	do
	{
		m_pWkeCtl = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(R->kWkeLogin_BgWeb));
		if (m_pWkeCtl)
		{
			LPCTSTR lpUrl = g_GameUtil->GetWebByName(_T("AdWeb_Login"));
			m_pWkeCtl->Navigate(lpUrl);
			m_pWkeCtl->SetClientCallback(this);
		}
	} while (0);

	// 进行网络连接
	do
	{
		m_iCurOperateStatus = OperateStatus::OS_UPDATE;
		m_pLoginServer = LoginServer::GetInstance();
		m_pLoginServer->SetDelegate(this, &m_PaintManager);
		m_pLoginServer->Connect();
	} while (0);

	g_Logger->TryInfo(_T("LoginFrame - Open Complete"));
}

void LoginFrame::OnWkeTitleChange(const CDuiString& strTitle)
{
}

void LoginFrame::OnWkeUrlChange(const CDuiString& strUrl)
{
	if (m_pWkeCtl->IsClickURL())
	{
		m_pWkeCtl->ResetClickURL();
		wkeWebView pWebView = m_pWkeCtl->GetWebView();
		pWebView->stopLoading();
		pWebView->loadURL(m_pWkeCtl->GetUrl().GetData());
		g_GameUtil->OpenWebByURL(strUrl.GetData());
	}
}

void LoginFrame::RefreshLoginData(LPCTSTR pzAccount /* = _T("") */, LPCTSTR pzPassword /* = _T("") */)
{
	m_pEdtAccount->SetText(pzAccount);
	m_pEdtPassword->SetText(pzPassword);
	_MatchAccount();
}

void LoginFrame::MsgBoxProc(DLG_ID id, TNotifyUI& msg)
{
	LoginMsgBoxID msg_box_id = (LoginMsgBoxID)id;
	switch (msg_box_id)
	{
		// 登录账号为空
		case LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_ACCOUNT:
		{
			m_PaintManager.SetFocusNeeded(m_pEdtAccount);
		}break;

		// 登录密码为空
		case LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_PASSWORD:
		{
			m_PaintManager.SetFocusNeeded(m_pEdtPassword);
		}break;

		// 客户端损坏
		case LoginMsgBoxID::LOGIN_MSG_BOX_ID_CLIENT_DAMAGED:
		{
			this->Close();
		}break;

		// 更新大厅
		case LoginMsgBoxID::LOGIN_MSG_BOX_ID_PLAZA_STATUS_LOW:
		{
			if (_tcscmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
			{
				if (_tcscmp(msg.pSender->GetName(), MsgBoxWnd::g_kBtnDialog_Ensure) == 0)
				{
					// 点击确认,直接更新
					this->StartUpdate();
				}
				else if (_tcscmp(msg.pSender->GetName(), MsgBoxWnd::g_kBtnDialog_Cancel) == 0
					|| _tcscmp(msg.pSender->GetName(), MsgBoxWnd::g_kBtnDialog_Close) == 0)
				{
					// 若为强制更新，取消则直接关闭大厅进程
					if (!m_bAllowConnect)
					{
						this->Close();
					}
				}
			}
		}break;

		// 更新失败
		case  LoginMsgBoxID::LOGIN_MSG_BOX_ID_PLAZA_UPDATE_FAILED:
		{
			this->Close();
		}break;

		default:
			break;
	}
}

void LoginFrame::CheckUpdate()
{
	if (m_pLoginServer->IsConnect())
	{
		tagLS_VersionReq tVersionReq;
		tVersionReq.dwPlazaVersion = VER_PLAZA_FRAME;
		m_pLoginServer->SendData(MAIN_LS_SYSTEM, SUB_LS_VERSION_REQ, &tVersionReq, sizeof(tagLS_VersionReq));
	}
	else
	{
		m_iCurOperateStatus = OperateStatus::OS_UPDATE;
		m_pLoginServer->Connect();
	}
}

void LoginFrame::LoginCheck()
{
	// 本地初次检测账号、密码输入是否为空
	if (m_pEdtAccount->GetText().GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LoginFrame::MsgBoxProc, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("账号不能为空!")
			, DLG_ID(LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_ACCOUNT)
			, DLG_STYLE::DLG_OK
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else if (m_pEdtPassword->GetText().GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LoginFrame::MsgBoxProc, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("密码不能为空!")
			, DLG_ID(LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_PASSWORD)
			, DLG_STYLE::DLG_OK
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	else	// 登录连接检测
	{
		ShowWaittingText(true, "正在连接服务器,请稍后...");

		if (m_pLoginServer->IsConnect())
		{
			this->LoginConnect();
		}
		else
		{
			m_iCurOperateStatus = OperateStatus::OS_LOGIN;
			m_pLoginServer->Connect();
		}
	}
}

void LoginFrame::StartUpdate()
{
	// 启动更新进程
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOWMAXIMIZED;
	PROCESS_INFORMATION	tProcessInfo;	// 游戏进程信息
	ZeroMemory(&tProcessInfo, 0, sizeof(tProcessInfo));

	TCHAR pzCommomLine[1024];
	_stprintf_s(pzCommomLine, _T("/Transmittal_Begin[vHigh=%u,vLow=%u,hResourceInst=%d,iResourceID=%d,iResourceType=%d,iSkinResID=%d,sResPath=%s,sResDllPath=%s]/Transmittal_End")
		, m_tCurServerVersion.wServerHigh
		, m_tCurServerVersion.wServerLow
		, (int)GameResource::g_ResourceInst
		, (int)GameResource::g_ResourceID
		, GameResource::g_ResourceType
		, GameResource::g_SkinResID
		, GameResource::g_ResourcePath
		, GameResource::g_ResourceDllPath);
	
	CDuiString pzFilePath;
	pzFilePath.Format(_T("%sUpdate.exe"),m_PaintManager.GetInstancePath().GetData());
	BOOL bSuccess = ::CreateProcess(pzFilePath.GetData(), pzCommomLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &tProcessInfo);
	if (TRUE == bSuccess)
	{
		this->Close();
	}
	else
	{
		TCHAR pzErrorDesc[MAX_PATH] = { 0 };
		_stprintf_s(pzErrorDesc, _T("%s(%d)"), _T("启动更新程序失败!"), (int)GetLastError());

		DialogCallBack dlg_cb = CC_CALLBACK_2(LoginFrame::MsgBoxProc, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, pzErrorDesc
			, DLG_ID(LoginMsgBoxID::LOGIN_MSG_BOX_ID_PLAZA_UPDATE_FAILED)
			, DLG_STYLE::DLG_OK
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
}

LRESULT LoginFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		CControlUI* pFocusCtl = m_PaintManager.GetFocus();
		if (pFocusCtl == m_pComboAccount)
		{
			if (m_pEdtAccount->GetText().GetLength() > 0 && m_pEdtPassword->GetText().GetLength() > 0)
			{
				this->LoginCheck();
			}
			else
			{
				m_pEdtAccount->SetFocus();
			}
		}

		if (m_pEdtAccount->IsFocused())
		{
			if (m_pEdtAccount->GetText().GetLength() > 0 && m_pEdtPassword->GetText().GetLength() <= 0)
			{
				m_pEdtPassword->SetFocus();
			}
			else
			{
				this->LoginCheck();
			}

			return TRUE;
		}
		else if (m_pBtnLogin->IsFocused() || m_pEdtPassword->IsFocused())
		{
			this->LoginCheck();

			return TRUE;
		}
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return FALSE;
}

void LoginFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnLogin_Close) == 0)					// 关闭
		{
			this->Exit();
		}
		else if(_tcscmp(sName, R->kBtnLogin_Exit) == 0)					// 退出
		{
			this->Exit();
		}
		else if (_tcscmp(sName, R->kBtnLogin_Min) == 0)					// 最小化
		{
			this->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (_tcscmp(sName, R->kBtnLogin_Retrieve) == 0)			// 找回密码
		{
			PasswordFrame* passwordFrame = new PasswordFrame();
			passwordFrame->CreateFrame(&m_PaintManager);
		}
		else if (_tcscmp(sName, R->kBtnLogin_UnBind) == 0)				// 解绑
		{
			LockLoginFrame* pLockFrame = new LockLoginFrame();
			pLockFrame->CreateFrame(&m_PaintManager);
		}
		else if (_tcscmp(sName, R->kBtnLogin_Regist) == 0)				// 注册
		{
			this->ShowWindow(false);
			RegistFrame* registFrame = new RegistFrame();
			registFrame->CreateFrame();
		}
		else if (_tcscmp(sName, R->kBtnLogin_Clear) == 0)				// 清除记录
		{
			this->_ClearAccountFile();
			DialogFrame::Toast(&m_PaintManager, _T("清除成功"));
		}
		else if (_tcscmp(sName, R->kOptLogin_Check) == 0)				// 记住密码
		{
			//do sth
		}
		else if (_tcscmp(sName, R->kBtnLogin_Login) == 0)				// 登录
		{
			this->LoginCheck();
		}
		else if (_tcscmp(sName, R->kBtnLogin_OfficialWeb) == 0)			// 官网
		{
			g_GameUtil->OpenWebByName(_T("Official"));
		}
		else if (_tcscmp(sName, R->kBtnLogin_Recharge) == 0)			// 充值
		{
			g_GameUtil->OpenWebByName(_T("Recharge"));
		}
		else if (_tcscmp(sName, R->kBtnLogin_CustomService) == 0)		// 客服
		{
			g_GameUtil->OpenWebByName(_T("CustomService"));
		}
		else if (_tcscmp(sName, R->kBtnLogin_FeedBack) == 0)			// 反馈
		{
			g_GameUtil->OpenWebByName(_T("FeedBack"));
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
		if (_tcscmp(sName, R->kCmbLogin_Account) == 0)
		{
			int iIndex = m_pComboAccount->GetCurSel();
			CListLabelElementUI* pListItem = static_cast<CListLabelElementUI*>(m_pComboAccount->GetItemAt(iIndex));
			if (pListItem)
			{
				tAccountInfo* pAccountInfo = (tAccountInfo*)pListItem->GetVoidData();
				if (pAccountInfo)
				{
					m_pEdtAccount->SetText(pAccountInfo->szAccount);
					m_pOptCheckPW->Selected(pAccountInfo->bTick);
					this->_MatchAccount();
				}
			}
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_DROPDOWN) == 0)
	{
		if (_tcscmp(sName, R->kCmbLogin_Account) == 0)
		{
			//m_pEdtAccount->SetText(msg.pSender->GetText());
		}
	}
	//__super::Notify(msg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//登录模块



bool LoginFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	g_Logger->TryInfo(_T("LoginFrame - LoginServer Close [ShutReason = %u]"), cbShutReason);
	
	return true;
}

void LoginFrame::_CreatUserDir(bool bUser /* = true */)
{
	CDuiString pzDir;
	pzDir.Format(_T("%sGamePlaza\\data\\user"), CPaintManagerUI::GetInstancePath().GetData());
	_mkdir(pzDir.GetData());

	if (bUser)
	{
		LPCTSTR lpAccount = m_pEdtAccount->GetText().GetData();
		_stprintf_s(m_pzBuffer, _T("%s\\%s"), pzDir.GetData(), lpAccount);
		_mkdir(m_pzBuffer);
	}
}

void LoginFrame::_ReadStyleFont()
{
	ServiceData* pServiceData = ServiceData::GetInstance();
	CDuiString pzFileName = _T("stylefont");
	
#ifdef UNICODE
	#ifdef SKIN_RES_TEST
		CXmlDocument xmlDoc;
		CXmlNode xmlNode, xmlSubNode;

		lpszFileName.Format(_T("%s%s"),CPaintManagerUI::GetResourcePath().GetData(), pzFileName.GetData());
		BOOL bRet = xmlDoc.Load(lpszFileName);
		if (!bRet)
			return FALSE;

		bRet = xmlDoc.SelectSingleNode(pzFileName, xmlNode);
		if (bRet)
		{
			bRet = xmlNode.GetFirstChildNode(_T("font"), xmlSubNode);
			while (bRet)
			{
				GameFontColor* pFontColor = new GameFontColor();
				if (pFontColor != NULL)
				{
					pFontColor->id = xmlSubNode.GetAttributeInt(_T("id"));
				}
				//else if
				bRet = xmlSubNode.GetNextSiblingNode(_T("font"), xmlSubNode);
			}
		}

		xmlSubNode.Release();
		xmlNode.Release();
		xmlDoc.Release();
	#endif //SKIN_RES_TEST
#else
	TiXmlDocument xmlDoc;
	TiXmlElement* xmlNode = nullptr;

#ifdef SKIN_RES_TEST
	pzFileName.Format(_T("%s%s"),CPaintManagerUI::GetResourcePath().GetData(), pzFileName.GetData());
	if (!xmlDoc.LoadFile(pzFileName, TIXML_ENCODING_UTF8))	return;
#else
	DWORD dwSize = CRenderEngine::GetZipItemSize(pzFileName.GetData());
	if (dwSize <= 0) return;
	char* pData = new char[dwSize];
	CRenderEngine::GetZipItemData(pzFileName.GetData(), pData);
	xmlDoc.Parse(pData);
	SAFE_DELETE(pData);
#endif // SKIN_RES_TEST

	xmlNode = xmlDoc.RootElement();
	if (xmlNode)
	{
		// 默认文本等属性配置
		do
		{
			//TiXmlAttribute* pAttrib = xmlNode->FirstAttribute();
			TiXmlNode* pFontsNode = xmlNode->FirstChild("fonts");
			if (pFontsNode)
			{
				TiXmlElement* pFontElement = pFontsNode->FirstChildElement();
				while (pFontElement)
				{
					GameFontColor* pFontColor = new GameFontColor();
					TiXmlAttribute* pAttrib = pFontElement->FirstAttribute();
					while (pAttrib)
					{
						if (_tcscmp(pAttrib->Name(), _T("id")) == 0)
						{
							pFontColor->id = pAttrib->IntValue();
						}
						else if (_tcscmp(pAttrib->Name(), _T("textcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwTextcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("hottextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwHotTextcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("pushedtextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwPushedTextcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("selectedtextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwSelectedTextcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("disabledtextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwDisabledtextcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("bkcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwBkcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("hotbkcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwHotBkcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("selectedbkcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwSelectedBkcolor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("focusbordercolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pFontColor->dwFocusborderColor = clrColor;
						}
						pAttrib = pAttrib->Next();
					}

					pServiceData->InsertGameFontColor(pFontColor);
					SAFE_DELETE(pFontColor);
					pFontElement = pFontElement->NextSiblingElement();
				}
			}
		} while (0);
		
		// 桌子文本等属性配置
		do
		{
			TiXmlNode* pTableNormalNode = xmlNode->FirstChild("table_normal");
			if (pTableNormalNode)
			{
				TiXmlElement* pTableElement = pTableNormalNode->ToElement();
				if (pTableElement)
				{
					GameNormalTableConfig* pNormalTable = new GameNormalTableConfig();
					TiXmlAttribute* pAttrib = pTableElement->FirstAttribute();
					while (pAttrib)
					{
						if (_tcscmp(pAttrib->Name(), _T("player_color")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pNormalTable->dwPlayerColor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("index_color")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pNormalTable->dwIndexColor = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("img_ready")) == 0)
						{
							::StrToCharArray(pNormalTable->pzImgReady, pAttrib->Value());
						}
						else if (_tcscmp(pAttrib->Name(), _T("img_lock")) == 0)
						{
							::StrToCharArray(pNormalTable->pzImgLock, pAttrib->Value());
						}
						else if (_tcscmp(pAttrib->Name(), _T("img_hotbox")) == 0)
						{
							::StrToCharArray(pNormalTable->pzImgHotBox, pAttrib->Value());
						}

						pAttrib = pAttrib->Next();
					}

					pServiceData->SetNormalTableConfig(pNormalTable);
					SAFE_DELETE(pNormalTable);
				}
			}
		} while (0);
		
		// 会员文本颜色等配置
		do
		{
			TiXmlNode* pMemberOrderNode = xmlNode->FirstChild("member_order");
			if (pMemberOrderNode)
			{
				TiXmlElement* pMemberOrderElement = pMemberOrderNode->ToElement();
				if (pMemberOrderElement)
				{
					MemberOrderConfig* pMemOrderConfig = new MemberOrderConfig();
					TiXmlAttribute* pAttrib = pMemberOrderElement->FirstAttribute();
					while (pAttrib)
					{
						if (_tcscmp(pAttrib->Name(), _T("order0_color")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pMemOrderConfig->dwOrder0Color = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("order1_color")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pMemOrderConfig->dwOrder1Color = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("order2_color")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pMemOrderConfig->dwOrder2Color = clrColor;
						}
						else if (_tcscmp(pAttrib->Name(), _T("order3_color")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pMemOrderConfig->dwOrder3Color = clrColor;
						}
						pAttrib = pAttrib->Next();
					}

					pServiceData->SetMemberOrderConfig(pMemOrderConfig);
					SAFE_DELETE(pMemOrderConfig);
				}
			}
		} while (0);

		// 聊天框字体颜色配置
		do
		{
			TiXmlNode* pChatColorNode = xmlNode->FirstChild("chat_color");
			if (pChatColorNode)
			{
				TiXmlElement* pChatColorElement = pChatColorNode->ToElement();
				if (pChatColorElement)
				{
					ChatColorConfig* pChatColorConfig = new ChatColorConfig();
					TiXmlAttribute* pAttrib = pChatColorElement->FirstAttribute();
					while (pAttrib)
					{
						if (_tcscmp(pAttrib->Name(), _T("defaultcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwDefaultColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("sysheadcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwSysHeadColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("systextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwSysTextColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("srcgameidcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwSrcGameIDColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("srcnameheadcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwSrcNameHeadColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("srcnametextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwSrcNameTextColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("rcvgameidcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwRcvGameIDColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("rcvnameheadcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwRcvNameHeadColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("rcvnametextcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwRcvNameTextColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("propsnamecolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwPropsNameColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("propscountcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwPropsCountColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("scrcharmcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwSrcCharmColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						else if (_tcscmp(pAttrib->Name(), _T("rcvcharmcolor")) == 0)
						{
							LPTSTR pstr = NULL;
							DWORD clrColor = _tcstoul(pAttrib->Value(), &pstr, 16);
							pChatColorConfig->dwRcvCharmColor = g_GameUtil->TransARGBToRGB(clrColor);
						}
						pAttrib = pAttrib->Next();
					}

					pServiceData->SetChatColorConfig(pChatColorConfig);
					SAFE_DELETE(pChatColorConfig);
				}
			}
		} while (0);
	}
	xmlDoc.Clear();
#endif // UNICODE
}

void LoginFrame::_ReadAccountFile()
{
	g_Logger->TryInfo(_T("LoginFrame - Init Account Config"));

	// 初始设置
	this->_CreatUserDir(false);
	m_pOptCheckPW->Selected(false);
	m_pBtnLogin->SetEnabled(true);
	m_pEdtAccount->SetText(_T(""));
	m_pEdtPassword->SetText(_T(""));

	// 读取本地账号配置
	m_pConfigFile = new JsonFile(_T("user\\INFO"));
	bool bResult = m_pConfigFile->initRootNode(m_vConfigRoot);
	if (bResult)
	{
		//账号候选列表
		if (m_vConfigRoot["AccountInfo"].isArray())
		{
			this->_ClearAccount();
			m_pComboAccount->RemoveAll();
			for (int i = 0; i < m_vConfigRoot["AccountInfo"].size(); ++i)
			{
				Json::Value& vNode = m_vConfigRoot["AccountInfo"][i];

				if (vNode["pzAccount"].isString() && vNode["pzPwd"].isString())
				{
					LPCTSTR lpAccount	= vNode["pzAccount"].asCString();
					LPCTSTR lpPdw		= vNode["pzPwd"].asCString();
					bool bTick			= vNode["bTicked"].asBool();
					int iSkinIndex		= vNode["iSkinIndex"].asInt();

					tAccountInfo* pAccountInfo	= new tAccountInfo();
					pAccountInfo->bTick			= bTick;
					pAccountInfo->iSkinIndex	= iSkinIndex;

					::StrToCharArray(pAccountInfo->szAccount, lpAccount);
					::StrToCharArray(pAccountInfo->szPwd, lpPdw);
					m_vAccountArr.push_back(pAccountInfo);

					// 数据插入候选框
					CListLabelElementUI* pListItem = new CListLabelElementUI();
					pListItem->SetText(lpAccount);
					pListItem->SetVoidData(pAccountInfo);
					m_pComboAccount->Add(pListItem);
				}
			}
		}
	}

	// 聚焦首个账号候选
	m_pComboAccount->SelectItem(0);

	g_Logger->TryInfo(_T("LoginFrame - Init Account Config Complete"));
}

void LoginFrame::_SaveAccountFile()
{
	g_Logger->TryInfo(_T("LoginFrame - Save Account Config"));

	LPCTSTR lpAccount	= m_pEdtAccount->GetText().GetData();
	LPCTSTR lpPassword	= m_pEdtPassword->GetText();

	//记录登录信息
	std::vector<tAccountInfo*> vstrArr;
	if (_tcscmp(lpAccount, _T("")) != 0 && _tcscmp(lpPassword, _T("")) != 0)
	{
		tAccountInfo* pAccountInfo = nullptr;
		if (m_pOptCheckPW)
		{
			pAccountInfo = new tAccountInfo();
			pAccountInfo->bTick = m_pOptCheckPW->IsSelected();
			::StrToCharArray(pAccountInfo->szAccount, lpAccount);
			if (pAccountInfo->bTick)
			{
				::StrToCharArray(pAccountInfo->szPwd, lpPassword);
			}

			vstrArr.push_back(pAccountInfo);
		}

		if (m_vConfigRoot["AccountInfo"].isArray())
		{
			for (int i = 0; i < m_vConfigRoot["AccountInfo"].size(); ++i)
			{
				Json::Value vNode = m_vConfigRoot["AccountInfo"][i];
				if (vNode["pzAccount"].isString() && vNode["pzPwd"].isString())
				{
					LPCTSTR strAccount	= vNode["pzAccount"].asCString();
					LPCTSTR strPwd		= vNode["pzPwd"].asCString();
					bool bTick			= vNode["bTicked"].asBool();
					int iSkinIndex		= vNode["iSkinIndex"].asInt();

					pAccountInfo				= new tAccountInfo();
					pAccountInfo->bTick			= bTick;
					pAccountInfo->iSkinIndex	= iSkinIndex;
					::StrToCharArray(pAccountInfo->szAccount, strAccount);
					::StrToCharArray(pAccountInfo->szPwd, strPwd);

					if (_tcscmp(lpAccount, strAccount) == 0) continue;
					vstrArr.push_back(pAccountInfo);
				}
			}
		}

		// 添加至账号候选列表
		m_vConfigRoot.removeMember("AccountInfo");
		for (size_t i = 0; i < vstrArr.size(); ++i)
		{
			Json::Value vNode;
			vNode["pzAccount"]	= vstrArr[i]->szAccount;
			vNode["pzPwd"]		= vstrArr[i]->szPwd;
			vNode["bTicked"]	= vstrArr[i]->bTick;
			vNode["iSkinIndex"]	= vstrArr[i]->iSkinIndex;

			m_vConfigRoot["AccountInfo"].append(vNode);
		}

		// 去除多余项
		for (int i = CANDIDATE_ACCOUNT_NUM; i < m_vConfigRoot["AccountInfo"].size(); ++i)
		{
			Json::Value vNode = m_vConfigRoot["AccountInfo"][i];
			m_vConfigRoot["AccountInfo"].removeIndex(i, &vNode);
		}
	}
	m_pConfigFile->SaveFile(m_vConfigRoot);

	g_Logger->TryInfo(_T("LoginFrame - Save Account Config Complete"));
}

void LoginFrame::_ClearAccountFile()
{
	if (!m_pConfigFile) return;

	CDuiString pzAccount = m_pEdtAccount->GetText();

	// 清除数据
	do
	{
		// 清除账号信息
		if (m_vConfigRoot["AccountInfo"].isArray())
		{
			for (int i = 0; i < m_vConfigRoot["AccountInfo"].size(); ++i)
			{
				int x = m_vConfigRoot["AccountInfo"].size();
				Json::Value vNode = m_vConfigRoot["AccountInfo"][i];
				if (vNode["pzAccount"].isString() && vNode["pzPwd"].isString())
				{
					LPCTSTR strAccount = vNode["pzAccount"].asCString();
					LPCTSTR strPwd = vNode["pzPwd"].asCString();
					if (_tcscmp(strAccount, pzAccount) == 0)
					{
						m_vConfigRoot["AccountInfo"].removeIndex(i, &vNode);
					}
				}
			}
		}

		// 清除与账号相关联的文件夹
		if (pzAccount.GetLength() > 0)
		{
			_stprintf_s(m_pzBuffer, _T("%s..\\data\\user\\%s"), CPaintManagerUI::GetResourcePath().GetData(), pzAccount.GetData());
			g_GameUtil->DeleteDirectory(m_pzBuffer);
		}

		// 写入文件
		m_pConfigFile->SaveFile(m_vConfigRoot);
	} while (0);

	// 清除UI
	do
	{
		m_pEdtAccount->SetText(_T(""));
		m_pEdtPassword->SetText(_T(""));
		m_pOptCheckPW->Selected(false);

		for (int i = 0; i < m_pComboAccount->GetCount(); ++i)
		{
			CListLabelElementUI* pListItem = static_cast<CListLabelElementUI*>(m_pComboAccount->GetItemAt(i));
			if (pListItem)
			{
				tAccountInfo* pAccountInfo = (tAccountInfo*)pListItem->GetVoidData();
				if (pAccountInfo)
				{
					if (_tcscmp(pzAccount, pAccountInfo->szAccount) == 0)
					{
						m_pComboAccount->RemoveAt(i);
					}
				}
			}
		}

		// 聚焦输入账号
		m_pEdtAccount->SetFocus();
	} while (0);
}

void LoginFrame::_MatchAccount()
{
	bool bRes = false;
	CDuiString& strAccount = m_pEdtAccount->GetText();
	for (const auto& it : m_vAccountArr)
	{
		if (_tcscmp(it->szAccount, strAccount.GetData()) == 0)
		{
			bRes = true;
			m_pEdtPassword->SetText(it->szPwd);
			break;
		}
	}

	if (!bRes)
	{
		m_pEdtPassword->SetText(_T(""));
	}

	if (m_pEdtAccount->GetText().GetLength() <= 0)
	{
		m_PaintManager.SetFocusNeeded(m_pEdtAccount);
	}
	else if (m_pEdtPassword->GetText().GetLength() <= 0)
	{
		m_PaintManager.SetFocusNeeded(m_pEdtPassword);
	}
	else
	{
		m_PaintManager.SetFocusNeeded(m_pBtnLogin);
	}
}

void LoginFrame::_ClearAccount()
{
	for (const auto& it : m_vAccountArr)
	{
		tAccountInfo* pAccountInfo = it;
		SAFE_DELETE(pAccountInfo);
	}

	m_vAccountArr.clear();
}

void LoginFrame::_ReadWebConfig()
{
	g_Logger->TryInfo(_T("LoginFrame - Init Web Config"));

	Json::Value vUrlRoot;
	JsonFile* pUrlFile = new JsonFile(_T("WEB"));
	bool bResult = pUrlFile->initRootNode(vUrlRoot);
	SAFE_DELETE(pUrlFile);
	if (bResult)
	{
		Json::Value& vWebNode = vUrlRoot["Web"];
		if (vWebNode.isArray())
		{
			ServiceData* pServiceData = ServiceData::GetInstance();
			for (int i = 0; i < vWebNode.size(); ++i)
			{
				LPCTSTR lpName = vWebNode[i]["name"].asCString();
				LPCTSTR lpUrl = vWebNode[i]["url"].asCString();
				pServiceData->AddUrlWeb(lpName, lpUrl);
			}
		}
	}

	// 检查、保存IP地址
	LPCTSTR lpServerUrl = g_GameUtil->GetWebByName(_T("LoginServer"));
	if (lpServerUrl)
	{
		::StrToCharArray(R->LoginServer_IP, lpServerUrl);
	}
	else
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(LoginFrame::MsgBoxProc, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示")
			, _T("文件已损坏,请重新下载安装!")
			, DLG_ID(LoginMsgBoxID::LOGIN_MSG_BOX_ID_CLIENT_DAMAGED)
			, DLG_STYLE::DLG_OK
			, dlg_cb
			, nullptr
			, dlg_cb);
		return;
	}

	g_Logger->TryInfo(_T("LoginFrame - Init Web Config Complete"));
}

void LoginFrame::_ReadUnrealConfig()
{
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	if (!pUnrealControl->IsLoadUnrealDll() && pUnrealControl->LoadUnrealDll())
	{
		pUnrealControl->LoadUnreal();
	}
}

void LoginFrame::_ExcStep(DWORD iStep)
{
	switch (iStep)
	{
		case TIMER_ID_LOGINEND_SERVER:
		{
			TimerCallBack timepro = CC_CALLBACK_2(LoginFrame::_OnTimer, this);
			g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_LOGINEND_SERVER, 100);
			m_pLoadingFrame->SetPercent(60);
		}break;
		
		case TIMER_ID_CREATEHALL:
		{
			// 创建大厅
			HallFrame* hallFrame = HallFrame::GetInstance();
			hallFrame->CreateFrame();
			hallFrame->ShowWindow(false);
			hallFrame->InitData();

			// 创建托盘窗口
			g_GameUtil->CreatTrayMenu(hallFrame->GetPaintManager());

			m_pLoadingFrame->SetPercent(80);
			TimerCallBack timepro = CC_CALLBACK_2(LoginFrame::_OnTimer, this);
			g_GameUtil->SetTimer(m_hWnd, timepro, TIMER_ID_CREATEHALL, 100);
		}break;

		case TIMER_ID_LOGINEND_CLIENT:
		{
			m_pLoadingFrame->SetPercent(100);

			// 关闭登陆界面
			this->Close(CMW_CLOSE_SURE);

			// 显示大厅UI
			HallFrame* hallFrame = HallFrame::GetInstance();
			hallFrame->ShowWindow(true);
		}break;
		default:
			break;
	}
}

void LoginFrame::_OnTimer(UINT_PTR idEvent, DWORD dwTime)
{
	LoginFrame* loginFrame = LoginFrame::GetInstance();
	switch (idEvent)
	{
		case TIMER_ID_LOGINEND_SERVER:
		{
			g_GameUtil->KillTimer(m_hWnd, TIMER_ID_LOGINEND_SERVER);
			loginFrame->_ExcStep(TIMER_ID_CREATEHALL);
		}break;
		
		case TIMER_ID_CREATEHALL:
		{
			g_GameUtil->KillTimer(m_hWnd, TIMER_ID_CREATEHALL);
			loginFrame->_ExcStep(TIMER_ID_LOGINEND_CLIENT);
		}break;
		
		default:
			break;
	}
}

void LoginFrame::_InitIPC()
{
	IPC* ipc = IPC::GetInstance();
}

void LoginFrame::_InitPrepare()
{
	this->_CreatUserDir();
	this->_SaveAccountFile();
	this->_InitIPC();

	this->ShowWindow(false);
	if (!m_pLoadingFrame)
	{
		m_pLoadingFrame = new LoadingFrame();
	}
}

void LoginFrame::_InitUserData(void* pData, WORD wDataSize)
{
	tagLS_LogonSuccess* pStruct = static_cast<tagLS_LogonSuccess*>(pData);
	if (pStruct)
	{
		tagGlobalUserData globalUser;
		DataManager* pDataManager = DataManager::GetInstance();
		UserData* pSelfData = pDataManager->CreatUserDataByID(pStruct->dwUserID, true);
		if (pSelfData)
		{
			globalUser.wFaceID			= pStruct->wFaceID;
			globalUser.cbGender			= pStruct->cbGender;
			globalUser.cbMember			= pStruct->cbMember;
			globalUser.cbMasterOrder	= pStruct->cbMasterOrder;
			globalUser.dwUserID			= pStruct->dwUserID;
			globalUser.dwGameID			= pStruct->dwGameID;
			globalUser.dwExperience		= pStruct->dwExperience;
			globalUser.dwCustomFaceVer	= pStruct->dwCustomFaceVer;
			globalUser.cbMoorMachine	= pStruct->cbMoorMachine;
			globalUser.lCharm			= pStruct->dwCharm;
			globalUser.lGameGlod		= pStruct->lGold;
			globalUser.lBankGold		= pStruct->lBankGold;
			
			//扩展信息
			void * pDataBuffer = NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pStruct + 1, wDataSize - sizeof(tagLS_LogonSuccess));
			while (true)
			{
				pDataBuffer = RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe == DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
					//用户帐户
					case DTP_ACCOUNT:
					{
						ASSERT(pDataBuffer != NULL);
						ASSERT(DataDescribe.wDataSize > 0);
						ASSERT(DataDescribe.wDataSize <= sizeof(globalUser.szAccount));
						if (DataDescribe.wDataSize <= sizeof(globalUser.szAccount))
						{
							CopyMemory(globalUser.szAccount, pDataBuffer, DataDescribe.wDataSize);
							globalUser.szAccount[ArrayCount(globalUser.szAccount) - 1] = 0;
						}
						break;
					}
					//昵称
					case DTP_NICKNAME:
					{
						ASSERT(pDataBuffer != NULL);
						ASSERT(DataDescribe.wDataSize > 0);
						ASSERT(DataDescribe.wDataSize <= sizeof(globalUser.szNickname));
						if (DataDescribe.wDataSize <= sizeof(globalUser.szNickname))
						{
							CopyMemory(globalUser.szNickname, pDataBuffer, DataDescribe.wDataSize);
							globalUser.szNickname[ArrayCount(globalUser.szNickname) - 1] = 0;
						}
						break;
					}
					//用户密码
					case DTP_USER_PWD:
					{
						ASSERT(pDataBuffer != NULL);
						ASSERT(DataDescribe.wDataSize > 0);
						ASSERT(DataDescribe.wDataSize <= sizeof(globalUser.szPwd));
						if (DataDescribe.wDataSize <= sizeof(globalUser.szPwd))
						{
							CopyMemory(globalUser.szPwd, pDataBuffer, DataDescribe.wDataSize);
							globalUser.szPwd[ArrayCount(globalUser.szPwd) - 1] = 0;
						}
						break;
					}
					//个性签名
					case DTP_UNDER_WRITE:
					{
						ASSERT(pDataBuffer != NULL);
						ASSERT(DataDescribe.wDataSize > 0);
						ASSERT(DataDescribe.wDataSize <= sizeof(globalUser.szUnderWrite));
						if (DataDescribe.wDataSize <= sizeof(globalUser.szUnderWrite))
						{
							CopyMemory(globalUser.szUnderWrite, pDataBuffer, DataDescribe.wDataSize);
							globalUser.szUnderWrite[ArrayCount(globalUser.szUnderWrite) - 1] = 0;
						}
						break;
					}
					//社团名字
					case DTP_USER_GROUP_NAME:
					{
						ASSERT(pDataBuffer != NULL);
						ASSERT(DataDescribe.wDataSize > 0);
						ASSERT(DataDescribe.wDataSize <= sizeof(globalUser.szGroupName));
						if (DataDescribe.wDataSize <= sizeof(globalUser.szGroupName))
						{
							CopyMemory(globalUser.szGroupName, pDataBuffer, DataDescribe.wDataSize);
							globalUser.szGroupName[ArrayCount(globalUser.szGroupName) - 1] = 0;
						}
						break;
					}
				}
			}

			pSelfData->SetAccount(globalUser.szAccount);
			pSelfData->SetPassword(m_pEdtPassword->GetText());
			pSelfData->SetNickName(globalUser.szNickname);
			pSelfData->SetGroupName(globalUser.szGroupName);
			pSelfData->SetUnderWrite(globalUser.szUnderWrite);

			pSelfData->SetFaceID(globalUser.wFaceID);
			pSelfData->SetGender(globalUser.cbGender);
			pSelfData->SetMemberOrder(globalUser.cbMember);
			pSelfData->SetMasterOrder(globalUser.cbMasterOrder);
			pSelfData->SetGameID(globalUser.dwGameID);
			pSelfData->SetExperience(globalUser.dwExperience);
			pSelfData->SetCustomFaceVer(globalUser.dwCustomFaceVer);
			pSelfData->SetMoorMachine(globalUser.cbMoorMachine);
			pSelfData->SetMachineID(g_GameUtil->GetMachineString().c_str());
			pSelfData->SetCharm(globalUser.lCharm);
			pSelfData->SetGameGold(globalUser.lGameGlod);
			pSelfData->SetBankScore(globalUser.lBankGold);
			pSelfData->ReadGameSetting();
		}

		// Unreal Init
		UnrealControl* pUnrealControl = UnrealControl::GetInstance();
		pUnrealControl->InitUnreal(pSelfData);
	}
	m_pLoadingFrame->SetPercent(10);
}

void LoginFrame::_InitGameConfigList(void* pData, WORD wDataSize)
{
	tagLS_ListConfig* pStruct = static_cast<tagLS_ListConfig*>(pData);
	if (pStruct)
	{
	}
	m_pLoadingFrame->SetPercent(20);
}

void LoginFrame::_InitGameTypeList(void* pData, WORD wDataSize)
{
	tagGameType* pStruct = static_cast<tagGameType*>(pData);
	ServiceData* pServerListData = ServiceData::GetInstance();
	if (!pStruct || !pServerListData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameType);
	for (size_t i = 0; i < wItemCount; ++i)
	{
		pServerListData->InsertGameType(pStruct++);
	}
	m_pLoadingFrame->SetPercent(30);
}

void LoginFrame::_InitGameKindList(void* pData, WORD wDataSize)
{
	tagGameKind* pStruct = static_cast<tagGameKind*>(pData);
	ServiceData* pServerListData = ServiceData::GetInstance();
	if (!pStruct || !pServerListData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameKind);
	for (size_t i = 0; i < wItemCount; ++i)
	{
		pServerListData->InsertGameKind(pStruct++);
	}

	m_pLoadingFrame->SetPercent(40);
}

void LoginFrame::_initGameStationList(void* pData, WORD wDataSize)
{
	tagGameStation* pStruct = static_cast<tagGameStation*>(pData);
	ServiceData* pServerListData = ServiceData::GetInstance();
	if (!pStruct || !pServerListData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameStation);
	for (size_t i = 0; i < wItemCount; ++i)
	{
		pServerListData->InsertGameStation(pStruct++);
	}

	m_pLoadingFrame->SetPercent(40);
}

void LoginFrame::_InitGameRoomList(void* pData, WORD wDataSize)
{
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();
	tagGameServer* pStruct = static_cast<tagGameServer*>(pData);
	ServiceData* pServerListData = ServiceData::GetInstance();
	if (!pStruct || !pServerListData) return;

	WORD wItemCount = wDataSize / sizeof(tagGameServer);
	for (size_t i = 0; i < wItemCount; ++i)
	{
		tagGameServer* pTmp = pStruct++;
		if (pTmp->bMobileRoom && (pTmp->wKindID == 30 || pTmp->wKindID == 31)
			&& (tSelf.GetMasterOrder() <= 0 && !tSelf.IsUnrealUser()))
		{
			continue;
		}

		pServerListData->InsertGameServer(pTmp);
	}

	m_pLoadingFrame->SetPercent(50);
}

void LoginFrame::_InitGameListComplete()
{
	m_pLoadingFrame->SetPercent(50);
}

void LoginFrame::_InitPropsList(void* pData, WORD wDataSize)
{
	ServiceData* pServerListData = ServiceData::GetInstance();
	tagPropsInfo* pPropsInfo = static_cast<tagPropsInfo*>(pData);
	if (!pPropsInfo || !pServerListData) return;

	WORD wItemCount = wDataSize / sizeof(tagPropsInfo);
	for (size_t i = 0; i < wItemCount; ++i)
	{
		pServerListData->InsertGameProps(pPropsInfo++);
	}
}

void LoginFrame::_InitLoganComplete()
{
	m_pLoginServer->CloseSocket();
	this->_ExcStep(TIMER_ID_LOGINEND_SERVER);
}

bool LoginFrame::OnConnectSuccessProc()
{
	g_Logger->TryInfo(_T("LoginFrame - ConnectServer OK"));
	switch (m_iCurOperateStatus)
	{
		case OperateStatus::OS_NONE:							break;
		case OperateStatus::OS_UPDATE:	this->CheckUpdate();	break;
		case OperateStatus::OS_LOGIN:	this->LoginConnect();	break;
		case OperateStatus::OS_EXTRA:							break;
		default:												break;
	}
	m_iCurOperateStatus = OperateStatus::OS_NONE;

	return true;
}

void LoginFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

void LoginFrame::OnWaittingTimeOut(DLG_ID id, TNotifyUI& msg)
{
	m_pLabWaitting->SetText(_T("连接服务器失败!"));
}

void LoginFrame::ShowWaittingText(bool bShow, CDuiString pzText /* = _T("") */)
{
	m_pLabWaitting->SetText(pzText);
	m_pLabWaitting->SetVisible(bShow);
	//m_pBtnLogin->SetEnabled(!bShow);

	if (bShow)
	{
		DialogCallBack cb = CC_CALLBACK_2(LoginFrame::OnWaittingTimeOut, this);
		m_dwWatingEventID = DialogFrame::Waitting(&m_PaintManager, DLG_ID::DLG_ID_NONE, cb);
	}
	else
	{
		DialogFrame::EndWaitting(m_dwWatingEventID);
		m_dwWatingEventID = INVALID_DWORD;
	}
}

void LoginFrame::Exit()
{
	// 清除登录服连接
	if (LoginServer::IsInstanceExist())
	{
		LoginServer* pLoginServer = LoginServer::GetInstance();
		pLoginServer->Release();
	}

	this->Close();
}

void LoginFrame::LoginConnect()
{
	g_Logger->TryInfo(_T("LoginFrame - Login Check"));

	//登录
	if (!m_pWaitDlgManager)
	{
		m_pWaitDlgManager = &m_PaintManager;
	}

	tagLS_LogonByAccount loginMsg;
	::ZeroMemory(&loginMsg, sizeof(tagLS_LogonByAccount));
	loginMsg.dwPlazaVersion = VER_PLAZA_FRAME;
	::StrToCharArray(loginMsg.szAccount, m_pEdtAccount->GetText());
	CMD5Encrypt::EncryptData(m_pEdtPassword->GetText(), loginMsg.szPwd);
	::StrToCharArray(loginMsg.szSMS, m_pzSMSVcode);
	m_pzSMSVcode.Empty();

	//追加机器码,平台标志
	tagClientSerial ClientSerial;
	::ZeroMemory(&ClientSerial, sizeof(tagClientSerial));
	ClientSerial.dwSystemVer = MACHINE_PC;
	::StrToCharArray(ClientSerial.szComputerID, g_GameUtil->GetMachineString().c_str());

	BYTE cbBuffer[TCP_PACKET_BUFFER_SIZE] = { 0 };
	::memcpy(cbBuffer, &loginMsg, sizeof(tagLS_LogonByAccount));
	CSendPacketHelper Packet(cbBuffer + sizeof(tagLS_LogonByAccount), sizeof(cbBuffer) - sizeof(tagLS_LogonByAccount));
	Packet.AddPacket(&ClientSerial, sizeof(ClientSerial), DTP_COMPUTER_ID);

	//发送
	g_Logger->TryInfo(_T("LoginFrame - Login Req"));
	g_Logger->TryDebug(_T("LoginFrame - Login Req [MID = %s]"), ClientSerial.szComputerID);
	DWORD iRes = m_pLoginServer->SendData(MAIN_LS_LOGON, SUB_LS_LOGON_ACCOUNT, cbBuffer, sizeof(tagLS_LogonByAccount) + Packet.GetDataSize());
	if (FALSE == iRes)
	{
		ShowWaittingText(false);

		DialogCallBack cb = CC_CALLBACK_2(LoginFrame::ReConnect, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("温馨提示:")
			, _T("登录失败，请检查您的网络!")
			, DLG_ID(LoginMsgBoxID::LOGIN_MSG_BOX_ID_NET_CONNECT_FAILED)
			, DLG_STYLE::DLG_RETRY_CANCEL
			, cb);
	}
}

bool LoginFrame::OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	ShowWaittingText(false);

	switch (cmd.wSubCmdID)
	{
		case SUB_LS_LOGON_ACCOUNT:
		{
			tagLS_LogonByAccount* pStruct = static_cast<tagLS_LogonByAccount*>(pDataBuffer);
			if (pStruct)
			{
			}
		}break;
		case SUB_LS_LOGON_GAME_ID:
		{
			tagLS_LogonByGameID* pStruct = static_cast<tagLS_LogonByGameID*>(pDataBuffer);
			if (pStruct)
			{
			}
		}break;
		case SUB_LS_REGISTER_ACCOUNT:
		{
			tagLS_RegisterAccount* pStruct = static_cast<tagLS_RegisterAccount*>(pDataBuffer);
			if (pStruct)
			{
			}
		}break;
		case SUB_LS_LOGON_SUCCESS:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login OK [Account = %s]"), m_pEdtAccount->GetText());

			// 如果存在换设备验证面板,则关闭该面板
			if (SMSLonginCheckFrame::IsInstanceExist())
			{
				SMSLonginCheckFrame* pSMSLoginCheckFrame = SMSLonginCheckFrame::GetInstance();
				pSMSLoginCheckFrame->Close(CMW_CLOSE_SURE);
			}

			// 初始化登录数据
			this->_InitPrepare();
			this->_InitUserData(pDataBuffer, wDataSize);
		}break;
		case SUB_LS_LOGON_ERROR:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login Failed"));
			tagLS_LogonError* pStruct = static_cast<tagLS_LogonError*>(pDataBuffer);
			if (pStruct)
			{
				g_Logger->TryInfo(_T("LoginFrame - Login Failed [ecode = %ld]"), pStruct->lErrorCode);

				uint uMsgID = LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_ACCOUNT;
				LONG e_code = pStruct->lErrorCode;
				switch (e_code)
				{
					// 账号不存在
					case 1:
					{
						uMsgID = LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_ACCOUNT;
					}break;

					// 账号关闭
					case 2:
					{

					}break;

					// 账号冻结
					case 3:
					{

					}break;

					// 已绑定别的机器
					case 4:
					{

					}break;

					// 密码错误
					case 5:
					{
						uMsgID = LoginMsgBoxID::LOGIN_MSG_BOX_ID_LOGIN_PASSWORD;
					}break;

					// 已经在房间中
					case 6:
					{
						
					}break;

					// 不同设备,验证码登录
					case 7:
					{
						SMSLonginCheckFrame* pSMSLoginCheckFrame = SMSLonginCheckFrame::GetInstance();
						pSMSLoginCheckFrame->CreateFrame(&m_PaintManager, m_pEdtAccount->GetText());
					}break;

					// 更换设备验证失败
					case 8:
					{
						if (SMSLonginCheckFrame::IsInstanceExist())
						{
							SMSLonginCheckFrame* pSMSLoginCheckFrame = SMSLonginCheckFrame::GetInstance();
							pSMSLoginCheckFrame->SMSCheckFailed(pStruct->szErrorDescribe);
						}
					}break;

					default:
						break;
				}
				
				if (_tcscmp(pStruct->szErrorDescribe, _T("")) != 0 && e_code != 7 && e_code != 8)
				{
					DialogCallBack dlg_cb = CC_CALLBACK_2(LoginFrame::MsgBoxProc, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示:")
						, pStruct->szErrorDescribe
						, DLG_ID(uMsgID)
						, DLG_STYLE::DLG_DEFAULT
						, dlg_cb
						, dlg_cb
						, dlg_cb);
				}
			}
		}break;
		case SUB_LS_LOGON_FINISH:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login Finish"));
			this->_InitLoganComplete();
		}break;
		default:
			break;
	}

	return true;
}

bool LoginFrame::OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	return true;
}

bool LoginFrame::OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS_LIST_CONFIG:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List Config"));
			this->_InitGameConfigList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_TYPE:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List Type"));
			this->_InitGameTypeList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_KIND:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List Kind"));
			this->_InitGameKindList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_STATION:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List Station"));
			this->_initGameStationList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_GAME_SERVER:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List GameServer"));
			this->_InitGameRoomList(pDataBuffer, wDataSize);
		}break;

		case SUB_LS_LIST_HALL_SERVER:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List HallServer"));
			tagGameStation* pStruct = static_cast<tagGameStation*>(pDataBuffer);
		}break;

		case SUB_LS_LIST_FINISH:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login List Finish"));
			this->_InitGameListComplete();
		}break;

		default:
			break;
	}

	return true;
}

bool LoginFrame::OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GF_LOADPROPS:
		{
			g_Logger->TryInfo(_T("LoginFrame - Login Load Props"));
			this->_InitPropsList(pDataBuffer, wDataSize);
		}break;

		default:
			break;
	}

	return true;
}

bool LoginFrame::OnNetwork_System(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		// 版本通知(最多下发两次:第一次为主动请求检测,第二次为登录时服务器检测)
		case SUB_LS_VERSION:
		{

#ifdef GAME_TEST_SWITCH
			return true;
#endif // GAME_TEST_SWITCH

			tagLS_Version* vStruct = static_cast<tagLS_Version*>(pDataBuffer);
			if (vStruct && !m_bAllowConnect)
			{
				memcpy(&m_tCurServerVersion, vStruct, sizeof(tagLS_Version));
				if (TRUE == vStruct->bNewVersion)
				{
					m_bAllowConnect = vStruct->bAllowConnect;
					TCHAR pzDesc[MAX_PATH] = { 0 };
					_stprintf_s(pzDesc, _T("发现最新版本v%d.%d.%d.%d，是否现在更新?")
						, GAMEPLAZA_VERSION_1(vStruct->wServerHigh)
						, GAMEPLAZA_VERSION_2(vStruct->wServerHigh)
						, GAMEPLAZA_VERSION_3(vStruct->wServerLow)
						, GAMEPLAZA_VERSION_4(vStruct->wServerLow));
					DialogCallBack dlg_cb = CC_CALLBACK_2(LoginFrame::MsgBoxProc, this);
					DialogFrame::MsgBox(&m_PaintManager
						, _T("温馨提示")
						, pzDesc
						, DLG_ID(LoginMsgBoxID::LOGIN_MSG_BOX_ID_PLAZA_STATUS_LOW)
						, DLG_STYLE::DLG_OK_CANCEL
						, dlg_cb
						, dlg_cb
						, dlg_cb);
				}
			}
		}break;
		
		// 系统维护
		case SUB_LS_MAINTENANCE:
		{
			tagLS_Maintenance* vStruct = static_cast<tagLS_Maintenance*>(pDataBuffer);
		}break;

		default:
			break;
	}
	return true;
}

bool LoginFrame::OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	return true;
}
