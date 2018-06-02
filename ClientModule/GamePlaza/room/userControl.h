#ifndef _USER_CONTROL_
#define _USER_CONTROL_

#include "..\\share\\control\\IUserControl.h"

typedef void(*ProcCreatUserMenu)(CPaintManagerUI*, ResInfo*, UserMenuStruct*, IUserControl*);
typedef void(*ProcDeleteUserMenu)(HWND hWnd);
typedef void(*ProcReciveWinScore)(LONGLONG lCurWinScore);
typedef void(*ProcReciveWinCount)(LONGLONG lCurWinCount);
typedef void(*ProcReciveLostCount)(LONGLONG lCurLostCount);
typedef void(*ProcReciveRoomSetting)(RoomSetData& tRoomSetData);
typedef void(*ProcReciveStokSetting)(LONGLONG lCurStokValue);
typedef void(*ProcReciveRobotCount)(LONGLONG lCurRobotCount);
typedef void(*ProcReciveRobotScore)(LONGLONG lCurRobotMax, LONGLONG lCurRobotMin);

class UserControl : public IUserControl
{
public:
	static UserControl* GetInstance();

	// ���ù�����ͼ������
	void BindPaintManager(CPaintManagerUI* pManager){ m_pManager = pManager; }

	// ��ʾ�˵�
	void ShowMenu(UserData* pTagUser);

	// ����һ��Ĳ˵�
	void CreateGeneralMenu();

	// ��������Ĳ˵�
	void CreatSpecialMenu();

	// ��ȡ��Ҷ���
	UserData* GetUserData(){ return m_pTagUser; }

	// �Ƿ��Ѿ�������DLL
	bool IsLoadMenuDll(){ return m_bLoadSuccess; }

	// ����DLL
	bool LoadUserMenuDll();

	// ж��DLL
	void UnLoadUserMenuDll();

	// ����ж��Ԥ����
	void PreUnLoadUserMenuDll();
	
	// ���շ������ص��Ŀ�������
	void OnGMControData(void* pDataBuffer, WORD wDataSize);

	// ���շ������ûص�����
	void OnGMRoomData(void* pDataBuffer, WORD wDataSize);

// -----------------------------------------------------------------------
// �˵��ص��ӿ�
public:
	
	void ReqSetScore();																// ����ǰ������õ���Ӯ����
	void ReqWinCount();																// ����ǰ���Ӯ��������
	void ReqLostCount();															// ����ǰ����伸������
	void ReqRoomSetting();															// ����ǰ��������
	void ReqStokSetting();															// ����ǰ�������
	void ReqRobotCount();															// ����ǰ��������������
	void ReqRobotScore();															// ����ǰ�����˷�������

	void CallBackLocation();														// ��λ���
	void CallBackSetScore(LONGLONG lGameID, LONGLONG lNewValue);					// ������Ӯ����
	void CallBackWinCount(LONGLONG lGameID, LONGLONG lCount);						// ����Ӯ����
	void CallBackLostCount(LONGLONG lGameID, LONGLONG lCount);						// �����伸��
	void CallBackKickRoom();														// �߳�����
	void CallBackSendSysMsg(LPCTSTR lpMsg, bool bSendToGame, bool bSendToRoom);		// ����ϵͳ��Ϣ
	void CallBackForbidAccount();													// �����û��˺�
	void CallBackLookIP();															// �鿴�û�IP
	void CallBackRoomSetting(bool bRoomChat, bool bRoomWisper, bool bGameChat
		, bool bEnterRoom, bool bEnterGame, bool bShallClose);						// ��������
	void CallBackStokSetting(LONGLONG lNewValue);									// ���õ�ǰ���
	void CallBackAddBlackList();													// ���������
	void CallBackDelBlackList();													// ɾ��������
	void CallBackSetRobotCount(DWORD dwRobotCount);									// ���û���������
	void CallBackSetRobotScore(LONGLONG lMax, LONGLONG lMin);						// ���û����˷�����Χ

private:
	UserControl();
	~UserControl();
	static UserControl*				g_instance;					// ����

	bool							m_bLoadSuccess;				// �Ƿ���سɹ�
	TCHAR							m_pzBuffer[MAX_PATH];		// �ַ���������
	std::map<CDuiString, bool>		m_MenuCheckInfo;			// ����˵��ĵ�ѡ��ѡ��Ϣ
	HINSTANCE						m_hInst;					// ��̬����
	CPaintManagerUI*				m_pManager;					// ���ڻ�����
	RoomManager*					m_pRoomManager;				// ���������
	UserData*						m_pTagUser;					// �û�����ָ��

	ProcCreatUserMenu				m_pFuncCreateMenu;			// �����˵����
	ProcDeleteUserMenu				m_pFuncDeleteMenu;			// ж�ز˵����
	ProcReciveWinScore				m_pFuncReciveWinScore;		// ����������Ӯ����
	ProcReciveWinCount				m_pFuncReciveWinCount;		// ����Ӯ����
	ProcReciveLostCount				m_pFuncReciveLostCount;		// �����伸��
	ProcReciveRoomSetting			m_pFuncReciveRoomSetting;	// ���շ�������
	ProcReciveStokSetting			m_pFuncReciveStokSetting;	// ���տ������
	ProcReciveRobotCount			m_pFuncReciveRobotCount;	// �������û���������
	ProcReciveRobotScore			m_pFuncReciveRobotScore;	// �������û����˷�����Χ
};

#endif // _USER_CONTROL_