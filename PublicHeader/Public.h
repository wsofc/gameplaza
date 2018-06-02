

#pragma once



const TCHAR	g_szProductName[]	= TEXT("wan1979");					//��Ʒ����
const TCHAR g_szSystemRegKey[]	= TEXT("wan1979");					//ע������
const TCHAR g_szStationPage[]	= TEXT("www.wan1979.com");			//��Ϸ��ҳ
const TCHAR g_szCookieUrl[]		= TEXT("http://www.wan1979.com");		///��¼��ַ

#define PLAZA_MAIN_WND_CLASS_NAME	TEXT("hhw1688.GamePlaza")			//��������


const TCHAR g_szExitPlazaInfo[]		= TEXT("6168���������ʣ�����");		//�뿪������ʾ��
const TCHAR g_szLogonWindowText[]	= TEXT("6168");					//��½�򴰿ڱ���



#define	FILE_LOG_FOLDER				_T("Log")
#define	AUTO_RUN					_T("AUTO")
#define	CONFIG_FOLDER				_T("Config")


#define	INVALID_MATCH_GROUP_ID		0xFF
#define TIMER_REPEAT_TIMER			DWORD(-1)							//�ظ�����

#define	DEFAULT_TAX_RATE			5		//ǧ�ֱ�

#define	_W_							10000

#define MAX_AREA_CNT				11     //������Ϸ�����������
#define MAX_CARD_TYPE_CNT			5	   //������Ϸ�����������


//��ע��ϸ�ܽڵ�
#define NAME_NT_BET_DETAIL				_T("BetDetail")
//��ע���ڵ�
#define NAME_NT_AREA_BET_S				_T("Areas")
#define NAME_NT_AREA_BET				_T("Area")
#define NAME_NT_AREA_BET_ID				_T("ID")
#define NAME_NT_AREA_BET_NAME			_T("Name")
#define NAME_NT_AREA_BET_MULTIPLE		_T("Odds")
#define NAME_NT_AREA_BET_GOLD			_T("Bet")
#define NAME_NT_AREA_BET_PAYOUT			_T("Payout")

//��Ͷע�����ɲ�
#define NAME_NT_AREA_ALL_BET			_T("TotalBet")
#define NAME_NT_AREA_ALL_PAYOUT			_T("TotalPayout")

//������ڵ�
#define NAME_NT_CARD_RESULT				_T("GameResult")	
#define NAME_NT_CARD_S					_T("Areas")
#define NAME_NT_CARD					_T("Area")
#define NAME_NT_CARD_ID					_T("ID")
#define NAME_NT_CARD_TYPE				_T("Name")
#define NAME_NT_CARD_VALUE				_T("Result")

//����������
enum emServerType
{
	SERVER_TYPE_ERROR	,
	SERVER_TYPE_CENTER	,	//���ķ�����
	SERVER_TYPE_LOGON	,	//��½������
	SERVER_TYPE_HALL	,	//����������
	SERVER_TYPE_GAME	,	//��Ϸ������
	SERVER_TYPE_DATEBASE,	//���ݿ������
};

////��������
//enum emNetType
//{
//	NET_TYPE_ERROR		,
//	NET_TYPE_DIAN_XIN	,	//����
//	NET_TYPE_WANG_TONG	,	//��ͨ
//};

//�����ַ����
enum emAddrType
{
	ADDR_TYPE_ERROR		,
	ADDR_TYPE_DOMAIN	,	//����
	ADDR_TYPE_IP		,	//IP
};

//�������λ��
enum emPlayerLocation
{
	PLAYER_LOCATION_ERROR,
	PLAYER_LOCATION_ROOM = 0x01,
	PLAYER_LOCATION_GAME = 0x02,
};

//��Ա�ȼ�
enum emMemberOrder
{
	MEMBER_ORDER_NORMAL	= 0,	//��ͨ��Ա
	MEMBER_ORDER_VIP    = 10,	//VIP��Ա
};

//��������
enum emPwdType
{
	PWD_TYPE_ERROR	,
	PWD_TYPE_LOGON	= 0x01,	//��¼����
	PWD_TYPE_BANK	= 0x02,	//��������
};

//������Ʒ
struct tagMatchPrize
{
	tagMatchPrize()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wExperienceCount;					//������Ŀ
	WORD		wMedalCount;						//������Ŀ
	LONG		lGoldCount;							//�����Ŀ
};

//�ֺ�
struct tagSetNumber
{
	tagSetNumber(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	//��Ϸ������λ
	WORD	wKindID;
	WORD	wServerID;
	WORD	wTableID;

	//��������,���һ�����������������
	//Ϊ��֤�ֺ�Ψһ
	WORD	wGameServerStartTimes;

	//�����ն�λ
	WORD	wYear;
	WORD	wMonth;
	WORD	wDay;
	//ϵͳʱ�������
	LONGLONG lMilliseconds;
	
	//���
	WORD	wIndex;
};

//��ע��ϸ
struct tagBetDetail
{
	tagBetDetail(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	BYTE		cbAreaCnt;							//��������
	TCHAR		szAreaName[MAX_AREA_CNT][32];		//����������
	TCHAR		szAreaMultiple[MAX_AREA_CNT][32];	//��������
	LONGLONG	lAreaBet[MAX_AREA_CNT];				//��������ע��
	LONGLONG	lAreaPayout[MAX_AREA_CNT];			//�������ɲʶ�

};

//���ֿ������ͼ���ֵ
struct tagCardResult
{
	tagCardResult(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	BYTE		cbCardTypeCnt;							//��������
	TCHAR		szAreaCardType[MAX_CARD_TYPE_CNT][32];	//����������
	TCHAR		szAreaCardValue[MAX_CARD_TYPE_CNT][32];	//��������ֵ
	LONGLONG	lTotalBet;								//�������������ע�ϼ�
	LONGLONG	lTotalPayout;							//������������ɲʺϼ�
};

//��֤Sub IDΨһ
#define	GAME_SUB_ID_SERVER_MIN			1000
#define	GAME_SUB_ID_CLIENT_MIN			1
#define	GAME_SUB_ID_CLIENT_MAX			999



//-----------------------------------------------
//					the end
//-----------------------------------------------
