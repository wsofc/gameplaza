#include "stdafx.h"
#include "table.h"
#include "roomManager.h"
#include "..\\hall\\lockFrame.h"
#include "..\\room\\userControl.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

Table::Table()
{
	this->_Reset();
}

Table::Table(WORD wRoomID, WORD wKindID, WORD wTableID, WORD wIndex, CPaintManagerUI* paintManager, WORD wChairCount, bool bHundredGame, bool bHundredLayout)
{
	//��ʽ����Ա����
	this->_Reset();
	m_wRoomID			= wRoomID;
	m_wKindID			= wKindID;
	m_wTableID			= wTableID;
	m_wIndex			= wIndex;
	m_paintManager		= paintManager;
	m_wChairCount		= wChairCount;
	m_bHundredGame		= bHundredGame;
	m_bHundredLayout	= bHundredLayout;
	
	//��������UI
	this->_CreateTile();
}

Table::~Table()
{
	//�����λ����
	for (auto& it : m_ChairsArr)
	{
		SAFE_DELETE(it);
	}
	m_ChairsArr.clear();
}

void Table::_Reset()
{
	m_wRoomID			= 0;
	m_wKindID			= 0;
	m_wTableID			= INVALID_TABLE;
	m_wIndex			= INVALID_TABLE;
	m_wChairCount		= 0;
	m_bTableLock		= 0;
	m_bPlayStatus		= 0;
	m_pContainer		= nullptr;
	m_paintManager		= nullptr;
	m_bHundredLayout	= false;
	memset(m_buffer, 0, ArrayCount(m_buffer) * sizeof(TCHAR));
}

void Table::_CreateTile()
{
	if (!m_paintManager) return;

	RoomManager* pRoomManager = RoomManager::GetInstance();
	Room* pCurRoom = pRoomManager->GetCurRoom();
	if (!pCurRoom) return;

	CDuiString pzFileName;
	CDuiString pzSuffixName = _T("config.xml");

	ServiceData* pServiceData = ServiceData::GetInstance();
	tagGameKind* pGameKind = pServiceData->SearchGameKind(pCurRoom->GetKindID());
	if (pGameKind)
	{
		// ��ȡ��Ϸģ����(ȥ��.exe)
		CDuiString pzNoExtention = g_GameUtil->GetFileNameWithoutExtension(pGameKind->szClientModuleName).c_str();
		pzFileName.Format(_T("hall\\table\\style_%s\\"), pzNoExtention.GetData());

#ifdef SKIN_RES_TEST
		if (!g_GameUtil->IsFileExist(CPaintManagerUI::GetResourcePath() + pzFileName + pzSuffixName))
#else
		if (!CRenderEngine::IsZipItemExist(pzFileName + pzSuffixName))
#endif //SKIN_RES_TEST
		{
			pzFileName.Format(_T("hall\\table\\style_%u\\"), m_wChairCount);
		}

		m_path = pzFileName;
		pzFileName += pzSuffixName;
	}

	CDialogBuilder builder;
	m_pContainer = static_cast<CContainerUI*>(builder.Create(pzFileName.GetData(), (UINT)0, 0, m_paintManager));

	//��ʼ���ؼ�����
	if (m_pContainer)
	{
		this->_InitControlData();
	}
}

