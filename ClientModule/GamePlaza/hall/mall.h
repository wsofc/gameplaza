#ifndef __MALL_H__
#define __MALL_H__

#include "hallFrame.h"
#include "..\\ui\\UIBagItem.h"

// �̳����
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

	// ��ҳ
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
	
	TCHAR							m_pzBuffer[MAX_PATH];	// �ַ���������
	CTabLayoutUI*					m_pTabLayout;			// �ܲ��ֿؼ�
	CLabelUI*						m_pLabPage;				// ҳ��
	CButtonUI*						m_pBtnPageLeft;			// ��ҳ��ť
	CButtonUI*						m_pBtnPageRight;		// �ҷ�ҳ��ť
	CContainerUI*					m_pCtnMallPanel;		// �̳ǲ��ֿؼ�
	CContainerUI*					m_pCtnBagPanel;			// �������ֿؼ�
	tGoodsTileDesc					m_tGoodsTileDesc;		// �̳���Ʒ�ؼ���������
	tItemsTileDesc					m_tBagTileDesc;			// ������Ʒ�ؼ���������
};

// �������
class MallBuyFrame : public WindowImplBase, public LoginSeverDelegate
{
public:
	static MallBuyFrame* GetInstance();
	~MallBuyFrame();
	static bool IsInstanceExist(){ return MallBuyFrame::g_instance != nullptr; }

	enum MallOperateStatus
	{
		MALL_BUY_OS_NONE					// ��״̬
		, MALL_REQ_NICKNAME					// ��ȡ�ǳ�
		, MALL_BUY_OS_BUY					// ����
		, MALL_BUY_OS_EXTRA					// ����Ԥ����
	};

	enum MallMsgBoxID
	{
		MSG_BOX_ID_NONE = 0					// ��
		, MSG_BOX_ID_MALL_BUY_COUNT			// ��������
		, MSG_BOX_ID_MALL_RECEIVE_ID		// ���շ�ID
		, MSG_BOX_ID_MALL_PW_ERROR			// ��������벻��ȷ
		, MSG_BOX_ID_MALL_BUY_ENSURE		// ����ȷ��
	};

	enum MallBuyType
	{
		MALL_BUY_TYPE_NONE = 0				// ��
		, MALL_BUY_TYPE_HALL				// �����й���(���ӵ�¼��)
		, MALL_BUY_TYPE_ROOM				// �����й���(������Ϸ��)
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
	
	void SetBuyBtnEnable(bool bEnable);																		// ���ƹ���,ֻ��һ��һ�εĹ�������͵���

public:
	
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);
	bool OnConnectSuccessProc();																			// �������ӳɹ��ص�
	bool OnSocketCloseProc(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient);							// Socket�Ͽ�
	void ReConnect(DLG_ID id, TNotifyUI& msg);																// ����
	bool OnNetwork_User(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);							// �û���Ϣ
	bool OnNetwork_Props(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize);						// ������Ϣ
	
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

	bool						m_bTickedBuyPw;			// MallBuyCheckFrame �Ƿ�ѡ��ס����
	CDuiString					m_pzBuyGoodsPw;			// MallBuyCheckFrame ����

	LONG						m_lMinBuyCount;			// ������С����
	LONG						m_lMaxBuyCount;			// �����������
	TCHAR						m_pzBuffer[MAX_PATH];	// �ַ���������
	DWORD						m_dwRevNameTxtColor;	// �����ǳ�Ĭ��������ɫ	
	tagPropsInfo*				m_pProps;				// ��Ʒ����
	CLabelUI*					m_pLabIcon;				// ��Ʒͼ��
	CTextUI*					m_pTxtName;				// ��Ʒ��
	CTextUI*					m_pTxtSrcCharm;			// ��Ʒ�������ͷ�����������
	CTextUI*					m_pTxtRevCharm;			// ��Ʒ�������շ�����������
	CTextUI*					m_pTxtPrice;			// ��Ʒ�۸�
	CTextUI*					m_pTxtCostCapital;		// �ܻ��Ѵ�д
	CRichEditUI*				m_pEdtCount;			// ��������
	CRichEditUI*				m_pEdtReceiveID;		// ����ID
	CRichEditUI*				m_pEdtReceiveNickName;	// �����ǳ�
	CButtonUI*					m_pBtnEnsureBuy;		// ȷ������ť
	CButtonUI*					m_pBtnPageLeft;			// ��ҳ��ť
	CButtonUI*					m_pBtnPageRight;		// �ҷ�ҳ��ť
	CPaintManagerUI*			m_pParentManager;		// �����ڻ��ƹ�����
	NumberConvert*				m_pNumberConvert;		// ����ת���Ĵ�д
	MallBuyType					m_iCurBuyType;			// ��ǰ������Ʒ��ʽ(����\���乺��)
	MallOperateStatus			m_iCurOperateStatus;	// ��ǰ����״̬
	LoginServer*				m_pLoginServer;			// ��¼�������ӿڶ���
};

// �������
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

	TCHAR						m_pzBuffer[MAX_PATH];	// �ַ���������
	tagPropsInfo*				m_pProps;				// ��Ʒ����
	CPaintManagerUI*			m_pParentManager;		// �����ڻ��ƹ�����
	CTextUI*					m_pTxtName;				// ��Ʒ��
	CRichEditUI*				m_pEdtCount;			// ��������
	CRichEditUI*				m_pEdtReceiveID;		// ����ID
};

// ����ʱ���������
class MallBuyCheckFrame : public WindowImplBase
{
public:

	static MallBuyCheckFrame* GetInstance();
	static bool IsInstanceExist(){ return MallBuyCheckFrame::g_instance != nullptr; }
	BOOL CreateFrame(MallBuyFrame* pBuyFrame);
	
public:
	enum MsgBoxID
	{
		MSG_BOX_ID_NONE = 0					// ��
		, MSG_BOX_ID_CHECK_PW				// ��������
		, MSG_BOX_ID_EXTRA					// Ԥ����
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
	CRichEditUI*				m_pEdtPassword;			// ��������
	COptionUI*					m_pOptPassword;			// ��ѡ��
};

#endif //__MALL_H__