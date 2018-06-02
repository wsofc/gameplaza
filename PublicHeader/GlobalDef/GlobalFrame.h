#ifndef GLOBAL_FRAME_HEAD_FILE
#define GLOBAL_FRAME_HEAD_FILE

#pragma once

#include "GlobalDef.h"
#include "GlobalProperty.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ״̬
#define GAME_SCENE_FREE								0							//����״̬
#define GAME_SCENE_PLAYING						100							//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//IPC �����¼�

#define IPC_MAIN_SOCKET					1								//������Ϣ

#define IPC_SUB_SOCKET_SEND				1								//���緢��
#define IPC_SUB_SOCKET_RECV				2								//�������

//IPC ������ṹ
struct IPC_SocketPackage
{
	CMD_Command							Command;						//������Ϣ
	BYTE								cbBuffer[TCP_PACKET_BUFFER_SIZE];		//���ݻ���
};

//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_CONFIG					2								//������Ϣ

#define IPC_SUB_SERVER_INFO				1								//������Ϣ
#define IPC_SUB_COLUMN_INFO				2								//�б���Ϣ

//��Ϸ��Ϣ
struct IPC_GF_ServerInfo
{
	DWORD		dwUserID;						//�û� I D
	WORD		wTableID;						//���Ӻ���
	WORD		wChairID;						//���Ӻ���
	WORD		wKindID;						//���ͱ�ʶ
	WORD		wServerID;						//�����ʶ
	WORD		wRoomServiceType;				//��Ϸ����
	WORD		wChairCount;					//������Ŀ
	BYTE		cbHideUserInfo;					//������Ϣ
	DWORD		dwVideoAddr;					//��Ƶ��ַ
	TCHAR		szKindName[KIND_LEN];			//��������
	TCHAR		szServerName[SERVER_LEN];		//��������
};

//////////////////////////////////////////////////////////////////////////
//IPC �û���Ϣ

#define IPC_MAIN_USER					3								//�û���Ϣ

#define IPC_SUB_USER_CONFIG				0								//�û�����
#define IPC_SUB_USER_COME				1								//�û���Ϣ
#define IPC_SUB_USER_STATUS				2								//�û�״̬
#define IPC_SUB_USER_SCORE				3								//�û�����
#define IPC_SUB_GAME_START				4								//��Ϸ��ʼ
#define IPC_SUB_GAME_FINISH				5								//��Ϸ����
#define IPC_SUB_UPDATE_FACE				6								//����ͷ��
#define IPC_SUB_MEMBERORDER				7								//����ͷ��

#define IPC_SUB_USER_PWD				8								//�û���Ϣ


//�û�״̬
struct IPC_UserStatus
{
	DWORD								dwUserID;						//�û� I D
	WORD								wNetDelay;						//������ʱ
	BYTE								cbUserStatus;					//�û�״̬
};

//�û�����
struct IPC_UserScore
{
	LONG								lCharm;							//�û�����
	DWORD								dwUserID;						//�û� I D
	tagUserScore						UserScore;						//�û�����
};

//��Ա�ȼ�
struct IPC_MemberOrder
{
	BYTE								cbMember;					//��Ա�ȼ�
	DWORD								dwUserID;						//�û� I D
};

//�û�����
struct IPC_UpdateFace
{
	DWORD								dwCustomFace;					//�û� I D
};

//�û�״̬
struct IPC_UserPwd
{
	DWORD								dwUserID;						//�û� I D
	TCHAR								szPwd[PWD_LEN];					//����
};

//�û���չ��Ϣ
struct tagFrameUserInfoEx
{
	tagFrameUserInfoEx()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD	dwUserID;
	TCHAR	szAccount[NAME_LEN];
	TCHAR	szMD5Pwd[PWD_LEN];
};
//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_CONCTROL				4								//������Ϣ

#define IPC_SUB_START_FINISH			1								//�������
#define IPC_SUB_CLOSE_FRAME				2								//�رտ��
#define IPC_SUB_JOIN_IN_GAME			3								//������Ϸ
#define IPC_SUB_CLOSE_STATUSWND			4								//�ر�״̬��ʾ

#define IPC_SUB_SWITCH_TABLE			10								//����

//������Ϸ
struct IPC_JoinInGame
{
	WORD								wTableID;						//���Ӻ���
	WORD								wChairID;						//���Ӻ���
};

