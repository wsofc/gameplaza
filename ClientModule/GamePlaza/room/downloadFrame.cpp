#include "stdafx.h"
#include "downloadFrame.h"
#include "..\\hall\hallFrame.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

// -------------------------------------------------------------------------------------------------
// 下载任务
bool DownloadTask::g_bOnceBlock = false;
DownloadTask::DownloadTask(DownloadFrame* pOwnerFrame, DWORD dwIndex, tagGameKind* pGameKind)
	: m_dwIndex(0)
	, m_pTaskCtl(nullptr)
	, m_pGameKind(nullptr)
	, m_pLabName(nullptr)
	, m_pLabSize(nullptr)
	, m_pLabStatus(nullptr)
	, m_pLabSpeed(nullptr)
	, m_pLabPercent(nullptr)
	, m_pProgress(nullptr)
	, m_pBtnCancel(nullptr)
	, m_pDownloader(nullptr)
	, m_pOwnerFrame(nullptr)
{
	memset(m_pzBuff, 0, MAX_PATH * sizeof(TCHAR));

	m_pOwnerFrame = pOwnerFrame;
	m_dwIndex = dwIndex;

	m_pGameKind = new tagGameKind();
	memcpy(m_pGameKind, pGameKind, sizeof(tagGameKind));

	g_Logger->TryInfo(_T("DownloadTask - new Task [index = %lu, game_id = %u, type_id = %u]"), dwIndex, m_pGameKind->wKindID, m_pGameKind->wTypeID);

	// 创建下载任务控件
	CDialogBuilder builder;
	CPaintManagerUI* pPaintManager = &m_pOwnerFrame->GetPaintManager();
	CControlUI* pTaskCtl = static_cast<CControlUI*>(builder.Create(_T("Update/task_item.xml"), (UINT)0, nullptr, pPaintManager));
	if (!pTaskCtl) return;

	int ipos_x = 0;
	int ipos_y = 65;
	ipos_y += (pTaskCtl->GetFixedHeight() + 10) * m_dwIndex;
	pTaskCtl->SetFloat(true);
	pTaskCtl->SetFixedXY({ ipos_x, ipos_y });

	m_pTaskCtl		= pTaskCtl;
	m_pLabName		= static_cast<CLabelUI*>(pPaintManager->FindSubControlByName(pTaskCtl,		R->kLabUpdateItem_Name));
	m_pLabPercent	= static_cast<CLabelUI*>(pPaintManager->FindSubControlByName(pTaskCtl,		R->kLabUpdateItem_Percent));
	m_pLabSize		= static_cast<CLabelUI*>(pPaintManager->FindSubControlByName(pTaskCtl,		R->kLabUpdateItem_Size));
	m_pLabStatus	= static_cast<CLabelUI*>(pPaintManager->FindSubControlByName(pTaskCtl,		R->kLabUpdateItem_Status));
	m_pLabSpeed		= static_cast<CLabelUI*>(pPaintManager->FindSubControlByName(pTaskCtl,		R->kLabUpdateItem_Speed));
	m_pProgress		= static_cast<CProgressUI*>(pPaintManager->FindSubControlByName(pTaskCtl,	R->kProUpdateItem_Progress));
	m_pBtnCancel	= static_cast<CButtonUI*>(pPaintManager->FindSubControlByName(pTaskCtl,		R->kBtnUpdateItem_Cancel));

	// 重置取消按钮名字
	_stprintf_s(m_pzBuff, _T("%s_%lu"), R->kBtnUpdateItem_Cancel, dwIndex);
	m_pBtnCancel->SetName(m_pzBuff);

	this->Init();
}

DownloadTask::~DownloadTask()
{
	SAFE_DELETE(m_pGameKind);
	SAFE_DELETE(m_pDownloader);
}

DWORD DownloadTask::GetTaskIndex()
{
	return m_dwIndex;
}

WORD DownloadTask::GetGameKind()
{
	return m_pGameKind->wKindID;
}

CControlUI* DownloadTask::GetTaskControl()
{
	return m_pTaskCtl;
}

