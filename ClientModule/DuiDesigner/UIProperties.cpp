#include "StdAfx.h"
#include "UIProperties.h"
#include "DuiDesigner.h"

#include "UIDesignerView.h"
#include "ImageDialog.h"
#include "DialogCustomFonts.h"
#include "DialogDefaultAttribList.h"

using DuiLib::TRelativePosUI;
using DuiLib::IListOwnerUI;

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridColor32Property

CMFCPropertyGridColor32Property::CMFCPropertyGridColor32Property(const CString& strName, const COLORREF& color, CPalette* pPalette/*=NULL*/, LPCTSTR lpszDescr/*=NULL*/, DWORD_PTR dwData/*=0*/)
	:CMFCPropertyGridColorProperty(strName, color, pPalette, lpszDescr, dwData)
{
}

BOOL CMFCPropertyGridColor32Property::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);
	if (_tcscmp(strText, _T("")) == 0)
		strText = _T("FFFFFFFF");

	COLORREF colorCurr = m_Color;
	int nA = 0, nR = 0, nG = 0, nB = 0;
	_stscanf_s(strText, _T("%2x%2x%2x%2x"), &nA, &nR, &nG, &nB);

	nA = 0xFF;
	m_Color = ARGB(nA, nR, nG, nB);

	if (colorCurr != m_Color)
		m_pWndList->OnPropertyChanged(this);

	return TRUE;
}

void CMFCPropertyGridColor32Property::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height();
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height();
	rectColor.DeflateRect(1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : (m_Color & 0x00FFFFFF));
	pDC->FillRect(rectColor, &br);
	pDC->Draw3dRect(rectColor, 0, 0);
}

CString CMFCPropertyGridColor32Property::FormatProperty()
{
	ASSERT_VALID(this);

	CString str;
	str.Format(_T("%02x%02x%02x%02x"), GetAValue(m_Color), GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color));

	return str;
}

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridImageProperty

IMPLEMENT_DYNAMIC(CMFCPropertyGridImageProperty, CMFCPropertyGridProperty)

CMFCPropertyGridImageProperty::CMFCPropertyGridImageProperty(const CString& strName, const CString& strImage, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
: CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strImage), lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CMFCPropertyGridImageProperty::OnClickButton(::CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CString strImage = m_varValue.bstrVal;
	CImageDialog dlg(strImage);
	if (dlg.DoModal() == IDOK)
	{
		strImage = dlg.GetImage();
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText(strImage);
	}
	m_varValue = (LPCTSTR)strImage;

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridCustomFontsProperty

IMPLEMENT_DYNAMIC(CMFCPropertyGridCustomFontsProperty, CMFCPropertyGridProperty)

CMFCPropertyGridCustomFontsProperty::CMFCPropertyGridCustomFontsProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CMFCPropertyGridCustomFontsProperty::OnClickButton(::CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CDialogCustomFonts dlg;
	dlg.DoModal();

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridDefaultAttribListProperty

IMPLEMENT_DYNAMIC(CMFCPropertyGridDefaultAttribListProperty, CMFCPropertyGridProperty)

CMFCPropertyGridDefaultAttribListProperty::CMFCPropertyGridDefaultAttribListProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CMFCPropertyGridDefaultAttribListProperty::OnClickButton(::CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CDialogDefaultAttribList dlg;
	dlg.DoModal();

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CUIProperties

UINT WM_UI_PROPERTY_CHANGED = ::RegisterWindowMessage(_T("WM_UI_PROPERTY_CHANGED"));

IMPLEMENT_DYNAMIC(CUIProperties, CWnd)

CUIProperties::CUIProperties(void)
{
}

CUIProperties::~CUIProperties(void)
{
}

BEGIN_MESSAGE_MAP(CUIProperties, CWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_PROPERTIES_TOOLBAR_SORT, OnPropertiesSort)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_TOOLBAR_SORT, OnUpdatePropertiesSort)
	ON_COMMAND(ID_PROPERTIES_TOOLBAR_ALPHABETIC, OnPropertiesAlphaBetic)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_TOOLBAR_ALPHABETIC, OnUpdatePropertiesAlphaBetic)
END_MESSAGE_MAP()

BOOL CUIProperties::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(afxGlobalData.RegisterWindowClass(_T("UIPropWnd")), _T(""), dwStyle, rect, pParentWnd, nID, NULL);
}

int CUIProperties::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �������:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;      // δ�ܴ���
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ����*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}

void CUIProperties::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CUIProperties::SetVSDotNetLook(BOOL bSet)
{
	m_wndPropList.SetVSDotNetLook(bSet);
	m_wndPropList.SetGroupNameFullWidth(bSet);
}

void CUIProperties::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_wndPropList.SetFocus();
}

void CUIProperties::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

LRESULT CUIProperties::OnPropertyChanged(WPARAM wp, LPARAM lp)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty *)lp;

	int nLevel = pProp->GetHierarchyLevel();
	for (; nLevel > 1; nLevel--)
		pProp = pProp->GetParent();

	GetOwner()->SendMessage(WM_UI_PROPERTY_CHANGED, wp, LPARAM(pProp));

	return TRUE;
}

void CUIProperties::OnPropertiesSort()
{
	m_wndPropList.SetAlphabeticMode(FALSE);
}

void CUIProperties::OnUpdatePropertiesSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(!m_wndPropList.IsAlphabeticMode());
}

void CUIProperties::OnPropertiesAlphaBetic()
{
	m_wndPropList.SetAlphabeticMode(TRUE);
}

void CUIProperties::OnUpdatePropertiesAlphaBetic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CUIProperties::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();

	CMFCPropertyGridProperty* pPropUI = NULL;
	CMFCPropertyGridProperty* pValueList = NULL;
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridColor32Property* pPropColor = NULL;
	CMFCPropertyGridImageProperty* pPropImage = NULL;

	//Window
