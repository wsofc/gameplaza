#include "stdafx.h"
#include "richEditUtil.h"

extern GameUtil* g_GameUtil;
RichEditUtil* RichEditUtil::g_instance = nullptr;

RichEditUtil::RichEditUtil()
	: m_pFaceList(nullptr)
{ }

RichEditUtil::~RichEditUtil()
{
	SAFE_DELETE(m_pFaceList);
}

RichEditUtil* RichEditUtil::GetInstance()
{
	if (NULL == RichEditUtil::g_instance)
	{
		RichEditUtil::g_instance = new (std::nothrow)RichEditUtil();
	}

	return RichEditUtil::g_instance;
}

bool RichEditUtil::LoadFaceFile(LPCTSTR lpFileName)
{
	if (!m_pFaceList)
	{
		m_pFaceList = new CFaceSelList();
	}

	m_pFaceList->Reset();
	bool bRes = m_pFaceList->LoadConfigFile(lpFileName);

	return bRes;
}

void RichEditUtil::CreatFaceFrame(HWND parenWnd, RECT& rcPos, ProcFaceCtrlSel proCtrlSel)
{
	FaceSelFrame* pFaceSelFrame = new FaceSelFrame();
	pFaceSelFrame->CreateFrame(parenWnd, rcPos, m_pFaceList, proCtrlSel);
}

bool RichEditUtil::InsertFace(CRichEditUI* pRichEdit
	, LPCTSTR lpszFileName
	, int nFaceId /* = -1  */
	, int nFaceIndex /* = -1  */
	, COLORREF iBackColor /* = RGB(255, 255, 255) */
	, bool bAutoScale /* = true  */
	, int nReservedWidth /* = 40 */)
{
	BSTR bstrFileName = NULL;
	CTxtWinHost* pTextHost = nullptr;
	ITextServices* pTextServices = nullptr;
	IRichEditOle * pRichEditOle = NULL;
	IOleClientSite *pOleClientSite = NULL;
	IImageOle* pImageOle = NULL;
	IOleObject *pOleObject = NULL;
	REOBJECT reobject = { 0 };
	HRESULT hr = E_FAIL;

	if (!lpszFileName) return false;

	pTextHost = pRichEdit->GetTxtWinHost();
	if (!pTextHost) return false;

	pTextServices = pTextHost->GetTextServices();
	if (!pTextServices) return false;

	TCHAR cProtocol[16] = { 0 };
	_tcsncpy(cProtocol, lpszFileName, 7);
	if ((_tcsicmp(cProtocol, _T("http://")) != 0) && ::GetFileAttributes(lpszFileName) == 0xFFFFFFFF)
		return false;

#ifdef UNICODE
	bstrFileName = ::SysAllocString(lpszFileName);
#else
	OLECHAR* psz = StringConvertor::AnsiToWide(lpszFileName);
	bstrFileName = ::SysAllocString(psz);
	StringConvertor::StringFree(psz);
#endif // UNICODE

	if (!bstrFileName) return false;

	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, SCF_DEFAULT, (LPARAM)&pRichEditOle, NULL);
	if (!pRichEditOle)
		goto Ret0;

	hr = ::CoCreateInstance(CLSID_ImageOle, NULL,
		CLSCTX_INPROC_SERVER, IID_IImageOle, (void**)&pImageOle);
	if (FAILED(hr))
	{
		int err_code = ::GetLastError();
		goto Ret0;
	}

	hr = pImageOle->QueryInterface(IID_IOleObject, (void **)&pOleObject);
	if (FAILED(hr))
		goto Ret0;

	pRichEditOle->GetClientSite(&pOleClientSite);
	if (NULL == pOleClientSite)
		goto Ret0;

	pOleObject->SetClientSite(pOleClientSite);

	pImageOle->SetTextServices(pTextServices);
	pImageOle->SetTextHost(pTextHost);
	pImageOle->SetFaceId(nFaceId);
	pImageOle->SetFaceIndex(nFaceIndex);
	pImageOle->SetBgColor(iBackColor);
	pImageOle->SetAutoScale(bAutoScale, nReservedWidth);
	pImageOle->LoadFromFile(bstrFileName);

	hr = ::OleSetContainedObject(pOleObject, true);

	reobject.cbStruct = sizeof(REOBJECT);
	reobject.clsid = CLSID_ImageOle;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.dwFlags = REO_BELOWBASELINE;
	reobject.poleobj = pOleObject;
	reobject.polesite = pOleClientSite;
	reobject.dwUser = 0;

	hr = pRichEditOle->InsertObject(&reobject);