void DownloadTask::ForwardIndex(DWORD dwDis /* = 1 */)
{
	int ipos_x = 0;
	int ipos_y = 65;
	m_dwIndex -= dwDis;
	m_dwIndex = (m_dwIndex == 0) ? 0 : m_dwIndex;
	ipos_y += (m_pTaskCtl->GetFixedHeight() + 10) * m_dwIndex;
	m_pTaskCtl->SetFloat(true);
	m_pTaskCtl->SetFixedXY({ ipos_x, ipos_y });

	_stprintf_s(m_pzBuff, _T("%s_%lu"), R->kBtnUpdateItem_Cancel, m_dwIndex);
	m_pBtnCancel->SetName(m_pzBuff);
}

void DownloadTask::Init()
{
	// 游戏名
	m_pLabName->SetText(m_pGameKind->szKindName);

	// 进度
	m_pProgress->SetValue(0);
	m_pProgress->SetMaxValue(100);

	// 大小
	m_pLabSize->SetText(_T("- / -"));

	// 速度
	m_pLabSpeed->SetText(_T("- /s"));

	// 状态描述
	m_pLabStatus->SetText(_T("获取中..."));

	g_Logger->TryInfo(_T("DownloadTask - new Task Complete [game_id = %u, type_id = %u]"), m_pGameKind->wKindID, m_pGameKind->wTypeID);
}

void DownloadTask::Download()
{
	g_Logger->TryInfo(_T("DownloadTask - [game_id = %d] Download Init"), m_pGameKind->wKindID);

	// 开始下载
	CPaintManagerUI* pPaintManager = &m_pOwnerFrame->GetPaintManager();
	if (!m_pDownloader)
	{
		m_pDownloader = new CLibcurlDownloader(pPaintManager);
	}
	if (m_pDownloader->InitOption())
	{
		CDuiString pzFileName;
		CDuiString pzFilePath;
		CDuiString pzUrl;
		CDuiString pzTemp;

		// 文件名
		pzTemp.Format(_T("Game_%u_V_%lu"), m_pGameKind->wKindID, m_pGameKind->dwMaxVersion);
		pzFileName = pzTemp + _T(".tmp");

		// 下载路径
		pzFilePath.Format(_T("%sUpdate"), pPaintManager->GetInstancePath().GetData());

		// 下载url
		pzUrl.Format(_T("%s%s"), g_GameUtil->GetWebByName(_T("Download")), pzTemp.GetData());
		
		g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Download Version = %lu"), m_pGameKind->wKindID, m_pGameKind->dwMaxVersion);
		g_Logger->TryDebug(_T("DownloadTask - [game_id = %u] Download URL = %s"), m_pGameKind->wKindID, pzUrl.GetData());

		// 请求下载文件大小
		m_pDownloader->ReqDownloadSize(pzFileName.GetData(), pzFilePath.GetData(), pzUrl.GetData(), this);
	}
	else
	{
		// 初始化失败
		DialogFrame::MsgBox(pPaintManager, _T("温馨提示"), _T("初始化失败!"));
	}
}

void DownloadTask::StopDownload()
{
	m_pDownloader->AbortDownload();
}

void DownloadTask::OnOnLibcurDownloadSize(double dlSize)
{
	g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Download FileSize = %f"), m_pGameKind->wKindID, dlSize);
	if (dlSize > 0)
	{
		g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Downloading"), m_pGameKind->wKindID);

		// 开始下载
		m_pLabStatus->SetText(_T("正在下载"));
		m_pDownloader->StartDownLoad();
	}
	else
	{
		// 请求下载文件大小,阻塞时
		// 必须等待线程关闭才能删除下载任务(一般超时30s左右)
		// 所有下载任务删除时才能关闭下载窗口
		if (DownloadTask::g_bOnceBlock)
		{
			m_pDownloader->AbortDownload();
		}
		// 请求下载文件大小,不阻塞时
		else
		{
			_stprintf_s(m_pzBuff, _T("【%s】%s"), m_pGameKind->szKindName, _T("版本过旧，请前往官网下载!"));
			UINT uEventID = DialogFrame::MsgBox(m_pOwnerFrame->GetHWND(), _T("温馨提示"), m_pzBuff, MB_OK);
			switch (uEventID)
			{
				case IDOK:
				case IDCLOSE:
				default:
				{
					CPaintManagerUI& paintManager = m_pOwnerFrame->GetPaintManager();
					CControlUI* pControl = paintManager.FindControl(R->kBtnUpdateGame_Download);
					if (pControl)
					{
						paintManager.SendNotify(pControl, DUI_MSGTYPE_CLICK);
					}
					m_pOwnerFrame->RemoveTaskItem(m_dwIndex);
				}break;
			}
		}
	}
}

