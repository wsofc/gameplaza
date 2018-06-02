/*
���ķ�������ID  0x1000


��½��������ID  0x2000


��Ϸ��������ID  0x3000


������������ID  0x4000


*/

/*

REQ -- request ����
ANW -- answer  Ӧ��

*/

#pragma once

#include "GlobalDef\GlobalDef.h"


#define	DATABASE_DEFAULT_PORT		1433
#define	LESS_CONNECT_COUNT			50							//��������
#define	PORT_DATABASE_SERVER		9050

#define	CENTER_SERVER_SERVICE_ID	100
#define	DATABASE_SERVER_SERVICE_ID	200


//���ݿ�����
const TCHAR	 g_szUserDB[]			= TEXT("QPUserDB");			//�û����ݿ�
const TCHAR	g_szServerInfoDB[]	= TEXT("QPServerInfoDB");	//�������ݿ�
const TCHAR	g_szTreasureDB[]		= TEXT("QPTreasureDB");		//�Ƹ����ݿ�
const TCHAR	g_szScoreDB[]			= TEXT("QPScoreDB");		//�������ݿ�
const TCHAR	g_szMatchDB[]			= TEXT("QPMatchDB");		//�������ݿ�



#define	LOGON_SERVER_MAINTANENCE_MESSAGE	_T("ϵͳ����ά��,�����Ժ����Ե�½.лл���Ĺ���.")


//����״̬
enum emServiceStatus
{
	SERVICE_STATUS_STOP,		//ֹͣ״̬
	SERVICE_STATUS_SERVICE,		//����״̬
};

//���ݿ���Ϣ
struct tagDatabaseInfo
{
	tagDatabaseInfo()
	{
		memset(this,0,sizeof(*this));
	}
	WORD		wPort;				//���ݿ�˿�
	TCHAR		szAddress[16];		//���ݿ��ַ
	TCHAR		szUser[32];			//���ݿ��û�
	TCHAR		szPwd[32];			//���ݿ�����
	TCHAR		szName[32];			//���ݿ�����
	TCHAR		szDoMain[50];		//���ݿ�����
	bool		bPriorityIP;		//����IP
};


//���ݿ���Ϣ
struct tagDatabaseInfoEx
{
	tagDatabaseInfoEx()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wPort;				//���ݿ�˿�
	DWORD		dwAddress;			//���ݿ��ַ
	TCHAR		szUser[32];			//���ݿ��û�
	TCHAR		szPwd[32];			//���ݿ�����
	TCHAR		szName[32];			//���ݿ�����
};


//��Ϸ��������
struct tagGameServiceAttrib
{
	tagGameServiceAttrib()
	{
		memset(this,0,sizeof(*this));
	}

	WORD		wKindID;							//���ƺ���
	WORD		wChairCount;						//������Ŀ
	BYTE		cbJoinInGame;						//��Ϸ����
	TCHAR		szDatabaseName[32];					//��Ϸ����
	TCHAR		szDescription[128];					//ģ������
	TCHAR		szKindName[KIND_LEN];				//��������
	TCHAR		szServerModuleName[MODULE_LEN];		//ģ������
	TCHAR		szClientModuleName[MODULE_LEN];		//ģ������
};

//��Ϸ��������
struct tagGameServiceOption
{
	tagGameServiceOption()
	{
		memset(this,0,sizeof(*this));
	}

	//��������
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wSortID;							//�����ʶ
	WORD							wNodeID;							//վ�����
	WORD							wServerID;							//�������
	WORD							wTableCount;						//������Ŀ
	WORD							wServerPort;						//����˿�
	WORD							wRoomServiceType;					//��������
	WORD							wMaxConnect;						//�������
	DWORD							dwServerAddr;						//�����ַ
	DWORD							dwVideoServer;						//��Ƶ��ַ
	DWORD							dwDatabaseAddr;						//���ݵ�ַ
	TCHAR							szDatabaseName[SERVER_LEN];			//��������
	TCHAR							szGameRoomName[SERVER_LEN];			//��������

	TCHAR							szDatabaseDomain[50];				//��������
	TCHAR							szServerDomain[50];					//��������
	bool							m_bDatabasePriorityIP;				//
	bool							m_bServerPriorityIP;				
	bool							bMobileRoom;						// �Ƿ����ֻ�����

	//��������
	WORD							wTaxRate;							//��Ϸ˰��
	LONG							lCellScore;							//��λ����
	LONGLONG						lLessScore;							//��ͻ���
	LONGLONG						lMaxScore;							//��߻���
	LONGLONG						lRestrictScore;						//���ƻ���

	//��չ����
	BYTE							cbHideUserInfo;						//������Ϣ
	BYTE							cbUnLookOnTag;						//�����Թ�
	BYTE							cbUnSameIPPlay;						//����ͬIP

	LONG							lMatchDraw;
	BYTE							cbControlStart;

	tagDayMatchOption				DayMatchOption;
};

//��������
enum emSignUpType
{
	SIGN_UP_TYPE_ERROR		,
	SIGN_UP_TYPE_HAPPY_BEAN	,	//���ֶ�
	SIGN_UP_TYPE_MATCH_QUAN	,	//����ȯ
};


//��־ѡ��
enum emLogOption
{
	LOG_OPTION_LOGON		= 0x00000001,		//��½
	LOG_OPTION_SYS_MESSAGE	= 0x00000002,		//ϵͳ��Ϣ
	LOG_OPTION_ROBOT		= 0x00000004,		//������
	LOG_OPTION_GAME_SERVER	= 0x00000008,		//��Ϸ������
	LOG_OPTION_DAY_MATCH	= 0x00000010,
};


//�����û�״̬
enum emMatchUserState
{
	MATCH_USER_STATE_ERROR		,
	MATCH_USER_STATE_SIGN_UP	,	//����
	MATCH_USER_STATE_GAME_NOW	,	//������Ϸ
	MATCH_USER_STATE_EXIT		,	//�˳�
	MATCH_USER_STATE_FINISH		,	//���
};





//-----------------------------------------------
//					the end
//-----------------------------------------------