//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_BANK					5								//������Ϣ

#define IPC_SUB_BANK_RESULT		1								//�������

//�������
struct IPC_BankResult
{
	BYTE                                cbGameAction;                   //��Ϸ����
	LONG								lResultType;						//�������
	LONG								lErrorCode;						//�������
	TCHAR								szErrorDescribe[128];			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//����������

#define MDM_GF_GAME						100								//��Ϸ��Ϣ
#define MDM_GF_FRAME						101								//�����Ϣ
#define	MDM_GF_PRESENT					102								//������Ϣ
#define	MDM_GF_BANK						103								//������Ϣ
#define MDM_GF_MAIL						104								//�ʼ���Ϣ

#define SUB_GF_INFO								1								//��Ϸ��Ϣ
#define SUB_GF_USER_READY					2								//�û�ͬ��
#define SUB_GF_LOOKON_CONTROL		3								//�Թۿ���
#define SUB_GF_KICK_TABLE_USER			4								//�����û�

#define SUB_GF_OPTION							100								//��Ϸ����
#define SUB_GF_SCENE							101								//������Ϣ

#define SUB_GF_USER_CHAT					200								//�û�����
#define SUB_GF_BET_RECORD_TIME			201								//��ע��¼ʱ������

#define SUB_GF_MESSAGE						300								//ϵͳ��Ϣ

//#define SUB_GF_GIFT								400								//������Ϣ

#define SUB_GF_BANK_SAVE					450							//���д洢
#define SUB_GF_BANK_WITHDRAW			451							//������ȡ
#define SUB_GF_BANK_TRANSFER            452                          //����ת��
#define SUB_GF_BANK_SECURITY				453                          //���밲ȫ
#define SUB_GF_BANK_TRANSFERLOG         454                      //ת�ʼ�¼��ѯ
#define SUB_GF_TRANSFERLOG_RESULT		455						//ת�ʼ�¼���
#define SUB_GF_BANK_RESULT				456								//���н��
#define SUB_GF_BANK_INFO				457								//������Ϣ
#define SUB_GF_BANK_GETINFO			458								//����������Ϣ
#define SUB_GF_MAIL_RESULT				482								//�ʼ����


#define SUB_GF_EXCHANGE_CHARM				502								//�һ�����
#define SUB_GF_EXCHANGE_CHARM_OK			503								//�һ������ɹ�
#define SUB_GF_EXCHANGE_CHARM_ERROR	504								//�һ�����ʧ��

#define SUB_GF_BUYPROPS_REQ				510								//������� ����
#define SUB_GF_BUYPROPS_OK				511								//������� �ɹ�
#define SUB_GF_BUYPROPS_ERROR			512								//������� ʧ��

#define SUB_GF_PROPERTY						550								//������Ϣ
#define SUB_GF_PROPERTY_RESULT			551								//���߽��
#define SUB_GF_RESIDUAL_PROPERTY		552								//ʣ�����
#define SUB_GF_PROP_ATTRIBUTE			553								//��������
#define SUB_GF_PROP_BUGLE					554								//���ȵ���

//����,ֻ�ܼ��ں���
#define	SUB_GF_DAY_MATCH_WAIT_FOR_SIT	601					//�ȴ�������λ
#define	SUB_GF_DAY_MATCH_SIT_FINISH		603					//�������
#define	SUB_GF_DAY_MATCH_SET_START		607					//һ�ֿ�ʼ
#define	SUB_GF_DAY_MATCH_SET_END		608					//һ�ֽ���
#define	SUB_GF_DAY_MATCH_WILL_CLOSE		609					//ÿ����������

#define	SUB_GF_MATCH_CONTINUE			611					//����						
#define	SUB_GF_MATCH_EXIT				613					//�˳�						
#define	SUB_GF_MATCH_SWITCH_TABLE		615					//����						

#define		SUB_GF_LOADPROPS				620					// ���ص���		


//�汾��Ϣ
struct CMD_GF_Info
{
	BYTE								bAllowLookon;					//�Թ۱�־
};

//��Ϸ����
struct CMD_GF_Option
{
	BYTE								bGameStatus;					//��Ϸ״̬
	BYTE								bAllowLookon;					//�����Թ�
};

//�Թۿ���
struct CMD_GF_LookonControl
{
	DWORD								dwUserID;						//�û���ʶ
	BYTE								bAllowLookon;					//�����Թ�
};

//�����û�
struct CMD_GF_KickTableUser
{
	DWORD								dwUserID;						//�û� I D
};

//����ṹ
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//��Ϣ����
	COLORREF						crFontColor;					//��Ϣ��ɫ
	DWORD							dwSendUserID;					//�����û�
	DWORD							dwTargetUserID;					//Ŀ���û�
	TCHAR								szChatMessage[MAX_CHAT_LEN];	//������Ϣ
};