void DownloadTask::OnLibcurlProgress(const double& dltotal, const double& dlnow)
{
	double dl_now = dlnow;
	double dl_total = dltotal;

	std::string pzDesc_Now;
	m_pDownloader->CalculateByteDesc(dl_now, pzDesc_Now);

	std::string pzDesc_Total;
	m_pDownloader->CalculateByteDesc(dl_total, pzDesc_Total);

	// 总大小
	_stprintf_s(m_pzBuff, _T("%s/%s"), pzDesc_Now.c_str(), pzDesc_Total.c_str());
	m_pLabSize->SetText(m_pzBuff);

	// 百分比
	UINT uiMaxPercent = 100;
	double dlPercent = (dlnow / dltotal) * uiMaxPercent;
	if (dlPercent >= uiMaxPercent)
	{
		_stprintf_s(m_pzBuff, _T("%s"), _T("100%"));
	}
	else
	{
		_stprintf_s(m_pzBuff, _T("%.1f%s"), dlPercent, _T("%"));
	}
	m_pLabPercent->SetText(m_pzBuff);

	// 进度
	m_pProgress->SetValue(dlPercent);
}

void DownloadTask::OnLibcurlSpeed(const double& dlspeed)
{
	double speed = dlspeed;
	std::string pzDesc;
	m_pDownloader->CalculateByteDesc(speed, pzDesc);

	_stprintf_s(m_pzBuff, _T("%s/s"), pzDesc.c_str());
	m_pLabSpeed->SetText(m_pzBuff);
}

void DownloadTask::OnLibcurlComplete()
{
	g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Download OK"), m_pGameKind->wKindID);

	// 进度条UI更新
	do
	{
		m_pProgress->SetValue(m_pProgress->GetMaxValue());
		m_pLabPercent->SetText(_T("100%"));
		m_pLabStatus->SetText(_T("正在安装"));
		m_pLabSpeed->SetText(_T("下载完成"));
	} while (0);

	g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Installing..."), m_pGameKind->wKindID);
	m_pDownloader->StartInstall();
}

void DownloadTask::OnInstallComplete(int iResCode)
{
	g_Logger->TryInfo(_T("DownloadTask - Installed Complete [code = %d]"), iResCode);

	if (ZlibUtil::ZlibUtil_Err_Code::ZLIB_ERR_NO != iResCode)
	{
		g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Installed Failed [code = %d]"), m_pGameKind->wKindID, iResCode);
		_stprintf_s(m_pzBuff, _T("%s(%d)"), _T("安装失败:"), iResCode);
		UINT uEventID = DialogFrame::MsgBox(m_pOwnerFrame->GetHWND(), _T("温馨提示"), m_pzBuff, MB_OK);
	}
	else
	{
		g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Installed OK"), m_pGameKind->wKindID);

		m_pLabStatus->SetText(_T("安装完成"));

		// 更新本地游戏版本
		ServiceData* pServerListData = ServiceData::GetInstance();
		pServerListData->SetLocalGameVersion(m_pGameKind->wKindID, m_pGameKind->dwMaxVersion);

		// 更新游戏列表容器版本
		HallFrame* pHallFrame = HallFrame::GetInstance();
		CGameItemUI* pGameItem = pHallFrame->FindGameItemByKindID(m_pGameKind->wKindID);
		pGameItem->SetMaxVersion(m_pGameKind->dwMaxVersion);
		pGameItem->SetGameStatus(CGameItemUI::GAME_STATE::GAME_STATE_VERSION_NORMAL);

		// 通知所属窗口处理
		m_pOwnerFrame->DealCompleteItem(m_dwIndex);
	}

	// 删除本条任务
	m_pOwnerFrame->RemoveTaskItem(m_dwIndex);
}

