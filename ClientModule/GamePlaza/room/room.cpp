#include "stdafx.h"
#include "room.h"
#include "..\\hall\\mall.h"
#include "..\\hall\\infoFrame.h"
#include "..\\data\\serverList.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

Room::Room(CPaintManagerUI* pManager, RoomManager* pRoomManagerRef)
	: m_wKindID(0)
	, m_wServerID(0)
	, m_wSortID(0)
	, m_wStationID(0)
	, m_wRoomServiceType(0)
	, m_dwOnlineCount(0)
	, m_wTableCount(0)
	, m_wChairCount(0)
	, m_wTaxRate(0)
	, m_lLessScore(0)
	, m_lMaxScore(0)
	, m_lRestrictScore(0)
	, m_wServerPort(0)
	, m_dwServerAddr(0)
	, m_dwVideoAddr(0)
	, m_cbHideUserInfo(0)
	, m_bHundredLayout(false)
	, m_bHundredGame(false)
	, m_pManager(pManager)
	, m_pTableManager(nullptr)
	, m_pRoomManagerRef(pRoomManagerRef)
{
	memset(m_pzBuffer, 0, ArrayCount(m_pzBuffer) * sizeof(TCHAR));

	g_Logger->TryInfo(_T("Room - Create"));
}

Room::~Room()
{
	SAFE_DELETE(m_pTableManager);

	// ����Ѵ��ڹ���������,��ر�
	if (MallBuyFrame::IsInstanceExist())
	{
		MallBuyFrame* pBuyFrame = MallBuyFrame::GetInstance();
		pBuyFrame->Close();
	}
	g_Logger->TryInfo(_T("Room - Destroy"));
}

void Room::InitRoomData(tagGameServer* pData)
{
	g_Logger->TryInfo(_T("Room - Init Room Data"));
	if (!pData) return;

	m_wSortID			= pData->wSortID;							//�������
	m_wKindID			= pData->wKindID;							//���ƺ���
	m_wServerID			= pData->wServerID;							//�������
	m_wStationID		= pData->wStationID;						//վ�����
	m_wServerPort		= pData->wServerPort;						//����˿�
	m_wRoomServiceType	= pData->wRoomServiceType;					//��������
	m_dwServerAddr		= pData->dwServerAddr;						//�����ַ
	m_dwOnlineCount		= pData->dwOnlineCount;						//��������
	m_pzName			= pData->szServerName;						//��������
	m_pzGameName		= m_pzName;									//������Ϸ��
	
	m_wTaxRate			= pData->wTaxRate;							//��Ϸ˰��
	m_lLessScore		= pData->lLessScore;						//��ͻ���
	m_lMaxScore			= pData->lMaxScore;							//��߻���
	m_lRestrictScore	= pData->lRestrictScore;					//���ƻ���

	//����������Ϸ��
	ServiceData* pServiceData = ServiceData::GetInstance();
	tagGameKind* pGameKind = pServiceData->SearchGameKind(m_wKindID);
	if (pGameKind)
	{
		m_pzGameName = pGameKind->szKindName;
	}

	//���������巿��,���������ð�ť
	bool bVisible = pGameKind->wKindID == 26;
	CButtonUI* pBtnRommSetting = static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnRoomPanel_Setting));
	if (pBtnRommSetting)
	{
		pBtnRommSetting->SetVisible(bVisible);
	}

	g_Logger->TryInfo(_T("Room - Init Room Data Complete"));
}

void Room::InitRoomData(CMD_GR_ServerInfo* pData)
{
	g_Logger->TryInfo(_T("Room - Init Room Data"));
	if (!pData) return;

	//��������
	m_wKindID			= pData->wKindID;								//���� I D
	m_wTableCount		= pData->wTableCount;							//������Ŀ
	m_wChairCount		= pData->wChairCount;							//������Ŀ
	m_dwVideoAddr		= pData->dwVideoAddr;							//��Ƶ��ַ
	m_bHundredGame		= m_wChairCount >= 100;							//�Ƿ��ǰ�����Ϸ
	m_bHundredLayout	= m_bHundredGame || m_wChairCount == 1;			//�Ƿ������Ϸ���ַ���
	
	//��չ����
	m_wRoomServiceType	= pData->wRoomServiceType;						//��������
	m_cbHideUserInfo	= pData->cbHideUserInfo;						//������Ϣ

	g_Logger->TryInfo(_T("Room - Init Room Data Complete"));
}

