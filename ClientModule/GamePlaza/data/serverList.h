#ifndef __SERVERLIST_H__
#define __SERVERLIST_H__

#include "..\\ipc\\loginServer.h"
class ServerList : public LoginSeverDelegate
{
public:

	enum OperateStatus
	{
		OS_NONE				= 0			// ��״̬
		, OS_REFRESH_LIST				// ˢ���б�
		, OS_EXTRA						// ����Ԥ����
	};

	enum RefreshType
	{
		RF_TYPE_NONE		= 0			// ��״̬
		, RF_TYPE_GAMELIST				// ˢ����Ϸ�б�
		, RF_TYPE_ROOMLIST				// ˢ�·����б�
	};

	static ServerList* GetInstance();
	~ServerList();

	void RefreshList(CPaintManagerUI* pManagerRef, RefreshType uType);				// ˢ���б���Ϣ

public:
	bool OnConnectSuccessProc();
	bool OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);

private:
	void _RefreshListReq();															// ˢ������
	void _InitGameConfigList(void* pData, WORD wDataSize);							// ��Ϸ�б�����
	void _InitGameTypeList(void* pData, WORD wDataSize);							// ��Ϸ�����б�
	void _InitGameKindList(void* pData, WORD wDataSize);							// ��Ϸ�����б�
	void _initGameStationList(void* pData, WORD wDataSize);							// ��Ϸվ���б�
	void _InitGameRoomList(void* pData, WORD wDataSize);							// ��Ϸ�����б�
	void _InitGameListComplete();													// ��Ϸ�б��������

private:

	ServerList();
	static ServerList*		g_instance;

	RefreshType				m_uRefreshType;						// ˢ���б�����(1 - ˢ����Ϸ�б�, 2 - ˢ�·����б� ����ͬʱˢ������,���̲�����)
	CPaintManagerUI*		m_pManagerRef;						// ���ڹ���������
	ServiceData*			m_pServiceData;						// ȫ�ַ�������
	LoginServer*			m_pLoginServer;						// ��¼�������ӿڶ���
	OperateStatus			m_iCurOperateStatus;				// ��ǰ����״̬
};

#endif // __SERVERLIST_H__