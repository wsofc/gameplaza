/*

REQ -- request ����
ANW -- answer  Ӧ��

*/

#pragma once


//�㳡�汾
#define VER_PLAZA_LOW					2								//�㳡�汾
#define VER_PLAZA_HIGH					16								//�㳡�汾
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)


//-----------------------------------------------
//��¼�����ʶ

#define ERC_GP_LOGON_SUCCESS			0								//��½�ɹ�
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//�ʺŲ�����
#define ERC_GP_LONG_NULLITY				2								//��ֹ��¼
#define ERC_GP_PASSWORD_ERCOR			3								//�������


//-----------------------------------------------
//��¼������

#define MAIN_LS_LOGON					0x2000								//�㳡��¼

#define SUB_LS_LOGON_ACCOUNT			0x2001							//�ʺŵ�¼
#define SUB_LS_LOGON_GAME_ID				0x2002						//I D ��¼
#define SUB_LS_REGISTER_ACCOUNT			0x2003						//ע���ʺ�

#define SUB_LS_LOGON_SUCCESS			0x2005							//��½�ɹ�
#define SUB_LS_LOGON_ERROR				0x2006							//��½ʧ��
#define SUB_LS_LOGON_FINISH				0x2007							//��½���

#define SUB_LS_REGISTER_SUCCESS			0x2008							//ע��ɹ�
#define SUB_LS_REGISTER_ERROR			0x2009							//ע��ʧ��

#define SUB_C2LS_CHECKINGSMS						0x2101								// �˲������֤
#define SUB_C2LS_GETSMS									0x2102								// ��ȡ������֤
#define SUB_LS2C_NEEDSMSCODE						0x2103								// ��Ҫ�˲������֤
#define SUB_LS2C_SENDSMSSUCCESS					0x2104								// �ѷ���sms�ɹ�
#define SUB_LS2C_SMS_ERROR							0x2105								// ����ʧ��

#define SUB_LS2C_PASS										0x2106								// �ѷ��Ͷ�����֤��

#define SUB_C2LS_GETPASSWORD						0x2110								//  �һ�����
#define SUB_LS2C_GETPASSWORD_SSUCCESS		0x2111								//  �һ�����ɹ�
#define SUB_LS2C_GETPASSWORD_ERROR			0x2112								//  �һ�����ʧ��

#define SUB_C2LS_CHECKACCOUNT						0x2115								//  ����˲��˺��Ƿ����
#define SUB_C2LS_CHECKACCOUNT_SSUCCESS		0x2116								//  ����˲��˺��Ƿ���óɹ�
#define SUB_C2LS_CHECKACCOUNT_ERROR			0x2117								//  ����˲��˺��Ƿ����ʧ��

#define SUB_C2LS_GETSERVERINFO_REQ				0x2118								//  ������·�������Ϣ

#define SUB_C2LS_GETUSERINFO_REQ					0x2119								//  �����ȡ�����Ϣ
#define SUB_LS2C_GETUSERINFO							0x2120								//  �����Ϣ����
#define SUB_LS2C_GETUSERINFO_ERROR				0x2121								//  �����Ϣ����ʧ��

#define SUB_C2LS_GETUSERGOLD_REQ					0x3000								//  �����ȡ��ҽ����Ϣ
#define SUB_LS2C_GETUSERGOLD							0x3001								//  ��ҽ����Ϣ����
#define SUB_LS2C_GETUSERGOLD_ERROR				0x3002								//  �����Ϣ�������ʧ��

// 
struct tag_C_GetSMS
{
	tag_C_GetSMS()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE					btType;										// ��������	1ע�᣻2�޸����룻3�޸��ֻ���4��½��5ȡ�6ת��; 7���;8��½
	TCHAR				szPhone[PHONE_LEN];				// �ֻ���
	TCHAR				szAccount[NAME_LEN];				//��¼�ʺ�
};

// ˢ�� ��ҽ����Ϣ
struct tag_S_UserGold
{
	tag_S_UserGold()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD						dwUserID;					// ���USERID
	LONGLONG					lGameGold;				// ��Ϸ���
	LONGLONG					lBankGold;				// ���н��
	LONGLONG					lCharm;						// ����
};

