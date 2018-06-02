#include "stdafx.h"
#include "gameUtil.h"
#include <commdlg.h>
#include <winioctl.h>

extern GameResource*	R;
extern CStaticLogger	g_Logger;
extern HANDLE			g_hMutex;

GameUtil* GameUtil::g_instance = NULL;

GameUtil::GameUtil()
	: m_cur_panel(TLS_NONE)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

GameUtil::~GameUtil()
{
	g_instance = NULL;
}

GameUtil* GameUtil::GetInstance()
{
	if (NULL == g_instance)
	{
		g_instance = new (std::nothrow)GameUtil();
	}

	return g_instance;
}

int WINAPI GameUtil::WriteWebBrowserRegKey(LPCTSTR lpKey, DWORD dwValue, tstring& strOutInfo)
{
	int iReturn = 0;
	HKEY hk;
	tstring str = _T("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\");
	str += lpKey;
	strOutInfo = str;
	iReturn = ::RegCreateKey(HKEY_LOCAL_MACHINE, str.c_str(), &hk);
	if (iReturn == 0)
	{
		iReturn = ::RegSetValueEx(hk, R->MainExeMoudleName, NULL, REG_DWORD, (const byte*)&dwValue, 4);
	}
	::RegCloseKey(hk);

	return iReturn;
}

bool GameUtil::SetClipBoardText(LPCSTR lpText, HWND hWnd)
{
	ASSERT(hWnd);

	//打开剪贴板
	if (!::OpenClipboard(hWnd))
	{
		return false;
	}
	//如果剪贴板有内容则关闭剪贴板
	if (!::EmptyClipboard())
	{
		::CloseClipboard();
		return false;
	}
	//获取需要设置文本的长度
	int len = _tcslen(lpText);

	//申请剪贴板空间
	HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (len + 1)*sizeof(char*));
	if (hClip == NULL)
	{
		::CloseClipboard();
		return false;
	}
	//给申请的空间加锁
	char* pBuf = (char*)::GlobalLock(hClip);
	if (pBuf == NULL)
	{
		::GlobalFree(hClip);
		::CloseClipboard();
		return false;
	}
	//拷贝文本内容到剪贴板
	_tcscpy(pBuf, lpText);

	//操作完成，释放锁
	::GlobalUnlock(hClip);
	if (NULL == ::SetClipboardData(CF_TEXT, hClip))
	{
		::GlobalFree(hClip);
		::CloseClipboard();
		return false;
	}

	::CloseClipboard();

	return true;
}

tstring GameUtil::GetClipBoardText(HWND hWnd)
{
	ASSERT(hWnd);
	tstring strText;

	//判断剪贴板的数据格式是否可以处理
	if (!::IsClipboardFormatAvailable(CF_TEXT))
	{
		return strText;
	}

	//打开剪贴板
	if (!::OpenClipboard(hWnd))
	{
		return strText;
	}

	//获取数据
	HANDLE hMem = ::GetClipboardData(CF_TEXT);
	if (hMem != NULL)
	{
		//获取字符串
		LPCTSTR lpStr = (LPCTSTR)::GlobalLock(hMem);
		if (lpStr != NULL)
		{
			strText = lpStr;
			//释放锁内存
			::GlobalUnlock(hMem);
		}
	}

	//关闭剪贴板
	::CloseClipboard();
	return strText;
}