Ret0:
	if (pOleObject != NULL)
		pOleObject->Release();

	if (pImageOle != NULL)
		pImageOle->Release();

	if (pOleClientSite != NULL)
		pOleClientSite->Release();

	if (pRichEditOle != NULL)
		pRichEditOle->Release();

	if (bstrFileName != NULL)
		::SysFreeString(bstrFileName);

	return SUCCEEDED(hr);
}

void RichEditUtil::AddMsgToInputEdit(CRichEditUI * pInputEdit, LPCTSTR lpText)
{
	_AddMsg(pInputEdit, lpText);
	this->LimitShowBuffer(pInputEdit);
	pInputEdit->EndDown();
}

void RichEditUtil::AddMsgToViewsEdit(CRichEditUI * pViewsEdit, UserData& tUser, LPCTSTR lpContent)
{
	if (NULL == pViewsEdit || NULL == lpContent || NULL == *lpContent) return;
	
	CTxtWinHost* pTxtWinHost = pViewsEdit->GetTxtWinHost();
	if (!pTxtWinHost) return;

	ITextServices * pTextServices = pTxtWinHost->GetTextServices();
	if (!pTxtWinHost) return;

	pViewsEdit->SetAutoURLDetect(true);

	tstring strTime;
	strTime = g_GameUtil->FormatTime(time(NULL), _T("%H:%M:%S"));

	CHARRANGE cr = { -1, -1 };
	pViewsEdit->SetSel(cr);
	TCHAR cText[MAX_PATH] = { 0 };
	m_pDefaultFontInfo.Reset("宋体", 10, RGB(0xFF, 0xB2, 0x00));
	
	_stprintf_s(cText, _T("%s"), tUser.GetNickName());
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	
	m_pDefaultFontInfo.dwColor = RGB(0x1F, 0xC6, 0xDE);
	_stprintf_s(cText, _T("(%s%lu)"), _T("ID:"), tUser.GetUserID());
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	
	m_pDefaultFontInfo.dwColor = RGB(0x3B, 0xAE, 0x00);
	_stprintf_s(cText, _T("%s\r\n"), strTime.c_str());
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	
	_AddMsg(pViewsEdit, lpContent);
	this->ReplaceSelText(pViewsEdit, _T("\r\n"));
	SetStartIndent(pViewsEdit, 0);
	this->LimitShowBuffer(pViewsEdit);
	pViewsEdit->EndDown();
}

void RichEditUtil::SendSystemMsg(CRichEditUI * pViewsEdit, LPCTSTR lpText)
{
	if (NULL == pViewsEdit || NULL == lpText || NULL == *lpText) return;

	CTxtWinHost* pTxtWinHost = pViewsEdit->GetTxtWinHost();
	if (!pTxtWinHost) return;

	ITextServices * pTextServices = pTxtWinHost->GetTextServices();
	if (!pTxtWinHost) return;

	pViewsEdit->SetAutoURLDetect(true);

	CHARRANGE cr = { -1, -1 };
	pViewsEdit->SetSel(cr);
	m_pDefaultFontInfo.Reset();

	// 插入系统喇叭gif
	CDuiString strPath = CPaintManagerUI::GetResourcePath();
	strPath += _T("face\\horn.gif");
	this->InsertFace(pViewsEdit, strPath);

	m_pDefaultFontInfo.dwColor = CFontInfo::ColorRed;
	this->ReplaceSelText(pViewsEdit, _T("【系统公告】"), &m_pDefaultFontInfo);
	
	m_pDefaultFontInfo.dwColor = CFontInfo::ColorWhite;
	this->ReplaceSelText(pViewsEdit, lpText, &m_pDefaultFontInfo);
	this->ReplaceSelText(pViewsEdit, _T("\r\n"), NULL);
	this->SetStartIndent(pViewsEdit, 0);
	this->LimitShowBuffer(pViewsEdit);
	pViewsEdit->EndDown();
}