void Room::InitPropsUI()
{
	g_Logger->TryInfo(_T("Room - Init Room Props"));

	m_tPropsPanel.Reset();
	m_tGiftsPanel.Reset();

	// �������߷���(���ߡ�����)
	DWORD iNextKey = 0;
	ServiceData* pServiceData = ServiceData::GetInstance();
	for (size_t i = 0; i < pServiceData->GetGamePropsCount(); ++i)
	{
		tagPropsInfo* pProps = pServiceData->EnumGameProps(iNextKey);
		if (pProps)
		{
			switch (pProps->wType)
			{
				// ����
				case 0:	m_tGiftsPanel.vPropsMapRef.push_back(pProps);	break;

				// ��Ա��
				case 1:	m_tPropsPanel.vPropsMapRef.push_back(pProps);	break;
				default:												break;
			}
		}
	}

	// ����ÿҳ��Ʒ�Ű���ʾ�������
	m_tPropsPanel.dwMinPage = 1;
	m_tPropsPanel.dwMaxCountsPerPage = 5;

	m_tGiftsPanel.dwMinPage = 1;
	m_tGiftsPanel.dwMaxCountsPerPage = 5;

	// ��ʼ��������
	do
	{
		m_tPropsPanel.pBtnLeft	= static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnPropPanel_Left));
		m_tPropsPanel.pBtnRight = static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnPropPanel_Right));
		m_tPropsPanel.pzField	= _T("prop_panel-prop_btn_");
		m_tPropsPanel.dwCurPage = 1;
		m_tPropsPanel.dwMaxPage = ceil(m_tPropsPanel.vPropsMapRef.size() / (float)m_tPropsPanel.dwMaxCountsPerPage);
		m_tPropsPanel.pBtnLeft->SetEnabled(false);
		m_tPropsPanel.pBtnRight->SetEnabled(true);
		if (m_tPropsPanel.vPropsMapRef.size() <= m_tPropsPanel.dwMaxCountsPerPage)
		{
			m_tPropsPanel.pBtnRight->SetEnabled(false);
		}

		// ������ͼ��
		for (size_t i = 0; i < m_tPropsPanel.vPropsMapRef.size(); ++i)
		{
			if (i < m_tPropsPanel.dwMaxCountsPerPage)
			{
				_stprintf_s(m_pzBuffer, _T("%s%u"), m_tPropsPanel.pzField, i);
				CButtonUI* pBtn = static_cast<CButtonUI*>(m_pManager->FindControl(m_pzBuffer));

				_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), m_tPropsPanel.vPropsMapRef[i]->wPropsID);
				pBtn->SetBkImage(m_pzBuffer);
				pBtn->SetToolTip(m_tPropsPanel.vPropsMapRef[i]->szName);
				pBtn->SetTag(m_tPropsPanel.vPropsMapRef[i]->wPropsID);
			}
		}
	} while (0);

	// ��ʼ��������
	do
	{
		m_tGiftsPanel.pBtnLeft	= static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnGiftPanel_Left));
		m_tGiftsPanel.pBtnRight = static_cast<CButtonUI*>(m_pManager->FindControl(R->kBtnGiftPanel_Right));
		m_tGiftsPanel.pzField	= _T("prop_panel-gift_btn_");
		m_tGiftsPanel.dwCurPage = 1;
		m_tGiftsPanel.dwMaxPage = ceil(m_tGiftsPanel.vPropsMapRef.size() / (float)m_tGiftsPanel.dwMaxCountsPerPage);
		m_tGiftsPanel.pBtnLeft->SetEnabled(false);
		m_tGiftsPanel.pBtnRight->SetEnabled(true);
		if (m_tGiftsPanel.vPropsMapRef.size() <= m_tGiftsPanel.dwMaxCountsPerPage)
		{
			m_tGiftsPanel.pBtnRight->SetEnabled(false);
		}

		// ������ͼ��
		for (size_t i = 0; i < m_tGiftsPanel.vPropsMapRef.size(); ++i)
		{
			if (i < m_tGiftsPanel.dwMaxCountsPerPage)
			{
				_stprintf_s(m_pzBuffer, _T("%s%u"), m_tGiftsPanel.pzField, i);
				CButtonUI* pBtn = static_cast<CButtonUI*>(m_pManager->FindControl(m_pzBuffer));
				
				_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), m_tGiftsPanel.vPropsMapRef[i]->wPropsID);
				pBtn->SetBkImage(m_pzBuffer);
				pBtn->SetToolTip(m_tGiftsPanel.vPropsMapRef[i]->szName);
				pBtn->SetTag(m_tGiftsPanel.vPropsMapRef[i]->wPropsID);
			}
		}
	} while (0);

	g_Logger->TryInfo(_T("Room - Init Room Props Complete"));
}