/*
* @breif:
*	因为Ansi下 std::regex_match 无法支持汉字匹配，
*	因此统一先转换为Unicode在进行匹配
*/
bool GameUtil::MatchWordByType(CDuiString& strIn, DWORD dwType)
{
	if (strIn.GetLength() <= 0) return false;

	bool bResult = false;
	CString pattern;

	switch (dwType)
	{
		case MatchWords_Type::REG_TYPE_ACCOUNT:
		{
			pattern = _T("[a-zA-Z0-9_]{5,18}");
		}break;

		case MatchWords_Type::REG_TYPE_NICKNAME:
		{
			pattern = _T("([\u4e00-\u9fa5])|([\\S]){2,30}");
		}break;

		case MatchWords_Type::REG_TYPE_PASSWORD:
		{
			pattern = _T("([\\S^\u4e00-\u9fa5]){6,18}");
		}break;

		case MatchWords_Type::REG_TYPE_TELPHONE:
		{
			pattern = _T("[1][3-9](\\d){9}");
		}break;

		case MatchWords_Type::REG_TYPE_VERTCODE:
		{
			pattern = _T("[A-Za-z0-9]{4,5}");
		}break;

		case MatchWords_Type::REG_TYPE_REALNAME:
		{
			pattern = _T("([\u4e00-\u9fa5]){2,6}");
		}break;

		case MatchWords_Type::REG_TYPE_EMAIL:
		{
			pattern = _T("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		}break;

		case MatchWords_Type::REG_TYPE_QQ_NUMBER:
		{
			pattern = _T("[1-9](\\d){4,18}");
		}break;

		case MatchWords_Type::REG_TYPE_QUESTION:
		{
			pattern = _T("([\u4e00-\u9fa5])|([\\S]){4,30}");
		}break;

		case MatchWords_Type::REG_TYPE_ANSWER:
		{
			pattern = _T("([\u4e00-\u9fa5])|([\\S]){4,30}");
		}break;
		
		case MatchWords_Type::REG_TYPE_IDCARD:
		{
			pattern = _T("([\\S]){15,18}");
		}break;

		case MatchWords_Type::REG_TYPE_UNREAL:
		{
			pattern = _T(".*unreal\\<[0-9]{9}\\>.*");
		}break;

		case MatchWords_Type::REG_TYPE_EXTRA:
		{
			pattern = _T(".*");
		}break;

		default:
			break;
	}

	return this->MatchRegular(strIn, pattern);
}

bool GameUtil::MatchRegular(CDuiString& strIn, LPCTSTR lpReg)
{
	bool bResult = false;
	if (strIn.GetLength() <= 0 || lpReg == nullptr)
	{
		return bResult;
	}

	std::wregex pattern;
	LPWSTR cstr_words = nullptr;
#ifdef UNICODE || _UNICODE
	pattern = strReg.GetData();
	cstr_words = pzWords.GetData();
	bResult = std::regex_match(cstr_words, pattern);
#else
	LPWSTR cstr_reg = StringConvertor::AnsiToWide(lpReg);
	pattern = cstr_reg;

	cstr_words = StringConvertor::AnsiToWide(strIn.GetData());
	bResult = std::regex_match(cstr_words, pattern);

	StringConvertor::StringFree(cstr_words);
	StringConvertor::StringFree(cstr_reg);

#endif // UNICODE || _UNICODE

	return bResult;
}

void GameUtil::MinimizeTraySaveControl(CControlUI* control /* = NULL */)
{
	TrayMenu* traymenu_instance = TrayMenu::GetInstance();
	traymenu_instance->ResetControlSoon(control);
}

void GameUtil::MinimizeTray(HWND hWnd)
{
	TrayMenu* traymenu_instance = TrayMenu::GetInstance();
	traymenu_instance->ShowCurruntWindow(hWnd, SW_HIDE);
}

void GameUtil::SetFrozenEnable(CPaintManagerUI* pManager, bool bEnable)
{
	if (pManager)
	{
		CContainerUI* pRoot = static_cast<CContainerUI*>(pManager->GetRoot());
		if (pRoot)
		{
			pRoot->SetMouseEnabled(bEnable);
			pRoot->SetMouseChildEnabled(bEnable);
			pRoot->SetKeyboardEnabled(bEnable);
		}
	}
}

// 不用继承CWindowWnd，直接把该方法提出来写入公用类中,方便调用
void GameUtil::ShowTopWindow(HWND hWnd, bool bShow /* = true */, bool bTakeFocus /* = true */)
{
	if (!::IsWindow(hWnd)) return;

	WPARAM wParam = bShow ? (bTakeFocus ? SW_SHOW : SW_SHOWNOACTIVATE) : SW_HIDE;
	::SendMessage(hWnd, WM_SYSCOMMAND, wParam, 0);

	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::UpdateWindow(hWnd);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void GameUtil::RestoreTopWindow(HWND hWnd)
{
	if (!::IsWindow(hWnd)) return;

	::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::UpdateWindow(hWnd);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void GameUtil::OpenWebByURL(LPCTSTR lpName)
{
	ASSERT(lpName);
	if (!lpName || _tcscmp(lpName, _T("")) == 0) return;
	::ShellExecute(NULL, _T("open"), lpName, NULL, NULL, SW_SHOWNORMAL);
}

void GameUtil::OpenWebByName(LPCTSTR lpName)
{
	ASSERT(lpName);
	if (!lpName || _tcscmp(lpName, _T("")) == 0) return;

	ServiceData* pServiceData = ServiceData::GetInstance();
	LPCTSTR lpUrl = pServiceData->GetUrlWeb(lpName);
	this->OpenWebByURL(lpUrl);
}

LPCTSTR GameUtil::GetWebByName(LPCTSTR lpName)
{
	LPCTSTR lpUrl = nullptr;
	if (lpName)
	{
		ServiceData* pServiceData = ServiceData::GetInstance();
		lpUrl = pServiceData->GetUrlWeb(lpName);
	}

	return lpUrl;
}

void GameUtil::CreatTrayMenu(CPaintManagerUI* pParentManager)
{
	TrayMenu* traymenu_instance = TrayMenu::GetInstance();
	if (traymenu_instance == nullptr) return;
	traymenu_instance->CreateFrame(pParentManager);
}

void GameUtil::RunWndUnFocusAction(HWND hWnd, DWORD dwActTimes /* = 3 */, DWORD dwActOffset /* = 5 */)
{
	if (FALSE == ::IsWindow(hWnd)) return;

	UINT_PTR idEvent = (UINT_PTR)hWnd;
	if (m_vUnFocusWndMap.find(idEvent) == m_vUnFocusWndMap.end())
	{
		TimerCallBack timepro = CC_CALLBACK_2(GameUtil::ProFuncWndUnFocus, this);
		this->SetTimer(hWnd, timepro, idEvent, 40);

		stuUnFoucsWnd* stu = new stuUnFoucsWnd();
		stu->hWnd = hWnd;
		stu->dwActCount = 0;
		stu->dwActTimes = dwActTimes;
		stu->dwActOffset = dwActOffset;
		::GetWindowRect(stu->hWnd, &stu->rectSrc);

		m_vUnFocusWndMap[idEvent] = stu;
	}
}

void GameUtil::ProFuncWndUnFocus(UINT_PTR idEvent, DWORD dwTime)
{
	std::map<UINT_PTR, stuUnFoucsWnd*>::iterator& it = m_vUnFocusWndMap.find(idEvent);
	if (it != m_vUnFocusWndMap.end())
	{
		stuUnFoucsWnd* stu = it->second;
		RECT rc = stu->rectSrc;
		
		if (stu->dwActCount < stu->dwActTimes * 2)
		{
			if (stu->dwActCount % 2)
			{
				rc.left = stu->rectSrc.left - stu->dwActOffset;
				rc.right = stu->rectSrc.right - stu->dwActOffset;
			}
			else
			{
				rc.left = stu->rectSrc.left + stu->dwActOffset;
				rc.right = stu->rectSrc.right + stu->dwActOffset;
			}
			++stu->dwActCount;
			::SetWindowPos(stu->hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		}
		else
		{
			::SetWindowPos(stu->hWnd, HWND_TOP, stu->rectSrc.left, stu->rectSrc.top
				, stu->rectSrc.right - stu->rectSrc.left, stu->rectSrc.bottom - stu->rectSrc.top, SWP_SHOWWINDOW);
			this->KillTimer(stu->hWnd, idEvent);
			
			SAFE_DELETE(stu);
			m_vUnFocusWndMap.erase(it);
		}
	}
}

void GameUtil::StopWndUnFocusAction(HWND hWnd)
{
	UINT_PTR idEvent = (UINT_PTR)hWnd;
	std::map<UINT_PTR, stuUnFoucsWnd*>::iterator& it = m_vUnFocusWndMap.find(idEvent);
	if (it != m_vUnFocusWndMap.end())
	{
		stuUnFoucsWnd* stu = it->second;

		::SetWindowPos(stu->hWnd, HWND_TOP, stu->rectSrc.left, stu->rectSrc.top
			, stu->rectSrc.right - stu->rectSrc.left, stu->rectSrc.bottom - stu->rectSrc.top, SWP_SHOWWINDOW);

		this->KillTimer(stu->hWnd, idEvent);

		SAFE_DELETE(stu);
		m_vUnFocusWndMap.erase(it);
	}
}

void GameUtil::SetCurPanel(CPaintManagerUI* pManager
	, Tls_Panel tls_panel /* = Tls_Panel::TLS_PANEL_HALL  */
	, Tls_SideBar tls_sidebar /* = Tls_SideBar::TLS_SIDEBAR_HALL */)
{
	if (!pManager)
		return;

	int panel_index = -1;
	int siderbar_index = -1;
	CTabLayoutUI* panel_switch = nullptr;
	CTabLayoutUI* sidebar_switch = nullptr;
	
#ifdef GAMEPLAZA_STYLE_1
	panel_switch = static_cast<CTabLayoutUI*>(pManager->FindControl(R->kTlsHallMain_HallPanel));
	sidebar_switch = static_cast<CTabLayoutUI*>(pManager->FindControl(R->kTlsHallMain_SiderBar));
#else if GAMEPLAZA_STYLE_2
	panel_switch = static_cast<CTabLayoutUI*>(pManager->FindControl(R->kTlsHallMain_HallPanel));
#endif

	switch (tls_panel)
	{
		case Tls_Panel::TLS_PANEL_HALL:
		{
			panel_index = 0;
		}break;
		case Tls_Panel::TLS_PANEL_ROOM_LIST:
		{
#ifdef GAMEPLAZA_STYLE_1
			panel_index = 1;
#else if GAMEPLAZA_STYLE_2
			panel_index = 0;
#endif
		}break;
		case Tls_Panel::TLS_PANEL_ROOM:
		{
#ifdef GAMEPLAZA_STYLE_1
			panel_index = 2;
#else if GAMEPLAZA_STYLE_2
			panel_index = 1;
#endif
		}break;

		default:
			break;
	}

	switch (tls_sidebar)
	{
		case Tls_SideBar::TLS_SIDEBAR_HALL:
			siderbar_index = 0;
			break;
		case Tls_SideBar::TLS_SIDEBAR_ROOM_LIST:
			siderbar_index = 0;
			break;
		case Tls_SideBar::TLS_SIDEBAR_ROOM:
			siderbar_index = 1;
			break;
		default:
			break;
	}

	//更换主面板
	if (panel_switch && panel_index >= 0)
	{
		m_cur_panel = tls_panel;
		panel_switch->SelectItem(panel_index);
	}

	//更换侧栏面板
	if (sidebar_switch && siderbar_index >= 0)
	{
		sidebar_switch->SelectItem(siderbar_index);
	}
}

Tls_Panel GameUtil::GetCurPanel()
{
	return m_cur_panel;
}

DWORD GameUtil::TransARGBToRGB(DWORD dwValue)
{
	//int a = LOBYTE((dwValue) >> 24);
	BYTE r = GetBValue(dwValue);
	BYTE g = GetGValue(dwValue);
	BYTE b = GetRValue(dwValue);
	return RGB(r, g, b);
}

DWORD GameUtil::TransRGBToARGB(DWORD dwValue)
{
	BYTE a = 0xFF;
	BYTE r = GetRValue(dwValue);
	BYTE g = GetGValue(dwValue);
	BYTE b = GetBValue(dwValue);

	int dwReturn = (DWORD)(a << 24) | (DWORD)(r << 16) | (DWORD)(g << 8) | (DWORD)(b);
	return dwReturn;
}

DWORD GameUtil::TransARGBToRGBA(DWORD dwValue)
{
	BYTE a = LOBYTE((dwValue) >> 24);
	BYTE r = GetBValue(dwValue);
	BYTE g = GetGValue(dwValue);
	BYTE b = GetRValue(dwValue);

	int dwReturn = (DWORD)(r << 24) | (DWORD)(g << 16) | (DWORD)(b << 8) | (DWORD)(a);
	return dwReturn;
}

////////////////////////////////////////////// 文件操作 //////////////////////////////////////////////
void GameUtil::traverseFolder(LPCTSTR lpstrFilter, std::vector<FILE_STRUCT>& files, BOOL isCircle /* = FALSE */, LPCTSTR lpExName /* = nullptr */)
{
	TCHAR szFind[MAX_PATH] = { _T("\0") };
	WIN32_FIND_DATA findFileData;
	BOOL bRet;

	_tcscpy_s(szFind, MAX_PATH, lpstrFilter);
	_tcscat_s(szFind, _T("\\*.*"));     //这里一定要指明通配符，不然不会读取所有文件和目录

	HANDLE hFind = ::FindFirstFile(szFind, &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}

	//遍历文件夹
	FILE_STRUCT file;
	while (TRUE)
	{
		//不是当前路径或者父目录的快捷方式
		if (findFileData.cFileName[0] != _T('.'))
		{
			_tprintf(_T("%s\\%s\n"), lpstrFilter, findFileData.cFileName);

			//这是一个普通目录(继续遍历子目录)
			if (isCircle && findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//设置下一个将要扫描的文件夹路径
				_tcscpy_s(szFind, MAX_PATH, lpstrFilter);
				_tcscat_s(szFind, _T("\\"));
				_tcscat_s(szFind, findFileData.cFileName);
				///_tcscat_s(szNextDir, _T("\\*"));

				//遍历该目录
				traverseFolder(szFind, files, isCircle);
			}
			else
			{
				file.fileName = findFileData.cFileName;
				file.size = findFileData.nFileSizeLow - findFileData.nFileSizeHigh;
				file.name = this->GetFileNameWithoutExtension(file.fileName.c_str()).c_str();
				file.exName = this->GetExtension(file.fileName.c_str()).c_str();
				if (!lpExName || _tcscmp(lpExName, _T("")) == 0 || _tcscmp(lpExName, file.exName.c_str()) == 0)
				{
					files.push_back(file);
				}
			}
		}
		//如果是当前路径或者父目录的快捷方式，或者是普通目录，则寻找下一个目录或者文件
		bRet = ::FindNextFile(hFind, &findFileData);
		if (!bRet)
		{//函数调用失败
			//cout << "FindNextFile failed, error code: " 
			//  << GetLastError() << endl;
			break;
		}
	}

	::FindClose(hFind);
}

bool GameUtil::openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti /* = false */)
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

bool GameUtil::saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath)
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

BOOL GameUtil::IsDirectory(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if ((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

BOOL GameUtil::IsFileExist(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	if (::GetFileAttributes(lpszFileName) != 0xFFFFFFFF)
		return TRUE;
	else
		return FALSE;
}

BOOL GameUtil::IsDirectoryExist(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if ((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

BOOL GameUtil::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	TCHAR cPath[MAX_PATH] = { 0 };
	TCHAR cTmpPath[MAX_PATH] = { 0 };
	TCHAR * lpPos = NULL;
	TCHAR cTmp = _T('\0');

	if (NULL == lpPathName || NULL == *lpPathName)
		return FALSE;

	_tcsncpy(cPath, lpPathName, MAX_PATH);

	for (int i = 0; i < (int)_tcslen(cPath); i++)
	{
		if (_T('\\') == cPath[i])
			cPath[i] = _T('/');
	}

	lpPos = _tcschr(cPath, _T('/'));
	while (lpPos != NULL)
	{
		if (lpPos == cPath)
		{
			lpPos++;
		}
		else
		{
			cTmp = *lpPos;
			*lpPos = _T('\0');
			_tcsncpy(cTmpPath, cPath, MAX_PATH);
			::CreateDirectory(cTmpPath, lpSecurityAttributes);
			*lpPos = cTmp;
			lpPos++;
		}
		lpPos = _tcschr(lpPos, _T('/'));
	}

	return TRUE;
}

BOOL GameUtil::DeleteDirectory(LPCTSTR lpPathName)
{
	BOOL bRet = FALSE;
	WIN32_FIND_DATA tFindFileData;
	memset(&tFindFileData, 0, sizeof(WIN32_FIND_DATA));

	// 这里一定要指明通配符，不然不会读取所有文件和目录
	TCHAR szFind[MAX_PATH] = { _T("\0") };
	_tcscpy_s(szFind, MAX_PATH, lpPathName);
	_tcscat_s(szFind, _T("\\*.*"));

	HANDLE hFind = ::FindFirstFile(szFind, &tFindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}

	do
	{
		if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_tcscmp(tFindFileData.cFileName, ".") != 0 && _tcscmp(tFindFileData.cFileName, "..") != 0)
			{
				//发现子目录，递归之
				CString strFile;
				strFile += lpPathName;
				strFile += _T("\\");
				strFile += tFindFileData.cFileName;
				GameUtil::DeleteDirectory(strFile);
			}
		}
		else
		{
			//找到文件，处理之
			CString strFile;
			strFile += lpPathName;
			strFile += _T("\\");
			strFile += tFindFileData.cFileName;
			
			::DeleteFile(strFile);
		}
	} while (::FindNextFile(hFind, &tFindFileData));

	::FindClose(hFind);

	// 删除根目录
	if (!RemoveDirectory(lpPathName))
	{
		return FALSE;
	}

	return TRUE;
}

tstring GameUtil::GetAppPath()
{
	static TCHAR szPath[MAX_PATH] = { 0 };

	if (szPath[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szPath, MAX_PATH);

		tstring strPath = GetDirectoryName(szPath);
		_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
	}

	return szPath;
}

tstring GameUtil::GetCurDir()
{
	TCHAR szCurDir[MAX_PATH] = { 0 };
	::GetCurrentDirectory(MAX_PATH, szCurDir);

	if (szCurDir[_tcslen(szCurDir) - 1] != _T('\\'))
		_tcscat(szCurDir, _T("\\"));

	return szCurDir;
}

tstring GameUtil::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	::GetTempPath(MAX_PATH, szTempPath);

	if (szTempPath[_tcslen(szTempPath) - 1] != _T('\\'))
		_tcscat(szTempPath, _T("\\"));

	return szTempPath;
}

tstring GameUtil::GetTempFileName(LPCTSTR lpszFileName)
{
	return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
}

tstring GameUtil::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	tstring strPath, strFileName, strExtFileName, strFullPath;
	TCHAR szBuf[MAX_PATH] = { 0 };

	if (!IsDirectoryExist(lpszPath))
		strPath = GetCurDir();
	else
		strPath = lpszPath;

	strFileName = GetFileNameWithoutExtension(lpszFileName);
	strExtFileName = GetExtension(lpszFileName);

	for (int i = 2; i < 10000; i++)
	{
		if (strExtFileName.empty())
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			_stprintf_s(szBuf, _T("%d"), i);
			strFullPath += szBuf;
		}
		else
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			_stprintf_s(szBuf, _T("%d."), i);
			strFullPath += szBuf;
			strFullPath += strExtFileName;
		}

		if (!IsFileExist(strFullPath.c_str()))
			return strFullPath;
	}

	return _T("");
}

tstring GameUtil::GetPathRoot(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.find(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos + 1);

	return strPath;
}

tstring GameUtil::GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos + 1);

	return strPath;
}