void RichEditUtil::ParseSystemMsg(CRichEditUI * pViewsEdit, LPCTSTR lpText, ChatColorConfig* pChatColorConfig)
{
	if (NULL == pViewsEdit || NULL == lpText || NULL == *lpText || NULL == pChatColorConfig) return;

	CTxtWinHost* pTxtWinHost = pViewsEdit->GetTxtWinHost();
	if (!pTxtWinHost) return;

	ITextServices * pTextServices = pTxtWinHost->GetTextServices();
	if (!pTxtWinHost) return;

	pViewsEdit->SetAutoURLDetect(true);

	CHARRANGE cr = { -1, -1 };
	pViewsEdit->SetSel(cr);
	m_pDefaultFontInfo.Reset();

	// 插入系统喇叭gif
	CDuiString strPath = CPaintManagerUI::GetResourcePath();
	strPath += _T("face\\horn.gif");
	this->InsertFace(pViewsEdit, strPath);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSysHeadColor;
	this->ReplaceSelText(pViewsEdit, _T("【系统提示】"), &m_pDefaultFontInfo);
		
	// 解析带有颜色符的文本(格式: [rgba] string content [-], 支持嵌套等)
	std::string s = lpText;
	string::const_iterator it = s.begin();
	string::const_iterator end = s.end();
	std::smatch m;
	std::regex reg("\\[[0x]*[0-9a-fA-F]+\\].*?\\[-\\]");

	int iLastEndIndex = 0;
	std::string strEnd = "[-]";
	while (std::regex_search(it, end, m, reg))
	{
		// 迭代器递增
		it = m[0].second;

		int iIndex = m.position(0);
		int len = m.length(0);
		std::string value = m.str();

		if (iIndex != 0)
		{
			m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
			std::string strExtra = s.substr(iLastEndIndex, iIndex);
			this->ReplaceSelText(pViewsEdit, strExtra.c_str(), &m_pDefaultFontInfo);
		}

		int ipos1 = value.find('[', 0);
		int ipos2 = value.find(']', 0);
		int ipos3 = len - strEnd.size();

		std::string sColor = value.substr(ipos1 + 1, ipos2 - ipos1 - 1);
		std::string sValue = value.substr(ipos2 + 1, ipos3 - ipos2 - 1);

		DWORD iColor = g_GameUtil->TransRGBToARGB(pChatColorConfig->dwDefaultColor);
		_stscanf(sColor.c_str(), "%x", &iColor);

		DWORD r = LOBYTE((iColor) >> 24);
		DWORD g = LOBYTE(HIWORD(iColor));
		DWORD b = LOBYTE(((WORD)(iColor)) >> 8);
		DWORD a = LOBYTE(iColor);
		m_pDefaultFontInfo.dwColor = RGB(r, g, b);
		this->ReplaceSelText(pViewsEdit, sValue.c_str(), &m_pDefaultFontInfo);

		iLastEndIndex += len + iIndex;
	}

	if (iLastEndIndex != s.size())
	{
		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
		std::string strExtra = s.substr(iLastEndIndex, s.size() - iLastEndIndex);
		this->ReplaceSelText(pViewsEdit, strExtra.c_str(), &m_pDefaultFontInfo);
	}
	
	this->ReplaceSelText(pViewsEdit, _T("\r\n"), NULL);
	this->SetStartIndent(pViewsEdit, 0);
	this->LimitShowBuffer(pViewsEdit);
	pViewsEdit->EndDown();
}

