#include "stdafx.h"
#include "mainFrame.h"

extern CStaticLogger	g_Logger;
extern ResInfo			g_tResInfo;

GameResource* GameResource::g_instance = NULL;
GameResource* GameResource::getInstance()
{
	if (GameResource::g_instance)
		return GameResource::g_instance;

	GameResource::g_instance = new GameResource();
	return GameResource::g_instance;
}

extern GameResource* R;
MainFrame* MainFrame::g_instance = nullptr;
MainFrame::MainFrame()
	: m_pProgress(nullptr)
	, m_pLabTotal(nullptr)
	, m_pLabSpeed(nullptr)
	, m_pDownloader(nullptr)
{
	memset(m_pzBuff, 0, MAX_PATH * sizeof(TCHAR));
}

MainFrame::~MainFrame()
{
	SAFE_DELETE(m_pDownloader);

	// 退出消息循环(其后不能再有消息的收发)
	g_Logger->TryInfo(_T("MainFrame - Destroy"));
	::PostQuitMessage(0);
}

MainFrame* MainFrame::getInstance()
{
	if (MainFrame::g_instance == nullptr)
		MainFrame::g_instance = new (std::nothrow)MainFrame();

	return MainFrame::g_instance;
}

int MainFrame::CreateFrame(LPCTSTR lpURLCombine)
{
	m_strUrl = lpURLCombine;
	if (FALSE == IsWindow(m_hWnd))
	{
		int style = UI_CLASSSTYLE_DIALOG | WS_MINIMIZEBOX;
		m_hWnd = this->Create(NULL, R->MainWndTitleName, style, 0L);

		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
		::SwitchToThisWindow(m_hWnd, true);

	return TRUE;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("MainFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_MY_LIBCURL:
		{
			DownLoadStruct* pDownloadStruct = (DownLoadStruct*)wParam;
			if (pDownloadStruct && pDownloadStruct->pDelegate)
			{
				this->OnLibcurlFailed(pDownloadStruct);
			}
			return TRUE;
		}break;
		
		default:
			break;
	}

	return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return g_tResInfo.iResourceType;
}

LPCTSTR MainFrame::GetResourceID() const
{
	return g_tResInfo.iResourceID;
}

CDuiString MainFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString MainFrame::GetSkinFile()
{
	return _T("update/update_plaza.xml");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
		return new CWkeWebkitUI();

	return NULL;
}

void MainFrame::InitWindow()
{
	g_Logger->TryInfo(_T("MainFrame - Open"));

	m_pProgress		= static_cast<CProgressUI*>(m_PaintManager.FindControl(R->kBtnUpdatePlaza_Progress));
	m_pLabTotal		= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabUpdatePlaza_Total));
	m_pLabSpeed		= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabUpdatePlaza_Speed));
	m_pBtnDownload	= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnUpdatePlaza_Download));
	m_pBtnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnUpdatePlaza_Cancel));
	m_pBtnClose		= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnUpdatePlaza_Close));
}

void MainFrame::InitData(TNotifyUI& msg)
{
	// 设置窗口图标
	this->SetIcon(IDI_UPDATEMODE);
	
	// 初始化下载
	this->InitDownload();

	g_Logger->TryInfo(_T("MainFrame - Open Complete"));
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return TRUE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return FALSE;
}

void MainFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)			//窗口初始化
	{
		this->InitData(msg);
	}
	else if (_tcscmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)	//鼠标单击
	{
		if (_tcscmp(sName, R->kBtnUpdatePlaza_Close) == 0 || _tcscmp(sName, R->kBtnUpdatePlaza_Cancel) == 0)
		{
			if (m_pDownloader)
			{
				m_pDownloader->AbortDownload();
			}
			else
			{
				this->Close();
			}
		}
		else if (_tcscmp(sName, R->kBtnUpdatePlaza_Download) == 0)
		{
			LPCTSTR lpDownName = _T("Official");
			Json::Value vRoot;
			JsonFile* pUrlFile = new JsonFile(_T("WEB"));
			if (pUrlFile->initRootNode(vRoot))
			{
				Json::Value& vRootNode = vRoot["Web"];
				if (vRootNode.isArray())
				{
					Json::Value vNode;
					for (size_t i = 0; i < vRootNode.size(); ++i)
					{
						vNode = vRootNode[i];
						if (_tcscmp(lpDownName, vNode["name"].asCString()) == 0)
						{
							LPCTSTR lpDownloadWeb = vNode["url"].asCString();
							::ShellExecute(NULL, _T("open"), lpDownloadWeb, NULL, NULL, SW_SHOWNORMAL);
						}
					}
				}
			}

			this->Close();
		}
	}

	//__super::Notify(msg);
}

void MainFrame::InitDownload()
{
	g_Logger->TryInfo(_T("MainFrame - InitDownload"));

	// 初始化下载UI提示
	do
	{
		m_pLabTotal->SetText(_T("正在加载中……"));
		m_pLabSpeed->SetText(_T("0B/s"));
	} while (0);

	// 解析下载地址
	do
	{
		Json::Value vRoot;
		JsonFile* pFile = new JsonFile(_T("WEB"));
		bool bRes = pFile->initRootNode(vRoot);
		SAFE_DELETE(pFile);
		if (bRes)
		{
			Json::Value vDownloadConfig = vRoot["Web"];
			if (vDownloadConfig.isArray())
			{
				for (int i = 0; i < vDownloadConfig.size(); ++i)
				{
					Json::Value& vNode = vDownloadConfig[i];
					if (_tcscmp(_T("Download"), vNode["name"].asCString()) == 0)
					{
						std::string strTmp = vNode["url"].asString();
						m_strUrl.Format(_T("%s%s"), strTmp.c_str(), m_strUrl.GetData());
					}
					
				}
			}
		}
		else
		{
			UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), _T("下载配置失败!"), MB_OK);
			switch (uEventID)
			{
				case IDOK:
				case IDCLOSE:
				default:
				{
					this->Close();
				}break;
			}
			return;
		}
	} while (0);
	
	// 请求下载
	do
	{
		if (!m_pDownloader)
		{
			m_pDownloader = new CLibcurlDownloader(&m_PaintManager);
		}
		if (m_pDownloader->InitOption())
		{
			CDuiString pzFileName = _T("GamePlaza.tmp");
			CDuiString pzFilePath = m_PaintManager.GetInstancePath() + _T("Update");
			CDuiString pzUrl = m_strUrl;

			// 请求下载文件大小
			m_pDownloader->ReqDownloadSize(pzFileName.GetData(), pzFilePath.GetData(), pzUrl.GetData(), this);
		}
		else
		{
			// 初始化失败
			DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), _T("初始化失败!"), MB_OK);
		}
	} while (0);

	g_Logger->TryInfo(_T("MainFrame - InitDownload Complete"));
}

void MainFrame::OnOnLibcurDownloadSize(double dlSize)
{
	g_Logger->TryInfo(_T("MainFrame - Download FileSize = %f"), dlSize);

	if (dlSize > 0)
	{
		g_Logger->TryInfo(_T("MainFrame - Downloading..."));

		// 开始下载
		m_pDownloader->StartDownLoad();
	}
	else
	{
		UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), _T("客户端版本过旧，请前往官网下载!"), MB_OK);
		switch (uEventID)
		{
			case IDOK:
			case IDCLOSE:
			default:
			{
				CControlUI* pControl = m_PaintManager.FindControl(R->kBtnUpdatePlaza_Download);
				if (pControl)
				{
					m_PaintManager.SendNotify(pControl, DUI_MSGTYPE_CLICK);
				}
			}break;
		}
	}
}

