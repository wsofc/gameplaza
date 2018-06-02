#ifndef __HALLFRAME_H__
#define __HALLFRAME_H__

#include "../room/roomManager.h"
#include "../ui/UIGameItem.h"
#include "../ipc/loginServer.h"

#include "../face/faceSelList.h"
#include "../face/faceSelFrame.h"

// 当前标记的游戏信息
struct tCurTaggedGameInfo
{
	tCurTaggedGameInfo()
	{
		memset(this, 0, sizeof(*this));
	}

	WORD			wKindID;
	WORD			wTypeID;
	WORD			wRoomID;
	CTreeNodeUI*	pTreeNode;
};

class HallFrame : public WindowImplBase, public CWkeWebkitClientCallback, public LoginSeverDelegate
{
public:
	static HallFrame* GetInstance();
	BOOL CreateFrame();
	void SwitchToLogin();
	CPaintManagerUI* GetPaintManager(){ return &m_PaintManager; }

	virtual ~HallFrame();

public:
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
	void OnCopyData(void* pData);

	// 检测游戏版本
	CGameItemUI::GAME_STATE CheckGameVersion(const tagGameKind* pData, CGameItemUI* pGameItem);

	// 初始化数据
	void InitData();

	// 初始化本地配置文件
	void InitLocalConfig();

	//初始化游戏列表
	void InitGameList();

	//清除游戏列表
	void CleanGameList();

	//初始化表情库
	void InitFaceData();

	//初始化房间、桌子管理器
	void InitHallManager();

	//初始化网页内容
	void InitWeb();

	//初始化大厅UI
	void InitHallUI();

	//通过游戏ID获取游戏子项
	CGameItemUI* FindGameItemByKindID(WORD wKindID);

	//通过控件名获取游戏子项
	CGameItemUI* FindGameItemByName(LPCTSTR lpName);

	//定时器回调函数
	void TimerProFunc(UINT_PTR idEvent, DWORD dwTime);

	//创建游戏类型单选按钮
	void CreatGameOptBtn(int iIndex, LPCTSTR pzText);

	//获取单选按钮组当前选中的下标
	int GetGameOptSelectIndex();

	//选中单选组按钮
	void SelectGameOptByIndex(int iIndex);

	//平铺大厅游戏列表
	void LayoutGameItem(CTileLayoutUI* tPanel, std::vector<CGameItemUI*>& vtGameItemArr);

	//"表情"控件选取回调消息
	void OnFaceCtrlSel(int iSelID, int iSelIndex, LPCTSTR lpSelName);
	
	//聊天面板右键菜单
	void OnMenuEvent(CDuiString* strMenuName, CControlUI* pControl);
	void ChatEdit_RButtonDown(TNotifyUI& msg);									// 右键菜单
	void ChatEdit_RCut(CControlUI* pParentControl);								// 剪切
	void ChatEdit_RCopy(CControlUI* pParentControl);							// 复制
	void ChatEdit_RPaste(CControlUI* pParentControl);							// 粘贴
	void ChatEdit_RSelAll(CControlUI* pParentControl);							// 全部选择
	void ChatEdit_RClear(CControlUI* pParentControl);							// 清屏
	void ChatEdit_RZoomRatio(CControlUI* pParentControl, LPCTSTR lpMenuName);	// 显示比例
	void ChatEdit_RSaveAs(CControlUI* pParentControl);							// 另存为
	
	//发送聊天消息
	void ChatEdit_SendMsg(CDuiString& strText);

	// 换肤
	void OnChangeSkin();

public:
	void OnWkeTitleChange(const CDuiString& strTitle);
	void OnWkeUrlChange(const CDuiString& strUrl);

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	bool _RegisterImgOle();																	// 注册ImgOle组件
	bool _UnRegisterImgOle();																// 反注册ImgOle组件
	bool _RegisterImgOleCallBack();															// 注册聊天编辑框Ole对象回调
	DWORD _ReCheckGameItemMap();															// 重新检测游戏Item容器
	void _InitExperinceConfig();															// 初始化经验值配置
	void _InitOnlineDescConfig();															// 初始化在线人数描述配置
	void _InitGameListStyle_1();															// 初始化游戏风格1列表
	void _InitGameListStyle_2();															// 初始化游戏风格2列表

private:
	
	static HallFrame*							g_instance;
	HallFrame();

	TCHAR										m_pzBuffer[4 * MAX_PATH];			// 字符串缓冲区
#ifdef GAMEPLAZA_STYLE_1
	CWkeWebkitUI*								m_pGameWebBrowser;					// 大厅浏览器控件
#else if GAMEPLAZA_STYLE_2
	IWebBrowser2*								m_pGameWebBrowser;					// 大厅浏览器控件
#endif
	bool										m_bSwitchToLogin;					// 关闭大厅切换到登录界面
	CTextUI*									m_pNickName;						// 昵称
	CTextUI*									m_pID;								// 用户ID
	CRichEditUI*								m_pViewEdit;						// 聊天面板,展示框
	CRichEditUI*								m_pSendEdit;						// 聊天面板,输入框
	POINT										m_pointChatROle;					// 聊天面板,右键菜单时图片ole对象相对窗口的坐标
	std::map<CDuiString, bool>					m_chatEditViewMenuCheckInfo;		// 保存菜单的单选复选信息
	std::map<CDuiString, bool>					m_chatEditInputMenuCheckInfo;		// 保存菜单的单选复选信息
	CControlUI*									m_pRollCtl;							// 底栏滚动文本
	RECT										m_rcLastWnd;						// 上次窗口大小
	RECT										m_rcLastRollCtl;					// 上次滚动文本位置

