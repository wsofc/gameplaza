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

	bool			bMust;		// 是否必填选项
	bool			bCheckMax;	// 是否检测最大长度
	bool			bCheckMin;	// 是否检测最小长度
	bool			bComplete;	// 填写完毕
	DWORD			dwType;		// 类型
	DWORD			dwMinLen;	// 字符最小长度
	DWORD			dwMaxLen;	// 字符最大长度
	CDuiString		pzName;		// 主编辑框控件名
	CDuiString		pzError;	// 错误提示语
	CRichEditUI*	pEdt;		// 编辑框
	CTextUI*		pTxt;		// 文本描述
	CTextUI*		pTxtMust;	// 必选文本描述
	CControlUI*		pTip;		// 状态图
};

class RegistFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	RegistFrame();
	~RegistFrame();
	BOOL CreateFrame();

	enum OperateStatus
	{
		OS_NONE						// 无状态
		, OS_ACCOUNT				// 核查账号
		, OS_NICKNAME				// 核查昵称
		, OS_GETVCODE				// 获取验证码
		, OS_REGIST					// 注册
		, OS_EXTRA					// 额外预留项
	};

	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0,		// 无
		MSG_BOX_ID_DEFAULT,			// 默认
		MSG_BOX_ID_REGIST_OK,		// 注册成功
		MSG_BOX_ID_VCODE,			// 注册验证码
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

	// 验证码定时器
	void SetVertcodeTimer();
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);

	// 清除相关输入内容
	void ClearCtlContent();

	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);
	
public:
	bool OnConnectSuccessProc();															// 网络连接回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// 网络断开通知
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// 重连
	void RegistReq();																		// 注册请求
	
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 短信
	bool OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// 登录消息
	
protected:
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("RegistFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	
private:
	DWORD							m_text_color_default;		// 默认提示颜色(跟配xml置色一致)
	DWORD							m_text_color_illegal;		// 错误提示颜色(红)
	DWORD							m_text_color_correct;		// 正确提示颜色(绿)

	BYTE							m_cbGender;					// 性别
	DWORD							m_dlgWaitID;				// 等待对话框 I D
	LONG							m_llVertcodeSec;			// 验证码计时秒数
	CRichEditUI*					m_pEdtErr;					// 未完整的输入框
	CButtonUI*						m_pBtnGetCode;				// 获取验证码按钮
	CButtonUI*						m_pBtnRegist;				// 注册按钮
	std::vector<tRegist_Ctl*>		m_vRegistCtl;				// 控件数组
	tagLS_RegisterAccount			m_tRegistAccount;			// 注册数据结构

	OperateStatus					m_iCurOperateStatus;		// 当前操作状态
	LoginServer*					m_pLoginServer;				// 登录服务器接口对象
};
#endif //__REGISTFRAME_H__