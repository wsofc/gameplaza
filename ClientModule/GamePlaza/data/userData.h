#pragma once

#ifndef __USERDATA_H__
#define __USERDATA_H__

#define GENDER_LEN	(5)

// -------------------------------------------------------------------------------------
// 传递至游戏中的用户数据结构
//#pragma pack(1)
struct UserStruct
{
	BYTE		pzName[NAME_LEN];		// 昵称
	BYTE		pzAccount[NAME_LEN];	// 账号
	BYTE		pzFacePath[MAX_PATH];	// 头像路径
	BYTE		cbStatus;				// 玩家状态
	BYTE		cbSelf;					// 是否是自己
	BYTE		cbRobot;				// 是否是机器人
	BYTE		cbMemberOrder;			// 会员等级
	BYTE		cbMasterOrder;			// 管理员等级
	DWORD		dwUserID;				// 用户 I D
	DWORD		dwFaceID;				// 头像 I D
	DWORD		dwGameID;				// 游戏显示 I D(非游戏真正的ID)
	DWORD		dwTableID;				// 桌子 I D
	DWORD		dwChairID;				// 椅子 I D
	LONGLONG	lGold;					// 金币
	LONGLONG	lBankGold;				// 银行金币
	LONGLONG	lScore;					// 用户分数
	LONGLONG	lCharm;					// 魅力
};

// 追加的用户数据结构
struct UserStructSuffix
{
	BYTE				cbGender;								// 用户性别

	DWORD				dwGroupID;								// 社团索引
	DWORD				dwExperience;							// 用户经验
	
	LONG				lWinCount;								// 胜利盘数
	LONG				lLostCount;								// 失败盘数
	LONG				lDrawCount;								// 和局盘数
	LONG				lFleeCount;								// 断线数目
	LONGLONG			llInsureScore;							// 消费金币
	
	BYTE				pzGroupName[GROUP_LEN];					// 社团名字
	BYTE				pzUnderWrite[UNDER_WRITE_LEN];			// 个性签名
	BYTE				pzLevelDesc[NAME_LEN];					// 等级描述
	BYTE				pzMachineID[COMPUTER_ID_LEN];			// 机器码
};

//#pragma pack()
// -------------------------------------------------------------------------------------

// 用户数据类
class UserData
{
public:

	UserData();
	~UserData();

	// ------------------------------------------------------------
	// 基础信息

	//用户 I D
	void SetUserID(DWORD dwUserID){ m_dwUserID = dwUserID; }
	const DWORD GetUserID(){ return m_dwUserID; }

	//玩家状态
	void SetUserStatus(BYTE cbStatus){ m_cbStatus = cbStatus; }
	const BYTE GetUserStatus(){ return m_cbStatus; }

	//玩家上一次状态
	void SetLastUserStatus(BYTE cbLastStatus){ m_cbLastStatus = cbLastStatus; }
	const BYTE GetLastUserStatus(){ return m_cbLastStatus; }

	//是否已经游戏中
	bool IsGaming(){ return m_cbStatus == US_PLAY; }

	//是否已准备好
	bool IsUserReady(){ return m_cbStatus == US_READY; }

	//玩家权限
	void SetUserRight(DWORD dwUserRight){ m_dwUserRight = dwUserRight; }
	DWORD GetUserRight(){ return m_dwUserRight; }

	//会员等级
	void SetMemberOrder(BYTE cbMemberOrder);
	const BYTE GetMemberOrder(){ return m_cbMemberOrder; }

	//对应列表字体颜色
	const DWORD GetMemberOrderColor(){ return m_dwMemberColor; }

	//是否开启管理员权限检测
	void SetMasterSwitchOn(bool bOpen){ m_bMasterSwitchOn = bOpen; }
	bool IsMasterSwitchOn(){ return m_bMasterSwitchOn; }

	//管理权限
	void SetMasterRight(DWORD dwMasterRight){ m_dwMasterRight = dwMasterRight; }
	DWORD GetMasterRight(){ return m_dwMasterRight; }

	//管理员等级
	void SetMasterOrder(BYTE cbMasterOrder){ m_cbMasterOrder = cbMasterOrder; }
	BYTE GetMasterOrder();
	BYTE GetRealMasterOrder(){ return m_cbMasterOrder; }

	//机器人
	void SetRobot(BOOL cbRobot){ m_cbRobot = cbRobot; }
	BOOL IsRobot(){ return m_cbRobot == TRUE; }

