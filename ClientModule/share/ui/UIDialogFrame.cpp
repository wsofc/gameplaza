#include "stdafx.h"
#include "UIDialogFrame.h"

LPCTSTR MsgBoxWnd::g_kBtnDialog_Ensure			= _T("dialog-btn_ensure");									//确定
LPCTSTR MsgBoxWnd::g_kBtnDialog_Retry			= _T("dialog-btn_retry");									//重试
LPCTSTR MsgBoxWnd::g_kBtnDialog_Cancel			= _T("dialog-btn_cancel");									//取消
LPCTSTR MsgBoxWnd::g_kBtnDialog_Close			= _T("dialog-btn_close");									//关闭
LPCTSTR MsgBoxWnd::g_kTxtDialog_Title			= _T("dialog-txt_title");									//标题文本
LPCTSTR MsgBoxWnd::g_kEdtDialog_Content			= _T("dialog-edt_content");									//内容文本
LPCTSTR MsgBoxWnd::g_kTxtDialog_Toast			= _T("toast-text_content");									//内容文本
LPCTSTR MsgBoxWnd::g_kCtl_FadeLayer				= _T("waitting-fade_layer");								//遮挡层

WndTimmer DialogFrame::g_wndTimmer;

std::map<DialogFrame*, MsgBoxWnd*> DialogFrame::g_MsgBoxMap;
std::map<UINT_PTR, DialogFrame*> DialogFrame::g_ToastMap;
std::map<UINT_PTR, DialogFrame*> DialogFrame::g_WattingMap;

// -------------------------------------- 对话窗口 --------------------------------------
MsgBoxWnd::MsgBoxWnd(HWND hParent, DLG_STYLE iStyle, LPCTSTR xmlFileStr)
	: m_bModel(false)
	, m_uEventID(0)
	, m_uMsgBoxID(DLG_ID::DLG_ID_NONE)
	, m_uType(iStyle)
	, m_pzXmlFile(xmlFileStr)
	, m_pTxtTitle(nullptr)
	, m_pEdtContent(nullptr)
	, m_pBtnEnsure(nullptr)
	, m_pBtnRetry(nullptr)
	, m_pBtnCancel(nullptr)
	, m_pEnsureFunc(nullptr)
	, m_pCancelFunc(nullptr)
	, m_pCloseFunc(nullptr)
{
	if (TRUE == ::IsWindow(hParent))
	{
		m_hWnd = this->Create(hParent, _T("MsgBoxWnd"), UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
}

MsgBoxWnd::~MsgBoxWnd()
{
}

void MsgBoxWnd::InitWindow()
{
	m_pTxtTitle		= static_cast<CTextUI*>(m_PaintManager.FindControl(g_kTxtDialog_Title));
	m_pEdtContent	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(g_kEdtDialog_Content));
	m_pBtnEnsure	= static_cast<CButtonUI*>(m_PaintManager.FindControl(g_kBtnDialog_Ensure));
	m_pBtnRetry		= static_cast<CButtonUI*>(m_PaintManager.FindControl(g_kBtnDialog_Retry));
	m_pBtnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(g_kBtnDialog_Cancel));
}

