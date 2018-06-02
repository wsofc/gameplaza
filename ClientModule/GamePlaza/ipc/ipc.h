#ifndef __IPC_H__
#define __IPC_H__

#include "../ipc/loginServer.h"
#include "../ipc/gameServer.h"

// ��Ϸ���������ṹ
struct tagGameFrameReconnect 
{
	BYTE	cbMaxCount;		// �������޴���
	BYTE	cbInterval;		// ÿ�εļ��ʱ��
	BYTE	cbStatus;		// ��������״̬(0 - ����, 1 - ��ʼ, 2 - �ɹ�, 3 - ʧ��)
};

// ������Ϸ���ڽṹ��
typedef struct tagWNDINFO
{
	DWORD dwProcessID;
	HWND hWnd;

}WNDINFO, *LPWNDINFO;

typedef std::function< void(bool bRes)> StartGamesCallBack;

/*@breif: ���̼�ͨ�Ź�����(���� + �Ǳ���)
* ���������Windows API SendMessage���Ʒ���WM_COPYDATA��Ϣ������ͨ��
*/
class IPC
{
public:
	
	enum IPCMsgBoxID
	{
		MSG_BOX_ID_START_GAME_FAILED,	// ������Ϸʧ��
		MSG_BOX_ID_IPC_TIPS,			// ipc��ʾ
	};

	// ��ȡ��ʵ��
	static IPC* GetInstance();
	static bool IsExistInstance(){ return g_instance != nullptr; }
	static void Release(){ SAFE_DELETE(g_instance); }
	void Reset();

	// ��ȡ��Ϸ���ھ��
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	HWND GetWindowHandleByPID(DWORD dwProcessID);
	
	// ������Ϸ
	void StartGames(CPaintManagerUI* pManager, LPCTSTR lpGameName, LPCTSTR lpModelName);
	void StartGamesInfo();
	bool IsGameStarted();
	bool IsGameFinished();
	void MsgBoxProc(DLG_ID id, TNotifyUI& msg);
	
	// �Ͽ�����
	void DisConnect(bool bShowTips = true);

	// ��Ϸ�������
	void StartHeartbeat(int iMaxHeartOutCount);
	void CloseHeartbeat();
	void OnHeartbeat(UINT_PTR idEvent, DWORD dwTime);
	void OnStepTimer(UINT_PTR idEvent, DWORD dwTime);
	void CloseStepTimer();

	// ��������ͨ��
	bool LocalSendData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize); // �·����������ؽ���
	bool LocalRecvData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize); // ������������������

private:

	IPC();
	~IPC();

	void _SendListHeadData();																						// �·��û��б�ͷ��Ϣ
	void _SendListBodyData();																						// �·��û��б�����Ϣ
	bool _LocalSendData(HWND hWnd, DWORD dwFlag, const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// �·�������ָ�����ڽ���
	bool _CreateGameProcess(CPaintManagerUI* pManager, LPCTSTR lpGameName, LPCTSTR lpModelName);					// ������Ϸ����
	void _CloseGameProcess();																						// �رս���
	
private:
	static IPC* g_instance;						// ��ʵ��
	CPaintManagerUI* m_pManager;				// ���ƹ�����
	TCHAR m_strBuffer[MAX_PATH];				// ���ַ���������

	HWND m_hWnd;								// ���������ھ��
	HWND m_hTargetWnd;							// Ŀ�괰�ھ��(Winform����Ϸ)
	bool m_bConnect;							// �Ƿ���������Ϸ����ͨѶ
	bool m_bGameFinished;						// ��Ϸ�Ƿ��������
	bool m_bHeartbeat;							// �Ƿ�������
	DWORD m_dwHeartOut;							// ������ʱ����
	DWORD m_dwMaxHeartOut;						// ������ʱ����������

	IPC_Buffer m_IPCBuf;						// ����ͨ��IPC����
	PROCESS_INFORMATION	m_tTargetProcessInfo;	// Ŀ�������Ϣ(Winform����Ϸ)
	PROCESS_INFORMATION m_tNestedProcessInfo;	// Ƕ�״��ڽ�����Ϣ
};

#endif //__IPC_H__