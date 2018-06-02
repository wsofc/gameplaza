#ifndef __UICONTAINERGDIPLUS_H__
#define __UICONTAINERGDIPLUS_H__

#define  DUI_CTR_CONTROLGDIPLUS			(_T("ControlGdiplus"))

namespace DuiLib
{
	class CControlGdiplusUI : public CControlUI
	{
	public:
		CControlGdiplusUI();
		~CControlGdiplusUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		bool RActivate();

		void BindTabLayoutName(LPCTSTR _TabLayoutName);
		void BindTriggerTabSel(int _SetSelectIndex = -1);

		void DoEvent(TEventUI& event);
		void PaintBkImage(HDC hDC);

		void SetEnabled(bool bEnable = true);

		//重置相关按钮状态(覆盖CControl虚方法)
		void RestState(UINT state = 0);

	private:
		UINT m_uButtonState;
	};
} // namespace DuiLib

#endif //__UISCREENSHOTBOX_H__