void MsgBoxWnd::InitData(TNotifyUI& msg)
{
	switch (m_uType)
	{
		case DLG_STYLE::DLG_NO_BTN:
		{
			m_pBtnEnsure->SetVisible(false);
			m_pBtnRetry->SetVisible(false);
			m_pBtnCancel->SetVisible(false);

			//内容文本向下偏移10像素
			DWORD iDif = 15;
			RECT rcItem = m_pEdtContent->GetPos();
			rcItem.top += iDif;
			rcItem.bottom += iDif;
			m_pEdtContent->SetPos(rcItem);
		}break;

		case DLG_STYLE::DLG_DEFAULT:
		case DLG_STYLE::DLG_OK:
		{
			m_pBtnEnsure->SetVisible(true);
			m_pBtnRetry->SetVisible(false);
			m_pBtnCancel->SetVisible(false);

			//按钮居中
			RECT rcItem = m_pBtnEnsure->GetPos();
			DWORD btn_w = m_pBtnEnsure->GetWidth();
			DWORD ctl_w = m_pBtnEnsure->GetParent()->GetWidth();
			DWORD pos_x = (ctl_w - btn_w) / 2;
			rcItem.left = pos_x;
			rcItem.right = rcItem.left + btn_w;
			m_pBtnEnsure->SetPos(rcItem);
			m_pBtnEnsure->SetText(m_pBtnEnsure->GetText());
		}break;

		case DLG_STYLE::DLG_CANCEL:
		{
			m_pBtnEnsure->SetVisible(false);
			m_pBtnRetry->SetVisible(false);
			m_pBtnCancel->SetVisible(true);

			//按钮居中
			RECT rcItem = m_pBtnCancel->GetPos();
			DWORD btn_w = m_pBtnCancel->GetWidth();
			DWORD ctl_w = m_pBtnCancel->GetParent()->GetWidth();
			DWORD pos_x = (ctl_w - btn_w) / 2;
			rcItem.left = pos_x;
			rcItem.right = rcItem.left + btn_w;
			m_pBtnCancel->SetPos(rcItem);
			m_pBtnCancel->SetText(m_pBtnCancel->GetText());
		}break;

		case DLG_STYLE::DLG_RETRY_CANCEL:
		{
			m_pBtnEnsure->SetVisible(false);
			m_pBtnRetry->SetVisible(true);
			m_pBtnCancel->SetVisible(true);

			m_pBtnRetry->SetText(m_pBtnRetry->GetText());
			m_pBtnCancel->SetText(m_pBtnCancel->GetText());
		}break;

		case DLG_STYLE::DLG_OK_CANCEL:
		{
			m_pBtnEnsure->SetVisible(true);
			m_pBtnRetry->SetVisible(false);
			m_pBtnCancel->SetVisible(true);
			
			m_pBtnEnsure->SetText(m_pBtnEnsure->GetText());
			m_pBtnCancel->SetText(m_pBtnCancel->GetText());
		}

		case DLG_STYLE::DLG_NONE:
		default:
			break;
	}
}

UINT MsgBoxWnd::ShowModal()
{
	m_bModel = true;
	return WindowImplBase::ShowModal();
}

void MsgBoxWnd::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	
	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(type, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, g_kBtnDialog_Close) == 0)
		{
			m_uEventID = IDCLOSE;

			if (m_pCloseFunc)
			{
				m_pCloseFunc(m_uMsgBoxID, msg);
			}
			this->Close();
		}
		else if (_tcscmp(sName, g_kBtnDialog_Ensure) == 0)
		{
			if (m_uType == DLG_STYLE::DLG_DEFAULT)
			{
				m_uEventID = IDOK;
			}
			else if(m_uType == DLG_STYLE::DLG_OK)
			{
				m_uEventID = IDOK;
			}
			else if (m_uType == DLG_STYLE::DLG_OK_CANCEL)
			{
				m_uEventID = IDOK;
			}
			else if (m_uType == DLG_STYLE::DLG_RETRY_CANCEL)
			{
				m_uEventID = IDRETRY;
			}

			this->ShowWindow(false);
			if (m_pEnsureFunc)
			{
				m_pEnsureFunc(m_uMsgBoxID, msg);
			}
			this->Close();
		}
		else if (_tcscmp(sName, g_kBtnDialog_Retry) == 0)
		{
			m_uEventID = IDRETRY;

			this->ShowWindow(false);
			if (m_pEnsureFunc)
			{
				m_pEnsureFunc(m_uMsgBoxID, msg);
			}
			this->Close();
		}
		else if (_tcscmp(sName, g_kBtnDialog_Cancel) == 0)
		{
			m_uEventID = IDCANCEL;

			this->ShowWindow(false);
			if (m_pCancelFunc)
			{
				m_pCancelFunc(m_uMsgBoxID, msg);
			}
			this->Close();
		}
	}
}

void MsgBoxWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	
	if (!this->m_bModel)
	{
		DialogFrame::EndMsgBox(this);
		delete this;
	}
}

