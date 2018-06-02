#ifndef __UIGAMEITEM_H__
#define __UIGAMEITEM_H__

#define DUI_MSGTYPE_GAMEITEM_CLICK	(_T("gameitem_click"))


// ��Ϸ�б���
namespace DuiLib {
	
	class CGameItemUI : public CLabelUI
	{
	public:
		CGameItemUI();
		~CGameItemUI();
		static bool less_sort_item_id(const CGameItemUI* a, const CGameItemUI* b);		// GameItem����
		static bool less_sort_type_id(const tagGameType* a, const tagGameType* b);		// ��Ϸ��������
		static bool less_sort_kind_id(const tagGameKind* a, const tagGameKind* b);		// ��Ϸ����
		static bool less_sort_room_id(const tagGameServer* a, const tagGameServer* b);	// ��������

		enum GAME_STATE
		{
			GAME_STATE_NOT_OPEN = 0,		// δ����
			GAME_STATE_NOT_EXIST,			// δ����
			GAME_STATE_VERSION_ERROE,		// �汾�쳣
			GAME_STATE_VERSION_NORMAL		// �汾����
		};

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;
		bool Activate();
		void SetGameStatus(GAME_STATE uGameState);
		GAME_STATE GetGameStatus(){ return m_uGameState; }
		LPCTSTR GetGameStatusDesc(){ return m_pzStatusDesc.GetData(); }
		bool InitData(const tagGameKind* pStruct, DWORD dwTextcolor);
		const tagGameKind* GetGameKind() const { return &m_tStruct; }
		const void SetMaxVersion(DWORD dwMaxVersion) { m_tStruct.dwMaxVersion = dwMaxVersion; }
		const DWORD GetMaxVersion() { return m_tStruct.dwMaxVersion; }
		std::vector<tagGameServer*>& GetGameRoomList(){ return m_vGameRoomList; }
		LPCTSTR GetNoExtensionName(){ return m_pzNoExtension.GetData(); }
		LPCTSTR GetCombinedFileName(){ return m_pzConbinedFileName.GetData(); }

	private:
		void DoEvent(TEventUI& event);
		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
		
	private:
		GAME_STATE		m_uGameState;							// ��Ϸ״̬
		DWORD			m_iOffset;								// ƫ�ƴ�С
		CButtonUI*		m_control;								// icon��ť
		tagGameKind		m_tStruct;								// ��Ϸ�ṹ

		UINT			m_uButtonState;
		CDuiString		m_pzImg;								// ����ͼƬ
		CDuiString		m_pzStatusDesc;							// ��Ϸ״̬������
		CDuiString		m_pzNoExtension;						// ȥ��׺�ļ���(ȥ��.exe)
		CDuiString		m_pzConbinedFileName;					// ����ļ���(ȥ��׺�ļ��� + ִ�г�����)
		TCHAR			m_pzModify[MAX_PATH];					
		bool			m_bFirstPadding;
		std::vector<tagGameServer*> m_vGameRoomList;
	};

} // namespace DuiLib

#endif //__UIGAMEITEM_H__