void DownloadTask::OnLibcurlFailed(DownLoadStruct* pDownloadStruct)
{
	if (!pDownloadStruct) return;
	g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] OnLibcurlFailed [code = %d]"), m_pGameKind->wKindID, pDownloadStruct->eCode);

	switch (pDownloadStruct->eCode)
	{
		case eDownloadStatus::E_DOWNLOAD_STATUS_NONE:
			break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_CREATE_DIR_FAILED:
		case eDownloadStatus::E_DOWNLOAD_STATUS_OPENFILE_FAILED:
		case eDownloadStatus::E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED:
		case eDownloadStatus::E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED:
		{
			g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Read Write File Failed"), m_pGameKind->wKindID);

			_stprintf_s(m_pzBuff, _T("%s(%d)"), _T("文件执行失败!"), pDownloadStruct->eCode);
			UINT uEventID = DialogFrame::MsgBox(m_pOwnerFrame->GetHWND(), _T("温馨提示"), m_pzBuff, MB_RETRYCANCEL);
			switch (uEventID)
			{
				case IDRETRY:
				{
					m_pDownloader->ContinueDownload();
				}break;

				case IDCLOSE:
				default:
				{
					m_pOwnerFrame->RemoveTaskItem(m_dwIndex);
				}break;
			}
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_REQ_SIZE_BLOCK:			// 请求获取下载文件大小线程阻塞时,必须先关闭子线程,才能关闭主线程窗口
		{
			// 多个请求阻塞时,只弹出一个提示
			if (!DownloadTask::g_bOnceBlock)
			{
				DownloadTask::g_bOnceBlock = true;

				int iMsgBoxID = DownloadFrame::MSG_BOX_ID_ONCEBLOCK;
				DialogCallBack dlg_cb = CC_CALLBACK_2(DownloadFrame::MsgBoxProcFocus, m_pOwnerFrame);
				DialogFrame::MsgBox(&m_pOwnerFrame->GetPaintManager(), _T("温馨提示"), _T("正在关闭,请稍后...")
					, (DLG_ID)iMsgBoxID
					, DLG_STYLE::DLG_CANCEL
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_ABORT:			// 终止下载
		{
			g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Download Abort"), m_pGameKind->wKindID);
			m_pOwnerFrame->RemoveTaskItem(m_dwIndex);
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_CONTINUE_TRANSFER:		// 继续下载
		{
			g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Download Continue"), m_pGameKind->wKindID);
			DWORD dwLocalFileSize = pDownloadStruct->dlLastSize;
			_stprintf_s(m_pzBuff, _T("【%s】%s"), m_pGameKind->szKindName, _T("是否继续上次未完成的下载?"));
			UINT uEventID = DialogFrame::MsgBox(m_pOwnerFrame->GetHWND(), _T("温馨提示"), m_pzBuff, MB_OKCANCEL);
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
					m_pOwnerFrame->RemoveTaskItem(m_dwIndex);
				}break;
			}
		}break;

		case eDownloadStatus::E_DOWNLOAD_STATUS_DOWNLOAD_FAILED:		// 下载失败
		{
			CURLcode dwCurlCode = (CURLcode)pDownloadStruct->eCode2;
			LPCTSTR	lpCaption = _T("下载出错!");

			g_Logger->TryInfo(_T("DownloadTask - [game_id = %u] Download Failed [CURLcode = %d]"), m_pGameKind->wKindID, dwCurlCode);

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
			UINT uEventID = DialogFrame::MsgBox(m_pOwnerFrame->GetHWND(), _T("温馨提示"), m_pzBuff, MB_RETRYCANCEL);
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
				{
					m_pDownloader->ReDownLoad();
				}break;

				case IDCLOSE:
				default:
				{
					m_pOwnerFrame->RemoveTaskItem(m_dwIndex);
				}break;
			}
		}break;

		default:
			break;
	}
}

// -------------------------------------------------------------------------------------------------
// 下载游戏窗口
DownloadFrame* DownloadFrame::g_instance = nullptr;
DownloadFrame::DownloadFrame()
	: m_pRootCtl(nullptr)
	, m_pParentManager(nullptr)
{
	memset(&m_tGameKind, 0, sizeof(tagGameKind));
}

DownloadFrame::~DownloadFrame()
{
	// 若还有任务列表，则清除UI列表
	for (std::list<DownloadTask*>::iterator& it = m_vDownloadTask.begin(); it != m_vDownloadTask.end();)
	{
		DownloadTask* pTask = (*it);
		SAFE_DELETE(pTask);

		it = m_vDownloadTask.erase(it);
		++it;
	}

	g_GameUtil->SetFrozenEnable(m_pParentManager, true);
	g_GameUtil->ShowTopWindow(m_pParentManager->GetPaintWindow());

	g_Logger->TryInfo(_T("DownloadFrame - Destroy"));
}

DownloadFrame* DownloadFrame::GetInstance()
{
	if (g_instance == NULL)
	{
		g_instance = new DownloadFrame();
	}

	return g_instance;
}

BOOL DownloadFrame::CreateFrame(CPaintManagerUI* pParentManager, const tagGameKind* pGameKind)
{
	ASSERT(pParentManager);
	ASSERT(pGameKind);
	if (!pParentManager || !pGameKind) return FALSE;

	m_pParentManager = pParentManager;
	memcpy(&m_tGameKind, pGameKind, sizeof(tagGameKind));

	if (FALSE == ::IsWindow(m_hWnd))
	{
		HWND hParent = m_pParentManager->GetPaintWindow();
		if (FALSE == ::IsWindow(hParent)) return FALSE;
		//g_GameUtil->SetFrozenEnable(m_pParentManager, false);

		m_hWnd = this->Create(hParent, NULL, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->CenterWindow();
		this->ShowWindow(true);
		this->AddTaskItem();
	}

	return TRUE;
}

void DownloadFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("DownloadFrame - Close"));
	WindowImplBase::OnFinalMessage(hWnd);
	SAFE_DELETE(DownloadFrame::g_instance);
}