void MainFrame::OnLibcurlProgress(const double& dltotal, const double& dlnow)
{
	double dl_now = dlnow;
	double dl_total = dltotal;

	std::string pzDesc_Now;
	m_pDownloader->CalculateByteDesc(dl_now, pzDesc_Now);

	std::string pzDesc_Total;
	m_pDownloader->CalculateByteDesc(dl_total, pzDesc_Total);

	// 总大小
	_stprintf_s(m_pzBuff, _T("%s/%s"), pzDesc_Now.c_str(), pzDesc_Total.c_str());
	m_pLabTotal->SetText(m_pzBuff);

	// 进度条
	UINT uiMaxPercent = 100;
	m_pProgress->SetMaxValue(uiMaxPercent);
	double dlPercent = (dlnow / dltotal) * uiMaxPercent;
	m_pProgress->SetValue(dlPercent);
}

void MainFrame::OnLibcurlSpeed(const double& dlspeed)
{
	double speed = dlspeed;
	std::string pzDesc;
	m_pDownloader->CalculateByteDesc(speed, pzDesc);
	
	_stprintf_s(m_pzBuff, _T("%s/s"), pzDesc.c_str());
	m_pLabSpeed->SetText(m_pzBuff);
}

void MainFrame::OnLibcurlComplete()
{
	g_Logger->TryInfo(_T("MainFrame - Download Complete"));

	// 进度条UI更新
	do
	{
		// 禁用面板事件
		this->SetFrozenEnable(false);

		m_pProgress->SetValue(m_pProgress->GetMaxValue());
		m_pLabTotal->SetText(_T("正在安装,请勿关闭……"));
		m_pLabSpeed->SetText(_T("下载完成"));
	} while (0);

	g_Logger->TryInfo(_T("MainFrame - Install..."));
	m_pDownloader->StartInstall();
}

void MainFrame::OnInstallComplete(int iResCode)
{
	g_Logger->TryInfo(_T("MainFrame - Installed Complete [code = %d]"), iResCode);

	// 恢复面板事件
	this->SetFrozenEnable(true);

	if (ZlibUtil::ZlibUtil_Err_Code::ZLIB_ERR_NO != iResCode)
	{
		_stprintf_s(m_pzBuff, _T("%s(%d)"), _T("安装失败:"), iResCode);
		UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), m_pzBuff, MB_OK);
		switch (uEventID)
		{
			case IDOK:
			case IDCLOSE:
			default:
			{
				this->Close();
			}break;
		}
	}
	else
	{
		g_Logger->TryInfo(_T("MainFrame - Installed OK"));
		m_pLabTotal->SetText(_T("安装完成!"));

		// 启动大厅
		CDuiString pzGamePlazaPath = m_PaintManager.GetInstancePath() + _T("GamePlaza.exe");
		_stprintf_s(m_pzBuff, _T("/Transmittal_Begin[%I64d]/Transmittal_End"), (INT64)m_hWnd);
		
		this->StartGamePlaza(pzGamePlazaPath.GetData(), m_pzBuff);
	}
}

void MainFrame::StartGamePlaza(LPCSTR lpApplicationName, LPSTR lpCommandLine)
{
	g_Logger->TryInfo(_T("MainFrame - Start GamePlaza..."));

	// 启动大厅进程
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOWMAXIMIZED;
	PROCESS_INFORMATION	tProcessInfo;	// 游戏进程信息
	ZeroMemory(&tProcessInfo, 0, sizeof(tProcessInfo));
	BOOL bSuccess = ::CreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &tProcessInfo);
	if (TRUE == bSuccess)
	{
		this->Close();
	}
	else
	{
		TCHAR pzBuff[MAX_PATH] = { 0 };
		_stprintf_s(pzBuff, _T("%s(%d)"), _T("启动大厅失败!"), (int)GetLastError());
		UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), pzBuff, MB_OK);
		switch (uEventID)
		{
			case IDOK:
			case IDCLOSE:
			default:
			{
				this->Close();
			}break;
		}
	}
}

