#include "stdafx.h"
#include "MainFrame.h"

LPCTSTR MainFrame::g_kBtn_Close			= _T("main-btn_close");								// 关闭
LPCTSTR MainFrame::g_kEdt_Encrypt		= _T("main-edt_encrypt");							// 加密路径
LPCTSTR MainFrame::g_kBtn_Encrypt		= _T("main-btn_encrypt");							// 加密按钮
LPCTSTR MainFrame::g_kEdt_Decrypt		= _T("main-edt_decrypt");							// 解密路径
LPCTSTR MainFrame::g_kBtn_Decrypt		= _T("main-btn_decrypt");							// 解密按钮
LPCTSTR MainFrame::g_kBtn_Encrypt_Look	= _T("main-btn_encrypt_look");						// 加密浏览
LPCTSTR MainFrame::g_kBtn_Decrypt_Look	= _T("main-btn_decrypt_look");						// 解密浏览

MainFrame::MainFrame()
	: m_pEdtEncrypt(nullptr)
	, m_pEdtDecrypt(nullptr)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

MainFrame::~MainFrame()
{
}

BOOL MainFrame::CreateFrame()
{
	if (FALSE == ::IsWindow(m_hWnd))
	{
		m_hWnd = this->Create(NULL, _T("JSON加解密"), UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}

	return TRUE;
}
UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return UILIB_RESOURCETYPE::UILIB_FILE;
}

CDuiString MainFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString MainFrame::GetSkinFile()
{
	return _T("main.xml");
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
};

void MainFrame::InitWindow()
{
	m_pEdtEncrypt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(MainFrame::g_kEdt_Encrypt));
	m_pEdtDecrypt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(MainFrame::g_kEdt_Decrypt));
}

void MainFrame::InitData(TNotifyUI& msg)
{
	// 设置窗口图标
	this->SetIcon(IDI_JSONINTERPRETER);
}

void MainFrame::_SetEdtText(CRichEditUI* pEdt, std::vector<CDuiString>& vNames)
{
	ASSERT(pEdt);
	if (!pEdt) return;

	CDuiString strPath;
	for (int i = 0; i < vNames.size(); ++i)
	{
		strPath += vNames[i];
		strPath += _T(";");
	}

	pEdt->SetText(_T(""));
	pEdt->Clear();
	pEdt->SetText(strPath.GetData());
	pEdt->EndDown();
	m_PaintManager.SetFocusNeeded(pEdt);
}

void MainFrame::_Parse(bool bExpress)
{
	CDuiString strText = _T("");
	CRichEditUI* pEdt = nullptr;

	if (bExpress)
	{
		strText = _T("加密");
		pEdt = m_pEdtEncrypt;
	}
	else
	{
		strText = _T("解密");
		pEdt = m_pEdtDecrypt;
	}
	
	std::vector<CDuiString> vPath;
	vPath.clear();
	
	// 先拆分路径
	CDuiString strPath = pEdt->GetText();
	for (int iStart = 0, iEnd = 0; iEnd != -1;)
	{
		iEnd = strPath.Find(_T(";"), iStart);
		if (iEnd != -1)
		{
			vPath.push_back(strPath.Mid(iStart, iEnd - iStart));
			iStart = iEnd + 1;
		}
	}

	// 若拆分后无子串，则直接添加
	if (vPath.size() < 0)
	{
		vPath.push_back(strPath);
	}

	// 最后检测文件个数
	if (vPath.size() > 0)
	{
		for (int i = 0; i < vPath.size(); ++i)
		{
			// 检测文件路径有效性
			if (FALSE == ::PathFileExists(vPath[i].GetData()))
			{
				goto err_lab;
			}

			JsonFile* pFile = new JsonFile(vPath[i], true);

			Json::Value vRoot;
			pFile->initRootNode(vRoot, !bExpress);
			pFile->SaveFile(vRoot, SaveType::SAVE_TYPE_STYLEDWRITER, bExpress);

			SAFE_DELETE(pFile);
		}

		_stprintf_s(m_pzBuffer, _T("%s成功!"), strText);
		::MessageBox(m_hWnd, m_pzBuffer, _T("温馨提示"), MB_OK);
	}
	else
	{
	err_lab:
		_stprintf_s(m_pzBuffer, _T("%s文件存在无效路径!"), strText);
		::MessageBox(m_hWnd, m_pzBuffer, _T("温馨提示"), MB_OK);
		pEdt->SetFocus();
	}
}

void MainFrame::Encrypt()
{
	_Parse(true);
}

void MainFrame::Decrypt()
{
	_Parse(false);
}