// ������ҽ����Ϣ
struct tag_C_GetUserGold_REQ
{
	tag_C_GetUserGold_REQ()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD						dwUserID;					// ���USERID
};

struct tag_S_ErrorMsg
{
	tag_S_ErrorMsg()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE					btErrorCode;											// ������
	TCHAR				szMsg[RESULT_CARD_TYPE];				//����
};

//�ʺŵ�¼
struct tagLS_LogonByAccount
{
	tagLS_LogonByAccount()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD			dwPlazaVersion;					//�㳡�汾
	TCHAR				szSMS[SMS_LEN];					// ������֤��
	TCHAR				szAccount[NAME_LEN];			//��¼�ʺ�
	TCHAR				szPwd[PWD_LEN];					//��¼����
};

//  �һ�����
struct tagLS_GetPassword
{
	tagLS_GetPassword()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR				szSMS[SMS_LEN];					// ������֤��
	TCHAR				szAccount[NAME_LEN];			//��¼�ʺ�
	TCHAR				szNewPwd[PWD_LEN];			//�µ�¼����
};

// ��ȡ�������б�
struct tagLS_GetServerInfo
{
	tagLS_GetServerInfo()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE				btPlatform;					// �����豸
};

//I D ��¼
struct tagLS_LogonByGameID
{
	tagLS_LogonByGameID()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwPlazaVersion;					//�㳡�汾
	DWORD				dwGameID;							//��Ϸ I D
	TCHAR					szPwd[PWD_LEN];					//��¼����
};

struct tagLS_CheckAccount
{
	tagLS_CheckAccount()
	{
		memset(this, 0,sizeof(*this));
	}
	BYTE					btType;		// 1:�˺ţ� 2���ǳ�
	TCHAR				szAccount[NAME_LEN];					//�ʺ�
};


struct tagLS_GetUserInfoByGameID
{
	tagLS_GetUserInfoByGameID()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD			dwGameID;
	DWORD			dwUserID;
	TCHAR				szAccount[NAME_LEN];
	TCHAR				szNickname[NAME_LEN];
};

//ע���ʺ�
struct tagLS_RegisterAccount
{
	tagLS_RegisterAccount()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wFaceID;										//ͷ���ʶ
	BYTE					cbGender;										//�û��Ա�
	DWORD			dwPlazaVersion;							//�㳡�汾
	TCHAR				szSpreader[NAME_LEN];				//�ƹ�����
	TCHAR				szAccount[NAME_LEN];					//�ʺ�
	TCHAR				szNickname[NAME_LEN];				//�ǳ�
	TCHAR				szPwd[PWD_LEN];							//��¼����
	TCHAR				szPhoneNumber[PHONE_LEN];		//�ֻ���

	TCHAR				szQQNumber[15];						//QQ����
	TCHAR				szRealName[NAME_LEN];			//��ʵ����
	TCHAR				szIDCard[IDCARD_LEN];			// ���֤����
	TCHAR				szEmail[22];								//����
	TCHAR				szPwdProtectQuestion[32];		//����
	TCHAR				szPwdProtectAnswer[32];			//��
	TCHAR				szSMS[SMS_LEN];					//������֤��
};


//��½�ɹ�
struct tagLS_LogonSuccess
{
	tagLS_LogonSuccess()
	{
		memset(this,0,sizeof(*this));
	}
	WORD					wFaceID;							//ͷ������
	BYTE						cbGender;							//�û��Ա�
	BYTE						cbMember;						//��Ա�ȼ�
	DWORD				dwUserID;							//�û� I D
	DWORD				dwGameID;						//��Ϸ I D
	DWORD				dwExperience;					//�û�����
	LONGLONG			dwCharm;							 //����

	LONGLONG            lGold;								//���
	LONGLONG            lBankGold;						//���н��

	//��չ��Ϣ
	DWORD				dwCustomFaceVer;				//ͷ��汾
	BYTE						cbMoorMachine;					//��������
	BYTE						cbMasterOrder;						//����ȼ�
};