#pragma region Window
	pPropUI = new CMFCPropertyGridProperty(_T("Window"), classWindow);

	pValueList = new CMFCPropertyGridProperty(_T("Size"), tagWindowSize, TRUE);//size
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(LONG)0, _T("����Ŀ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(LONG)0, _T("����ĸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("Caption"), tagCaption, TRUE);//caption
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("�����Leftλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("�����Topλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("�����Rightλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("�����Bottomλ��"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("SizeBox"), tagSizeBox, TRUE);//sizebox
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("�ߴ�е�Leftλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("�ߴ�е�Topλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("�ߴ�е�Rightλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("�ߴ�е�Bottomλ��"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("RoundCorner"), tagRoundCorner, TRUE);//roundcorner
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(LONG)0, _T("Բ�ǵĿ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(LONG)0, _T("Բ�ǵĸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("MinInfo"), tagMinInfo, TRUE);//mininfo
	pProp = new CMFCPropertyGridProperty(_T("MinWidth"), (_variant_t)(LONG)0, _T("���ڵ���С���ٿ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("MinHeight"), (_variant_t)(LONG)0, _T("���ڵ���С���ٸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("MaxInfo"), tagMinInfo, TRUE);//maxinfo
	pProp = new CMFCPropertyGridProperty(_T("MaxWidth"), (_variant_t)(LONG)0, _T("���ڵ������ٿ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Maxeight"), (_variant_t)(LONG)0, _T("���ڵ������ٸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp = new CMFCPropertyGridProperty(_T("ShowDirty"), (_variant_t)false, _T("ָʾ�Ƿ���ʾ��������"), tagShowDirty);//showdirty
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridCustomFontsProperty(_T("CustomFonts"), (_variant_t)_T("�������"), _T("�����Զ��������"), tagCustomFonts);//customfonts
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridDefaultAttribListProperty(_T("DefaultAttribList"), (_variant_t)_T("Ĭ�������б����"), _T("����Ĭ�ϵ������б�"), tagDefaultAttribList);//defaultattriblist
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	// tagAlpha
	pProp = new CMFCPropertyGridProperty(_T("Alpha"), (_variant_t)(LONG)0, _T("���ڵ�alphaֵ(0-255)\n255"), tagAlpha);
	pPropUI->AddSubItem(pProp);

	// tagBkTrans
	pProp = new CMFCPropertyGridProperty(_T("BkTrans"), (_variant_t)false, _T("�����Ƿ�ʹ�þ�̬͸������\nfalse"), tagBkTrans);
	pPropUI->AddSubItem(pProp);

	// tagDefaultFontColor
	pPropColor = new CMFCPropertyGridColor32Property(_T("DefaultFontColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��Ĭ�ϵ�������ɫ"), tagDefaultFontColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagSelectedFontColor
	pPropColor = new CMFCPropertyGridColor32Property(_T("SelectedColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��Ĭ�ϵ�selected������ɫ"), tagSelectedFontColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//	DisabledFontColor
	pPropColor = new CMFCPropertyGridColor32Property(_T("DisabledFontColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��Ĭ�ϵ�Disabled������ɫ"), tagDisabledFontColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagLinkFontColor
	pPropColor = new CMFCPropertyGridColor32Property(_T("LinkFontColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��Ĭ�ϵ�link������ɫ"), tagLinkFontColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagLinkHoverFontColor
	pPropColor = new CMFCPropertyGridColor32Property(_T("LinkHoverFontColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��Ĭ�ϵ�linkhoverfont������ɫ"), tagLinkHoverFontColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Window

	//Control
#pragma region Control
	pPropUI = new CMFCPropertyGridProperty(_T("Control"), classControl);

	pProp = new CMFCPropertyGridProperty(_T("Name"), (_variant_t)_T(""), _T("�ؼ�������"), tagName);//name
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Text"), (_variant_t)_T(""), _T("�ؼ�����ʾ�ı�"), tagText);//text
	pPropUI->AddSubItem(pProp);

	pValueList = new CMFCPropertyGridProperty(_T("Pos"), tagPos, TRUE);//pos
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("�ؼ���Leftλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("�ؼ���Topλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("�ؼ���Rightλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("�ؼ���Bottomλ��"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("RelativePos"), tagRelativePos, TRUE);//relativepos
	pProp = new CMFCPropertyGridProperty(_T("MoveX"), (_variant_t)(LONG)0, _T("�ؼ���ˮƽλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("MoveY"), (_variant_t)(LONG)0, _T("�ؼ��Ĵ�ֱλ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ZoomX"), (_variant_t)(LONG)0, _T("�ؼ���ˮƽ����"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ZoomY"), (_variant_t)(LONG)0, _T("�ؼ��Ĵ�ֱ����"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("Size"), tagSize, TRUE);//size
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(LONG)0, _T("�ؼ��Ŀ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(LONG)0, _T("�ؼ��ĸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("MinSize"), tagMinSize, TRUE);//minsize
	pProp = new CMFCPropertyGridProperty(_T("MinWidth"), (_variant_t)(LONG)0, _T("ָ���ؼ�����С���"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("MinHeight"), (_variant_t)(LONG)0, _T("ָ���ؼ�����С�߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("MaxSize"), tagMaxSize, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("MaxWidth"), (_variant_t)(LONG)0, _T("ָ���ؼ��������"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("MaxHeight"), (_variant_t)(LONG)0, _T("ָ���ؼ������߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("Padding"), tagPadding, TRUE);//padding
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("ָ���ؼ��ڲ�����߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("ָ���ؼ��ڲ����ϱ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("ָ���ؼ��ڲ����ұ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("ָ���ؼ��ڲ����±߾�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pPropImage = new CMFCPropertyGridImageProperty(_T("BkImage"), _T(""), _T("ָ���ؼ��ı���ͼƬ"), tagBkImage);//bkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropColor = new CMFCPropertyGridColor32Property(_T("BkColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���ؼ��ı�����ɫ"), tagBkColor);//bkcolor
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pPropColor = new CMFCPropertyGridColor32Property(_T("BkColor2"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���ؼ��ı�����ɫ2"), tagBkColor2);//bkcolor2
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pPropColor = new CMFCPropertyGridColor32Property(_T("BorderColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���ؼ��ı߿���ɫ"), tagBorderColor);//bordercolor
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//focusbordercolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("focusbordercolor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���ؼ��߿��ý���ʱ�߿����ɫ"), tagFocusBorderColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pProp = new CMFCPropertyGridProperty(_T("BorderSize"), (_variant_t)(LONG)0, _T("ָ���ؼ��ı߿��߿�\n1"), tagBorderSize);//bordersize
	pPropUI->AddSubItem(pProp);

	//borderround
	pValueList = new CMFCPropertyGridProperty(_T("borderround"), tagBorderRound, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(LONG)0, _T("�߿�Բ�ǵĿ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(LONG)0, _T("�߿�Բ�ǵĸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp = new CMFCPropertyGridProperty(_T("Enabled"), (_variant_t)true, _T("ָʾ�Ƿ������øÿؼ�\nTrue"), tagEnabled);//enabled
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Float"), (_variant_t)false, _T("ȷ���ÿؼ��ǹ̶��ģ����Ǹ�����\nFalse"), tagFloat);//float
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Visible"), (_variant_t)true, _T("ȷ���ÿؼ��ǿɼ��ģ��������ص�\nTrue"), tagVisible);//visible
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Mouse"), (_variant_t)true, _T("ָʾ�ÿؼ��Ƿ���Ӧ������\nTrue"), tagMouse);//mouse
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Menu"), (_variant_t)false, _T("ָʾ�ÿؼ��Ƿ���Ҫ�Ҽ��˵�\nFalse"), tagMenu);//menu
	pPropUI->AddSubItem(pProp);

	//colorhsl
	pProp = new CMFCPropertyGridProperty(_T("ColorHSL"), (_variant_t)false, _T("ָʾ�ÿؼ�����ɫ�Ƿ��洰�ڵ�hsl�仯���仯\nFalse"), tagColorHSL);
	pPropUI->AddSubItem(pProp);

	//tooltip
	pProp = new CMFCPropertyGridProperty(_T("Tooltip"), (_variant_t)_T(""), _T("ָʾ�ÿؼ����������ʾ"), tagTooltip);
	pPropUI->AddSubItem(pProp);

	//userdata
	pProp = new CMFCPropertyGridProperty(_T("UserData"), (_variant_t)_T(""), _T("ָʾ�ÿؼ��Զ����ʶ\nFalse"), tagUserData);
	pPropUI->AddSubItem(pProp);

	//keyboard
	pProp = new CMFCPropertyGridProperty(_T("KeyBoard"), (_variant_t)false, _T("ָʾCButton��ؼ��Ƿ����TabStop�Ͱ����¼�\nFalse"), tagKeyBoard);
	pPropUI->AddSubItem(pProp);

	//Tab
	pProp = new CMFCPropertyGridProperty(_T("Tab"), (_variant_t)false, _T("ָʾ�ÿؼ��Ƿ����Tab�����¼�\nFalse"), tagTab);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Control

	//Label
#pragma region Label
	pPropUI = new CMFCPropertyGridProperty(_T("Label"), classLabel);

	//align
	pProp = new CMFCPropertyGridProperty(_T("Align"), _T("Center"), _T("ָʾ�ı��Ķ��뷽ʽ"), tagAlign);
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Right"));
	pProp->AddOption(_T("Top"));
	pProp->AddOption(_T("Bottom"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//textcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("TextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���ı�����ɫ"), tagTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//disabledtextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("DisabledTextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���ı�����ɫ"), tagDisabledTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//font
	pProp = new CMFCPropertyGridProperty(_T("Font"), (_variant_t)(LONG)-1, _T("ָ���ı�������"), tagFont);
	pPropUI->AddSubItem(pProp);

	//textpadding
	pValueList = new CMFCPropertyGridProperty(_T("TextPadding"), tagTextPadding, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("ָ���ı��������߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("ָ���ı�������ϱ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("ָ���ı�������ұ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("ָ���ı�������±߾�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//showhtml
	pProp = new CMFCPropertyGridProperty(_T("ShowHtml"), (_variant_t)false, _T("ָʾ�Ƿ�ʹ��HTML��ʽ���ı�"), tagShowHtml);
	pPropUI->AddSubItem(pProp);

	//endellipsis
	pProp = new CMFCPropertyGridProperty(_T("EndEllipsis"), (_variant_t)false, _T("ָʾ��ĩ��ʾ��ȫ�Ƿ�ʹ��...����"), tagEndEllipsis);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Label

	//Button
#pragma region Button
	pPropUI = new CMFCPropertyGridProperty(_T("Button"), classButton);

	//normalimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("NormalImage"), _T(""), _T("ָ����ť������ʾʱ��ͼƬ"), tagButtonNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("HotImage"), _T(""), _T("ָ����ť����ȵ�ʱ��ͼƬ"), tagButtonHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//pushedimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("PushedImage"), _T(""), _T("ָ����ť����ѹ��ʱ��ͼƬ"), tagButtonPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//focusedimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("FocusedImage"), _T(""), _T("ָ����ť��ý�����ͼƬ"), tagButtonFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//disabledimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("DisabledImage"), _T(""), _T("ָ����ť�����ú��ͼƬ"), tagButtonDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//foreimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("ForeImage"), _T(""), _T("ָ����ť��ǰ��ͼƬ\nһ�����ڰ�ť����ӵ�����ͼƬ"), tagButtonForeImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotforeimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("HotForeimage"), _T(""), _T("ָ����ť����ȵ�ʱ��ǰ��ͼƬ\n"), tagButtonHotForeImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotbkcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("HotBkColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ����ť����ȵ�ʱ�ı�����ɫ"), tagButtonHotBkColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//hottextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("HotTextcolor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ����ť����ȵ�ʱ���ı���ɫ"), tagButtonHotTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//pushedtextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("PushedTextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ����ť������ʱ���ı���ɫ"), tagButtonPushedTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//focusedtextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("FocusedTextcolor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ����ť��ý����ʱ���ı���ɫ"), tagButtonFocusedTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Button

	//Edit
#pragma region Edit
	pPropUI = new CMFCPropertyGridProperty(_T("Edit"), classEdit);

	//normalimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("NormalImage"), _T(""), _T("ָ���༭��������ʾʱ��ͼƬ"), tagEditNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("HotImage"), _T(""), _T("ָ���༭�����ȵ�ʱ��ͼƬ"), tagEditHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//focusedimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("FocusedImage"), _T(""), _T("ָ���༭���ý�����ͼƬ"), tagEditFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//disabledimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("DisabledImage"), _T(""), _T("ָ���༭�򱻽��ú��ͼƬ"), tagEditDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//readonly
	pProp = new CMFCPropertyGridProperty(_T("ReadOnly"), (_variant_t)false, _T("ָʾ�Ƿ�ֻ��"), tagReadOnly);
	pPropUI->AddSubItem(pProp);

	//password
	pProp = new CMFCPropertyGridProperty(_T("Password"), (_variant_t)false, _T("ָʾ�Ƿ�ʹ�������"), tagPassword);
	pPropUI->AddSubItem(pProp);

	//maxchar
	pProp = new CMFCPropertyGridProperty(_T("MaxChar"), (_variant_t)(LONG)0, _T("ָʾ�����ַ���󳤶�\n255"), tagMaxChar);
	pPropUI->AddSubItem(pProp);

	//nativebkcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("NativeBKColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��Windowsԭ��Edit�ؼ��ı�����ɫ"), tagNativeBKColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Edit

	//RichEdit
#pragma region RichEdit
	pPropUI = new CMFCPropertyGridProperty(_T("RichEdit"), classRichEdit);

	//normalimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("NormalImage"), _T(""), _T("ָ�����ı��༭��������ʾʱ��ͼƬ"), tagRichEditNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("HotImage"), _T(""), _T("ָ�����ı��༭�����ȵ�ʱ��ͼƬ"), tagRichEditHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//focusedimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("FocusedImage"), _T(""), _T("ָ�����ı��༭���ý�����ͼƬ"), tagRichEditFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//disabledimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("DisabledImage"), _T(""), _T("ָ�����ı��༭�򱻽��ú��ͼƬ"), tagRichEditDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//vscrollbar
	pProp = new CMFCPropertyGridProperty(_T("VScrollBar"), (_variant_t)false, _T("�Ƿ�ʹ�����������"), tagRichEditVScrollBar);
	pPropUI->AddSubItem(pProp);

	//autovscroll
	pProp = new CMFCPropertyGridProperty(_T("AutoVScroll"), (_variant_t)false, _T("�Ƿ��������������"), tagRichEditAutoVScroll);
	pPropUI->AddSubItem(pProp);

	//hscrollbar
	pProp = new CMFCPropertyGridProperty(_T("HScrollBar"), (_variant_t)false, _T("�Ƿ�ʹ�ú��������"), tagRichEditHScrollBar);
	pPropUI->AddSubItem(pProp);

	//autohscroll
	pProp = new CMFCPropertyGridProperty(_T("AutoHScroll"), (_variant_t)false, _T("�Ƿ�������������"), tagRichEditAutoHScroll);
	pPropUI->AddSubItem(pProp);

	//wanttab
	pProp = new CMFCPropertyGridProperty(_T("WantTab"), (_variant_t)false, _T("�Ƿ����tab������Ϣ"), tagRichEditWantTab);
	pPropUI->AddSubItem(pProp);

	//wantreturn
	pProp = new CMFCPropertyGridProperty(_T("WantReturn"), (_variant_t)false, _T("�Ƿ����return������Ϣ"), tagRichEditWantReturn);
	pPropUI->AddSubItem(pProp);

	//wantctrlreturn
	pProp = new CMFCPropertyGridProperty(_T("WantCtrlReturn"), (_variant_t)false, _T("�Ƿ����ctrl+return������Ϣ"), tagRichEditWantCtrlReturn);
	pPropUI->AddSubItem(pProp);

	//rich
	pProp = new CMFCPropertyGridProperty(_T("Rich"), (_variant_t)false, _T("�Ƿ�ʹ�ø���ʽ"), tagRichEditRich);
	pPropUI->AddSubItem(pProp);

	//multiline
	pProp = new CMFCPropertyGridProperty(_T("MultiLine"), (_variant_t)false, _T("�Ƿ�ʹ�ö���"), tagRichEditMultiLine);
	pPropUI->AddSubItem(pProp);

	//readonly
	pProp = new CMFCPropertyGridProperty(_T("ReadOnly"), (_variant_t)false, _T("�Ƿ�ֻ��"), tagRichEditReadOnly);
	pPropUI->AddSubItem(pProp);

	//readonlyshowcaret
	pProp = new CMFCPropertyGridProperty(_T("ReadOnlyShowCaret"), (_variant_t)false, _T("ֻ��״̬���Ƿ���ʾ�������"), tagRichEditReadOnlyShowCaret);
	pPropUI->AddSubItem(pProp);

	//canpaste
	pProp = new CMFCPropertyGridProperty(_T("CanPaste"), (_variant_t)true, _T("�Ƿ�����ճ������(Ĭ������true)"), tagRichEditCanPaste);
	pPropUI->AddSubItem(pProp);

	//password
	pProp = new CMFCPropertyGridProperty(_T("Password"), (_variant_t)false, _T("�Ƿ�ʹ�������"), tagRichEditPassword);
	pPropUI->AddSubItem(pProp);

	//align
	pProp = new CMFCPropertyGridProperty(_T("Align"), _T("Left"), _T("���ֶ��뷽ʽ,ȡֵleft��right��cente"), tagRichEditAlign);
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//font
	pProp = new CMFCPropertyGridProperty(_T("Font"), (_variant_t)(LONG)0, _T("����id"), tagRichEditFont);
	pPropUI->AddSubItem(pProp);

	//textcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("TextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("������ɫ"), tagRichEditTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//maxchar
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, _T("%s%d"), L"ָʾ�����ַ���󳤶�\n", DuiLib::cInitTextMax);
	pProp = new CMFCPropertyGridProperty(_T("MaxChar"), (_variant_t)(LONG)DuiLib::cInitTextMax, szBuf, tagRichEditMaxChar);
	pPropUI->AddSubItem(pProp);

	//textpadding
	pValueList = new CMFCPropertyGridProperty(_T("TextPadding"), tagRichEditTextPadding, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("ָ���ı��������߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("ָ���ı�������ϱ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("ָ���ı�������ұ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("ָ���ı�������±߾�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//tipvalue
	pProp = new CMFCPropertyGridProperty(_T("TipValue"), (_variant_t)_T(""), _T("Ԥ����ʾ��"), tagRichEditTipValue);
	pPropUI->AddSubItem(pProp);

	//tipvaluecolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("TipValueColor"), (COLORREF)ARGB(0xFF, 0, 0, 0), NULL, _T("Ԥ����ʾ����ɫ"), tagRichEditTipValueColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion RichEdit

	//Option
#pragma region Option
	pPropUI = new CMFCPropertyGridProperty(_T("Option"), classOption);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ForeImage"), _T(""), _T("ָ����ѡ���ǰ��ͼƬ"), tagOptForeImage);//foreimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("SelectedImage"), _T(""), _T("ָ����ѡ��ѡ����ͼƬ"), tagSelectedImage);//selectedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pProp = new CMFCPropertyGridProperty(_T("Selected"), (_variant_t)false, _T("ָʾ�Ƿ��ѱ�ѡ��"), tagSelected);//selected
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Group"), (_variant_t)_T(""), _T("ָ��������ϵ�����"), tagGroup);//group
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Option

	//Progress
#pragma region Progress
	pPropUI = new CMFCPropertyGridProperty(_T("Progress"), classProgress);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ForeImage"), _T(""), _T("ָ����������ǰ��ͼƬ"), tagForeImage);//foreimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList = new CMFCPropertyGridProperty(_T("MinMax"), tagMinMax, TRUE);//minmax
	pProp = new CMFCPropertyGridProperty(_T("Min"), (_variant_t)(LONG)0, _T("ָ������������Сֵ"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Max"), (_variant_t)(LONG)0, _T("ָ�������������ֵ"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp = new CMFCPropertyGridProperty(_T("Value"), (_variant_t)(LONG)0, _T("ָ����������ֵ"), tagValue);//value
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Hor"), (_variant_t)false, _T("ָʾ�������Ƿ�ˮƽ"), tagHor);//hor
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("IsStretchFore"), (_variant_t)false, _T("ָ��ǰ���Ƿ�����"), tagIsStretchFore);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Progress

	//Slider
#pragma region Slider
	pPropUI = new CMFCPropertyGridProperty(_T("Slider"), classSlider);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbImage"), _T(""), _T("ָ������Ļ���ͼƬ"), tagThumbImage);//thumbimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbHotImage"), _T(""), _T("ָ����������ȵ�ʱ��ͼƬ"), tagThumbHotImage);//thumbhotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbPushedImage"), _T(""), _T("ָ����������ѹ���ͼƬ"), tagThumbPushedImage);//thumbpushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList = new CMFCPropertyGridProperty(_T("ThumbSize"), tagThumbSize, TRUE);//thumbsize
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(LONG)0, _T("ָ�������Ŀ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(LONG)0, _T("ָ�������ĸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Slider

	//ActiveX
#pragma region ActiveX
	pPropUI = new CMFCPropertyGridProperty(_T("ActiveX"), classActiveX);

	//clsid
	pProp = new CMFCPropertyGridProperty(_T("Clsid"), (_variant_t)_T(""), _T("ָ��ActiveX�ؼ���Clsidֵ"), tagClsid);
	pPropUI->AddSubItem(pProp);

	//delaycreate
	pProp = new CMFCPropertyGridProperty(_T("DelayCreate"), (_variant_t)true, _T("ָʾ�Ƿ��ӳټ���ActiveX�ؼ�"), tagDelayCreate);
	pPropUI->AddSubItem(pProp);//added by �˾��� 2011-09-08

	// modulename
	pProp = new CMFCPropertyGridProperty(_T("ModuleName"), (_variant_t)_T(""), _T("ָʾ��ָ��λ�ü���ActiveX�ؼ�\n��(flash/flash.ocx)"), tagModuleName);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion ActiveX

	//Container
#pragma region Container
	pPropUI = new CMFCPropertyGridProperty(_T("Container"), classContainer);

	pValueList = new CMFCPropertyGridProperty(_T("Inset"), tagInset, TRUE);//inset
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("ָ�������ͻ��������߾�\n0"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("ָ�������ͻ�������ϱ߾�\n0"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("ָ�������ͻ�������ұ߾�\n0"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("ָ�������ͻ�������±߾�\n0"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp = new CMFCPropertyGridProperty(_T("ChildPadding"), (_variant_t)(LONG)0, _T("ָ���ӿؼ�֮��ļ��\n0"), tagChildPadding);//childpadding
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("MouseChild"), (_variant_t)false, _T("ָ�����ؼ����ӿؼ��Ƿ������Ӧ�û�����\nTrue"), tagMouseChild);//mousechild
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("HScrollBar"), (_variant_t)false, _T("ָʾ�Ƿ�����ˮƽ������\nFalse"), tagHScrollBar);//hscrollbar
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("VScrollBar"), (_variant_t)false, _T("ָʾ�Ƿ����ô�ֱ������\nFalse"), tagVScrollBar);//vscrollbar
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Container

	//Item
#pragma region Item
	pPropUI = new CMFCPropertyGridProperty(_T("Item"), classItem);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ItemBkImage"), _T(""), _T("ָ������������ʾʱ��ͼƬ"), tagItemBkImage);//itembkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ItemHotImage"), _T(""), _T("ָ���������ȵ�ʱ��ͼƬ"), tagItemHotImage);//itemhotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ItemSelectedImage"), _T(""), _T("ָ�����ѡ��ʱ��ͼƬ"), tagItemSelectedImage);//itemselectedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ItemDisabledImage"), _T(""), _T("ָ��������ú��ͼƬ"), tagItemDisabledImage);//itemdisabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//itemtextpadding
	pValueList = new CMFCPropertyGridProperty(_T("ItemTextPadding"), tagItemTextPadding, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("ָ�������ı��������߾�\n0"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("ָ�������ı�������ϱ߾�\n0"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("ָ�������ı�������ұ߾�\n0"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("ָ�������ı�������±߾�\n0"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//itemalign
	pProp = new CMFCPropertyGridProperty(_T("ItemAlign"), _T("Center"), _T("ָʾ�����ı��Ķ��뷽ʽ\nCenter"), tagItemAlign);
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//itemtextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemTextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ�������ı�����ɫ"), tagItemTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itembkcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemBkColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ�����������ɫ"), tagItemBkColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemselectedtextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemSelectedTextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ�����ѡ�к��ı�����ɫ"), tagItemSelectedTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemselectedbkcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemSelectedBkColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ�����ѡ�к󱳾�����ɫ"), tagItemSelectedBkColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemhottextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemHotTextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���������ȵ�ʱ�ı�����ɫ"), tagItemHotTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemhotbkcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemHotBkColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ���������ȵ�ʱ��������ɫ"), tagItemHotBkColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemdisabledtextcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemDisabledTextColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��������ú��ı�����ɫ"), tagItemDisabledTextColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemdisabledbkcolor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemDisabledBkColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ��������ú󱳾�����ɫ"), tagItemDisabledBkColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//ItemLineColor
	pPropColor = new CMFCPropertyGridColor32Property(_T("ItemLineColor"), (LONG)ARGB(0xFF, 0, 0, 0), NULL, _T("ָ������ָ��ߵ���ɫ"), tagItemLineColor);
	pPropColor->EnableOtherButton(_T("����..."));
	pPropColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//ItemFont
	pProp = new CMFCPropertyGridProperty(_T("ItemFont"), (_variant_t)(LONG)-1, _T("ָ�������ı�������\n-1"), tagItemFont);
	pPropUI->AddSubItem(pProp);

	//ItemShowHtml
	pProp = new CMFCPropertyGridProperty(_T("ItemShowHtml"), (_variant_t)false, _T("ָʾ�Ƿ�ʹ��Html��ʽ�ı�\nFalse"), tagItemShowHtml);
	pPropUI->AddSubItem(pProp);

	//MultiExpanding
	pProp = new CMFCPropertyGridProperty(_T("MultiExpanding"), (_variant_t)false, _T("ָʾ�Ƿ�֧�ֶ��itemͬʱ��\nFalse"), tagMultiExpanding);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Item

	//Combo
#pragma region Combo
	pPropUI = new CMFCPropertyGridProperty(_T("Combo"), classCombo);

	pPropImage = new CMFCPropertyGridImageProperty(_T("NormalImage"), _T(""), _T("ָ����Ͽ�������ʾʱ��ͼƬ"), tagComboNormalImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("HotImage"), _T(""), _T("ָ����Ͽ����ȵ�ʱ��ͼƬ"), tagComboHotImage);//hotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("PushedImage"), _T(""), _T("ָ����Ͽ򱻰�ѹ��ʱ��ͼƬ"), tagComboPushedImage);//pushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("FocusedImage"), _T(""), _T("ָ����Ͽ��ý�����ͼƬ"), tagComboFocusedImage);//focusedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("DisabledImage"), _T(""), _T("ָ����Ͽ򱻽��ú��ͼƬ"), tagComboDisabledImage);//disabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList = new CMFCPropertyGridProperty(_T("TextPadding"), tagComboTextPadding, TRUE);//textpadding
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)(LONG)0, _T("ָ���ı��������߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)(LONG)0, _T("ָ���ı�������ϱ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)(LONG)0, _T("ָ���ı�������ұ߾�"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)(LONG)0, _T("ָ���ı�������±߾�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList = new CMFCPropertyGridProperty(_T("DropBoxSize"), tagComboDropBoxSize, TRUE);//dropboxsize
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(LONG)0, _T("�����б�Ŀ��"));
	pValueList->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(LONG)0, _T("�����б�ĸ߶�"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Combo

	//HorizontalLayout
#pragma region HorizontalLayout
	pPropUI = new CMFCPropertyGridProperty(_T("HorizontalLayout"), classHorizontalLayout);

	pProp = new CMFCPropertyGridProperty(_T("SepWidth"), (_variant_t)(LONG)0, _T("�ָ�����,������ʾ�ָ�������߻����ұ�\n0"), tagSepWidth);//sepwidth
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("SepImm"), (_variant_t)false, _T("�϶��ָ����Ƿ������ı��С\nfalse"), tagSepImm);//sepimm
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion HorizontalLayout

	//TileLayout
#pragma region TileLayout
	pPropUI = new CMFCPropertyGridProperty(_T("TileLayout"), classTileLayout);

	pProp = new CMFCPropertyGridProperty(_T("Columns"), (_variant_t)(LONG)0, _T("ָ�����в��ֵ�����"), tagColumns);//columns
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion TileLayout

	//List
#pragma region List
	pPropUI = new CMFCPropertyGridProperty(_T("List"), classList);

	pProp = new CMFCPropertyGridProperty(_T("Header"), (_variant_t)false, _T("ָ���Ƿ���ʾ�б��ͷ\nTrue"), tagListHeader);
	pPropUI->AddSubItem(pProp);

	pPropImage = new CMFCPropertyGridImageProperty(_T("headerbkimage"), _T(""), _T("ָ����ͷ����ͼƬ"), tagListHeaderBKImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion List

	//ScrollBar
#pragma region ScrollBar

	pPropUI = new CMFCPropertyGridProperty(_T("ScrollBar"), classScrollBar);

	pProp = new CMFCPropertyGridProperty(_T("Range"), (_variant_t)(LONG)0, _T("ָ��������Χ\n100"), tagScrollBarRange);
	pPropUI->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Value"), (_variant_t)(LONG)0, _T("ָ������λ��\n0"), tagScrollBarValue);
	pPropUI->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("LineSize"), (_variant_t)(LONG)0, _T("ָ������һ�еĴ�С\n8"), tagScrollBarLineSize);
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("ShowButton1"), (_variant_t)false, _T("�Ƿ���ʾ����ϰ�ť\nTrue"), tagScrollBarShowButton1);
	pPropUI->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ShowButton2"), (_variant_t)false, _T("�Ƿ���ʾ����ϰ�ť\nTrue"), tagScrollBarShowButton2);
	pPropUI->AddSubItem(pProp);

	pPropImage = new CMFCPropertyGridImageProperty(_T("Button1NormalImage"), _T(""), _T("ָ������ϰ�ť��ͨ״̬ͼƬ"), tagScrollBarButton1NormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("Button1HotImage"), _T(""), _T("ָ������ϰ�ť�������״̬ͼƬ"), tagScrollBarButton1HotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("Button1PushedImage"), _T(""), _T("ָ������ϰ�ť��갴��״̬ͼƬ"), tagScrollBarButton1PushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("Button1DisabledImage"), _T(""), _T("ָ������ϰ�ť������״̬ͼƬ"), tagScrollBarButton1DisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("Button2NormalImage"), _T(""), _T("ָ������ϰ�ť��ͨ״̬ͼƬ"), tagScrollBarButton2NormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("Button2HotImage"), _T(""), _T("ָ������ϰ�ť�������״̬ͼƬ"), tagScrollBarButton2HotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("Button2PushedImage"), _T(""), _T("ָ������ϰ�ť��갴��״̬ͼƬ"), tagScrollBarButton2PushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("Button2DisabledImage"), _T(""), _T("ָ������ϰ�ť���������̬ͼƬ"), tagScrollBarButton2DisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbNormalImage"), _T(""), _T("ָ��������ͨ״̬ͼƬ"), tagScrollBarThumbNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbHotImage"), _T(""), _T("ָ�������������״̬ͼƬ"), tagScrollBarThumbHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbPushedImage"), _T(""), _T("ָ��������갴��״̬ͼƬ"), tagScrollBarThumbPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("ThumbDisabledImage"), _T(""), _T("ָ���������״̬ͼƬ"), tagScrollBarThumbDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("RailNormalImage"), _T(""), _T("ָ�������м��ʶ��ͨ״̬ͼƬ"), tagScrollBarRailNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("RailHotImage"), _T(""), _T("ָ�������м��ʶ�������״̬ͼƬ"), tagScrollBarRailHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("RailPushedImage"), _T(""), _T("ָ�������м��ʶ��갴��״̬ͼƬ"), tagScrollBarRailPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("RailDisabledImage"), _T(""), _T("ָ�������м��ʶ����״̬ͼƬ"), tagScrollBarRailDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage = new CMFCPropertyGridImageProperty(_T("BKNormalImage"), _T(""), _T("ָ��������ͨ״̬ͼƬ"), tagScrollBarBKNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("BKHotImage"), _T(""), _T("ָ�������������״̬ͼƬ"), tagScrollBarBKHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("BKPushedImage"), _T(""), _T("ָ��������갴��״̬ͼƬ"), tagScrollBarBKPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage = new CMFCPropertyGridImageProperty(_T("BKDisabledImage"), _T(""), _T("ָ����������״̬ͼƬ"), tagScrollBarBKDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);

#pragma endregion ScrollBar

#pragma region TabLayout
	pPropUI = new CMFCPropertyGridProperty(_T("TabLayout"), classTabLayout);

	// selectedid
	pProp = new CMFCPropertyGridProperty(_T("selectedid"), (_variant_t)(LONG)0, _T("Ĭ��ѡ�е�ҳ��ID\n��0��ʼ����"), tagSelectedID);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);

#pragma endregion TabLayout

#pragma region ListHeaderItem
	pPropUI = new CMFCPropertyGridProperty(_T("ListHeaderItem"), classListHeaderItem);

	// dragable
	pProp = new CMFCPropertyGridProperty(_T("Dragable"), (_variant_t)true, _T("�Ƿ���϶��ı��С\ntrue"), tagDragable);
	pPropUI->AddSubItem(pProp);

	// sepwidth
	pProp = new CMFCPropertyGridProperty(_T("SepWidth"), (_variant_t)(LONG)0, _T("�ָ�����\n4"), tagListHeaderItemSepWidth);
	pPropUI->AddSubItem(pProp);

	// normalimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("NormalImage"), _T(""), _T("��ͨ״̬ͼƬ"), tagListHeaderItemNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// hotimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("HotImage"), _T(""), _T("���������״̬ͼƬ"), tagListHeaderItemHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// PushedImage
	pPropImage = new CMFCPropertyGridImageProperty(_T("PushedImage"), _T(""), _T("��갴�µ�״̬ͼƬ"), tagListHeaderItemPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// focusedimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("FocusedImage"), _T(""), _T("��ý���ʱ��״̬ͼƬ"), tagListHeaderItemFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// sepimage
	pPropImage = new CMFCPropertyGridImageProperty(_T("SepImage"), _T(""), _T("�϶���ͼƬ"), tagSepImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion ListHeaderItem

#pragma region WebBrowser

	pPropUI = new CMFCPropertyGridProperty(_T("WebBrowser"), classWebBrowser);

	// homepage
	pProp = new CMFCPropertyGridProperty(_T("homepage"), (_variant_t)_T(""), _T("Ĭ����ַ\n"), tagWebBrowserHomePage);
	pPropUI->AddSubItem(pProp);

	// autonavi
	pProp = new CMFCPropertyGridProperty(_T("autonavi"), (_variant_t)true, _T("�Ƿ���ʾĬ��ҳ��\ntrue"), tagDragable);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion WebBrowser

	HideAllProperties();
}

void CUIProperties::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

DWORD CUIProperties::TransColorToARGB(COLORREF argb)
{
	DWORD iColor = argb;
	int nA = 0, nR = 0, nG = 0, nB = 0;
	nA = GetAValue(iColor);
	nR = LOBYTE((iColor) >> 16);
	nG = GetGValue(iColor);
	nB = LOBYTE((iColor));

		if (nA <= 0)
			nA = 0xFF;

	iColor = ARGB(nA, nR, nG, nB);
	return iColor;
}

void CUIProperties::HideAllProperties(BOOL bAdjustLayout/*=FALSE*/)
{
	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
		m_wndPropList.GetProperty(i)->Show(FALSE, bAdjustLayout);

	m_pControl = NULL;
}

void CUIProperties::ShowProperty(CControlUI* pControl)
{
	if (m_pControl == pControl)
		return;

	if (pControl == NULL)
	{
		HideAllProperties(TRUE);
		return;
	}

	HideAllProperties();

	m_wndPropList.SetCurSel(NULL, FALSE);

	ExtendedAttributes mDummy;
	ExtendedAttributes* pExtended = (ExtendedAttributes*)pControl->GetTag();
	if (pExtended == NULL)
	{
		pExtended = &mDummy;
		ZeroMemory(pExtended, sizeof(ExtendedAttributes));
	}
	switch (pExtended->nClass)
	{
		case classWindow:
			ShowWindowProperty(pControl);
			break;
		case classControl:
			ShowControlProperty(pControl);
			break;
		case classLabel:
		case classText:
			ShowLabelProperty(pControl);
			break;
		case classButton:
			ShowButtonProperty(pControl);
			break;
		case classEdit:
			ShowEditProperty(pControl);
			break;
		case classRichEdit:
			ShowRichEditProperty(pControl);
			break;
		case classOption:
			ShowOptionProperty(pControl);
			break;
		case classProgress:
			ShowProgressProperty(pControl);
			break;
		case classSlider:
			ShowSliderProperty(pControl);
			break;
		case classCombo:
			ShowComboProperty(pControl);
			break;
		case classActiveX:
			ShowActiveXProperty(pControl);
			break;
		case classContainer:
		case classVerticalLayout:
		case classListContainerElement:/*!*/
			ShowContainerProperty(pControl);
			break;
		case classTabLayout:
			ShowTabLayoutPropery(pControl);
			break;
		case classList:
			ShowListProperty(pControl);
			break;
		case classHorizontalLayout:
			ShowHorizontalLayoutProperty(pControl);
			break;
		case classTileLayout:
			ShowTileLayoutProperty(pControl);
			break;
		case classScrollBar:
			ShowScrollBarProperty(pControl);
			break;
		case classListHeaderItem:
			ShowListHeaderItemPropery(pControl);
			break;
		case classWebBrowser:
			ShowWebBrowserPropery(pControl);
			break;
		default:
			ShowControlProperty(pControl);
			break;
	}

	m_pControl = pControl;
	m_wndPropList.AdjustLayout();
}

void CUIProperties::ShowWindowProperty(CControlUI* pControl)
{
	ASSERT(pControl);
	CFormUI* pForm = static_cast<CFormUI*>(pControl->GetInterface(_T("Form")));
	ASSERT(pForm);

	CMFCPropertyGridProperty* pPropForm = m_wndPropList.FindItemByData(classWindow, FALSE);
	ASSERT(pPropForm);

	int iIndex = 0;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;
	SIZE size = { 0, 0 };
	RECT rect = { 0, 0, 0, 0 };

	//size
	size = pForm->GetInitSize();
	iIndex = tagWindowSize - tagWindow;
	CMFCPropertyGridProperty* pValueList = pPropForm->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);

	//caption
	rect = pForm->GetCaptionRect();
	iIndex = tagCaption - tagWindow;
	pValueList = pPropForm->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//sizebox
	rect = pForm->GetSizeBox();
	iIndex = tagSizeBox - tagWindow;
	pValueList = pPropForm->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//roundcorner
	size = pForm->GetRoundCorner();
	iIndex = tagRoundCorner - tagWindow;
	pValueList = pPropForm->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);

	//mininfo
	size = pForm->GetMinInfo();
	iIndex = tagMinInfo - tagWindow;
	pValueList = pPropForm->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);

	//maxinfo
	size = pForm->GetMaxInfo();
	iIndex = tagMaxInfo - tagWindow;
	pValueList = pPropForm->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);

	//showdirty
	iIndex = tagShowDirty - tagWindow;
	pPropForm->GetSubItem(iIndex)->SetValue((_variant_t)pForm->IsShowUpdateRect());
	pPropForm->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pForm->IsShowUpdateRect());

	// tagAlpha
	iIndex = tagAlpha - tagWindow;
	pPropForm->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pForm->GetAlpha());
	pPropForm->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pForm->GetAlpha());

	// tagBkTrans
	iIndex = tagBkTrans - tagWindow;
	pPropForm->GetSubItem(iIndex)->SetValue((_variant_t)pForm->GetBackgroundTransparent());
	pPropForm->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pForm->GetBackgroundTransparent());

	// tagDefaultFontColor
	iIndex = tagDefaultFontColor - tagWindow;
	iColor = TransColorToARGB(pForm->GetDefaultFontColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	// tagSelectedFontColor
	iIndex = tagSelectedFontColor - tagWindow;
	iColor = TransColorToARGB(pForm->GetDefaultSelectedFontColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	// tagDisabledFontColor
	iIndex = tagDisabledFontColor - tagWindow;
	iColor = TransColorToARGB(pForm->GetDefaultDisabledFontColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	// tagLinkFontColor
	iIndex = tagLinkFontColor - tagWindow;
	iColor = TransColorToARGB(pForm->GetDefaultLinkFontColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	// tagLinkHoverFontColor
	iIndex = tagLinkHoverFontColor - tagWindow;
	iColor = TransColorToARGB(pForm->GetDefaultLinkHoverFontColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	pPropForm->Show(TRUE, FALSE);
}

void CUIProperties::ShowControlProperty(CControlUI* pControl)
{
	ASSERT(pControl);

	CMFCPropertyGridProperty* pPropControl = m_wndPropList.FindItemByData(classControl, FALSE);
	ASSERT(pPropControl);

	DWORD iIndex = tagControl;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;
	RECT rect = { 0, 0, 0, 0 };

	//name
	iIndex = tagName - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->GetName());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->GetName());

	//text
	iIndex = tagText - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->GetText());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->GetText());

	//pos
	rect = pControl->GetPos();
	SIZE szXY = { rect.left, rect.top };
	INT iWidth = rect.right - rect.left;
	INT iHeight = rect.bottom - rect.top;
	iIndex = tagPos - tagControl;
	CMFCPropertyGridProperty* pValueList = pPropControl->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)szXY.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)szXY.cy);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)(szXY.cx + iWidth));
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)(szXY.cy + iHeight));
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)szXY.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)szXY.cy);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)(szXY.cx + iWidth));
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)(szXY.cy + iHeight));

	//relativepos
	TRelativePosUI posRelative = pControl->GetRelativePos();
	iIndex = tagRelativePos - tagControl;
	pValueList = pPropControl->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)posRelative.nMoveXPercent);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)posRelative.nMoveYPercent);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)posRelative.nZoomXPercent);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)posRelative.nZoomYPercent);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)posRelative.nMoveXPercent);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)posRelative.nMoveYPercent);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)posRelative.nZoomXPercent);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)posRelative.nZoomYPercent);

	//size
	iIndex = tagSize - tagControl;
	pValueList = pPropControl->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetWidth());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetHeight());

	//minsize
	iIndex = tagMinSize - tagControl;
	pValueList = pPropControl->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetMinWidth());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetMinHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetMinWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetMinHeight());

	//maxsize
	iIndex = tagMaxSize - tagControl;
	pValueList = pPropControl->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetMaxWidth());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetMaxHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetMaxWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetMaxHeight());

	//padding
	iIndex = tagPadding - tagControl;
	pValueList = pPropControl->GetSubItem(iIndex);
	rect = pControl->GetPadding();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//bkimage
	iIndex = tagBkImage - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->GetBkImage());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->GetBkImage());

	//bkcolor
	iIndex = tagBkColor - tagControl;
	iColor = TransColorToARGB(pControl->GetBkColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	//bkcolor2
	iIndex = tagBkColor2 - tagControl;
	iColor = TransColorToARGB(pControl->GetBkColor2());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	//bordercolor
	iIndex = tagBorderColor - tagControl;
	iColor = TransColorToARGB(pControl->GetBorderColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	//focusbordercolor
	iIndex = tagFocusBorderColor - tagControl;
	iColor = TransColorToARGB(pControl->GetFocusBorderColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	//bordersize
	iIndex = tagBorderSize - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pControl->GetBorderSize());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pControl->GetBorderSize());
	
	//borderround
	iIndex = tagBorderRound - tagControl;
	pValueList = pPropControl->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetBorderRound().cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetBorderRound().cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetBorderRound().cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetBorderRound().cy);
	
	//enabled
	iIndex = tagEnabled - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsEnabled());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsEnabled());
	
	//float
	iIndex = tagFloat - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsFloat());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsFloat());
	
	//visible
	iIndex = tagVisible - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsVisible());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsVisible());
	
	//mouse
	iIndex = tagMouse - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsMouseEnabled());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsMouseEnabled());
	
	//menu
	iIndex = tagMenu - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsContextMenuUsed());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsContextMenuUsed());
	
	//colorhsl
	iIndex = tagColorHSL - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsColorHSL());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsColorHSL());
	
	//tooltip
	iIndex = tagTooltip - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->GetToolTip());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->GetToolTip());
	
	//userdata
	iIndex = tagUserData - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->GetUserData());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->GetUserData());
	
	//keyboard
	iIndex = tagKeyBoard - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsKeyboardEnabled());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsKeyboardEnabled());

	//Tab
	iIndex = tagTab - tagControl;
	pPropControl->GetSubItem(iIndex)->SetValue((_variant_t)pControl->IsTabEnable());
	pPropControl->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pControl->IsTabEnable());

	pPropControl->Show(TRUE, FALSE);
}

