#ifndef __LOADINGFRAME_H__
#define __LOADINGFRAME_H__

class LoadingFrame : public WindowImplBase
{

public:
	LoadingFrame();
	~LoadingFrame();

	void OnFinalMessage(HWND hWnd);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void InitWindow();
	void Notify(TNotifyUI& msg);

	void AddPercent();
	void SetPercent(int iPercent);

protected:

	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	LPCTSTR GetWindowClassName(void) const{ return _T("LoadingFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	CProgressUI* m_progress;
	UINT m_iStep;
	UINT m_iRatio;
};

#endif //__LOADINGFRAME_H__