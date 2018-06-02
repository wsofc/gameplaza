#ifndef __SERVICEDATA_H__
#define __SERVICEDATA_H__

typedef std::map<WORD, tagGameType*>	GameTypeItemMap;
typedef std::map<WORD, tagGameKind*>	GameKindItemMap;
typedef std::map<WORD, tagGameStation*>	GameStationItemMap;
typedef std::map<WORD, tagGameServer*>	GameServerItemMap;
typedef std::map<WORD, tagPropsInfo*>	GamePropsItemMap;

// ��Ϸ�ڻ�������(�������ڴ��е����ݵ�)
struct GameCacheData
{
	GameCacheData()
	{
		this->bSaveBankTakePw = false;
		this->bSaveExCharmsPw = false;
		this->bSaveBuyGoodsPw = false;

		this->pzBankTakePw = _T("");
		this->pzExCharmsPw = _T("");
		this->pzBuyGoodsPw = _T("");
	}

	bool			bSaveBankTakePw;		// �Ƿ񱣴�����ȡ������
	bool			bSaveExCharmsPw;		// �Ƿ񱣴�һ�����ʱ��ȡ������
	bool			bSaveBuyGoodsPw;		// �Ƿ񱣴湺����Ʒʱ��ȡ������
	CDuiString		pzBankTakePw;			// ȡ������
	CDuiString		pzExCharmsPw;			// �һ���������
	CDuiString		pzBuyGoodsPw;			// ������Ʒ����
};

struct GameFontColor
{
	GameFontColor()
	{
		memset(this, 0, sizeof(*this));
	}

	int id;
	DWORD dwTextcolor;
	DWORD dwHotTextcolor;
	DWORD dwPushedTextcolor;
	DWORD dwSelectedTextcolor;
	DWORD dwDisabledtextcolor;
	DWORD dwBkcolor;
	DWORD dwHotBkcolor;
	DWORD dwSelectedBkcolor;
	DWORD dwFocusborderColor;
};

struct GameNormalTableConfig
{
	GameNormalTableConfig()
	{
		dwPlayerColor	= 0xFFFFFFFF;
		dwIndexColor	= 0xFFFF5F1F;

		memset(pzImgReady,	0, ArrayCount(pzImgReady)	* sizeof(TCHAR));
		memset(pzImgLock,	0, ArrayCount(pzImgLock)	* sizeof(TCHAR));
		memset(pzImgHotBox, 0, ArrayCount(pzImgHotBox)	* sizeof(TCHAR));
	}

	DWORD dwPlayerColor;
	DWORD dwIndexColor;
	TCHAR pzImgReady[MAX_PATH];
	TCHAR pzImgLock[MAX_PATH];
	TCHAR pzImgHotBox[MAX_PATH];
};

struct MemberOrderConfig
{
	DWORD dwOrder0Color = 0xFFFFFFFF;	// ��ɫ
	DWORD dwOrder1Color = 0xFFFF0024;	// ��ɫ
	DWORD dwOrder2Color = 0xFFFFC600;	// ��ɫ
	DWORD dwOrder3Color = 0xFFFF00FC;	// ��ɫ
};

struct ChatColorConfig 
{
	DWORD dwDefaultColor		= 0xFFFFFFFF;	// Ĭ��ɫ:��ɫ
	DWORD dwSysHeadColor		= 0xFFFF0000;	// ϵͳ����ɫ:��ɫ
	DWORD dwSysTextColor		= 0xFFFFFFFF;	// ϵͳ�ı�ɫ:��ɫ

	DWORD dwSrcNameHeadColor	= 0xFF00FFFF;	// Դ�û�������ɫ:����ɫ
	DWORD dwSrcNameTextColor	= 0xFFFFFFFF;	// Դ�û����ı�ɫ:��ɫ
	DWORD dwSrcGameIDColor		= 0xFF00FFFF;	// Դ�û�ID:����ɫ