void CUIProperties::ShowLabelProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CLabelUI* pLabel = static_cast<CLabelUI*>(pControl->GetInterface(_T("Label")));
	ASSERT(pLabel);

	CMFCPropertyGridProperty* pPropLabel = m_wndPropList.FindItemByData(classLabel, FALSE);
	ASSERT(pPropLabel);

	int iIndex = 0;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;
	SIZE size = { 0, 0 };
	RECT rect = { 0, 0, 0, 0 };

	//align
	UINT uStyle = pLabel->GetTextStyle();
	CString strStyle = _T("left");
	if (uStyle&DT_CENTER)
		strStyle = _T("center");
	else if (uStyle&DT_LEFT)
		strStyle = _T("left");
	else if (uStyle&DT_RIGHT)
		strStyle = _T("right");

	else if (uStyle & (DT_TOP | DT_SINGLELINE))
		strStyle = _T("top");
	else if (uStyle & (DT_VCENTER | DT_SINGLELINE))
		strStyle = _T("vcenter");
	else if (uStyle & (DT_BOTTOM | DT_SINGLELINE))
		strStyle = _T("bottom");
	iIndex = tagAlign - tagLabel;
	pPropLabel->GetSubItem(iIndex)->SetValue((_variant_t)strStyle);
	pPropLabel->GetSubItem(iIndex)->SetOriginalValue((_variant_t)strStyle);

	//textcolor
	iIndex = tagTextColor - tagLabel;
	iColor = TransColorToARGB(pLabel->GetTextColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//disabledtextcolor
	iIndex = tagDisabledTextColor - tagLabel;
	iColor = TransColorToARGB(pLabel->GetDisabledTextColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//font
	iIndex = tagFont - tagLabel;
	pPropLabel->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pLabel->GetFont());
	pPropLabel->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pLabel->GetFont());

	//textpadding
	iIndex = tagTextPadding - tagLabel;
	CMFCPropertyGridProperty* pValueList = pPropLabel->GetSubItem(iIndex);
	rect = pLabel->GetTextPadding();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//showhtml
	iIndex = tagShowHtml - tagLabel;
	pPropLabel->GetSubItem(iIndex)->SetValue((_variant_t)pLabel->IsShowHtml());
	pPropLabel->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pLabel->IsShowHtml());

	//endellipsis
	DWORD dwStyle = pLabel->GetTextStyle();
	bool bEndEllipsis = (dwStyle & DT_END_ELLIPSIS) != 0;
	iIndex = tagEndEllipsis - tagLabel;
	pPropLabel->GetSubItem(iIndex)->SetValue((_variant_t)bEndEllipsis);
	pPropLabel->GetSubItem(iIndex)->SetOriginalValue((_variant_t)bEndEllipsis);

	pPropLabel->Show(TRUE, FALSE);
}