tstring GameUtil::GetFileName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(nPos + 1);

	return strPath;
}

tstring GameUtil::GetFileNameWithoutExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(nPos + 1);

	nPos = strPath.rfind(_T('.'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos);

	return strPath;
}

tstring GameUtil::GetExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::size_type nPos = strPath.rfind(_T('.'));
	if (nPos != tstring::npos)
	{
		strPath = strPath.substr(nPos + 1);
	}
	else
	{
		strPath.clear();
	}

	return strPath;
}

tstring GameUtil::GetFullPath(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	TCHAR cFirstChar = strPath.at(0);
	TCHAR cSecondChar = strPath.at(1);

	if (cFirstChar == _T('/'))
	{
		tstring strCurDir = GetAppPath();
		tstring strRootPath = GetPathRoot(strCurDir.c_str());
		return strRootPath + strPath;
	}
	else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
	{
		return strPath;
	}
	else
	{
		tstring strCurDir = GetAppPath();
		return strCurDir + strPath;
	}
}

GUID GameUtil::GetFileExtensionGuid(LPCTSTR lpExtension)
{
	GUID guid = GUID_NULL;

	if (NULL == lpExtension)
		return guid;

	UINT numEncoders = 0, size = 0;
	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
	if (status != Gdiplus::Ok)
		return guid;

	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (NULL == lpEncoders)
		return guid;

	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
	if (Gdiplus::Ok == status)
	{
		for (UINT i = 0; i < numEncoders; i++)
		{
			BOOL bFind = FALSE;
			LPTSTR pTempStr = nullptr;
			

#ifdef UNICODE
			pTempStr = StringConvertor::WideStrDup(lpEncoders[i].FilenameExtension);
#else
			pTempStr = StringConvertor::WideToAnsi(lpEncoders[i].FilenameExtension);
#endif // UNICODE
			LPTSTR pStart = pTempStr;
			LPTSTR pEnd = _tcsrchr(pStart, _T(';'));
			do
			{
				if (NULL == pEnd)
				{
					LPTSTR lpExt = _tcsrchr(pStart, _T('.'));
					if ((lpExt != NULL) && (_tcsicmp(lpExt, lpExtension) == 0))
					{
						guid = lpEncoders[i].FormatID;
						bFind = TRUE;
					}
					break;
				}

				int nLen = pEnd - pStart;
				if (nLen < MAX_PATH)
				{
					TCHAR cBuf[MAX_PATH] = { 0 };
					_tcsncpy(cBuf, pStart, nLen);
					LPTSTR lpExt = _tcsrchr(cBuf, _T('.'));
					if ((lpExt != NULL) && (_tcsicmp(lpExt, lpExtension) == 0))
					{
						guid = lpEncoders[i].FormatID;
						bFind = TRUE;
						break;
					}
				}
				pStart = pEnd + 1;
				if (_T('\0') == *pStart)
					break;
				pEnd = _tcschr(pStart, _T(';'));
			} while (1);

			StringConvertor::StringFree(pTempStr);
			if (bFind)
				break;
		}
	}

	free(lpEncoders);

	return guid;
}

