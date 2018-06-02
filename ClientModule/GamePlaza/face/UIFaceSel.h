#ifndef __UIFACESEL_H__
#define __UIFACESEL_H__

#pragma once

#include "UIFaceSel.h"

namespace DuiLib {

class CFaceSelUI : public CControlUI
{
public:
	CFaceSelUI(void);
	virtual ~CFaceSelUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetLineColor(COLORREF color);
	void SetFocusBorderColor(COLORREF color);
	void SetZoomBorderColor(COLORREF color);
	void SetRowAndCol(int nRow, int nCol);
	void SetItemSize(int nWidth, int nHeight);
	void SetZoomSize(int nWidth, int nHeight);
	void SetFaceList(CFaceSelList * lpFaceList);
	void SetCurPage(int nPageIndex);

	CFaceInfo * GetFaceInfo(int nItemIndex);

	//SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintBkColor(HDC hDC);
	void PaintBkImage(HDC hDC);
	void PaintStatusImage(HDC hDC);

private:
	void CalcCenterRect(RECT& rcDest, int cx, int cy, RECT& rcCenter);
	int HitTest(POINT pt);
	void CalcZoomRect(POINT point);
	BOOL GetItemRect(int nItemIndex, RECT& rect);
	CGifImage * GetZoomImage();
	void DrawLine(HDC hDC);
	void DrawFocusBorder(HDC hDC, const RECT& rect);
	void DrawZoomImage(HDC hDC);

	BOOL LoadImage(int nPageIndex);
	void DestroyImage();

private:
	COLORREF m_clrLine;
	COLORREF m_clrFocusBorder;
	COLORREF m_clrZoomBorder;
	int m_nRow, m_nCol;
	int m_nItemWidth, m_nItemHeight;
	int m_nZoomWidth, m_nZoomHeight;
	int m_nCurPage, m_nPageCnt;
	int m_nHoverIndex;
	int m_nFramePos;
	RECT m_rcZoomImg;
	CFaceSelList * m_lpFaceList;
	std::vector<CGifImage *> m_arrImage;
};

} // namespace DuiLib

#endif	//__UIFACESEL_H__