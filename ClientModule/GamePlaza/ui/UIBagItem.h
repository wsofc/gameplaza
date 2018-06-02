#ifndef __UIBAGITEM_H__
#define __UIBAGITEM_H__

#include "..\\data\\serviceData.h"

// 游戏列表项
namespace DuiLib {

	class CBagTipsWnd;
	class CBagTileLayout;
	class CBagItemUI;
	class CBagBoxUI;

	struct tItemsTileDesc;
	struct tGoodsTileDesc;
	
	// 物品布局描述
	struct tItemsTileDesc
	{
		int					icol;		// 列
		int					irol;		// 行
		int					iw;			// 总宽
		int					ih;			// 总高
		UINT				uCurPage;	// 当前页
		UINT				uMinPage;	// 最小页
		UINT				uMaxPage;	// 最大页

		SIZE				tileSize;	// 格子大小
		RECT				rcPadding;	// 格子间距
		CBagTileLayout*		pLayout;	// 格子所属布局

		tItemsTileDesc()
		{
			memset(this, 0, sizeof(this));
		}
	};

	// 商品布局描述
	struct tGoodsTileDesc : public tItemsTileDesc
	{
		UINT				uTxtHeight;	// 描述文本高
		SIZE				iconSize;	// iocn大小

		tGoodsTileDesc()
		{
			memset(this, 0, sizeof(this));
		}
	};

	// 提示信息
	class CBagTipsWnd : public WindowImplBase
	{
	public:
		CBagTipsWnd();
		~CBagTipsWnd();

		BOOL CreateFrame(CPaintManagerUI* pManager);			// 创建窗口
		void FillGoodsData(tagPropsInfo* pProps);				// 填充物品数据
		SIZE EstimateSize(SIZE szAvailable);					// 计算窗口大小
		void ResizeShow(RECT rc);								// 适配显示窗口
		void InitWindow();

	protected:
		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		CDuiString GetSkinFolder();
		CDuiString GetSkinFile();
		LPCTSTR GetWindowClassName(void) const;

	private:
		SIZE		m_cxyFixed;									// 保存控件最大大小
		TCHAR		m_pzBuffer[MAX_PATH];						// 字符串缓冲区
		CTextUI*	m_pTxtGoodsName;							// 物品名
		CLabelUI*	m_pLabGoodsIcon;							// 物品图标
		CTextUI*	m_pTxtGoodsDesc;							// 物品描述
		CTextUI*	m_pTxtGoodsPrice;							// 物品价格
	};

	// 格子布局
	class CBagTileLayout : public CTileLayoutUI
	{
	public:
		CBagTileLayout();
		~CBagTileLayout();
	
	protected:

	private:

	};
	
	// 物品格子控件
	class CBagItemUI : public CButtonUI
	{
	public:
		CBagItemUI();
		~CBagItemUI();
	
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void DoEvent(TEventUI& event);
		void DoPaint(HDC hDC, const RECT& rcPaint);
		void PaintBackground(HDC hDC);

		LPCTSTR GetBackgroundImg();
		void SetBackgroundImg(LPCTSTR pStrImage);
		void SetGoodsData(tagPropsInfo* pProps);
		void ShowTips(bool bVisible = true);
		tagPropsInfo* GetGoods();

	protected:
		CDuiString						m_sBackgroundImage;
		tagPropsInfo*					m_pProps;
		static CBagTipsWnd*				g_pTipsFrame;
	};
	
	// 背包格子控件
	class CBagBoxUI :public CBagItemUI
	{
	public:
		CBagBoxUI();
		~CBagBoxUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;
		void DoEvent(TEventUI& event);

		void ShowMenu();

	private:
		std::map<CDuiString, bool>		m_MenuCheckInfo;		// 保存菜单的单选复选信息
	};

} // namespace DuiLib

#endif //__UIBAGITEM_H__