void Room::ResetPropsUI()
{
	g_Logger->TryInfo(_T("Room - Reset Room Props"));

	// ����������
	for (int i = 0; i < m_tPropsPanel.dwMaxCountsPerPage; ++i)
	{
		_stprintf_s(m_pzBuffer, _T("%s%d"), m_tPropsPanel.pzField, i);
		CButtonUI* pPropsBtn = static_cast<CButtonUI*>(m_pManager->FindControl(m_pzBuffer));
		pPropsBtn->SetBkImage(_T(""));
		pPropsBtn->SetToolTip(_T(""));
		pPropsBtn->SetTag(NULL);
	}
	m_tPropsPanel.pBtnLeft->SetVisible(true);
	m_tPropsPanel.pBtnLeft->SetEnabled(true);
	m_tPropsPanel.pBtnRight->SetVisible(true);
	m_tPropsPanel.pBtnRight->SetEnabled(true);
	m_tPropsPanel.Reset();

	// ����������
	for (int i = 0; i < m_tGiftsPanel.dwMaxCountsPerPage; ++i)
	{
		_stprintf_s(m_pzBuffer, _T("%s%d"), m_tGiftsPanel.pzField, i);
		CButtonUI* pGiftsBtn = static_cast<CButtonUI*>(m_pManager->FindControl(m_pzBuffer));
		pGiftsBtn->SetBkImage(_T(""));
		pGiftsBtn->SetToolTip(_T(""));
		pGiftsBtn->SetTag(NULL);
	}
	m_tGiftsPanel.pBtnLeft->SetVisible(true);
	m_tGiftsPanel.pBtnLeft->SetEnabled(true);
	m_tGiftsPanel.pBtnRight->SetVisible(true);
	m_tGiftsPanel.pBtnRight->SetEnabled(true);
	m_tGiftsPanel.Reset();

	g_Logger->TryInfo(_T("Room - Reset Room Props Complete"));
}

void Room::PropsTurnPage(PropsPanelStruct& tTarget, bool bNext)
{
	int iCurIndex = 0;
	if (tTarget.dwCurPage < tTarget.dwMinPage || tTarget.dwCurPage > tTarget.dwMaxPage)
	{
		return;
	}

	// �ҷ�ҳ
	if (bNext)
	{
		iCurIndex = tTarget.dwMaxCountsPerPage * tTarget.dwCurPage++;
		int iLastIndex = iCurIndex + tTarget.dwMaxCountsPerPage;
		iCurIndex = min(iLastIndex, tTarget.vPropsMapRef.size());
		iCurIndex = iCurIndex - tTarget.dwMaxCountsPerPage;
		iCurIndex = max(0, iCurIndex);

		tTarget.pBtnLeft->SetEnabled(true);
		if (tTarget.dwCurPage >= tTarget.dwMaxPage)
		{
			tTarget.pBtnRight->SetEnabled(false);
		}
	}
	// ��ҳ
	else
	{
		iCurIndex = tTarget.dwMaxCountsPerPage * tTarget.dwCurPage--;
		iCurIndex = min(iCurIndex, tTarget.vPropsMapRef.size());
		iCurIndex = iCurIndex - tTarget.dwMaxCountsPerPage;
		int iStartIndex = iCurIndex - tTarget.dwMaxCountsPerPage;
		iCurIndex = max(0, iStartIndex);

		tTarget.pBtnRight->SetEnabled(true);
		if (tTarget.dwCurPage <= tTarget.dwMinPage)
		{
			tTarget.pBtnLeft->SetEnabled(false);
		}
	}

	// �滻ͼ����Ϣ��
	for (int i = 0; i < tTarget.dwMaxCountsPerPage; ++i)
	{
		_stprintf_s(m_pzBuffer, _T("%s%d"), tTarget.pzField, i);
		CButtonUI* pBtn = static_cast<CButtonUI*>(m_pManager->FindControl(m_pzBuffer));

		int iTmpInedx = iCurIndex + i;
		_stprintf_s(m_pzBuffer, _T("hall\\mall\\goods\\%u.png"), tTarget.vPropsMapRef[iTmpInedx]->wPropsID);
		pBtn->SetBkImage(m_pzBuffer);
		pBtn->SetToolTip(tTarget.vPropsMapRef[iTmpInedx]->szName);
		pBtn->SetTag(tTarget.vPropsMapRef[iTmpInedx]->wPropsID);
	}
}

