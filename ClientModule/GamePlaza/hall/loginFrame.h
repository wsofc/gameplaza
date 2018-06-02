#ifndef __LOGINFRAME_H__
#define __LOGINFRAME_H__

#include "loadingFrame.h"
#include "ipc/loginServer.h"

struct tAccountInfo
{
	tAccountInfo()
	{
		memset(this, 0, sizeof(*this));
	}

	TCHAR				szAccount[NAME_LEN];			// ��¼�˺�
	TCHAR				szPwd[PWD_LEN];					// ��¼����
	bool				bTick;							// �Ƿ��ס����
	int					iSkinIndex;						// Ƥ������
};

struct tagGlobalUserData
{
	tagGlobalUserData()
	{
		memset(this, 0, sizeof(*this));
	}

	WORD				wFaceID;						// ͷ������
	BYTE				cbGender;						// �û��Ա�
	BYTE				cbMember;						// ��Ա�ȼ�
	BYTE				cbMasterOrder;					// ����Ա�ȼ�
	DWORD				dwUserID;						// �û� I D
	DWORD				dwGameID;						// ��Ϸ I D
	DWORD				dwExperience;					// �û�����
	TCHAR				szAccount[NAME_LEN];			// �ʺ�
	TCHAR				szNickname[NAME_LEN];			// �ǳ�
	TCHAR				szPwd[PWD_LEN];					// ��¼����
	TCHAR				szGroupName[GROUP_LEN];			// ������Ϣ
	TCHAR				szUnderWrite[UNDER_WRITE_LEN];	// ����ǩ��

	//��չ��Ϣ
	DWORD				dwCustomFaceVer;				// ͷ��汾
	BYTE				cbMoorMachine;					// ��������

	LONGLONG            lCharm;							// ����
	LONGLONG            lGameGlod;						// ���
	LONGLONG            lBankGold;						// ���н��
};

class LoginFrame : public WindowImplBase, public CWkeWebkitClientCallback, public LoginSeverDelegate
{
public:
	~LoginFrame();
	static LoginFrame*	GetInstance();
	BOOL				CreateFrame();

	enum OperateStatus
	{
		OS_NONE				// ��״̬
		, OS_UPDATE			// ������
		, OS_LOGIN			// ��¼
		, OS_EXTRA			// ����Ԥ����
	};

	enum LoginMsgBoxID
	{
		LOGIN_MSG_BOX_ID_NONE = 0
		, LOGIN_MSG_BOX_ID_CLIENT_DAMAGED				// �ͻ�������
		, LOGIN_MSG_BOX_ID_PLAZA_STATUS_LOW				// �ͻ��˰汾̫��
		, LOGIN_MSG_BOX_ID_PLAZA_UPDATE_FAILED			// �ͻ��˸���ʧ��
		, LOGIN_MSG_BOX_ID_NET_COM_CREATFAILED			// �����������ʧ��
		, LOGIN_MSG_BOX_ID_NET_CONNECT_FAILED			// ��������ʧ��
		, LOGIN_MSG_BOX_ID_LOGIN_ACCOUNT				// ��¼�˺�
		, LOGIN_MSG_BOX_ID_LOGIN_PASSWORD				// ��¼����
	};

public:
	CControlUI*			CreateControl(LPCTSTR pstrClassName);
	void				InitWindow();
	void				Notify(TNotifyUI& msg);
	LRESULT				OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void				OnFinalMessage(HWND hWnd);
	UINT				GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT				HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	void InitData(TNotifyUI& msg);
	void RefreshLoginData(LPCTSTR pzAccount = _T(""), LPCTSTR pzPassword = _T(""));
	
	void MsgBoxProc(DLG_ID id, TNotifyUI& msg);
	void StartUpdate();

	void CheckUpdate();																		// ���汾����

	void SetLoginSMS(LPCTSTR pzSMSVcode){ m_pzSMSVcode = pzSMSVcode; }						// ���õ�¼��֤��
	void SetWaittingDlgManager(CPaintManagerUI* pManager){ m_pWaitDlgManager = pManager; }	// ���õȴ��Ի��򴰿ڹ�����

	void LoginCheck();																		// ��¼���
	void LoginConnect();																	// ��¼����
	void ShowWaittingText(bool bShow, CDuiString pzText = _T(""));							// ��ʾ���ӵȴ���ʾ��
	void Exit();																			// �˳�

public:
	void OnWkeTitleChange(const CDuiString& strTitle);
	void OnWkeUrlChange(const CDuiString& strUrl);

public:
	bool OnConnectSuccessProc();															// ���ӳɹ��ص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// ����Ͽ�֪ͨ
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// ����
	void OnWaittingTimeOut(DLG_ID id, TNotifyUI& msg);										// ��¼���ӵȴ���ʱ