void CUIProperties::ShowButtonProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CButtonUI* pButton = static_cast<CButtonUI*>(pControl->GetInterface(_T("Button")));
	ASSERT(pButton);

	CMFCPropertyGridProperty* pPropButton = m_wndPropList.FindItemByData(classButton, FALSE);
	ASSERT(pPropButton);

	int iIndex = 0;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;

	//normalimage
	iIndex = tagButtonNormalImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetNormalImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetNormalImage());

	//hotimage
	iIndex = tagButtonHotImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetHotImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetHotImage());

	//pushedimage
	iIndex = tagButtonPushedImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetPushedImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetPushedImage());

	//focusedimage
	iIndex = tagButtonFocusedImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetFocusedImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetFocusedImage());

	//disabledimage
	iIndex = tagButtonDisabledImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetDisabledImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetDisabledImage());

	//foreimage
	iIndex = tagButtonForeImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetForeImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetForeImage());

	//hotforeimage
	iIndex = tagButtonHotForeImage - tagButton;
	pPropButton->GetSubItem(iIndex)->SetValue((_variant_t)pButton->GetHotForeImage());
	pPropButton->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pButton->GetHotForeImage());

	//hotbkcolor
	iIndex = tagButtonHotBkColor - tagButton;
	iColor = TransColorToARGB(pButton->GetHotBkColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//hottextcolor
	iIndex = tagButtonHotTextColor - tagButton;
	iColor = TransColorToARGB(pButton->GetHotTextColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//pushedtextcolor
	iIndex = tagButtonPushedTextColor - tagButton;
	iColor = TransColorToARGB(pButton->GetPushedTextColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//focusedtextcolor
	iIndex = tagButtonFocusedTextColor - tagButton;
	iColor = TransColorToARGB(pButton->GetFocusedTextColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	pPropButton->Show(TRUE, FALSE);
}

void CUIProperties::ShowEditProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CEditUI* pEdit = static_cast<CEditUI*>(pControl->GetInterface(_T("Edit")));
	ASSERT(pEdit);

	CMFCPropertyGridProperty* pPropEdit = m_wndPropList.FindItemByData(classEdit, FALSE);
	ASSERT(pPropEdit);

	int iIndex = 0;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;
	SIZE size = { 0, 0 };
	RECT rect = { 0, 0, 0, 0 };

	//normalimage
	iIndex = tagEditNormalImage - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)pEdit->GetNormalImage());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pEdit->GetNormalImage());

	//hotimage
	iIndex = tagEditHotImage - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)pEdit->GetHotImage());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pEdit->GetHotImage());

	//focusedimage
	iIndex = tagEditFocusedImage - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)pEdit->GetFocusedImage());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pEdit->GetFocusedImage());

	//disabledimage
	iIndex = tagEditDisabledImage - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)pEdit->GetDisabledImage());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pEdit->GetDisabledImage());

	//readonly
	iIndex = tagReadOnly - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)pEdit->IsReadOnly());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pEdit->IsReadOnly());

	//password
	iIndex = tagPassword - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)pEdit->IsPasswordMode());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pEdit->IsPasswordMode());

	//maxchar
	iIndex = tagMaxChar - tagEdit;
	pPropEdit->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pEdit->GetMaxChar());
	pPropEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pEdit->GetMaxChar());

	//nativebkcolor
	iIndex = tagNativeBKColor - tagEdit;
	iColor = TransColorToARGB(pEdit->GetNativeEditBkColor());
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropEdit->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropEdit->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	pPropEdit->Show(TRUE, FALSE);
}

