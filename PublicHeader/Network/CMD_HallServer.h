/*

REQ -- request ����
ANW -- answer  Ӧ��

*/

#pragma once


//-----------------------------------------------
//��¼������

#define MAIN_HS_LOGON					0x4000							//�㳡��¼

#define SUB_HS_LOGON_USER_ID			0x4002							//I D ��¼

#define SUB_HS_LOGON_SUCCESS			0x4005							//��½�ɹ�
#define SUB_HS_LOGON_ERROR				0x4006							//��½ʧ��
#define SUB_HS_LOGON_FINISH				0x4007							//��½���


//I D ��¼
struct tagHS_LogonByUserID
{
	tagHS_LogonByUserID()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;						//�û�I D
	TCHAR		szPwd[PWD_LEN];					//��¼����
};

//��½�ɹ�
struct tagHS_LogonSuccess
{
	tagHS_LogonSuccess()
	{
		memset(this,0,sizeof(*this));
	}
};

//��½ʧ��
struct tagHS_LogonError
{
	tagHS_LogonError()
	{
		memset(this,0,sizeof(*this));
	}
	LONG		lErrorCode;						//�������
	TCHAR		szErrorDescribe[128];			//������Ϣ
};


//-----------------------------------------------
//
#define MAIN_HS_USER					0x4100			//�û���Ϣ

#define SUB_HS_MODIFY_BASE_INFO			0x4103			//�޸ĸ�����Ϣ
#define SUB_HS_MODIFY_PWD				0x4106			//�޸�����
#define SUB_HS_MODIFY_USER_INFO_RESULT	0x4107			//�޸Ľ��

#define SUB_HS_SAFE_BIND				0x4110			//��Ұ�
#define SUB_HS_SAFE_UNBIND				0x4111			//�����

#define SUB_HS_GET_MAIL_COUNT			0x4120			//�û�δ���ʼ�����
#define SUB_HS_GET_SERVER_TIMES			0x4121			//�û����������ʱ��

#define	SUB_HS_GET_GAME_SCORE_COUNT			0x4122			//�û�����ˢ�½������
//��������
struct tagHS_ModifyBaseInfo
{
	tagHS_ModifyBaseInfo()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;				//��� ID

	TCHAR		szNickname[NAME_LEN];	//����ǳ�
	BYTE		cbGender;				//����Ա�
	WORD		wFaceID;				
	TCHAR		szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��
};

//�䳤���ݰ�
//���ֻ�޸�һ������,��ôֻ������Ҫ��
//����޸���������,��ô�������е�
struct tagHS_ModifyPwd
{
	tagHS_ModifyPwd()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;		//��� ID
	BYTE		cbPwdType;

	TCHAR		szOldPwd0[PWD_LEN];	
	TCHAR		szNewPwd0[PWD_LEN];

	TCHAR		szOldPwd1[PWD_LEN];	
	TCHAR		szNewPwd1[PWD_LEN];	
};

////��������
//enum emPwdType
//{
//	PWD_TYPE_ERROR	,
//	PWD_TYPE_LOGON	= 0x01,	//��¼����
//	PWD_TYPE_BANK	= 0x02,	//��������
//};
#define	MODIFY_TYPE_BASE_INFO	0x04

//�޸Ľ��
struct tagHS_ModifyUserInfoResult
{
	tagHS_ModifyUserInfoResult()
	{
		memset(this,0,sizeof(*this));
	}
	bool		bSuccess;				//�ɹ���ʶ
	BYTE		cbModifyType;			//�޸�����,PWD_TYPE_LOGON,PWD_TYPE_LOGON,MODIFY_TYPE_BASE_INFO
	TCHAR		szErrorDescribe[128];	//������Ϣ
};

//
//

//���ʺ�
struct tagHS_SafeBind
{
	tagHS_SafeBind()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;						//���ID
	tagClientSerial		ClientSerial;					//��������
	TCHAR				szPwd[PWD_LEN];					//��¼����
};

//�����
struct tagHS_SafeUnbind
{
	tagHS_SafeUnbind()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD	dwUserID;						//���ID
	TCHAR	szPwd[PWD_LEN];					//��¼����
};

//�����
struct tagHS_BindResult
{
	tagHS_BindResult()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE	cbIsBind;
	BYTE	cbSuccess;
	TCHAR	szErrorDescribe[128];			//������Ϣ
};

//�õ��ʼ�����
struct tagHS_GetMailCount
{
	tagHS_GetMailCount()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;						//�û�I D
	UINT		nMailCount;						//�û�δ���ʼ�����
	bool		bSuccess;							//�ɹ���ʶ
};
//�õ��������
struct tagHS_GetGameScore
{
	tagHS_GetGameScore()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;						//�û�I D
	LONGLONG	lGameScore;						//�û��������
};

//�õ�������ʱ��
struct tagHS_GetServerTimes
{
	tagHS_GetServerTimes()
	{
		memset(this,0,sizeof(*this));
	}
	LONGLONG	lServerMilliseconds;				//���������ص���΢����
	TCHAR		szKey[37];							//GUID key
	WORD		wButtonFlag;						//��ť��־
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
