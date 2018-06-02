#include "stdafx.h"
#include "mall.h"

extern GameUtil* g_GameUtil;
extern GameResource* R;
extern CStaticLogger g_Logger;

// ------------------------------------------------------------------------------------------------------------------------------------
// MallFrame - �̳ǽ���

MallFrame* MallFrame::g_instance = nullptr;

MallFrame::MallFrame()
	: m_pTabLayout(nullptr)
	, m_pLabPage(nullptr)
	, m_pBtnPageLeft(nullptr)
	, m_pBtnPageRight(nullptr)
	, m_pCtnMallPanel(nullptr)
	, m_pCtnBagPanel(nullptr)
{
	memset(m_pzBuffer,			0,	ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	memset(&m_tGoodsTileDesc,	0,	sizeof(tGoodsTileDesc));
	memset(&m_tBagTileDesc,		0,	sizeof(tItemsTileDesc));
}

MallFrame::~MallFrame()
{
	// ���HallFrame�¼�����
	HallFrame* pHallFrame = HallFrame::GetInstance();
	CPaintManagerUI* pHallPaintManager = pHallFrame->GetPaintManager();
	g_GameUtil->SetFrozenEnable(pHallPaintManager, true);
	g_GameUtil->ShowTopWindow(pHallPaintManager->GetPaintWindow());

	g_Logger->TryInfo(_T("MallFrame - Destroy"));
}

MallFrame* MallFrame::GetInstance()
{
	if (MallFrame::g_instance == NULL)
		MallFrame::g_instance = new (std::nothrow)MallFrame();

	return MallFrame::g_instance;
}

BOOL MallFrame::CreateFrame()
{
	// ����Ѵ��ڹ���������,��ر�
	if (MallBuyFrame::IsInstanceExist())
	{
		MallBuyFrame* pBuyFrame = MallBuyFrame::GetInstance();
		pBuyFrame->Close();
	}

	// �����̳����
	if (FALSE == IsWindow(m_hWnd))
	{
		HallFrame* pHallFrame = HallFrame::GetInstance();
		CPaintManagerUI* pManager = pHallFrame->GetPaintManager();
		HWND hParentWnd = pManager->GetPaintWindow();
		g_GameUtil->SetFrozenEnable(pManager, false);
		m_hWnd = this->Create(hParentWnd, NULL, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}

	return TRUE;
}

UILIB_RESOURCETYPE MallFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR MallFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString MallFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString MallFrame::GetSkinFile()
{
	return _T("hall\\mall\\mall.xml");
}

void MallFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("MallFrame - Close"));
	__super::OnFinalMessage(hWnd);
	SAFE_DELETE(MallFrame::g_instance);
};

void MallFrame::InitWindow()
{
	g_Logger->TryInfo(_T("MallFrame - Open"));

	m_pTabLayout	= static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(R->kTabMall_TabLayout));
	m_pCtnMallPanel = static_cast<CContainerUI*>(m_PaintManager.FindControl(R->kCtnMall_MallPanel));
	m_pCtnBagPanel	= static_cast<CContainerUI*>(m_PaintManager.FindControl(R->kCtnMall_BagPanel));
	m_pLabPage		= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabMall_Page));
	m_pBtnPageLeft	= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnMall_LastPage));
	m_pBtnPageRight = static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnMall_NextPage));
}

void MallFrame::AddGoodsItem(tagPropsInfo* pProps)
{
	CVerticalLayoutUI* vLayout = new CVerticalLayoutUI();
	vLayout->SetFixedWidth(m_tGoodsTileDesc.tileSize.cx);
	vLayout->SetFixedHeight(m_tGoodsTileDesc.tileSize.cy);
	m_tGoodsTileDesc.pLayout->Add(vLayout);
	
	// ��Ʒ�ؼ�
	CBagItemUI* pBagItem = new CBagItemUI();
	CDuiString pzBkImg = _T("hall\\mall\\item.png");
	CDuiString pzHotImg = _T("file='common\\button\\btn_hot.png' corner='6,6,6,6'");
	CDuiString pzPushImg = _T("file='common\\button\\btn_push.png' corner='6,6,6,6'");
	pBagItem->SetBackgroundImg(pzBkImg);
	pBagItem->SetHotImage(pzHotImg);
	pBagItem->SetPushedImage(pzPushImg);
	pBagItem->SetFixedHeight(m_tGoodsTileDesc.iconSize.cx);
	pBagItem->SetFixedWidth(m_tGoodsTileDesc.iconSize.cy);
	pBagItem->SetPadding(m_tGoodsTileDesc.rcPadding);
	vLayout->Add(pBagItem);
	
	// �ı��ؼ�
	RECT rcTextPadding = { m_tGoodsTileDesc.rcPadding.left, 4, 0, 0 };
	CTextUI* pTxt = new CTextUI();
	pTxt->SetText(_T(""));
	pTxt->SetFont(0);
	pTxt->SetFixedHeight(m_tGoodsTileDesc.uTxtHeight);
	pTxt->SetTextColor(0xFF007CFF);
	pTxt->SetEndEllipsis(true);
	pTxt->SetTextPadding(rcTextPadding);
	vLayout->Add(pTxt);

	if (pProps)
	{
		pBagItem->SetTag(pProps->wPropsID);
		pBagItem->SetName(R->kBtnMall_Goods);
		pBagItem->SetGoodsData(pProps);

		_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), pProps->wPropsID);
		pBagItem->SetBkImage(m_pzBuffer);

		CDuiString pzFormat;
		LONGLONG lRange1 = pow(10, 4);
		LONGLONG lRange2 = pow(10, 7);
		LONGLONG lRange3 = pow(10, 8);
		LONGLONG lRange4 = pow(10, 12);
		if (pProps->lPrice >= lRange1 && pProps->lPrice < lRange2)
		{
			DOUBLE dfPrice = pProps->lPrice / (DOUBLE)lRange1;
			if (0 == pProps->lPrice % lRange1)
			{
				pzFormat = _T("%.0f��");
			}
			else
			{
				pzFormat = _T("%.01f��");
			}
			_stprintf_s(m_pzBuffer, pzFormat.GetData(), dfPrice);
		}
		else if (pProps->lPrice >= lRange2 && pProps->lPrice < lRange3)
		{
			DOUBLE dfPrice = pProps->lPrice / (DOUBLE)lRange2;
			if (0 == pProps->lPrice % lRange2)
			{
				pzFormat = _T("%.0fǧ��");
			}
			else
			{
				pzFormat = _T("%.01fǧ��");
			}
			_stprintf_s(m_pzBuffer, pzFormat.GetData(), dfPrice);
		}
		else if (pProps->lPrice >= lRange3 && pProps->lPrice < lRange4)
		{
			DOUBLE dfPrice = pProps->lPrice / (DOUBLE)lRange3;
			if (0 == pProps->lPrice % lRange3)
			{
				pzFormat = _T("%.0f��");
			}
			else
			{
				pzFormat = _T("%.01f��");
			}
			_stprintf_s(m_pzBuffer, pzFormat.GetData(), dfPrice);
		}
		else if (pProps->lPrice >= lRange4)
		{
			_stprintf_s(m_pzBuffer, _T("%s"), _T("���̫��"));
		}
		else
		{
			_stprintf_s(m_pzBuffer, _T("%ld"), pProps->lPrice);
		}

		CDuiString pzPrice = m_pzBuffer;
		_stprintf_s(m_pzBuffer, _T("%s\n��ң�%s"), pProps->szName, pzPrice.GetData());
		pTxt->SetText(m_pzBuffer);
	}
}