void MsgBoxWnd::LayoutText(LPCTSTR pzTitle, LPCTSTR pzTextContent)
{
	if (m_pTxtTitle)
	{
		CDuiString pzTitleStr = _T("");
		pzTitleStr += pzTitle;
		m_pTxtTitle->SetText(pzTitleStr);
	}

	if (m_pEdtContent)
	{
		CDuiString pzContentStr = _T("");
		pzContentStr += pzTextContent;
		m_pEdtContent->SetText(pzContentStr);

		// 计算文字排版
		LONG lw = m_pEdtContent->GetFixedWidth();
		LONG lh = m_pEdtContent->GetFixedHeight();
		RECT rcText = { 0, 0, lw, lh };
		RECT rcPadding = { 2, 2, 2, 2 };
		CRenderEngine::DrawText(m_PaintManager.GetPaintDC(), &m_PaintManager, rcText, pzContentStr
			, m_pEdtContent->GetTextColor(), m_pEdtContent->GetFont(), DT_CALCRECT);

		// 文本居中计算
		LONG lTxt_w = rcText.right - rcText.left;
		LONG lTxt_h = rcText.bottom - rcText.top;
		if (lTxt_w < lw)
		{
			rcPadding.left = (lw - lTxt_w) / 2;
			rcPadding.right = 0;
		}
		
		if (lTxt_h < lh)
		{
			rcPadding.top = (lh - lTxt_h) / 2;
			rcPadding.bottom = 0;
		}
		m_pEdtContent->SetTextPadding(rcPadding);
		m_pEdtContent->HomeUp();
	}
}

void MsgBoxWnd::SetCallBack(DLG_ID iMsgBoxID
	, DialogCallBack pEnsureFunc /* = nullptr  */
	, DialogCallBack pCancelFunc /* = nullptr  */
	, DialogCallBack pCloseFunc /* = nullptr */)
{
	m_uMsgBoxID = iMsgBoxID;
	m_pEnsureFunc = pEnsureFunc;
	m_pCancelFunc = pCancelFunc;
	m_pCloseFunc = pCloseFunc;
}

CDuiString MsgBoxWnd::GetSkinFile()
{
	m_pzXmlFile.Format(_T("dialog\\%s.xml"), m_pzXmlFile.GetData());
	return m_pzXmlFile;
}

CDuiString MsgBoxWnd::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE MsgBoxWnd::GetResourceType() const
{
	return UILIB_RESOURCETYPE::UILIB_FILE;
}

LPCTSTR MsgBoxWnd::GetResourceID() const
{
	return _T("");
}

LPCTSTR MsgBoxWnd::GetWindowClassName() const
{
	return _T("MsgBoxWnd");
}

LRESULT MsgBoxWnd::ResponseDefaultKeyEvent(WPARAM wParam)
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

// -------------------------------------- 对话框类 --------------------------------------
DialogFrame::DialogFrame()
	: m_pRootControl(nullptr)
	, m_pSubControl(nullptr)
	, m_pManager(nullptr)
	, m_pCallBackFunc(nullptr)
	, m_iDlg_ID(DLG_ID::DLG_ID_NONE)
	, m_cbFade(0xFF)
{

}

DialogFrame::~DialogFrame()
{
}

void DialogFrame::SetFrozenEnable(bool bEnable)
{
	if (m_pRootControl)
	{
		if (!bEnable)
		{
			TCHAR pzBuff[MAX_PATH] = { 0 };
			_stprintf_s(pzBuff, _T("%d"), this);
			CControlUI* pShadeLayer = new CControlUI();
			RECT rc = m_pRootControl->GetPos();
			pShadeLayer->SetName(pzBuff);
			pShadeLayer->SetPos(rc);
			pShadeLayer->SetEnabled(true);
			pShadeLayer->SetVisible(true);
			pShadeLayer->SetMouseEnabled(true);
			pShadeLayer->SetKeyboardEnabled(false);
			m_pRootControl->Add(pShadeLayer);
		}
		else
		{
			TCHAR pzBuff[MAX_PATH] = { 0 };
			_stprintf_s(pzBuff, _T("%d"), this);
			for (int i = 0; i < m_pRootControl->GetCount(); ++i)
			{
				CControlUI* pCtl = m_pRootControl->GetItemAt(i);
				if (pCtl && _tcscmp(pCtl->GetName().GetData(), pzBuff) == 0)
				{
					m_pRootControl->Remove(pCtl);
					break;
				}
			}
		}
		//m_pRootControl->SetMouseEnabled(bEnable);
		//m_pRootControl->SetMouseChildEnabled(bEnable);
		//m_pRootControl->SetKeyboardEnabled(bEnable);
	}
}

