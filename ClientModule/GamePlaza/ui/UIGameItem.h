#ifndef __UIGAMEITEM_H__
#define __UIGAMEITEM_H__

#define DUI_MSGTYPE_GAMEITEM_CLICK	(_T("gameitem_click"))


// 游戏列表项
namespace DuiLib {
	
	class CGameItemUI : public CLabelUI
	{
	public:
		CGameItemUI();
		~CGameItemUI();
		static bool less_sort_item_id(const CGameItemUI* a, const CGameItemUI* b);		// GameItem排序
		static bool less_sort_type_id(const tagGameType* a, const tagGameType* b);		// 游戏类型排序
		static bool less_sort_kind_id(const tagGameKind* a, const tagGameKind* b);		// 游戏排序
		static bool less_sort_room_id(const tagGameServer* a, const tagGameServer* b);	// 房间排序

		enum GAME_STATE
		{
			GAME_STATE_NOT_OPEN = 0,		// 未开放
			GAME_STATE_NOT_EXIST,			// 未下载
			GAME_STATE_VERSION_ERROE,		// 版本异常
			GAME_STATE_VERSION_NORMAL		// 版本正常
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
		GAME_STATE		m_uGameState;							// 游戏状态
		DWORD			m_iOffset;								// 偏移大小
		CButtonUI*		m_control;								// icon按钮
		tagGameKind		m_tStruct;								// 游戏结构

		UINT			m_uButtonState;
		CDuiString		m_pzImg;								// 背景图片
		CDuiString		m_pzStatusDesc;							// 游戏状态描述语
		CDuiString		m_pzNoExtension;						// 去后缀文件名(去掉.exe)
		CDuiString		m_pzConbinedFileName;					// 组合文件名(去后缀文件名 + 执行程序名)
		TCHAR			m_pzModify[MAX_PATH];					
		bool			m_bFirstPadding;
		std::vector<tagGameServer*> m_vGameRoomList;
	};

} // namespace DuiLib

#endif //__UIGAMEITEM_H__
