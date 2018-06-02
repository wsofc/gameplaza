#ifndef __LOCK_FRAME_H__
#define __LOCK_FRAME_H__

#include "loginFrame.h"
class LoginFrame;
class HallFrame;

// ------------------------------------------------------------------------------------------------------
// ������֤��¼
class SMSLonginCheckFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	enum OperateStatus
	{
		OS_NONE				// ��״̬
		, OS_GETVCODE		// ��ȡ��֤��
		, OS_EXTRA			// ����Ԥ����
	};

	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0,		// ��
		MSG_BOX_ID_DEFAULT,			// Ĭ��
		MSG_BOX_ID_VERTCODE			// ������֤��
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

	// ��֤�붨ʱ��
	void SetVertcodeTimer();
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);

public:
	bool OnConnectSuccessProc();															// �������ӻص�
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// ����
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// ����Ͽ�֪ͨ
	void GetServerSMS();																	// �ӷ���˻�ȡ����
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// ����

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
	CPaintManagerUI*				m_pParentManager;		// �����ڵĻ��ƹ�����

	LONG							m_llVertcodeSec;		// ��֤���ʱ����
	OperateStatus					m_iCurOperateStatus;	// ��ǰ����״̬
	LoginServer*					m_pLoginServer;			// ��¼�������ӿڶ���

};

// ------------------------------------------------------------------------------------------------------
// ��¼����
class LockLoginFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	LockLoginFrame();
	~LockLoginFrame();
	BOOL CreateFrame(CPaintManagerUI* pParentManager);

	enum OperateStatus
	{
		OS_NONE				// ��״̬
		, OS_GETVCODE		// ��ȡ��֤��
		, OS_MODIFY			// �޸�����
		, OS_EXTRA			// ����Ԥ����
	};

	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0			// ��
		, MSG_BOX_ID_ACCOUNT		// �˺�
		, MSG_BOX_ID_TELPHONE		// �ֻ���
		, MSG_BOX_ID_VERTCODE		// ��֤��
		, MSG_BOX_EXTRA				// Ԥ��
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

	// ��֤�붨ʱ��
	void SetVertcodeTimer();
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);

	// �������������
	void SetEdtCtlEnable(bool bEnable);

public:
	bool OnConnectSuccessProc();															// �������ӻص�
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// ����
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// ����Ͽ�֪ͨ
	void GetServerSMS();																	// �ӷ���˻�ȡ����

	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// ����
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// �û���Ϣ

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("LockLoginFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	CRichEditUI*				m_pEdtAccount;			// �˺�
	CControlUI*					m_pCtlAccount;

	CRichEditUI*				m_pEdtTelphone;			// �ֻ���
	CControlUI*					m_pCtlTelphone;

	CRichEditUI*				m_pEdtVertCode;			// ��֤��
	CControlUI*					m_pCtlVertCode;

	LONG						m_llVertcodeSec;		// ��֤���ʱ����
	CButtonUI*					m_pBtnGetCode;			// ��ȡ��֤��
	std::vector<stuMsgBoxFlag>	m_vCompleteFlag;		// �ؼ�����
	CPaintManagerUI*			m_pParentManager;		// �����ڵĻ��ƹ�����

	OperateStatus				m_iCurOperateStatus;	// ��ǰ����״̬
	LoginServer*				m_pLoginServer;			// ��¼�������ӿڶ���
};

// ------------------------------------------------------------------------------------------------------
// ������������
class LockMachineFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	~LockMachineFrame();
	static LockMachineFrame* GetInstance();

	BOOL CreateFrame(CPaintManagerUI* pParentManager);
	
	enum LockOperateStatus
	{
		LOCK_OS_NONE					// ��״̬
		, LOCK_OS_LOCKMACHINE			// ��������
		, LOCK_OS_UNLOCKMACHINE			// ��������
		, LOCK_OS_EXTRA					// ����Ԥ����
	};

	enum LockMsgBoxID
	{
		MSG_BOX_ID_NONE = 0				// ��
		, MSG_BOX_ID_LOCK_PW			// ��������
	};

public:

	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);

	void InitData(TNotifyUI& msg);

	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);
	void TimerProFunc(UINT_PTR idEvent, DWORD dwTime);
	void LockMachine(BOOL lbLock);																			// �⡢������ 
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);							// �û���Ϣ
	bool OnConnectSuccessProc();																			// �������ӳɹ��ص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);							// Socket�Ͽ�
	void ReConnect(DLG_ID id, TNotifyUI& msg);																// ����

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
	CRichEditUI*				m_pEdtBankPwCtl;		// ��������
	DataManager*				m_pDataManager;			// ���ݹ�����
	LoginServer*				m_pLoginServer;			// ��¼�������ӿڶ���
	LockOperateStatus			m_iCurOperateStatus;	// ��ǰ����״̬
};

// ------------------------------------------------------------------------------------------------------
// ��������
class LockTableFrame : public WindowImplBase
{
public:
	enum LockTableMsgBoxID
	{
		MSG_BOX_ID_NONE = 0,		// ��
		MSG_BOX_ID_DEFAULT,			// Ĭ��
		MSG_BOX_ID_PASSWORD			// ��������
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