void DialogFrame::SetCallBack(DLG_ID id, DialogCallBack pFunc)
{
	m_iDlg_ID = id;
	m_pCallBackFunc = pFunc;
}

void DialogFrame::SetPaintManager(CPaintManagerUI* pManager)
{
	m_pManager = pManager;
	if (m_pManager)
	{
		m_pRootControl = static_cast<CContainerUI*>(m_pManager->GetRoot());
	}
}

SIZE DialogFrame::EstimateSize(CPaintManagerUI* pManager, CContainerUI* pRoot, SIZE szAvailable)
{
	SIZE szRoot = { pRoot->GetFixedWidth(), pRoot->GetFixedHeight() };
	SIZE cXY = { 0, 0 };
	SIZE disXY = { 0 };
	SIZE fixed_xy = { 0 };
	for (int it = 0; it < pRoot->GetCount(); ++it)
	{
		CControlUI* pControl = static_cast<CControlUI*>(pRoot->GetItemAt(it));
		if (!pControl->IsVisible()) continue;

		// 若有扩充高度，则依次下移后面的控件
		if (disXY.cy > 0)
		{
			fixed_xy = pControl->GetFixedXY();
			fixed_xy.cy += disXY.cy;
			pControl->SetFixedXY(fixed_xy);
		}

		CLabelUI* pLab = (CLabelUI*)pControl->GetInterface(DUI_CTR_LABEL);
		if (pLab && pLab->GetText().GetLength() > 0)
		{
			RECT rcText = { 0, 0, 9999, 9999 };
			rcText.left += pLab->GetTextPadding().left;
			rcText.right -= pLab->GetTextPadding().right;

			CDuiString strText = pLab->GetText();
			DWORD iTextColor = pLab->GetTextColor();
			int nFont = pLab->GetFont();
			UINT uTextStyle = pLab->GetTextStyle();
			CRenderEngine::DrawText(pManager->GetPaintDC(), pManager, rcText, strText.GetData(), iTextColor, nFont, DT_CALCRECT | uTextStyle);

			LONG lw = rcText.right - rcText.left + pLab->GetTextPadding().left + pLab->GetTextPadding().right;
			LONG lh = rcText.bottom - rcText.top + pLab->GetTextPadding().top + pLab->GetTextPadding().bottom;
			if (lw > pLab->GetFixedWidth())
			{
				LONG fixed_x = pLab->GetFixedWidth();
				LONG fixed_y = lh;
				LONG lCount = ceil((DOUBLE)lh / fixed_y);
				lCount *= ceil((DOUBLE)lw / fixed_x);
				LONG H = lCount * fixed_y - (lCount - 1) * (pLab->GetTextPadding().top + pLab->GetTextPadding().bottom);
				pLab->SetFixedHeight(H);
				disXY.cy += H - fixed_y;
			}
			else
			{
				//若小于预设大小,则不缩小
				//pText->SetFixedWidth(lw);
			}
		}
		SIZE sz = pControl->EstimateSize(szAvailable);
		cXY.cy += sz.cy;
		if (cXY.cx < sz.cx)
		{
			cXY.cx = sz.cx;
		}
	}
	
	if (cXY.cy < szRoot.cy)
	{
		cXY.cy = szRoot.cy;
	}
	if (cXY.cx < szRoot.cx)
	{
		cXY.cx = szRoot.cx;
	}

	return cXY;
}

MsgBoxWnd* DialogFrame::_CreatSubWindow(CPaintManagerUI* pManager, DLG_STYLE iStyle, LPCTSTR xmlFileStr)
{
	if (!pManager || !xmlFileStr || _tcscmp(xmlFileStr, _T("")) == 0) return nullptr;

	HWND hParentWnd = pManager->GetPaintWindow();
	MsgBoxWnd* pMsgWnd = new MsgBoxWnd(hParentWnd, iStyle, xmlFileStr);

	return pMsgWnd;
}

