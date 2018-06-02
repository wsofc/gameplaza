#ifndef __FACESELFRAME_H__
#define __FACESELFRAME_H__

#pragma once

#include "UIFaceSel.h"

typedef std::function< void(int, int, LPCTSTR) > ProcFaceCtrlSel;

class FaceSelFrame : public WindowImplBase
{
public:
	FaceSelFrame();
	~FaceSelFrame();

public:
	void CreateFrame(HWND parenWnd, RECT& rcPos, CFaceSelList* lpFaceList, ProcFaceCtrlSel proCtrlSel);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND hWnd);
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	void SetFaceList(CFaceSelList * lpFaceList);
	int GetSelFaceId();
	int GetSelFaceIndex();
	tstring GetSelFaceFileName();

protected:
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("FaceSelFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	CFaceSelUI * m_pFaceCtrl;
	CFaceSelList * m_lpFaceList;
	int m_nSelFaceId;
	int m_nSelFaceIndex;
	tstring m_strSelFaceFileName;
	ProcFaceCtrlSel m_pFuncCtrlSel;
};

#endif	//__FACESELFRAME_H__