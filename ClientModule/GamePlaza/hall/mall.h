#ifndef __MALL_H__
#define __MALL_H__

#include "hallFrame.h"
#include "..\\ui\\UIBagItem.h"

// 商城面板
class MallBuyFrame;
class MallFrame : public WindowImplBase
{
public:
	~MallFrame();
	static MallFrame* GetInstance();
	BOOL CreateFrame();
	
public:

	void InitWindow();
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnMenuEvent(CDuiString* strMenuName, CControlUI* pControl);
	void Notify(TNotifyUI& msg);
	
	void InitGoods();
	void InitBags();
	void InitData(TNotifyUI& msg);
	
	void AddGoodsItem(tagPropsInfo* pGoods);
	void AddBagItem(tagPropsInfo* pGoods);

	// 翻页
	void UpdatePages(int iCurSel);
	void GoodsTurnPage(tItemsTileDesc& tTarget, bool bNext);
	void BagsTurnPage(tItemsTileDesc& tTarget, bool bNext);

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("MallFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	MallFrame();
	static MallFrame*				g_instance;
	
	TCHAR							m_pzBuffer[MAX_PATH];	// 字符串缓冲区
	CTabLayoutUI*					m_pTabLayout;			// 总布局控件
	CLabelUI*						m_pLabPage;				// 页数
	CButtonUI*						m_pBtnPageLeft;			// 左翻页按钮
	CButtonUI*						m_pBtnPageRight;		// 右翻页按钮
	CContainerUI*					m_pCtnMallPanel;		// 商城布局控件
	CContainerUI*					m_pCtnBagPanel;			// 背包布局控件
	tGoodsTileDesc					m_tGoodsTileDesc;		// 商城物品控件布局描述
	tItemsTileDesc					m_tBagTileDesc;			// 背包物品控件布局描述
};

// 购买面板
class MallBuyFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	static MallBuyFrame* GetInstance();
	~MallBuyFrame();
	static bool IsInstanceExist(){ return MallBuyFrame::g_instance != nullptr; }

	enum MallOperateStatus
	{
		MALL_BUY_OS_NONE					// 无状态
		, MALL_REQ_NICKNAME					// 获取昵称
		, MALL_BUY_OS_BUY					// 购买
		, MALL_BUY_OS_EXTRA					// 额外预留项
	};

	enum MallMsgBoxID
	{
		MSG_BOX_ID_NONE = 0					// 无
		, MSG_BOX_ID_MALL_BUY_COUNT			// 购买数量
		, MSG_BOX_ID_MALL_RECEIVE_ID		// 接收方ID
		, MSG_BOX_ID_MALL_PW_ERROR			// 输入的密码不正确
		, MSG_BOX_ID_MALL_BUY_ENSURE		// 购买确认
	};

	enum MallBuyType
	{
		MALL_BUY_TYPE_NONE = 0				// 无
		, MALL_BUY_TYPE_HALL				// 大厅中购买(连接登录服)
		, MALL_BUY_TYPE_ROOM				// 房间中购买(连接游戏服)
	};

	BOOL CreateFrame(CPaintManagerUI* pParentManager, tagPropsInfo* pProps, MallBuyType buyType = MallBuyType::MALL_BUY_TYPE_HALL);
	void OnFinalMessage(HWND hWnd);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void InitWindow();
	void Notify(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void InitData();
	void AddBuyCount(LONG lCount, bool bClear = false);
	void UpdateBuyPrice(WORD wTotalCount);
	void CheckBuy();
	void BuyInHall();
	void BuyInRoom();
	void SendBuyReq();
	void CheckNickName();

	void SetTickedBuyPw(bool bTicked);
	void SetBuyGoodsPw(CDuiString& pzBuyPw);
	
	void SetBuyBtnEnable(bool bEnable);																		// 限制购买,只能一次一次的购买或赠送道具

public:
	
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);
	bool OnConnectSuccessProc();																			// 网络连接成功回调
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);							// Socket断开
	void ReConnect(DLG_ID id, TNotifyUI& msg);																// 重连
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);							// 用户消息
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);						// 道具消息
	
protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("MallBuyFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	MallBuyFrame();
	static MallBuyFrame*		g_instance;

	bool						m_bTickedBuyPw;			// MallBuyCheckFrame 是否勾选记住密码
	CDuiString					m_pzBuyGoodsPw;			// MallBuyCheckFrame 密码

	LONG						m_lMinBuyCount;			// 购买最小数量
	LONG						m_lMaxBuyCount;			// 购买最大数量
	TCHAR						m_pzBuffer[MAX_PATH];	// 字符串缓冲区
	DWORD						m_dwRevNameTxtColor;	// 接收昵称默认文字颜色	
	tagPropsInfo*				m_pProps;				// 物品对象
	CLabelUI*					m_pLabIcon;				// 物品图标
	CTextUI*					m_pTxtName;				// 物品名
	CTextUI*					m_pTxtSrcCharm;			// 物品附带赠送方的魅力点数
	CTextUI*					m_pTxtRevCharm;			// 物品附带接收方的魅力点数
	CTextUI*					m_pTxtPrice;			// 物品价格
	CTextUI*					m_pTxtCostCapital;		// 总花费大写
	CRichEditUI*				m_pEdtCount;			// 购买数量
	CRichEditUI*				m_pEdtReceiveID;		// 接收ID
	CRichEditUI*				m_pEdtReceiveNickName;	// 接收昵称
	CButtonUI*					m_pBtnEnsureBuy;		// 确定购买按钮
	CButtonUI*					m_pBtnPageLeft;			// 左翻页按钮
	CButtonUI*					m_pBtnPageRight;		// 右翻页按钮
	CPaintManagerUI*			m_pParentManager;		// 父窗口绘制管理器
	NumberConvert*				m_pNumberConvert;		// 数字转中文大写
	MallBuyType					m_iCurBuyType;			// 当前购买商品方式(大厅\房间购买)
	MallOperateStatus			m_iCurOperateStatus;	// 当前操作状态
	LoginServer*				m_pLoginServer;			// 登录服务器接口对象
};

// 赠送面板
class MallGiveFrame : public WindowImplBase
{
public:
	MallGiveFrame();
	~MallGiveFrame();

	BOOL CreateFrame(CPaintManagerUI* pParentManager, tagPropsInfo* pGoods);
	void OnFinalMessage(HWND hWnd);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void InitWindow();
	void Notify(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void InitData(TNotifyUI& msg);
	void Check();

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("MallGiveFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	TCHAR						m_pzBuffer[MAX_PATH];	// 字符串缓冲区
	tagPropsInfo*				m_pProps;				// 物品对象
	CPaintManagerUI*			m_pParentManager;		// 父窗口绘制管理器
	CTextUI*					m_pTxtName;				// 物品名
	CRichEditUI*				m_pEdtCount;			// 赠送数量
	CRichEditUI*				m_pEdtReceiveID;		// 接收ID
};

// 购买时密码检测面板
class MallBuyCheckFrame : public WindowImplBase
{
public:

	static MallBuyCheckFrame* GetInstance();
	static bool IsInstanceExist(){ return MallBuyCheckFrame::g_instance != nullptr; }
	BOOL CreateFrame(MallBuyFrame* pBuyFrame);
	
public:
	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0					// 无
		, MSG_BOX_ID_CHECK_PW				// 银行密码
		, MSG_BOX_ID_EXTRA					// 预留项
	};

	~MallBuyCheckFrame();
	void OnFinalMessage(HWND hWnd);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void InitWindow();
	void Notify(TNotifyUI& msg);

	void InitData(TNotifyUI& msg);
	void Check();
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);
	
protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("MallBuyCheckFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	MallBuyCheckFrame();
	static MallBuyCheckFrame*	g_instance;
	static bool					g_bTickedBankPw;
	
	MallBuyFrame*				m_pBuyFrame;
	CRichEditUI*				m_pEdtPassword;			// 银行密码
	COptionUI*					m_pOptPassword;			// 单选框
};

#endif //__MALL_H__