////////////////////////////////////////////// 获取硬件信息 //////////////////////////////////////////////
BOOL GameUtil::GetPhyDriveSerial(LPTSTR pModelNo, LPTSTR pSerialNo)
{
	//-1是因为 SENDCMDOUTPARAMS 的结尾是 BYTE bBuffer[1];
	BYTE IdentifyResult[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
	DWORD dwBytesReturned;
	GETVERSIONINPARAMS get_version;
	SENDCMDINPARAMS send_cmd = { 0 };

	HANDLE hFile = CreateFile(_T("\\\\.\\PHYSICALDRIVE0"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int e_code = GetLastError();
		return FALSE;
	}

	//get version
	DeviceIoControl(hFile, SMART_GET_VERSION, NULL, 0,
		&get_version, sizeof(get_version), &dwBytesReturned, NULL);

	//identify device
	send_cmd.irDriveRegs.bCommandReg = (get_version.bIDEDeviceMap & 0x10) ? ATAPI_ID_CMD : ID_CMD;
	DeviceIoControl(hFile, SMART_RCV_DRIVE_DATA, &send_cmd, sizeof(SENDCMDINPARAMS) - 1,
		IdentifyResult, sizeof(IdentifyResult), &dwBytesReturned, NULL);
	CloseHandle(hFile);

	//adjust the byte order
	PUSHORT pWords = (USHORT*)(((SENDCMDOUTPARAMS*)IdentifyResult)->bBuffer);
	ToLittleEndian(pWords, 27, 46, pModelNo);
	ToLittleEndian(pWords, 10, 19, pSerialNo);
	return TRUE;
}

void GameUtil::ToLittleEndian(PUSHORT pWords, int nFirstIndex, int nLastIndex, LPTSTR pBuf)
{
	int index;
	LPTSTR pDest = pBuf;
	for (index = nFirstIndex; index <= nLastIndex; ++index)
	{
		pDest[0] = pWords[index] >> 8;
		pDest[1] = pWords[index] & 0xFF;
		pDest += 2;
	}
	*pDest = 0;

	//trim space at the endof string; 0x20: _T(' ')
	--pDest;
	while (*pDest == 0x20)
	{
		*pDest = 0;
		--pDest;
	}
}

void GameUtil::TrimStart(LPTSTR pBuf)
{
	if (*pBuf != 0x20)
		return;

	LPTSTR pDest = pBuf;
	LPTSTR pSrc = pBuf + 1;
	while (*pSrc == 0x20)
		++pSrc;

	while (*pSrc)
	{
		*pDest = *pSrc;
		++pDest;
		++pSrc;
	}
	*pDest = 0;
}

BOOL GameUtil::GetActiveMac(LPTSTR pBuf)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
	}
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		char szMac[20] = { 0 };
		char szAddress[4] = { 0 };
		while (pAdapter)
		{

			if (strcmp(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0") != 0)
			{
				for (UINT i = 0; i < pAdapter->AddressLength; i++)
				{
					sprintf_s(szAddress, "%02X", pAdapter->Address[i]);
					strcat_s(szMac, szAddress);
					if (i != pAdapter->AddressLength - 1)
					{
						strcat_s(szMac, "-");
					}
				}
				break;
			}
			pAdapter = pAdapter->Next;
		}
		memcpy(pBuf, szMac, _tcslen(szMac) + 1);
		return TRUE;
	}

	return FALSE;
}

BOOL GameUtil::GetCPU(LPTSTR pBuf)
{
	BOOL bException = FALSE;
	BYTE szCpu[16] = { 0 };
	UINT uCpuID = 0U;
	__try
	{
		_asm
		{
			mov eax, 0
				cpuid
				mov dword ptr szCpu[0], ebx
				mov dword ptr szCpu[4], edx
				mov dword ptr szCpu[8], ecx
				mov eax, 1
				cpuid
				mov uCpuID, edx
		}
	}

	__except (EXCEPTION_EXECUTE_HANDLER)

	{
		bException = TRUE;
	}

	wsprintf(pBuf, _T("%s"), szCpu);

	return TRUE;
}

////////////////////////////////////////////// 字符串操作 //////////////////////////////////////////////
CString GameUtil::newGUID()
{
	CString buf;
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		buf.Format(_T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]);
	}
	return buf;
}

