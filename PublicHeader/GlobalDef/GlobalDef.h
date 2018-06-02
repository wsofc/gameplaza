
#pragma once

#include "../SafeDelete.h"
#include "GlobalSocket.h"
#include "GlobalIpc.h"

//////////////////////////////////////////////////////////////////////////
//��������
#define MACHINE_PC						0							// ����
#define MACHINE_IOS						1							// ƻ��
#define MACHINE_ANDROID			2							// ��׿

#define MAX_CHAIR							100							//�������
#define MAX_CHAIR_NORMAL			8								//�������

#define MAX_ROBOT						550								//������������������
#define MAX_CHAT_LEN					200								//���쳤��
#define LIMIT_CHAT_TIMES				1200								//��ʱ����
#define MAX_SYSMESSAGE_LEN		1024								//ϵͳ��Ϣ����

/////////////////////////////////////////////////////////////////////////////////////////
//�궨��

//�˿ڶ���
#define PORT_VIDEO_SERVER					7600								//��Ƶ������
#define PORT_LOGON_SERVER				9001								//��½������
#define PORT_CENTER_SERVER				9010								//���ķ�����
#define PORT_HALL_SERVER_BASE			9020								//����������


//////////////////////////////////////////////////////////////////////////

//���Ⱥ궨��
#define TYPE_LEN							32									//���೤��
#define KIND_LEN						32									//���ͳ���
#define STATION_LEN					32									//վ�㳤��
#define SERVER_LEN						32									//���䳤��
#define MODULE_LEN					32									//���̳���
#define SET_NUMBER_LEN			64									//�ֺų���
#define RESULT_CARD_TYPE			128								//����������������

//�Ա���
#define GENDER_NULL						0									//δ֪�Ա�
#define GENDER_BOY						1									//�����Ա�
#define GENDER_GIRL						2									//Ů���Ա�

//�����������
enum emRoomServiceType
{
	ROOM_SERVICE_TYPE_ERROR		= 0x0000,			//��Ч����	
	ROOM_SERVICE_TYPE_SCORE		= 0x0001,			//��ֵ����	
	ROOM_SERVICE_TYPE_GOLD		= 0x0002,			//�������
	ROOM_SERVICE_TYPE_DAY_MATCH	= 0x0004,		//ÿ����
};

//��Ϸ���ͽṹ
struct tagGameType
{
	tagGameType()
	{
		memset(this,0,sizeof(*this));
	}
	WORD			wSortID;									//�������
	WORD			wTypeID;									//�������
	TCHAR			szTypeName[TYPE_LEN];			//��������
};

//��Ϸ���ƽṹ
struct tagGameKind
{
	tagGameKind()
	{
		memset(this,0,sizeof(*this));
	}
	WORD			wSortID;							//�������
	WORD			wTypeID;							//���ͺ���
	WORD			wKindID;							//���ƺ���
	DWORD			dwMaxVersion;						//���°汾
	DWORD			dwOnlineCount;						//������Ŀ
	TCHAR			szKindName[KIND_LEN];				//��Ϸ����
	TCHAR			szClientModuleName[MODULE_LEN];		//�ͻ���ģ������
};

//��Ϸվ��ṹ
struct tagGameStation
{
	tagGameStation()
	{
		memset(this,0,sizeof(*this));
	}

	WORD			wSortID;							//�������
	WORD			wKindID;							//���ƺ���
	WORD			wJoinID;							//�ҽӺ���
	WORD			wStationID;							//վ�����
	TCHAR			szStationName[STATION_LEN];			//վ������
};

//ÿ����
struct tagDayMatchOption
{
	tagDayMatchOption()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwMatchID;				//��������ID

	BYTE		cbMatchStartTime;		//��ʼʱ��
	BYTE		cbMatchEndTime;			//����ʱ��
	WORD		wSetCnt;				//��������	

	DWORD		dwSignUpHappyBeanCnt;	//�۳����ֶ�����
	DWORD		dwSignUpMatchQuanCnt;	//�۳�����ȯ����
};