	//机器人名称前缀
	void SetRobotPrefix(LPCTSTR pzPrefix){ ::StrToCharArray(m_szRobotPrefix, pzPrefix); }
	LPCTSTR GetRobotPrefix(){ return m_szRobotPrefix; }

	//用户性别
	void SetGender(BYTE cbGender);
	BYTE GetGender(){ return m_cbGender; }
	LPTSTR GetGenderStr(){ return m_szGender; }

	//设置为自己
	void SetSelf(BYTE bSelf){ m_cbSelf = bSelf; }
	BYTE IsSelf(){ return m_cbSelf; }

	//用户游戏"显示 I D"
	void SetGameID(DWORD dwGameID){ m_dwGameID = dwGameID; }
	DWORD GetGameID(){ return m_dwGameID; }

	//用户经验
	void SetExperience(DWORD dwExperience){ m_dwExperience = dwExperience; }
	DWORD GetExperience(){ return m_dwExperience; }
	
	//金币
	void SetGameGold(LONGLONG lGameGold){ m_lGameGold = lGameGold; }
	LONGLONG GetGameGold(){ return m_lGameGold; }

	//消费金币
	void SetInsureScore(LONGLONG lInsureScore){ m_lInsureScore = lInsureScore; }
	LONGLONG GetInsureScore(){ return m_lInsureScore; }

	//银行金币
	void SetBankScore(LONGLONG lBankScore){ m_lBankScore = lBankScore; }
	LONGLONG GetBankScore(){ return m_lBankScore; }

	//用户分数
	void SetScore(LONGLONG lScore){ m_lScore = lScore; }
	LONGLONG GetScore(){ return m_lScore; }

	//魅力值
	void SetCharm(LONGLONG lCharm){ m_lCharm = lCharm; }
	LONGLONG GetCharm(){ return m_lCharm; }

	//胜利盘数
	void SetWinCount(LONG lWinCount){ m_lWinCount = lWinCount; }
	LONG GetWinCount(){ return m_lWinCount; }

	//失败盘数
	void SetLostCount(LONG lLostCount){ m_lLostCount = lLostCount; }
	LONG GetLostCount(){ return m_lLostCount; }

	//和局盘数
	void SetDrawCount(LONG lDrawCount){ m_lDrawCount = lDrawCount; }
	LONG GetDrawCount(){ return m_lDrawCount; }

	//总盘局数
	LONG GetTotalCount(){ return m_lWinCount + m_lLostCount + m_lDrawCount; }

	//断线数目
	void SetFleeCount(LONG lFleeCount){ m_lFleeCount = lFleeCount; }
	LONG GetFleeCount(){ return m_lFleeCount; }

	//头像版本
	void SetCustomFaceVer(DWORD dwCustomFaceVer){ m_dwCustomFaceVer = dwCustomFaceVer; }
	DWORD GetCustomFaceVer(){ return m_dwCustomFaceVer; }
	
	//账号
	void SetAccount(LPCTSTR pzAccount){ ::StrToCharArray(m_szAccount, pzAccount); }
	LPCTSTR GetAccount(){ return m_szAccount; }

	//密码
	void SetPassword(LPCTSTR pzPassword){ ::StrToCharArray(m_szPassword, pzPassword); }
	LPCTSTR GetPassword(){ return m_szPassword; }
	
	//玩家昵称
	void SetNickName(LPCTSTR szNickname){ ::StrToCharArray(m_szNickname, szNickname); }
	LPCTSTR GetNickName(){ return m_szNickname; }

	//头像索引
	void SetFaceID(WORD wFaceID);
	WORD GetFaceID(){ return m_wFaceID; }

	//头像ICON
	void SetIcon(LPCTSTR szIcon){ ::StrToCharArray(m_szIcon, szIcon); }
	LPCTSTR GetIcon();

	//个性签名
	void SetUnderWrite(LPCTSTR szUnderWrite){ ::StrToCharArray(m_szUnderWrite, szUnderWrite); }
	LPCTSTR GetUnderWrite(){ return m_szUnderWrite; }

	//社团 I D
	void SetGroupID(DWORD dwGroupID){ m_dwGroupID = dwGroupID; }
	DWORD GetGroupID(){ return m_dwGroupID; }

	//社团名
	void SetGroupName(LPCTSTR szGroupName){ ::StrToCharArray(m_szGroupName, szGroupName); }
	LPCTSTR GetGroupName(){ return m_szGroupName; }

	//Unreal
	void SetUnrealUser(BYTE bUnreal){ m_cbUnreal = bUnreal; }
	BYTE IsUnrealUser(){ return m_cbUnreal; }

