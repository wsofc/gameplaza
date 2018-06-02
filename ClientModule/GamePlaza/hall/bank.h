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
		BANCK_OS_NONE					// 无状态
		, BANCK_OS_LOGIN				// 登录
		, BANCK_OS_SAVE					// 存入
		, BANCK_OS_TAKE					// 取出
		, BANCK_OS_MODIFY_PW			// 修改密码
		, BANCK_OS_EXCHANGE				// 兑换魅力值
		, BANCK_OS_REFRESH				// 兑换魅力值
		, BANCK_OS_EXTRA				// 额外预留项
	};

	enum BankMsgBoxID
	{
		MSG_BOX_ID_NONE = 0				// 无
		, MSG_BOX_ID_SAVE				// 存
		, MSG_BOX_ID_TAKE				// 取
		, MSG_BOX_ID_TAKE_PW			// 取密码
		, MSG_BOX_ID_SRC_PW				// 原密码
		, MSG_BOX_ID_NEW_PW				// 新密码
		, MSG_BOX_ID_CFM_PW				// 确认密码
		, MSG_BOX_ID_CHARM_COUNT		// 输入魅力值
		, MSG_BOX_ID_CHARM_PW			// 兑换魅力密码
	};

public:

	void InitData(TNotifyUI& msg);

	void InitBankPanel();																					// 初始化银行面板
	void UpdateBankPanel();																					// 更新银行面板

	void AddTempGold(CRichEditUI* pEdt, LONGLONG llNums, bool bGold, bool bSupp = false);					// 添加临时显示金额(bSupp - 是否叠加)
	void CheckLogin();																						// 登录
	void CheckSave();																						// 存入
	void CheckTake();																						// 取出
	void CheckModifyPW();																					// 修改密码
	void CheckExchange();																					// 兑换魅力
	void CheckRefresh();																					// 刷新
	void ChangeTipsImage(CControlUI* pControl, DWORD dwStatus);												// 改变tips状态图
	void TickingMaxCharms(bool bTicked);																	// 勾选每次兑换魅力最大值(默认100)
	void SaveBankSetting(int iType, bool bSaveToLocalFile = false);											// 保存银行设置(true-保存为本地文件, false-保存到内存中)
	void ReadBankSetting(bool bReadFromLocalFile = false);													// 读取银行设置(true-从本地文件读取, false-从内存中读取)

	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

public:
	void OnWkeTitleChange(const CDuiString& strTitle);
	void OnWkeUrlChange(const CDuiString& strUrl);

public:
	bool OnConnectSuccessProc();																			// 网络连接回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);							// Socket断开
	void ReConnect(DLG_ID id, TNotifyUI& msg);																// 重连
	bool OnNetwork_Bank(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);							// 银行消息
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);						// 道具消息

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

	UserData*			m_pSelfData;						// 当前玩家数据
	DWORD				m_dwCurPageIndex;					// 当前页索引
	TCHAR				m_pzBuffer[MAX_PATH];				// 字符缓冲数组

	CTabLayoutUI*		m_pTabLaout;						// 银行面板容器

	//第1页(存取款)
	CRichEditUI*		m_pSwitch1_EdtYXB;					// 游戏币
	CRichEditUI*		m_pSwitch1_EdtSaveNum;				// 存入金额
	
	CRichEditUI*		m_pSwitch1_EdtDeposit;				// 存款
	CRichEditUI*		m_pSwitch1_EdtTakeNum;				// 取出金额
	CRichEditUI*		m_pSwitch1_EdtTakePassword;			// 取出密码
	COptionUI*			m_pSwitch1_OptMarkPw;				// 记住密码
	
	//第2页(修改密码)
	CRichEditUI*		m_pSwitch2_EdtSrcPw;				// 原密码
	CControlUI*			m_pSwitch2_CtlSrcPw;

	CRichEditUI*		m_pSwitch2_EdtNewPw;				// 新密码
	CControlUI*			m_pSwitch2_CtlNewPw;

	CRichEditUI*		m_pSwitch2_EdtCfmPw;				// 确认密码
	CControlUI*			m_pSwitch2_CtlCfmPw;

	//第3页(魅力兑换)
	LONG				m_lMaxCharmOnce;					// 限制每次兑换次数
	CRichEditUI*		m_pSwitch3_EdtCharms;				// 魅力值
	CRichEditUI*		m_pSwitch3_EdtDeposit;				// 银行存款
	CRichEditUI*		m_pSwitch3_EdtExCount;				// 兑换数量
	COptionUI*			m_pSwitch3_Opt;						// 兑换单选项
	CRichEditUI*		m_pSwitch3_EdtTakePassword;			// 取款密码
	COptionUI*			m_pSwitch3_OptMarkPw;				// 记住密码

	//第4页(魅力明细)
	CWkeWebkitUI*		m_pSwitch4_WebCharmsDetail;			// 魅力明细

	//第5页(魅力明细)
	CWkeWebkitUI*		m_pSwitch5_WebScoreRebate;			// 积分返利

	std::vector<bool>	m_vPwComplete;

	LoginServer*		m_pLoginServer;						// 登录服务器接口对象
	BanckOperateStatus	m_iCurOperateStatus;				// 当前操作状态
};

#endif //__BANK_H__