void CUIProperties::ShowRichEditProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(pControl->GetInterface(_T("RichEdit")));
	ASSERT(pRichEdit);

	LONG lTwhStyle = pRichEdit->GetWinStyle();
	CMFCPropertyGridProperty* pPropRichEdit = m_wndPropList.FindItemByData(classRichEdit, FALSE);
	ASSERT(pPropRichEdit);

	int iIndex = 0;
	bool bResult = false;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;

	//normalimage
	iIndex = tagRichEditNormalImage - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->GetNormalImage());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pRichEdit->GetNormalImage());

	//hotimage
	iIndex = tagRichEditHotImage - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->GetHotImage());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pRichEdit->GetHotImage());

	//focusedimage
	iIndex = tagRichEditFocusedImage - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->GetFocusedImage());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pRichEdit->GetFocusedImage());

	//disabledimage
	iIndex = tagRichEditDisabledImage - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->GetDisabledImage());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pRichEdit->GetDisabledImage());

	//vscrollbar
	iIndex = tagRichEditVScrollBar - tagRichEdit;
	bResult = lTwhStyle & (ES_DISABLENOSCROLL | WS_VSCROLL);
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)bResult);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//autovscroll
	iIndex = tagRichEditAutoVScroll - tagRichEdit;
	bResult = lTwhStyle & ES_AUTOVSCROLL;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)bResult);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//hscrollbar
	iIndex = tagRichEditHScrollBar - tagRichEdit;
	bResult = lTwhStyle & (ES_DISABLENOSCROLL | WS_HSCROLL);
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)bResult);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//autohscroll
	iIndex = tagRichEditAutoHScroll - tagRichEdit;
	bResult = lTwhStyle & ES_AUTOHSCROLL;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)bResult);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//wanttab
	iIndex = tagRichEditWantTab - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsWantTab());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//wantreturn
	iIndex = tagRichEditWantReturn - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsWantReturn());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//wantctrlreturn
	iIndex = tagRichEditWantCtrlReturn - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsWantCtrlReturn());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//rich
	iIndex = tagRichEditRich - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsRich());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//multiline
	iIndex = tagRichEditMultiLine - tagRichEdit;
	bResult = lTwhStyle & ES_MULTILINE;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)bResult);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//readonly
	iIndex = tagRichEditReadOnly - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsReadOnly());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//readonlyshowcaret
	iIndex = tagRichEditReadOnlyShowCaret - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsReadOnlyShowCaret());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//canpaste
	iIndex = tagRichEditCanPaste - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->IsCanPaste());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//password
	iIndex = tagRichEditPassword - tagRichEdit;
	bResult = lTwhStyle & ES_PASSWORD;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)bResult);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)false);

	//align
	iIndex = tagRichEditAlign - tagRichEdit;
	CString strStyle = _T("Left");
	if (lTwhStyle & DT_CENTER)
		strStyle = _T("Center");
	else if (lTwhStyle&DT_LEFT)
		strStyle = _T("Left");
	else if (lTwhStyle & DT_RIGHT)
		strStyle = _T("Right");
	else if (lTwhStyle & DT_TOP)
		strStyle = _T("Top");
	else if (lTwhStyle & DT_BOTTOM)
		strStyle = _T("Bottom");
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)strStyle);
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)_T("Left"));

	//font
	iIndex = tagRichEditFont - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pRichEdit->GetFont());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)-1);

	//textcolor
	iIndex = tagRichEditTextColor - tagRichEdit;
	iColor = TransColorToARGB(pRichEdit->GetTextColor());
	iOrignalColor = 0;
	static_cast<CMFCPropertyGridColor32Property*>(pPropRichEdit->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropRichEdit->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//maxchar
	iIndex = tagRichEditMaxChar - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pRichEdit->GetLimitText());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)DuiLib::cInitTextMax);

	//textpadding
	iIndex = tagRichEditTextPadding - tagRichEdit;
	RECT rcTexPaddding = pRichEdit->GetTextPadding();
	CMFCPropertyGridProperty* pItem = pPropRichEdit->GetSubItem(iIndex);
	pItem->GetSubItem(0)->SetValue((_variant_t)(LONG)rcTexPaddding.left);
	pItem->GetSubItem(1)->SetValue((_variant_t)(LONG)rcTexPaddding.top);
	pItem->GetSubItem(2)->SetValue((_variant_t)(LONG)rcTexPaddding.right);
	pItem->GetSubItem(3)->SetValue((_variant_t)(LONG)rcTexPaddding.bottom);
	pItem->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rcTexPaddding.left);
	pItem->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rcTexPaddding.top);
	pItem->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rcTexPaddding.right);
	pItem->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rcTexPaddding.bottom);

	//tipvalue
	iIndex = tagRichEditTipValue - tagRichEdit;
	pPropRichEdit->GetSubItem(iIndex)->SetValue((_variant_t)pRichEdit->GetTipValue());
	pPropRichEdit->GetSubItem(iIndex)->SetOriginalValue((_variant_t)_T(""));

	//tipvaluecolor
	iIndex = tagRichEditTipValueColor - tagRichEdit;
	iColor = TransColorToARGB(pRichEdit->GetTipValueColor());
	iOrignalColor = 0xFFBAC0C5;
	static_cast<CMFCPropertyGridColor32Property*>(pPropRichEdit->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)(iColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropRichEdit->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	pPropRichEdit->Show(TRUE, FALSE);
}

