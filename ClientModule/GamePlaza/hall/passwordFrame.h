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
		, MSG_BOX_ID_NEW_PW			// 新密码
		, MSG_BOX_ID_CFM_PW			// 确认密码
		, MSG_BOX_ID_SUCCESS		// 修改成功
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

	void TurnToLoginUI();

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
	LPCTSTR GetWindowClassName(void) const{ return _T("PasswordFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	CRichEditUI*				m_pEdtAccount;			// 账号
	CControlUI*					m_pCtlAccount;

	CRichEditUI*				m_pEdtTelphone;			// 手机号
	CControlUI*					m_pCtlTelphone;

	CRichEditUI*				m_pEdtVertCode;			// 验证码
	CControlUI*					m_pCtlVertCode;

	CRichEditUI*				m_pEdtPassword;			// 新密码
	CControlUI*					m_pCtlPassword;

	CRichEditUI*				m_pEdtPasswordEx;		// 确认密码
	CControlUI*					m_pCtlPasswordEx;
	
	CButtonUI*					m_pBtnGetCode;			// 获取验证码
	std::vector<stuMsgBoxFlag>	m_vCompleteFlag;		// 控件数组
	CPaintManagerUI*			m_pParentManager;		// 父窗口的绘制管理器

	LONG						m_llVertcodeSec;		// 验证码计时秒数
		
	OperateStatus				m_iCurOperateStatus;	// 当前操作状态
	LoginServer*				m_pLoginServer;			// 登录服务器接口对象
};
#endif //__PASSWORDFRAME_H__