void Table::_InitControlData()
{
	/*
	*	��Ϊ������岻�ǵ����Ĵ���,ֱ��д��һ��cpp��Notify�������ӣ�
	*	����Ϊ�˰�������Ϣ�������������˱����������������õ��Ŀؼ�ͳһ���������
	*	ͳһΪ"table_���_�ؼ���" �Ա���ֱ�ӵ���PaintManager::FindControl��ȡ��Ӧ�ؼ�
	*/

	memset(m_buffer, 0, MAX_PATH * sizeof(TCHAR));
	ServiceData* pServerData = ServiceData::GetInstance();
	GameNormalTableConfig* pNormalTableCfg = pServerData->GetNormalTableConfig();

	//�������
	do
	{
		_stprintf_s(m_buffer, _T("table_x_index"));
		CTextUI* table_x_index = static_cast<CTextUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
		if (table_x_index)
		{
			_stprintf_s(m_buffer, _T("_%u_"), m_wIndex + 1);
			table_x_index->SetText(m_buffer);

			//�����ɫ
			if (pNormalTableCfg)
			{
				table_x_index->SetTextColor(pNormalTableCfg->dwIndexColor);
			}
		}
	} while (0);

	//���ӱ���
	do
	{
		if (m_bHundredLayout)
		{
			// ������
			CControlUI* pCtlRoomBgBlock = static_cast<CControlUI*>(m_paintManager->FindControl(R->kCtlRoomPanel_BgBlock));
			if (pCtlRoomBgBlock)
			{
				_stprintf_s(m_buffer, _T("file='%s%s' corner='10, 10, 10, 10'"), m_path.GetData(), _T("block.jpg"));
				pCtlRoomBgBlock->SetBkImage(m_buffer);
			}

			// ����
			CControlGdiplusUI* pCtlRoomBg = static_cast<CControlGdiplusUI*>(m_paintManager->FindControl(R->kCtlRoomPanel_Bg));
			if (pCtlRoomBg)
			{
				memset(m_buffer, 0, MAX_PATH * sizeof(TCHAR));
				_stprintf_s(m_buffer, _T("%s%s"), m_path.GetData(), _T("bg.jpg"));
				pCtlRoomBg->SetBkImage(m_buffer);
			}
			
			// ������Ϸ��ť
			CButtonUI* pEnterBtn = static_cast<CButtonUI*>(m_paintManager->FindSubControlByName(m_pContainer, R->kBtnRoomPanel_Enter));
			if (pEnterBtn)
			{
				_stprintf_s(m_buffer, _T("file='%s%s' aligncenter='true'"), m_path.GetData(), pEnterBtn->GetForeImage());
				pEnterBtn->SetForeImage(m_buffer);
			}
		}
		else
		{
			_stprintf_s(m_buffer, _T("table_x_bg"));
			CControlUI* table_x_bg = static_cast<CControlUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
			if (table_x_bg)
			{
				_stprintf_s(m_buffer, _T("table_%d_bg"), m_wIndex);
				table_x_bg->SetName(m_buffer);
				_stprintf_s(m_buffer, _T("%s%s"), m_path.GetData(), table_x_bg->GetBkImage());
				table_x_bg->SetBkImage(m_buffer);
			}
		}
	} while (0);

	//������
	do
	{
		_stprintf_s(m_buffer, _T("table_x_locker"));
		CControlUI* table_x_locker = static_cast<CControlUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
		if (table_x_locker)
		{
			_stprintf_s(m_buffer, _T("table_%d_locker"), m_wIndex);
			table_x_locker->SetName(m_buffer);

			if (pNormalTableCfg && _tcscmp(pNormalTableCfg->pzImgLock, _T("")) != 0)
			{
				table_x_locker->SetBkImage(pNormalTableCfg->pzImgLock);
			}
			else
			{
				_stprintf_s(m_buffer, _T("%s%s"), m_path.GetData(), table_x_locker->GetBkImage());
				table_x_locker->SetBkImage(m_buffer);
			}
		}
	} while (0);

	//��λ�ؼ�
	do
	{
		memset(m_buffer, 0, MAX_PATH * sizeof(TCHAR));
		for (int i = 0; i < m_wChairCount; ++i)
		{
			Chair* pChair = new Chair();
			m_ChairsArr.push_back(pChair);

			//��λ�û���Ϣ
			m_ChairsArr[i]->pUser	= nullptr;
			m_ChairsArr[i]->iIndex	= i;

			//��λ���ؼ�
			_stprintf_s(m_buffer, _T("table_x_seat_box_%d"), i);
			CButtonUI* table_seat_box_x = static_cast<CButtonUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
			if (table_seat_box_x)
			{
				_stprintf_s(m_buffer, _T("table_%d_seat_box_%d"), m_wIndex, i);
				table_seat_box_x->SetName(m_buffer);
				table_seat_box_x->SetBkImage(_T(""));	//��λ�����icon(Ĭ����ʾΪ��)

				m_ChairsArr[i]->scName = m_buffer;

				if (pNormalTableCfg && _tcscmp(pNormalTableCfg->pzImgHotBox, _T("")) != 0)
				{
					m_ChairsArr[i]->sBoxBg = pNormalTableCfg->pzImgHotBox;
				}
				else
				{
					m_ChairsArr[i]->sBoxBg = m_path + table_seat_box_x->GetHotImage();
				}
				table_seat_box_x->SetHotImage(m_ChairsArr[i]->sBoxBg);
			}

			//��λ����ͼ
			_stprintf_s(m_buffer, _T("table_x_seat_bg_%d"), i);
			CControlUI* table_seat_bg = static_cast<CControlUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
			if (table_seat_bg)
			{
				_stprintf_s(m_buffer, _T("table_%d_seat_bg_%d"), m_wIndex, i);
				table_seat_bg->SetName(m_buffer);
				m_ChairsArr[i]->sBg = table_seat_bg->GetBkImage();
				this->SetChairBg(m_ChairsArr[i]->iIndex, m_ChairsArr[i]->sBg);
			}

			//��λ׼��״̬ͼ
			_stprintf_s(m_buffer, _T("table_x_seat_%d_ready"), i);
			CControlUI* table_x_seat_ready = static_cast<CControlUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
			if (table_x_seat_ready)
			{
				if (pNormalTableCfg && _tcscmp(pNormalTableCfg->pzImgReady, _T("")) != 0)
				{
					table_x_seat_ready->SetBkImage(pNormalTableCfg->pzImgReady);
				}
				else
				{
					_stprintf_s(m_buffer, _T("%s%s"), m_path.GetData(), table_x_seat_ready->GetBkImage());
					table_x_seat_ready->SetBkImage(m_buffer);
				}

				table_x_seat_ready->SetVisible(false);
				_stprintf_s(m_buffer, _T("table_%d_seat_%d_ready"), m_wIndex, i);
				table_x_seat_ready->SetName(m_buffer);
				m_ChairsArr[i]->srName = m_buffer;
			}

			//��λ�ı��ؼ�
			_stprintf_s(m_buffer, _T("table_x_seat_%d_text"), i);
			CTextUI* table_x_seat_x_text = static_cast<CTextUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
			if (table_x_seat_x_text)
			{
				_stprintf_s(m_buffer, _T("table_%d_seat_%d_text"), m_wIndex, i);
				table_x_seat_x_text->SetName(m_buffer);
				table_x_seat_x_text->SetText(_T(""));			//��λ�������(Ĭ����ʾΪ��)
				table_x_seat_x_text->SetToolTip(_T(""));		//��λ���������ʾ
				m_ChairsArr[i]->stName = m_buffer;

				//��λ�������ɫ
				if (pNormalTableCfg)
				{
					table_x_seat_x_text->SetTextColor(pNormalTableCfg->dwPlayerColor);
				}
			}
		}
	} while (0);
}

