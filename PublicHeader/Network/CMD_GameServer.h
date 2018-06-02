
#pragma once

//-----------------------------------------------
//��¼���ݰ�����
#define MAIN_GS_LOGON				0x3000								//�����¼

#define SUB_GS_LOGON_BY_USER_ID		0x3002								//I D ��¼

#define SUB_GS_LOGON_SUCCESS		0x3003								//��¼�ɹ�
#define SUB_GS_LOGON_ERROR			0x3004								//��¼ʧ��
#define SUB_GS_LOGON_FINISH			0x3005								//��¼���

//���� ID ��¼
struct tagGS_LogonByUserID
{
	tagGS_LogonByUserID()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwProcessVersion;					//���̰汾
	DWORD							dwUserID;									//�û� I D
	TCHAR								szPwd[PWD_LEN];						//��¼����
};

//��¼�ɹ���Ϣ
struct tagGS_RT_LogonSuccess
{
	tagGS_RT_LogonSuccess()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD							dwUserID;							//�û� I D
};

//��¼ʧ��
struct tagGS_RT_LogonError
{
	tagGS_RT_LogonError()
	{
		memset(this,0,sizeof(*this));
	}
	LONG							lErrorCode;							//�������
	TCHAR							szErrorDescribe[128];				//������Ϣ
};



//-----------------------------------------------
//�û����ݰ�����

#define MDM_GR_USER					0x3100									//�û���Ϣ

#define SUB_GR_USER_SIT_REQ			1									//��������
#define SUB_GR_USER_LOOKON_REQ		2									//�Թ�����
#define SUB_GR_USER_STANDUP_REQ		3									//��������
#define SUB_GR_USER_LEFT_GAME_REQ	4									//�뿪��Ϸ

#define SUB_GR_USER_COME			100									//�û�����
#define SUB_GR_USER_STATUS			101									//�û�״̬
#define SUB_GR_USER_SCORE			102									//�û�����
#define SUB_GR_USER_SIT_FAILED		103									//����ʧ��
#define SUB_GR_USER_RIGHT			104									//�û�Ȩ��
#define SUB_GR_USER_MEMBER_ORDER	105									//��Ա�ȼ�

#define SUB_GR_USER_CHAT			200									//������Ϣ
#define SUB_GR_USER_WISPER			201									//˽����Ϣ
#define SUB_GR_USER_RULE			202									//�û�����

#define SUB_GR_USER_INVITE			300									//������Ϣ
#define SUB_GR_USER_INVITE_REQ		301									//��������

#define SUB_GR_USER_UPDATE_GOLD		401									//����������Ϣ

//��Ա�ȼ�
struct CMD_GR_MemberOrder
{
	DWORD							dwUserID;							//���ݿ� ID
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//�û�Ȩ��
struct CMD_GR_UserRight
{
	DWORD							dwUserID;							//���ݿ� ID
	DWORD							dwUserRight;						//�û�Ȩ��
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//���ݿ� ID
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbUserStatus;						//�û�״̬
};

//�û�����
struct CMD_GR_UserScore
{
	LONGLONG							lCharm;								//�û�����
	//LONG							lBankScore;						//���ѽ��
	//LONG							lGameGold;							//��Ϸ���
	DWORD							dwUserID;							//�û� I D
	tagUserScore					UserScore;							//������Ϣ
};

//��������
struct CMD_GR_UserSitReq
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbPassLen;							//���볤��
	TCHAR							szTablePass[PWD_LEN];				//��������
};

//�����û� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//����ʧ��
struct CMD_GR_SitFailed
{
	CMD_GR_SitFailed()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR			szFailedDescribe[256];				//��������
};

//����ṹ 
struct CMD_GR_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	COLORREF					crFontColor;						//��Ϣ��ɫ
	DWORD						dwSendUserID;						//�����û�
	DWORD						dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//������Ϣ
};

