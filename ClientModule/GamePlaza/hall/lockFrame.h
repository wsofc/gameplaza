#ifndef __LOCK_FRAME_H__
#define __LOCK_FRAME_H__

#include "loginFrame.h"
class LoginFrame;
class HallFrame;

// ------------------------------------------------------------------------------------------------------
// 短信验证登录
class SMSLonginCheckFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	enum OperateStatus
	{
		OS_NONE				// 无状态
		, OS_GETVCODE		// 获取验证码
		, OS_EXTRA			// 额外预留项
	};

	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0,		// 无
		MSG_BOX_ID_DEFAULT,			// 默认
		MSG_BOX_ID_VERTCODE			// 短信验证码
	};
	
	~SMSLonginCheckFrame();
	static SMSLonginCheckFrame* GetInstance();
	static bool IsInstanceExist(){ return SMSLonginCheckFrame::g_instance != nullptr; }

	BOOL CreateFrame(CPaintManagerUI* pParentManager, LPCTSTR pzAccount);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);

	void InitData(TNotifyUI& msg);
	void Check();
	void SMSCheckFailed(LPCTSTR lpErrorMsg);
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	// 验证码定时器
	void SetVertcodeTimer();
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);

public:
	bool OnConnectSuccessProc();															// 网络连接回调
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// 重连
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// 网络断开通知
	void GetServerSMS();																	// 从服务端获取短信
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 短信

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("SMSLonginCheckFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	SMSLonginCheckFrame();
	static SMSLonginCheckFrame*		g_instance;

	CButtonUI*						m_pBtnGetCode;
	CButtonUI*						m_pBtnLogin;
	CRichEditUI*					m_pEdtVertCode;
	CDuiString						m_pzAccount;
	CPaintManagerUI*				m_pParentManager;		// 父窗口的绘制管理器

	LONG							m_llVertcodeSec;		// 验证码计时秒数
	OperateStatus					m_iCurOperateStatus;	// 当前操作状态
	LoginServer*					m_pLoginServer;			// 登录服务器接口对象

};

// ------------------------------------------------------------------------------------------------------
// 登录解锁
class LockLoginFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	LockLoginFrame();
	~LockLoginFrame();
	BOOL CreateFrame(CPaintManagerUI* pParentManager);

	enum OperateStatus
	{
		OS_NONE				// 无状态
		, OS_GETVCODE		// 获取验证码
		, OS_MODIFY			// 修改密码
		, OS_EXTRA			// 额外预留项
	};

	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0			// 无
		, MSG_BOX_ID_ACCOUNT		// 账号
		, MSG_BOX_ID_TELPHONE		// 手机号
		, MSG_BOX_ID_VERTCODE		// 验证码
		, MSG_BOX_EXTRA				// 预留
	};

public:
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

	void InitData(TNotifyUI& msg);
	void ChangeTipsImage(CControlUI* pControl, DWORD dwStatus);
	void Check();
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	// 验证码定时器
	void SetVertcodeTimer();
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);

	// 相关输入禁用与否
	void SetEdtCtlEnable(bool bEnable);

public:
	bool OnConnectSuccessProc();															// 网络连接回调
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// 重连
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// 网络断开通知
	void GetServerSMS();																	// 从服务端获取短信

	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 短信
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 用户消息

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("LockLoginFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	CRichEditUI*				m_pEdtAccount;			// 账号
	CControlUI*					m_pCtlAccount;

	CRichEditUI*				m_pEdtTelphone;			// 手机号
	CControlUI*					m_pCtlTelphone;

	CRichEditUI*				m_pEdtVertCode;			// 验证码
	CControlUI*					m_pCtlVertCode;

	LONG						m_llVertcodeSec;		// 验证码计时秒数
	CButtonUI*					m_pBtnGetCode;			// 获取验证码
	std::vector<stuMsgBoxFlag>	m_vCompleteFlag;		// 控件数组
	CPaintManagerUI*			m_pParentManager;		// 父窗口的绘制管理器

	OperateStatus				m_iCurOperateStatus;	// 当前操作状态
	LoginServer*				m_pLoginServer;			// 登录服务器接口对象
};

// ------------------------------------------------------------------------------------------------------
// 大厅锁定本机
class LockMachineFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	~LockMachineFrame();
	static LockMachineFrame* GetInstance();

	BOOL CreateFrame(CPaintManagerUI* pParentManager);
	
	enum LockOperateStatus
	{
		LOCK_OS_NONE					// 无状态
		, LOCK_OS_LOCKMACHINE			// 锁定本机
		, LOCK_OS_UNLOCKMACHINE			// 解锁本机
		, LOCK_OS_EXTRA					// 额外预留项
	};

	enum LockMsgBoxID
	{
		MSG_BOX_ID_NONE = 0				// 无
		, MSG_BOX_ID_LOCK_PW			// 锁定密码
	};

public:

	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);

	void InitData(TNotifyUI& msg);

	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);
	void TimerProFunc(UINT_PTR idEvent, DWORD dwTime);
	void LockMachine(BOOL lbLock);																			// 解、锁本机 
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);							// 用户消息
	bool OnConnectSuccessProc();																			// 网络连接成功回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);							// Socket断开
	void ReConnect(DLG_ID id, TNotifyUI& msg);																// 重连

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("LockMachineFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	LockMachineFrame();
	static LockMachineFrame*	g_instance;

	CPaintManagerUI*			m_pParentManager;
	CRichEditUI*				m_pEdtBankPwCtl;		// 银行密码
	DataManager*				m_pDataManager;			// 数据管理器
	LoginServer*				m_pLoginServer;			// 登录服务器接口对象
	LockOperateStatus			m_iCurOperateStatus;	// 当前操作状态
};

// ------------------------------------------------------------------------------------------------------
// 房间锁桌
class LockTableFrame : public WindowImplBase
{
public:
	enum LockTableMsgBoxID
	{
		MSG_BOX_ID_NONE = 0,		// 无
		MSG_BOX_ID_DEFAULT,			// 默认
		MSG_BOX_ID_PASSWORD			// 锁桌密码
	};

	LockTableFrame();
	~LockTableFrame();

	BOOL CreateFrame(Table* pTable, WORD wChairIndex);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);

	void InitData(TNotifyUI& msg);
	void Check();
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("LockTableFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	CRichEditUI*		m_pEdtPassword;
	Table*				m_pTable;
	WORD				m_wChairIndex;
};

#endif //__LOCK_FRAME_H__