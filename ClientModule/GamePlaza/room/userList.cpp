#include "stdafx.h"
#include "userList.h"

extern GameUtil*		g_GameUtil;
extern GameResource*	R;
extern CStaticLogger	g_Logger;

UserList* UserList::g_instance = nullptr;
UserList::UserList()
	: m_pList(nullptr)
	, m_pUserControl(nullptr)
	, m_bDistinguish(false)
{
	::ZeroMemory(m_pzBuffer, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
	m_pUserControl = UserControl::GetInstance();

	g_Logger->TryInfo(_T("UserList - Create"));
}

UserList::~UserList()
{
	g_Logger->TryInfo(_T("UserList - Destroy"));
}

UserList* UserList::GetInstance()
{
	if (!UserList::g_instance)
	{
		UserList::g_instance = new (std::nothrow)UserList();
	}
	return UserList::g_instance;
}

void UserList::Release()
{
	// 卸载玩家控件预处理
	// 正真的卸载Dll在退出房间之后通过PostMessage消息通知清理
	if (m_pUserControl)
	{
		m_pUserControl->PreUnLoadUserMenuDll();
		m_pUserControl = nullptr;
	}
	
	this->Clean();
	SAFE_DELETE(UserList::g_instance);
}

void UserList::InsertSelfDefineHeadItem()
{
#ifdef DEBUG || _DEBUG
	// 添加第0列:用户状态
	tagColumnItem* pHeadItem_0 = new tagColumnItem();
	memset(pHeadItem_0, 0, sizeof(tagColumnItem));
	pHeadItem_0->wColumnWidth = 45;
	pHeadItem_0->wDataDescribe = DTP_USER_STATUS;
	this->InsertHeadItem(pHeadItem_0);
#endif

	// 添加第1列:用户会员
	tagColumnItem* pHeadItem_1 = new tagColumnItem();
	memset(pHeadItem_1, 0, sizeof(tagColumnItem));
	pHeadItem_1->wColumnWidth = 25;
	pHeadItem_1->wDataDescribe = DTP_MEMBERORDER;
	this->InsertHeadItem(pHeadItem_1);

	// 添加第2列:用户头像
	tagColumnItem* pHeadItem_2 = new tagColumnItem();
	memset(pHeadItem_2, 0, sizeof(tagColumnItem));
	pHeadItem_2->wColumnWidth = 25;
	pHeadItem_2->wDataDescribe = DTP_USER_FACE;
	this->InsertHeadItem(pHeadItem_2);
}

bool UserList::InsertHeadItem(tagColumnItem* pColumItem)
{
	ASSERT(pColumItem != nullptr);
	if (!pColumItem) return false;

	for (const auto& it : m_vHeadArry)
	{
		if (it->wDataDescribe == pColumItem->wDataDescribe)
			return true;
	}

	tagColumnItem* pHeadItem = nullptr;
	pHeadItem = new tagColumnItem;
	::memcpy(pHeadItem, pColumItem, sizeof(tagColumnItem));
	m_vHeadArry.push_back(pHeadItem);

	return true;
}

tagColumnItem* UserList::EnumHeadItem(DWORD& iNextKey)
{
	tagColumnItem* pHeadItem = nullptr;
	if (iNextKey >= 0 && iNextKey < m_vHeadArry.size())
	{
		pHeadItem = m_vHeadArry[iNextKey];

		// iNextKey往后偏移
		++iNextKey;
	}
	return pHeadItem;
}

bool UserList::RemoveHeadItem(WORD wDataDescribe)
{
	bool bRes = false;
	tagColumnItem* pHeadItem = nullptr;
	std::vector<tagColumnItem*>::iterator it;
	size_t i = 0;
	for (it = m_vHeadArry.begin(), i = 0; it != m_vHeadArry.end(); ++i)
	{
		if ((*it)->wDataDescribe == wDataDescribe)
		{
			if (m_pList)
			{
				CListHeaderUI* pHeaderUI = m_pList->GetHeader();
				if (pHeaderUI)
				{
					pHeaderUI->RemoveAt(i);
				}
			}

			tagColumnItem* pTagItem = *it;
			SAFE_DELETE(pTagItem);
			
			m_vHeadArry.erase(it++);
			bRes = true;

			break;
		}
	}

	return bRes;
}

void UserList::RemovaAllHeadItem()
{
	std::vector<tagColumnItem*>::iterator it;
	for (it = m_vHeadArry.begin(); it != m_vHeadArry.end(); ++it)
	{
		tagColumnItem* pTagItem = *it;
		SAFE_DELETE(pTagItem);
	}
	m_vHeadArry.clear();

	if (m_pList)
	{
		CListHeaderUI* pHeaderUI = m_pList->GetHeader();
		if (pHeaderUI)
		{
			pHeaderUI->RemoveAll();
		}
	}
}

CListHeaderItemUI* UserList::CreatFixedHeadItem(WORD wIndex, WORD wColumnWidth, bool bSep /* = true */)
{
	TCHAR pzName[MAX_PATH] = { 0 };
	CListHeaderItemUI* pHeadItem = nullptr;
	_stprintf_s(pzName, _T("sidebar_table-list_head_btn%u"), wIndex);

	pHeadItem = new CListHeaderItemUI();
	pHeadItem->SetName(pzName);
	pHeadItem->SetText(_T(""));
	pHeadItem->SetFont(3);
	pHeadItem->SetMinWidth(LISTHEAD_MIN_WIDTH);
	pHeadItem->SetBkImage(_T("file='common/list/bg.png' corner='4,4,4,4'"));
	pHeadItem->SetHotImage(_T("file='common/button/btn_hot.png' corner='4,4,4,4'"));
	pHeadItem->SetPushedImage("file='common/button/btn_push.png' corner='4,4,4,4'");
	pHeadItem->SetEnabled(false);
	pHeadItem->SetDragable(false);

	if (bSep)
	{
		pHeadItem->SetFixedWidth(wColumnWidth);
		pHeadItem->SetSepWidth(LISTHEAD_SEP_WIDTH);
		pHeadItem->SetSepImage(_T("file='common/list/sep.png' aligncenter='true'"));
	}

	return pHeadItem;
}

CListHeaderItemUI* UserList::CreatFloatHeadItem(WORD wIndex, tagColumnItem& item)
{
	DWORD dwTextcolor = 0xFFFFFFFF;
	ServiceData* pServiceData = ServiceData::GetInstance();
	GameFontColor* pFontColor = pServiceData->GetFontColorByID(6);
	if (pFontColor)
	{
		dwTextcolor = pFontColor->dwTextcolor;
	}

	TCHAR pzName[MAX_PATH] = { 0 };
	CListHeaderItemUI* pHeadItem = nullptr;
	_stprintf_s(pzName, _T("sidebar_table-list_head_btn%u"), wIndex);

	pHeadItem = new CListHeaderItemUI();
	pHeadItem->SetName(pzName);
	pHeadItem->SetText(item.szColumnName);
	pHeadItem->SetFixedWidth(item.wColumnWidth);
	pHeadItem->SetSepWidth(LISTHEAD_SEP_WIDTH);
	pHeadItem->SetFont(3);
	pHeadItem->SetTextColor(dwTextcolor);
	pHeadItem->SetMinWidth(LISTHEAD_MIN_WIDTH);
	pHeadItem->SetBkImage(_T("file='common/list/bg.png' corner='4,4,4,4'"));
	pHeadItem->SetHotImage(_T("file='common/button/btn_hot.png' corner='4,4,4,4'"));
	pHeadItem->SetPushedImage("file='common/button/btn_push.png' corner='4,4,4,4'");
	pHeadItem->SetSepImage(_T("file='common/list/sep.png' aligncenter='true'"));

	return pHeadItem;
}

void UserList::LoadHeaderItem(CListUI* pList)
{
	if (!pList) return;
	m_pList = pList;
	m_pList->SetTextCallback(this);
	m_pList->GetHeader()->RemoveAll();
	m_pList->RemoveAll();

	// 加载 head
	WORD wIndex = 0;
	CListHeaderItemUI* pHeadItem = nullptr;
	for (size_t i = 0; i < m_vHeadArry.size(); ++i)
	{
		switch (m_vHeadArry[i]->wDataDescribe)
		{
			case DTP_USER_STATUS:
			{
				pHeadItem = CreatFixedHeadItem(wIndex++, 40);
			}break;

			case  DTP_MEMBERORDER:
			{
				pHeadItem = CreatFixedHeadItem(wIndex++, 25);
			}break;

			case DTP_USER_FACE:
			{
				pHeadItem = CreatFixedHeadItem(wIndex++, 25);
			}break;

			default:
			{
				pHeadItem = CreatFloatHeadItem(wIndex++, *m_vHeadArry[i]);
			}break;
		}
		
		m_pList->Add(pHeadItem);
	}

	// 若是管理员,则优先显示普通玩家(与机器人有显著的区别)
	DataManager* pDataManager = DataManager::GetInstance();
	UserData& tSelf = pDataManager->GetSelfData();
	if (tSelf.GetMasterOrder() > 0 || tSelf.IsUnrealUser())
	{
		m_bDistinguish = true;
	}
}

void UserList::InsertBodyItem(WORD wFixedHeight, UserData* pUser, UserData* pSelf /* = nullptr */)
{
	if (!m_pList ||	!pUser) return;
	if (!pSelf)
	{
		DataManager* pDataManager = DataManager::GetInstance();
		UserData& tSelf = pDataManager->GetSelfData();
		pSelf = &tSelf;
	}

	// 先检测一遍,避免重复添加(主要针对多次重复调用时生效)
	for (int i = 0; i < m_vBodyArry.size(); ++i)
	{
		if (m_vBodyArry[i]->GetUserID() == pUser->GetUserID())
		{
			return;
		}
	}

	// 插入玩家(隐藏权限等级高于自身的玩家)
	bool bHide = false;
	BYTE cbUserMOrder = pUser->GetMasterOrder();
	BYTE cbSelfMOrder = pSelf->GetMasterOrder();
	if (pUser->GetUserID() != pSelf->GetUserID() && !pSelf->IsUnrealUser())
	{
		if (cbUserMOrder > cbSelfMOrder && cbUserMOrder > 0)
		{
			bHide = true;
		}
	}
	if (bHide) return;
	
	// 添加玩家数据
	do
	{
		bool bPushBack = true;
		BYTE cbUserMasterOrder = pUser->GetMasterOrder();
		BYTE cbUserMemberOrder = pUser->GetMemberOrder();
		BYTE cbBodyMasterOrder = 0;
		BYTE cbBodyMemberOrder = 0;
		for (int i = 0; i < m_vBodyArry.size(); ++i)
		{
			cbBodyMasterOrder = m_vBodyArry[i]->GetMasterOrder();
			cbBodyMemberOrder = m_vBodyArry[i]->GetMemberOrder();
			
			// 自身只插入一次(因为服务器会广播自己两次,所以避免重复插入)
			if (m_vBodyArry[i]->GetUserID() == pUser->GetUserID())
			{
				return;
			}

			// 优先插入管理员(降序)
			else if (pSelf->IsUnrealUser() && cbUserMasterOrder > 0 && cbUserMasterOrder > cbBodyMasterOrder && !m_vBodyArry[i]->IsSelf())
			{
				bPushBack = false;
				m_vBodyArry.insert(m_vBodyArry.begin() + i, pUser);
				break;
			}

			// 优先插入会员等级高的玩家(管理员 > 玩家 > 会员机器人 > 普通机器人)
			else if (cbUserMemberOrder > cbBodyMemberOrder && !m_vBodyArry[i]->IsSelf())
			{
				if (pSelf->IsUnrealUser() && (cbUserMasterOrder < cbBodyMasterOrder || (pUser->IsRobot() && !m_vBodyArry[i]->IsRobot())))
				{
					continue;
				}
				else if (pSelf->GetMasterOrder() > 0 && (pUser->IsRobot() && !m_vBodyArry[i]->IsRobot()))
				{
					continue;
				}

				bPushBack = false;
				m_vBodyArry.insert(m_vBodyArry.begin() + i, pUser);
				break;
			}

			// 管理员模式(优先插入真实玩家)
			else if ((pSelf->GetMasterOrder() > 0 || pSelf->IsUnrealUser()) && !pUser->IsRobot() && m_vBodyArry[i]->IsRobot())
			{
				bPushBack = false;
				m_vBodyArry.insert(m_vBodyArry.begin() + i, pUser);
				break;
			}

			// 插入到离线玩家之前
			else if (m_vBodyArry[i]->GetUserStatus() == US_OFFLINE)
			{
				bPushBack = false;
				m_vBodyArry.insert(m_vBodyArry.begin() + i, pUser);
				break;
			}
		}

		// 以上条件都不满足,则插入末尾
		if (bPushBack)
		{
			m_vBodyArry.push_back(pUser);
		}
	} while (0);

	// 第一位显示自己
	do
	{
		// 列表中提取玩家自己(并在列表中移除对自身的引用)
		UserData* pSelf = nullptr;
		for (int i = 0; i < m_vBodyArry.size(); ++i)
		{
			if (m_vBodyArry[i]->IsSelf() && i != 0)
			{
				pSelf = m_vBodyArry[i];
				m_vBodyArry.erase(m_vBodyArry.begin() + i);
				break;
			}
		}

		// 自身插入第一位
		if (pSelf)
		{
			m_vBodyArry.insert(m_vBodyArry.begin() + 0, pSelf);
		}

	} while (0);

	// 添加玩家列表
	CListTextElementUI* pListElement = new CListTextElementUI();
	pListElement->SetFixedHeight(wFixedHeight);
	m_pList->Add(pListElement);
	
	// 自定义排序(如金币、魅力值等)
	this->SortBodyItem(false);
}

void UserList::RemoveBodyItem(DWORD wUserID)
{
	size_t iIndex = 0;
	std::vector<UserData*>::iterator it;
	for (it = m_vBodyArry.begin(); it != m_vBodyArry.end();)
	{
		if (m_vBodyArry[iIndex]->GetUserID() == wUserID)
		{
			m_pList->RemoveAt(iIndex);
			m_vBodyArry.erase(it++);
			break;
		}

		++iIndex;
		++it;
	}
}

void UserList::RemoveAllBodyItem()
{
	if (m_pList)
	{
		m_pList->RemoveAll();
	}

	m_vBodyArry.clear();
}

void UserList::Clean()
{
	g_Logger->TryInfo(_T("UserList - Clean"));
	RemovaAllHeadItem();
	RemoveAllBodyItem();
	g_Logger->TryInfo(_T("UserList - Clean Complete"));
}

bool UserList::SortItemFunc(UserData* item_a, UserData* item_b)
{
	bool bRes = false;

	if (item_a->GetMemberOrder() > item_b->GetMemberOrder())
	{
		bRes = true;
	}
	else if (item_a->GetMemberOrder() == item_b->GetMemberOrder())
	{
		bRes = item_a->GetGameGold() > item_b->GetGameGold();
	}

	return bRes;
}

void UserList::SortBodyItem(bool bOpen /* = true */)
{
	if (bOpen && m_pList)
	{
		if (m_vBodyArry.size() > 1)
		{
			// std::sort排序的坑(比较==时必须返回false，否则进入死循环)
			std::sort(++m_vBodyArry.begin(), m_vBodyArry.end(), UserList::SortItemFunc);
		}

		m_pList->Invalidate();
	}
}

void UserList::Update(UserData* pTagUser /* = nullptr */)
{
	if (m_pList)
	{
		m_pList->Invalidate();
	}
}

LPCTSTR UserList::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	::ZeroMemory(m_pzBuffer, ArrayCount(m_pzBuffer) * sizeof(TCHAR));

	// 判断是否越界
	if (iIndex < 0 || iIndex >= m_vBodyArry.size()) return m_pzBuffer;

	if (pControl->GetParent()->GetParent()->GetName() == R->kLstChatPanel_UserList)
	{
		UINT uFont			= 1;													// 字体索引
		UINT cbMemberOrder	= m_vBodyArry[iIndex]->GetMemberOrder();				// 会员等级
		DWORD dwTxtColor	= m_vBodyArry[iIndex]->GetMemberOrderColor();			// 文子颜色
		bool bGray			= m_vBodyArry[iIndex]->GetUserStatus() == US_OFFLINE;	// 灰度显示
		LPCTSTR pzGray		= bGray ? _T("true") : _T("false");

		// 真实玩家优先会员色，非会员则标记为红色
		if (m_bDistinguish && !m_vBodyArry[iIndex]->IsRobot() && cbMemberOrder < 1)
		{
			dwTxtColor = 0xFFFF0000;
		}

		// 离线文字灰色
		dwTxtColor = bGray ? 0XFFA0A0A4 : dwTxtColor;

		// 从服务器读取的列表数据
		if(iSubItem >= 0 && iSubItem < m_vHeadArry.size())
		{
			WORD w_index = iSubItem;
			switch (m_vHeadArry[w_index]->wDataDescribe)
			{
				// 用户状态
				case DTP_USER_STATUS:
				{
					BYTE cbStatus = m_vBodyArry[iIndex]->GetUserStatus();
					CDuiString pzDesc;
					switch (cbStatus)
					{
						case US_NULL:		pzDesc = _T("无");		break;
						case US_FREE:		pzDesc = _T("站立");	break;
						case US_SIT:		pzDesc = _T("坐下");	break;
						case US_READY:		pzDesc = _T("准备");	break;
						case US_LOOKON:		pzDesc = _T("旁观");	break;
						case US_PLAY:		pzDesc = _T("游戏");	break;
						case US_OFFLINE:	pzDesc = _T("断线");	break;
						default:			pzDesc = _T("无");		break;
					}
					_stprintf_s(m_pzBuffer, _T("<text content='%u-%s' font='%u' textcolor='#%lu'>"), cbStatus, pzDesc.GetData(), uFont, dwTxtColor);
				}break;

				// 会员图标
				case DTP_MEMBERORDER:
				{
					if (cbMemberOrder > 0)
					{
						_stprintf_s(m_pzBuffer, _T("<bkimage file='common/list/member_%u.png' aligncenter='true' gray='%s'>"), cbMemberOrder, pzGray);
					}
				}break;

				case DTP_USER_FACE:
				{
					WORD wFaceID = m_vBodyArry[iIndex]->GetFaceID();
					LPCTSTR pzIcon = m_vBodyArry[iIndex]->GetIcon();
					_stprintf_s(m_pzBuffer, _T("<bkimage file='%s' aligncenter='true' gray='%s'>"), pzIcon, pzGray);
				}break;

				// 用户 I D
				case DTP_USER_ID:
				{
					WORD wShowID = m_vBodyArry[iIndex]->GetGameID();
					_stprintf_s(m_pzBuffer, _T("<text content='%u' font='%u' textcolor='#%lu'>"), wShowID, uFont, dwTxtColor);
				}break;

				// 游戏 I D
				case DTP_GAME_ID:
				{
					WORD wGameID = m_vBodyArry[iIndex]->GetGameID();
					_stprintf_s(m_pzBuffer, _T("<text content='%u' font='%u' textcolor='#%lu'>"), wGameID, uFont, dwTxtColor);
				}break;

				// 昵称
				case DTP_NICKNAME:
				{
					LPCTSTR pzNickName = m_vBodyArry[iIndex]->GetNickName();
					_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>"), pzNickName, uFont, dwTxtColor);
				}break;

				// 游戏金币
				case DTP_GAME_GOLD:
				{
					LONGLONG lGameGold = m_vBodyArry[iIndex]->GetGameGold();
					_stprintf_s(m_pzBuffer, _T("<text content='%lld' font='%u' textcolor='#%lu'>"), lGameGold, uFont, dwTxtColor);
				}break;

				// 用户分数
				case DTP_USER_SCORE:
				{
					LONGLONG lGameScore = m_vBodyArry[iIndex]->GetScore();
					_stprintf_s(m_pzBuffer, _T("<text content='%lld' font='%u' textcolor='#%lu'>"), lGameScore, uFont, dwTxtColor);
				}break;

				// 游戏等级描述
				case DTP_GAME_LEVEL:
				{
					LONGLONG lScore = m_vBodyArry[iIndex]->GetScore();
					DataManager* pDataManager = DataManager::GetInstance();
					CDuiString pzScoreDesc = pDataManager->GetLevelDescByScore(lScore);
					_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>"), pzScoreDesc.GetData(), uFont, dwTxtColor);
				}break;

				// 桌号
				case DTP_USER_TABLE:
				{
					::ZeroMemory(m_pzBuffer, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
					WORD wTableID = m_vBodyArry[iIndex]->GetTableID();
					if (INVALID_TABLE != wTableID)
					{
						_stprintf_s(m_pzBuffer, _T("<text content='%u' font='%u' textcolor='#%lu'>"), wTableID + 1, uFont, dwTxtColor);
					}
					else
					{
						_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>"), _T("-"), uFont, dwTxtColor);
					}
				}break;

				// 用户权限
				case DTP_USER_RIGHT:
				{
					DWORD dUserRight = m_vBodyArry[iIndex]->GetUserRight();
					_stprintf_s(m_pzBuffer, _T("<text content='%lu' font='%u' textcolor='#%lu'>"), dUserRight, uFont, dwTxtColor);
				}break;

				// 管理员权限
				case DTP_MASTER_RIGHT:
				{
					BYTE cbMasterOrder = m_vBodyArry[iIndex]->GetMasterOrder();
					_stprintf_s(m_pzBuffer, _T("<text content='%u' font='%u' textcolor='#%lu'>"), cbMasterOrder, uFont, dwTxtColor);
				}break;

				// 个性签名
				case DTP_UNDER_WRITE:
				{
					CDuiString pzUnderWrite = m_vBodyArry[iIndex]->GetUnderWrite();
					_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>"), pzUnderWrite.GetData(), uFont, dwTxtColor);
				}break;

				// 用户魅力值
				case DTP_CHARM:
				{
					LONG lCharm = m_vBodyArry[iIndex]->GetCharm();
					_stprintf_s(m_pzBuffer, _T("<text content='%ld' font='%u' textcolor='#%lu'>"), lCharm, uFont, dwTxtColor);
				}break;

				// 总局盘数
				case DTP_USER_PLAY_COUNT:
				{
					LONG lTotalCount = m_vBodyArry[iIndex]->GetTotalCount();
					_stprintf_s(m_pzBuffer, _T("<text content='%ld' font='%u' textcolor='#%lu'>"), lTotalCount, uFont, dwTxtColor);
				}break;

				// 用户经验
				case DTP_USER_EXPERIENCE:
				{
					DWORD dwExperience = m_vBodyArry[iIndex]->GetExperience();
					_stprintf_s(m_pzBuffer, _T("<text content='%lu' font='%u' textcolor='#%lu'>"), dwExperience, uFont, dwTxtColor);
				}

				default:
					break;
			}
		}
	}

	return m_pzBuffer;
}

void UserList::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	// 选中
	if (_tcscmp(sType, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
	}

	// 双击
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMDBCLICK) == 0)
	{
	}

	// enter键
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMACTIVATE) == 0)
	{
	}

	// 右键弹起
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMRCLICK) == 0)
	{
		CListTextElementUI* pListElement = static_cast<CListTextElementUI*>(msg.pSender);
		if (pListElement && m_pUserControl)
		{
			int iIndex = pListElement->GetIndex();
			if (iIndex >= 0 && iIndex < m_vBodyArry.size())
			{
				m_pUserControl->ShowMenu(m_vBodyArry[iIndex]);
			}
		}
	}
}

