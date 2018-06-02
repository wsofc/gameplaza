#include "stdafx.h"
#include "MainFrame.h"

LPCTSTR MainFrame::g_kBtn_Close			= _T("main-btn_close");								// �ر�
LPCTSTR MainFrame::g_kEdt_Encrypt		= _T("main-edt_encrypt");							// ����·��
LPCTSTR MainFrame::g_kBtn_Encrypt		= _T("main-btn_encrypt");							// ���ܰ�ť
LPCTSTR MainFrame::g_kEdt_Decrypt		= _T("main-edt_decrypt");							// ����·��
LPCTSTR MainFrame::g_kBtn_Decrypt		= _T("main-btn_decrypt");							// ���ܰ�ť
LPCTSTR MainFrame::g_kBtn_Encrypt_Look	= _T("main-btn_encrypt_look");						// �������
LPCTSTR MainFrame::g_kBtn_Decrypt_Look	= _T("main-btn_decrypt_look");						// �������

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
		m_hWnd = this->Create(NULL, _T("JSON�ӽ���"), UI_CLASSSTYLE_DIALOG, 0L);
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
	// ���ô���ͼ��
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
		strText = _T("����");
		pEdt = m_pEdtEncrypt;
	}
	else
	{
		strText = _T("����");
		pEdt = m_pEdtDecrypt;
	}
	
	std::vector<CDuiString> vPath;
	vPath.clear();
	
	// �Ȳ��·��
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

	// ����ֺ����Ӵ�����ֱ�����
	if (vPath.size() < 0)
	{
		vPath.push_back(strPath);
	}

	// ������ļ�����
	if (vPath.size() > 0)
	{
		for (int i = 0; i < vPath.size(); ++i)
		{
			// ����ļ�·����Ч��
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

		_stprintf_s(m_pzBuffer, _T("%s�ɹ�!"), strText);
		::MessageBox(m_hWnd, m_pzBuffer, _T("��ܰ��ʾ"), MB_OK);
	}
	else
	{
	err_lab:
		_stprintf_s(m_pzBuffer, _T("%s�ļ�������Ч·��!"), strText);
		::MessageBox(m_hWnd, m_pzBuffer, _T("��ܰ��ʾ"), MB_OK);
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

	// �������ļ��ĶԻ���
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
	openfilename.hwndOwner = hwndOwner;					// �����߶ԵĻ��򴰿ھ��
	openfilename.hInstance = NULL;						// һ��ΪNULL����ʾ����ʹ�õ���ͨ�öԻ���,����ָ���Զ���Ի���
	openfilename.lpstrFilter = lpszFilter;				// ���͹���
	openfilename.lpstrCustomFilter = NULL;				// �û��Զ����������ʽ
	openfilename.nMaxCustFilter = 0L;					// �û��Զ����������ʽ��С
	openfilename.nFilterIndex = 0; // 1L				// ��ǰѡ��Ĺ�����������
	openfilename.lpstrFile = szFilePath;				// �ļ�·��
	openfilename.nMaxFile = MAX_PATH;					// �ļ�·����С
	openfilename.lpstrFileTitle = szFileTitle;			// �ļ���
	openfilename.nMaxFileTitle = MAX_PATH;				// �ļ�����С
	openfilename.lpstrInitialDir = NULL;				// ��ʼ��·��
	openfilename.lpstrTitle = lpszTitle;				// �Ի������
	openfilename.nFileOffset = 0;						// �ļ�·����ȥ�ļ�����ƫ�Ƴ���
	openfilename.nFileExtension = 0;					// �ļ�·����ȥ��չ�����ƫ�Ƴ���
	openfilename.lpstrDefExt = lpszDefExt;				// Ĭ����չ��
	openfilename.lCustData = 0;							// Ӧ�ó����������
	openfilename.Flags = 0;

	//�����ļ�����
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
			BOOL iRes = this->openFileDialog(_T("�ı��ļ�(*.json)\0*.JSON\0\0"), m_hWnd, vNames, true);
			if (TRUE == iRes && vNames.size() > 0)
			{
				this->_SetEdtText(m_pEdtEncrypt, vNames);
			}
		}
		else if (_tcscmp(sName, MainFrame::g_kBtn_Decrypt_Look) == 0)
		{
			std::vector<CDuiString> vNames;
			BOOL iRes = this->openFileDialog(_T("�ı��ļ�(*.lm)\0*.LM\0\0"), m_hWnd, vNames, true);
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