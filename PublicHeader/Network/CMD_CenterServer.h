/*

REQ -- request ����
ANW -- answer  Ӧ��

*/
#pragma once


//-----------------------------------------------
//��Ϸ�б�������
//

#define MAIN_CS_SERVER_LIST				0x1000							//�б���Ϣ

#define SUB_CS_GET_SERVER_LIST		0x1001							//�����б�

#define SUB_CS_LIST_BEGIN					0x1002							//�б���Ϣ
#define SUB_CS_LIST_TYPE					0x1003							//�����б�
#define SUB_CS_LIST_KIND					0x1004							//�����б�
#define SUB_CS_LIST_STATION				0x1005							//վ���б�
#define SUB_CS_LIST_GAME_SERVER	0x1006							//����������б�
#define SUB_CS_LIST_HALL_SERVER		0x1007							//�����������б�
#define SUB_CS_LIST_FINISH					0x1010							//�б����

//�б�ʼ
struct tagCS_ListBegin
{
	tagCS_ListBegin()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwTypeCount;					//������Ŀ
	DWORD				dwKindCount;					//������Ŀ
	DWORD				dwGameServerCount;				//������Ŀ
	DWORD				dwHallServerCount;				//������������Ŀ
	DWORD				dwStationCount;					//վ����Ŀ
};



//-----------------------------------------------
//�������
//

#define MAIN_CS_SERVER_MANAGER					0x1100				//�������

#define SUB_CS_LS_REGISTER								0x1101				//ע���½������
#define SUB_CS_LS_UNREGISTER							0x1102				//ע��

#define SUB_CS_HS_REGISTER								0x1110				//ע�����������
#define SUB_CS_HS_UNREGISTER							0x1111				//ע��
#define SUB_CS_HS_ADD_GAME_SERVER				0x1112				//���ӷ���
#define SUB_CS_HS_SUB_GAME_SERVER				0x1113				//���ٷ���

#define SUB_CS_GS_REGISTER								0x1120				//ע����Ϸ������
#define SUB_CS_GS_UNREGISTER							0x1121				//ע��
#define SUB_CS_GS_SERVER_ONLINE_COUNT		0x1122				//��������


//ע�����������
struct tagCS_HS_RegisterHallServer
{
	tagCS_HS_RegisterHallServer()
	{
		memset(this,0,sizeof(*this));
	}
	tagHallServer		HallServer;						
};

//ע������������
struct tagCS_HS_UnregisterHallServer
{
	tagCS_HS_UnregisterHallServer()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wServerID;				
};


struct tagCS_HS_AddGameServer
{
	tagCS_HS_AddGameServer()
	{
		memset(this,0,sizeof(*this));
	}
	tagGameServer		GameServer;						
};
struct tagCS_HS_SubGameServer
{
	tagCS_HS_SubGameServer()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wKindID;
	WORD				wServerID;
};

//ע�᷿��
struct tagCS_GS_RegisterServer
{
	tagCS_GS_RegisterServer()
	{
		memset(this,0,sizeof(*this));
	}
	tagGameServer		GameServer;						//������Ϣ
};

//ע������
struct tagCS_GS_UnregisterServer
{
	tagCS_GS_UnregisterServer()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wKindID;						//���ƺ���
	WORD				wServerID;						//�������
};

//��������
struct tagCS_GS_ServerOnlineCount
{
	tagCS_GS_ServerOnlineCount()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wKindID;						//���ƺ���
	WORD				wServerID;						//�������
	DWORD				dwOnlineCount;					//��������
};



//-----------------------------------------------
//ƽ̨ά��
//

#define MAIN_CS_PLATFORM_MAINTENANCE			0x1200				

#define SUB_CS_MAINTENANCE				0x1201			//ά��


//ά��
struct tagCS_Maintenance
{
	tagCS_Maintenance()
	{
		memset(this,0,sizeof(*this));
		wDelayCloseMinutes = 10;
	}
	bool	bMaintenanceFlag;				//�Ƿ񽫹ر�
	WORD	wDelayCloseMinutes;	//�ӳٹرյ�ʱ��
};


// ----------------------------------------------
// ������

#define MAIN_CS_SERVER_ROBOT							0x1300				// ����������
#define SUB_GC_ROBOT_LOADROBOT_REQ			0x1301				// ����������˺�
#define SUB_GC_ROBOT_OFFLINE_REQ					0x1302				// ������������Ϣ

#define SUB_CG_ROBOT_SENDACCOUNTS				0x1303				// ���ͻ������˺�

//�����û�
struct tagCS_RobotUser
{
	tagCS_RobotUser()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;			// �û� I D
	DWORD							dwRoomID;		// ���ڷ���
	DWORD							dwWorkTime;	// ��ʼ����ʱ��
	BOOL								bEnabled;			// �Ƿ���Ч
};

// �������ӻ�����
struct tagCS_NeedRobot_Req
{
	tagCS_NeedRobot_Req()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;				// ���� I D
	DWORD							dwRobotCounts;		// ����������
};

// ���ͻ������˺�
struct tagCS_SendRobotAccounts
{
	tagCS_SendRobotAccounts()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;							// ���� I D
	DWORD							dwUserID[MAX_ROBOT];		// ������userid
	DWORD							dwRobotCounts;					// ����������
};

// ��������������
struct tagCS_OffLineReq
{
	tagCS_OffLineReq()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;		// ���� I D
	DWORD							dwUserID;			// ������userid
};


// ----------------------------------------------
// ������

#define MAIN_CS_STOCKSCORE							0x2400				// �������
#define SUB_CS_STOCKSCORE_SET_REQ				0x2401				// �������ÿ��
#define SUB_CS_STOCKSCORE_UPDATE_REQ		0x2402				// ������¿��
#define SUB_CS_STOCKSCORE_GET						0x2403				// ��ȡ���


struct tagCS_StockScore
{
	tagCS_StockScore()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD							dwRoomID;				// ���� I D
	LONGLONG						lScore;						// ���ֵ/����ֵ
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
