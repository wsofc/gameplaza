#ifndef __INFOFRAME_H__
#define __INFOFRAME_H__

#include "../ipc/loginServer.h"

class InfoFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	static InfoFrame*  GetInstance();
	static bool IsInstanceExist();
	~InfoFrame();

	BOOL CreateFrame(CPaintManagerUI* pParentManager, int infoPanelIndex = 0);
	
	enum MSGBOX_EVENT_ID
	{
		MB_ID_NONE							// ��
		, MB_ID_SWITCH1_NiCK_NAME			// Switch1-�ǳ�
		, MB_ID_SWITCH1_REAL_NAME			// Switch1-��ʵ����
		, MB_ID_SWITCH1_TEL					// Switch1-�绰
		, MB_ID_SWITCH1_IDCARD				// Switch1-���֤
		, MB_ID_SWITCH1_GENDER				// Switch1-�Ա�
		, MB_ID_SWITCH1_EMAIL				// Switch1-����
		, MB_ID_SWITCH1_QQ					// Switch1-QQ����
		, MB_ID_SWITCH1_UNDERWRITE			// Switch1-����˵��
		, MB_ID_SWITCH3_NEW_TEL				// Switch3-���ֻ���
		, MB_ID_SWITCH3_VCODE				// Switch3-��֤��
		, MB_ID_SWITCH3_OLD_PW				// Switch3-������
		, MB_ID_SWITCH3_NEW_PW				// Switch3-������
		, MB_ID_SWITCH3_CFM_PW				// Switch3-ȷ������
		, MB_ID_SWITCH4_PASSWORD			// Switch4-����Я������
		, MB_ID_SWITCH4_LIMIT_SCORE_MIN		// Switch4-������ͻ���
		, MB_ID_SWITCH4_LIMIT_SCORE_MAX		// Switch4-������߷�Χ
		, MB_ID_EXTRA						// ����Ԥ����
	};

	enum InfoOperateStatus
	{
		INFO_OS_NONE					// ��״̬
		, INFO_OS_GET_INFO				// ��ȡ������Ϣ
		, INFO_OS_MODIFY_INFO			// �޸ĸ�������
		, INFO_OS_MODIFY_HEAD_PIC		// �޸�ͷ��
		, INFO_OS_MODIFY_TEL			// �޸��ֻ���
		, INFO_OS_MODIFY_PW				// �޸�����
		, INFO_OS_GET_VCODE				// ��ȡ��֤��
		, INFO_OS_EXTRA					// ����Ԥ����
	};

public:
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);
	
	void InitData(TNotifyUI& msg);															// ��ʼ������
	void ShowLeftWords();																	// ʣ��������ʾ
	void LayoutHeadPanel();																	// ����ϵͳͷ��
	void ShowPanelByIndex(int infoPanelIndex = 0);											// ��ʾָ�����������
	bool CheckCustomPicPanel();																// ����Ƿ����Զ���ͷ����
	void ShowWaittingText(bool bShow, CDuiString pzText = _T(""));							// ��ʾ���ӵȴ���ʾ��

	void ShowToastText(CDuiString pzText = _T("")											// ��ʾ��ʾ��
		, DLG_ID id = DLG_ID::DLG_ID_NONE			
		, DialogCallBack pCallBackFunc = nullptr
		, bool bFadeOut = false);											

	void UpdateInfo(tagLS_UpdateUserInfo* pUserInfo);										// ���¸�����Ϣ����
	void ReqGetInfo();																		// ��ȡ������Ϣ

	void GetServerSMS();																	// ��ȡ������֤��
	void SetVertcodeTimer();																// ��֤�붨ʱ��
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);										

	void ModifyUserInfo();																	// �޸ĸ�������
	void ModifyHeadPic();																	// �޸�ͷ��ѡ��
	void ModifyTelphone();																	// �޸��ֻ���
	void ModifyPassword();																	// �޸ĵ�¼����
	void FrozenModify();																	// ���������޸���
	void ModifyGameSetting();																// �޸���Ϸ������

	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);										// Msg��Ϣ�ص�

public:
	bool OnConnectSuccessProc();															// �������ӻص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// Socket�Ͽ�
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// ����
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// �û���Ϣ
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// ����

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("InfoFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

	static InfoFrame*		g_instance;							// ����
	InfoFrame();

	CPaintManagerUI*		m_pParentManager;					// �����ڻ�ͼ������

private:

	DialogFrame*			m_pWaitDlg;							// �ȴ��Ի���
	LONG					m_llVertcodeSec;					// ��֤���ʱ����
	DWORD					m_dwSwitchIndex;					// ����±�����
	TCHAR					m_pzBuffer[MAX_PATH];				// �ַ�����������
	UserData*				m_pSelfData;						// ��ǰ���
	tagLS_UpdateUserInfo	m_tTempUnserInfo;					// ����ĸ�����������
	LoginServer*			m_pLoginServer;						// ��¼�������ӿڶ���
	InfoOperateStatus		m_iCurOperateStatus;				// ��ǰ����״̬

	// ����
	CTabLayoutUI*			m_pInfoSwitch;						// �л����ָ��ؼ�

	// shitch_1
	CRichEditUI*			m_pSwitch1_EdtAccount;				// �˺�
	CRichEditUI*			m_pSwitch1_EdtRealName;				// ��ʵ����
	CRichEditUI*			m_pSwitch1_EdtTelphone;				// �绰
	CRichEditUI*			m_pSwitch1_EdtIDCard;				// ���֤
		
	CRichEditUI*			m_pSwitch1_EdtNickName;				// �ǳ�
	CRichEditUI*			m_pSwitch1_EdtGender;				// �Ա�
	CRichEditUI*			m_pSwitch1_EdtEmail;				// ����
	CRichEditUI*			m_pSwitch1_EdtQQNumber;				// QQ��

	CComboUI*				m_pSwitch1_ComboGender;				// �Ա�������
	
	CRichEditUI*			m_pSwitch1_EdtUnderWrite;			// ����˵��
	CLabelUI*				m_pSwitch1_LabRemainWords;			// "����˵��"ʣ������lab

	// shitch_3
	CLabelUI*				m_pSwitch3_LabOldTel;				// ���ֻ���
	CRichEditUI*			m_pSwitch3_EdtNewTel;				// ���ֻ�����
	CRichEditUI*			m_pSwitch3_EdtVCode;				// ��֤��

	CDuiString				m_pzSwitch3_BtnGetVCodeText;		// ��ť����
	CButtonUI*				m_pSwitch3_BtnGetVCode;				// ��ȡ��֤��
	CButtonUI*				m_pSwitch3_BtnBind;					// ��
	
	CRichEditUI*			m_pSwitch3_EdtOldPw;				// ԭ����
	CRichEditUI*			m_pSwitch3_EdtNewPw;				// ������
	CRichEditUI*			m_pSwitch3_EdtCfmPw;				// ȷ������
	CButtonUI*				m_pSwitch3_BtnEnsure;				// ȷ���޸�

	// shitch_4
	COptionUI*				m_pSwitch4_OptWin;					// ���ʤ��
	CRichEditUI*			m_pSwitch4_EdtWin;

	COptionUI*				m_pSwitch4_OptFlee;					// ���������
	CRichEditUI*			m_pSwitch4_EdtFlee;

	COptionUI*				m_pSwitch4_OptPassword;				// ����Я������
	CRichEditUI*			m_pSwitch4_EdtPassword;

	COptionUI*				m_pSwitch4_OptScore;				// ���ƻ��ַ�Χ
	CRichEditUI*			m_pSwitch4_EdtScoreMin;
	CRichEditUI*			m_pSwitch4_EdtScoreMax;

	COptionUI*				m_pSwitch4_OptSameIP;				// ������ͬIP
};

#endif //__INFOFRAME_H__