LRESULT DownloadFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_MY_LIBCURL:
		{
			DownLoadStruct* pDownloadStruct = (DownLoadStruct*)wParam;
			if (pDownloadStruct && pDownloadStruct->pDelegate)
			{
				for (const auto& it : m_vDownloadTask)
				{
					if (it == pDownloadStruct->pDelegate)
					{
						it->OnLibcurlFailed(pDownloadStruct);
						break;
					}
				}
			}
			return TRUE;
		}break;

		default:
			break;
	}

	return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

UILIB_RESOURCETYPE DownloadFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR DownloadFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString DownloadFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString DownloadFrame::GetSkinFile()
{
	return _T("Update/update_game.xml");
}

CControlUI* DownloadFrame::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
		return new CWkeWebkitUI();

	return NULL;
}

void DownloadFrame::InitWindow()
{
	g_Logger->TryInfo(_T("DownloadFrame - Open"));
	m_pRootCtl = static_cast<CContainerUI*>(m_PaintManager.FindControl(R->kCtnUpdateGame_TaskMap));
}

void DownloadFrame::InitData(TNotifyUI& msg)
{
	this->AddTaskItem();
	g_Logger->TryInfo(_T("DownloadFrame - Open Complete"));
}

void DownloadFrame::AddTaskItem()
{
	g_Logger->TryInfo(_T("DownloadFrame - AddTask"));
	if (!m_pRootCtl) return;
	
	if (m_vDownloadTask.size() < DOWNLOAD_TASK_MAX_COUNT)
	{
		// 如果任务已存在,直接返回
		for (const auto& it : m_vDownloadTask)
		{
			if (m_tGameKind.wKindID == it->GetGameKind())
			{
				return;
			}
		}

		// 添加到下载任务列表
		DWORD dwIndex = m_vDownloadTask.size();
		DownloadTask* pDownloadTask = new DownloadTask(this, dwIndex, &m_tGameKind);
		m_vDownloadTask.push_back(pDownloadTask);

		g_Logger->TryInfo(_T("DownloadFrame - AddTask Complete [index = %lu]"), dwIndex);

		CControlUI* pTaskCtl = pDownloadTask->GetTaskControl();
		if (pTaskCtl)
		{
			m_pRootCtl->Add(pTaskCtl);
		}

		// 开始下载
		pDownloadTask->Download();
	}
	else
	{
		DialogFrame::MsgBox(&m_PaintManager, _T("温馨提示"), _T("下载任务过多!"));
	}
}

