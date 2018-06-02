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

	// �����б���
	void AddToOffLineMap(DWORD dwUserID);						// ����������б�
	void RemoveOffLineMap(DWORD dwUserID);						// ��������б�����
	void RecoverOffLineMap(DWORD dwUserID);						// �������б��лָ������

	// ��ȡ�б���Ҷ�������
	const std::vector<UserData*>& GetListBodyArray() const { return m_vBodyArry; }

private:
	UserList();
	~UserList();
	static UserList* g_instance;
		
	bool							m_bDistinguish;				// �Ƿ���������������
	CListUI*						m_pList;					// �б�ؼ�
	TCHAR							m_pzBuffer[2 * MAX_PATH];	// �ַ���������
	std::vector<tagColumnItem*>		m_vHeadArry;				// �б�ͷ����
	std::vector<UserData*>			m_vBodyArry;				// �����������
	std::map<DWORD, UserData*>		m_vDisConnectMap;			// ��Ҷ����б�
	UserControl*					m_pUserControl;				// �û�������
};

#endif // !__USERLIST_H__