//��Ϸ�����б�ṹ
struct tagGameServer
{
	tagGameServer()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wSortID;							//�������
	WORD		wKindID;							//���ƺ���
	WORD		wServerID;							//�������
	WORD		wStationID;							//վ�����
	WORD		wServerPort;						//����˿�
	WORD		wRoomServiceType;					//��������
	LONG		lCellScore;			
	DWORD		dwServerAddr;						//�����ַ
	DWORD		dwOnlineCount;						//��������
	TCHAR		szServerName[SERVER_LEN];			//��������

	tagDayMatchOption DayMatchOption;				//����ѡ��

	//��������
	WORD		wTaxRate;							//��Ϸ˰��
	LONGLONG	lLessScore;							//��ͻ���
	LONGLONG	lMaxScore;							//��߻���
	LONGLONG	lRestrictScore;						//���ƻ���
	bool				bMobileRoom;					// �ֻ�����
};

//��½������
struct tagLogonServer
{
	tagLogonServer(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	DWORD			dwStuff;
};

//����������
struct tagHallServer
{
	tagHallServer(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}

	WORD			wServerID;					//ID
	WORD			wServerPort;				//�˿�
	DWORD			dwServerAddr;				//��ַ
	DWORD			dwOnlineCount;				//��������
};


//��Ϸ����ṹ
struct tagLevelItem
{
	LONG								lLevelScore;						//�������
	WCHAR								szLevelName[16];					//��������
};
//////////////////////////////////////////////////////////////////////////

//�û�״̬����
#define US_NULL							0x00							//û��״̬
#define US_FREE							0x01							//վ��״̬
#define US_SIT								0x02							//����״̬
#define US_READY						0x03							//ͬ��״̬
#define US_LOOKON					0x04							//�Թ�״̬
#define US_PLAY							0x05							//��Ϸ״̬
#define US_OFFLINE						0x06							//����״̬

//���Ⱥ궨��
#define NAME_LEN						32								//���ֳ���
#define PWD_LEN							33								//���볤��
#define EMAIL_LEN						32								//���䳤��
#define GROUP_LEN						32								//���ų���
#define COMPUTER_ID_LEN			33								//��������
#define UNDER_WRITE_LEN			32								//����ǩ��
#define PHONE_LEN						15								//�ֻ���
#define IDCARD_LEN					20								//���֤
#define SMS_LEN							6								//������֤��
#define INFO_DESC_LEN				20								//������Ϣ����
#define PROPS_NAME					20								//��������

//�û�������Ϣ
struct tagUserScore
{
	tagUserScore()
	{
		memset(this,0,sizeof(*this));
	}
	LONGLONG							lScore;								//�û�����
	LONGLONG							lGameGold;						//��Ϸ���
	LONGLONG							lBankScore;						//�洢���
	LONG								lWinCount;							//ʤ������
	LONG								lLostCount;							//ʧ������
	LONG								lDrawCount;							//�;�����
	LONG								lFleeCount;							//������Ŀ
	DWORD							dwExperience;						//�û�����
};

//�û�״̬��Ϣ
struct tagUserStatus
{
	WORD								wTableID;							//���Ӻ���
	WORD								wChairID;							//����λ��
	BYTE								cbUserStatus;						//�û�״̬
};

//�û�������Ϣ�ṹ
struct tagUserInfoHead
{
	//�û�����
	WORD									wFaceID;							//ͷ������
	DWORD								dwUserID;							//�û� I D
	DWORD								dwGameID;						//��Ϸ I D
	DWORD								dwGroupID;						//��������
	DWORD								dwUserRight;					//�û��ȼ�
	LONGLONG							lCharm;								//�û�����
	DWORD								dwMasterRight;					//����Ȩ��

	//�û�����
	BYTE								cbGender;						    	//�û��Ա�
	BYTE								cbMemberOrder;					//��Ա�ȼ�
	BYTE								cbMasterOrder;						//����ȼ�
	BOOL							bRobot;								//������

	//�û�״̬
	WORD								wTableID;							//���Ӻ���
	WORD								wChairID;							//����λ��
	BYTE								cbUserStatus;						//�û�״̬

	//�û�����
	tagUserScore						UserScoreInfo;						//������Ϣ

	//��չ��Ϣ
	//LONG								lInsureScore;						//���ѽ��
	//LONG								lGameGold;						//��Ϸ���
	DWORD								dwCustomFaceVer;			//�ϴ�ͷ��
};

//�û�������Ϣ�ṹ
struct tagUserInfoHeadMatch
{
	tagUserInfoHeadMatch()
	{
		memset(this,0,sizeof(*this));
	}	
	//�û�����
	WORD					wFaceID;								//ͷ������
	DWORD				dwUserID;								//�û� I D
	DWORD				dwGameID;							//��Ϸ I D
	DWORD				dwGroupID;							//��������
	DWORD				dwUserRight;						//�û��ȼ�
	LONG					lCharm;									//�û�����
	DWORD				dwMasterRight;						//����Ȩ��

	//�û�����
	BYTE				cbGender;									//�û��Ա�
	BYTE				cbMemberOrder;						//��Ա�ȼ�
	BYTE				cbMasterOrder;							//����ȼ�

	//�û�״̬
	WORD				wTableID;							//���Ӻ���
	WORD				wChairID;							//����λ��
	BYTE				cbUserStatus;						//�û�״̬

	//�û�����
	tagUserScore		UserScoreInfo;						//������Ϣ

	//��չ��Ϣ
	//LONG				lInsureScore;						//���ѽ��
	//LONG				lGameGold;							//��Ϸ���
	DWORD				dwCustomFaceVer;					//�ϴ�ͷ��

	//������Ϣ
	BYTE				cbHaveSignUp;						
	WORD				wSignUpTimes;	
	WORD				wSetCnt;	
};


//�û���Ϣ�ṹ
struct tagUserData
{
	tagUserData()
	{
		memset(this,0,sizeof(*this));
	}
	//�û�����
	WORD					wFaceID;									//ͷ������
	DWORD				dwCustomFaceVer;					//�ϴ�ͷ��
	DWORD				dwUserID;									//�û� I D
	DWORD				dwGroupID;								//��������
	DWORD				dwGameID;								//��Ϸ I D
	DWORD				dwUserRight;							//�û��ȼ�
	LONGLONG			lCharm;										//�û�����
	DWORD				dwMasterRight;							//����Ȩ��
	TCHAR					szNickname[NAME_LEN];			//�ǳ�
	TCHAR					szGroupName[GROUP_LEN];				//��������
	TCHAR					szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//�û�����
	BYTE				cbGender;									//�û��Ա�
	BYTE				cbMemberOrder;						//��Ա�ȼ�
	BYTE				cbMasterOrder;							//����ȼ�
	BOOL			bRobot;									//������

	//�û�����
	LONGLONG			lInsureScore;						//���ѽ��
	LONGLONG			lGameGold;						//��Ϸ���
	LONGLONG			lScore;								//�û�����
	LONG					lWinCount;						//ʤ������
	LONG					lLostCount;						//ʧ������
	LONG					lDrawCount;						//�;�����
	LONG					lFleeCount;						//������Ŀ
	DWORD				dwExperience;					//�û�����

	//�û�״̬
	WORD				wTableID;							//���Ӻ���
	WORD				wChairID;							//����λ��
	BYTE				cbUserStatus;						//�û�״̬

	//������Ϣ
	BYTE				cbCompanion;						//�û���ϵ

	//����
	WORD				wSignUpTimes;						//��������			
	WORD				wSetCnt;							//����				
	bool				cbHaveSignUp;						//�Ƿ��Ѿ�����
};

//////////////////////////////////////////////////////////////////////////

//���û���ṹ
struct tagOptionBuffer
{
	BYTE								cbBufferLen;						//���ݳ���
	BYTE								cbOptionBuf[32];					//���û���
};

//////////////////////////////////////////////////////////////////////////




//-----------------------------------------------
//					the end
//-----------------------------------------------