void CUIProperties::ShowOptionProperty(CControlUI* pControl)
{
	ShowButtonProperty(pControl);

	ASSERT(pControl);
	COptionUI* pOption = static_cast<COptionUI*>(pControl->GetInterface(_T("Option")));
	ASSERT(pOption);

	CMFCPropertyGridProperty* pPropOption = m_wndPropList.FindItemByData(classOption, FALSE);
	ASSERT(pPropOption);

	int iIndex = 0;

	//foreimage
	iIndex = tagOptForeImage - tagOption;
	pPropOption->GetSubItem(iIndex)->SetValue((_variant_t)pOption->GetForeImage());
	pPropOption->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pOption->GetForeImage());

	//selectedimage
	iIndex = tagSelectedImage - tagOption;
	pPropOption->GetSubItem(iIndex)->SetValue((_variant_t)pOption->GetSelectedImage());
	pPropOption->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pOption->GetSelectedImage());

	//selected
	iIndex = tagSelected - tagOption;
	pPropOption->GetSubItem(iIndex)->SetValue((_variant_t)pOption->IsSelected());
	pPropOption->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pOption->IsSelected());

	//group
	iIndex = tagGroup - tagOption;
	pPropOption->GetSubItem(iIndex)->SetValue((_variant_t)pOption->GetGroup());
	pPropOption->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pOption->GetGroup());

	pPropOption->Show(TRUE, FALSE);
}

void CUIProperties::ShowProgressProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CProgressUI* pProgress = static_cast<CProgressUI*>(pControl->GetInterface(_T("Progress")));
	ASSERT(pProgress);

	CMFCPropertyGridProperty* pPropProgress = m_wndPropList.FindItemByData(classProgress, FALSE);
	ASSERT(pPropProgress);

	int iIndex = 0;

	//foreimage
	iIndex = tagForeImage - tagProgress;
	pPropProgress->GetSubItem(iIndex)->SetValue((_variant_t)pProgress->GetForeImage());
	pPropProgress->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pProgress->GetForeImage());

	//minmax
	iIndex = tagMinMax - tagProgress;
	CMFCPropertyGridProperty* pValueList = pPropProgress->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pProgress->GetMinValue());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pProgress->GetMaxValue());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pProgress->GetMinValue());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pProgress->GetMaxValue());

	//value
	iIndex = tagValue - tagProgress;
	pPropProgress->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pProgress->GetValue());
	pPropProgress->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pProgress->GetValue());

	//hor
	iIndex = tagHor - tagProgress;
	pPropProgress->GetSubItem(iIndex)->SetValue((_variant_t)pProgress->IsHorizontal());
	pPropProgress->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pProgress->IsHorizontal());

	//tagIsStretchFore
	iIndex = tagIsStretchFore - tagProgress;
	pPropProgress->GetSubItem(iIndex)->SetValue((_variant_t)pProgress->IsStretchForeImage());
	pPropProgress->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pProgress->IsStretchForeImage());

	pPropProgress->Show(TRUE, FALSE);
}