tstring GameUtil::GetMachineString()
{
	tstring strMachine;
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	if (pUnrealControl && pUnrealControl->IsMachineIDSwitchOn())
	{
		TCHAR pzMachineID[COMPUTER_ID_LEN] = { 0 };
		pUnrealControl->GetMachineID(pzMachineID);
		strMachine = pzMachineID;
	}
	else
	{
		// 网卡 MAC 地址，注意: MAC 地址是可以在注册表中修改的
		memset(this->m_pzBuffer, 0, ArrayCount(this->m_pzBuffer) * sizeof(TCHAR));
		this->GetActiveMac(this->m_pzBuffer);
		strMachine += this->m_pzBuffer;

		// 硬盘序列号，注意：有的硬盘没有序列号
		TCHAR szBuf1[48] = { 0 }, szBuf2[24] = { 0 };
		if (this->GetPhyDriveSerial(szBuf1, szBuf2))
		{
			this->TrimStart(szBuf2);
		}
		strMachine += szBuf2;

		// CPU
		memset(this->m_pzBuffer, 0, ArrayCount(this->m_pzBuffer) * sizeof(TCHAR));
		this->GetCPU(this->m_pzBuffer);
		strMachine += this->m_pzBuffer;
	}

	// 机器码转为MD5
	//memset(this->m_pzBuffer, 0, ArrayCount(this->m_pzBuffer) * sizeof(TCHAR));
	//CMD5Encrypt::EncryptData(strMachine.c_str(), this->m_pzBuffer);
	//strMachine = this->m_pzBuffer;

	return strMachine;
}

