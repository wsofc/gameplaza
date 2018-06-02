#ifndef __BANK_H__
#define __BANK_H__

#include "ipc/loginServer.h"

class BankFrame : public WindowImplBase, public CWkeWebkitClientCallback, public LoginSeverDelegate
{
public:
	static BankFrame*  GetInstance();
	static bool IsInstanceExist();

	~BankFrame();
	BOOL CreateFrame(CPaintManagerUI* pParentManager);

	enum BanckOperateStatus
	{
		BANCK_OS_NONE					// ��״̬
		, BANCK_OS_LOGIN				// ��¼
		, BANCK_OS_SAVE					// ����
		, BANCK_OS_TAKE					// ȡ��
		, BANCK_OS_MODIFY_PW			// �޸�����
		, BANCK_OS_EXCHANGE				// �һ�����ֵ
		, BANCK_OS_REFRESH				// �һ�����ֵ
		, BANCK_OS_EXTRA				// ����Ԥ����
	};

	enum BankMsgBoxID
	{
		MSG_BOX_ID_NONE = 0				// ��
		, MSG_BOX_ID_SAVE				// ��
		, MSG_BOX_ID_TAKE				// ȡ
		, MSG_BOX_ID_TAKE_PW			// ȡ����
		, MSG_BOX_ID_SRC_PW				// ԭ����
		, MSG_BOX_ID_NEW_PW				// ������
		, MSG_BOX_ID_CFM_PW				// ȷ������
		, MSG_BOX_ID_CHARM_COUNT		// ��������ֵ
		, MSG_BOX_ID_CHARM_PW			// �һ���������
	};

public:

	void InitData(TNotifyUI& msg);

	void InitBankPanel();																					// ��ʼ���������
	void UpdateBankPanel();																					// �����������

	void AddTempGold(CRichEditUI* pEdt, LONGLONG llNums, bool bGold, bool bSupp = false);					// �����ʱ��ʾ���(bSupp - �Ƿ����)
	void CheckLogin();																						// ��¼
	void CheckSave();																						// ����
	void CheckTake();																						// ȡ��
	void CheckModifyPW();																					// �޸�����
	void CheckExchange();																					// �һ�����
	void CheckRefresh();																					// ˢ��
	void ChangeTipsImage(CControlUI* pControl, DWORD dwStatus);												// �ı�tips״̬ͼ
	void TickingMaxCharms(bool bTicked);																	// ��ѡÿ�ζһ��������ֵ(Ĭ��100)
	void SaveBankSetting(int iType, bool bSaveToLocalFile = false);											// ������������(true-����Ϊ�����ļ�, false-���浽�ڴ���)
	void ReadBankSetting(bool bReadFromLocalFile = false);													// ��ȡ��������(true-�ӱ����ļ���ȡ, false-���ڴ��ж�ȡ)

	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

public:
	void OnWkeTitleChange(const CDuiString& strTitle);
	void OnWkeUrlChange(const CDuiString& strUrl);

public:
	bool OnConnectSuccessProc();																			// �������ӻص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);							// Socket�Ͽ�
	void ReConnect(DLG_ID id, TNotifyUI& msg);																// ����
	bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);							// ������Ϣ
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);						// ������Ϣ

protected:

	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("BankFrame"); };

	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void Notify(TNotifyUI& msg);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	static BankFrame* g_instance;
	BankFrame();

	CPaintManagerUI* m_pParentManager;

private:

	UserData*			m_pSelfData;						// ��ǰ�������
	DWORD				m_dwCurPageIndex;					// ��ǰҳ����
	TCHAR				m_pzBuffer[MAX_PATH];				// �ַ���������

	CTabLayoutUI*		m_pTabLaout;						// �����������

	//��1ҳ(��ȡ��)
	CRichEditUI*		m_pSwitch1_EdtYXB;					// ��Ϸ��
	CRichEditUI*		m_pSwitch1_EdtSaveNum;				// ������
	
	CRichEditUI*		m_pSwitch1_EdtDeposit;				// ���
	CRichEditUI*		m_pSwitch1_EdtTakeNum;				// ȡ�����
	CRichEditUI*		m_pSwitch1_EdtTakePassword;			// ȡ������
	COptionUI*			m_pSwitch1_OptMarkPw;				// ��ס����
	
	//��2ҳ(�޸�����)
	CRichEditUI*		m_pSwitch2_EdtSrcPw;				// ԭ����
	CControlUI*			m_pSwitch2_CtlSrcPw;

	CRichEditUI*		m_pSwitch2_EdtNewPw;				// ������
	CControlUI*			m_pSwitch2_CtlNewPw;

	CRichEditUI*		m_pSwitch2_EdtCfmPw;				// ȷ������
	CControlUI*			m_pSwitch2_CtlCfmPw;

	//��3ҳ(�����һ�)
	LONG				m_lMaxCharmOnce;					// ����ÿ�ζһ�����
	CRichEditUI*		m_pSwitch3_EdtCharms;				// ����ֵ
	CRichEditUI*		m_pSwitch3_EdtDeposit;				// ���д��
	CRichEditUI*		m_pSwitch3_EdtExCount;				// �һ�����
	COptionUI*			m_pSwitch3_Opt;						// �һ���ѡ��
	CRichEditUI*		m_pSwitch3_EdtTakePassword;			// ȡ������
	COptionUI*			m_pSwitch3_OptMarkPw;				// ��ס����

	//��4ҳ(������ϸ)
	CWkeWebkitUI*		m_pSwitch4_WebCharmsDetail;			// ������ϸ

	//��5ҳ(������ϸ)
	CWkeWebkitUI*		m_pSwitch5_WebScoreRebate;			// ���ַ���

	std::vector<bool>	m_vPwComplete;

	LoginServer*		m_pLoginServer;						// ��¼�������ӿڶ���
	BanckOperateStatus	m_iCurOperateStatus;				// ��ǰ����״̬
};

#endif //__BANK_H__