//˽��ṹ 
struct CMD_GR_Wisper
{
	WORD							wChatLength;						//��Ϣ����
	COLORREF						crFontColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//������Ϣ
};

//�û�����
struct CMD_GR_UserRule
{
	bool							bPassword;							//��������
	bool							bLimitWin;							//����ʤ��
	bool							bLimitFlee;							//��������
	bool							bLimitScore;							//���Ʒ���
	bool							bCheckSameIP;						//Ч���ַ
	WORD						wWinRate;							//����ʤ��
	WORD						wFleeRate;							//��������
	LONGLONG 				lMaxScore;							//��߷��� 
	LONGLONG 				lLessScore;							//��ͷ���
	TCHAR						szPassword[PWD_LEN];			//��������
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};


//-----------------------------------------------
//������Ϣ���ݰ�

#define MDM_GR_INFO					0x3200								//������Ϣ

#define SUB_GR_SERVER_INFO			100									//��������
#define SUB_GR_ORDER_INFO			101									//�ȼ�����
#define SUB_GR_MEMBER_INFO			102									//��Ա����
#define SUB_GR_COLUMN_INFO			103									//�б�����
#define SUB_GR_CONFIG_FINISH		104									//�������

//��Ϸ������Ϣ
struct CMD_GR_ServerInfo
{
	//��������
	WORD							wKindID;							//���� I D
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ
	DWORD							dwVideoAddr;						//��Ƶ��ַ

	//��չ����
	WORD							wRoomServiceType;					//��������
	BYTE							cbHideUserInfo;						//������Ϣ
};

//����������Ϣ
struct CMD_GR_ScoreInfo
{
	WORD							wDescribeCount;						//������Ŀ
	WORD							wDataDescribe[16];					//���ݱ�־
};

//�ȼ������ṹ
struct tagOrderItem
{
	long 							lScore;								//�ȼ�����
	WORD							wOrderDescribe[16];					//�ȼ�����
};

//�ȼ�������Ϣ
struct CMD_GR_OrderInfo
{
	WORD							wOrderCount;						//�ȼ���Ŀ
	tagOrderItem					OrderItem[128];						//�ȼ�����
};

//�б��������ṹ
struct tagColumnItem
{
	WORD							wColumnWidth;						//�б���
	WORD							wDataDescribe;						//�ֶ�����
	TCHAR							szColumnName[16];					//�б�����
};

//�б�������Ϣ
struct CMD_GR_ColumnInfo
{
	WORD							wColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[32];						//�б�����
};



//-----------------------------------------------
//����״̬���ݰ�

#define MAIN_GS_STATUS				0x3300								//״̬��Ϣ

#define SUB_GS_TABLE_INFO			0x3301								//������Ϣ
#define SUB_GS_TABLE_STATUS			0x3302								//����״̬

//����״̬�ṹ
struct tagTableStatus
{
	tagTableStatus()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE							bTableLock;							//����״̬
	BYTE							bPlayStatus;						//��Ϸ״̬
};

//����״̬����
struct tagGS_TableInfo
{
	tagGS_TableInfo()
	{
		memset(this,0,sizeof(*this));
	}
	WORD							wTableCount;						//������Ŀ
	tagTableStatus				TableStatus[512];					//״̬����
};

//����״̬��Ϣ
struct tagGS_TableStatus
{
	tagGS_TableStatus()
	{
		memset(this,0,sizeof(*this));
	}
	WORD						wTableID;							//���Ӻ���
	BYTE							bTableLock;							//����״̬
	BYTE							bPlayStatus;						//��Ϸ״̬
};



//-----------------------------------------------
//�������ݰ�

#define MDM_GR_MANAGER				0x3400								//��������

#define SUB_GR_SEND_WARNING		1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER					4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//���ý����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������
#define SUB_GR_OPTION_SERVER		7									//���÷�������
#define SUB_GR_GMCONTROL				8									//GM����
#define SUB_GR_GMGETINFO				9									//GM��ȡ��Ϣ 

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;											//��Ϣ����
	DWORD						dwTargetUserID;										//Ŀ���û�
	TCHAR							szWarningMessage[MAX_CHAT_LEN];		//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[MAX_CHAT_LEN];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccount
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;				//������Ϣ
	BYTE							cbLimitLookonGame;			//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionServer
{
	bool					bRoomChat;				//��������
	bool					bGameChat;				//��Ϸ����
	bool					bRoomWisper;			//����˽��
	bool					bEnterRoom;				//���뷿��
	bool					bEnterGame;				//������Ϸ����
	bool					bShallClose;				//�����ر�
};

#define GM_OPERATION_WINORLOST				1					// ���������Ӯ
#define GM_OPERATION_WININNINGS			2					// Ӯ����
#define GM_OPERATION_LOSTINNINGS			3					// �伸��
#define GM_OPERATION_STORESCORE			4					// ���ÿ��
#define GM_OPERATION_ROBOTNUMS			5					// ���û���������
#define GM_OPERATION_ROBOTSCORE			6					// ���û����˷�����Χ
#define GM_OPERATION_ADDBLACKUSER		7					// ���������
#define GM_OPERATION_DELBLACKUSER		8					// ɾ��������
#define GM_OPERATION_GETROOMINFO		9					// ��ȡ��������

//GM���ã����պͷ��Ͷ��������
struct CMD_GM_Option
{
	CMD_GM_Option()
	{
		memset(this, 0, sizeof(*this));
	}
	BYTE								btOperationID;		// GM����ID
	DWORD						dwUserID;				//	�û�ID
	LONGLONG					lValue;			// ����һ
	LONGLONG					lValue2;			// ������
	LONGLONG					lValue3;			// ������
};

//-----------------------------------------------
//ϵͳ���ݰ�

#define MAIN_GS_SYSTEM				0x3500								//ϵͳ��Ϣ
#define SUB_GS_MESSAGE				0x3501								//ϵͳ��Ϣ

//��Ϣ����
#define SMT_INFO					0x0001								//��Ϣ��Ϣ
#define SMT_EJECT					0x0002								//������Ϣ
#define SMT_GLOBAL					0x0004							//ȫ����Ϣ
#define SMT_CLOSE_ROOM				0x1000						//�رշ���
#define SMT_INTERMIT_LINE			0x4000						//�ж�����

//��Ϣ���ݰ�
struct tagGS_Message
{
	tagGS_Message()
	{
		memset(this,0,sizeof(*this));
	}

	WORD							wMessageType;						//��Ϣ����
	WORD							wMessageLength;						//��Ϣ����
	char								szContent[MAX_SYSMESSAGE_LEN];						//��Ϣ����
};



//-----------------------------------------------
//�������ݰ�

#define MAIN_GS_SERVER_INFO			0x3600								//������Ϣ

#define SUB_GS_ONLINE_COUNT_INFO	0x3601								//������Ϣ

//������Ϣ
struct tagOnlineCountInfo
{
	tagOnlineCountInfo()
	{
		memset(this,0,sizeof(*this));
	}

	WORD							wKindID;							//���ͱ�ʶ
	DWORD							dwOnlineCount;						//��������
};


/*-----------*/
struct tagRoomRobotInfo
{
	tagRoomRobotInfo()
	{
		memset(this, 0, sizeof(*this));
	}

	int								iRobotCounts;					// ���������� 
	LONGLONG				lRobotMinScore;				// ��������С��ֵ
	LONGLONG				lRobotMaxScore;				// ����������ֵ
	int								iArrayCounts;					// �����˷���ϸ������
	int								iArrayRate[10];					// �����˷���ϸ�� �ٷֱ�
	LONGLONG				lArrayScore[10];					// �����˷���ϸ�� ���� 
};
//-----------------------------------------------
//					the end
//-----------------------------------------------
