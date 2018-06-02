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

	TCHAR				szAccount[NAME_LEN];			// 登录账号
	TCHAR				szPwd[PWD_LEN];					// 登录密码
	bool				bTick;							// 是否记住密码
	int					iSkinIndex;						// 皮肤索引
};

struct tagGlobalUserData
{
	tagGlobalUserData()
	{
		memset(this, 0, sizeof(*this));
	}

	WORD				wFaceID;						// 头像索引
	BYTE				cbGender;						// 用户性别
	BYTE				cbMember;						// 会员等级
	BYTE				cbMasterOrder;					// 管理员等级
	DWORD				dwUserID;						// 用户 I D
	DWORD				dwGameID;						// 游戏 I D
	DWORD				dwExperience;					// 用户经验
	TCHAR				szAccount[NAME_LEN];			// 帐号
	TCHAR				szNickname[NAME_LEN];			// 昵称
	TCHAR				szPwd[PWD_LEN];					// 登录密码
	TCHAR				szGroupName[GROUP_LEN];			// 社团信息
	TCHAR				szUnderWrite[UNDER_WRITE_LEN];	// 个性签名

	//扩展信息
	DWORD				dwCustomFaceVer;				// 头像版本
	BYTE				cbMoorMachine;					// 锁定机器

	LONGLONG            lCharm;							// 魅力
	LONGLONG            lGameGlod;						// 金币
	LONGLONG            lBankGold;						// 银行金币
};

class LoginFrame : public WindowImplBase, public CWkeWebkitClientCallback, public LoginSeverDelegate
{
public:
	~LoginFrame();
	static LoginFrame*	GetInstance();
	BOOL				CreateFrame();

	enum OperateStatus
	{
		OS_NONE				// 无状态
		, OS_UPDATE			// 检查更新
		, OS_LOGIN			// 登录
		, OS_EXTRA			// 额外预留项
	};

	enum LoginMsgBoxID
	{
		LOGIN_MSG_BOX_ID_NONE = 0
		, LOGIN_MSG_BOX_ID_CLIENT_DAMAGED				// 客户端已损坏
		, LOGIN_MSG_BOX_ID_PLAZA_STATUS_LOW				// 客户端版本太低
		, LOGIN_MSG_BOX_ID_PLAZA_UPDATE_FAILED			// 客户端更新失败
		, LOGIN_MSG_BOX_ID_NET_COM_CREATFAILED			// 网络组件创建失败
		, LOGIN_MSG_BOX_ID_NET_CONNECT_FAILED			// 网络连接失败
		, LOGIN_MSG_BOX_ID_LOGIN_ACCOUNT				// 登录账号
		, LOGIN_MSG_BOX_ID_LOGIN_PASSWORD				// 登录密码
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

	void CheckUpdate();																		// 检查版本更新

	void SetLoginSMS(LPCTSTR pzSMSVcode){ m_pzSMSVcode = pzSMSVcode; }						// 设置登录验证码
	void SetWaittingDlgManager(CPaintManagerUI* pManager){ m_pWaitDlgManager = pManager; }	// 设置等待对话框窗口管理器

	void LoginCheck();																		// 登录检测
	void LoginConnect();																	// 登录连接
	void ShowWaittingText(bool bShow, CDuiString pzText = _T(""));							// 显示连接等待提示语
	void Exit();																			// 退出

public:
	void OnWkeTitleChange(const CDuiString& strTitle);
	void OnWkeUrlChange(const CDuiString& strUrl);

public:
	bool OnConnectSuccessProc();															// 连接成功回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);			// 网络断开通知
	void ReConnect(DLG_ID id, TNotifyUI& msg);												// 重连
	void OnWaittingTimeOut(DLG_ID id, TNotifyUI& msg);										// 登录连接等待超时