void RichEditUtil::LimitShowBuffer(CRichEditUI* pRichEdit, LONG lLimit /* = 2048 */)
{
	ASSERT(pRichEdit);
	if (!pRichEdit) return;

	LONG lTotalLen = pRichEdit->GetTextLength();
	if (lTotalLen > lLimit)
	{
		LONG lStartChar = 0, lEndChar = 0;
		lEndChar = lStartChar + lTotalLen - lLimit;
		pRichEdit->SetSel(lStartChar, lEndChar);
		pRichEdit->ReplaceSel(_T(""), false);
	}
}

void RichEditUtil::SendBuyPropsMsg(CRichEditUI* pViewsEdit, WORD wCount, LPCTSTR lpPropsName , LPCTSTR lpPropsIcon
	, LONGLONG lSrcCharms, LONGLONG lTarCharms, ChatColorConfig* pChatColorConfig)
{
	if (!pViewsEdit || !pChatColorConfig) return;

	CTxtWinHost* pTxtWinHost = pViewsEdit->GetTxtWinHost();
	if (!pTxtWinHost) return;

	ITextServices * pTextServices = pTxtWinHost->GetTextServices();
	if (!pTxtWinHost) return;

	// 开启URL检测
	pViewsEdit->SetAutoURLDetect(true);
	
	CHARRANGE cr = { -1, -1 };
	pViewsEdit->SetSel(cr);
	TCHAR cText[MAX_PATH] = { 0 };
	m_pDefaultFontInfo.Reset();

	// 插入系统喇叭gif
	CDuiString strPath = CPaintManagerUI::GetResourcePath();
	strPath += _T("face\\horn.gif");
	_stprintf_s(cText, _T("%s"), strPath.GetData());
	InsertFace(pViewsEdit, cText);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSysHeadColor;
	_stprintf_s(cText, _T("%s"), _T("【系统公告】"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
	_stprintf_s(cText, _T("%s"), _T("购买"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwPropsCountColor;
	_stprintf_s(cText, _T("%u"), wCount);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
	_stprintf_s(cText, _T("%s"), _T("个"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwPropsNameColor;
	_stprintf_s(cText, _T("%s"), lpPropsName);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	
	InsertFace(pViewsEdit, lpPropsIcon);
	
	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
	_stprintf_s(cText, _T("%s"), _T("并使用成功!"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	if (lSrcCharms > 0)
	{
		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
		_stprintf_s(cText, _T("%s"), _T("自身魅力值增加"));
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

		LPCTSTR lpSign = _T("");
		if (lTarCharms > 0)
		{
			lpSign = _T(",");
		}

		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSrcCharmColor;
		_stprintf_s(cText, _T("%lld%s"), lSrcCharms, lpSign);
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	}

	if (lTarCharms > 0)
	{
		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
		_stprintf_s(cText, _T("%s"), _T("对方魅力值增加"));
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwRcvCharmColor;
		_stprintf_s(cText, _T("%lld"), lTarCharms);
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	}
	
	this->ReplaceSelText(pViewsEdit, _T("\r\n"), &m_pDefaultFontInfo);
	SetStartIndent(pViewsEdit, 0);
	this->LimitShowBuffer(pViewsEdit);
	pViewsEdit->EndDown();
}

void RichEditUtil::SendGivePropsMsg(CRichEditUI* pViewsEdit, LPCTSTR lpSrcName, DWORD dwSrcID, LPCTSTR lpTarName, DWORD dwTarID, WORD wCount
	, LPCTSTR lpPropsName, LPCTSTR lpPropsIcon, LONGLONG lSrcCharms, LONGLONG lTarCharms, ChatColorConfig* pChatColorConfig)
{
	if (!pViewsEdit || !pChatColorConfig) return;

	CTxtWinHost* pTxtWinHost = pViewsEdit->GetTxtWinHost();
	if (!pTxtWinHost) return;

	ITextServices * pTextServices = pTxtWinHost->GetTextServices();
	if (!pTxtWinHost) return;

	// 开启URL检测
	pViewsEdit->SetAutoURLDetect(true);

	CHARRANGE cr = { -1, -1 };
	pViewsEdit->SetSel(cr);
	TCHAR cText[MAX_PATH] = { 0 };
	m_pDefaultFontInfo.Reset();

	// 插入系统喇叭gif
	CDuiString strPath = CPaintManagerUI::GetResourcePath();
	strPath += _T("face\\horn.gif");
	_stprintf_s(cText, _T("%s"), strPath.GetData());
	InsertFace(pViewsEdit, cText);
	
	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSysHeadColor;
	_stprintf_s(cText, _T("%s"), _T("【系统公告】"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSrcNameHeadColor;
	_stprintf_s(cText, _T("%s"), lpSrcName);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSrcGameIDColor;
	_stprintf_s(cText, _T("(ID:%lu)"), dwSrcID);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
	_stprintf_s(cText, _T("%s"), _T("赠送"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	
	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwRcvNameHeadColor;
	_stprintf_s(cText, _T("%s"), lpTarName);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwRcvGameIDColor;
	_stprintf_s(cText, _T("(ID:%lu)"), dwTarID);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwPropsCountColor;
	_stprintf_s(cText, _T("%u"), wCount);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
	_stprintf_s(cText, _T("%s"), _T("个"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwPropsNameColor;
	_stprintf_s(cText, _T("%s"), lpPropsName);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	InsertFace(pViewsEdit, lpPropsIcon);

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
	_stprintf_s(cText, _T("%s"), _T("成功!"));
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	if (lSrcCharms > 0)
	{
		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
		_stprintf_s(cText, _T("%s"), _T("自身魅力值增加"));
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

		LPCTSTR lpSign = _T("");
		if (lTarCharms > 0)
		{
			lpSign = _T(",");
		}

		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSrcCharmColor;
		_stprintf_s(cText, _T("%lld%s"), lSrcCharms, lpSign);
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	}

	if (lTarCharms > 0)
	{
		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwDefaultColor;
		_stprintf_s(cText, _T("%s"), _T("对方魅力值增加"));
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

		m_pDefaultFontInfo.dwColor = pChatColorConfig->dwRcvCharmColor;
		_stprintf_s(cText, _T("%lld"), lTarCharms);
		this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	}
	
	this->ReplaceSelText(pViewsEdit, _T("\r\n"), &m_pDefaultFontInfo);
	SetStartIndent(pViewsEdit, 0);
	this->LimitShowBuffer(pViewsEdit);
	pViewsEdit->EndDown();
}


void RichEditUtil::SendUserLeaveMsg(CRichEditUI* pViewsEdit, LPCTSTR lpName, LPCTSTR lpText, ChatColorConfig* pChatColorConfig)
{
	if (!pViewsEdit || !pChatColorConfig) return;

	CTxtWinHost* pTxtWinHost = pViewsEdit->GetTxtWinHost();
	if (!pTxtWinHost) return;

	ITextServices * pTextServices = pTxtWinHost->GetTextServices();
	if (!pTxtWinHost) return;

	// 开启URL检测
	pViewsEdit->SetAutoURLDetect(true);

	CHARRANGE cr = { -1, -1 };
	pViewsEdit->SetSel(cr);
	TCHAR cText[MAX_PATH] = { 0 };
	m_pDefaultFontInfo.Reset();

	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSrcNameHeadColor;
	_stprintf_s(cText, _T("[%s] "), lpName);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);
	
	m_pDefaultFontInfo.dwColor = pChatColorConfig->dwSrcNameTextColor;
	_stprintf_s(cText, _T("%s"), lpText);
	this->ReplaceSelText(pViewsEdit, cText, &m_pDefaultFontInfo);

	this->ReplaceSelText(pViewsEdit, _T("\r\n"), &m_pDefaultFontInfo);
	SetStartIndent(pViewsEdit, 0);
	this->LimitShowBuffer(pViewsEdit);
	pViewsEdit->EndDown();
}

bool RichEditUtil::GetImageOle(CRichEditUI* pRichEdit, POINT pt, IImageOle** pImageOle)
{
	if (NULL == pImageOle) return false;
	*pImageOle = NULL;

	IRichEditOle* pRichEditOle = nullptr;
	REOBJECT reobject;
	HRESULT hr = NULL;
	bool bRet = false;

	pRichEditOle = pRichEdit->GetRichEditOle();
	if (nullptr == pRichEditOle) return false;

	for (int i = 0; i < pRichEditOle->GetObjectCount(); i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		hr = pRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);
		if (S_OK == hr)
		{
			if (reobject.poleobj != NULL)
			{
				if (CLSID_ImageOle == reobject.clsid)
				{
					IImageOle * pImageOle2 = NULL;
					hr = reobject.poleobj->QueryInterface(__uuidof(IImageOle), (void**)&pImageOle2);
					reobject.poleobj->Release();
					if (SUCCEEDED(hr))
					{
						RECT rcObject = { 0 };
						hr = pImageOle2->GetObjectRect(&rcObject);
						if (SUCCEEDED(hr))
						{
							if (::PtInRect(&rcObject, pt))
							{
								*pImageOle = pImageOle2;
								bRet = true;
								break;
							}
						}
						pImageOle2->Release();
					}
				}
				else
				{
					reobject.poleobj->Release();
				}
			}
		}
	}

	pRichEditOle->Release();

	return bRet;
}

void RichEditUtil::SetSelTextColor(CRichEditUI* pRichEdit, LPCTSTR lpText, COLORREF color)
{
	if (!pRichEdit || !lpText) return;

	LRESULT lRes = 0;
	CHARRANGE cr = { 0, 0 };
	pRichEdit->GetSel(cr);
	cr.cpMax = cr.cpMin + _tcslen(lpText);
	pRichEdit->SetSel(cr);

	CHARFORMAT2 cf2;
	memset(&cf2, 0, sizeof(CHARFORMAT2));
	lRes = pRichEdit->GetSelectionCharFormat(cf2);

	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = cf2.dwMask | CFM_COLOR;
	cf2.crTextColor = color;
	cf2.dwEffects &= ~CFE_AUTOCOLOR;
	lRes = pRichEdit->SetSelectionCharFormat(cf2);
}

void RichEditUtil::SetSelLinkText(CRichEditUI* pRichEdit, bool bEnable)
{
	if (!pRichEdit) return;

	CHARFORMAT2 cf2;
	memset(&cf2, 0, sizeof(CHARFORMAT2));
	pRichEdit->GetSelectionCharFormat(cf2);

	cf2.dwMask = cf2.dwMask | CFM_LINK;
	if (bEnable)							// 设置超链接
		cf2.dwEffects |= CFE_LINK;
	else
		cf2.dwEffects &= ~CFE_LINK;

	pRichEdit->SetSelectionCharFormat(cf2);
}

bool RichEditUtil::SetFontInfo(CRichEditUI* pRichEdit, CFontInfo& pFontInfo)
{
	if (!pRichEdit) return false;

	CHARFORMAT2 cf2;
	memset(&cf2, 0, sizeof(CHARFORMAT2));

	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;
	cf2.dwEffects &= ~CFE_AUTOCOLOR;

	// 设置字体名称
	if (!pFontInfo.lpName)
	{
		cf2.dwMask = cf2.dwMask | CFM_FACE;
		_tcscpy(cf2.szFaceName, pFontInfo.lpName);
	}

	// 设置字体高度
	if (pFontInfo.dwSize > 0)
	{
		cf2.dwMask = cf2.dwMask | CFM_SIZE;
		cf2.yHeight = pFontInfo.dwSize * 20;
	}

	// 设置字体颜色
	cf2.crTextColor = pFontInfo.dwColor;

	// 设置粗体
	if (pFontInfo.bBold)
		cf2.dwEffects |= CFE_BOLD;
	else
		cf2.dwEffects &= ~CFE_BOLD;

	// 设置倾斜
	if (pFontInfo.bItalic)
		cf2.dwEffects |= CFE_ITALIC;
	else
		cf2.dwEffects &= ~CFE_ITALIC;

	// 设置下划线
	if (pFontInfo.bUnderLine)
		cf2.dwEffects |= CFE_UNDERLINE;
	else
		cf2.dwEffects &= ~CFE_UNDERLINE;

	// 设置超链接
	if (pFontInfo.bLink)
		cf2.dwEffects |= CFE_LINK;
	else
		cf2.dwEffects &= ~CFE_LINK;

	LRESULT lRes = pRichEdit->SetSelectionCharFormat(cf2);
	return S_OK == lRes;
}

bool RichEditUtil::SetStartIndent(CRichEditUI* pRichEdit, LONG dxStartIndent)
{
	bool lRes = false;
	if (!pRichEdit) return lRes;

	PARAFORMAT2 pf2;
	memset(&pf2, 0, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = dxStartIndent;
	lRes = pRichEdit->SetParaFormat(pf2);

	return lRes;
}

void RichEditUtil::ReplaceSelText(CRichEditUI* pRichEdit, LPCTSTR lpszNewText, CFontInfo* pFontInfo /* = nullptr */)
{
	LONG lStartChar = 0, lEndChar = 0;
	if (pFontInfo)
	{
		pRichEdit->GetSel(lStartChar, lEndChar);
		pRichEdit->ReplaceSel(lpszNewText, pFontInfo->bCanUndo);
		lEndChar = lStartChar + _tcslen(lpszNewText);
		pRichEdit->SetSel(lStartChar, lEndChar);

		SetFontInfo(pRichEdit, *pFontInfo);
		SetStartIndent(pRichEdit, pFontInfo->lStartIndent);
		pRichEdit->SetSel(lEndChar, lEndChar);
	}
	else
	{
		SetSelTextColor(pRichEdit, lpszNewText, RGB(255, 255, 255));
		pRichEdit->ReplaceSel(lpszNewText, false);
	}
}

void RichEditUtil::GetOleText(CRichEditUI* pRichEdit, std::wstring& wstrText)
{
	if (!pRichEdit) return;

	REOBJECT reobject;
	LONG nFaceId, nPos = 0;
	std::wstring wstrOrgText, wstrTemp;

	IRichEditOle * pRichEditOle = pRichEdit->GetRichEditOle();
	if (!pRichEditOle) return;

	CHARRANGE chrg = { 0, pRichEdit->GetTextLength() };
#ifdef UNICODE || _UNICODE
	wstrOrgText = pRichEdit->GetTextRange(chrg.cpMin, chrg.cpMax).GetData();
#else
	wstrOrgText = StringConvertor::AnsiToWide(pRichEdit->GetTextRange(chrg.cpMin, chrg.cpMax));
#endif // UNICODE

	for (LONG i = 0; i < wstrOrgText.size(); ++i)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.cp = i;
		HRESULT hr = pRichEditOle->GetObject(REO_IOB_USE_CP, &reobject, REO_GETOBJ_POLEOBJ);
		if (SUCCEEDED(hr))
		{
			if (reobject.cp > 0 && reobject.cp > nPos)
			{
				wstrTemp = wstrOrgText.substr(nPos, reobject.cp - nPos);
				g_GameUtil->Replace(wstrTemp, L"/", L"//");
				wstrText += wstrTemp.c_str();
			}
			nPos = reobject.cp + 1;

			if (NULL == reobject.poleobj)
				continue;

			if (CLSID_ImageOle == reobject.clsid)
			{
				IImageOle * pImageOle = NULL;
				hr = reobject.poleobj->QueryInterface(__uuidof(IImageOle), (void**)&pImageOle);
				if (SUCCEEDED(hr))
				{
					pImageOle->GetFaceId(&nFaceId);
					if (nFaceId != -1)
					{
						WCHAR cBuf[32] = { 0 };
						wsprintfW(cBuf, L"/f[\"%03d\"]", nFaceId);
						wstrText += cBuf;
					}
					else
					{
						wstrText += L"/c[\"";
						BSTR bstrFileName = NULL;
						pImageOle->GetFileName(&bstrFileName);
						wstrText += bstrFileName;
						::SysFreeString(bstrFileName);
						wstrText += L"\"]";
					}
					pImageOle->Release();
				}
			}
			reobject.poleobj->Release();
		}
	}

	if (nPos < wstrOrgText.size())
	{
		wstrTemp = wstrOrgText.substr(nPos);
		g_GameUtil->Replace(wstrTemp, L"/", L"//");
		wstrText += wstrTemp.c_str();
	}

	pRichEditOle->Release();
}

// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void RichEditUtil::_AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); ++p)
	{
		if (*p == _T('/'))
		{
			if (*(p + 1) == _T('/'))
			{
				strText += *p;
				++p;
				continue;
			}
			else if (*(p + 1) == _T('f'))
			{
				if (_HandleSysFaceId(pRichEdit, p, strText))
					continue;
			}
			else if (*(p + 1) == _T('s'))
			{
				if (_HandleSysFaceIndex(pRichEdit, p, strText))
					continue;
			}
			else if (*(p + 1) == _T('c'))
			{
				if (_HandleCustomPic(pRichEdit, p, strText))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		this->ReplaceSelText(pRichEdit, strText.c_str());
}

bool RichEditUtil::_HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	if (nullptr == m_pFaceList) return false;
	INT nFaceId = g_GameUtil->GetBetweenInt(p + 2, _T("[\""), _T("\"]"), -1);
	CFaceInfo * lpFaceInfo = m_pFaceList->GetFaceInfoById(nFaceId);
	if (nullptr == lpFaceInfo) return false;

	if (!strText.empty())
	{
		ReplaceSelText(pRichEdit, strText.c_str());
		strText = _T("");
	}

	this->InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

	p = _tcsstr(p + 2, _T("\"]"));
	++p;

	return true;
}

bool RichEditUtil::_HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	if (nullptr == m_pFaceList) return false;
	INT nFaceIndex = g_GameUtil->GetBetweenInt(p + 2, _T("[\""), _T("\"]"), -1);
	CFaceInfo * lpFaceInfo = m_pFaceList->GetFaceInfoByIndex(nFaceIndex);
	if (nullptr == lpFaceInfo) return false;

	if (!strText.empty())
	{
		ReplaceSelText(pRichEdit, strText.c_str());
		strText = _T("");
	}

	this->InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

	p = _tcsstr(p + 2, _T("\"]"));
	++p;

	return true;
}

bool RichEditUtil::_HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	tstring strFileName = g_GameUtil->GetBetweenString(p + 2, _T("[\""), _T("\"]"));
	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			ReplaceSelText(pRichEdit, strText.c_str());
			strText = _T("");
		}

		this->InsertFace(pRichEdit, strFileName.c_str());

		p = _tcsstr(p + 2, _T("\"]"));
		++p;

		return true;
	}
	return false;
}
