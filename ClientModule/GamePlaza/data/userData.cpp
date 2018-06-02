#include "stdafx.h"
#include "userData.h"

extern GameResource* R;
extern CStaticLogger g_Logger;

UserData::UserData()
	: m_dwUserID(0)
	, m_dwGameID(0)
	, m_wRoomID(0)
	, m_wTableID(INVALID_TABLE)
	, m_wChairID(INVALID_CHAIR)
	, m_wLastTableID(INVALID_TABLE)
	, m_wLastChairID(INVALID_CHAIR)
	, m_wFaceID(0)
	, m_dwGroupID(0)
	, m_cbSelf(false)
	, m_cbUnreal(false)
	, m_cbStatus(US_NULL)
	, m_cbLastStatus(0)
	, m_cbGender(0)
	, m_cbMemberOrder(0)
	, m_dwExperience(0)
	, m_dwCustomFaceVer(0)
	, m_cbMoorMachine(0)
	, m_dwUserRight(0)
	, m_dwMasterRight(0)
	, m_cbMasterOrder(0)
	, m_bMasterSwitchOn(false)
	, m_lGameGold(0)
	, m_lBankScore(0)
	, m_lScore(0)
	, m_lInsureScore(0)
	, m_lCharm(0)
	, m_lWinCount(0)
	, m_lLostCount(0)
	, m_lDrawCount(0)
	, m_lFleeCount(0)
	, m_cbRobot(FALSE)
	, m_cbCompanion(0)
	, m_wSignUpTimes(0)
	, m_cbHaveSignUp(false)
	, m_dwMemberColor(0xFFFFFF)
{
	memset(m_szAccount,			0,	ArrayCount(m_szAccount)			*	sizeof(TCHAR));				// 游戏账号
	memset(m_szPassword,		0,	ArrayCount(m_szPassword)		*	sizeof(TCHAR));				// 密码
	memset(m_szPasswordEncrypt, 0,	ArrayCount(m_szPasswordEncrypt)	*	sizeof(TCHAR));				// 加密后的密码
	memset(m_szUnderWrite,		0,	ArrayCount(m_szUnderWrite)		*	sizeof(TCHAR));				// 个性签名
	memset(m_szNickname,		0,	ArrayCount(m_szNickname)		*	sizeof(TCHAR));				// 玩家昵称
	memset(m_szIcon,			0,	ArrayCount(m_szIcon)			*	sizeof(TCHAR));				// 玩家头像icon
	memset(m_szGroupName,		0,	ArrayCount(m_szGroupName)		*	sizeof(TCHAR));				// 社团名
	memset(m_szGender,			0,	ArrayCount(m_szGender)			*	sizeof(TCHAR));				// 性别字符串
	memset(m_szMachineID,		0,	ArrayCount(m_szMachineID)		*	sizeof(TCHAR));				// 机器码
	memset(m_szRobotPrefix,		0,	ArrayCount(m_szRobotPrefix)		*	sizeof(TCHAR));				// 机器人昵称前缀

	memset(&m_uGameRule,		0,										sizeof(CMD_GR_UserRule));	// 游戏设置
	memset(&m_uStruct,			0,										sizeof(UserStruct));		// IPC传递消息用的结构体
	memset(&m_uStructSuffix,	0,										sizeof(UserStructSuffix));	// IPC传递消息用的附加结构体

	this->SetRobotPrefix(_T("[R]_"));
}

UserData::~UserData()
{
}

void UserData::SetFaceID(WORD wFaceID)
{
	m_wFaceID = wFaceID;

	TCHAR pzPath[MAX_PATH] = { 0 };
	::PathCanonicalize(pzPath, CPaintManagerUI::GetResourcePath().GetData());

	_stprintf_s(m_szIcon, _T("common\\headPic\\%u.png"), m_wFaceID);

	bool bRes = false;
#ifdef SKIN_RES_TEST
	_stprintf_s(pzPath, _T("%s%s"), pzPath, m_szIcon);
	bRes = TRUE == ::PathFileExists(pzPath);
#else
	bRes = CRenderEngine::IsZipItemExist(m_szIcon);
#endif // SKIN_RES_TEST
	if (!bRes)
	{
		::memset(m_szIcon, 0, ArrayCount(m_szIcon) * sizeof(TCHAR));
	}
}