bool Room::OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize)
{
	if (!MallBuyFrame::IsInstanceExist()) return false;

	MallBuyFrame* pBuyFrame = MallBuyFrame::GetInstance();
	return pBuyFrame->OnNetwork_Props(cmd, pDataBuffer, wDataSize);
}

void Room::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0 || _tcscmp(sType, DUI_MSGTYPE_RCLICK) == 0)
	{
		int iTableStrPos = sName.Find(_T("table_"));
		int iPropsStrPos = sName.Find(m_tPropsPanel.pzField);
		int iGiftsStrPos = sName.Find(m_tGiftsPanel.pzField);
		if (iTableStrPos != -1)
		{
			CDuiString str = _T("table_");
			int n = str.GetLength();
			iTableStrPos += n;
			CDuiString str_index = _T("");
			for (;;)
			{
				TCHAR v_char = sName.GetAt(iTableStrPos++);
				str_index += v_char;
				int v_ascii = (int)v_char;
				if (v_ascii >= 48 && v_ascii <= 57)
					continue;
				break;
			}

			if (_tcscmp(str_index, _T("")) != 0)
			{
				int table_index = _ttoi(str_index);
				Table* pTable = m_pTableManager->GetTableByID(table_index);
				if (pTable)
				{
					pTable->Notify(msg);
				}
			}
		}
		else if (iPropsStrPos != -1)
		{
			UINT uId = msg.pSender->GetTag();
			tagPropsInfo* pProps = nullptr;
			for (const auto& it : m_tPropsPanel.vPropsMapRef)
			{
				if (uId == it->wPropsID)
				{
					pProps = it;
					break;
				}
			}
			
			MallBuyFrame* pBuyFrame = MallBuyFrame::GetInstance();
			pBuyFrame->CreateFrame(m_pManager, pProps, MallBuyFrame::MallBuyType::MALL_BUY_TYPE_ROOM);
		}
		else if (iGiftsStrPos != -1)
		{
			UINT uId = msg.pSender->GetTag();
			tagPropsInfo* pProps = nullptr;
			for (const auto& it : m_tGiftsPanel.vPropsMapRef)
			{
				if (uId == it->wPropsID)
				{
					pProps = it;
					break;
				}
			}
			
			MallBuyFrame* pBuyFrame = MallBuyFrame::GetInstance();
			pBuyFrame->CreateFrame(m_pManager, pProps, MallBuyFrame::MallBuyType::MALL_BUY_TYPE_ROOM);
		}
		else if (_tcscmp(sName, R->kBtnPropPanel_Left) == 0)
		{
			this->PropsTurnPage(m_tPropsPanel, false);
		}
		else if (_tcscmp(sName, R->kBtnPropPanel_Right) == 0)
		{
			this->PropsTurnPage(m_tPropsPanel, true);
		}
		else if (_tcscmp(sName, R->kBtnGiftPanel_Left) == 0)
		{
			this->PropsTurnPage(m_tGiftsPanel, false);
		}
		else if (_tcscmp(sName, R->kBtnGiftPanel_Right) == 0)
		{
			this->PropsTurnPage(m_tGiftsPanel, true);
		}
		else if (_tcscmp(sName, R->kBtnChatPanel_Clean) == 0)
		{
			CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(R->kEdtChatPanel_View));
			if (pViewEdit)
			{
				pViewEdit->SetText(_T(""));
				pViewEdit->Clear();
			}
		}
		else if (_tcscmp(sName, R->kBtnRoomPanel_Setting) == 0)
		{
			InfoFrame* pInfoFrame = InfoFrame::GetInstance();
			pInfoFrame->CreateFrame(m_pManager, 3);
		}
		else if (_tcscmp(sName, R->kBtnRoomPanel_Back) == 0)
		{
			IPC* ipc = IPC::GetInstance();
			if (ipc->IsGameStarted())
			{
				DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ!"), _T("�����˳���Ϸ!"));
			}
			else
			{
				if (m_pRoomManagerRef)
				{
					ServerList* pServerList = ServerList::GetInstance();
#ifdef GAMEPLAZA_STYLE_1
					// ˢ�·����б�
					pServerList->RefreshList(m_pRoomManagerRef->GetPaintManager(), ServerList::RefreshType::RF_TYPE_ROOMLIST);

					// �˳�����(ɾ��Room����)
					m_pRoomManagerRef->ExitPanel(false);
#else if GAMEPLAZA_STYLE_2
					// ˢ�´�����Ϸ�б�
					pServerList->RefreshList(m_pRoomManagerRef->GetPaintManager(), ServerList::RefreshType::RF_TYPE_GAMELIST);

					// �˻ش���
					m_pRoomManagerRef->ExitPanel(true);
#endif
				}
			}
		}
		else if (_tcscmp(sName, R->kBtnRoomPanel_Join) == 0)
		{
			this->QuickJoin(true);
		}
		else if (_tcscmp(sName, R->kBtnRoomPanel_Enter) == 0)
		{
			this->QuickJoin(false);
		}
	}

	if (UserList::IsExistInstance())
	{
		UserList* pUserList = UserList::GetInstance();
		pUserList->Notify(msg);
	}
}

