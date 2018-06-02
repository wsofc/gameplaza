#ifndef __UIBAGITEM_H__
#define __UIBAGITEM_H__

#include "..\\data\\serviceData.h"

// ��Ϸ�б���
namespace DuiLib {

	class CBagTipsWnd;
	class CBagTileLayout;
	class CBagItemUI;
	class CBagBoxUI;

	struct tItemsTileDesc;
	struct tGoodsTileDesc;
	
	// ��Ʒ��������
	struct tItemsTileDesc
	{
		int					icol;		// ��
		int					irol;		// ��
		int					iw;			// �ܿ�
		int					ih;			// �ܸ�
		UINT				uCurPage;	// ��ǰҳ
		UINT				uMinPage;	// ��Сҳ
		UINT				uMaxPage;	// ���ҳ

		SIZE				tileSize;	// ���Ӵ�С
		RECT				rcPadding;	// ���Ӽ��
		CBagTileLayout*		pLayout;	// ������������

		tItemsTileDesc()
		{
			memset(this, 0, sizeof(this));
		}
	};

	// ��Ʒ��������
	struct tGoodsTileDesc : public tItemsTileDesc
	{
		UINT				uTxtHeight;	// �����ı���
		SIZE				iconSize;	// iocn��С

		tGoodsTileDesc()
		{
			memset(this, 0, sizeof(this));
		}
	};

	// ��ʾ��Ϣ
	class CBagTipsWnd : public WindowImplBase
	{
	public:
		CBagTipsWnd();
		~CBagTipsWnd();

		BOOL CreateFrame(CPaintManagerUI* pManager);			// ��������
		void FillGoodsData(tagPropsInfo* pProps);				// �����Ʒ����
		SIZE EstimateSize(SIZE szAvailable);					// ���㴰�ڴ�С
		void ResizeShow(RECT rc);								// ������ʾ����
		void InitWindow();

	protected:
		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		CDuiString GetSkinFolder();
		CDuiString GetSkinFile();
		LPCTSTR GetWindowClassName(void) const;

	private:
		SIZE		m_cxyFixed;									// ����ؼ�����С
		TCHAR		m_pzBuffer[MAX_PATH];						// �ַ���������
		CTextUI*	m_pTxtGoodsName;							// ��Ʒ��
		CLabelUI*	m_pLabGoodsIcon;							// ��Ʒͼ��
		CTextUI*	m_pTxtGoodsDesc;							// ��Ʒ����
		CTextUI*	m_pTxtGoodsPrice;							// ��Ʒ�۸�
	};

	// ���Ӳ���
	class CBagTileLayout : public CTileLayoutUI
	{
	public:
		CBagTileLayout();
		~CBagTileLayout();
	
	protected:

	private:

	};
	
	// ��Ʒ���ӿؼ�
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
	
	// �������ӿؼ�
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
		std::map<CDuiString, bool>		m_MenuCheckInfo;		// ����˵��ĵ�ѡ��ѡ��Ϣ
	};

} // namespace DuiLib

#endif //__UIBAGITEM_H__