	bool OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// ��¼��Ϣ
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// ����
	bool OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);	// �б���Ϣ
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// ������Ϣ
	bool OnNetwork_System(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// ϵͳ��Ϣ
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// �û���Ϣ

protected:

	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	LPCTSTR GetWindowClassName(void) const{ return _T("LoginFrame"); }
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	void _ReadStyleFont();										// ��ȡ����������֧��
	void _ReadAccountFile();									// ��ȡ�˺�����
	void _SaveAccountFile();									// �����˺�����������
	void _ClearAccountFile();									// ����˺�����(����˺���ص���������)
	void _MatchAccount();										// �Զ�ƥ���˺�����
	void _ClearAccount();										// ��������˺�����
	void _ReadWebConfig();										// ��ȡ������ҳ����
	void _ReadUnrealConfig();									// ��ȡ����Unreal����

	void _CreatUserDir(bool bUser = true);						// �����û�������ϢĿ¼
	void _ExcStep(DWORD iStep);									// �л����ز���
	void _OnTimer(UINT_PTR idEvent, DWORD dwTime);				// ��ʱ��(������Ҫ����Loading�ռ�)
	void _InitIPC();											// ��ʼ��IPCʵ��
	void _InitPrepare();										// loading׼��
	void _InitUserData(void* pData, WORD wDataSize);			// �û�������Ϣ
	void _InitGameConfigList(void* pData, WORD wDataSize);		// ��Ϸ�б�����
	void _InitGameTypeList(void* pData, WORD wDataSize);		// ��Ϸ�����б�
	void _InitGameKindList(void* pData, WORD wDataSize);		// ��Ϸ�����б�
	void _initGameStationList(void* pData, WORD wDataSize);		// ��Ϸվ���б�
	void _InitGameRoomList(void* pData, WORD wDataSize);		// ��Ϸ�����б�
	void _InitGameListComplete();								// ��Ϸ�б��������
	void _InitPropsList(void* pData, WORD wDataSize);			// �����б�����
	void _InitLoganComplete();									// ��¼���

private:

	bool						m_bAllowConnect;				// �����汾��¼(�Ƿ�Ϊ��ǿ�Ƹ���)
	
	TCHAR						m_pzBuffer[MAX_PATH];			// �ַ���������
	JsonFile*					m_pConfigFile;					// �����ļ�
	Json::Value					m_vConfigRoot;					// 
	std::vector<tAccountInfo*>	m_vAccountArr;					// �˺ź�ѡ����

	CRichEditUI*				m_pEdtAccount;					// �˺�
	CDuiString					m_pzAccount;

	CRichEditUI*				m_pEdtPassword;					// ����
	CDuiString					m_pzPassword;

	DWORD						m_dwWatingEventID;				// �ȴ���ID
	CLabelUI*					m_pLabWaitting;					// �ȴ���ʾ��
	CLabelUI*					m_pLabVersion;					// �汾��

	CDuiString					m_pzSMSVcode;					// ������֤��(�쳣��¼ʱ��ֵ)
	CPaintManagerUI*			m_pWaitDlgManager;				// �ȴ��Ի��������Ĵ��ڹ�����(������ʾ�öԻ�������ĸ�������,������õײ��¼�)

	bool						m_bRefreshAcconut;				// �Ƿ�ˢ�»�����˺�
	CDuiString					m_pzTmpAccount;					// ��ʱ������˺�
	CDuiString					m_pzTmpPassword;				// ��ʱ���������

	COptionUI*					m_pOptCheckPW;					// ��ס���뵥ѡ��
	CComboUI*					m_pComboAccount;				// �˺ź�ѡ������

	CWkeWebkitUI*				m_pWkeCtl;						// wke������ؼ�
	CButtonUI*					m_pBtnLogin;					// ��¼��ť

	OperateStatus				m_iCurOperateStatus;			// ��ǰ����״̬
	LoadingFrame*				m_pLoadingFrame;				// ���ؽ���ָ��
	LoginServer*				m_pLoginServer;					// ��¼�������ӿڶ���
	
	tagLS_Version				m_tCurServerVersion;			// �����汾��(��ǰ�����Ϊ׼)
	
	LoginFrame();												// ��¼ģ�鵥ʵ��
	static LoginFrame*			g_instance;						//
};

#endif //__LOGINFRAME_H__