void Room::CreatTables()
{
	if (!m_pManager) return;
	g_Logger->TryInfo(_T("Room - CreatTables"));

	// �����������Ѵ���,�������
	if (m_pTableManager)
	{
		SAFE_DELETE(m_pTableManager);
	}

	// ��������
	m_pTableManager = new TableManager(m_pManager);
	m_pTableManager->CreatDeskPanel(m_wServerID, m_wKindID, m_wTableCount, m_wChairCount, IsHundredGame(), IsHundredLayout());

	// ���÷�������ı�
	this->RefreshTitleLabel();
}

void Room::RefreshTitleLabel(bool bShowOnline /* = true */)
{
	if (!m_pManager) return;
	CLabelUI* pLabTitleName = static_cast<CLabelUI*>(m_pManager->FindControl(R->kLabRoomPanel_TitleName));
	if (pLabTitleName)
	{
		if (bShowOnline)
		{
			_stprintf_s(m_pzBuffer, _T("[%s] - [%s] (%u��)"), this->GetGameName(), this->GetName(), this->GetOnlineCount());
		}
		else
		{
			_stprintf_s(m_pzBuffer, _T("[%s] - [%s]"), this->GetGameName(), this->GetName());
		}

		pLabTitleName->SetText(m_pzBuffer);
		TFontInfo* pFontInfo1 = m_pManager->GetFontInfo(pLabTitleName->GetFont());
		if (pFontInfo1)
		{
			int iWidth = pLabTitleName->GetText().GetLength() * pFontInfo1->iSize;
			iWidth /= 2;
			pLabTitleName->SetFixedWidth(iWidth);
		}
	}
}

void Room::UpdateTableStatus(tagGS_TableStatus& tStruct)
{
	if (m_pTableManager)
	{
		m_pTableManager->UpdateTableStatus(tStruct);
	}
}

void Room::UpdateTableStatus(WORD wTableID, tagTableStatus& tStruct)
{
	if (m_pTableManager)
	{
		m_pTableManager->UpdateTableStatus(wTableID, tStruct);
	}
}

bool Room::UpdateChairsStatus(UserData& uStruct)
{
	if (m_pTableManager)
	{
		Table* pTable = m_pTableManager->GetTable(uStruct);
		if (pTable)
		{
			return pTable->UpdateChairsUI(uStruct);
		}
	}

	return false;
}

void Room::SitDownOnTableReq(UserData& uStruct)
{
	g_Logger->TryInfo(_T("Room - SitDownOnTable Req"));
	if (m_pTableManager)
	{
		Table* pTable = m_pTableManager->GetTable(uStruct);
		if (pTable)
		{
			pTable->SitDownReq(uStruct.GetChairID());
		}
	}
}