void CUIProperties::ShowSliderProperty(CControlUI* pControl)
{
	ShowProgressProperty(pControl);

	ASSERT(pControl);
	CSliderUI* pSlider = static_cast<CSliderUI*>(pControl->GetInterface(_T("Slider")));
	ASSERT(pSlider);

	CMFCPropertyGridProperty* pPropSlider = m_wndPropList.FindItemByData(classSlider, FALSE);
	ASSERT(pPropSlider);

	int iIndex = 0;

	//thumbimage
	iIndex = tagThumbImage - tagSlider;
	pPropSlider->GetSubItem(iIndex)->SetValue((_variant_t)pSlider->GetThumbImage());
	pPropSlider->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pSlider->GetThumbImage());

	//thumbhotimage
	iIndex = tagThumbHotImage - tagSlider;
	pPropSlider->GetSubItem(iIndex)->SetValue((_variant_t)pSlider->GetThumbHotImage());
	pPropSlider->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pSlider->GetThumbHotImage());

	//thumbpushedimage
	iIndex = tagThumbPushedImage - tagSlider;
	pPropSlider->GetSubItem(iIndex)->SetValue((_variant_t)pSlider->GetThumbPushedImage());
	pPropSlider->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pSlider->GetThumbPushedImage());

	//thumbsize
	iIndex = tagThumbSize - tagSlider;
	CMFCPropertyGridProperty* pValueList = pPropSlider->GetSubItem(iIndex);
	RECT rect = pSlider->GetThumbRect();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)(rect.right - rect.left));
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)(rect.bottom - rect.top));
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)(rect.right - rect.left));
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)(rect.bottom - rect.top));

	pPropSlider->Show(TRUE, FALSE);
}

void CUIProperties::ShowComboProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);
	ShowItemProperty(pControl);

	ASSERT(pControl);
	CComboUI* pCombo = static_cast<CComboUI*>(pControl->GetInterface(_T("Combo")));
	ASSERT(pCombo);

	CMFCPropertyGridProperty* pPropCombo = m_wndPropList.FindItemByData(classCombo, FALSE);
	ASSERT(pPropCombo);

	int iIndex = 0;

	//normalimage
	iIndex = tagComboNormalImage - tagCombo;
	pPropCombo->GetSubItem(iIndex)->SetValue((_variant_t)pCombo->GetNormalImage());
	pPropCombo->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pCombo->GetNormalImage());

	//hotimage
	iIndex = tagComboHotImage - tagCombo;
	pPropCombo->GetSubItem(iIndex)->SetValue((_variant_t)pCombo->GetHotImage());
	pPropCombo->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pCombo->GetHotImage());

	//pushedimage
	iIndex = tagComboPushedImage - tagCombo;
	pPropCombo->GetSubItem(iIndex)->SetValue((_variant_t)pCombo->GetPushedImage());
	pPropCombo->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pCombo->GetPushedImage());

	//focusedimage
	iIndex = tagComboFocusedImage - tagCombo;
	pPropCombo->GetSubItem(iIndex)->SetValue((_variant_t)pCombo->GetFocusedImage());
	pPropCombo->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pCombo->GetFocusedImage());

	//disabledimage
	iIndex = tagComboDisabledImage - tagCombo;
	pPropCombo->GetSubItem(iIndex)->SetValue((_variant_t)pCombo->GetDisabledImage());
	pPropCombo->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pCombo->GetDisabledImage());

	//textpadding
	RECT rect = pCombo->GetTextPadding();
	iIndex = tagComboTextPadding - tagCombo;
	CMFCPropertyGridProperty* pValueList = pPropCombo->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//dropboxsize
	SIZE size = pCombo->GetDropBoxSize();
	iIndex = tagComboDropBoxSize - tagCombo;
	pValueList = pPropCombo->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);
	pPropCombo->Show(TRUE, FALSE);
}

void CUIProperties::ShowActiveXProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CActiveXUI* pActiveX = static_cast<CActiveXUI*>(pControl->GetInterface(_T("ActiveX")));
	ASSERT(pActiveX);

	CMFCPropertyGridProperty* pPropActiveX = m_wndPropList.FindItemByData(classActiveX, FALSE);
	ASSERT(pPropActiveX);

	int iIndex = 0;

	CLSID clsid = pActiveX->GetClisd();
	TCHAR strCLSID[48];
	StringFromGUID2(clsid, strCLSID, 48);

	//clsid
	iIndex = tagClsid - tagActiveX;
	pPropActiveX->GetSubItem(iIndex)->SetValue((_variant_t)strCLSID);
	pPropActiveX->GetSubItem(iIndex)->SetOriginalValue((_variant_t)strCLSID);

	//delaycreate
	iIndex = tagDelayCreate - tagActiveX;
	pPropActiveX->GetSubItem(iIndex)->SetValue((_variant_t)pActiveX->IsDelayCreate());
	pPropActiveX->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pActiveX->IsDelayCreate());

	//modulename
	iIndex = tagModuleName - tagActiveX;
	pPropActiveX->GetSubItem(iIndex)->SetValue((_variant_t)pActiveX->GetModuleName());
	pPropActiveX->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pActiveX->GetModuleName());

	pPropActiveX->Show(TRUE, FALSE);
}

void CUIProperties::ShowContainerProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CContainerUI* pContainer = static_cast<CContainerUI*>(pControl->GetInterface(_T("Container")));
	ASSERT(pContainer);

	CMFCPropertyGridProperty* pPropContainer = m_wndPropList.FindItemByData(classContainer, FALSE);
	ASSERT(pPropContainer);

	int iIndex = 0;

	//inset
	RECT rect = pContainer->GetInset();
	iIndex = tagInset - tagContainer;
	CMFCPropertyGridProperty* pValueList = pPropContainer->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//childpadding
	iIndex = tagChildPadding - tagContainer;
	pPropContainer->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pContainer->GetChildPadding());
	pPropContainer->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pContainer->GetChildPadding());

	//mousechild
	iIndex = tagMouseChild - tagContainer;
	pPropContainer->GetSubItem(iIndex)->SetValue((_variant_t)pContainer->IsMouseChildEnabled());
	pPropContainer->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pContainer->IsMouseChildEnabled());

	//hscrollbar
	iIndex = tagHScrollBar - tagContainer;
	pPropContainer->GetSubItem(iIndex)->SetValue((_variant_t)(pContainer->GetHorizontalScrollBar() == NULL ? false : true));
	pPropContainer->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(pContainer->GetHorizontalScrollBar() == NULL ? false : true));

	//vscrollbar
	iIndex = tagMouseChild - tagContainer;
	pPropContainer->GetSubItem(iIndex)->SetValue((_variant_t)(pContainer->GetVerticalScrollBar() == NULL ? false : true));
	pPropContainer->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(pContainer->GetVerticalScrollBar() == NULL ? false : true));

	pPropContainer->Show(TRUE, FALSE);
}

void CUIProperties::ShowHorizontalLayoutProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CHorizontalLayoutUI* pHorizontalLayout = static_cast<CHorizontalLayoutUI*>(pControl->GetInterface(_T("HorizontalLayout")));
	ASSERT(pHorizontalLayout);

	CMFCPropertyGridProperty* pPropHorizontalLayout = m_wndPropList.FindItemByData(classHorizontalLayout, FALSE);
	ASSERT(pPropHorizontalLayout);

	int iIndex = 0;

	//sepwidth
	iIndex = tagSepWidth - tagHorizontalLayout;
	pPropHorizontalLayout->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pHorizontalLayout->GetSepWidth());
	pPropHorizontalLayout->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pHorizontalLayout->GetSepWidth());

	//sepimm
	iIndex = tagSepImm - tagHorizontalLayout;
	pPropHorizontalLayout->GetSubItem(iIndex)->SetValue((_variant_t)pHorizontalLayout->IsSepImmMode());
	pPropHorizontalLayout->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pHorizontalLayout->IsSepImmMode());

	pPropHorizontalLayout->Show(TRUE, FALSE);
}

void CUIProperties::ShowTileLayoutProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CTileLayoutUI* pTileLayout = static_cast<CTileLayoutUI*>(pControl->GetInterface(_T("TileLayout")));
	ASSERT(pTileLayout);

	CMFCPropertyGridProperty* pPropTileLayout = m_wndPropList.FindItemByData(classTileLayout, FALSE);
	ASSERT(pPropTileLayout);

	int iIndex = 0;

	//sepwidth
	iIndex = tagSepWidth - tagHorizontalLayout;
	pPropTileLayout->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pTileLayout->GetColumns());
	pPropTileLayout->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pTileLayout->GetColumns());

	pPropTileLayout->Show(TRUE, FALSE);
}

CMFCPropertyGridProperty* CUIProperties::FindPropByData(DWORD_PTR dwData, BOOL bSearchSubProps/* = TRUE*/) const
{
	return m_wndPropList.FindItemByData(dwData, bSearchSubProps);
}

CString CUIProperties::FormatOrigProperty(CMFCPropertyGridProperty* pProp)
{
	ASSERT(pProp);

	COleVariant var = pProp->GetOriginalValue();
	CString strOrigVal;

	int nCount = pProp->GetSubItemsCount();
	if (nCount > 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			CMFCPropertyGridProperty* pSubProp = pProp->GetSubItem(i);
			ASSERT_VALID(pSubProp);

			strOrigVal += FormatOrigProperty(pSubProp);

			if (i != nCount - 1)
			{
				strOrigVal += _T(',');
				strOrigVal += _T(' ');
			}
		}

		return strOrigVal;
	}

	switch (var.vt)
	{
		case VT_BSTR:
			strOrigVal = var.bstrVal;
			break;

		case VT_I2:
			strOrigVal.Format(_T("%d"), (short)var.iVal);
			break;

		case VT_I4:
		case VT_INT:
			strOrigVal.Format(_T("%ld"), (long)var.lVal);
			break;

		case VT_UI1:
			if ((BYTE)var.bVal != 0)
			{
				strOrigVal.Format(_T("%c"), (TCHAR)(BYTE)var.bVal);
			}
			break;

		case VT_UI2:
			strOrigVal.Format(_T("%u"), var.uiVal);
			break;

		case VT_UINT:
		case VT_UI4:
			strOrigVal.Format(_T("%u"), var.ulVal);
			break;

		case VT_R4:
			strOrigVal.Format(_T("%f"), (float)var.fltVal);
			break;

		case VT_R8:
			strOrigVal.Format(_T("%lf"), (double)var.dblVal);
			break;

		case VT_BOOL:
			strOrigVal = var.boolVal == VARIANT_TRUE ? _T("True") : _T("False");
			break;

		default:
			// Unsupported type
			strOrigVal = _T("*** error ***");
	}

	return strOrigVal;
}