void Table::_SetPlayVisible(bool bVisible)
{
	memset(m_buffer, 0, MAX_PATH * sizeof(TCHAR));
	_stprintf_s(m_buffer, _T("table_%u_bg"), m_wIndex);
	CControlUI* table_x_bg = static_cast<CControlUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
	if (table_x_bg)
	{
		if (TRUE == bVisible)
		{
			_stprintf_s(m_buffer, _T("%s%s"), m_path.GetData(), _T("TablePlaying.png"));
		}
		else
		{
			_stprintf_s(m_buffer, _T("%s%s"), m_path.GetData(), _T("TableNormal.png"));
		}
		
		table_x_bg->SetBkImage(m_buffer);
	}
}

void Table::_SetLockVisible(bool bVisible)
{
	memset(m_buffer, 0, ArrayCount(m_buffer) * sizeof(TCHAR));
	_stprintf_s(m_buffer, _T("table_%u_locker"), m_wIndex);
	CControlUI* table_x_locker = static_cast<CControlUI*>(m_paintManager->FindSubControlByName(m_pContainer, m_buffer));
	if (table_x_locker)
	{
		table_x_locker->SetVisible(bVisible);
	}
}

void Table::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	if (_tcscmp(sType, DUI_MSGTYPE_CLICK) == 0)
	{
		// ���"����"������Ϸ
		for (size_t i = 0; i < m_wChairCount; ++i)
		{
			if (_tcscmp(sName, m_ChairsArr[i]->scName) == 0)
			{
				if (IsEmptyChair(i))
				{
					UserData& tSelf = DataManager::GetInstance()->GetSelfData();
					
					// �Ӹ�������Ϸ(����״̬Ҳ����Ϸ��)���ж�
					// ��ֹ��������Ӧ����ʱ,���²�����Ϸ����Ҳ�ܻ���
					bool bFishGame = tSelf.GetGameID() == 30 || tSelf.GetGameID() == 31;
					bool bFishGameChangeTable = bFishGame && (tSelf.GetUserStatus() == US_SIT);

					if (tSelf.IsGaming() || tSelf.IsUserReady() || bFishGameChangeTable)
					{
						DialogFrame::MsgBox(m_paintManager, _T("��ܰ��ʾ"), _T("��������Ϸ�У������˳���Ϸ!"));
					}
					else
					{
						SitDownReq(i);
					}
				}
				else
				{
					DialogFrame::MsgBox(m_paintManager, _T("��ܰ��ʾ"), _T("��λ���Ѿ����˽����ȵ���!"));
				}

				break;
			}
		}
	}
	else if (_tcscmp(sType, DUI_MSGTYPE_RCLICK) == 0)
	{
		for (size_t i = 0; i < m_wChairCount; ++i)
		{
			if (_tcscmp(sName, m_ChairsArr[i]->scName) == 0)
			{
				if (!IsEmptyChair(i))
				{
					UserControl* pUserControl = UserControl::GetInstance();
					pUserControl->ShowMenu(m_ChairsArr[i]->pUser);
				}
				else
				{
					// ������Ϊ��
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
// Chair
void Table::SetChairBg(WORD wChairIndex, CDuiString sBg)
{
	if (wChairIndex > m_ChairsArr.size()) return;

	m_ChairsArr[wChairIndex]->sBg = sBg;
	memset(m_buffer, 0, MAX_PATH * sizeof(TCHAR));
	_stprintf_s(m_buffer, _T("table_%u_seat_bg_%u"), m_wIndex, wChairIndex);
	CControlUI* table_seat_bg = m_paintManager->FindSubControlByName(m_pContainer, m_buffer);
	if (table_seat_bg)
	{
		table_seat_bg->SetBkImage(m_path + sBg);
	}
}

const CDuiString Table::GetChairBg(WORD wChairIndex)
{
	if (wChairIndex > m_ChairsArr.size()) return _T("");

	return m_ChairsArr[wChairIndex]->sBg;
}

void Table::SetChairIcon(WORD wChairIndex, CDuiString sIcon)
{
	if (wChairIndex > m_ChairsArr.size()) return;
	
	CDuiString& stName = m_ChairsArr[wChairIndex]->scName;
	CButtonUI* seat_x = static_cast<CButtonUI*>(m_paintManager->FindSubControlByName(m_pContainer, stName));
	if (seat_x)
	{
		seat_x->SetBkImage(sIcon);
	}
}

const CDuiString Table::GetChairIcon(WORD wChairIndex)
{
	if (wChairIndex > m_ChairsArr.size()) return _T("");

	UserData* pUser = m_ChairsArr[wChairIndex]->pUser;
	if (pUser)
		return pUser->GetIcon();
	return _T("");
}

void Table::SetChairReady(WORD wChairIndex, bool bReady)
{
	if (wChairIndex > m_ChairsArr.size()) return;

	CDuiString& srName = m_ChairsArr[wChairIndex]->srName;
	CButtonUI* seat_x_ready = static_cast<CButtonUI*>(m_paintManager->FindSubControlByName(m_pContainer, srName));
	if (seat_x_ready)
	{
		seat_x_ready->SetVisible(bReady);
	}
}

const bool Table::GetChairReady(WORD wChairIndex)
{
	bool bRes = false;
	if (wChairIndex > m_ChairsArr.size()) return bRes;

	CDuiString& srName = m_ChairsArr[wChairIndex]->srName;
	CButtonUI* seat_x_ready = static_cast<CButtonUI*>(m_paintManager->FindSubControlByName(m_pContainer, srName));
	if (seat_x_ready)
	{
		bRes = seat_x_ready->IsVisible();
	}
	return bRes;
}

void Table::SetChairText(WORD wChairIndex, CDuiString sText)
{
	if (wChairIndex > m_ChairsArr.size()) return;

	CDuiString& stName = m_ChairsArr[wChairIndex]->stName;
	CTextUI* seat_text_x = static_cast<CTextUI*>(m_paintManager->FindSubControlByName(m_pContainer, stName));
	if (seat_text_x)
	{
		seat_text_x->SetText(sText);
		seat_text_x->SetToolTip(sText);
	}
}

const CDuiString Table::GetChairText(WORD wChairIndex)
{
	if (wChairIndex > m_ChairsArr.size()) return _T("");

	UserData* pUser = m_ChairsArr[wChairIndex]->pUser;
	if (pUser)
	{
		return pUser->GetNickName();
	}
	return _T("");
}

bool Table::IsEmptyTable()
{
	bool bRes = true;
	for (const auto&it : m_ChairsArr)
	{
		bRes &= !it->pUser;
	}

	return bRes;
}

bool Table::IsFreeTable()
{
	bool bRes = false;
	if (FALSE == m_bTableLock)
	{
		// 1.���Ӵ��ڷ���Ϸ״̬
		// 2.������Ϊ��ռ����������(���Ӳ���30, ҡǮ��31)
		// 3.������Ϊ������Ϸ��������
		// ��������֮һ����������
		if (m_bPlayStatus == FALSE || (m_wKindID == 30 || m_wKindID == 31) || IsHundredLayout())
		{
			for (const auto&it : m_ChairsArr)
			{
				if (!it->pUser)
				{
					bRes = true;
					break;
				}
			}
		}
	}

	return bRes;
}

bool Table::IsEmptyChair(WORD wChairIndex)
{
	bool bRes = false;
	if (wChairIndex >= m_ChairsArr.size()) return bRes;

	bRes = !m_ChairsArr[wChairIndex]->pUser;
	return bRes;
}

WORD Table::GetFreeChairIndex()
{
	WORD wIndex = INVALID_CHAIR;

	for (int i = 0; i < m_ChairsArr.size(); ++i)
	{
		if (m_ChairsArr[i] && !m_ChairsArr[i]->pUser)
		{
			wIndex = m_ChairsArr[i]->iIndex;
			break;
		}
	}

	return wIndex;
}

Chair* Table::GetChairByIndex(WORD wChairIndex)
{
	if (wChairIndex >= m_ChairsArr.size()) return false;

	return m_ChairsArr[wChairIndex];
}

void Table::SitDownReq(WORD wChairIndex, CDuiString pzPassword /* = _T("") */)
{
	g_Logger->TryInfo(_T("Table - SitDown [cid = %u, count = %u]"), wChairIndex, m_ChairsArr.size());

	if (wChairIndex >= m_ChairsArr.size())
	{
		_stprintf_s(m_buffer, _T("��Ч��λ(%u,%u)"), wChairIndex, m_ChairsArr.size());
		DialogFrame::MsgBox(m_paintManager, _T("��ܰ��ʾ"), m_buffer);
	}
	else if (nullptr == m_ChairsArr[wChairIndex]->pUser)
	{
		DataManager* pDataMgr = DataManager::GetInstance();
		UserData& tSelf = pDataMgr->GetSelfData();
		if (TRUE == m_bTableLock && pzPassword.GetLength() <= 0 && tSelf.GetUserStatus() != US_SIT)
		{
			LockTableFrame* pTableLock = new LockTableFrame();
			pTableLock->CreateFrame(this, wChairIndex);
		}
		else
		{
			// ������������
			CMD_GR_UserSitReq pStruct;
			memset(&pStruct, 0, sizeof(pStruct));

			pStruct.wTableID = m_wTableID;
			pStruct.wChairID = wChairIndex;
			
			::StrToCharArray(pStruct.szTablePass, pzPassword.GetData());
			pStruct.cbPassLen = CountStringBuffer(pStruct.szTablePass);
			WORD wSendSize = sizeof(pStruct) - sizeof(pStruct.szTablePass) + pStruct.cbPassLen;

			g_Logger->TryInfo(_T("Table - SitDown Req [chair_id = %u]"), wChairIndex);
			RoomManager* roomManager = RoomManager::GetInstance();
			DWORD iRes = roomManager->SendData(MDM_GR_USER, SUB_GR_USER_SIT_REQ, &pStruct, wSendSize);
			if (iRes == FALSE)
			{
				DialogCallBack dlg_cb = CC_CALLBACK_2(Table::MsgBoxProcFocus, this);
				DialogFrame::MsgBox(m_paintManager
					, _T("��ܰ��ʾ:")
					, _T("���������ѶϿ�,�����½��뷿��!")
					, DLG_ID(ServerBase::ServerBaseMsxBoxID::MSG_BOX_ID_NET_DISCONNECTION)
					, DLG_STYLE::DLG_OK
					, dlg_cb
					, dlg_cb
					, dlg_cb);
			}
		}
	}
	else
	{
		DialogFrame::MsgBox(m_paintManager, _T("��ܰ��ʾ"), _T("��λ���Ѿ����˽����ȵ���!"));
	}
}

void Table::StandUpReq(WORD wChairIndex)
{
	g_Logger->TryInfo(_T("Table - StandUp"));

	if (wChairIndex < m_wChairCount)
	{
		if (nullptr != m_ChairsArr[wChairIndex]->pUser)
		{
			g_Logger->TryInfo(_T("Table - StandUp Req [chair_id = %u]"), wChairIndex);

			// ������������
			RoomManager* roomManager = RoomManager::GetInstance();
			DWORD iRes = roomManager->SendData(MDM_GR_USER, SUB_GR_USER_STANDUP_REQ);
		}
	}
}

void Table::UpdateTableStatus()
{
	this->_SetPlayVisible(m_bPlayStatus);
	this->_SetLockVisible(m_bTableLock);
}

bool Table::UpdateChairsUI(UserData& uStruct)
{
	bool bRes = false;

	WORD wTableID = uStruct.GetTableID();
	WORD wChairID = uStruct.GetChairID();

	// ��Ϊ��Ч��,��Ч����(����ʱ)
	if ((INVALID_TABLE == wTableID) || (INVALID_CHAIR == wChairID))
	{
		for (size_t i = 0; i < m_ChairsArr.size(); ++i)
		{
			UserData* pUser = m_ChairsArr[i]->pUser;
			if (pUser && (pUser->GetUserID() == uStruct.GetUserID()))
			{
				//�˴�ָ��Ϊ����,����delete
				m_ChairsArr[i]->pUser = nullptr;
				SetChairIcon(i, _T(""));
				SetChairText(i, _T(""));
				SetChairReady(i, uStruct.IsUserReady());

				bRes = true;
				break;
			}
		}
	}
	// ��Ч��ʱ
	else
	{
		if (wChairID >= 0 && wChairID < m_wChairCount)
		{
			// ����������ݽṹ����������(������һ��Ҫ���)
			uStruct.SetLastTableID(wTableID);
			uStruct.SetLastChairID(wChairID);

			m_ChairsArr[wChairID]->pUser = &uStruct;
			SetChairIcon(wChairID, uStruct.GetIcon());
			SetChairText(wChairID, uStruct.GetNickName());
			SetChairReady(wChairID, uStruct.IsUserReady());

			bRes = true;
		}
	}
	
	return bRes;
}

void Table::StartGames(WORD wKindID, UserData& uStruct)
{
	ServiceData* pSeviceData = ServiceData::GetInstance();
	tagGameKind* pGameKind = pSeviceData->SearchGameKind(wKindID);
	if (pGameKind)
	{
		IPC* ipc = IPC::GetInstance();
		ipc->StartGames(m_paintManager, pGameKind->szKindName, pGameKind->szClientModuleName);
	}
}

bool Table::LeftGameReq()
{
	g_Logger->TryInfo(_T("Table - LeftGame Req"));

	RoomManager* roomManager = RoomManager::GetInstance();
	DWORD iRes = roomManager->SendData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ);
	return iRes == TRUE;
}

void Table::MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg)
{
	switch (id)
	{
		case ServerBase::ServerBaseMsxBoxID::MSG_BOX_ID_NET_DISCONNECTION:
		{
			IPC* ipc = IPC::GetInstance();
			ipc->DisConnect(false);

			RoomManager* pRoomManager = RoomManager::GetInstance();
			pRoomManager->ExitPanel(false);
		}break;

		default:
			break;
	}
}