bool MainFrame::openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti /* = false */)
{
	DWORD dwFlag = isMulti ? OFN_ALLOWMULTISELECT : 0;
	TCHAR szFileName[MAX_PATH * 101 + 1] = _T("");

	OPENFILENAME openfilename = { 0 };

	::ZeroMemory(&openfilename, sizeof(OPENFILENAME));

	CDuiString s_title;

	openfilename.lStructSize = sizeof(OPENFILENAME);
	openfilename.hwndOwner = hwndOwner;
	openfilename.hInstance = NULL;
	openfilename.lpstrFilter = lpstrFilter;
	openfilename.lpstrCustomFilter = NULL;
	openfilename.nMaxCustFilter = 0L;
	openfilename.nFilterIndex = 1L;
	openfilename.lpstrFile = szFileName;
	openfilename.nMaxFile = MAX_PATH * 101 + 1;
	openfilename.lpstrFileTitle = NULL;
	openfilename.nMaxFileTitle = 0;
	openfilename.lpstrInitialDir = NULL;
	openfilename.lpstrTitle = s_title;
	openfilename.nFileOffset = 0;
	openfilename.nFileExtension = 0;
	openfilename.lpstrDefExt = _T("*.*");
	openfilename.lCustData = 0;
	openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY | OFN_EXPLORER | dwFlag;

	// 弹出打开文件的对话框
	CDuiString str;

	if (::GetOpenFileName(&openfilename))
	{
		LPTSTR p = szFileName;
		CDuiString TempPath;
		if (*p != NULL)
		{
			TempPath = p;
			p += TempPath.GetLength() + 1;
		}

		if (*p == NULL)
		{
			//	TempPath = TempPath.Left(TempPath.ReverseFind(L'\\'));
			fileNames.push_back(TempPath);
		}


		while (*p != NULL)
		{
			CDuiString str = p;

			p += str.GetLength() + 1;

			fileNames.push_back(TempPath + _T("\\") + str);
		}

		return TRUE;
	}
	else
		return FALSE;
}

bool MainFrame::saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath)
{
	TCHAR szFilePath[MAX_PATH] = TEXT("");
	TCHAR szFileTitle[MAX_PATH] = TEXT("");
	OPENFILENAME openfilename = { 0 };
	::ZeroMemory(&openfilename, sizeof(OPENFILENAME));

	openfilename.lStructSize = sizeof(OPENFILENAME);
	openfilename.hwndOwner = hwndOwner;					// 所有者对的话框窗口句柄
	openfilename.hInstance = NULL;						// 一般为NULL，表示我们使用的是通用对话框,否则指向自定义对话框
	openfilename.lpstrFilter = lpszFilter;				// 类型过滤
	openfilename.lpstrCustomFilter = NULL;				// 用户自定义过滤器样式
	openfilename.nMaxCustFilter = 0L;					// 用户自定义过滤器样式大小
	openfilename.nFilterIndex = 0; // 1L				// 当前选择的过滤器的索引
	openfilename.lpstrFile = szFilePath;				// 文件路径
	openfilename.nMaxFile = MAX_PATH;					// 文件路径大小
	openfilename.lpstrFileTitle = szFileTitle;			// 文件名
	openfilename.nMaxFileTitle = MAX_PATH;				// 文件名大小
	openfilename.lpstrInitialDir = NULL;				// 初始化路径
	openfilename.lpstrTitle = lpszTitle;				// 对话框标题
	openfilename.nFileOffset = 0;						// 文件路径除去文件名的偏移长度
	openfilename.nFileExtension = 0;					// 文件路径除去扩展名后的偏移长度
	openfilename.lpstrDefExt = lpszDefExt;				// 默认扩展名
	openfilename.lCustData = 0;							// 应用程序定义的数据
	openfilename.Flags = 0;

	//保存文件窗口
	BOOL bRes = False;
	bRes = ::GetSaveFileName(&openfilename);

	if (lpOutFilePath)
		_tcscpy(lpOutFilePath, szFilePath);
	return bRes;
}

void MainFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
		this->InitData(msg);
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, MainFrame::g_kBtn_Close) == 0)
		{
			::PostQuitMessage(0);
		}
		else if (_tcscmp(sName, MainFrame::g_kBtn_Encrypt_Look) == 0)
		{
			std::vector<CDuiString> vNames;
			BOOL iRes = this->openFileDialog(_T("文本文件(*.json)\0*.JSON\0\0"), m_hWnd, vNames, true);
			if (TRUE == iRes && vNames.size() > 0)
			{
				this->_SetEdtText(m_pEdtEncrypt, vNames);
			}
		}
		else if (_tcscmp(sName, MainFrame::g_kBtn_Decrypt_Look) == 0)
		{
			std::vector<CDuiString> vNames;
			BOOL iRes = this->openFileDialog(_T("文本文件(*.lm)\0*.LM\0\0"), m_hWnd, vNames, true);
			if (TRUE == iRes && vNames.size() > 0)
			{
				this->_SetEdtText(m_pEdtDecrypt, vNames);
			}
		}
		else if (_tcscmp(sName, MainFrame::g_kBtn_Encrypt) == 0)
		{
			this->Encrypt();
		}
		else if (_tcscmp(sName, MainFrame::g_kBtn_Decrypt) == 0)
		{
			this->Decrypt();
		}
	}

	//__super::Notify(msg);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (m_PaintManager.GetRoot()->IsKeyboardEnabled())
	{
		if (wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			return FALSE;
		}
	}
	return FALSE;
}