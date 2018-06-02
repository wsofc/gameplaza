#ifndef __USERLIST_H__
#define __USERLIST_H__

#define LISTHEAD_SEP_WIDTH	2
#define LISTHEAD_MIN_WIDTH	2

#include "userControl.h"
#include "unrealControl.h"

class UserList : public IListCallbackUI
{
public:
	static UserList* GetInstance();
	static bool IsExistInstance(){ return g_instance != nullptr; }
	void Release();

	void InsertSelfDefineHeadItem();
	bool InsertHeadItem(tagColumnItem* pColumItem);
	int GetHeadItemCount(){ return m_vHeadArry.size(); }
	tagColumnItem* EnumHeadItem(DWORD& iNextKey);
	bool RemoveHeadItem(WORD wDataDescribe);
	void RemovaAllHeadItem();

	CListHeaderItemUI* CreatFixedHeadItem(WORD wIndex, WORD wColumnWidth, bool bSep = true);
	CListHeaderItemUI* CreatFloatHeadItem(WORD wIndex, tagColumnItem& item);
	void LoadHeaderItem(CListUI* pList);
	void InsertBodyItem(WORD wFixedHeight, UserData* pUser, UserData* pSelf = nullptr);
	void RemoveBodyItem(DWORD wUserID);
	void RemoveAllBodyItem();
	void Clean();
	
	static bool SortItemFunc(UserData* item_a, UserData* item_b);
	void SortBodyItem(bool bOpen = true);
	void Update(UserData* pTagUser = nullptr);
	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	void Notify(TNotifyUI& msg);
	void SelectByUserID(DWORD dwUserID);

	// 断线列表处理
	void AddToOffLineMap(DWORD dwUserID);						// 添加至离线列表
	void RemoveOffLineMap(DWORD dwUserID);						// 清除离线列表该玩家
	void RecoverOffLineMap(DWORD dwUserID);						// 从离线列表中恢复该玩家

	// 获取列表玩家对象数组
	const std::vector<UserData*>& GetListBodyArray() const { return m_vBodyArry; }

private:
	UserList();
	~UserList();
	static UserList* g_instance;
		
	bool							m_bDistinguish;				// 是否区分玩家与机器人
	CListUI*						m_pList;					// 列表控件
	TCHAR							m_pzBuffer[2 * MAX_PATH];	// 字符串缓冲区
	std::vector<tagColumnItem*>		m_vHeadArry;				// 列表头数组
	std::vector<UserData*>			m_vBodyArry;				// 玩家数据数组
	std::map<DWORD, UserData*>		m_vDisConnectMap;			// 玩家断线列表
	UserControl*					m_pUserControl;				// 用户控制类
};

#endif // !__USERLIST_H__
