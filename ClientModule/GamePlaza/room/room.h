#ifndef __ROOM_H__
#define __ROOM_H__

#include "roomManager.h"
#include "tableManager.h"

enum ROOM_STATUS //����״̬
{
	ROOM_STATUS_NULL = 0,	//Ĭ��:��
	ROOM_STATUS_FREE,		//����

	ROOM_STATUS_EXTRA		//�����Ԥ����
};

enum ROOM_TYPE //��������
{
	ROOM_TYPE_NULL = 0,		//Ĭ��:��
	ROOM_TYPE_NORMAL,		//����
	ROOM_TYPE_TIME_MATCH,	//��ʱ��

	ROOM_TYPE_EXTRA			//�����Ԥ����
};

// �������ṹ
struct PropsPanelStruct
{
	DWORD						dwMaxCountsPerPage;		// һҳ��ʾ�������Ʒ����
	DWORD						dwCurPage;				// ��ǰҳ
	DWORD						dwMinPage;				// ��Сҳ
	DWORD						dwMaxPage;				// ���ҳ
	CButtonUI*					pBtnLeft;				// ��ҳ��ť
	CButtonUI*					pBtnRight;				// �ҷ�ҳ��ť
	CString						pzField;				// �ؼ������ֶ�
	std::vector<tagPropsInfo*>	vPropsMapRef;			// ������������

	void Reset()
	{
		dwMaxCountsPerPage	= 0;
		dwCurPage			= 0;
		dwMinPage			= 0;
		dwMaxPage			= 0;
		pBtnLeft			= nullptr;
		pBtnRight			= nullptr;
		pzField				= _T("");
		vPropsMapRef.clear();
	}
};

class MallBuyFrame;
class RoomManager;

class Room
{
public:

	Room(CPaintManagerUI* pManager, RoomManager* pRoomManagerRef);
	~Room();

	void InitPropsUI();																		// ��ʼ��������UI
	void ResetPropsUI();																	// ���������UI
	void PropsTurnPage(PropsPanelStruct& tTarget, bool bNext);								// ��������ҳ
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// ������Ϣ

	void InitRoomData(tagGameServer* pData);												// ������¼ʱ����
	void InitRoomData(CMD_GR_ServerInfo* pData);											// �����¼ʱ����
	void Notify(TNotifyUI& msg);															// �ؼ���Ϣ
	void CreatTables();																		// ��������
	void RefreshTitleLabel(bool bShowOnline = true);										// ���·������Lab
	void UpdateTableStatus(tagGS_TableStatus& tStruct);										// ��������״̬
	void UpdateTableStatus(WORD wTableID, tagTableStatus& tStruct);							// ��������״̬
	bool UpdateChairsStatus(UserData& uStruct);												// ��������״̬
	void SitDownOnTableReq(UserData& uStruct);												// ��������
	void StandUpOnTableReq(UserData& uStruct);												// ��������
	void QuickJoin(bool bAnimation);														// ���ټ���
	void QuickLocation(DWORD dwUserID);														// ���ٶ�λ���
	
	void AddOnLineCount(int iValue){ m_dwOnlineCount += iValue; }
	void SetServerID(WORD wServerID)	{ m_wServerID = wServerID; }
	void StartGames();																		// ������Ϸ
	void CloseGameNotify(bool bEnforce = false);											// �ر���Ϸ(bEnforce:ǿ�ƹر�)
	void ClearUserDataMap();																// ���������ݱ�
	void ClearUserListUI();																	// ��������б�UI
	void ResetChatUI();																		// ��������UI
	void ClearAll();																		// ���������������

	const bool IsHundredGame()			{ return m_bHundredGame; }
	const bool IsHundredLayout()		{ return m_bHundredLayout; }
	const WORD GetKindID()				{ return m_wKindID; }
	const WORD GetServerID()			{ return m_wServerID; }
	const WORD GetSortID()				{ return m_wSortID; }
	const WORD GetStationID()			{ return m_wStationID; }
	const WORD GetServerType()			{ return m_wRoomServiceType; }
	const DWORD GetOnlineCount()		{ return m_dwOnlineCount; }
	const LPCTSTR GetName()				{ return m_pzName; }
	const LPCTSTR GetGameName()			{ return m_pzGameName; }
	const LPCTSTR GetSeasonName()		{ return m_pzSeasonName; }

	const WORD GetTableCount()			{ return m_wTableCount; }
	const WORD GetChairCount()			{ return m_wChairCount; }
	const LONGLONG GetLessScore()		{ return m_lLessScore; }
	const LONGLONG GetMaxScore()		{ return m_lMaxScore; }
	const LONGLONG GetRestrictScore()	{ return m_lRestrictScore; }
	const WORD GetServerPort()			{ return m_wServerPort; }
	const DWORD GetServerAddr()			{ return m_dwServerAddr; }
	const DWORD GetVideoAddr()			{ return m_dwVideoAddr; }
	const BYTE GetHideUserInfo()		{ return m_cbHideUserInfo; }
	TableManager* GetTableManager()		{ return m_pTableManager; }

private:

	bool						m_bHundredGame;						// �Ƿ��ǰ�����Ϸ
	bool						m_bHundredLayout;					// �Ƿ������Ϸ����(���˲���:������, ���沼��:������)
	WORD						m_wKindID;							// ������ϷI D
	WORD						m_wServerID;						// ����ID
	WORD						m_wSortID;							// �������
	WORD						m_wStationID;						// վ�����
	WORD						m_wRoomServiceType;					// ����/��������
	DWORD						m_dwOnlineCount;					// ��������
	CDuiString					m_pzName;							// ������
	CDuiString					m_pzSeasonName;						// ������
	CDuiString					m_pzGameName;						// ������Ϸ��

	WORD						m_wTableCount;						// ������Ŀ
	WORD						m_wChairCount;						// ������Ŀ
	WORD						m_wTaxRate;							// ��Ϸ˰��
	LONGLONG					m_lLessScore;						// ��ͻ���
	LONGLONG					m_lMaxScore;						// ��߻���
	LONGLONG					m_lRestrictScore;					// ���ƻ���
	
	WORD						m_wServerPort;						// ����˿�
	DWORD						m_dwServerAddr;						// �����ַ
	DWORD						m_dwVideoAddr;						// ��Ƶ��ַ
	
	BYTE						m_cbHideUserInfo;					// ����������Ϣ
	
	CPaintManagerUI*			m_pManager;							// ���ڻ�ͼ������
	RoomManager*				m_pRoomManagerRef;					// �������������
	TableManager*				m_pTableManager;					// ���ӹ�����

	TCHAR						m_pzBuffer[MAX_PATH];				// �ַ���������
	PropsPanelStruct			m_tPropsPanel;						// �������
	PropsPanelStruct			m_tGiftsPanel;						// �������
};

#endif //__ROOM_H__