void CUIProperties::ShowListProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);
	ShowItemProperty(pControl);

	ASSERT(pControl);
	CListUI* pList = static_cast<CListUI*>(pControl->GetInterface(_T("List")));
	ASSERT(pList);

	CMFCPropertyGridProperty* pPropList = m_wndPropList.FindItemByData(classList, FALSE);
	ASSERT(pPropList);

	int iIndex = 0;

	//header
	iIndex = tagListHeader - tagList;
	pPropList->GetSubItem(iIndex)->SetValue((_variant_t)pList->GetHeader()->IsVisible());
	pPropList->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pList->GetHeader()->IsVisible());

	//headerBKImage
	iIndex = tagListHeaderBKImage - tagList;
	pPropList->GetSubItem(iIndex)->SetValue((_variant_t)pList->GetHeader()->GetBkImage());
	pPropList->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pList->GetHeader()->GetBkImage());

	pPropList->Show(TRUE, FALSE);
}

void CUIProperties::ShowItemProperty(CControlUI* pControl)
{
	ASSERT(pControl);
	IListOwnerUI* pList = static_cast<IListOwnerUI*>(pControl->GetInterface(_T("IListOwner")));
	ASSERT(pList);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classItem, FALSE);
	ASSERT(pPropItem);

	DWORD iIndex = 0;
	DWORD iColor = 0;
	DWORD iOrignalColor = 0;
	DWORD iFont = -1;
	TListInfoUI* pListInfo = pList->GetListInfo();

	//itembkimage
	iIndex = tagItemBkImage - tagItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListInfo->sBkImage);
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListInfo->sBkImage);

	//itemhotimage
	iIndex = tagItemHotImage - tagItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListInfo->sHotImage);
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListInfo->sHotImage);

	//itemselectedimage
	iIndex = tagItemSelectedImage - tagItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListInfo->sSelectedImage);
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListInfo->sSelectedImage);

	//itemdisabledimage
	iIndex = tagItemDisabledImage - tagItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListInfo->sDisabledImage);
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListInfo->sDisabledImage);

	//itemtextpadding
	iIndex = tagItemTextPadding - tagItem;
	CMFCPropertyGridProperty*  pValueList = pPropItem->GetSubItem(iIndex);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.bottom);

	//itemalign
	CString strStyle = _T("Left");
	if (pListInfo->uTextStyle == DT_LEFT)
		strStyle = _T("Left");
	else if (pListInfo->uTextStyle&DT_CENTER)
		strStyle = _T("Center");
	else if (pListInfo->uTextStyle&DT_RIGHT)
		strStyle = _T("Right");
	iIndex = tagItemAlign - tagItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)strStyle);
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)strStyle);

	//itemtextcolor
	iIndex = tagItemTextColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwTextColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itembkcolor
	iIndex = tagItemBkColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwBkColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemselectedtextcolor
	iIndex = tagItemSelectedTextColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwSelectedTextColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);
	
	//itemselectedbkcolor
	iIndex = tagItemSelectedBkColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwSelectedBkColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemhottextcolor
	iIndex = tagItemHotTextColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwHotTextColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemhotbkcolor
	iIndex = tagItemHotBkColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwHotBkColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemdisabledtextcolor
	iIndex = tagItemDisabledTextColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwDisabledTextColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemdisabledbkcolor
	iIndex = tagItemDisabledBkColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwDisabledBkColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemlinecolor
	iIndex = tagItemLineColor - tagItem;
	iColor = TransColorToARGB(pListInfo->dwLineColor);
	iOrignalColor = iColor;
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetColor((_variant_t)(LONG)iColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(iIndex))->SetOriginalValue((_variant_t)(LONG)iOrignalColor);

	//itemfont
	iFont = pListInfo->nFont;
	if (iFont != -1)
	{
		iIndex = tagItemFont - tagItem;
		pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)iFont);
		pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)iFont);
	}

	//itemshowhtml
	iIndex = tagMultiExpanding - tagItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListInfo->bMultiExpandable);
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListInfo->bMultiExpandable);

	pPropItem->Show(TRUE, FALSE);
}

void CUIProperties::ShowScrollBarProperty(CControlUI* pControl)
{
	//�󲿷�ʱ��ֻ��Ĭ������������ȫ�ֹ���������
	//Ĭ�ϲ���ʾ�����ؼ�����
	//ShowControlProperty(pControl);

	ASSERT(pControl);
	CScrollBarUI* pScrollBar = static_cast<CScrollBarUI*>(pControl->GetInterface(_T("ScrollBar")));
	ASSERT(pScrollBar);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classScrollBar, FALSE);
	ASSERT(pPropItem);

	int iIndex = 0;

	//tagScrollBarRange
	iIndex = tagScrollBarRange - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pScrollBar->GetScrollRange());

	//tagScrollBarValue
	iIndex = tagScrollBarValue - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pScrollBar->GetScrollRange());

	//tagScrollBarLineSize
	iIndex = tagScrollBarLineSize - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pScrollBar->GetScrollRange());

	// tagScrollBarShowButton1
	iIndex = tagScrollBarShowButton1 - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetShowButton1());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetShowButton1());

	//	tagScrollBarShowButton2
	iIndex = tagScrollBarShowButton2 - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetShowButton2());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetShowButton2());

	//	tagScrollBarButton1NormalImage
	iIndex = tagScrollBarButton1NormalImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton1NormalImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton1NormalImage());

	//	tagScrollBarButton1HotImage
	iIndex = tagScrollBarButton1HotImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton1HotImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton1HotImage());

	//	tagScrollBarButton1PushedImage
	iIndex = tagScrollBarButton1PushedImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton1PushedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton1PushedImage());

	//	tagScrollBarButton1DisabledImage
	iIndex = tagScrollBarButton1DisabledImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton1DisabledImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton1DisabledImage());

	//	tagScrollBarButton2NormalImage
	iIndex = tagScrollBarButton2NormalImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton2NormalImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton2NormalImage());

	//	tagScrollBarButton2HotImage
	iIndex = tagScrollBarButton2HotImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton2HotImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton2HotImage());

	//	tagScrollBarButton2PushedImage
	iIndex = tagScrollBarButton2PushedImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton2PushedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton2PushedImage());

	//	tagScrollBarButton2DisabledImage
	iIndex = tagScrollBarButton2DisabledImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetButton2DisabledImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton2DisabledImage());

	//	tagScrollBarThumbNormalImage
	iIndex = tagScrollBarThumbNormalImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetThumbNormalImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetThumbNormalImage());

	//	tagScrollBarThumbHotImage
	iIndex = tagScrollBarThumbHotImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetThumbHotImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetThumbHotImage());

	//	tagScrollBarThumbPushedImage
	iIndex = tagScrollBarThumbPushedImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetThumbPushedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetThumbPushedImage());

	//	tagScrollBarThumbDisabledImage
	iIndex = tagScrollBarThumbDisabledImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetThumbDisabledImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetThumbDisabledImage());

	//	tagScrollBarRailNormalImage
	iIndex = tagScrollBarRailNormalImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetRailNormalImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetRailNormalImage());

	//	tagScrollBarRailHotImage
	iIndex = tagScrollBarRailHotImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetRailHotImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetButton1HotImage());

	//	tagScrollBarRailPushedImage
	iIndex = tagScrollBarRailPushedImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetRailPushedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetRailPushedImage());

	//	tagScrollBarRailDisabledImage
	iIndex = tagScrollBarRailDisabledImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetRailDisabledImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetRailDisabledImage());

	//	tagScrollBarBKNormalImage
	iIndex = tagScrollBarBKNormalImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetBkNormalImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetBkNormalImage());

	//	tagScrollBarBKHotImage
	iIndex = tagScrollBarBKHotImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetBkHotImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetBkHotImage());

	//	tagScrollBarBKPushedImage
	iIndex = tagScrollBarBKPushedImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetBkPushedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetBkPushedImage());

	//	tagScrollBarBKDisabledImage
	iIndex = tagScrollBarBKDisabledImage - tagScrollBar;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pScrollBar->GetBkDisabledImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pScrollBar->GetBkDisabledImage());

	pPropItem->Show(TRUE, FALSE);
}
void CUIProperties::ShowTabLayoutPropery(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(pControl->GetInterface(_T("TabLayout")));
	ASSERT(pTabLayout);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classTabLayout, FALSE);
	ASSERT(pPropItem);

	int iIndex = 0;

	//	SelectedID
	iIndex = tagSelectedID - tagTabLayout;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pTabLayout->GetCurSel());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pTabLayout->GetCurSel());

	pPropItem->Show(TRUE, FALSE);
}

void CUIProperties::ShowListHeaderItemPropery(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CListHeaderItemUI* pListHeaderItem = static_cast<CListHeaderItemUI*>(pControl->GetInterface(_T("ListHeaderItem")));
	ASSERT(pListHeaderItem);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classListHeaderItem, FALSE);
	ASSERT(pPropItem);

	int iIndex = 0;

	//	dragable
	iIndex = tagDragable - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListHeaderItem->IsDragable());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListHeaderItem->IsDragable());

	//	sepwidth
	iIndex = tagListHeaderItemSepWidth - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)(LONG)pListHeaderItem->GetSepWidth());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)(LONG)pListHeaderItem->GetSepWidth());

	//	normalimage
	iIndex = tagListHeaderItemNormalImage - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListHeaderItem->GetNormalImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListHeaderItem->GetNormalImage());

	//	hotimage
	iIndex = tagListHeaderItemHotImage - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListHeaderItem->GetHotImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListHeaderItem->GetHotImage());

	//	pushedimage
	iIndex = tagListHeaderItemPushedImage - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListHeaderItem->GetPushedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListHeaderItem->GetPushedImage());

	//	focusedimage
	iIndex = tagListHeaderItemFocusedImage - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListHeaderItem->GetFocusedImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListHeaderItem->GetFocusedImage());

	//	sepimage
	iIndex = tagSepImage - classListHeaderItem;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pListHeaderItem->GetSepImage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pListHeaderItem->GetSepImage());

	pPropItem->Show(TRUE, FALSE);
}

void CUIProperties::ShowWebBrowserPropery(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CWebBrowserUI* pWebBrowser = static_cast<CWebBrowserUI*>(pControl->GetInterface(_T("WebBrowser")));
	ASSERT(pWebBrowser);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classWebBrowser, FALSE);
	ASSERT(pPropItem);

	int iIndex = 0;

	//	homepage
	iIndex = tagWebBrowserHomePage - tagWebBrowser;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pWebBrowser->GetHomePage());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pWebBrowser->GetHomePage());

	//	autonavi
	iIndex = tagWebBrowserAutoNavi - tagWebBrowser;
	pPropItem->GetSubItem(iIndex)->SetValue((_variant_t)pWebBrowser->IsAutoNavigation());
	pPropItem->GetSubItem(iIndex)->SetOriginalValue((_variant_t)pWebBrowser->IsAutoNavigation());

	pPropItem->Show(TRUE, FALSE);
}