void MallFrame::AddBagItem(tagPropsInfo* pProps)
{
	CBagBoxUI* pBagBox = new CBagBoxUI();
	CDuiString pzBkgImg = _T("hall\\mall\\item.png");
	CDuiString pzHotImg = _T("file='common\\button\\btn_hot.png' corner='6,6,6,6'");
	CDuiString pzPushImg = _T("file='common\\button\\btn_push.png' corner='6,6,6,6'");
	pBagBox->SetBackgroundImg(pzBkgImg);
	pBagBox->SetHotImage(pzHotImg);
	pBagBox->SetPushedImage(pzPushImg);
	pBagBox->SetFixedHeight(m_tBagTileDesc.tileSize.cx);
	pBagBox->SetFixedWidth(m_tBagTileDesc.tileSize.cy);
	pBagBox->SetPadding(m_tBagTileDesc.rcPadding);
	m_tBagTileDesc.pLayout->Add(pBagBox);

	if (pProps)
	{
		pBagBox->SetTag(pProps->wPropsID);
		pBagBox->SetGoodsData(pProps);
		_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), pProps->wPropsID);
		pBagBox->SetBkImage(m_pzBuffer);
	}
}

void MallFrame::UpdatePages(int iCurSel)
{
	tItemsTileDesc* pTarget = nullptr;
	switch (iCurSel)
	{
		case 0:	pTarget = &m_tGoodsTileDesc;	break;
		case 1:	pTarget = &m_tBagTileDesc;		break;
		default:								break;
	}

	if (pTarget)
	{
		ServiceData* pServerData = ServiceData::GetInstance();
		int nCount = pTarget->icol * pTarget->irol;

		pTarget->uCurPage = 1;
		pTarget->uMinPage = 1;
		pTarget->uMaxPage = ceil(pServerData->GetGamePropsCount() / (float)nCount);
		_stprintf_s(m_pzBuffer, _T("%u/%u"), pTarget->uCurPage, pTarget->uMaxPage);
		m_pLabPage->SetText(m_pzBuffer);
		m_pBtnPageLeft->SetEnabled(false);
		m_pBtnPageRight->SetEnabled(true);
		if (pServerData->GetGamePropsCount() <= nCount)
		{
			m_pBtnPageRight->SetEnabled(false);
		}
	}
}

void MallFrame::GoodsTurnPage(tItemsTileDesc& tTarget, bool bNext)
{
	if (tTarget.uCurPage < tTarget.uMinPage || tTarget.uCurPage > tTarget.uMaxPage)
	{
		return;
	}

	tTarget.pLayout->RemoveAll();
	ServiceData* pServerData = ServiceData::GetInstance();
	int nCount = tTarget.icol * tTarget.irol;
	DWORD iNextKey = 0;
	if (bNext)
	{
		m_pBtnPageLeft->SetEnabled(true);
		iNextKey = tTarget.uCurPage++ * nCount;
		if (tTarget.uCurPage >= tTarget.uMaxPage)
		{
			m_pBtnPageRight->SetEnabled(false);
		}
	}
	else
	{
		
		iNextKey = tTarget.uCurPage-- * nCount;
		iNextKey -= nCount;
		iNextKey -= nCount;

		m_pBtnPageRight->SetEnabled(true);
		if (tTarget.uCurPage <= tTarget.uMinPage)
		{
			m_pBtnPageLeft->SetEnabled(false);
		}
	}

	for (int i = 0; i < nCount; ++i)
	{
		tagPropsInfo* pProps = nullptr;
		if (i < pServerData->GetGamePropsCount())
		{
			pProps = pServerData->EnumGameProps(iNextKey);
		}

		this->AddGoodsItem(pProps);
	}
	_stprintf_s(m_pzBuffer, _T("%u/%u"), tTarget.uCurPage, tTarget.uMaxPage);
	m_pLabPage->SetText(m_pzBuffer);
}

void MallFrame::BagsTurnPage(tItemsTileDesc& tTarget, bool bNext)
{

}

void MallFrame::InitGoods()
{
	// Init
	m_tGoodsTileDesc.icol = 5;														// ��
	m_tGoodsTileDesc.irol = 3;														// ��
	m_tGoodsTileDesc.iw = m_pCtnMallPanel->GetParent()->GetWidth();					// �ܿ�
	m_tGoodsTileDesc.ih = m_pCtnMallPanel->GetParent()->GetHeight();				// �ܸ�
	m_tGoodsTileDesc.uTxtHeight = 35;												// �����ı��߶�
	m_tGoodsTileDesc.iconSize = { 65, 65 };											// icon��С
	m_tGoodsTileDesc.tileSize.cx = m_tGoodsTileDesc.iw / m_tGoodsTileDesc.icol;
	m_tGoodsTileDesc.tileSize.cy = m_tGoodsTileDesc.ih / m_tGoodsTileDesc.irol;

	m_tGoodsTileDesc.rcPadding.left = (m_tGoodsTileDesc.tileSize.cx - m_tGoodsTileDesc.iconSize.cx) / 2;
	m_tGoodsTileDesc.rcPadding.right = (m_tGoodsTileDesc.tileSize.cx - m_tGoodsTileDesc.iconSize.cy) / 2;
	m_tGoodsTileDesc.rcPadding.top = (m_tGoodsTileDesc.ih - m_tGoodsTileDesc.irol * m_tGoodsTileDesc.iconSize.cy) / m_tGoodsTileDesc.irol - m_tGoodsTileDesc.uTxtHeight;
	m_tGoodsTileDesc.rcPadding.bottom = 0;
	m_tGoodsTileDesc.pLayout = new CBagTileLayout();

	// Add
	m_tGoodsTileDesc.pLayout->SetItemSize(m_tGoodsTileDesc.tileSize);
	m_pCtnMallPanel->Add(m_tGoodsTileDesc.pLayout);
	
	// Add Tile
	ServiceData* pServerData = ServiceData::GetInstance();
	int nCount = m_tGoodsTileDesc.icol * m_tGoodsTileDesc.irol;
	DWORD iNextKey = 0;
	for (int i = 0; i < nCount; ++i)
	{
		tagPropsInfo* pProps = nullptr;
		if (i < pServerData->GetGamePropsCount())
		{
			pProps = pServerData->EnumGameProps(iNextKey);
		}

		this->AddGoodsItem(pProps);
	}
}