LPCTSTR UserData::GetIcon()
{
	if (_tcscmp(m_szIcon, _T("")) == 0)
	{
		TCHAR pzPath[MAX_PATH] = { 0 };
		_stprintf_s(pzPath, _T("common\\headPic\\%u.png"), m_wFaceID);

		bool bRes = false;
#ifdef SKIN_RES_TEST
		::PathCanonicalize(m_szIcon, CPaintManagerUI::GetResourcePath().GetData());
		_stprintf_s(m_szIcon, _T("%s%s"), m_szIcon, pzPath);
		bRes = TRUE == ::PathFileExists(m_szIcon);
#else
		bRes = CRenderEngine::IsZipItemExist(pzPath);
#endif // SKIN_RES_TEST
		if (!bRes)
		{
			//找不到路径,则设置默认路径
			if (0 == GetGender())
			{
				_stprintf_s(m_szIcon, _T("common\\headPic\\0.png"));
			}
			else
			{
				_stprintf_s(m_szIcon, _T("common\\headPic\\1.png"));
			}
		}
	}

	return m_szIcon;
}

void UserData::SetGender(BYTE cbGender)
{
	m_cbGender = cbGender;
	switch (m_cbGender)
	{
		case 0:
		{
			_stprintf_s(m_szGender, _T("%s"), _T("男"));
		}break;

		case 1:
		{
			_stprintf_s(m_szGender, _T("%s"), _T("女"));
		}break;

		default:
		{
			_stprintf_s(m_szGender, _T("%s"), _T("男"));
		}break;
	}
}

// 设置会员等级(增序)
void UserData::SetMemberOrder(BYTE cbMemberOrder)
{
	m_cbMemberOrder = cbMemberOrder;

	ServiceData* pServiceData = ServiceData::GetInstance();
	MemberOrderConfig* pMemOrderConfig = pServiceData->GetMemberOrderConfig();
	if (!pMemOrderConfig) return;

	switch (m_cbMemberOrder)
	{
		case 0:		m_dwMemberColor = pMemOrderConfig->dwOrder0Color;	break;
		case 1:		m_dwMemberColor = pMemOrderConfig->dwOrder1Color;	break;
		case 2:		m_dwMemberColor = pMemOrderConfig->dwOrder2Color;	break;
		case 3:		m_dwMemberColor = pMemOrderConfig->dwOrder3Color;	break;
		default:	m_dwMemberColor = pMemOrderConfig->dwOrder0Color;	break;
	}
}

BYTE UserData::GetMasterOrder()
{
	BYTE cbMOrder = 0;
	if (this->IsSelf())
	{
		if (this->IsMasterSwitchOn())
		{
			cbMOrder = this->IsUnrealUser() ? 0xFF : m_cbMasterOrder;
		}
	}
	else
	{
		cbMOrder = m_cbMasterOrder;
	}

	return cbMOrder;
}

void UserData::SaveGameSetting()
{
	g_Logger->TryInfo(_T("UserData - Save Game Setting"));

	CDuiString pzPath;
	pzPath.Empty();
	pzPath.Format(_T("%s..\\data\\user\\%s\\%s")
		, CPaintManagerUI::GetResourcePath().GetData()
		, m_szAccount, _T("GAMESETTING"));

	Json::Value vRoot;
	JsonFile* pFile = new JsonFile(pzPath);

	vRoot["bLimitWin"] = m_uGameRule.bLimitWin;
	vRoot["bLimitFlee"] = m_uGameRule.bLimitFlee;
	vRoot["bLimitScore"] = m_uGameRule.bLimitScore;
	vRoot["bPassword"] = m_uGameRule.bPassword;
	vRoot["bCheckSameIP"] = m_uGameRule.bCheckSameIP;

	vRoot["wWinRate"] = m_uGameRule.wWinRate;
	vRoot["wFleeRate"] = m_uGameRule.wFleeRate;
	vRoot["lLessScore"] = m_uGameRule.lLessScore;
	vRoot["lMaxScore"] = m_uGameRule.lMaxScore;
	vRoot["szPassword"] = m_uGameRule.szPassword;

	pFile->SaveFile(vRoot);
	SAFE_DELETE(pFile);

	g_Logger->TryInfo(_T("UserData - Save Game Setting Complete"));
}

