#pragma once

#ifndef __USERDATA_H__
#define __USERDATA_H__

#define GENDER_LEN	(5)

// -------------------------------------------------------------------------------------
// ��������Ϸ�е��û����ݽṹ
//#pragma pack(1)
struct UserStruct
{
	BYTE		pzName[NAME_LEN];		// �ǳ�
	BYTE		pzAccount[NAME_LEN];	// �˺�
	BYTE		pzFacePath[MAX_PATH];	// ͷ��·��
	BYTE		cbStatus;				// ���״̬
	BYTE		cbSelf;					// �Ƿ����Լ�
	BYTE		cbRobot;				// �Ƿ��ǻ�����
	BYTE		cbMemberOrder;			// ��Ա�ȼ�
	BYTE		cbMasterOrder;			// ����Ա�ȼ�
	DWORD		dwUserID;				// �û� I D
	DWORD		dwFaceID;				// ͷ�� I D
	DWORD		dwGameID;				// ��Ϸ��ʾ I D(����Ϸ������ID)
	DWORD		dwTableID;				// ���� I D
	DWORD		dwChairID;				// ���� I D
	LONGLONG	lGold;					// ���
	LONGLONG	lBankGold;				// ���н��
	LONGLONG	lScore;					// �û�����
	LONGLONG	lCharm;					// ����
};

// ׷�ӵ��û����ݽṹ
struct UserStructSuffix
{
	BYTE				cbGender;								// �û��Ա�

	DWORD				dwGroupID;								// ��������
	DWORD				dwExperience;							// �û�����
	
	LONG				lWinCount;								// ʤ������
	LONG				lLostCount;								// ʧ������
	LONG				lDrawCount;								// �;�����
	LONG				lFleeCount;								// ������Ŀ
	LONGLONG			llInsureScore;							// ���ѽ��
	
	BYTE				pzGroupName[GROUP_LEN];					// ��������
	BYTE				pzUnderWrite[UNDER_WRITE_LEN];			// ����ǩ��
	BYTE				pzLevelDesc[NAME_LEN];					// �ȼ�����
	BYTE				pzMachineID[COMPUTER_ID_LEN];			// ������
};

//#pragma pack()
// -------------------------------------------------------------------------------------

// �û�������
class UserData
{
public:

	UserData();
	~UserData();

	// ------------------------------------------------------------
	// ������Ϣ

	//�û� I D
	void SetUserID(DWORD dwUserID){ m_dwUserID = dwUserID; }
	const DWORD GetUserID(){ return m_dwUserID; }

	//���״̬
	void SetUserStatus(BYTE cbStatus){ m_cbStatus = cbStatus; }
	const BYTE GetUserStatus(){ return m_cbStatus; }

	//�����һ��״̬
	void SetLastUserStatus(BYTE cbLastStatus){ m_cbLastStatus = cbLastStatus; }
	const BYTE GetLastUserStatus(){ return m_cbLastStatus; }

	//�Ƿ��Ѿ���Ϸ��
	bool IsGaming(){ return m_cbStatus == US_PLAY; }

	//�Ƿ���׼����
	bool IsUserReady(){ return m_cbStatus == US_READY; }

	//���Ȩ��
	void SetUserRight(DWORD dwUserRight){ m_dwUserRight = dwUserRight; }
	DWORD GetUserRight(){ return m_dwUserRight; }

	//��Ա�ȼ�
	void SetMemberOrder(BYTE cbMemberOrder);
	const BYTE GetMemberOrder(){ return m_cbMemberOrder; }

	//��Ӧ�б�������ɫ
	const DWORD GetMemberOrderColor(){ return m_dwMemberColor; }

	//�Ƿ�������ԱȨ�޼��
	void SetMasterSwitchOn(bool bOpen){ m_bMasterSwitchOn = bOpen; }
	bool IsMasterSwitchOn(){ return m_bMasterSwitchOn; }

	//����Ȩ��
	void SetMasterRight(DWORD dwMasterRight){ m_dwMasterRight = dwMasterRight; }
	DWORD GetMasterRight(){ return m_dwMasterRight; }