void MallFrame::InitBags()
{
	// Init Struct
	m_tBagTileDesc.icol = 7;										// ��
	m_tBagTileDesc.irol = 5;										// ��
	m_tBagTileDesc.iw = m_pCtnBagPanel->GetParent()->GetWidth();	// �ܿ�
	m_tBagTileDesc.ih = m_pCtnBagPanel->GetParent()->GetHeight();	// �ܸ�
	m_tBagTileDesc.tileSize = { 65, 65 };							// ���Ӵ�С
	m_tBagTileDesc.rcPadding = { 0, 0, 0, 0 };						// ���Ӽ��

	LONG lTop = (m_tBagTileDesc.ih - m_tBagTileDesc.irol * m_tBagTileDesc.tileSize.cy) / m_tBagTileDesc.irol;
	m_tBagTileDesc.rcPadding.top = lTop;
	m_tBagTileDesc.pLayout = new CBagTileLayout();

	// Add Layout
	m_tBagTileDesc.pLayout->SetItemSize(m_tBagTileDesc.tileSize);
	m_tBagTileDesc.pLayout->SetColumns(m_tBagTileDesc.icol);
	m_pCtnBagPanel->Add(m_tBagTileDesc.pLayout);

	// Add Tile
	int nCount = m_tBagTileDesc.icol * m_tBagTileDesc.irol;
	ServiceData* pServerData = ServiceData::GetInstance();
	DWORD iNextKey = 0;
	for (int i = 0; i < nCount; ++i)
	{
		tagPropsInfo* pProps = nullptr;
		if (i < pServerData->GetGamePropsCount())
		{
			pProps = pServerData->EnumGameProps(iNextKey);
		}

		this->AddBagItem(pProps);
	}
}

void MallFrame::InitData(TNotifyUI& msg)
{
	this->InitGoods();
	//this->InitBags();

	// ���õ�ǰ����±�Ϊ0
	// ��Ʒ���
	int iCurSel = 0;
	m_pTabLayout->SelectItem(iCurSel);
	this->UpdatePages(iCurSel);

	g_Logger->TryInfo(_T("MallFrame - Open Complete"));
}

LRESULT MallFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_MENUCLICK:
		{
			CDuiString* strMenuName = (CDuiString*)(wParam);
			CControlUI* pControl = (CControlUI*)(lParam);
			this->OnMenuEvent(strMenuName, pControl);
			return 0;
		}break;

		default:
			break;
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void MallFrame::OnMenuEvent(CDuiString* strMenuName, CControlUI* pControl)
{
	if (!strMenuName) return;

	// ʹ��
	if (_tcscmp(*strMenuName, _T("usemenu-use")) == 0)
	{
		CBagTileLayout* pBagLayout = m_tBagTileDesc.pLayout;
		if (pBagLayout)
		{
			pBagLayout->Remove(pControl);
			this->AddBagItem(nullptr);
		}
	}
	// ����
	else if (_tcscmp(*strMenuName, _T("usemenu-give")) == 0)
	{
		CBagBoxUI* pBagBox = static_cast<CBagBoxUI*>(pControl);
		if (pBagBox)
		{
			MallGiveFrame* pGiveFrame = new MallGiveFrame();
			pGiveFrame->CreateFrame(&m_PaintManager, pBagBox->GetGoods());
		}
	}
}

void MallFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnMall_Close) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kOptMall_Mall) == 0)
		{
			int iCurSel = 0;
			m_pTabLayout->SelectItem(iCurSel);
			this->UpdatePages(iCurSel);
		}
		else if (_tcscmp(sName, R->kOptMall_Bag) == 0)
		{
			int iCurSel = 1;
			m_pTabLayout->SelectItem(iCurSel);
			this->UpdatePages(iCurSel);
		}
		else if (_tcscmp(sName, R->kBtnMall_Goods) == 0)
		{
			UINT uId = msg.pSender->GetTag();
			MallBuyFrame* pBuyFrame = MallBuyFrame::GetInstance();

			DWORD iNextKey = 0;
			tagPropsInfo* pProps = nullptr;
			ServiceData* pServerData = ServiceData::GetInstance();
			for (int i = 0; i < pServerData->GetGamePropsCount(); ++i)
			{
				tagPropsInfo* pTempGoods = pServerData->EnumGameProps(iNextKey);
				if (uId == pTempGoods->wPropsID)
				{
					pProps = pTempGoods;
					break;
				}
			}
			pBuyFrame->CreateFrame(&m_PaintManager, pProps);
		}
		else if (_tcscmp(sName, R->kBtnMall_LastPage) == 0)
		{
			int iCurSel = m_pTabLayout->GetCurSel();
			switch (iCurSel)
			{
				case 0:	this->GoodsTurnPage(m_tGoodsTileDesc, false);	break;
				case 1:	this->BagsTurnPage(m_tBagTileDesc, false);		break;
				default:												break;
			}
		}
		else if (_tcscmp(sName, R->kBtnMall_NextPage) == 0)
		{
			int iCurSel = m_pTabLayout->GetCurSel();
			switch (iCurSel)
			{
				case 0:	this->GoodsTurnPage(m_tGoodsTileDesc, true);	break;
				case 1:	this->BagsTurnPage(m_tBagTileDesc, true);		break;
				default:												break;
			}
		}
	}

	//__super::Notify(msg);
}

LRESULT MallFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

// ------------------------------------------------------------------------------------------------------------------------------------
// MallBuyFrame - �������

MallBuyFrame* MallBuyFrame::g_instance = nullptr;
MallBuyFrame::MallBuyFrame()
	: m_pParentManager(nullptr)
	, m_lMinBuyCount(0)
	, m_lMaxBuyCount(0)
	, m_dwRevNameTxtColor(0)
	, m_pNumberConvert(nullptr)
	, m_pBtnEnsureBuy(nullptr)
	, m_pBtnPageLeft(nullptr)
	, m_pBtnPageRight(nullptr)
	, m_pProps(nullptr)
	, m_pLabIcon(nullptr)
	, m_pTxtName(nullptr)
	, m_pTxtSrcCharm(nullptr)
	, m_pTxtRevCharm(nullptr)
	, m_pTxtPrice(nullptr)
	, m_pTxtCostCapital(nullptr)
	, m_pEdtCount(nullptr)
	, m_pEdtReceiveID(nullptr)
	, m_pEdtReceiveNickName(nullptr)
	, m_pLoginServer(nullptr)
	, m_bTickedBuyPw(false)
	, m_iCurBuyType(MallBuyType::MALL_BUY_TYPE_NONE)
	, m_iCurOperateStatus(MallOperateStatus::MALL_BUY_OS_NONE)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));

	m_pNumberConvert = new NumberConvert();

	// �Ƿ�ѡ��ס������ߵ�����
	ServiceData* pServerData = ServiceData::GetInstance();
	GameCacheData& tGameCacheData = pServerData->GetGameCacheData();
	m_bTickedBuyPw = tGameCacheData.bSaveBuyGoodsPw;

	// ��ʼ����¼����������
	m_pLoginServer = LoginServer::GetInstance();
	m_pLoginServer->SetDelegate(this, &m_PaintManager);
}

MallBuyFrame::~MallBuyFrame()
{
	//g_GameUtil->SetFrozenEnable(m_pParentManager, true);

	SAFE_DELETE(m_pNumberConvert);

	// �ͷŵ�¼����������
	if (LoginServer::IsInstanceExist())
	{
		m_pLoginServer->SetDelegate(nullptr, m_pParentManager);
		m_pLoginServer->CloseSocket();
	}
}