void UserData::ReadGameSetting()
{
	g_Logger->TryInfo(_T("UserData - Read Game Setting"));

	CDuiString pzPath;
	pzPath.Empty();
	pzPath.Format(_T("user\\%s\\%s"), m_szAccount, _T("GAMESETTING"));

	Json::Value vRoot;
	JsonFile* pFile = new JsonFile(pzPath);
	bool bResult = pFile->initRootNode(vRoot);
	SAFE_DELETE(pFile);
	if (bResult)
	{
		if (!vRoot["bLimitWin"].empty())	m_uGameRule.bLimitWin		= vRoot["bLimitWin"].asBool();
		if (!vRoot["bLimitFlee"].empty())	m_uGameRule.bLimitFlee		= vRoot["bLimitFlee"].asBool();
		if (!vRoot["bLimitScore"].empty())	m_uGameRule.bLimitScore		= vRoot["bLimitScore"].asBool();
		if (!vRoot["bPassword"].empty())	m_uGameRule.bPassword		= vRoot["bPassword"].asBool();
		if (!vRoot["bCheckSameIP"].empty())	m_uGameRule.bCheckSameIP	= vRoot["bCheckSameIP"].asBool();

		if (!vRoot["wWinRate"].empty())		m_uGameRule.wWinRate		= vRoot["wWinRate"].asUInt();
		if (!vRoot["wFleeRate"].empty())	m_uGameRule.wFleeRate		= vRoot["wFleeRate"].asUInt();
		if (!vRoot["lLessScore"].empty())	m_uGameRule.lLessScore		= vRoot["lLessScore"].asInt64();
		if (!vRoot["lMaxScore"].empty())	m_uGameRule.lMaxScore		= vRoot["lMaxScore"].asInt64();

		if (!vRoot["szPassword"].empty())
		{
			CDuiString pzPassword = vRoot["szPassword"].asCString();
			::StrToCharArray(m_uGameRule.szPassword, pzPassword.GetData());
		}
	}

	g_Logger->TryInfo(_T("UserData - Read Game Setting Complete"));
}

void UserData::UpdateUI(CPaintManagerUI* pManager)
{
	if (!pManager)
	{
		DialogFrame::MsgBox(pManager, _T("温馨提示"), _T("更新玩家数据失败!"));
	}
	else
	{
		TCHAR pzBuffer[MAX_PATH] = { 0 };

		// 游戏ID
		CTextUI* pTxtID = static_cast<CTextUI*>(pManager->FindControl(R->kTxtHallMain_ID));
		if (pTxtID)
		{
			_stprintf_s(pzBuffer, _T("%lu"), m_dwGameID);
			pTxtID->SetText(pzBuffer);
		}

		// 昵称
		CTextUI* pTxtNickName = static_cast<CTextUI*>(pManager->FindControl(R->kTxtHallMain_NickName));
		if (pTxtNickName)
		{
			pTxtNickName->SetText(m_szNickname);
			pTxtNickName->SetToolTip(m_szNickname);
		}
		
		// 等级(经验值)
		CTextUI* pTxtLevel = static_cast<CTextUI*>(pManager->FindControl(R->kTxtHallMain_Level));
		if (pTxtLevel)
		{
			DWORD dwXPLevel = 0;
			DataManager* pDataManager = DataManager::GetInstance();
			dwXPLevel = pDataManager->GetXPLevelByScore(m_dwExperience);

			_stprintf_s(pzBuffer, _T("%lu"), dwXPLevel);
			pTxtLevel->SetText(pzBuffer);
		}

		// 金币
		CTextUI* pTxtGolds = static_cast<CTextUI*>(pManager->FindControl(R->kTxtHallMain_Golds));
		if (pTxtGolds)
		{
			_stprintf_s(pzBuffer, _T("%lld"), m_lGameGold);
			pTxtGolds->SetText(pzBuffer);
		}

		// 头像
		CControlGdiplusUI* pCtlHeadIcon = static_cast<CControlGdiplusUI*>(pManager->FindControl(R->kBtnHallMain_HeadIcon));
		if (pCtlHeadIcon)
		{
			pCtlHeadIcon->SetBkImage(this->GetIcon());
		}

		// 会员等级
		CLabelUI* pLabMember = static_cast<CButtonUI*>(pManager->FindControl(R->kLabHallMain_Member));
		if (pLabMember)
		{
			_stprintf_s(pzBuffer, _T("file='common/list/member_%u.png' aligncenter='true'"), m_cbMemberOrder);
			pLabMember->SetBkImage(pzBuffer);
		}
	}
}

void UserData::CleanStatus()
{
	m_wRoomID		= 0;							//玩家所在房间号
	m_wChairID		= INVALID_CHAIR;				//椅子位置
	m_wTableID		= INVALID_TABLE;				//桌子号码
	m_wLastChairID	= INVALID_CHAIR;				//上次椅子位置
	m_wLastTableID	= INVALID_TABLE;				//上次桌子号码
	
	memset(&m_uStruct, 0, sizeof(UserStruct));		//玩家结构(用于IPC通信)
}