	DWORD dwRcvNameHeadColor	= 0xFF00FFFF;	// Ŀ�껧������ɫ:����ɫ
	DWORD dwRcvNameTextColor	= 0xFFFFFFFF;	// Ŀ�껧���ı�ɫ:��ɫ
	DWORD dwRcvGameIDColor		= 0xFF00FFFF;	// Ŀ���û�ID:����ɫ

	DWORD dwPropsNameColor		= 0xFF00FFFF;	// ������ɫ:����ɫ
	DWORD dwPropsCountColor		= 0xFF00FFFF;	// ��������ɫ:����ɫ
	DWORD dwSrcCharmColor		= 0xFF00FFFF;	// Դ����ֵɫ:����ɫ
	DWORD dwRcvCharmColor		= 0xFF00FFFF;	// Ŀ������ɫ:����ɫ
};

// ��������
class ServiceData
{
public:
	DWORD								m_dwAllOnLineCount;						// ����������

private:
	ServiceData();
	static ServiceData*					g_pServerListData;						// ʵ��ָ��

	GameTypeItemMap						m_GameTypeItemMap;						// ��Ϸ��������
	GameKindItemMap						m_GameKindItemMap;						// ��Ϸ�ṹ����
	GameStationItemMap					m_GameStationItemMap;					// ��Ϸվ��ṹ����
	GameServerItemMap					m_GameServerItemMap;					// ��Ϸ��������
	GamePropsItemMap					m_GamePropsItemMap;						// ��Ϸ��Ʒ����
	
	std::map<DWORD, DWORD>				m_dwLocalGameVersionMap;				// ������Ϸ�汾������
	std::map<std::string, std::string>	m_urlWebMap;							// ��վurl����
	GameCacheData						m_GameCacheData;						// ��Ϸ�������ݶ���

	std::map<int, GameFontColor*>		m_GameFontColorMap;						// ��̬������ɫ����
	GameNormalTableConfig*				m_pNormalTableConfig;					// ������������
	MemberOrderConfig*					m_pMemberOrderConfig;					// ��Ա��������
	ChatColorConfig*					m_pChatColorConfig;						// ������ı���ɫ����

// ���ú���
public:
	~ServiceData();
	static ServiceData * GetInstance();											// ��ȡ��ʵ��
	static void Release(){ SAFE_DELETE(g_pServerListData); }
	DWORD GetAllOnLineCount(){ return m_dwAllOnLineCount; }						// ��ȡ����������
	void SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount);					// ���ø���Ϸ��������
	void SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount);				// ���÷�����������

	// -----------------------------------------------------------------------------------------------------
	// ��Ϸ����б����

	bool InsertGameType(tagGameType * pGameType);								// ������Ϸ����
	bool InsertGameKind(tagGameKind * pGameKind);								// ������Ϸ�ṹ
	bool InsertGameStation(tagGameStation* pGameStation);						// ������Ϸվ��
	bool InsertGameServer(tagGameServer * pGameServer);							// ������Ϸ����
	bool InsertGameProps(tagPropsInfo* pGameProps);								// ������Ʒ����

	bool DeleteGameType(WORD wTypeID);											// ɾ����Ϸ����
	bool DeleteGameKind(WORD wKindID);											// ɾ����Ϸ�ṹ
	bool DeleteGameStation(WORD wStationID);									// ɾ����Ϸվ��
	bool DeleteGameServer(WORD wServerID);										// ɾ����Ϸ����
	bool DeleteGameProps(WORD wPropsID);										// ɾ����Ϸ��Ʒ

	void DeleteAllGameType();													// ɾ��������Ϸ����
	void DeleteAllGameKind();													// ɾ��������Ϸ�ṹ
	void DeleteAllGameStation();												// ɾ��������Ϸվ��
	void DeleteAllGameServer();													// ɾ��������Ϸ����
	void DeleteAllGameProps();													// ɾ��������Ϸ��Ʒ

	tagGameType*	SearchGameType(WORD wTypeID);								// ������Ϸ����
	tagGameKind*	SearchGameKind(WORD wKindID);								// ������Ϸ�ṹ
	tagGameStation* SearchGameStation(WORD wStationID);							// ������Ϸվ��
	tagGameServer*	SearchGameServer(WORD ServerID);							// ������Ϸ����
	tagPropsInfo*	SearchGameProps(WORD wPropsID);								// ������Ϸ��Ʒ

	DWORD GetGameTypeCount() { return (DWORD)m_GameTypeItemMap.size(); }		// ��Ϸ������Ŀ
	DWORD GetGameKindCount() { return (DWORD)m_GameKindItemMap.size(); }		// ��Ϸ�ṹ��Ŀ
	DWORD GetGameStationCount() { return (DWORD)m_GameStationItemMap.size(); }	// ��Ϸվ����Ŀ
	DWORD GetGameServerCount() { return (DWORD)m_GameServerItemMap.size(); }	// ��Ϸ������Ŀ
	DWORD GetGamePropsCount() { return (DWORD)m_GamePropsItemMap.size(); }		// ��Ϸ��Ʒ��Ŀ

	tagGameType*	EnumGameType(DWORD& iNextKey);								// ö����Ϸ����
	tagGameKind*	EnumGameKind(DWORD& iNextKey);								// ö����Ϸ�ṹ
	tagGameStation* EnumGameStation(DWORD& iNextKey);							// ö����Ϸվ��
	tagGameServer*	EnumGameServer(DWORD& iNextKey);							// ö����Ϸ����
	tagPropsInfo*	EnumGameProps(DWORD& iNextKey);								// ö����Ϸ��Ʒ
	
	GameCacheData&	GetGameCacheData(){ return m_GameCacheData; }				// ��ȡ��Ϸ�������ݶ�������
	
	// -----------------------------------------------------------------------------------------------------
	// ��̬������ɫ����
	bool InsertGameFontColor(GameFontColor* pFontColor);
	GameFontColor* GetFontColorByID(int id);
	void DeleteAllGameFontColor();

	// -----------------------------------------------------------------------------------------------------
	// ��������
	void SetNormalTableConfig(GameNormalTableConfig* pNormalTable);
	GameNormalTableConfig* GetNormalTableConfig(){ return m_pNormalTableConfig; }


	// -----------------------------------------------------------------------------------------------------
	// ��Ա����
	void SetMemberOrderConfig(MemberOrderConfig* pMemberOrderConfig);
	MemberOrderConfig* GetMemberOrderConfig(){ return m_pMemberOrderConfig; }

	// -----------------------------------------------------------------------------------------------------
	// �����ı���ɫ����
	void SetChatColorConfig(ChatColorConfig* pChatColorConfig);
	ChatColorConfig* GetChatColorConfig(){ return m_pChatColorConfig; }

	// -----------------------------------------------------------------------------------------------------
	// ��Ϸ��վ����
	void AddUrlWeb(LPCTSTR lpName, LPCTSTR lpUrl);
	LPCTSTR GetUrlWeb(LPCTSTR lpName);
		
	// -----------------------------------------------------------------------------------------------------
	// ��Ϸ�汾����
	DWORD GetExeVersion(LPCTSTR strFile);										//��ȡָ��exe�İ汾��
	void GetExeVersion(LPCTSTR strFile, CDuiString& strVersion);				//��ȡָ��exe�İ汾��

	void SetLocalGameVersion(UINT wKindID, UINT wVersion);						// ������Ϸ�汾
	UINT GetLocalGameVersion(UINT wKindID);										// ��ѯ��Ϸ�汾

	DWORD ReadExeVersionInfo(UINT wKindID, CDuiString& pzExePath);				// ��ȡexe�İ汾��Ϣ

};

#endif // !__SERVICEDATA_H__