//��ע��¼ʱ��
struct CMD_GF_GetServerTimes
{
	LONGLONG						   lServerMilliseconds;				//���������ص���΢����
	TCHAR							   szKey[37];						//GUID key
	WORD							   wButtonFlag;						//��ť��־
};
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define SMT_INFO						0x0001							//��Ϣ��Ϣ
#define SMT_EJECT						0x0002							//������Ϣ
#define SMT_GLOBAL						0x0004							//ȫ����Ϣ
#define SMT_CLOSE_GAME					0x1000							//�ر���Ϸ

//��Ϣ���ݰ�
struct CMD_GF_Message
{
	WORD								wMessageType;					//��Ϣ����
	WORD								wMessageLength;					//��Ϣ����
	TCHAR								szContent[MAX_SYSMESSAGE_LEN];				//��Ϣ����
};

//�ȴ�����
struct CMD_GF_DayMatch_WaitForSit
{
	CMD_GF_DayMatch_WaitForSit()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wSecondCnt;			//����		
	BYTE	cbPlayerLocation;	//�������λ��
};

//�������
struct CMD_GF_DayMatch_SitFinish
{
	CMD_GF_DayMatch_SitFinish()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD dwStuff;
};

//ÿ����,ÿ�ֿ�ʼ
struct CMD_GF_DayMatch_SetStart
{
	CMD_GF_DayMatch_SetStart()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wKindID;
	WORD	wServerID;
	DWORD	dwMatchID;
};

//ÿ����,ÿ�ֽ���
struct CMD_GF_DayMatch_SetEnd
{
	CMD_GF_DayMatch_SetEnd()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wKindID;
	WORD	wServerID;
	DWORD	dwMatchID;
};

//�������ر�
struct CMD_GF_DayMatch_WillClose
{
	CMD_GF_DayMatch_WillClose()
	{
		memset(this,0,sizeof(*this));
	}
	WORD	wSecondCnt;			//����		
	BYTE	cbMatchEndTime;				
	BYTE	cbPlayerLocation;	//�������λ��
};



//////////////////////////////////////////////////////////////////////////

//�����ڴ涨��
struct tagShareMemory
{
	WORD								wDataSize;						//���ݴ�С
	HWND								hWndGameFrame;			//��ܾ��
	HWND								hWndGamePlaza;				//�㳡���
	HWND								hWndGameServer;			//������
};

//////////////////////////////////////////////////////////////////////////

//���ͳ���
#define	LOCATION_GAME_ROOM				1								//��Ϸ����
#define	LOCATION_PLAZA_ROOM				2								//��������
#define	LOCATION_SHOPPINGMALL			3								//�̳�
//////////////////////////////////////////////////////////////////////////

//���ͽṹ
struct CMD_GF_Gift
{
	BYTE								cbSendLocation;				//���ͳ���
	DWORD						dwSendGameID;				//������ID
	DWORD						dwRcvGameID;					//������ID
	WORD							wGiftID;								//����ID
	WORD							wFlowerCount;					//�ʻ���Ŀ
};

//	�������
struct CMD_GF_BuyProps
{
	BYTE										cbSendLocation;					//���ͳ���
	WORD									wPropsID;								//����ID
	DWORD								dwPachurseCount;				//������Ŀ
	DWORD								dwSourceGameID;				//�������
	DWORD								dwTargetGameID;					//����Ŀ��gameID���
	TCHAR									szPassword[PWD_LEN];		//��������
};

//���Ƚṹ
struct CMD_GF_BugleProperty
{
	BYTE								cbSendLocation;							//���ͳ���
	TCHAR							szUserName[32];							//����ʺ�
	COLORREF					crText;											//������ɫ
	TCHAR							szContext[BUGLE_MAX_CHAR];		//��������
};

//�һ��ṹ
struct CMD_GF_ExchangeCharm
{
	DWORD						dwUserID;							// USERID
	BYTE								cbSendLocation;				//���ͳ���
	LONGLONG					lCharm;								//������ֵ
	TCHAR							szPassword[PWD_LEN];		//��������
};