const TCHAR * GameUtil::FormatTime(time_t lTime, LPCTSTR lpFmt)
{
	struct tm * lpTimeInfo;
	static TCHAR cTime[32];

	memset(cTime, 0, sizeof(cTime));

	lpTimeInfo = localtime(&lTime);
	if (lpTimeInfo != NULL)
		_tcsftime(cTime, sizeof(cTime) / sizeof(TCHAR), lpFmt, lpTimeInfo);
	return cTime;
}

BOOL GameUtil::IsDigit(const TCHAR * lpStr)
{
	for (const TCHAR * p = lpStr; *p != _T('\0'); p++)
	{
		if (!isdigit(*p))
			return FALSE;
	}
	return TRUE;
}

void GameUtil::Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr)
{
	if (NULL == lpOldStr || NULL == lpNewStr)
		return;

	int nOldStrLen = wcslen(lpOldStr);
	int nNewStrLen = wcslen(lpNewStr);

	tstring::size_type nPos = 0;
	while ((nPos = strText.find(lpOldStr, nPos)) != tstring::npos)
	{
		strText.replace(nPos, nOldStrLen, lpNewStr);
		nPos += nNewStrLen;
	}
}

tstring GameUtil::GetBetweenString(const TCHAR * pStr, UINT cStart, UINT cEnd)
{
	tstring strText;

	if (NULL == pStr)
		return _T("");

	const TCHAR * p1 = _tcschr(pStr, cStart);
	if (NULL == p1)
		return _T("");

	const TCHAR * p2 = _tcschr(p1 + 1, cEnd);
	if (NULL == p2)
		return _T("");

	int nLen = p2 - (p1 + 1);
	if (nLen <= 0)
		return _T("");

	TCHAR * lpText = new TCHAR[nLen + 1];
	if (NULL == lpText)
		return _T("");

	memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
	_tcsncpy(lpText, p1 + 1, nLen);
	strText = lpText;
	delete[]lpText;

	return strText;
}

tstring GameUtil::GetBetweenString(const TCHAR * pStr, const TCHAR* pStart, const TCHAR* pEnd)
{
	tstring strText;

	if (NULL == pStr || NULL == pStart || NULL == pEnd)
		return _T("");

	int nStartLen = _tcslen(pStart);

	const TCHAR * p1 = _tcsstr(pStr, pStart);
	if (NULL == p1)
		return _T("");

	const TCHAR * p2 = _tcsstr(p1 + nStartLen, pEnd);
	if (NULL == p2)
		return _T("");

	int nLen = p2 - (p1 + nStartLen);
	if (nLen <= 0)
		return _T("");

	TCHAR * lpText = new TCHAR[nLen + 1];
	if (NULL == lpText)
		return _T("");

	memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
	_tcsncpy(lpText, p1 + nStartLen, nLen);
	strText = lpText;
	delete[]lpText;

	return strText;
}

int GameUtil::GetBetweenInt(const TCHAR* pStr, UINT cStart, UINT cEnd, int nDefValue /* = 0 */)
{
	tstring strText = GetBetweenString(pStr, cStart, cEnd);
	if (!strText.empty() && IsDigit(strText.c_str()))
		return _tcstol(strText.c_str(), NULL, 10);
	else
		return nDefValue;
}

int GameUtil::GetBetweenInt(const TCHAR* pStr, const TCHAR* pStart, const TCHAR* pEnd, int nDefValue /* = 0 */)
{
	tstring strText = GetBetweenString(pStr, pStart, pEnd);
	if (!strText.empty() && IsDigit(strText.c_str()))
		return _tcstol(strText.c_str(), NULL, 10);
	else
		return nDefValue;
}

///////////////////////////////////////////////////////// 定时器操作 /////////////////////////////////////////////////////////
void GameUtil::SetTimer(HWND hWnd, TimerCallBack pCallBack, UINT_PTR nIDEvent, UINT uElapse)
{
	m_WndTimmer.SetTimer(hWnd, pCallBack, nIDEvent, uElapse);
}

void GameUtil::KillTimer(HWND hWnd, UINT_PTR nIDEvent)
{
	m_WndTimmer.KillTimer(hWnd, nIDEvent);
}

void GameUtil::RemoveAllTimers()
{
	m_WndTimmer.RemoveAllTimers();
}