CControlUI* DialogFrame::_CreatSubControl(CPaintManagerUI* pManager, LPCTSTR xmlFileStr /* = nullptr */)
{
	if (!pManager || !xmlFileStr || _tcscmp(xmlFileStr, _T("")) == 0) return nullptr;

	CDialogBuilder builder;
	CControlUI* pControl = nullptr;
	pControl = static_cast<CControlUI*>(builder.Create(xmlFileStr, (UINT)0, nullptr, pManager));
	CContainerUI* pRoot = static_cast<CContainerUI*>(pManager->GetRoot());
	if (!pRoot || !pControl) return nullptr;

	TCHAR str_name[MAX_PATH] = { 0 };
	_stprintf_s(str_name, _T("toast_%lu"), (DWORD)pControl);
	pControl->SetName(str_name);
	::RECT rect = { 0, 0, 0, 0 };
	::RECT rootRect = pRoot->GetPos();
	::LONG pos_x = rootRect.left;
	::LONG pos_y = rootRect.top;
	::LONG box_w = rootRect.right - rootRect.left;
	::LONG box_h = rootRect.bottom - rootRect.top;

	::SIZE szFixed = { 0, 0 };
	szFixed.cx = pos_x + (box_w - pManager->GetInitSize().cx) / 2;
	szFixed.cy = pos_y + (box_h - pManager->GetInitSize().cy) / 2;
	box_w = pManager->GetInitSize().cx;
	box_h = pManager->GetInitSize().cy;
	pControl->SetFloat(true);
	pControl->SetFixedXY(szFixed);
	pControl->SetFixedWidth(box_w);
	pControl->SetFixedHeight(box_h);
	pRoot->Add(pControl);

	return pControl;
}

int DialogFrame::MsgBox(HWND hWnd
	, LPCTSTR lpText
	, LPCTSTR lpCaption
	, UINT uType
	, DialogCallBack cb1 /* = nullptr  */
	, DialogCallBack cb2 /* = nullptr  */
	, DialogCallBack cb3 /* = nullptr */)
{
	DLG_STYLE iStyle = DLG_STYLE::DLG_DEFAULT;
	switch (uType)
	{
		case MB_OK:				iStyle = DLG_STYLE::DLG_OK;					break;
		case MB_OKCANCEL:		iStyle = DLG_STYLE::DLG_OK_CANCEL;			break;
		case MB_RETRYCANCEL:	iStyle = DLG_STYLE::DLG_RETRY_CANCEL;		break;
		default:				iStyle = DLG_STYLE::DLG_DEFAULT;			break;
	}

	MsgBoxWnd* pMsgWnd = new MsgBoxWnd(hWnd, iStyle, _T("default_dlg"));
	pMsgWnd->LayoutText(lpText, lpCaption);

	int iReturnID = IDNO;
	pMsgWnd->ShowModal();
	iReturnID = pMsgWnd->GetEventID();
	delete pMsgWnd;

	return iReturnID;
}

MsgBoxWnd* DialogFrame::MsgBox(CPaintManagerUI* pManager
	, LPCTSTR pzTitle
	, LPCTSTR pzTextContent
	, DLG_ID id /* = DLG_ID::DLG_ID_NONE  */
	, DLG_STYLE iStyle /* = DLG_STYLE::DLG_DEFAULT  */
	, DialogCallBack pEnsure_Fuc /* = nullptr  */
	, DialogCallBack pCancel_Fuc /* = nullptr  */
	, DialogCallBack pClose_Fuc /* = nullptr */)
{
	MsgBoxWnd* pMsgWnd = nullptr;

	if (pManager)
	{
		pMsgWnd = DialogFrame::_CreatSubWindow(pManager, iStyle, _T("default_dlg"));
		if (pMsgWnd)
		{
			pMsgWnd->LayoutText(pzTitle, pzTextContent);
			pMsgWnd->SetCallBack(id, pEnsure_Fuc, pCancel_Fuc, pClose_Fuc);

			DialogFrame* pDialog = new DialogFrame();
			pDialog->SetPaintManager(pManager);
			pDialog->SetFrozenEnable(false);

			DialogFrame::g_MsgBoxMap[pDialog] = pMsgWnd;
		}
	}

	return pMsgWnd;
}