void Room::StandUpOnTableReq(UserData& uStruct)
{
	g_Logger->TryInfo(_T("Room - StandUpOnTable Req"));
	if (m_pTableManager)
	{
		Table* pTable = m_pTableManager->GetTable(uStruct);
		if (pTable)
		{
			pTable->StandUpReq(uStruct.GetChairID());
		}
	}
}

void Room::QuickJoin(bool bAnimation)
{
	IPC* ipc = IPC::GetInstance();
	if (ipc->IsGameStarted())
	{
		DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ!"), _T("��������Ϸ��!"));
	}
	else
	{
		Table* pTable = m_pTableManager->GetFreeTable();
		if (pTable)
		{
			WORD wIndex = pTable->GetFreeChairIndex();
			if (INVALID_CHAIR != wIndex)
			{
				// ������������
				if (bAnimation)
				{
					m_pTableManager->QuickJoin(pTable, wIndex);
				}
				else
				{
					pTable->SitDownReq(wIndex);
				}
			}
			else
			{
				DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ!"), _T("��Ч����!"));
			}
		}
		else
		{
			DialogFrame::MsgBox(m_pManager, _T("��ܰ��ʾ!"), _T("δ�ҵ���Ч����!"));
		}
	}
}

void Room::QuickLocation(DWORD dwUserID)
{
	g_Logger->TryInfo(_T("Room - QuickLocation"));
	UserData* pUser = DataManager::GetInstance()->GetUserDataByID(dwUserID);
	if (!pUser) return;

	Table* pTable = m_pTableManager->GetTableByID(pUser->GetTableID());
	if (!pTable) return;
	
	if (m_pTableManager)
	{
		m_pTableManager->QuickJoin(pTable, pUser->GetChairID(), false);
	}
}

void Room::StartGames()
{
	g_Logger->TryInfo(_T("Room - StartGames Notify"));
	if (m_pTableManager)
	{
		UserData& pSelf = DataManager::GetInstance()->GetSelfData();
		Table* pTable = m_pTableManager->GetTable(pSelf);
		if (pTable)
		{
			pTable->StartGames(m_wKindID, pSelf);
		}
	}
}

void Room::CloseGameNotify(bool bEnforce /* = false */)
{
	g_Logger->TryInfo(_T("Room - CloseGame Notify [bEnforce = %u]"), bEnforce);
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& pSelf = pDataManager->GetSelfData();
	Table* pTable = m_pTableManager->GetTable(pSelf);
	if (pTable)
	{
		if (bEnforce)
		{
			pTable->LeftGameReq();
		}
		else
		{
			pTable->StandUpReq(pSelf.GetChairID());
		}
	}
}

void Room::ClearUserDataMap()
{
	// ������ݵ�
	// �����������
	DataManager* pManager = DataManager::GetInstance();
	pManager->CleanOtherUserMap();

	// ����������״̬
	UserData& tUser = pManager->GetSelfData();
	tUser.CleanStatus();
}

void Room::ClearUserListUI()
{
	if (UserList::IsExistInstance())
	{
		UserList* pUserList = UserList::GetInstance();
		pUserList->Release();
	}

	//���ù��ֹ�����ʼֵ
	CListUI* pListUI = static_cast<CListUI*>(m_pManager->FindControl(R->kLstChatPanel_UserList));
	if (pListUI)
	{
		pListUI->SetScrollPos({ 0, 0 });
	}
}

void Room::ResetChatUI()
{
	CRichEditUI* pViewEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(R->kEdtChatPanel_View));
	CRichEditUI* pSendEdit = static_cast<CRichEditUI*>(m_pManager->FindControl(R->kEdtChatPanel_Input));
	if (pViewEdit && pSendEdit)
	{
		pViewEdit->SetText(_T(""));
		pViewEdit->Clear();

		pSendEdit->SetText(_T(""));
		pSendEdit->Clear();
	}
}

void Room::ClearAll()
{
	g_Logger->TryInfo(_T("Room - Clean"));

	// ���õ������
	this->ResetPropsUI();

	// ������������
	this->ResetChatUI();

	// �������б����
	this->ClearUserListUI();

	// ���������ݱ�
	this->ClearUserDataMap();

	// �����Ϸ����״̬
	IPC* ipc = IPC::GetInstance();
	ipc->DisConnect(false);

	// ������ӹ�����
	SAFE_DELETE(m_pTableManager);

	g_Logger->TryInfo(_T("Room - Clean Complete"));
}