void MainFrame::SetFrozenEnable(bool bEnable)
{
	//CContainerUI* pRoot = static_cast<CContainerUI*>(this->m_PaintManager.GetRoot());
	//if (pRoot)
	//{
	//	pRoot->SetEnabled(bEnable);
	//	pRoot->SetMouseEnabled(bEnable);
	//	pRoot->SetMouseChildEnabled(bEnable);
	//}

	if (this->m_pBtnDownload)	this->m_pBtnDownload->SetEnabled(bEnable);
	if (this->m_pBtnCancel)		this->m_pBtnCancel->SetEnabled(bEnable);
	if (this->m_pBtnClose)		this->m_pBtnClose->SetEnabled(bEnable);
}

void MainFrame::OnLibcurlFailed(DownLoadStruct* pDownloadStruct)
{
	if (!pDownloadStruct) return;

	g_Logger->TryInfo(_T("MainFrame - OnLibcurlFailed [code = %d]"), pDownloadStruct->eCode);

	switch (pDownloadStruct->eCode)
	{
		case eDownloadStatus::E_DOWNLOAD_STATUS_NONE:
			break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_CREATE_DIR_FAILED:
		case eDownloadStatus::E_DOWNLOAD_STATUS_OPENFILE_FAILED:
		case eDownloadStatus::E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED:
		case eDownloadStatus::E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED:
		{
			g_Logger->TryInfo(_T("MainFrame - Read Write File Failed"));

			_stprintf_s(m_pzBuff, _T("%s(%d)"), _T("文件执行失败!"), pDownloadStruct->eCode);
			UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), m_pzBuff, MB_RETRYCANCEL);
			switch (uEventID)
			{
				case IDRETRY:
				{
					m_pDownloader->ContinueDownload();
				}break;

				case IDCLOSE:
				default:
				{
					this->Close();
				}break;
			}
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_ABORT:			// 终止下载
		{
			g_Logger->TryInfo(_T("MainFrame - Download Abort"));
			this->Close();
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_CONTINUE_TRANSFER:		// 继续下载
		{
			g_Logger->TryInfo(_T("MainFrame - Download Continue"));

			DWORD dwLocalFileSize = pDownloadStruct->dlLastSize;
			UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), _T("是否继续上次未完成的下载?"), MB_OKCANCEL);
			switch (uEventID)
			{
				case IDOK:
				{
					if (dwLocalFileSize >= pDownloadStruct->dlTotalSize)
					{
						this->OnLibcurlComplete();
					}
					else
					{
						m_pDownloader->ContinueDownload();
					}
				}break;

				case IDCANCEL:
				{
					m_pDownloader->ReDownLoad();
				}break;

				case IDCLOSE:
				default:
				{
					this->Close();
				}break;
			}
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_FAILED:		// 下载失败
		{
			CURLcode dwCurlCode = (CURLcode)pDownloadStruct->eCode2;
			LPCTSTR	lpCaption = _T("下载出错!");

			g_Logger->TryInfo(_T("MainFrame - Download Failed [CURLcode = %d]"), dwCurlCode);

			if (dwCurlCode >= 5 && dwCurlCode <= 9)
			{
				lpCaption = _T("网络异常");
			}
			else if (dwCurlCode == 42)
			{
				lpCaption = _T("由于网络不稳定，下载被中断");
			}
			else if (dwCurlCode >= 55 && dwCurlCode <= 65)
			{
				lpCaption = _T("网络异常");
			}

			// 处理对话框事件
			_stprintf_s(m_pzBuff, _T("%s(%d)"), lpCaption, dwCurlCode);
			UINT uEventID = DialogFrame::MsgBox(m_hWnd, _T("温馨提示"), m_pzBuff, MB_RETRYCANCEL);
			switch (uEventID)
			{
				case IDRETRY:
				{
					// 点击重试则,继续下载
					m_pDownloader->ContinueDownload();

					// 断网
					if (CURLE_COULDNT_RESOLVE_HOST == dwCurlCode)
					{
						// do sth
					}
				}break;

				case IDCANCEL:
				case IDCLOSE:
				default:
				{
					this->Close();
				}break;
			}
		}break;

		default:
			break;
	}
}
