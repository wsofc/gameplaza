#ifndef __PASSWORDFRAME_H__
#define __PASSWORDFRAME_H__

#include "ipc/loginServer.h"

class PasswordFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	PasswordFrame();
	~PasswordFrame();
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
		, MSG_BOX_ID_NEW_PW			// ������
		, MSG_BOX_ID_CFM_PW			// ȷ������
		, MSG_BOX_ID_SUCCESS		// �޸ĳɹ�
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

	void TurnToLoginUI();

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
	LPCTSTR GetWindowClassName(void) const{ return _T("PasswordFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	CRichEditUI*				m_pEdtAccount;			// �˺�
	CControlUI*					m_pCtlAccount;

	CRichEditUI*				m_pEdtTelphone;			// �ֻ���
	CControlUI*					m_pCtlTelphone;

	CRichEditUI*				m_pEdtVertCode;			// ��֤��
	CControlUI*					m_pCtlVertCode;

	CRichEditUI*				m_pEdtPassword;			// ������
	CControlUI*					m_pCtlPassword;

	CRichEditUI*				m_pEdtPasswordEx;		// ȷ������
	CControlUI*					m_pCtlPasswordEx;
	
	CButtonUI*					m_pBtnGetCode;			// ��ȡ��֤��
	std::vector<stuMsgBoxFlag>	m_vCompleteFlag;		// �ؼ�����
	CPaintManagerUI*			m_pParentManager;		// �����ڵĻ��ƹ�����

	LONG						m_llVertcodeSec;		// ��֤���ʱ����
		
	OperateStatus				m_iCurOperateStatus;	// ��ǰ����״̬
	LoginServer*				m_pLoginServer;			// ��¼�������ӿڶ���
};
#endif //__PASSWORDFRAME_H__