	// ------------------------------------------------------------
	// 房间信息

	//所在房间 I D
	void SetRoomID(WORD wRoomID){ m_wRoomID = wRoomID; }
	WORD GetRoomID(){ return m_wRoomID; }

	//所在桌子 I D
	void SetTableID(WORD wTableID){ m_wTableID = wTableID; }
	WORD GetTableID(){ return m_wTableID; }

	//所在座位 I D
	void SetChairID(WORD wChairID){ m_wChairID = wChairID; }
	WORD GetChairID(){ return m_wChairID; }

	//上次所在桌子 I D
	void SetLastTableID(WORD wLastTableID){ m_wLastTableID = wLastTableID; }
	WORD GetLastTableID(){ return m_wLastTableID; }

	//上次所在座位 I D
	void SetLastChairID(WORD wLastChairID){ m_wLastChairID = wLastChairID; }
	WORD GetLastChairID(){ return m_wLastChairID; }
	
	// ------------------------------------------------------------
	// 比赛

	//报名次数
	void SetSignUpTimes(WORD wSignUpTimes){ m_wSignUpTimes = wSignUpTimes; }
	WORD GetSignUpTimes(){ return m_wSignUpTimes; }

	//是否已报名
	void SetHaveSignUp(bool bHaveSignUp){ m_cbHaveSignUp = bHaveSignUp; }
	bool GetHaveSignUp(){ return m_cbHaveSignUp; }
	
	// ------------------------------------------------------------
	// 其他

	//锁定机器
	void SetMoorMachine(BYTE cbMoorMachine){ m_cbMoorMachine = cbMoorMachine; }
	bool IsMoorMachine(){ return m_cbMoorMachine != 0; }
	void SetMachineID(LPCTSTR szMachineID){ ::StrToCharArray(m_szMachineID, szMachineID); }
	LPCTSTR GetMachineID(){ return m_szMachineID; }

	//游戏设置
	void SetGameSetting(CMD_GR_UserRule& tGameRule){ memcpy(&m_uGameRule, &tGameRule, sizeof(CMD_GR_UserRule)); }
	const CMD_GR_UserRule& GetGameSetting(){ return m_uGameRule; }

	//保存游戏设置
	void SaveGameSetting();
	//加载游戏设置
	void ReadGameSetting();

	//更新玩家UI
	void UpdateUI(CPaintManagerUI* pManager);

	//清除玩家状态
	void CleanStatus();

	//填充IPC传输的用户结构
	UserStruct& GetUserStruct();

	//填充IPC传输的附加用户结构
	UserStructSuffix& GetUserStructSuffix();

private:
	// ------------------------------------------------------------
	// 基础信息
	WORD			m_wFaceID;								//头像索引
	DWORD			m_dwUserID;								//用户 I D
	DWORD			m_dwGameID;								//游戏 I D
	DWORD			m_dwGroupID;							//社团 I D
	DWORD			m_dwUserRight;							//玩家权限
	DWORD			m_dwExperience;							//用户经验
	DWORD			m_dwMasterRight;						//管理权限

	bool			m_bMasterSwitchOn;						//是否开启管理员检测
	BYTE			m_cbUnreal;								//Unreal
	BYTE			m_cbSelf;								//是否是自己
	BYTE			m_cbStatus;								//用户状态
	BYTE			m_cbLastStatus;							//用户上次状态
	BYTE			m_cbGender;								//用户性别(0 - 男, 1 - 女)
	BYTE			m_cbMemberOrder;						//会员等级
	BYTE			m_cbMasterOrder;						//管理员等级
	DWORD			m_dwMemberColor;						//对应会员列表字体颜色

	LONGLONG		m_lGameGold;							//金币
	LONGLONG		m_lBankScore;							//银行金币
	LONGLONG		m_lScore;								//用户分数
	LONGLONG		m_lCharm;								//魅力值
	LONGLONG		m_lInsureScore;							//消费金币

	LONG			m_lWinCount;							//胜利盘数
	LONG			m_lLostCount;							//失败盘数
	LONG			m_lDrawCount;							//和局盘数
	LONG			m_lFleeCount;							//断线数目

	//其他信息
	BOOL			m_cbRobot;								//机器人
	BYTE			m_cbCompanion;							//用户关系

	//比赛
	WORD			m_wSignUpTimes;							//报名次数			
	bool			m_cbHaveSignUp;							//是否已经报名

	//扩展信息
	DWORD			m_dwCustomFaceVer;						//头像版本
	BYTE			m_cbMoorMachine;						//锁定机器