	bool OnNetwork_Logon(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// 登录消息
	bool OnNetwork_SMS(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 短信
	bool OnNetwork_ServerList(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);	// 列表消息
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// 礼物消息
	bool OnNetwork_System(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);		// 系统消息
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);			// 用户消息

protected:

	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	LPCTSTR GetWindowClassName(void) const{ return _T("LoginFrame"); }
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	void _ReadStyleFont();										// 读取字体风格配置支持
	void _ReadAccountFile();									// 读取账号数据
	void _SaveAccountFile();									// 保存账号密码至本地
	void _ClearAccountFile();									// 清除账号数据(与该账号相关的所有数据)
	void _MatchAccount();										// 自动匹配账号密码
	void _ClearAccount();										// 清除本地账号数组
	void _ReadWebConfig();										// 读取本地网页配置
	void _ReadUnrealConfig();									// 读取本地Unreal配置

	void _CreatUserDir(bool bUser = true);						// 创建用户缓存信息目录
	void _ExcStep(DWORD iStep);									// 切换加载步骤
	void _OnTimer(UINT_PTR idEvent, DWORD dwTime);				// 定时器(这里主要用于Loading收集)
	void _InitIPC();											// 初始化IPC实例
	void _InitPrepare();										// loading准备
	void _InitUserData(void* pData, WORD wDataSize);			// 用户基础信息
	void _InitGameConfigList(void* pData, WORD wDataSize);		// 游戏列表配置
	void _InitGameTypeList(void* pData, WORD wDataSize);		// 游戏类型列表
	void _InitGameKindList(void* pData, WORD wDataSize);		// 游戏种类列表
	void _initGameStationList(void* pData, WORD wDataSize);		// 游戏站点列表
	void _InitGameRoomList(void* pData, WORD wDataSize);		// 游戏房间列表
	void _InitGameListComplete();								// 游戏列表配置完成
	void _InitPropsList(void* pData, WORD wDataSize);			// 道具列表配置
	void _InitLoganComplete();									// 登录完成

private:

	bool						m_bAllowConnect;				// 允许跨版本登录(是否为非强制更新)
	
	TCHAR						m_pzBuffer[MAX_PATH];			// 字符串缓冲区
	JsonFile*					m_pConfigFile;					// 配置文件
	Json::Value					m_vConfigRoot;					// 
	std::vector<tAccountInfo*>	m_vAccountArr;					// 账号候选数组

	CRichEditUI*				m_pEdtAccount;					// 账号
	CDuiString					m_pzAccount;

	CRichEditUI*				m_pEdtPassword;					// 密码
	CDuiString					m_pzPassword;

	DWORD						m_dwWatingEventID;				// 等待框ID
	CLabelUI*					m_pLabWaitting;					// 等待提示语
	CLabelUI*					m_pLabVersion;					// 版本号

	CDuiString					m_pzSMSVcode;					// 短信验证吗(异常登录时赋值)
	CPaintManagerUI*			m_pWaitDlgManager;				// 等待对话框所属的窗口管理器(用于显示该对话框挂于哪个窗口上,方便禁用底层事件)

	bool						m_bRefreshAcconut;				// 是否刷新缓存的账号
	CDuiString					m_pzTmpAccount;					// 临时缓存的账号
	CDuiString					m_pzTmpPassword;				// 临时缓存的密码

	COptionUI*					m_pOptCheckPW;					// 记住密码单选框
	CComboUI*					m_pComboAccount;				// 账号候选下拉框

	CWkeWebkitUI*				m_pWkeCtl;						// wke浏览器控件
	CButtonUI*					m_pBtnLogin;					// 登录按钮

	OperateStatus				m_iCurOperateStatus;			// 当前操作状态
	LoadingFrame*				m_pLoadingFrame;				// 加载界面指针
	LoginServer*				m_pLoginServer;					// 登录服务器接口对象
	
	tagLS_Version				m_tCurServerVersion;			// 大厅版本号(当前服务端为准)
	
	LoginFrame();												// 登录模块单实例
	static LoginFrame*			g_instance;						//
};

#endif //__LOGINFRAME_H__