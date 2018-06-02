#ifndef __REGISTFRAME_H__
#define __REGISTFRAME_H__

#include "ipc/loginServer.h"

struct tRegist_Ctl
{
	tRegist_Ctl()
	{
		memset(this, 0, sizeof(*this));
	}

	bool CheckPointer()
	{
		return pEdt && pTxt && pTip;
	}

	bool			bMust;		// �Ƿ����ѡ��
	bool			bCheckMax;	// �Ƿ�����󳤶�
	bool			bCheckMin;	// �Ƿ�����С����
	bool			bComplete;	// ��д���
	DWORD			dwType;		// ����
	DWORD			dwMinLen;	// �ַ���С����
	DWORD			dwMaxLen;	// �ַ���󳤶�
	CDuiString		pzName;		// ���༭��ؼ���
	CDuiString		pzError;	// ������ʾ��
	CRichEditUI*	pEdt;		// �༭��
	CTextUI*		pTxt;		// �ı�����
	CTextUI*		pTxtMust;	// ��ѡ�ı�����
	CControlUI*		pTip;		// ״̬ͼ
};

class RegistFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	RegistFrame();
	~RegistFrame();
	BOOL CreateFrame();

	enum OperateStatus
	{
		OS_NONE						// ��״̬
		, OS_ACCOUNT				// �˲��˺�
		, OS_NICKNAME				// �˲��ǳ�
		, OS_GETVCODE				// ��ȡ��֤��
		, OS_REGIST					// ע��
		, OS_EXTRA					// ����Ԥ����
	};

	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0,		// ��
		MSG_BOX_ID_DEFAULT,			// Ĭ��
		MSG_BOX_ID_REGIST_OK,		// ע��ɹ�
		MSG_BOX_ID_VCODE,			// ע����֤��
	};

public:
	void InitWindow();
	void InitData(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void ChangeTipsImage(CControlUI* pControl, DWORD dwStatus);
	void ChangeTipsText(CTextUI* pText, LPCTSTR pzWord, DWORD dwTextColor);
	void SetTextMustVisible(CTextUI* pTextMust, bool bVisible);
	void SwitchToLoginFrame(tagLS_RegisterAccount* pRegistInfo = nullptr);
	void GetServerSMS();
	void CheckAccount(BYTE cbType);
	bool CheckRegist();
	tRegist_Ctl* FindRegistCtl(LPCTSTR pzName);
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

	// ��֤�붨ʱ��
	void SetVertcodeTimer();
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);

	// ��������������
	void ClearCtlContent();

	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);
	
public:
	bool OnConnectSuccessProc();															// �������ӻص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// ����Ͽ�֪ͨ
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// ����
	void RegistReq();																		// ע������
	
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// ����
	bool OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// ��¼��Ϣ
	
protected:
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("RegistFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	
private:
	DWORD							m_text_color_default;		// Ĭ����ʾ��ɫ(����xml��ɫһ��)
	DWORD							m_text_color_illegal;		// ������ʾ��ɫ(��)
	DWORD							m_text_color_correct;		// ��ȷ��ʾ��ɫ(��)

	BYTE							m_cbGender;					// �Ա�
	DWORD							m_dlgWaitID;				// �ȴ��Ի��� I D
	LONG							m_llVertcodeSec;			// ��֤���ʱ����
	CRichEditUI*					m_pEdtErr;					// δ�����������
	CButtonUI*						m_pBtnGetCode;				// ��ȡ��֤�밴ť
	CButtonUI*						m_pBtnRegist;				// ע�ᰴť
	std::vector<tRegist_Ctl*>		m_vRegistCtl;				// �ؼ�����
	tagLS_RegisterAccount			m_tRegistAccount;			// ע�����ݽṹ

	OperateStatus					m_iCurOperateStatus;		// ��ǰ����״̬
	LoginServer*					m_pLoginServer;				// ��¼�������ӿڶ���
};
#endif //__REGISTFRAME_H__