	RoomManager*								m_pRoomManager;						// 房间管理器
	DataManager*								m_pDataManager;						// 数据管理器
	std::map<WORD, std::vector<CGameItemUI*>>	m_vGameItemMap;						// 游戏Item容器
	CDuiString									m_pzCurRoomName;					// 当前房间名
	std::vector<CDuiString>						m_vOptName;							// 单选按钮名

// ------------------------------------------------------------------------------------------------------------------------
// 风格2

private:
	
	std::vector<CTreeNodeUI*>				m_vGameTypeNodeUI;					// 记录游戏类型UI节点数组
	std::vector<CTreeNodeUI*>				m_vGameKindNodeUI;					// 记录游戏子项UI节点数组
	std::vector<CTreeNodeUI*>				m_vGameRoomNodeUI;					// 记录游戏房间UI节点数组

	tCurTaggedGameInfo						m_tCurTaggedGameInfo;				// 当前标记的游戏信息
	CDuiString								m_sLastRoomNodeName;				// 记录上一次房间节点名
	std::vector<CDuiString>					m_vLastFolderFlagMap;				// 记录上一次游戏列表折叠标志(该容器只记录展开的节点,默认都是折叠的)
	SIZE									m_tLastScrollPos;					// 记录上一次游戏列表滚动条滚动位置
	SIZE									m_tLastScrollRange;					// 记录上一次游戏列表滚动条滚动范围
	WORD									m_wMaxFrequentCount;				// 记录最大常登陆游戏数量
	
	CContainerUI*							m_pGameListRoot;					// 游戏列表根节点
	CTreeViewUI*							m_pGameListTreeView;				// 游戏列表树状视图节点
	tagGameType								m_pFrequentType;					// 经常登录的游戏类型
	std::vector<WORD>						m_vFrequentGameKinds;				// 经常登录的游戏数组
	CDuiString								m_pzUrlOfficial;					// 官网地址
	CDuiString								m_pzUrlFormat;						// 游戏网页格式串

public:

	static const int						g_iTypeNodeHeight = 25;				// 游戏类型UI节点高度
	static const int						g_iGameNodeHeight = 25;				// 游戏子项UI节点高度
	static const int						g_iRoomNodeHeight = 25;				// 游戏房间UI节点高度

private:

	LPCTSTR	_GetGameTypeIconPath(tagGameType* pData);
	LPCTSTR	_GetGameKindIconPath(CGameItemUI* pData);
	LPCTSTR	_GetGameRoomIconPath(tagGameServer* pData);
	void _InitFrequentNode();

public:
	
	bool FindFolderFlagByName(CDuiString pzName);														// 查找游戏列表折叠标志
	void AddFolderFlagByName(CDuiString pzName);														// 添加游戏列表折叠标志
	
	bool OnGameTypeNotify(void* param);																	// 游戏类型事件
	bool OnGameKindNotify(void* param);																	// 游戏点击事件
	bool OnGameRoomNotify(void* param);																	// 游戏房间事件
	bool CheckGameUpdate(CControlUI* pNode, CGameItemUI* pGameItem, WORD wRoomID = INVALID_WORD);		// 检查游戏更新
	void OnGameDownloadComplete(WORD wGameKindID);														// 游戏下载回调
	
	CTreeViewUI* AddSeasonRoot(CContainerUI* pParentCtl, LPCTSTR lpName);								// 添加列表视图根节点
	CTreeNodeUI* AddGameTypeNode(CTreeViewUI* pParentView, tagGameType* pData, WORD wTypeID);			// 添加游戏类型
	CTreeNodeUI* AddGameKindNode(CTreeNodeUI* pParentNode, CGameItemUI* pData, WORD wTypeID);			// 添加游戏子项
	CTreeNodeUI* AddGameRoomNode(CTreeNodeUI* pParentNode, tagGameServer* pData, WORD wTypeID);			// 添加游戏房间

	void RefreshGameTypeNode(CTreeNodeUI* pNode, tagGameType* pData);									// 刷新游戏类型UI
	void RefreshGameKindNode(CTreeNodeUI* pNode, CGameItemUI* pData);									// 刷新游戏子项UI
	void RefreshGameRoomNode(CTreeNodeUI* pNode, tagGameServer* pData);									// 刷新游戏房间UI

	void AddFrequentGames(WORD wKindID, bool bAddToFirst = false);										// 添加至常玩游戏列表
	bool RemoveFrequentGames(WORD wKindID);																// 从常玩游戏列表中移除
	void ClearFrequentGames();																			// 清除常玩游戏列表
	void ReadFrequentGamesConfig();																		// 读取常玩游戏数据配置
	void SaveFrequentGamesConfig();																		// 保存常玩游戏数据配置
	void SaveSkinConfig();																				// 保存皮肤数据
	void SaveLocalData();																				// 保存本地数据
};
#endif //__HALLFRAME_H__