//�һ������ɹ�
struct CMD_GF_ExchangeCharmOK
{
	DWORD						dwUserID;							// USERID
	LONGLONG					lBankScore;						//���н��
	LONGLONG					lGameScore;						//��Ϸ���
	LONGLONG					lCharm;								//������ֵ

};

//�������͵���֪ͨ
struct CMD_GF_PropsNotify
{
	BYTE								cbSendLocation;						//���ͳ���
	BYTE								cbPropType;								// ��������  0:��� 1����Ա��
	DWORD						dwSrcGameID;							//������gameID
	TCHAR							szSrcNickname[NAME_LEN];		//�������ǳ�
	DWORD						dwRcvGameID;							//������gameID
	TCHAR							szRcvNickname[NAME_LEN];		//�����ǳ�
	WORD							wGiftID;										//����ID
	TCHAR							szPropsName[PROPS_NAME];	//��������
	WORD							wCount;									//������Ŀ
	LONGLONG					lBankScore;								// ���н��
	LONGLONG					lSrcCharm;								// ��������������
	LONGLONG					lRecCharm;								// ��������������
};



//�洢���
struct CMD_GF_BankStorage
{
	LONGLONG 							lStorageValue;					//�洢���
	BYTE										cbGameAction;					//��Ϸ����
	TCHAR									szPassword[PWD_LEN];		//�û�����
};

//��ȡ���
struct CMD_GF_BankGet
{
	LONGLONG						lGetValue;							//��ȡ���
	BYTE									cbGameAction;					//��Ϸ����
	TCHAR								szPassword[PWD_LEN];		//�û�����
};

//ת�����
struct CMD_GF_BankTransfer
{
	LONGLONG                      lTransferValue;                 //ת�����
	BYTE								    cbGameAction;                   //��Ϸ����
	DWORD                           dwTagGameID;                   //Ŀ��USER I D
	TCHAR                             szTagAccounts[NAME_LEN];        //Ŀ���ʺ�
	TCHAR                             szPassword[PWD_LEN];
};

//���밲ȫ
struct CMD_GF_BankSecurity
{
	TCHAR								szAccounts[NAME_LEN];			//�ʺ�
	TCHAR								szPassword[PWD_LEN];           //�ɵ�����
	TCHAR								szNewPassword[PWD_LEN];    //�µ�����
};

//���д�����
struct CMD_GF_BankError
{
	BYTE                                cbGameAction;              //��Ϸ����
	LONG								lResultType;					//�������
	LONG								lErrorCode;					//�������
	TCHAR								szErrorDescribe[128];		//������Ϣ
};

//������Ϣ
struct CMD_GF_BankInfo
{
	BYTE                                cbGameAction;					    //��Ϸ����  1->�棻 2->ȡ��3->ת��;  0->��½
	TCHAR                             szAccounts[NAME_LEN];		  // �ʺ�
	LONGLONG						lBankScore;							 //  ��ǰ�������н��
	LONGLONG						lGameScore;							 //  ��ǰ������Ϸ���
	LONGLONG						lCharm;									//   ��ǰ����
};

// ���в���
struct CMD_GF_BankAction
{
	BYTE									btActionID;								// ����ID			1->�棻 2->ȡ;  3->ת��
	DWORD                           dwTagGameID;						   //Ŀ��USER I D
	LONGLONG						lScore;									  // ��� 
	TCHAR                             szAccounts[NAME_LEN];		  //�ʺ�
	TCHAR                             szTagAccounts[NAME_LEN];   //Ŀ���ʺ�
	TCHAR                             szPassword[PWD_LEN];		     // ��������	
};

//�ʼ����
struct CMD_GF_UserMailResult
{
	DWORD								dwUserID;
	LONG								lUnreadMailCnt;					//δ���ʼ�����
};

//ʣ�����
struct CMD_GF_ResidualProperty
{
	DWORD								dwResidualTime[PROPERTY_COUNT];	//ʣ��ʱ��
};

//  ����
struct tagError
{
	tagError()
	{
		memset(this, 0, sizeof(*this));
	}
	LONG								lErrorCode;						//�������
	TCHAR								szErrorDescribe[128];			//������Ϣ
};
//////////////////////////////////////////////////////////////////////////
#endif