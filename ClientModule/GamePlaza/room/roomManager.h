#ifndef __ROOM_MANAGER_H__
#define __ROOM_MANAGER_H__
#include "room.h"
#include "ui/UIGameItem.h"
#include "ipc/gameServer.h"

class Room;
class RoomManager : public GameSeverDelegate
{
public:

	// ����״̬
	typedef enum Broken_Network_Status
	{
		ONS_NORMAL	= 0,					// �����Ͽ�
		ONS_INTERMIT,						// ��Զ���ж�
		ONS_EXCEPTION,						// �����쳣
		ONS_FINALCLOSE						// �����ر�(������ʾ)
	}OFFNetStatus;

	enum RoomMsgBoxID
	{
		MSG_BOX_ID_NONE = 0,				// ��
		MSG_BOX_ID_GAME_STATUS_NONE,		// ��Ϸ������
		MSG_BOX_ID_GAME_STATUS_LOW,			// ��Ϸ�汾����
		MSG_BOX_ID_LOGIN_ROOM_CANCEL,		// ȡ����¼����
		MSG_BOX_ID_LOGIN_ROOM_DISCONNECT,	// ��������Ͽ�
		MSG_BOX_ID_LOGIN_ROOM_FAILED,		// ��¼����ʧ��
	};

	static RoomManager* GetInstance();
	static void Release();

	void InitStyle_1(CPaintManagerUI* pManager, CGameItemUI* pGameItem);
	void InitStyle_2(CPaintManagerUI* pManager, CGameItemUI* pGameItem);

	// �����������͵�ѡ��ť
	void CreatRoomOptBtn(int iIndex, LPCTSTR pzText);

	// ˢ�·����б�
	void RefreshRoomList(int iStyle = 0);

	// ���÷����б��۵����
	void SetRoomListExpand(bool bExpand);

	// ��ӡ��ҽ���������ʾ�������
	void PrintUserInOutRoom(UserData* pUser, bool bInOut);

	// ���뷿��
	void EnterRoomReq(WORD wRoomID);

	// �˳�����
	void ExitPanel(bool bExitToHall);

	// �������
	void Clean();

	// ��ȡ��ǰ���ڹ�����
	CPaintManagerUI* GetPaintManager() { return m_pManagerRef; }

	// ��ȡ��ǰ����ID
	WORD GetCurRoomID() { return m_wCurRoomID; }

	// ��ȡ��ǰ����
	Room* GetCurRoom(){ return m_curRoom; }

	// ������ҿ������
	bool LoadUserControl(CPaintManagerUI* pManager);

	// ж����ҿ������
	void UnLoadUserControl();

	// �����Ϸ״̬
	void CheckGameStatus();

	// �Ի����¼��ص�
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	// ֪ͨ��Ϸ�������������ݵ�
	void NotifyGameServerUpdateBank();

	void Notify(TNotifyUI& msg);

	/*@breif: �����ͼ���ڵ�
	*@param	tabLayout		- �ҽӵĸ��ڵ�
	*@param pzName			- ���θ��ڵ���
	*/
	CTreeViewUI* AddSeasonRoot(CTabLayoutUI* tabLayout, LPCTSTR pzName);

	/*@breif: ��ӳ��νڵ�
	*@param	pControl		- ������CTreeViewUI����(�ú����ڲ�����dynamic_cast���)
	*@param name			- ���νڵ���
	*@param text			- ������ʾ�ı�
	*@param height			- ���νڵ�߶�ֵ
	*/
	CTreeNodeUI* AddSeasonNode(CControlUI* pControl, LPCTSTR pzName, LPCTSTR pzText, DWORD height);

	/*@breif: ��ӳ�������
	*@param	pSeason			- ���νڵ�
	*@param pRoomData		- ����ṹ
	*/
	CTreeNodeUI* AddSeasonItem(CTreeNodeUI* pSeason, tagGameServer* pRoomData);

	void MsgBoxProc(DLG_ID id, TNotifyUI& msg);
	void ShowWaittingText(bool bShow, CDuiString pzText = _T(""));							// ��ʾ���ӵȴ���ʾ��
	void ShowDisconnectText(bool bShow, CDuiString pzText = _T(""));						// ��ʾ����������ʾ��

	// ----------------------------------------------------------------------------------------
	// ����ģ��
	void Download();																		// ����

	// ����ģ��
	void GameServerConnect(WORD wRoomID);													// ��¼����
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// ����
	void DisConnect();																		// �Ͽ�����
	void OffNetTips(BYTE cbShutReason);														// ������ʾ
	bool OnConnectProc(bool bSuccess);														// ���ӽ���ص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// ����Ͽ�֪ͨ