	//����Ա�ȼ�
	void SetMasterOrder(BYTE cbMasterOrder){ m_cbMasterOrder = cbMasterOrder; }
	BYTE GetMasterOrder();
	BYTE GetRealMasterOrder(){ return m_cbMasterOrder; }

	//������
	void SetRobot(BOOL cbRobot){ m_cbRobot = cbRobot; }
	BOOL IsRobot(){ return m_cbRobot == TRUE; }

	//����������ǰ׺
	void SetRobotPrefix(LPCTSTR pzPrefix){ ::StrToCharArray(m_szRobotPrefix, pzPrefix); }
	LPCTSTR GetRobotPrefix(){ return m_szRobotPrefix; }

	//�û��Ա�
	void SetGender(BYTE cbGender);
	BYTE GetGender(){ return m_cbGender; }
	LPTSTR GetGenderStr(){ return m_szGender; }

	//����Ϊ�Լ�
	void SetSelf(BYTE bSelf){ m_cbSelf = bSelf; }
	BYTE IsSelf(){ return m_cbSelf; }

	//�û���Ϸ"��ʾ I D"
	void SetGameID(DWORD dwGameID){ m_dwGameID = dwGameID; }
	DWORD GetGameID(){ return m_dwGameID; }

	//�û�����
	void SetExperience(DWORD dwExperience){ m_dwExperience = dwExperience; }
	DWORD GetExperience(){ return m_dwExperience; }
	
	//���
	void SetGameGold(LONGLONG lGameGold){ m_lGameGold = lGameGold; }
	LONGLONG GetGameGold(){ return m_lGameGold; }

	//���ѽ��
	void SetInsureScore(LONGLONG lInsureScore){ m_lInsureScore = lInsureScore; }
	LONGLONG GetInsureScore(){ return m_lInsureScore; }

	//���н��
	void SetBankScore(LONGLONG lBankScore){ m_lBankScore = lBankScore; }
	LONGLONG GetBankScore(){ return m_lBankScore; }

	//�û�����
	void SetScore(LONGLONG lScore){ m_lScore = lScore; }
	LONGLONG GetScore(){ return m_lScore; }

	//����ֵ
	void SetCharm(LONGLONG lCharm){ m_lCharm = lCharm; }
	LONGLONG GetCharm(){ return m_lCharm; }

	//ʤ������
	void SetWinCount(LONG lWinCount){ m_lWinCount = lWinCount; }
	LONG GetWinCount(){ return m_lWinCount; }

	//ʧ������
	void SetLostCount(LONG lLostCount){ m_lLostCount = lLostCount; }
	LONG GetLostCount(){ return m_lLostCount; }

	//�;�����
	void SetDrawCount(LONG lDrawCount){ m_lDrawCount = lDrawCount; }
	LONG GetDrawCount(){ return m_lDrawCount; }

	//���̾���
	LONG GetTotalCount(){ return m_lWinCount + m_lLostCount + m_lDrawCount; }

	//������Ŀ
	void SetFleeCount(LONG lFleeCount){ m_lFleeCount = lFleeCount; }
	LONG GetFleeCount(){ return m_lFleeCount; }

	//ͷ��汾
	void SetCustomFaceVer(DWORD dwCustomFaceVer){ m_dwCustomFaceVer = dwCustomFaceVer; }
	DWORD GetCustomFaceVer(){ return m_dwCustomFaceVer; }
	
	//�˺�
	void SetAccount(LPCTSTR pzAccount){ ::StrToCharArray(m_szAccount, pzAccount); }
	LPCTSTR GetAccount(){ return m_szAccount; }

	//����
	void SetPassword(LPCTSTR pzPassword){ ::StrToCharArray(m_szPassword, pzPassword); }
	LPCTSTR GetPassword(){ return m_szPassword; }
	
	//����ǳ�
	void SetNickName(LPCTSTR szNickname){ ::StrToCharArray(m_szNickname, szNickname); }
	LPCTSTR GetNickName(){ return m_szNickname; }

	//ͷ������
	void SetFaceID(WORD wFaceID);
	WORD GetFaceID(){ return m_wFaceID; }

