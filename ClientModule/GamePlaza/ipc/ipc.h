#ifndef __IPC_H__
#define __IPC_H__

#include "../ipc/loginServer.h"
#include "../ipc/gameServer.h"

// 游戏断线重连结构
struct tagGameFrameReconnect 
{
	BYTE	cbMaxCount;		// 重连上限次数
	BYTE	cbInterval;		// 每次的间隔时间
	BYTE	cbStatus;		// 断线重连状态(0 - 结束, 1 - 开始, 2 - 成功, 3 - 失败)
};

// 查找游戏窗口结构体
typedef struct tagWNDINFO
{
	DWORD dwProcessID;
	HWND hWnd;

}WNDINFO, *LPWNDINFO;

typedef std::function< void(bool bRes)> StartGamesCallBack;

/*@breif: 进程间通信管理类(本机 + 非本机)
* 本机间采用Windows API SendMessage机制发送WM_COPYDATA消息来进行通信
*/
class IPC
{
public:
	
	enum IPCMsgBoxID
	{
		MSG_BOX_ID_START_GAME_FAILED,	// 启动游戏失败
		MSG_BOX_ID_IPC_TIPS,			// ipc提示
	};

	// 获取单实例
	static IPC* GetInstance();
	static bool IsExistInstance(){ return g_instance != nullptr; }
	static void Release(){ SAFE_DELETE(g_instance); }
	void Reset();

	// 获取游戏窗口句柄
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	HWND GetWindowHandleByPID(DWORD dwProcessID);
	
	// 启动游戏
	void StartGames(CPaintManagerUI* pManager, LPCTSTR lpGameName, LPCTSTR lpModelName);
	void StartGamesInfo();
	bool IsGameStarted();
	bool IsGameFinished();
	void MsgBoxProc(DLG_ID id, TNotifyUI& msg);
	
	// 断开连接
	void DisConnect(bool bShowTips = true);

	// 游戏心跳检测
	void StartHeartbeat(int iMaxHeartOutCount);
	void CloseHeartbeat();
	void OnHeartbeat(UINT_PTR idEvent, DWORD dwTime);
	void OnStepTimer(UINT_PTR idEvent, DWORD dwTime);
	void CloseStepTimer();

	// 本机进程通信
	bool LocalSendData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize); // 下发数据至本地进程
	bool LocalRecvData(const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize); // 上行数据至大厅进程

private:

	IPC();
	~IPC();

	void _SendListHeadData();																						// 下发用户列表头信息
	void _SendListBodyData();																						// 下发用户列表项信息
	bool _LocalSendData(HWND hWnd, DWORD dwFlag, const CMD_Command& cmd, void* pDataBuffer, WORD wDataSize);		// 下发数据至指定窗口进程
	bool _CreateGameProcess(CPaintManagerUI* pManager, LPCTSTR lpGameName, LPCTSTR lpModelName);					// 开启游戏进程
	void _CloseGameProcess();																						// 关闭进程
	
private:
	static IPC* g_instance;						// 单实例
	CPaintManagerUI* m_pManager;				// 绘制管理器
	TCHAR m_strBuffer[MAX_PATH];				// 宽字符缓冲数组

	HWND m_hWnd;								// 大厅主窗口句柄
	HWND m_hTargetWnd;							// 目标窗口句柄(Winform或游戏)
	bool m_bConnect;							// 是否已连接游戏进程通讯
	bool m_bGameFinished;						// 游戏是否启动完毕
	bool m_bHeartbeat;							// 是否有心跳
	DWORD m_dwHeartOut;							// 心跳超时次数
	DWORD m_dwMaxHeartOut;						// 心跳超时重连最大次数

	IPC_Buffer m_IPCBuf;						// 本机通信IPC缓存
	PROCESS_INFORMATION	m_tTargetProcessInfo;	// 目标进程信息(Winform或游戏)
	PROCESS_INFORMATION m_tNestedProcessInfo;	// 嵌套窗口进程信息
};

#endif //__IPC_H__