MallBuyFrame* MallBuyFrame::GetInstance()
{
	if (MallBuyFrame::g_instance == NULL)
		MallBuyFrame::g_instance = new (std::nothrow)MallBuyFrame();

	return MallBuyFrame::g_instance;
}

BOOL MallBuyFrame::CreateFrame(CPaintManagerUI* pParentManager, tagPropsInfo* pProps, MallBuyType buyType /* = MallBuyType::MALL_BUY_TYPE_HALL */)
{
	ASSERT(pParentManager);
	if (!pParentManager || !pProps) return FALSE;

	m_pProps = pProps;
	m_iCurBuyType = buyType;
	m_pParentManager = pParentManager;
	
	if (FALSE == IsWindow(m_hWnd))
	{
		HWND hParentWnd = m_pParentManager->GetPaintWindow();
		//g_GameUtil->SetFrozenEnable(m_pParentManager, true);
		m_hWnd = this->Create(hParentWnd, NULL, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->InitData();
		this->CenterWindow();
		this->ShowWindow(true);
	}

	return TRUE;
}

UILIB_RESOURCETYPE MallBuyFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR MallBuyFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString MallBuyFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString MallBuyFrame::GetSkinFile()
{
	return _T("hall\\mall\\buy.xml");
}

LRESULT MallBuyFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

void MallBuyFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	SAFE_DELETE(MallBuyFrame::g_instance);
}

void MallBuyFrame::InitWindow()
{
	m_pLabIcon				= static_cast<CLabelUI*>(m_PaintManager.FindControl(R->kLabMallBuy_Icon));
	m_pTxtName				= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMallBuy_Name));
	m_pTxtRevCharm			= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMallBuy_Charm));
	m_pTxtPrice				= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMallBuy_Price));
	m_pTxtCostCapital		= static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMallBuy_CostCapital));
	m_pEdtCount				= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMallBuy_Count));
	m_pEdtReceiveID			= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMallBuy_ReceiveID));
	m_pEdtReceiveNickName	= static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMallBuy_ReceiveNickName));
	m_pBtnPageLeft			= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnMallBuy_Sub));
	m_pBtnPageRight			= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnMallBuy_Add));
	m_pBtnEnsureBuy			= static_cast<CButtonUI*>(m_PaintManager.FindControl(R->kBtnMallBuy_Ensure));

	// ����Ĭ�Ͻ���ID
	m_pEdtReceiveID->SetText(_T(""));

	// ����Ĭ�ϵĽ����ǳ�
	m_pEdtReceiveNickName->SetReadOnly(true);
	m_pEdtReceiveNickName->SetReadOnlyShowCaret(true);
	m_pEdtReceiveNickName->SetLimitText(NAME_LEN);
	m_pEdtReceiveNickName->SetText(_T(""));
	m_dwRevNameTxtColor = m_pEdtReceiveNickName->GetTextColor();
}

void MallBuyFrame::InitData()
{
	// ���ö�Ӧ��Ʒͼ��
	_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), m_pProps->wPropsID);
	m_pLabIcon->SetBkImage(m_pzBuffer);

	// ���ö�Ӧ��Ʒ��
	m_pTxtName->SetText(m_pProps->szName);

	// ����Ĭ�Ϲ�������Ϊ1
	WORD wBuyCount = 1;
	_stprintf_s(m_pzBuffer, _T("%u"), wBuyCount);
	m_pEdtCount->SetText(m_pzBuffer);

	// ���ö�Ӧ��Ʒ�������ͷ�����������
	// ...

	// ���ö�Ӧ��Ʒ�������շ�����������
	LONG lRevCharm = wBuyCount * m_pProps->lRcvCharm;
	_stprintf_s(m_pzBuffer, _T("%ld��"), lRevCharm);
	m_pTxtRevCharm->SetText(m_pzBuffer);
	
	// ���ö�Ӧ��Ʒ�۸�
	LONGLONG lTotalPrice = wBuyCount * (LONGLONG)m_pProps->lPrice;
	_stprintf_s(m_pzBuffer, _T("%lld���"), lTotalPrice);
	m_pTxtPrice->SetText(m_pzBuffer);

	// �����ܻ��Ѵ�дֵ
	CString cstrCostCapital = m_pNumberConvert->NumToChineseStr(lTotalPrice);
	m_pTxtCostCapital->SetText(cstrCostCapital);
	m_pTxtCostCapital->SetToolTip(cstrCostCapital);

	// ���ù�����С����
	m_lMinBuyCount = 0;

	// ���ù����������(����涨�����λ��Ϊ: [1,8])
	int iMaxLimit = max(m_pEdtCount->GetLimitText(), 1);
	iMaxLimit = min(iMaxLimit, 8);
	m_pEdtCount->SetLimitText(iMaxLimit);
	m_lMaxBuyCount = pow(10, iMaxLimit);
	m_lMaxBuyCount -= 1;

	// �۽�������ID
	m_PaintManager.SetFocusNeeded(m_pEdtReceiveID);
}

LRESULT MallBuyFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_CHAR:
		case WM_IME_CHAR:
		{
			if (m_pEdtCount && m_pEdtCount->IsFocused())
			{
				if (m_pEdtCount->GetText().GetLength() == 0)
				{
					m_pEdtCount->SetText(_T("0"));
				}

				// ֻ��������
				if (wParam >= 48 && wParam <= 57)
				{
					CDuiString& pzText = m_pEdtCount->GetText();
					if (pzText.GetLength() == 1 && _tcscmp(pzText.GetData(), _T("0")) == 0)
					{
						if (wParam == 48)
						{
							bHandled = TRUE;
							return TRUE;
						}
						else
						{
							m_pEdtCount->SetText(_T(""));
						}
					}
				}
				else
				{
					bHandled = TRUE;
					return TRUE;
				}
			}
			else if (m_pEdtReceiveID && m_pEdtReceiveID->IsFocused())
			{
				// ֻ��������
				if (wParam < 48 || wParam > 57)
				{
					bHandled = TRUE;
					return TRUE;
				}
			}
		}break;
		
		case WM_KEYDOWN:
		{
			// ճ�� ctrl + v �Ĵ���
			if ((GetKeyState(VK_CONTROL) < 0) && (wParam == 'V') && m_pEdtReceiveID && m_pEdtReceiveID->IsFocused())
			{
				// ��ȡճ������
				CDuiString pzCopyText;
				m_pEdtReceiveID->GetClipboardText(pzCopyText);
				pzCopyText.Replace(_T(" "), _T(""));

				// ������������
				if (!g_GameUtil->MatchRegular(pzCopyText, _T("[0-9]+")))
				{
					bHandled = TRUE;
					return TRUE;
				}
				else
				{
					// ��������,ȥ��ճ���ı��еĿո�
					m_pEdtReceiveID->SetClipboardText(pzCopyText);
				}
			}
		}break;

		default:
			break;
	}

	bHandled = FALSE;
	return FALSE;
}

void MallBuyFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData();
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnMallBuy_Close) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnMallBuy_Cancel) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnMallBuy_ReceiveNickName) == 0)
		{
			if (m_pEdtReceiveID)
			{
				CDuiString pzRecieveID = m_pEdtReceiveID->GetText();
				if (pzRecieveID.GetLength() <= 0 || _tcscmp(pzRecieveID.GetData(), _T("")) == 0)
				{
					int iMsgBoxID = MallMsgBoxID::MSG_BOX_ID_MALL_RECEIVE_ID;
					DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyFrame::MsgBoxProcFocus, this);
					DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), _T("����ID����Ϊ��!"), DLG_ID(iMsgBoxID), DLG_STYLE::DLG_OK, dlg_cb, dlg_cb, dlg_cb);
				}
				else
				{
					if (m_pLoginServer->IsConnect())
					{
						this->CheckNickName();
					}
					else
					{
						m_iCurOperateStatus = MallOperateStatus::MALL_REQ_NICKNAME;
						m_pLoginServer->Connect();
					}
				}
			}
		}
		else if (_tcscmp(sName, R->kBtnMallBuy_Ensure) == 0)
		{
			this->CheckBuy();
		}
		else if (_tcscmp(sName, R->kBtnMallBuy_Sub) == 0)
		{
			this->AddBuyCount(-1);
		}
		else if (_tcscmp(sName, R->kBtnMallBuy_Add) == 0)
		{
			this->AddBuyCount(1);
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_TEXTCHANGED) == 0)
	{
		if (m_pEdtCount && m_pEdtCount->IsFocused())
		{
			WORD wBuyCount = _ttoi(m_pEdtCount->GetText().GetData());
			this->UpdateBuyPrice(wBuyCount);
		}
		else if (m_pEdtReceiveID && m_pEdtReceiveID->IsFocused())
		{
			if (m_pEdtReceiveNickName && m_pEdtReceiveNickName->GetText().GetLength() > 0)
			{
				m_pEdtReceiveNickName->SetTextColor(m_dwRevNameTxtColor);
				m_pEdtReceiveNickName->SetText(_T(""));
				m_PaintManager.SetFocusNeeded(m_pEdtReceiveID);
			}
		}
	}
}

void MallBuyFrame::AddBuyCount(LONG lCount, bool bClear /* = false */)
{
	LONG lTotalCount = 0;
	if (!bClear)
	{
		lTotalCount = _ttol(m_pEdtCount->GetText());
	}
	
	lTotalCount += lCount;
	lTotalCount = max(lTotalCount, m_lMinBuyCount);
	lTotalCount = min(lTotalCount, m_lMaxBuyCount);

	_stprintf_s(m_pzBuffer, _T("%ld"), lTotalCount);
	m_pEdtCount->SetText(m_pzBuffer);

	this->UpdateBuyPrice(lTotalCount);
}

void MallBuyFrame::UpdateBuyPrice(WORD wTotalCount)
{
	// ������������
	LONG lRevCharm = wTotalCount * m_pProps->lRcvCharm;
	_stprintf_s(m_pzBuffer, _T("%ld��"), lRevCharm);
	m_pTxtRevCharm->SetText(m_pzBuffer);

	// �����ܻ��ѵĽ��
	LONGLONG lTotalPrice = wTotalCount * (LONGLONG)m_pProps->lPrice;
	_stprintf_s(m_pzBuffer, _T("%lld���"), lTotalPrice);
	m_pTxtPrice->SetText(m_pzBuffer);

	// �����ܻ��ѵĽ���д
	CString cstrCostCapital = m_pNumberConvert->NumToChineseStr(lTotalPrice);
	m_pTxtCostCapital->SetText(cstrCostCapital);
	m_pTxtCostCapital->SetToolTip(cstrCostCapital);

	m_pBtnPageLeft->SetEnabled(true);
	m_pBtnPageRight->SetEnabled(true);
	if (wTotalCount <= m_lMinBuyCount)
	{
		m_pBtnPageLeft->SetEnabled(false);
	}
	else if (wTotalCount >= m_lMaxBuyCount)
	{
		m_pBtnPageRight->SetEnabled(false);
	}
}

void MallBuyFrame::CheckBuy()
{
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();

	DWORD dwBuyCount		= _ttol(m_pEdtCount->GetText().GetData());
	DWORD dwSourceGameID	= tSelf.GetGameID();
	DWORD dwTargetGameID	= _ttol(m_pEdtReceiveID->GetText().GetData());
	LONGLONG lTotalCost		= _ttoll(m_pTxtPrice->GetText().GetData());
	CDuiString& pzRevID		= m_pEdtReceiveID->GetText();

	// ��⹺������
	if (dwBuyCount <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("��ܰ��ʾ")
			, _T("������������С��1")
			, DLG_ID(MallMsgBoxID::MSG_BOX_ID_MALL_BUY_COUNT)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb);
	}
	// ���������ID�Ƿ�Ϊ��
	else if (pzRevID.GetLength() <= 0)
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("��ܰ��ʾ")
			, _T("���շ�ID����Ϊ��")
			, DLG_ID(MallMsgBoxID::MSG_BOX_ID_MALL_RECEIVE_ID)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	// ���������IDֻ��Ϊ����
	else if (!g_GameUtil->MatchRegular(pzRevID, _T("[0-9]+")))
	{
		DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("��ܰ��ʾ")
			, _T("������������������ַ�!")
			, DLG_ID(MallMsgBoxID::MSG_BOX_ID_MALL_RECEIVE_ID)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
	// ������н���Ƿ��㹻
	else if (tSelf.GetBankScore() < lTotalCost)
	{
		DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), _T("�������н�Ҳ��㣬���ȳ�ֵ��ѽ�Ҵ������У�"));
	}
	else
	{
		if (false)
		{
			// ����ȷ�Ͻ���
			if (dwTargetGameID == dwSourceGameID)
			{
				_stprintf_s(m_pzBuffer, _T("���Ƿ�ȷ�Ϲ���[%lu]��[%s]?"), dwBuyCount, m_pProps->szName);
			}
			else
			{
				_stprintf_s(m_pzBuffer, _T("���Ƿ�ȷ������[%lu]��[%s]��IDΪ��%lu�������?"), dwBuyCount, m_pProps->szName, dwTargetGameID);
			}

			DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager
				, _T("��ܰ��ʾ")
				, m_pzBuffer
				, (DLG_ID)MallMsgBoxID::MSG_BOX_ID_MALL_BUY_ENSURE
				, DLG_STYLE::DLG_OK_CANCEL
				, dlg_cb);
		}
		else
		{
			// ֱ����ת����
			TNotifyUI tMsg;
			this->MsgBoxProcFocus((DLG_ID)MallMsgBoxID::MSG_BOX_ID_MALL_BUY_ENSURE, tMsg);
		}
	}
}