void GameUtil::SendSystemMsgText(CRichEditUI* pEdit, LPCTSTR pzText)
{
	if (!pEdit || !pzText) return;

	ServiceData* pServiceData = ServiceData::GetInstance();
	ChatColorConfig* pChatColorConfig = pServiceData->GetChatColorConfig();
	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();

	g_RichEdtUtil->ParseSystemMsg(pEdit, pzText, pChatColorConfig);

	//CRichEditUI* pSendEdit = static_cast<CRichEditUI*>(pEdit->GetManager()->FindControl(R->kEdtChatPanel_Input));
	//if (pSendEdit && pSendEdit->IsFocused())
	//{
	//	pEdit->GetManager()->SetFocusNeeded(pSendEdit);
	//}
}

void GameUtil::SendChatMsgText(CRichEditUI* pEdit, UserData& tUser, const LPCTSTR pzContent)
{
	if (!pEdit || !pzContent) return;
	
	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	g_RichEdtUtil->AddMsgToViewsEdit(pEdit, tUser, pzContent);

	CRichEditUI* pSendEdit = static_cast<CRichEditUI*>(pEdit->GetManager()->FindControl(R->kEdtChatPanel_Input));
	if (pSendEdit && pSendEdit->IsFocused())
	{
		pEdit->GetManager()->SetFocusNeeded(pSendEdit);
	}
}

void GameUtil::SendBuyPropsMsgText(CRichEditUI* pEdit, CMD_GF_PropsNotify* pStuOk)
{
	if (!pEdit || !pStuOk) return;
	_stprintf_s(m_pzBuffer, _T("%shall\\mall\\goods\\mini_%u.png"), CPaintManagerUI::GetResourcePath().GetData(), pStuOk->wGiftID);

	ServiceData* pServiceData = ServiceData::GetInstance();
	ChatColorConfig* pChatColorConfig = pServiceData->GetChatColorConfig();

	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	g_RichEdtUtil->SendBuyPropsMsg(pEdit, pStuOk->wCount, pStuOk->szPropsName, m_pzBuffer, pStuOk->lSrcCharm, pStuOk->lRecCharm, pChatColorConfig);
}

void GameUtil::SendGivePropsMsgText(CRichEditUI* pEdit, CMD_GF_PropsNotify* pStuOk)
{
	if (!pEdit || !pStuOk) return;
	_stprintf_s(m_pzBuffer, _T("%shall\\mall\\goods\\mini_%u.png"), CPaintManagerUI::GetResourcePath().GetData(), pStuOk->wGiftID);

	ServiceData* pServiceData = ServiceData::GetInstance();
	ChatColorConfig* pChatColorConfig = pServiceData->GetChatColorConfig();

	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	g_RichEdtUtil->SendGivePropsMsg(pEdit, pStuOk->szSrcNickname, pStuOk->dwSrcGameID, pStuOk->szRcvNickname, pStuOk->dwRcvGameID
		, pStuOk->wCount, pStuOk->szPropsName, m_pzBuffer, pStuOk->lSrcCharm, pStuOk->lRecCharm, pChatColorConfig);
}

void GameUtil::SendUserLeaveText(CRichEditUI* pEdit, LPCTSTR pzName, LPCTSTR pzText)
{
	if (!pEdit) return;

	ServiceData* pServiceData = ServiceData::GetInstance();
	ChatColorConfig* pChatColorConfig = pServiceData->GetChatColorConfig();

	RichEditUtil* g_RichEdtUtil = RichEditUtil::GetInstance();
	g_RichEdtUtil->SendUserLeaveMsg(pEdit, pzName, pzText, pChatColorConfig);
}

bool GameUtil::LoadSensitiveVocabulary()
{
	HMODULE hInstance = GetModuleHandle(NULL);
	HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(IDR_TXT_SensitiveVocabulary), _T("TXT"));
	if (hResource == NULL) return false;

	HGLOBAL hGlobal = ::LoadResource(hInstance, hResource);
	if (hGlobal == NULL)
	{
#if defined(WIN32) && !defined(UNDER_CE)
		::FreeResource(hResource);
#endif
		return false;
	}

	DWORD dwSize = ::SizeofResource(hInstance, hResource);
	if (dwSize == 0) return false;

	BYTE* pData = new BYTE[dwSize];
	::ZeroMemory(pData, dwSize * sizeof(BYTE));
	::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
	::FreeResource(hResource);

	dwSize = ::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)pData, -1, NULL, 0);
	WCHAR* wcstr = new WCHAR[dwSize];
	::ZeroMemory(wcstr, dwSize * sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)pData, -1, wcstr, dwSize);
	SAFE_DELETE_ARRAY(pData);

#ifdef TRUE// UNICODE || _UNICODE
	m_tstrSensitiveVocabulary = wcstr;
#else
	dwSize = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, NULL, 0, NULL, FALSE);
	char* cstr = new char[dwSize];
	::ZeroMemory(cstr, dwSize * sizeof(char));
	::WideCharToMultiByte(CP_ACP, NULL, wcstr, -1, cstr, dwSize, NULL, FALSE);
	m_tstrSensitiveVocabulary = cstr;
	SAFE_DELETE_ARRAY(cstr);
#endif // UNICODE || _UNICODE
	SAFE_DELETE_ARRAY(wcstr);

	return true;
}

