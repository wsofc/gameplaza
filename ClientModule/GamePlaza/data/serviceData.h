#ifndef __SERVICEDATA_H__
#define __SERVICEDATA_H__

typedef std::map<WORD, tagGameType*>	GameTypeItemMap;
typedef std::map<WORD, tagGameKind*>	GameKindItemMap;
typedef std::map<WORD, tagGameStation*>	GameStationItemMap;
typedef std::map<WORD, tagGameServer*>	GameServerItemMap;
typedef std::map<WORD, tagPropsInfo*>	GamePropsItemMap;

// 游戏内缓存数据(保存在内存中的数据等)
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

	bool			bSaveBankTakePw;		// 是否保存银行取款密码
	bool			bSaveExCharmsPw;		// 是否保存兑换魅力时的取款密码
	bool			bSaveBuyGoodsPw;		// 是否保存购买物品时的取款密码
	CDuiString		pzBankTakePw;			// 取款密码
	CDuiString		pzExCharmsPw;			// 兑换魅力密码
	CDuiString		pzBuyGoodsPw;			// 购买物品密码
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
	DWORD dwOrder0Color = 0xFFFFFFFF;	// 白色
	DWORD dwOrder1Color = 0xFFFF0024;	// 红色
	DWORD dwOrder2Color = 0xFFFFC600;	// 黄色
	DWORD dwOrder3Color = 0xFFFF00FC;	// 紫色
};

struct ChatColorConfig 
{
	DWORD dwDefaultColor		= 0xFFFFFFFF;	// 默认色:白色
	DWORD dwSysHeadColor		= 0xFFFF0000;	// 系统标题色:红色
	DWORD dwSysTextColor		= 0xFFFFFFFF;	// 系统文本色:白色

	DWORD dwSrcNameHeadColor	= 0xFF00FFFF;	// 源用户名标题色:淡蓝色
	DWORD dwSrcNameTextColor	= 0xFFFFFFFF;	// 源用户名文本色:白色
	DWORD dwSrcGameIDColor		= 0xFF00FFFF;	// 源用户ID:淡蓝色

	DWORD dwRcvNameHeadColor	= 0xFF00FFFF;	// 目标户名标题色:淡蓝色
	DWORD dwRcvNameTextColor	= 0xFFFFFFFF;	// 目标户名文本色:白色
	DWORD dwRcvGameIDColor		= 0xFF00FFFF;	// 目标用户ID:淡蓝色

	DWORD dwPropsNameColor		= 0xFF00FFFF;	// 道具名色:淡蓝色
	DWORD dwPropsCountColor		= 0xFF00FFFF;	// 道具数量色:淡蓝色
	DWORD dwSrcCharmColor		= 0xFF00FFFF;	// 源魅力值色:淡蓝色
	DWORD dwRcvCharmColor		= 0xFF00FFFF;	// 目标魅力色:淡蓝色
};

// 服务数据
class ServiceData
{
public:
	DWORD								m_dwAllOnLineCount;						// 总在线人数

private:
	ServiceData();
	static ServiceData*					g_pServerListData;						// 实例指针

	GameTypeItemMap						m_GameTypeItemMap;						// 游戏种类索引
	GameKindItemMap						m_GameKindItemMap;						// 游戏结构索引
	GameStationItemMap					m_GameStationItemMap;					// 游戏站点结构索引
	GameServerItemMap					m_GameServerItemMap;					// 游戏房间索引
	GamePropsItemMap					m_GamePropsItemMap;						// 游戏商品索引
	
	std::map<DWORD, DWORD>				m_dwLocalGameVersionMap;				// 本地游戏版本号容器
	std::map<std::string, std::string>	m_urlWebMap;							// 网站url容器
	GameCacheData						m_GameCacheData;						// 游戏缓存数据对象