void MallBuyFrame::BuyInHall()
{
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();

	DWORD dwPropsID = m_pProps->wPropsID;
	DWORD dwBuyCount = _ttol(m_pEdtCount->GetText().GetData());
	DWORD dwSourceGameID = tSelf.GetGameID();
	DWORD dwTargetGameID = _ttol(m_pEdtReceiveID->GetText().GetData());
	BYTE cbSendLocation = LOCATION_PLAZA_ROOM;

	CMD_GF_BuyProps tGifts;
	memset(&tGifts, 0, sizeof(CMD_GF_BuyProps));

	tGifts.cbSendLocation	= cbSendLocation;
	tGifts.wPropsID			= dwPropsID;
	tGifts.dwPachurseCount	= dwBuyCount;
	tGifts.dwSourceGameID	= dwSourceGameID;
	tGifts.dwTargetGameID	= dwTargetGameID;
	CMD5Encrypt::EncryptData(m_pzBuyGoodsPw, tGifts.szPassword);

	g_Logger->TryInfo(_T("MallBuyFrame - BuyProps in Hall Req"));
	this->SetBuyBtnEnable(false);
	DWORD bRes = m_pLoginServer->SendData(MDM_GF_PRESENT, SUB_GF_BUYPROPS_REQ, &tGifts, sizeof(CMD_GF_BuyProps));
	if (!bRes)
	{
		// �ָ�������ð�ť
		this->SetBuyBtnEnable(true);
		DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), _T("����ʧ��!"));
	}

	// Unreal Check
	LONGLONG lTotalCost = _ttoll(m_pTxtPrice->GetText().GetData());
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	pUnrealControl->CheckUnreal_User(UnrealNSP::UnrealChargeType::UCT_TRANSFER, lTotalCost, tGifts.szPassword, cbSendLocation);
}

void MallBuyFrame::BuyInRoom()
{
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();

	DWORD dwPropsID = m_pProps->wPropsID;
	DWORD dwBuyCount = _ttol(m_pEdtCount->GetText().GetData());
	DWORD dwSourceGameID = tSelf.GetGameID();
	DWORD dwTargetGameID = _ttol(m_pEdtReceiveID->GetText().GetData());
	BYTE cbSendLocation = LOCATION_GAME_ROOM;

	CMD_GF_BuyProps tGifts;
	memset(&tGifts, 0, sizeof(CMD_GF_BuyProps));

	tGifts.cbSendLocation = cbSendLocation;
	tGifts.wPropsID = dwPropsID;
	tGifts.dwPachurseCount = dwBuyCount;
	tGifts.dwSourceGameID = dwSourceGameID;
	tGifts.dwTargetGameID = dwTargetGameID;
	CMD5Encrypt::EncryptData(m_pzBuyGoodsPw, tGifts.szPassword);

	g_Logger->TryInfo(_T("MallBuyFrame - BuyProps in Room Req"));

	this->SetBuyBtnEnable(false);
	RoomManager* pRoomManager = RoomManager::GetInstance();
	DWORD bRes = pRoomManager->SendData(MDM_GF_PRESENT, SUB_GF_BUYPROPS_REQ, &tGifts, sizeof(CMD_GF_BuyProps));
	if (!bRes)
	{
		// �ָ�������ð�ť
		this->SetBuyBtnEnable(true);
		DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), _T("����ʧ��!"));
	}

	// Unreal Check
	LONGLONG lTotalCost = _ttoll(m_pTxtPrice->GetText().GetData());
	UnrealControl* pUnrealControl = UnrealControl::GetInstance();
	pUnrealControl->CheckUnreal_User(UnrealNSP::UnrealChargeType::UCT_TRANSFER, lTotalCost, tGifts.szPassword, cbSendLocation);
}

void MallBuyFrame::CheckNickName()
{
	g_Logger->TryInfo(_T("MallBuyFrame - Req NickName By RevID"));

	// ����ս����ǳ�
	m_pEdtReceiveNickName->SetTextColor(m_dwRevNameTxtColor);
	m_pEdtReceiveNickName->SetText(_T(""));

	// ���Ͳ�ѯ�ǳ�����
	CDuiString pzRecieveID = m_pEdtReceiveID->GetText();
	g_Logger->TryInfo(_T("MallBuyFrame - Req NickName By RevID [gid = %s]"), pzRecieveID.GetData());

	tagLS_GetUserInfoByGameID tGetUserInfo;
	memset(&tGetUserInfo, 0, sizeof(tagLS_GetUserInfoByGameID));
	tGetUserInfo.dwGameID = _ttol(pzRecieveID.GetData());
	DWORD bRes = m_pLoginServer->SendData(MAIN_LS_USER, SUB_C2LS_GETUSERINFO_REQ, &tGetUserInfo, sizeof(tagLS_GetUserInfoByGameID));
	if (!bRes)
	{
		DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), _T("��ȡ�ǳ�ʧ��!"));
	}
}

void MallBuyFrame::SetTickedBuyPw(bool bTicked)
{
	m_bTickedBuyPw = bTicked;
}

void MallBuyFrame::SetBuyGoodsPw(CDuiString& pzBuyPw)
{
	m_pzBuyGoodsPw = pzBuyPw;
}

void MallBuyFrame::SendBuyReq()
{
	switch (m_iCurBuyType)
	{
		case MallBuyFrame::MALL_BUY_TYPE_NONE:
		case MallBuyFrame::MALL_BUY_TYPE_HALL:
		{
			if (m_pLoginServer->IsConnect())
			{
				this->BuyInHall();
			}
			else
			{
				m_iCurOperateStatus = MallOperateStatus::MALL_BUY_OS_BUY;
				m_pLoginServer->Connect();
			}
		}break;

		case MallBuyFrame::MALL_BUY_TYPE_ROOM:
		{
			this->BuyInRoom();
		}break;

		default:
			break;
	}
}

void MallBuyFrame::SetBuyBtnEnable(bool bEnable)
{
	if (m_pBtnEnsureBuy)
	{
		m_pBtnEnsureBuy->SetEnabled(bEnable);
	}
}

void MallBuyFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case MallMsgBoxID::MSG_BOX_ID_MALL_BUY_COUNT:
		{
			m_pEdtCount->SetFocus();
		}break;

		case MallMsgBoxID::MSG_BOX_ID_MALL_RECEIVE_ID:
		{
			m_pEdtReceiveID->SetFocus();
		}break;

		case MallMsgBoxID::MSG_BOX_ID_MALL_BUY_ENSURE:
		{
			ServiceData* pServerData = ServiceData::GetInstance();
			GameCacheData& tGameCacheData = pServerData->GetGameCacheData();
			if (MallBuyCheckFrame::IsInstanceExist() || !tGameCacheData.bSaveBuyGoodsPw || tGameCacheData.pzBuyGoodsPw.GetData() <= 0)
			{
				MallBuyCheckFrame* pCheckFrame = MallBuyCheckFrame::GetInstance();
				pCheckFrame->CreateFrame(this);
			}
			else
			{
				m_pzBuyGoodsPw = tGameCacheData.pzBuyGoodsPw;
				this->SendBuyReq();
			}
		}break;

		case MallMsgBoxID::MSG_BOX_ID_MALL_PW_ERROR:
		{
			MallBuyCheckFrame* pCheckFrame = MallBuyCheckFrame::GetInstance();
			pCheckFrame->CreateFrame(this);
		}break;

		default:
			break;
	}
}