bool GameUtil::CheckSensitiveVocabulary(CDuiString& pzOut, bool bReplace /* = false */)
{
	if (pzOut.GetLength() <= 0 || _tcscmp(pzOut.GetData(), _T(" ")) == 0) return false;

	int ipos1 = 0;
	int ipos2 = 0;

	bool bMatch = false;
	std::wstring tsPattern;
	std::map<int, bool> vReplaceMap;

	// 窄字符转宽字符
	do 
	{
		int isize = ::MultiByteToWideChar(CP_ACP, 0, pzOut.GetData(), -1, NULL, 0);
		WCHAR* wcstr = new WCHAR[isize];
		::ZeroMemory(wcstr, isize * sizeof(WCHAR));
		::MultiByteToWideChar(CP_ACP, 0, pzOut.GetData(), -1, wcstr, isize);
		tsPattern = wcstr;
		SAFE_DELETE_ARRAY(wcstr);
	} while (0);

	// 开始检索
	do 
	{
		std::wstring tsTmp;
		for (int j = 0; j < m_tstrSensitiveVocabulary.size(); ++j)
		{
			if (m_tstrSensitiveVocabulary[j] == '\r')
			{
				ipos2 = j;
				tsTmp = m_tstrSensitiveVocabulary.substr(ipos1, ipos2 - ipos1).c_str();
				ipos1 = j + 2;

				int iTmp = tsPattern.find(tsTmp);
				if (iTmp != -1)
				{
					bMatch = true;

					if (bReplace)
					{
						for (int m = 0; m < tsPattern.size(); ++m)
						{
							for (int n = 0; n < tsTmp.size(); ++n)
							{
								if (tsPattern[m] == tsTmp[n])
								{
									vReplaceMap[m] = true;
								}
							}
						}
					}
				}
			}
		}
	} while (0);

	if (bMatch && bReplace)
	{
		std::wstring wcstr = L"*";
		for (const auto& it : vReplaceMap)
		{
			tsPattern.replace(it.first, wcstr.size(), wcstr);
		}

		// 宽字符转窄字符
		LPCWSTR wstr = tsPattern.c_str();
		int isize = ::WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, FALSE);
		char* cstr = new char[isize];
		::ZeroMemory(cstr, isize * sizeof(char));
		::WideCharToMultiByte(CP_ACP, NULL, wstr, -1, cstr, isize, NULL, FALSE);
		pzOut = cstr;
		SAFE_DELETE_ARRAY(cstr);
	}

	return bMatch;
}

bool GameUtil::GetSkinResPath(LPCTSTR lpFolderName, CDuiString& pzOutPath)
{
	std::vector<FILE_STRUCT> files;
	LPCTSTR lpExName = _T("");

#ifndef SKIN_RES_TEST
	lpExName = _T("dll");
#endif
	this->traverseFolder(lpFolderName, files, FALSE, lpExName);

	if (files.size() > 0)
	{
		if (GameResource::g_SkinResID >= files.size())
		{
			GameResource::g_SkinResID = 0;
		}

		pzOutPath = files[GameResource::g_SkinResID].fileName.c_str();
		return true;
	}

	return false;
}

// 读取皮肤配置
int GameUtil::ReadSkinConfig()
{
	int iSkinIndex = 0;
	CDuiString pzModulePath = CPaintManagerUI::GetInstancePath();
	pzModulePath += _T("GamePlaza\\data\\user\\INFO.lm");

	Json::Value vRoot;
	JsonFile* pJsonFile = new JsonFile(pzModulePath, true);
	bool bRes = pJsonFile->initRootNode(vRoot);
	SAFE_DELETE(pJsonFile);
	if (bRes)
	{
		if (!vRoot["AccountInfo"].empty() && vRoot["AccountInfo"].isArray())
		{
			Json::Value& vNode = vRoot["AccountInfo"][0];
			if (!vNode.empty() && !vNode["iSkinIndex"].empty() && vNode["iSkinIndex"].isInt())
			{
				iSkinIndex = vNode["iSkinIndex"].asInt();
			}
		}
	}

	return iSkinIndex;
}

// 保存皮肤配置
void GameUtil::SaveSkinConfig(LPCTSTR lpAccount)
{
	CDuiString pzModulePath = CPaintManagerUI::GetInstancePath();
	pzModulePath += _T("GamePlaza\\data\\user\\INFO.lm");

	Json::Value vRoot;
	JsonFile* pJsonFile = new JsonFile(pzModulePath, true);
	bool bRes = pJsonFile->initRootNode(vRoot);
	if (bRes)
	{
		if (!vRoot["AccountInfo"].empty() && vRoot["AccountInfo"].isArray())
		{
			for (int i = 0; i < vRoot["AccountInfo"].size(); ++i)
			{
				Json::Value& vNode = vRoot["AccountInfo"][i];
				LPCTSTR pzAccount = vNode["pzAccount"].asCString();
				if (_tcscmp(pzAccount, lpAccount) == 0)
				{
					vNode["iSkinIndex"] = GameResource::g_SkinResID;
					pJsonFile->SaveFile(vRoot);
					break;
				}
			}
		}
	}
	SAFE_DELETE(pJsonFile);
}

// 加载资源dll
bool GameUtil::LoadSkinResFromDll(LPCTSTR lpFileName)
{
	ASSERT(R);
	if (!R) return false;
	if (!lpFileName || _tcscmp(lpFileName, _T("")) == 0) return false;
	
	//GameResource::g_ResourceInst = ::LoadLibraryEx(lpFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	GameResource::g_ResourceInst = ::LoadLibrary(lpFileName);
	if (GameResource::g_ResourceInst == NULL)
	{
		int e_code = ::GetLastError();
		g_Logger->TryError(_T("Load Skin Dll Failed [%u]"), e_code);

		::FreeLibrary(GameResource::g_ResourceInst);
		return false;
	}

	// 从Dll资源压缩包中读取
	GameResource::g_ResourceType = UILIB_RESOURCETYPE::UILIB_ZIPRESOURCE;

	// 压缩包资源id
#ifdef SKINE_ZIPRE_ID
	GameResource::g_ResourceID = MAKEINTRESOURCE(SKINE_ZIPRE_ID);
#else
	GameResource::g_ResourceID = _T("");
#endif //SKINE_ZIPRE_ID

	return true;
}

void GameUtil::EndHall()
{
	g_Logger->TryInfo(_T("GameUtil - End Hall"));

	this->RemoveAllTimers();

	// 关闭游戏进程
	if (IPC::IsExistInstance())
	{
		IPC* ipc = IPC::GetInstance();
		ipc->DisConnect(false);
		IPC::Release();
	}

	// 如果存在托盘图标，则删除
	if (TrayMenu::IsExistInstance())
	{
		TrayMenu* traymenu_instance = TrayMenu::GetInstance();
		traymenu_instance->Close();
	}

	// 释放进程互斥对象
	::CloseHandle(g_hMutex);

	// 退出消息循环(其后不能再有消息的收发)
	::PostQuitMessage(0);

	g_Logger->TryInfo(_T("GameUtil - End Hall Complete"));
}