MsgBoxWnd* DialogFrame::ToastBox(CPaintManagerUI* pManager
	, LPCTSTR pzTextContent
	, bool bAutoClose /* = false */
	, DLG_ID id /* = DLG_ID::DLG_ID_NONE  */
	, DialogCallBack cb /* = nullptr  */)
{
	MsgBoxWnd* pMsgWnd = nullptr;

	if (pManager)
	{
		pMsgWnd = DialogFrame::_CreatSubWindow(pManager, DLG_STYLE::DLG_NONE, _T("toast_dlg"));
		if (pMsgWnd)
		{
			DialogFrame* pDialog = new DialogFrame();
			pDialog->SetPaintManager(pManager);
			pDialog->SetFrozenEnable(false);
			DialogFrame::g_MsgBoxMap[pDialog] = pMsgWnd;

			if (bAutoClose)
			{
				pMsgWnd->GetCancelBtn()->SetVisible(false);
				SIZE csize = pMsgWnd->GetWindSize();
				pMsgWnd->SetContentPos({ 0, 0, csize.cx, csize.cy });

				TimerCallBack timepro = CC_CALLBACK_2(DialogFrame::ToastBoxTimerFunc, pDialog);
				g_wndTimmer.SetTimer(pManager->GetPaintWindow(), timepro, (UINT_PTR)pMsgWnd, TIME_TOAST_OVER);
			}

			pMsgWnd->LayoutText(_T(""), pzTextContent);
			pMsgWnd->SetCallBack(id, nullptr, cb, nullptr);
		}
	}

	return pMsgWnd;
}

CControlUI* DialogFrame::Toast(CPaintManagerUI* pManager
	, LPCTSTR pzTextContent
	, DLG_ID id /* = DLG_ID::DLG_ID_NONE  */
	, DialogCallBack pCallBackFunc /* = nullptr  */
	, bool bFadeOut /* = false */)
{
	CControlUI* pToastCtl = DialogFrame::_CreatSubControl(pManager, _T("dialog/toast.xml"));
	if (pToastCtl)
	{
		DialogFrame* pDialog = new DialogFrame();
		pDialog->SetPaintManager(pManager);
		pDialog->SetCallBack(id, pCallBackFunc);
		pDialog->SetSubControl(pToastCtl);

		UINT_PTR dwDialogID = (DWORD)(pDialog);
		g_ToastMap[dwDialogID] = pDialog;

		CLabelUI* pToastText = static_cast<CTextUI*>(pManager->FindControl(MsgBoxWnd::g_kTxtDialog_Toast));
		if (pToastText)
		{
			CDuiString pzTitleStr = _T("");
			pzTitleStr += pzTextContent;
			pToastText->SetText(pzTextContent);
		}

		SIZE sz = pDialog->EstimateSize(pManager, (CContainerUI*)pToastCtl, {0, 0});
		pToastCtl->SetFixedWidth(sz.cx);
		pToastCtl->SetFixedHeight(sz.cy);
		
		if (bFadeOut)
		{
			TimerCallBack timepro = CC_CALLBACK_2(DialogFrame::ToastTimerFuncFadeOut, pDialog);
			g_wndTimmer.SetTimer(pManager->GetPaintWindow(), timepro, dwDialogID, TIME_TOAST_PERFRAME);
		}
		else
		{
			TimerCallBack timepro = CC_CALLBACK_2(DialogFrame::ToastTimerFunc, pDialog);
			g_wndTimmer.SetTimer(pManager->GetPaintWindow(), timepro, dwDialogID, TIME_TOAST_OVER);
		}
	}

	return pToastCtl;
}

DialogFrame* DialogFrame::ToastWait(CPaintManagerUI* pManager
	, LPCTSTR pzTextContent
	, DLG_ID id /* = DLG_ID::DLG_ID_NONE  */
	, DialogCallBack pCallBackFunc /* = nullptr  */)
{
	DialogFrame* pDialog = nullptr;
	CControlUI* pToastCtl = DialogFrame::_CreatSubControl(pManager, _T("dialog/toast.xml"));
	if (pToastCtl)
	{
		pDialog = new DialogFrame();
		pDialog->SetPaintManager(pManager);
		pDialog->SetCallBack(id, pCallBackFunc);
		pDialog->SetSubControl(pToastCtl);

		UINT_PTR dwDialogID = (DWORD)(pDialog);
		g_ToastMap[dwDialogID] = pDialog;

		CLabelUI* pToastText = static_cast<CTextUI*>(pManager->FindControl(MsgBoxWnd::g_kTxtDialog_Toast));
		if (pToastText)
		{
			CDuiString pzTitleStr = _T("");
			pzTitleStr += pzTextContent;
			pToastText->SetText(pzTextContent);
		}

		SIZE sz = pDialog->EstimateSize(pManager, (CContainerUI*)pToastCtl, { 0, 0 });
		pToastCtl->SetFixedWidth(sz.cx);
		pToastCtl->SetFixedHeight(sz.cy);
	}

	return pDialog;
}