//��½ʧ��
struct tagLS_LogonError
{
	tagLS_LogonError()
	{
		memset(this,0,sizeof(*this));
	}
	LONG				lErrorCode;						//�������
	TCHAR				szErrorDescribe[128];			//������Ϣ
};



//-----------------------------------------------
//��Ϸ�б�������

#define MAIN_LS_SERVER_LIST				0x2100							//�б���Ϣ

#define SUB_LS_LIST_TYPE				0x2101							//�����б�
#define SUB_LS_LIST_KIND				0x2102							//�����б�
#define SUB_LS_LIST_STATION				0x2103							//վ���б�
#define SUB_LS_LIST_GAME_SERVER			0x2104							//�����б�
#define	SUB_LS_LIST_HALL_SERVER			0x2105				
#define SUB_LS_LIST_FINISH				0x2106							//�������
#define SUB_LS_LIST_CONFIG				0x2107							//�б�����

//�б�����
struct tagLS_ListConfig
{
	tagLS_ListConfig()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE				bShowOnlineCount;				//��ʾ����
};


//-----------------------------------------------
//ϵͳ������

#define MAIN_LS_SYSTEM					0x2200								//ϵͳ����

#define SUB_LS_VERSION						0x2201								//�汾֪ͨ
#define SUB_LS_MAINTENANCE			0x2202								//ϵͳά��
#define SUB_LS_VERSION_REQ				0x2203								// �汾�������

//�汾֪ͨ
struct tagLS_Version
{
	tagLS_Version()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE					bNewVersion;					//���°汾
	BYTE					bAllowConnect;					//��������
	WORD				wServerLow;
	WORD				wServerHigh;

};
//�汾�������
struct tagLS_VersionReq
{
	tagLS_VersionReq()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD			dwPlazaVersion;					//�㳡�汾
};


//ϵͳά��
struct tagLS_Maintenance
{
	tagLS_Maintenance()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wHintMessageLen;
	TCHAR				szHintMessage[256];
};


//-----------------------------------------------
//
#define MAIN_LS_USER						0x2300					// �û���Ϣ

#define SUB_LS_USER_UPLOAD_FACE				0x2301					// �ϴ�ͷ��
#define SUB_LS_USER_DOWNLOAD_FACE			0x2302					// ����ͷ��
#define SUB_LS_UPLOAD_FACE_RESULT			0x2303					// �ϴ����
#define SUB_LS_DELETE_FACE_RESULT			0x2304					// ɾ�����
#define SUB_LS_CUSTOM_FACE_DELETE			0x2305					// ɾ��ͷ��

#define SUB_LS_USERINFO_GET					0x2310					// ��ȡ������Ϣ
#define SUB_LS_USERINFO_GET_OK				0x2311					// ��ȡ������Ϣ�ɹ�
#define SUB_LS_USERINFO_GET_ERROR			0x2312					// ��ȡ������Ϣʧ��

#define SUB_LS_USERINFO_UPDATE				0x2320					// �޸ĸ�����Ϣ
#define SUB_LS_USERINFO_UPDATE_OK			0x2321					// �޸ĸ�����Ϣ�ɹ�
#define SUB_LS_USERINFO_UPDATE_ERROR		0x2322					// �޸ĸ�����Ϣʧ��

#define SUB_LS_USERINFO_MODTEL				0x2330					// �޸��ֻ���
#define SUB_LS_USERINFO_MODTEL_OK			0x2331					// �޸��ֻ��ųɹ�
#define SUB_LS_USERINFO_MODTEL_ERROR		0x2332					// �޸��ֻ���ʧ��

#define SUB_LS_USERINFO_MODPW				0x2340					// �޸�����
#define SUB_LS_USERINFO_MODPW_OK			0x2341					// �޸�����ɹ�
#define SUB_LS_USERINFO_MODPW_ERROR			0x2342					// �޸�����ʧ��

#define SUB_LS_USER_LOCK_MACHINE			0x2350					// ��������
#define SUB_LS_USER_LOCK_MACHINE_OK			0x2351					// ���������ɹ�
#define SUB_LS_USER_LOCK_MACHINE_ERROR		0x2352					// ��������ʧ��

#define SUB_LS_USER_UNLOCK_MACHINE			0x2360					// ��������
#define SUB_LS_USER_UNLOCK_MACHINE_OK		0x2361					// ���������ɹ�
#define SUB_LS_USER_UNLOCK_MACHINE_ERROR	0x2362					// ��������ʧ��

#define MAIN_LS_SMS							0x3000					// ����


//�޸������Ϣ
struct tagLS_UpdateUserInfo
{
	tagLS_UpdateUserInfo()
	{
		memset(this, 0, sizeof(*this));
	}
	WORD				wFaceID;										// ͷ���ʶ
	BYTE					cbGender;										// �û��Ա�
	TCHAR				szAccount[NAME_LEN];					// �ʺ�
	TCHAR				szPwd[PWD_LEN];							// ��¼����

	TCHAR				szNickname[NAME_LEN];				// �ǳ�
	TCHAR				szRealname[NAME_LEN];				// ��ʵ����
	TCHAR				szEmail[EMAIL_LEN];						// ����
	TCHAR				szTelphone[PHONE_LEN];				// �绰����
	TCHAR				szQQNumber[PHONE_LEN];			// QQ����
	TCHAR				szIDCard[IDCARD_LEN];				// ���֤
	TCHAR				szUnderWrite[UNDER_WRITE_LEN];			// ����ǩ��
};

//�޸��ֻ���
struct tagLS_ModTelphone
{
	tagLS_ModTelphone()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR				szAccount[NAME_LEN];			//��¼�ʺ�
	TCHAR				szPwd[PWD_LEN];					//��¼����
	TCHAR				szSMS[SMS_LEN];					//������֤��
	TCHAR				szOldTel[PHONE_LEN];			//���ֻ���
	TCHAR				szNewTel[PHONE_LEN];			//���ֻ���
};

//�޸�����
struct tagLS_ModPassword
{
	tagLS_ModPassword()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR				szAccount[NAME_LEN];			//��¼�ʺ�
	TCHAR				szOldPwd[PWD_LEN];				//�ɵ�¼����
	TCHAR				szNewPwd[PWD_LEN];				//�µ�¼����
};

//��������
struct tagLS_LockMachine
{
	tagLS_LockMachine()
	{
		memset(this, 0, sizeof(*this));
	}

	bool					bLock;														// �Ƿ�����(true:������false:����)
	TCHAR				szSMS[SMS_LEN];										//������֤��
	TCHAR				szAccount[NAME_LEN];								// �ʺ�
	TCHAR				szBankPwd[PWD_LEN];								// ��������
	TCHAR				szComputerID[COMPUTER_ID_LEN];			// ������
};

//����ͷ��
struct tagLS_UploadCustomFace
{
	tagLS_UploadCustomFace()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;						//��� ID
	WORD				wCurrentSize;					//��ǰ��С
	bool				bLastPacket;					//����ʶ
	bool				bFirstPacket;					//��һ����ʶ
	BYTE				bFaceData[2048];				//ͷ������
};

//����ͷ��
struct tagLS_DownloadFace
{
	tagLS_DownloadFace()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;							//��� ID
};

//�ϴ����
struct tagLS_UploadFaceResult
{
	tagLS_UploadFaceResult()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR				szDescribeMsg[128];					//������Ϣ
	DWORD				dwFaceVer;							//;
	bool				bOperateSuccess;					//�ɹ���ʶ
};

//���سɹ�
struct tagLS_DownloadFaceSuccess
{
	tagLS_DownloadFaceSuccess()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwToltalSize;						//�ܹ���С
	DWORD				dwCurrentSize;						//��ǰ��С
	DWORD				dwUserID;							//��� ID
	BYTE				bFaceData[2048];					//ͷ������
};

//ɾ�����
struct tagLS_DeleteFaceResult
{
	tagLS_DeleteFaceResult()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR				szDescribeMsg[128];					//������Ϣ
	DWORD				dwFaceVer;							//ͷ��汾
	bool				bOperateSuccess;					//�ɹ���ʶ
};

//ɾ����Ϣ
struct tagLS_CustomFaceDelete
{
	tagLS_CustomFaceDelete()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;							//��� ID
	DWORD				dwFaceVer;							//ͷ��汾
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