	void StartReTry();																		// �����Զ�����
	void OnRetry();																			// ��ʼ������
	void EndReTry();																		// �����Զ�����
	void ReconnectClear();																	// �������֮ǰ������״̬��UI��
	void OnTimer(UINT_PTR idEvent, DWORD dwTime);

	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID);										// ����
	DWORD SendData(WORD wMainCmdID, WORD wSubCmdID, void* const pData, WORD wDataSize);		// ����
	bool OnNetwork_GameToServer(void* pData);												// ��Ϸ��������Ϣ
	bool OnNetwork_TurnToGame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);	// ��Ϸ��������Ϣ

private:
	bool OnNetwork_Logon(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// �����¼
	bool OnNetwork_Status(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// ������Ϣ
	bool OnNetwork_HallMSG(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// ϵͳ��Ϣ
	bool OnNetwork_ServerInfo(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);	// ������Ϣ
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// ������Ϣ
	bool OnNetwork_User(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);			// �û���Ϣ
	bool OnNetwork_Info(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);			// ������Ϣ
	bool OnNetwork_Manager(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// ��������
	bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// ������Ϣ
	bool OnNetwork_Game(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);			// ��Ϸ�߼���Ϣ
	bool OnNetwork_GameFrame(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);	// ��Ϸ״̬��Ϣ
	
	void _InitGameLevelConfig();															// ��ʼ���ȼ�����
	void _InitRoomSeasonConfig();															// ��ʼ�����䳡������
	void _InitRoomListUI_1();																// ��ʼ�������б���1(��ʱע��)
	void _InitRoomListUI_2();																// ��ʼ�������б���2
	void _CleanRoomListUI();																// ����������UI

	void _UpdateSystemMessage(tagGS_Message& msg);											// ����ϵͳ��Ϣ
	void _UpdateUserMessage(CMD_GR_UserChat& msg);											// �����û���Ϣ
	bool _UpdateUserComing(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// �����û���Ϣ
	void _UpdateUserStatus(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// �����û�״̬
	void _UpdateUserScore(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// �����û�����
	void _UpdateUserRight(void* pDataBuffer, WORD wDataSize);								// �����û�Ȩ��
	void _UpdateUserMember(void* pDataBuffer, WORD wDataSize);								// �����û���Ա�ȼ�

	void _UpdateUserStatusUI(const CMD_Command& cmd, UserData& stuTagUser);					// �������״̬UI
	void _EnterRoom();																		// ���뷿��
	void _ExitRoom();																		// ���˳�����
	void _ExitToHall();																		// �˳�������

private:
	static RoomManager* g_instance;
	RoomManager();
	~RoomManager();
	
	bool				m_bLoginSuccess;													// �Ƿ��¼����ɹ�
	bool				m_bLoginFinished;													// �Ƿ��¼�����������
	int					m_iSelfComingCount;													// �Լ���coming�㲥����(��Ϊ��㲥�Լ�����)
	TCHAR				m_pzBuffer[MAX_PATH];												// �ַ���������
	BYTE				m_cbBuffer[IPC_BUFFER];												// ���ݻ�����

	CPaintManagerUI*	m_pManagerRef;
	DWORD				m_wSeasonNodeHeight;												// ���䳡�α�ͷUI�߶�
	CGameItemUI*		m_pGameItem;														// ������Ϸ
	Room*				m_curRoom;															// ��ҵ�ǰ����
	MsgBoxWnd*			m_pWaittingMsgBoxDlg;												// �ȴ��Ի���
	MsgBoxWnd*			m_pDisconnectMsgBoxDlg;												// ���������Ի���
	WORD				m_wCurRoomID;														// ��ǰ����ID
	OFFNetStatus		m_uOffNetStatus;													// ����Ͽ�״̬(���ڶ�����ʾ,������)
	GameServer*			m_pGameServer;														// ��Ϸ�������ӿڶ���
	BYTE				m_cbReConnectCount;													// �����쳣��������
	BYTE				m_cbReConnecting;													// ��������״̬(1 - ��ʼ, 0 - ����)
	LONGLONG			m_llReConnectTotalCount;											// ���������ڼ���������ܴ���
	
	std::vector<CDuiString>							m_vOptName;								// ��ѡ��ť��
	std::map<DWORD, std::vector<tagGameServer*> >	m_RoomListMap;							// �����б�
};

#endif //__ROOM_MANAGER_H__