bool MallBuyFrame::OnConnectSuccessProc()
{
	switch (m_iCurOperateStatus)
	{
		case MallOperateStatus::MALL_BUY_OS_NONE:									break;
		case MallOperateStatus::MALL_REQ_NICKNAME:		this->CheckNickName();		break;
		case MallOperateStatus::MALL_BUY_OS_BUY:		this->BuyInHall();			break;
		case MallOperateStatus::MALL_BUY_OS_EXTRA:									break;
		default:																	break;
	}
	m_iCurOperateStatus = MallOperateStatus::MALL_BUY_OS_NONE;

	return true;
}

bool MallBuyFrame::OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)
{
	return true;
}

void MallBuyFrame::ReConnect(DLG_ID id, TNotifyUI& msg)
{
	if (!m_pLoginServer)
	{
		m_pLoginServer = LoginServer::GetInstance();
	}

	m_pLoginServer->Connect();
}

bool MallBuyFrame::OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_LS2C_GETUSERINFO:
		{
			tagLS_GetUserInfoByGameID* pGetUserInfo = static_cast<tagLS_GetUserInfoByGameID*>(pDataBuffer);
			if (pGetUserInfo && m_pEdtReceiveNickName)
			{
				DWORD dwGameID = pGetUserInfo->dwGameID;
				DWORD dwUserID = pGetUserInfo->dwUserID;
				m_pEdtReceiveNickName->SetFocus();
				m_pEdtReceiveNickName->SetText(pGetUserInfo->szNickname);
			}
		}break;

		case SUB_LS2C_GETUSERINFO_ERROR:
		{
			//DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), _T("��ȡ�ǳ�ʧ��!"));
			if (m_pEdtReceiveNickName)
			{
				m_pEdtReceiveNickName->SetTextColor(0xFFFF0000);
				m_pEdtReceiveNickName->SetText(_T("���û�������"));
			}
		}break;
	}

	return true;
}

bool MallBuyFrame::OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	switch (cmd.wSubCmdID)
	{
		case SUB_GF_BUYPROPS_OK:
		{
			// �ָ�������ð�ť
			this->SetBuyBtnEnable(true);

			CMD_GF_PropsNotify* pStuOk = static_cast<CMD_GF_PropsNotify*>(pDataBuffer);
			if (pStuOk)
			{
				g_Logger->TryInfo(_T("MallBuyFrame - BuyProps OK [location = %u, type = %u]"), pStuOk->cbSendLocation, pStuOk->cbPropType);
				
				// ������Ǽ�ס�����������
				ServiceData* pServerData = ServiceData::GetInstance();
				GameCacheData& tGameCacheData = pServerData->GetGameCacheData();
				tGameCacheData.bSaveBuyGoodsPw = m_bTickedBuyPw;
				if (tGameCacheData.bSaveBuyGoodsPw)
				{
					tGameCacheData.pzBuyGoodsPw = m_pzBuyGoodsPw;
				}

				// Unreal Check
				DWORD dwRecieveID = _ttol(m_pEdtReceiveID->GetText().GetData());
				UnrealControl* pUnrealControl = UnrealControl::GetInstance();
				if (pUnrealControl->CheckUnreal_Return(pStuOk->dwRcvGameID, dwRecieveID))
				{
					return true;
				}

				// ������Լ������ߣ��򵯳�����ɹ���ʾ��
				DataManager* pDataManager = DataManager::GetInstance();
				UserData& tSelf = pDataManager->GetSelfData();
				if (pStuOk->dwSrcGameID == tSelf.GetGameID())
				{
					tSelf.SetBankScore(pStuOk->lBankScore);
					if (pStuOk->dwRcvGameID == tSelf.GetGameID())
					{
						_stprintf_s(m_pzBuffer, _T("����[%u]��[%s]�ɹ�!"), pStuOk->wCount, pStuOk->szPropsName);
						DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), m_pzBuffer);
					}
					else
					{
						_stprintf_s(m_pzBuffer, _T("[%s](ID:%u)��[%s](ID:%u)������%u��%s!")
							, pStuOk->szSrcNickname
							, pStuOk->dwSrcGameID
							, pStuOk->szRcvNickname
							, pStuOk->dwRcvGameID
							, pStuOk->wCount
							, pStuOk->szPropsName);
						DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), m_pzBuffer);
					}
				}
			}

			// �������ID,�����ǳ�
			m_pEdtReceiveID->SetText(_T(""));
			m_pEdtReceiveNickName->SetText(_T(""));

			// �۽�����ID
			m_pEdtReceiveID->SetFocus();
		}break;

		case SUB_GF_BUYPROPS_ERROR:
		{
			//1 �������벻��
			//2 ��˶ԶԷ�����ϷID
			//3 ������һë��û�У����Ƚ���Ҵ������С����ֵ
			//4 �����еĽ�Ҳ����������Ƚ���Ҵ������С����ֵ
			//5 �Է������Ǹ��߼��Ļ�Ա���㲻�����͵ͼ���Ա��

			// �ָ�������ð�ť
			this->SetBuyBtnEnable(true);

			// �������ʧ��,ֻ��㲥���Լ�
			tagError* pStuError = static_cast<tagError*>(pDataBuffer);
			if (!pStuError) return false;
			
			g_Logger->TryInfo(_T("MallBuyFrame - BuyProps Failed [code = %ld]"), pStuError->lErrorCode);

			CDuiString pzErrDesc;
			int iMsgBoxID = MallMsgBoxID::MSG_BOX_ID_NONE;
			switch (pStuError->lErrorCode)
			{
				case 1:
				{
					iMsgBoxID = MallMsgBoxID::MSG_BOX_ID_MALL_PW_ERROR;

					do // ���������Ʒ�Ǳ������������
					{
						ServiceData* pServerData = ServiceData::GetInstance();
						GameCacheData& tGameCacheData = pServerData->GetGameCacheData();

						tGameCacheData.bSaveBuyGoodsPw = false;
						tGameCacheData.pzBuyGoodsPw = _T("");
					} while (0);
				}break;

				case 2:
				case 3:
				case 4:
				case 5:
				{
					iMsgBoxID = MallMsgBoxID::MSG_BOX_ID_MALL_RECEIVE_ID;
				}break;

				default:
					break;
			}

			pzErrDesc = pStuError->szErrorDescribe;
			DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyFrame::MsgBoxProcFocus, this);
			DialogFrame::MsgBox(&m_PaintManager, _T("��ܰ��ʾ"), pzErrDesc.GetData(), DLG_ID(iMsgBoxID), DLG_STYLE::DLG_OK, dlg_cb, dlg_cb, dlg_cb);
		}break;

		default:
			break;
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------
// MallGiveFrame - ���ͽ���
MallGiveFrame::MallGiveFrame()
	: m_pParentManager(nullptr)
	, m_pProps(nullptr)
	, m_pTxtName(nullptr)
	, m_pEdtCount(nullptr)
	, m_pEdtReceiveID(nullptr)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

MallGiveFrame::~MallGiveFrame()
{
	g_GameUtil->SetFrozenEnable(m_pParentManager, true);
}