	//其他信息
	TCHAR			m_szAccount[NAME_LEN];					//游戏账号
	TCHAR			m_szPassword[PWD_LEN];					//密码
	TCHAR			m_szPasswordEncrypt[PWD_LEN];			//加密后的密码
	TCHAR			m_szGroupName[GROUP_LEN];				//社团名
	TCHAR			m_szUnderWrite[UNDER_WRITE_LEN];		//个性签名
	TCHAR			m_szNickname[NAME_LEN];					//玩家昵称
	TCHAR			m_szIcon[MAX_PATH];						//玩家头像icon
	TCHAR			m_szMachineID[COMPUTER_ID_LEN];			//机器码
	TCHAR			m_szGender[GENDER_LEN];					//性别字符串
	TCHAR			m_szRobotPrefix[NAME_LEN];				//机器人昵称前缀

	// ------------------------------------------------------------
	// 房间信息
	int				m_wRoomID;								//玩家所在房间号
	WORD			m_wChairID;								//椅子位置
	WORD			m_wTableID;								//桌子号码
	WORD			m_wLastChairID;							//上次椅子位置
	WORD			m_wLastTableID;							//上次桌子号码
	
	CMD_GR_UserRule		m_uGameRule;						//游戏设置
	UserStruct			m_uStruct;							//玩家结构(用于IPC通信)
	UserStructSuffix	m_uStructSuffix;					//附加玩家结构(用于IPC通信)
};

// ------------------------------------------------------------
// 在线人数描述配置信息
#define  MAX_DESC_LEN	32
class OnlineDescConfig
{
public:
	OnlineDescConfig();
	~OnlineDescConfig();

	void SetOnlineCount(DWORD dwOnlineCount){ m_dwOnlineCount = dwOnlineCount; }
	DWORD GetOnlineCount(){ return m_dwOnlineCount; }

	void SetDescribe(std::string& pzDesc);
	TCHAR* GetDescribe(){ return m_pzOnlineDesc; }
	void SetStatus(int iStatus){ m_iStatus = iStatus; }
	int GetStatus(){ return m_iStatus; }

private:
	DWORD	m_dwOnlineCount;				// 在线人数
	TCHAR	m_pzOnlineDesc[MAX_DESC_LEN];	// 描述
	int		m_iStatus;						// 状态
};

// ------------------------------------------------------------
// 等级描述配置信息
class LevelConfig
{
public:
	LevelConfig();
	~LevelConfig();

	void SetLevel(DWORD dwLevel){ m_dwLevel = dwLevel; }
	DWORD GetLevel(){ return m_dwLevel; }

	void SetScore(LONGLONG lScore){ m_lScore = lScore; }
	LONGLONG GetScore(){ return m_lScore; }

	void SetDescribe(std::string& pzDesc);
	TCHAR* GetDescribe(){ return m_pzOrderDesc; }

private:
	DWORD		m_dwLevel;						// 等级
	LONGLONG	m_lScore;						// 积分
	TCHAR		m_pzOrderDesc[MAX_DESC_LEN];	// 描述
};


// ------------------------------------------------------------
// 经验值描述配置信息
class ExperinceConfig
{
public:
	ExperinceConfig();
	~ExperinceConfig();

	void SetExperince(DWORD dwLevel){ m_dwLevel = dwLevel; }
	DWORD GetExperince(){ return m_dwLevel; }

	void SetScore(LONGLONG lScore){ m_lScore = lScore; }
	LONGLONG GetScore(){ return m_lScore; }

private:
	DWORD		m_dwLevel;						// 等级
	LONGLONG	m_lScore;						// 积分
};

// ------------------------------------------------------------
// 房间场次配置信息
class RoomSeasonConfig
{
public:
	RoomSeasonConfig();
	~RoomSeasonConfig();

	void SetSeasonID(DWORD dwSeasonID){ m_dwSeasonID = dwSeasonID; }
	DWORD GetSeasonID(){ return m_dwSeasonID; }
	
	void SetSeasonName(std::string& pzName);
	TCHAR* GetSeasonName(){ return m_pzSeasonName; }

	void SetSeasonDescribe(std::string& pzDesc);
	TCHAR* GetSeasonDescribe(){ return m_pzSeasonDesc; }

private:
	DWORD	m_dwSeasonID;					// 场次对应 I D
	TCHAR	m_pzSeasonName[MAX_DESC_LEN];	// 场次名
	TCHAR	m_pzSeasonDesc[MAX_DESC_LEN];	// 场次描述内容
};

#endif // __USERDATA_H__