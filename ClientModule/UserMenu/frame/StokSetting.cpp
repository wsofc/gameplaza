#include "stdafx.h"
#include "StokSetting.h"

namespace UserMenuNSP
{
	extern NumberConvert 	g_NumberConvert;
	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	StokSetting* StokSetting::g_instance = nullptr;
	StokSetting::StokSetting()
		: m_pLabCurValue(nullptr)
		, m_pLabCurCapital(nullptr)
		, m_pEdtNewValue(nullptr)
		, m_pLabNewCapital(nullptr)
		, m_pBtnCancel(nullptr)
		, m_pBtnEnsure(nullptr)
		, m_pBtnClose(nullptr)
	{
		memset(m_pzBuffer, 0, MAX_PATH * sizeof(TCHAR));
	}

	StokSetting::~StokSetting()
	{
	}

	StokSetting* StokSetting::GetInstance()
	{
		if (!g_instance)
		{
			g_instance = new StokSetting();
		}

		return g_instance;
	}

	int StokSetting::CreatFrame(CPaintManagerUI* pParentManager)
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

	void StokSetting::InitWindow()
	{
		// 初始化控件名
		m_strCurValue		= _T("panel_stock_setting-lab_cur_value");
		m_strCurCapital		= _T("panel_stock_setting-lab_cur_capital");
		m_strNewValue		= _T("panel_stock_setting-edt_new_value");
		m_strNewCapital		= _T("panel_stock_setting-lab_new_capital");
		m_strCancel			= _T("panel_stock_setting-btn_cancel");
		m_strEnsure			= _T("panel_stock_setting-btn_ensure");
		m_strClose			= _T("panel_stock_setting-btn_close");

		// 初始化控件对象
		m_pLabCurValue		= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strCurValue));
		m_pLabCurCapital	= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strCurCapital));
		m_pEdtNewValue		= static_cast<CRichEditUI*>(m_PaintManager.FindControl(m_strNewValue));
		m_pLabNewCapital	= static_cast<CLabelUI*>(m_PaintManager.FindControl(m_strNewCapital));
		m_pBtnCancel		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strCancel));
		m_pBtnEnsure		= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strEnsure));
		m_pBtnClose			= static_cast<CButtonUI*>(m_PaintManager.FindControl(m_strClose));
	}

	void StokSetting::InitData(TNotifyUI& msg)
	{
		// 更新当前库存、大小写
		this->UpdateLastData();

		// 聚焦"输入新库存"输入框
		m_PaintManager.SetFocusNeeded(m_pEdtNewValue);

		// 新库存值大写
		LONGLONG lNewStokValue = _ttoll(m_pEdtNewValue->GetText());
		std::string strNewNum = g_NumberConvert.NumToChineseStr(lNewStokValue);
		m_pLabNewCapital->SetText(strNewNum.c_str());
		m_pLabNewCapital->SetToolTip(m_pLabCurCapital->GetText());
		m_pLabNewCapital->SetEndEllipsis(true);
	}

	void StokSetting::UpdateLastData()
	{
		// 当前库存值
		_stprintf_s(m_pzBuffer, _T("%lld"), g_tMenuData.lCurStokValue);
		m_pLabCurValue->SetText(m_pzBuffer);

		// 当前库存值大写
		std::string strNum = g_NumberConvert.NumToChineseStr(g_tMenuData.lCurStokValue);
		m_pLabCurCapital->SetText(strNum.c_str());
		m_pLabCurCapital->SetToolTip(m_pLabCurCapital->GetText());
		m_pLabCurCapital->SetEndEllipsis(true);
	}

	void StokSetting::Notify(TNotifyUI& msg)
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
			// 更新新值输入的大写描述
			if (m_pEdtNewValue && m_pEdtNewValue->IsFocused())
			{
				LONGLONG lNewValue = _ttoll(m_pEdtNewValue->GetText());
				std::string strNum = g_NumberConvert.NumToChineseStr(lNewValue);
				m_pLabNewCapital->SetText(strNum.c_str());
				m_pLabNewCapital->SetToolTip(m_pLabNewCapital->GetText());
				m_pLabNewCapital->SetEndEllipsis(true);
			}
		}
	}

	void StokSetting::Check()
	{
		bool bInputError = false;
		bool bInputEmpty = false;
		int iMsgEventID = MsgEventID::MSGID_NONE;
		CDuiString& pzNewValue = m_pEdtNewValue->GetText();
		if (!MatchRegular(pzNewValue, _T("[0-9]+")))
		{
			bInputError = true;
			bInputEmpty = pzNewValue.GetLength() <= 0;
			iMsgEventID = MsgEventID::MSGID_STOKE_SETTING;
			goto lab_err;
		}
		else
		{
			if (g_pDelegate)
			{
				LONGLONG lNewValue = _ttoll(m_pEdtNewValue->GetText());
				if (lNewValue > pow(10, 16) - 1)
				{
					goto lab_err;
				}
				g_pDelegate->CallBackStokSetting(lNewValue);
			}
			this->Close();
		}
		
	lab_err:
		if (bInputError)
		{
			LPCTSTR lpDescStr = bInputEmpty ? _T("输入不能为空!") : _T("输入不正确!");
			DialogCallBack dlg_cb = CC_CALLBACK_2(StokSetting::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("温馨提示")
				, lpDescStr
				, (DLG_ID)iMsgEventID
				, DLG_STYLE::DLG_DEFAULT
				, dlg_cb
				, dlg_cb
				, dlg_cb);
		}
	}

	void StokSetting::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
	{
		switch (id)
		{
			case MsgEventID::MSGID_STOKE_SETTING:	m_pEdtNewValue->SetFocus();		break;
			default:																break;
		}
	}

	LRESULT StokSetting::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg)
		{
			case WM_CHAR:
			case WM_IME_CHAR:
			{
				if ((m_pEdtNewValue && m_pEdtNewValue->IsFocused()))
				{
					// 只接收数字
					if (wParam < 48 || wParam > 57)
					{
						bHandled = TRUE;
						return TRUE;
					}
				}
			}break;

			case WM_KEYDOWN:
			{
				// 粘贴 ctrl + v 的处理
				if ((GetKeyState(VK_CONTROL) < 0) && (wParam == 'V'))
				{
					if (m_pEdtNewValue && m_pEdtNewValue->IsFocused())
					{
						// 获取粘贴内容
						CDuiString pzCopyText;
						m_pEdtNewValue->GetClipboardText(pzCopyText);
						pzCopyText.Replace(_T(" "), _T(""));

						// 非数字则跳过
						if (!UserMenuNSP::MatchRegular(pzCopyText, _T("[0-9]+")))
						{
							bHandled = TRUE;
							return TRUE;
						}
						else
						{
							// 满足条件,去除粘贴文本中的空格
							m_pEdtNewValue->SetClipboardText(pzCopyText);
						}
					}
				}
			}break;

			default:
				break;
		}
		
		bHandled = FALSE;
		return FALSE;
	}

	void StokSetting::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete g_instance;
		g_instance = nullptr;
	}

	UILIB_RESOURCETYPE StokSetting::GetResourceType() const
	{
		return g_tMenuData.tResInfo.iResourceType;
	}

	LPCTSTR StokSetting::GetResourceID() const
	{
		return g_tMenuData.tResInfo.iResourceID;
	}

	CDuiString StokSetting::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString StokSetting::GetSkinFile()
	{
		return IDR_XML_PanelStockSetting;
	}

	LRESULT StokSetting::ResponseDefaultKeyEvent(WPARAM wParam)
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
