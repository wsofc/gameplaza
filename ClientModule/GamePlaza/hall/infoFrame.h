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
		MB_ID_NONE							// 无
		, MB_ID_SWITCH1_NiCK_NAME			// Switch1-昵称
		, MB_ID_SWITCH1_REAL_NAME			// Switch1-真实姓名
		, MB_ID_SWITCH1_TEL					// Switch1-电话
		, MB_ID_SWITCH1_IDCARD				// Switch1-身份证
		, MB_ID_SWITCH1_GENDER				// Switch1-性别
		, MB_ID_SWITCH1_EMAIL				// Switch1-邮箱
		, MB_ID_SWITCH1_QQ					// Switch1-QQ号码
		, MB_ID_SWITCH1_UNDERWRITE			// Switch1-个人说明
		, MB_ID_SWITCH3_NEW_TEL				// Switch3-新手机号
		, MB_ID_SWITCH3_VCODE				// Switch3-验证码
		, MB_ID_SWITCH3_OLD_PW				// Switch3-旧密码
		, MB_ID_SWITCH3_NEW_PW				// Switch3-新密码
		, MB_ID_SWITCH3_CFM_PW				// Switch3-确认密码
		, MB_ID_SWITCH4_PASSWORD			// Switch4-桌子携带密码
		, MB_ID_SWITCH4_LIMIT_SCORE_MIN		// Switch4-限制最低积分
		, MB_ID_SWITCH4_LIMIT_SCORE_MAX		// Switch4-限制最高范围
		, MB_ID_EXTRA						// 额外预留项
	};

	enum InfoOperateStatus
	{
		INFO_OS_NONE					// 无状态
		, INFO_OS_GET_INFO				// 获取个人信息
		, INFO_OS_MODIFY_INFO			// 修改个人资料
		, INFO_OS_MODIFY_HEAD_PIC		// 修改头像
		, INFO_OS_MODIFY_TEL			// 修改手机号
		, INFO_OS_MODIFY_PW				// 修改密码
		, INFO_OS_GET_VCODE				// 获取验证码
		, INFO_OS_EXTRA					// 额外预留项
	};

public:
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);
	
	void InitData(TNotifyUI& msg);															// 初始化数据
	void ShowLeftWords();																	// 剩余字数显示
	void LayoutHeadPanel();																	// 布局系统头像
	void ShowPanelByIndex(int infoPanelIndex = 0);											// 显示指定索引的面板
	bool CheckCustomPicPanel();																// 检测是否开启自定义头像功能
	void ShowWaittingText(bool bShow, CDuiString pzText = _T(""));							// 显示连接等待提示语

	void ShowToastText(CDuiString pzText = _T("")											// 显示提示语
		, DLG_ID id = DLG_ID::DLG_ID_NONE			
		, DialogCallBack pCallBackFunc = nullptr
		, bool bFadeOut = false);											

	void UpdateInfo(tagLS_UpdateUserInfo* pUserInfo);										// 更新个人信息属性
	void ReqGetInfo();																		// 获取个人信息

	void GetServerSMS();																	// 获取短信验证码
	void SetVertcodeTimer();																// 验证码定时器
	void OnVertcodeTimer(UINT_PTR idEvent, DWORD dwTime);										

	void ModifyUserInfo();																	// 修改个人资料
	void ModifyHeadPic();																	// 修改头像选择
	void ModifyTelphone();																	// 修改手机号
	void ModifyPassword();																	// 修改登录密码
	void FrozenModify();																	// 禁用若干修改项
	void ModifyGameSetting();																// 修改游戏设置项

	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);										// Msg消息回调

public:
	bool OnConnectSuccessProc();															// 网络连接回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// Socket断开
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// 重连
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 用户消息
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 短信

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("InfoFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

	static InfoFrame*		g_instance;							// 单例
	InfoFrame();

	CPaintManagerUI*		m_pParentManager;					// 父窗口绘图管理器

private:

	DialogFrame*			m_pWaitDlg;							// 等待对话框
	LONG					m_llVertcodeSec;					// 验证码计时秒数
	DWORD					m_dwSwitchIndex;					// 面板下标索引
	TCHAR					m_pzBuffer[MAX_PATH];				// 字符串缓冲数组
	UserData*				m_pSelfData;						// 当前玩家
	tagLS_UpdateUserInfo	m_tTempUnserInfo;					// 缓存的个人中心数据
	LoginServer*			m_pLoginServer;						// 登录服务器接口对象
	InfoOperateStatus		m_iCurOperateStatus;				// 当前操作状态

	// 公用
	CTabLayoutUI*			m_pInfoSwitch;						// 切换布局父控件

	// shitch_1
	CRichEditUI*			m_pSwitch1_EdtAccount;				// 账号
	CRichEditUI*			m_pSwitch1_EdtRealName;				// 真实姓名
	CRichEditUI*			m_pSwitch1_EdtTelphone;				// 电话
	CRichEditUI*			m_pSwitch1_EdtIDCard;				// 身份证
		
	CRichEditUI*			m_pSwitch1_EdtNickName;				// 昵称
	CRichEditUI*			m_pSwitch1_EdtGender;				// 性别
	CRichEditUI*			m_pSwitch1_EdtEmail;				// 邮箱
	CRichEditUI*			m_pSwitch1_EdtQQNumber;				// QQ号

	CComboUI*				m_pSwitch1_ComboGender;				// 性别下拉框
	
	CRichEditUI*			m_pSwitch1_EdtUnderWrite;			// 个人说明
	CLabelUI*				m_pSwitch1_LabRemainWords;			// "个人说明"剩余字数lab

	// shitch_3
	CLabelUI*				m_pSwitch3_LabOldTel;				// 旧手机号
	CRichEditUI*			m_pSwitch3_EdtNewTel;				// 新手机号码
	CRichEditUI*			m_pSwitch3_EdtVCode;				// 验证码

	CDuiString				m_pzSwitch3_BtnGetVCodeText;		// 按钮文字
	CButtonUI*				m_pSwitch3_BtnGetVCode;				// 获取验证码
	CButtonUI*				m_pSwitch3_BtnBind;					// 绑定
	
	CRichEditUI*			m_pSwitch3_EdtOldPw;				// 原密码
	CRichEditUI*			m_pSwitch3_EdtNewPw;				// 新密码
	CRichEditUI*			m_pSwitch3_EdtCfmPw;				// 确认密码
	CButtonUI*				m_pSwitch3_BtnEnsure;				// 确定修改

	// shitch_4
	COptionUI*				m_pSwitch4_OptWin;					// 最低胜率
	CRichEditUI*			m_pSwitch4_EdtWin;

	COptionUI*				m_pSwitch4_OptFlee;					// 最低逃跑率
	CRichEditUI*			m_pSwitch4_EdtFlee;

	COptionUI*				m_pSwitch4_OptPassword;				// 桌子携带密码
	CRichEditUI*			m_pSwitch4_EdtPassword;

	COptionUI*				m_pSwitch4_OptScore;				// 限制积分范围
	CRichEditUI*			m_pSwitch4_EdtScoreMin;
	CRichEditUI*			m_pSwitch4_EdtScoreMax;

	COptionUI*				m_pSwitch4_OptSameIP;				// 限制相同IP
};

#endif //__INFOFRAME_H__