	//ͷ��ICON
	void SetIcon(LPCTSTR szIcon){ ::StrToCharArray(m_szIcon, szIcon); }
	LPCTSTR GetIcon();

	//����ǩ��
	void SetUnderWrite(LPCTSTR szUnderWrite){ ::StrToCharArray(m_szUnderWrite, szUnderWrite); }
	LPCTSTR GetUnderWrite(){ return m_szUnderWrite; }

	//���� I D
	void SetGroupID(DWORD dwGroupID){ m_dwGroupID = dwGroupID; }
	DWORD GetGroupID(){ return m_dwGroupID; }

	//������
	void SetGroupName(LPCTSTR szGroupName){ ::StrToCharArray(m_szGroupName, szGroupName); }
	LPCTSTR GetGroupName(){ return m_szGroupName; }

	//Unreal
	void SetUnrealUser(BYTE bUnreal){ m_cbUnreal = bUnreal; }
	BYTE IsUnrealUser(){ return m_cbUnreal; }

	// ------------------------------------------------------------
	// ������Ϣ

	//���ڷ��� I D
	void SetRoomID(WORD wRoomID){ m_wRoomID = wRoomID; }
	WORD GetRoomID(){ return m_wRoomID; }

	//�������� I D
	void SetTableID(WORD wTableID){ m_wTableID = wTableID; }
	WORD GetTableID(){ return m_wTableID; }

	//������λ I D
	void SetChairID(WORD wChairID){ m_wChairID = wChairID; }
	WORD GetChairID(){ return m_wChairID; }

	//�ϴ��������� I D
	void SetLastTableID(WORD wLastTableID){ m_wLastTableID = wLastTableID; }
	WORD GetLastTableID(){ return m_wLastTableID; }

	//�ϴ�������λ I D
	void SetLastChairID(WORD wLastChairID){ m_wLastChairID = wLastChairID; }
	WORD GetLastChairID(){ return m_wLastChairID; }
	
	// ------------------------------------------------------------
	// ����

	//��������
	void SetSignUpTimes(WORD wSignUpTimes){ m_wSignUpTimes = wSignUpTimes; }
	WORD GetSignUpTimes(){ return m_wSignUpTimes; }

	//�Ƿ��ѱ���
	void SetHaveSignUp(bool bHaveSignUp){ m_cbHaveSignUp = bHaveSignUp; }
	bool GetHaveSignUp(){ return m_cbHaveSignUp; }
	
	// ------------------------------------------------------------
	// ����

	//��������
	void SetMoorMachine(BYTE cbMoorMachine){ m_cbMoorMachine = cbMoorMachine; }
	bool IsMoorMachine(){ return m_cbMoorMachine != 0; }
	void SetMachineID(LPCTSTR szMachineID){ ::StrToCharArray(m_szMachineID, szMachineID); }
	LPCTSTR GetMachineID(){ return m_szMachineID; }

	//��Ϸ����
	void SetGameSetting(CMD_GR_UserRule& tGameRule){ memcpy(&m_uGameRule, &tGameRule, sizeof(CMD_GR_UserRule)); }
	const CMD_GR_UserRule& GetGameSetting(){ return m_uGameRule; }

	//������Ϸ����
	void SaveGameSetting();
	//������Ϸ����
	void ReadGameSetting();

	//�������UI
	void UpdateUI(CPaintManagerUI* pManager);

	//������״̬
	void CleanStatus();

	//���IPC������û��ṹ
	UserStruct& GetUserStruct();

	//���IPC����ĸ����û��ṹ
	UserStructSuffix& GetUserStructSuffix();

private:
	// ------------------------------------------------------------
	// ������Ϣ
	WORD			m_wFaceID;								//ͷ������
	DWORD			m_dwUserID;								//�û� I D
	DWORD			m_dwGameID;								//��Ϸ I D
	DWORD			m_dwGroupID;							//���� I D
	DWORD			m_dwUserRight;							//���Ȩ��
	DWORD			m_dwExperience;							//�û�����
	DWORD			m_dwMasterRight;						//����Ȩ��

	bool			m_bMasterSwitchOn;						//�Ƿ�������Ա���
	BYTE			m_cbUnreal;								//Unreal
	BYTE			m_cbSelf;								//�Ƿ����Լ�
	BYTE			m_cbStatus;								//�û�״̬
	BYTE			m_cbLastStatus;							//�û��ϴ�״̬
	BYTE			m_cbGender;								//�û��Ա�(0 - ��, 1 - Ů)
	BYTE			m_cbMemberOrder;						//��Ա�ȼ�
	BYTE			m_cbMasterOrder;						//����Ա�ȼ�
	DWORD			m_dwMemberColor;						//��Ӧ��Ա�б�������ɫ

	LONGLONG		m_lGameGold;							//���
	LONGLONG		m_lBankScore;							//���н��
	LONGLONG		m_lScore;								//�û�����
	LONGLONG		m_lCharm;								//����ֵ
	LONGLONG		m_lInsureScore;							//���ѽ��

	LONG			m_lWinCount;							//ʤ������
	LONG			m_lLostCount;							//ʧ������
	LONG			m_lDrawCount;							//�;�����
	LONG			m_lFleeCount;							//������Ŀ

	//������Ϣ
	BOOL			m_cbRobot;								//������
	BYTE			m_cbCompanion;							//�û���ϵ

	//����
	WORD			m_wSignUpTimes;							//��������			
	bool			m_cbHaveSignUp;							//�Ƿ��Ѿ�����

	//��չ��Ϣ
	DWORD			m_dwCustomFaceVer;						//ͷ��汾
	BYTE			m_cbMoorMachine;						//��������

	//������Ϣ
	TCHAR			m_szAccount[NAME_LEN];					//��Ϸ�˺�
	TCHAR			m_szPassword[PWD_LEN];					//����
	TCHAR			m_szPasswordEncrypt[PWD_LEN];			//���ܺ������
	TCHAR			m_szGroupName[GROUP_LEN];				//������
	TCHAR			m_szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��
	TCHAR			m_szNickname[NAME_LEN];					//����ǳ�
	TCHAR			m_szIcon[MAX_PATH];						//���ͷ��icon
	TCHAR			m_szMachineID[COMPUTER_ID_LEN];			//������
	TCHAR			m_szGender[GENDER_LEN];					//�Ա��ַ���
	TCHAR			m_szRobotPrefix[NAME_LEN];				//�������ǳ�ǰ׺

	// ------------------------------------------------------------
	// ������Ϣ
	int				m_wRoomID;								//������ڷ����
	WORD			m_wChairID;								//����λ��
	WORD			m_wTableID;								//���Ӻ���
	WORD			m_wLastChairID;							//�ϴ�����λ��
	WORD			m_wLastTableID;							//�ϴ����Ӻ���
	
	CMD_GR_UserRule		m_uGameRule;						//��Ϸ����
	UserStruct			m_uStruct;							//��ҽṹ(����IPCͨ��)
	UserStructSuffix	m_uStructSuffix;					//������ҽṹ(����IPCͨ��)
};

// ------------------------------------------------------------
// ������������������Ϣ
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
	DWORD	m_dwOnlineCount;				// ��������
	TCHAR	m_pzOnlineDesc[MAX_DESC_LEN];	// ����
	int		m_iStatus;						// ״̬
};

// ------------------------------------------------------------
// �ȼ�����������Ϣ
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
	DWORD		m_dwLevel;						// �ȼ�
	LONGLONG	m_lScore;						// ����
	TCHAR		m_pzOrderDesc[MAX_DESC_LEN];	// ����
};


// ------------------------------------------------------------
// ����ֵ����������Ϣ
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
	DWORD		m_dwLevel;						// �ȼ�
	LONGLONG	m_lScore;						// ����
};

// ------------------------------------------------------------
// ���䳡��������Ϣ
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
	DWORD	m_dwSeasonID;					// ���ζ�Ӧ I D
	TCHAR	m_pzSeasonName[MAX_DESC_LEN];	// ������
	TCHAR	m_pzSeasonDesc[MAX_DESC_LEN];	// ������������
};

#endif // __USERDATA_H__