void DownloadFrame::RemoveTaskItem(DWORD dwIndex)
{
	g_Logger->TryInfo(_T("DownloadFrame - RemoveTask [index = %lu]"), dwIndex);
	CContainerUI* pRootCtl = static_cast<CContainerUI*>(m_PaintManager.FindControl(R->kCtnUpdateGame_TaskMap));
	if (!pRootCtl) return;

	bool bMove = false;
	std::list<DownloadTask*>::iterator it_temp = m_vDownloadTask.end();
	for (std::list<DownloadTask*>::iterator it = m_vDownloadTask.begin(); it != m_vDownloadTask.end(); ++it)
	{
		if (dwIndex == (*it)->GetTaskIndex())
		{
			bMove = true;
			it_temp = it;
		}

		// 控件上移一个索引
		if (bMove)
		{
			(*it)->ForwardIndex(1);
		}
	}

	// 容器删除该元素
	if (it_temp != m_vDownloadTask.end())
	{
		pRootCtl->Remove((*it_temp)->GetTaskControl());
		SAFE_DELETE(*it_temp);

		m_vDownloadTask.erase(it_temp);
	}

	// 容器为空，则关闭窗口
	if (m_vDownloadTask.size() <= 0)
	{
		g_Logger->TryInfo(_T("DownloadFrame - RemoveTask All Done"));

		DownloadTask::ResetOnceBlock();
		this->Close();
	}
}

void DownloadFrame::RemoveAllTaskItem()
{
	g_Logger->TryInfo(_T("DownloadFrame - RemoveAllTask"));
	for (const auto& it : m_vDownloadTask)
	{
		it->StopDownload();
	}
}

void DownloadFrame::DealCompleteItem(DWORD dwIndex)
{
	g_Logger->TryInfo(_T("DownloadFrame - DealedTask [index = %lu]"), dwIndex);
	for (const auto& it : m_vDownloadTask)
	{
		if (dwIndex == it->GetTaskIndex())
		{
			// 已全部下载完成
			// 通知主线程更新UI
			if (m_vDownloadTask.size() <= 1)
			{
				WORD dwGameKindID = it->GetGameKind();
				::PostMessage(m_pParentManager->GetPaintWindow(), DOWNLOAD_ENTER_ROOM, (WPARAM)dwGameKindID, 0);
			}
		}
	}
}

void DownloadFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case DownloadFrame::MSG_BOX_ID_ONCEBLOCK:
		{
			DownloadTask::ResetOnceBlock();
		}break;

		default:
			break;
	}
}

LRESULT DownloadFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

void DownloadFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnUpdateGame_Close) == 0 || _tcscmp(sName, R->kBtnUpdateGame_Cancel) == 0)
		{
			if (m_vDownloadTask.size() > 0)
			{
				this->RemoveAllTaskItem();
			}
			else
			{
				this->Close();
			}
		}
		else if (_tcscmp(sName, R->kBtnUpdateGame_Download) == 0)
		{
			g_GameUtil->OpenWebByName(_T("Official"));
		}

		if (m_vDownloadTask.size() > 0)
		{
			int iPos = sName.Find(R->kBtnUpdateItem_Cancel);
			if (iPos != -1)
			{
				iPos = sName.GetLength() - _tcslen(R->kBtnUpdateItem_Cancel) - 1;
				CDuiString pzIndex = sName.Right(iPos);
				int iIndex = _ttoi(pzIndex.GetData());
				for (const auto& it : m_vDownloadTask)
				{
					if (it->GetTaskIndex() == iIndex)
					{
						it->StopDownload();
					}
				}
			}
		}
	}

	//__super::Notify(msg);
}