UserStruct& UserData::GetUserStruct()
{
	memset(&m_uStruct, 0, sizeof(UserStruct));
	
	memcpy(m_uStruct.pzName,		m_szNickname,	ArrayCount(m_szNickname)	* sizeof(TCHAR));
	memcpy(m_uStruct.pzAccount,		m_szAccount,	ArrayCount(m_szAccount)		* sizeof(TCHAR));
	memcpy(m_uStruct.pzFacePath,	m_szIcon,		ArrayCount(m_szIcon)		* sizeof(TCHAR));

	m_uStruct.cbStatus		= GetUserStatus();
	m_uStruct.cbSelf		= IsSelf();
	m_uStruct.cbRobot		= IsRobot();
	m_uStruct.cbMemberOrder = GetMemberOrder();
	m_uStruct.cbMasterOrder = GetMasterOrder();
	m_uStruct.dwUserID		= GetUserID();
	m_uStruct.dwFaceID		= GetFaceID();
	m_uStruct.dwGameID		= GetGameID();
	m_uStruct.dwTableID		= GetTableID();
	m_uStruct.dwChairID		= GetChairID();
	m_uStruct.lGold			= GetGameGold();
	m_uStruct.lBankGold		= GetBankScore();
	m_uStruct.lScore		= GetScore();
	m_uStruct.lCharm		= GetCharm();
	
	return m_uStruct;
}

UserStructSuffix& UserData::GetUserStructSuffix()
{
	memset(&m_uStructSuffix, 0, sizeof(UserStructSuffix));

	memcpy(m_uStructSuffix.pzGroupName,		m_szGroupName,	ArrayCount(m_szGroupName)	* sizeof(TCHAR));
	memcpy(m_uStructSuffix.pzUnderWrite,	m_szUnderWrite,	ArrayCount(m_szUnderWrite)	* sizeof(TCHAR));
	memcpy(m_uStructSuffix.pzMachineID,		m_szMachineID,	ArrayCount(m_szMachineID)	* sizeof(TCHAR));

	// 等级描述
	DataManager* pDataManager = DataManager::GetInstance();
	CDuiString pzScoreDesc = pDataManager->GetLevelDescByScore(m_lScore);
	int iMinCount = min(ArrayCount(m_uStructSuffix.pzLevelDesc), pzScoreDesc.GetLength() + 1);
	memcpy(m_uStructSuffix.pzLevelDesc,		pzScoreDesc,	iMinCount	* sizeof(TCHAR));

	m_uStructSuffix.cbGender		= GetGender();
	m_uStructSuffix.dwGroupID		= GetGroupID();
	m_uStructSuffix.dwExperience	= GetExperience();
	m_uStructSuffix.lWinCount		= GetWinCount();
	m_uStructSuffix.lLostCount		= GetLostCount();
	m_uStructSuffix.lDrawCount		= GetDrawCount();
	m_uStructSuffix.lFleeCount		= GetFleeCount();
	m_uStructSuffix.llInsureScore	= GetInsureScore();

	return m_uStructSuffix;
}

// ------------------------------------------------------------
// 在线人数描述配置信息
OnlineDescConfig::OnlineDescConfig()
	: m_dwOnlineCount(0)
	, m_iStatus(0)
{
	memset(m_pzOnlineDesc, 0, MAX_DESC_LEN * sizeof(TCHAR));
}

OnlineDescConfig::~OnlineDescConfig()
{

}

void OnlineDescConfig::SetDescribe(std::string& pzDesc)
{
	if (pzDesc.size() > 0)
	{
		::StrToCharArray(m_pzOnlineDesc, pzDesc.c_str());
	}
}

// ------------------------------------------------------------
// 等级配置信息

LevelConfig::LevelConfig()
	: m_dwLevel(1)
	, m_lScore(0)
{
	memset(m_pzOrderDesc, 0, MAX_DESC_LEN * sizeof(TCHAR));
}

LevelConfig::~LevelConfig()
{

}

void LevelConfig::SetDescribe(std::string& pzDesc)
{
	if (pzDesc.size() > 0)
	{
		::StrToCharArray(m_pzOrderDesc, pzDesc.c_str());
	}
}


// ------------------------------------------------------------
// 经验值描述配置信息
ExperinceConfig::ExperinceConfig()
	: m_dwLevel(1)
	, m_lScore(0)
{

}

ExperinceConfig::~ExperinceConfig()
{

}

// ------------------------------------------------------------
// 房间场次配置信息
RoomSeasonConfig::RoomSeasonConfig()
	: m_dwSeasonID(0)
{
	memset(m_pzSeasonName, 0, MAX_DESC_LEN * sizeof(TCHAR));
	memset(m_pzSeasonDesc, 0, MAX_DESC_LEN * sizeof(TCHAR));
}

RoomSeasonConfig::~RoomSeasonConfig()
{
}

void RoomSeasonConfig::SetSeasonName(std::string& pzName)
{
	if (pzName.size() > 0)
	{
		::StrToCharArray(m_pzSeasonName, pzName.c_str());
	}
}

void RoomSeasonConfig::SetSeasonDescribe(std::string& pzDesc)
{
	if (pzDesc.size() > 0)
	{
		::StrToCharArray(m_pzSeasonDesc, pzDesc.c_str());
	}
}