	std::map<int, GameFontColor*>		m_GameFontColorMap;						// 动态字体颜色容器
	GameNormalTableConfig*				m_pNormalTableConfig;					// 常规桌子配置
	MemberOrderConfig*					m_pMemberOrderConfig;					// 会员属性配置
	ChatColorConfig*					m_pChatColorConfig;						// 聊天框文本颜色配置

// 配置函数
public:
	~ServiceData();
	static ServiceData * GetInstance();											// 获取单实例
	static void Release(){ SAFE_DELETE(g_pServerListData); }
	DWORD GetAllOnLineCount(){ return m_dwAllOnLineCount; }						// 获取总在线人数
	void SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount);					// 设置该游戏在线人数
	void SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount);				// 设置房间在线人数

	// -----------------------------------------------------------------------------------------------------
	// 游戏相关列表操作

	bool InsertGameType(tagGameType * pGameType);								// 插入游戏种类
	bool InsertGameKind(tagGameKind * pGameKind);								// 插入游戏结构
	bool InsertGameStation(tagGameStation* pGameStation);						// 插入游戏站点
	bool InsertGameServer(tagGameServer * pGameServer);							// 插入游戏房间
	bool InsertGameProps(tagPropsInfo* pGameProps);								// 插入商品数据

	bool DeleteGameType(WORD wTypeID);											// 删除游戏种类
	bool DeleteGameKind(WORD wKindID);											// 删除游戏结构
	bool DeleteGameStation(WORD wStationID);									// 删除游戏站点
	bool DeleteGameServer(WORD wServerID);										// 删除游戏房间
	bool DeleteGameProps(WORD wPropsID);										// 删除游戏商品

	void DeleteAllGameType();													// 删除所有游戏种类
	void DeleteAllGameKind();													// 删除所有游戏结构
	void DeleteAllGameStation();												// 删除所有游戏站点
	void DeleteAllGameServer();													// 删除所有游戏房间
	void DeleteAllGameProps();													// 删除所有游戏商品

	tagGameType*	SearchGameType(WORD wTypeID);								// 查找游戏种类
	tagGameKind*	SearchGameKind(WORD wKindID);								// 查找游戏结构
	tagGameStation* SearchGameStation(WORD wStationID);							// 查找游戏站点
	tagGameServer*	SearchGameServer(WORD ServerID);							// 查找游戏房间
	tagPropsInfo*	SearchGameProps(WORD wPropsID);								// 查找游戏商品

	DWORD GetGameTypeCount() { return (DWORD)m_GameTypeItemMap.size(); }		// 游戏种类数目
	DWORD GetGameKindCount() { return (DWORD)m_GameKindItemMap.size(); }		// 游戏结构数目
	DWORD GetGameStationCount() { return (DWORD)m_GameStationItemMap.size(); }	// 游戏站点数目
	DWORD GetGameServerCount() { return (DWORD)m_GameServerItemMap.size(); }	// 游戏房间数目
	DWORD GetGamePropsCount() { return (DWORD)m_GamePropsItemMap.size(); }		// 游戏商品数目

	tagGameType*	EnumGameType(DWORD& iNextKey);								// 枚举游戏种类
	tagGameKind*	EnumGameKind(DWORD& iNextKey);								// 枚举游戏结构
	tagGameStation* EnumGameStation(DWORD& iNextKey);							// 枚举游戏站点
	tagGameServer*	EnumGameServer(DWORD& iNextKey);							// 枚举游戏房间
	tagPropsInfo*	EnumGameProps(DWORD& iNextKey);								// 枚举游戏商品
	
	GameCacheData&	GetGameCacheData(){ return m_GameCacheData; }				// 获取游戏缓存数据对象引用
	
	// -----------------------------------------------------------------------------------------------------
	// 动态字体颜色配置
	bool InsertGameFontColor(GameFontColor* pFontColor);
	GameFontColor* GetFontColorByID(int id);
	void DeleteAllGameFontColor();

	// -----------------------------------------------------------------------------------------------------
	// 桌子配置
	void SetNormalTableConfig(GameNormalTableConfig* pNormalTable);
	GameNormalTableConfig* GetNormalTableConfig(){ return m_pNormalTableConfig; }


	// -----------------------------------------------------------------------------------------------------
	// 会员配置
	void SetMemberOrderConfig(MemberOrderConfig* pMemberOrderConfig);
	MemberOrderConfig* GetMemberOrderConfig(){ return m_pMemberOrderConfig; }

	// -----------------------------------------------------------------------------------------------------
	// 聊天文本颜色配置
	void SetChatColorConfig(ChatColorConfig* pChatColorConfig);
	ChatColorConfig* GetChatColorConfig(){ return m_pChatColorConfig; }

	// -----------------------------------------------------------------------------------------------------
	// 游戏网站设置
	void AddUrlWeb(LPCTSTR lpName, LPCTSTR lpUrl);
	LPCTSTR GetUrlWeb(LPCTSTR lpName);
		
	// -----------------------------------------------------------------------------------------------------
	// 游戏版本控制
	DWORD GetExeVersion(LPCTSTR strFile);										//获取指定exe的版本号
	void GetExeVersion(LPCTSTR strFile, CDuiString& strVersion);				//获取指定exe的版本号

	void SetLocalGameVersion(UINT wKindID, UINT wVersion);						// 设置游戏版本
	UINT GetLocalGameVersion(UINT wKindID);										// 查询游戏版本

	DWORD ReadExeVersionInfo(UINT wKindID, CDuiString& pzExePath);				// 读取exe的版本信息

};

#endif // !__SERVICEDATA_H__