DWORD DialogFrame::Waitting(CPaintManagerUI* pManager
	, DLG_ID id /* = DLG_ID::DLG_ID_NONE  */
	, DialogCallBack pCallBackFunc/* = nullptr */)
{
	if (!pManager) return 0xFFFFFFFF;

	DialogFrame* pDialog = new DialogFrame();
	pDialog->SetPaintManager(pManager);
	pDialog->SetCallBack(id, pCallBackFunc);

	UINT_PTR dwDialogID = (DWORD)(pDialog);
	g_WattingMap[dwDialogID] = pDialog;

	CControlUI* pWaittingCtl = DialogFrame::_CreatSubControl(pManager, _T("dialog/waitting.xml"));
	if (pWaittingCtl)
	{
		pDialog->SetSubControl(pWaittingCtl);
		
		CControlUI* pFadeLayer = pManager->FindControl(MsgBoxWnd::g_kCtl_FadeLayer);
		if (pFadeLayer)
		{
			pFadeLayer->SetVisible(true);
			pFadeLayer->SetMouseEnabled(true);
			pFadeLayer->SetKeyboardEnabled(true);

			//禁用底层键盘事件
			//析构时恢复状态
			pDialog->GetRootControl()->SetKeyboardEnabled(false);
		}
		else
		{
			pDialog->SetFrozenEnable(false);
		}

		TimerCallBack timepro = CC_CALLBACK_2(DialogFrame::WaittingTimerFunc, pDialog);
		g_wndTimmer.SetTimer(pManager->GetPaintWindow(), timepro, dwDialogID, TIME_WAITTING_OVER);
	}

	return dwDialogID;
}

void DialogFrame::CloseMsgBox(MsgBoxWnd* pMsgBox)
{
	if (!pMsgBox) return;

	std::map<DialogFrame*, MsgBoxWnd*>::iterator& it = g_MsgBoxMap.begin();
	for (; it != g_MsgBoxMap.end();)
	{
		if (it->second == pMsgBox)
		{
			pMsgBox->Close();
			break;
		}
		++it;
	}
}

void DialogFrame::EndMsgBox(MsgBoxWnd* pMsgBox)
{
	if (!pMsgBox) return;

	std::map<DialogFrame*, MsgBoxWnd*>::iterator& it = g_MsgBoxMap.begin();
	for (; it != g_MsgBoxMap.end();)
	{
		if (it->second == pMsgBox)
		{
			DialogFrame* pDialog = it->first;
			pDialog->SetFrozenEnable(true);
			CPaintManagerUI* pManager = pDialog->GetPaintManager();
			g_wndTimmer.KillTimer(pManager->GetPaintWindow(), (UINT_PTR)pMsgBox);
			
			SAFE_DELETE(pDialog);

			it = g_MsgBoxMap.erase(it);
			break;
		}
		++it;
	}
}

void DialogFrame::EndToastBox(DialogFrame* pDialog)
{
	if (!pDialog) return;

	UINT_PTR dwDialogID = (DWORD)(pDialog);
	DialogFrame::EndToast(dwDialogID);
}

void DialogFrame::EndToast(DWORD idEvent)
{
	std::map<UINT_PTR, DialogFrame*>::iterator& it = g_ToastMap.find(idEvent);
	if (it != g_ToastMap.end())
	{
		DialogFrame* pDialog = it->second;
		CControlUI* pControl = pDialog->GetSubControl();
		CContainerUI* pRoot = pDialog->GetRootControl();
		if (pControl && pRoot)
		{
			CPaintManagerUI* pManager = pDialog->GetPaintManager();
			g_wndTimmer.KillTimer(pManager->GetPaintWindow(), idEvent);

			pRoot->Remove(pControl);
			delete pDialog;

			g_ToastMap.erase(idEvent);
		}
	}
}