void UserList::SelectByUserID(DWORD dwUserID)
{
	if (m_pList)
	{
		int iItemIndex = 0;
		for (; iItemIndex < m_vBodyArry.size(); ++iItemIndex)
		{
			if (m_vBodyArry[iItemIndex]->GetUserID() == dwUserID)
			{
				m_pList->SelectItem(iItemIndex, true);
				break;
			}
		}
	}
}

// 添加至离线列表
void UserList::AddToOffLineMap(DWORD dwUserID)
{
	// 离线的玩家所有显示变灰, 排序在最末尾
	// 删除所在位置
	UserData* pUser = nullptr;
	std::vector<UserData*>::iterator it = m_vBodyArry.begin();
	for (; it != m_vBodyArry.end();)
	{
		if (dwUserID == (*it)->GetUserID())
		{
			// 从数组中移除该引用
			pUser = *it;
			m_vBodyArry.erase(it);
			break;
		}

		++it;
	}

	if (pUser != nullptr)
	{
		// push到末尾
		m_vBodyArry.push_back(pUser);

		// 添加至离线列表
		m_vDisConnectMap[dwUserID] = pUser;
	}
}

// 从离线列表中恢复该玩家
void UserList::RecoverOffLineMap(DWORD dwUserID)
{
	if (m_vDisConnectMap.find(dwUserID) != m_vDisConnectMap.end())
	{
		// 先清除离线列表该玩家
		this->RemoveOffLineMap(dwUserID);

		// 然后重新插入
		DataManager* pDataMgr = DataManager::GetInstance();
		UserData* pUser = pDataMgr->GetUserDataByID(dwUserID);
		if (pUser)
		{
			this->InsertBodyItem(25, pUser);
		}
	}
}

// 清除离线列表该玩家
void UserList::RemoveOffLineMap(DWORD dwUserID)
{
	std::map<DWORD, UserData*>::iterator map_it = m_vDisConnectMap.find(dwUserID);
	if (map_it != m_vDisConnectMap.end())
	{
		// 删除所在位置
		if (map_it->second)
		{
			this->RemoveBodyItem(map_it->second->GetUserID());
		}

		// 移出离线列表
		m_vDisConnectMap.erase(map_it);
	}
}