BOOL MallGiveFrame::CreateFrame(CPaintManagerUI* pParentManager, tagPropsInfo* pProps)
{
	ASSERT(pParentManager);
	if (!pParentManager || !pProps) return FALSE;

	m_pProps = pProps;
	m_pParentManager = pParentManager;
	if (FALSE == IsWindow(m_hWnd))
	{
		HWND hParentWnd = m_pParentManager->GetPaintWindow();
		g_GameUtil->SetFrozenEnable(m_pParentManager, false);
		m_hWnd = this->Create(hParentWnd, NULL, UI_CLASSSTYLE_DIALOG, 0L);
		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}

	return TRUE;
}

UILIB_RESOURCETYPE MallGiveFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR MallGiveFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString MallGiveFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString MallGiveFrame::GetSkinFile()
{
	return _T("hall\\mall\\give.xml");
}

LRESULT MallGiveFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

void MallGiveFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void MallGiveFrame::InitWindow()
{
	m_pTxtName = static_cast<CTextUI*>(m_PaintManager.FindControl(R->kTxtMallGive_Name));
	m_pEdtCount = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMallGive_Count));
	m_pEdtReceiveID = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMallGive_ReceiveID));
}

void MallGiveFrame::InitData(TNotifyUI& msg)
{
	// ���ö�Ӧ��Ʒ��
	m_pTxtName->SetText(m_pProps->szName);

	// Ĭ����������
	m_pEdtCount->SetText(_T("1"));

	// �۽�����ID
	m_pEdtReceiveID->SetText(_T(""));
	m_PaintManager.SetFocusNeeded(m_pEdtReceiveID);
}

LRESULT MallGiveFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_CHAR:
		case WM_IME_CHAR:
		{
			if (m_pEdtCount && m_pEdtCount->IsFocused())
			{
				if (m_pEdtCount->GetText().GetLength() == 0)
				{
					m_pEdtCount->SetText(_T("0"));
				}

				// ֻ��������
				if (wParam >= 48 && wParam <= 57)
				{
					CDuiString& pzText = m_pEdtCount->GetText();
					if (pzText.GetLength() == 1 && _tcscmp(pzText.GetData(), _T("0")) == 0)
					{
						if (wParam == 48)
						{
							bHandled = TRUE;
							return TRUE;
						}
						else
						{
							m_pEdtCount->SetText(_T(""));
						}
					}
				}
				else
				{
					bHandled = TRUE;
					return TRUE;
				}
			}
			else if (m_pEdtReceiveID && m_pEdtReceiveID->IsFocused())
			{
				// ֻ��������
				if (wParam < 48 || wParam > 57)
				{
					bHandled = TRUE;
					return TRUE;
				}
			}
		}break;

		default:
			break;
	}

	bHandled = FALSE;
	return FALSE;
}

void MallGiveFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnMallGive_Close) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnMallGive_Cancel) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnMallGive_Ensure) == 0)
		{
			this->Check();
		}
	}
}
void MallGiveFrame::Check()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------
// MallBuyCheckFrame - ����ʱ���������

MallBuyCheckFrame* MallBuyCheckFrame::g_instance = nullptr;
MallBuyCheckFrame::MallBuyCheckFrame()
: m_pBuyFrame(nullptr)
, m_pEdtPassword(nullptr)
, m_pOptPassword(nullptr)
{

}

MallBuyCheckFrame::~MallBuyCheckFrame()
{

}

MallBuyCheckFrame* MallBuyCheckFrame::GetInstance()
{
	if (MallBuyCheckFrame::g_instance == NULL)
	{
		MallBuyCheckFrame::g_instance = new (std::nothrow)MallBuyCheckFrame();
	}

	return MallBuyCheckFrame::g_instance;
}

BOOL MallBuyCheckFrame::CreateFrame(MallBuyFrame* pBuyFrame)
{
	ASSERT(pBuyFrame);
	if (!pBuyFrame) return FALSE;

	m_pBuyFrame = pBuyFrame;
	if (FALSE == IsWindow(m_hWnd))
	{
		HWND hParentWnd = pBuyFrame->GetHWND();
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

UILIB_RESOURCETYPE MallBuyCheckFrame::GetResourceType() const
{
	return GameResource::g_ResourceType;
}

LPCTSTR MallBuyCheckFrame::GetResourceID() const
{
	return GameResource::g_ResourceID;
}

CDuiString MallBuyCheckFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString MallBuyCheckFrame::GetSkinFile()
{
	return _T("hall\\mall\\buy_check.xml");
}

LRESULT MallBuyCheckFrame::ResponseDefaultKeyEvent(WPARAM wParam)
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

void MallBuyCheckFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	SAFE_DELETE(MallBuyCheckFrame::g_instance);
}

void MallBuyCheckFrame::InitWindow()
{
	m_pEdtPassword = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMallBuyCheck_Password));
	m_pOptPassword = static_cast<COptionUI*>(m_PaintManager.FindControl(R->kOptMallBuyCheck_Password));

	// �۽����������
	m_PaintManager.SetFocusNeeded(m_pEdtPassword);
}

void MallBuyCheckFrame::InitData(TNotifyUI& msg)
{
}

void MallBuyCheckFrame::Check()
{
	if (!m_pEdtPassword || !m_pOptPassword) return;
	CDuiString& pzPw = m_pEdtPassword->GetText();
	bool bTickedBankPw = m_pOptPassword->IsSelected();

	if (g_GameUtil->MatchWordByType(pzPw, MatchWords_Type::REG_TYPE_PASSWORD))
	{
		if (m_pBuyFrame)
		{
			m_pBuyFrame->SetTickedBuyPw(bTickedBankPw);
			m_pBuyFrame->SetBuyGoodsPw(pzPw);
			m_pBuyFrame->SendBuyReq();
			this->Close();
		}
	}
	else
	{
		CDuiString pzErrMsg;
		if (pzPw.GetLength() <= 0)
		{
			pzErrMsg = _T("���벻��Ϊ��!");
		}
		else if (g_GameUtil->MatchRegular(pzPw, _T(".*[\\s]+.*")))
		{
			pzErrMsg = _T("���ܰ����հ��ַ���!");
		}
		else
		{
			pzErrMsg = _T("���ܰ������ֻ�������ŵ�!");
		}

		DialogCallBack dlg_cb = CC_CALLBACK_2(MallBuyCheckFrame::MsgBoxProcFocus, this);
		DialogFrame::MsgBox(&m_PaintManager
			, _T("��ܰ��ʾ")
			, pzErrMsg.GetData()
			, DLG_ID(MsgBoxID::MSG_BOX_ID_CHECK_PW)
			, DLG_STYLE::DLG_DEFAULT
			, dlg_cb
			, dlg_cb
			, dlg_cb);
	}
}

void MallBuyCheckFrame::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case MsgBoxID::MSG_BOX_ID_NONE:									break;
		case MsgBoxID::MSG_BOX_ID_CHECK_PW:	m_pEdtPassword->SetFocus();	break;
		default:														break;
	}
}

void MallBuyCheckFrame::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnMallBuyCheck_Close) == 0)
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnMallBuyCheck_Ensure) == 0)
		{
			this->Check();
		}
	}
}