void DialogFrame::EndWaitting(DWORD idEvent)
{
	std::map<UINT_PTR, DialogFrame*>::iterator& it = g_WattingMap.find(idEvent);
	if (it != g_WattingMap.end())
	{
		DialogFrame* pDialog = it->second;
		CControlUI* pControl = pDialog->GetSubControl();
		CContainerUI* pRoot = pDialog->GetRootControl();
		if (pControl && pRoot)
		{
			CPaintManagerUI* pManager = pDialog->GetPaintManager();
			g_wndTimmer.KillTimer(pManager->GetPaintWindow(), idEvent);

			CControlUI* pFadeLayer = pManager->FindControl(MsgBoxWnd::g_kCtl_FadeLayer);
			if (pFadeLayer)
			{
				pFadeLayer->SetVisible(false);
				pFadeLayer->SetMouseEnabled(false);
				pFadeLayer->SetKeyboardEnabled(false);
				
				//恢复底层键盘事件
				pDialog->GetRootControl()->SetKeyboardEnabled(true);
			}

			pDialog->SetFrozenEnable(true);
			pRoot->Remove(pControl);
			delete pDialog;

			g_WattingMap.erase(idEvent);
		}
	}
}

void DialogFrame::ToastTimerFunc(UINT_PTR idEvent, DWORD dwTime)
{
	std::map<UINT_PTR, DialogFrame*>::iterator& it = g_ToastMap.find(idEvent);
	if (it != g_ToastMap.end())
	{
		if (it->second->m_pCallBackFunc)
		{
			TNotifyUI msg;
			::ZeroMemory(&msg, sizeof(TNotifyUI));
			it->second->m_pCallBackFunc(m_iDlg_ID, msg);
		}

		DialogFrame::EndToast(idEvent);
	}
}

void DialogFrame::ToastTimerFuncFadeOut(UINT_PTR idEvent, DWORD dwTime)
{
	CControlUI* pSubCtl = this->GetSubControl();
	CVerticalLayoutUI* pVLayout = static_cast<CVerticalLayoutUI*>(pSubCtl);
	CLabelUI* pToastText = static_cast<CTextUI*>(m_pManager->FindSubControlByName(pSubCtl, MsgBoxWnd::g_kTxtDialog_Toast));
	if (pVLayout)
	{
		BYTE iDis = 0xFF / (TIME_TOAST_OVER / (float)TIME_TOAST_PERFRAME);
		if (m_cbFade - iDis <= 0)
		{
			m_cbFade = 0xFF;
			this->ToastTimerFunc(idEvent, dwTime);
		}
		else
		{
			m_cbFade -= iDis;
			TCHAR pzBuffer[MAX_PATH] = { 0 };
			_stprintf_s(pzBuffer, _T("file='dialog/toast_bg.png' corner='10,10,10,10' fade='%u'"), m_cbFade);

			DWORD dwTextColor = pToastText->GetTextColor();
			BYTE cbAlpha = LOBYTE((dwTextColor & 0xFF000000) >> 24);
			cbAlpha -= iDis;
			dwTextColor &= 0x00FFFFFF;
			DWORD dwAValue = (cbAlpha << 24) & 0xFF000000;
			dwTextColor |= dwAValue;

			pToastText->SetTextColor(dwTextColor);
			pVLayout->SetBkImage(pzBuffer);
		}
	}
}

void DialogFrame::ToastBoxTimerFunc(UINT_PTR idEvent, DWORD dwTime)
{
	MsgBoxWnd* pMsgBox = (MsgBoxWnd*)idEvent;
	if (TRUE == ::IsWindow(pMsgBox->GetHWND()))
	{
		pMsgBox->Close();
	}
}

void DialogFrame::WaittingTimerFunc(UINT_PTR idEvent, DWORD dwTime)
{
	std::map<UINT_PTR, DialogFrame*>::iterator& it = g_WattingMap.find(idEvent);
	if (it != g_WattingMap.end())
	{
		if (it->second->m_pCallBackFunc)
		{
			TNotifyUI msg;
			::ZeroMemory(&msg, sizeof(TNotifyUI));
			it->second->m_pCallBackFunc(m_iDlg_ID, msg);
		}

		CPaintManagerUI* pManager = it->second->GetPaintManager();
		DialogFrame::EndWaitting(idEvent);

		DialogFrame::MsgBox(pManager, _T("温馨提示:"), _T("超时未响应!"));
	}
}

