#include "stdafx.h"
#include "UIScreenshotBox.h"

#define MAX_BITS	1024*1024*4

namespace DuiLib {

CScreenshotBoxUI::CScreenshotBoxUI(HWND hParentWnd)
	: m_iCursor(0)
	, m_uButtonState(0)
	, m_ClipBorderSize(0)
	, m_bFirst(TRUE)
	, m_hParentWnd(hParentWnd)
	, m_Bits(nullptr)
	, m_iMinWidth(0)
	, m_bMouseEnter(false)
{
	m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
	m_ptLastMouse.x = m_ptLastMouse.y = 0;
	m_rcOffset = { 0, 0, 0, 0 };
	m_rcClipPadding = { 0, 0, 0, 0 };
	
	memset(m_sClipDrawStringBuf, 0, ArrayCount(m_sClipDrawStringBuf) * sizeof(TCHAR));
	
	m_Bits = new unsigned char[MAX_BITS];
	memset(m_Bits, 0, MAX_BITS * sizeof(unsigned char));

	m_rcPic = { 0, 0, 0, 0 };
}

CScreenshotBoxUI::~CScreenshotBoxUI()
{
	SAFE_DELETE_ARRAY(m_Bits);

	if (!m_thumbnailMap.empty())
	{
		for (auto it : m_thumbnailMap)
		{
			SAFE_DELETE(it.second);
		}
		m_thumbnailMap.clear();
	}
}

LPCTSTR CScreenshotBoxUI::GetClass() const
{
	return DUI_CTR_SCREENSHOTBOX;
}

LPVOID CScreenshotBoxUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, DUI_CTR_SCREENSHOTBOX) == 0) return static_cast<CScreenshotBoxUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

UINT CScreenshotBoxUI::GetControlFlags() const
{
	return UIFLAG_SETCURSOR;
}

void CScreenshotBoxUI::SetPicture(LPCTSTR lPic)
{
	m_pic = lPic;
}

void CScreenshotBoxUI::CreateThumbnailBitmap(LPCTSTR controlName, LPCTSTR bitmapName)
{
	if (!controlName || _tcscmp(controlName, _T("")) == 0)
		return;

	CPaintManagerUI* pManager = this->GetManager();
	CControlUI* pControl = pManager->FindControl(controlName);

	if (!pControl)
		return;
	
	RECT rcItem = pControl->GetPos();
	int nW = rcItem.right - rcItem.left;
	int nH = rcItem.bottom - rcItem.top;
	HBITMAP hBitmap = CRenderEngine::GenerateBitmap(pManager, pControl, rcItem);

	pManager->AddImage(bitmapName, hBitmap, nW, nH, true);
	const TImageInfo* pTemp = pManager->GetImageEx(bitmapName, 0, 0);

	TImageInfo* pImageInfo		= new TImageInfo;
	pImageInfo->alphaChannel	= pTemp->alphaChannel;
	pImageInfo->dwMask			= pTemp->dwMask;
	pImageInfo->hBitmap			= pTemp->hBitmap;
	pImageInfo->nX				= pTemp->nX;
	pImageInfo->nY				= pTemp->nY;
	pImageInfo->sResType		= pTemp->sResType;
	m_thumbnailMap[controlName] = pImageInfo;
}

void CScreenshotBoxUI::DrawThumbnailBitmap(HDC hDC)
{
	if (m_thumbnailMap.empty())
		return;

	CPaintManagerUI* pManager = this->GetManager();
	for (auto it : m_thumbnailMap)
	{
		RECT pos = this->GetPos();

		CControlUI* control = pManager->FindControl(it.first);
		RECT rcItem = control->GetPos();
		int x = 0;
		int y = 0;
		int w = rcItem.right - rcItem.left;
		int h = rcItem.bottom - rcItem.top;
		
		HBITMAP h_cur_bitmap = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);
		Gdiplus::Bitmap cur_bitmap(h_cur_bitmap, NULL);

		Gdiplus::Bitmap bmp(it.second->hBitmap, NULL);
		Gdiplus::Graphics graphics(&bmp);

		Gdiplus::RectF rectF(0, 0, w, h);
		graphics.DrawImage(&cur_bitmap
			, rectF
			, m_rcClipPadding.left + 2 * m_ClipBorderSize
			, m_rcClipPadding.top + 2 * m_ClipBorderSize
			, m_rcClipPadding.right - m_rcClipPadding.left - 4 * m_ClipBorderSize
			, m_rcClipPadding.bottom - m_rcClipPadding.top - 4 * m_ClipBorderSize
			, Gdiplus::UnitPixel);

		HBITMAP hBitmap = NULL;
		bmp.GetHBITMAP(NULL, &hBitmap);

		DWORD dwSize = bmp.GetHeight() * bmp.GetWidth() * 4;
		memset(m_Bits, 0, MAX_BITS * sizeof(unsigned char));
		LONG dl = GetBitmapBits(hBitmap, dwSize, m_Bits);
		::SetBitmapBits(it.second->hBitmap, dwSize, m_Bits);

		::DeleteObject(hBitmap);

		control->SetBkImage(it.first);
		control->Invalidate();
	}
}

void CScreenshotBoxUI::Reset()
{
	m_bFirst	= true;
	m_pic		= _T("");
	m_rcPic		= { 0, 0, 0, 0 };
	
	memset(m_sClipDrawStringBuf, 0, ArrayCount(m_sClipDrawStringBuf) * sizeof(TCHAR));
	memset(m_Bits, 0, MAX_BITS * sizeof(unsigned char));

	if (!m_thumbnailMap.empty())
	{
		for (auto it : m_thumbnailMap)
		{
			SAFE_DELETE(it.second);
		}
		m_thumbnailMap.clear();
	}

	this->SetVisible(false);
}

BOOL CScreenshotBoxUI::SaveBitmapToFile(HBITMAP hBitmap, LPTSTR lpFileName)
{
	HDC hDC;                        // 设备描述表  

	int iBits;                      // 当前显示分辨率下每个像素所占字节数  
	WORD wBitCount;                 // 位图中每个像素所占字节数  
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;    // 调色板大小，位图数据大小，位图文件大小，写入文件字节数  
	BITMAP Bitmap;                  //位图属性结构  
	BITMAPFILEHEADER bmfHdr;        // 位图文件头  
	BITMAPINFOHEADER bi;            // 位图信息头  
	LPBITMAPINFOHEADER lpbi;        // 指向位图信息头结构  

	HANDLE fh, hDib;                // 定义文件，分配内存句柄  
	HPALETTE hPal, hOldPal = NULL;    // 调色板句柄  

	// 计算位图文件每个像素所占字节数  
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 32;
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);       // 计算调色板大小  

	// 设置位图信息头结构  
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  // 为位图内容分配内存  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板  
	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, hPal, FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值  
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	if (hOldPal)                // 恢复调色板  
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	// 创建位图文件   
	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	// 设置位图文件头  
	bmfHdr.bfType = 0x4D42;     // 文件类型: "BM"  
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;  // 位图文件大小  
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  // 写入位图文件头  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);                    // 写入位图文件其余内容  

	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}

void CScreenshotBoxUI::_InitShotBoxBitmap()
{
	DWORD color = 0x4F000000;
	HDC hDCMem = CreateCompatibleDC(NULL);
	HDC hDC = GetDC(this->m_hParentWnd);
	RECT rect = { 0 };
	rect.right = this->m_rcItem.right - this->m_rcItem.left;
	rect.bottom = this->m_rcItem.bottom - this->m_rcItem.top;
	HBITMAP hBmp = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);

	ReleaseDC(this->m_hParentWnd, hDC);

	HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
	CRenderEngine::DrawColor(hDCMem, rect, color);

	SelectObject(hDCMem, hOld);
	DeleteObject(hDCMem);

	//镂空绘制初始化
	LPCTSTR bitmapName = _T("shotboxbitmap");
	CPaintManagerUI* paintManager = this->GetManager();
	paintManager->AddImage(bitmapName, hBmp, rect.right, rect.bottom, true);
	_stprintf_s(m_sClipDrawStringBuf
		, _T("file='%s' corner='%d,%d,%d,%d' hole='false'") \
		, bitmapName \
		, m_rcOffset.left \
		, m_rcOffset.top \
		, m_rcOffset.right \
		, m_rcOffset.bottom);

	this->SetBkImage(m_sClipDrawStringBuf);
}

void CScreenshotBoxUI::_InitPictrue()
{
	CPaintManagerUI* pManager = this->GetManager();
	const TImageInfo* pImageInfo = pManager->GetImageEx(m_pic.GetData(), 0, 0);
	if (!pImageInfo)
	{
		this->Reset();

		//此处可加入提示对话框
		// ...

		return;
	}

	float iw = m_rcItem.right - m_rcItem.left;
	float ih = m_rcItem.bottom - m_rcItem.top;

	// 等比缩放
	if (iw > 0 && ih > 0)
	{
		float scale = 1.0;
		float sx = pImageInfo->nX / iw;
		float sy = pImageInfo->nY / ih;
		sx = 1 / sx;
		sy = 1 / sy;
		scale = min(sx, sy);

		float pw = pImageInfo->nX * scale;
		float ph = pImageInfo->nY * scale;
		m_rcPic.X = m_rcItem.left + (iw - pw) / 2;
		m_rcPic.Y = m_rcItem.top + (ih - ph) / 2;
		m_rcPic.Width = pw;
		m_rcPic.Height = ph;
	}
}

void CScreenshotBoxUI::_InitData()
{
	long w = m_rcItem.right - m_rcItem.left;
	long h = m_rcItem.bottom - m_rcItem.top;
	float fInitScale = 1 / 5.0;
	float fMinScale = (1 / 3.0);
	long iMin = min(w, h);

	m_rcOffset.left 
		= m_rcOffset.top
		= m_rcOffset.right
		= m_rcOffset.bottom
		= iMin * fInitScale;

	//初始化裁剪框
	m_rcClipPadding.left = m_rcItem.left + m_rcOffset.left;
	m_rcClipPadding.right = m_rcItem.right - m_rcOffset.right;
	m_rcClipPadding.top = m_rcItem.top + m_rcOffset.top;
	m_rcClipPadding.bottom = m_rcItem.bottom - m_rcOffset.bottom;

	//初始化裁剪框最小宽度
	m_iMinWidth = (m_rcClipPadding.right - m_rcClipPadding.left) * fMinScale;


	//初始化朦层位图
	this->_InitShotBoxBitmap();

	//适配背景图绘制坐标(用于裁剪)
	this->_InitPictrue();
}

RECT CScreenshotBoxUI::GetSizerRect(int iIndex)
{
	RECT rect = m_rcClipPadding;
	LONG lMiddleX = (rect.left + rect.right) / 2;
	LONG lMiddleY = (rect.top + rect.bottom) / 2;
	LONG SIZER_WIDTH = m_rcInset.left * 2;
	switch (iIndex) {
		case 0:
			return CDuiRect(rect.left, rect.top, rect.left + SIZER_WIDTH, rect.top + SIZER_WIDTH);
		case 1:
			return CDuiRect(lMiddleX - SIZER_WIDTH / 2, rect.top, lMiddleX + SIZER_WIDTH / 2, rect.top + SIZER_WIDTH);
		case 2:
			return CDuiRect(rect.right - SIZER_WIDTH, rect.top, rect.right, rect.top + SIZER_WIDTH);
		case 3:
			return CDuiRect(rect.left, lMiddleY - SIZER_WIDTH / 2, rect.left + SIZER_WIDTH, lMiddleY + SIZER_WIDTH / 2);
		case 4:
			break;
		case 5:
			return CDuiRect(rect.right - SIZER_WIDTH, lMiddleY - SIZER_WIDTH / 2, rect.right, lMiddleY + SIZER_WIDTH / 2);
		case 6:
			return CDuiRect(rect.left, rect.bottom - SIZER_WIDTH, rect.left + SIZER_WIDTH, rect.bottom);
		case 7:
			return CDuiRect(lMiddleX - SIZER_WIDTH / 2, rect.bottom - SIZER_WIDTH, lMiddleX + SIZER_WIDTH / 2, rect.bottom);
		case 8:
			return CDuiRect(rect.right - SIZER_WIDTH, rect.bottom - SIZER_WIDTH, rect.right, rect.bottom);
		default:
			break;
	}
	return CDuiRect();
}

LPTSTR CScreenshotBoxUI::GetSizerCursor(POINT& pt, int& iCursor)
{
	LONG SIZER_WIDTH = m_rcInset.left * 2;
	LONG SIZER_TO_ROOT = 20;
	RECT rcRoot = m_pManager->GetRoot()->GetPos();

	iCursor = -1;
	for (int i = 8; i >= 0; --i) {
		RECT rc = GetSizerRect(i);
		if (::PtInRect(&rc, pt)) {
			iCursor = i;
			break;
		}
	}

	if (iCursor < 0) {
		RECT rc = CDuiRect(m_rcClipPadding.left, m_rcClipPadding.top, m_rcClipPadding.right, m_rcClipPadding.bottom);
		if (::PtInRect(&rc, pt)) {
			iCursor = 4;
		}
	}

	switch (iCursor) {
		case 0:
			return IDC_SIZENWSE;
		case 1:
			return IDC_SIZENS;
		case 2:
			return IDC_SIZENESW;
		case 3:
			return IDC_SIZEWE;
		case 4:
			return IDC_SIZEALL;
		case 5:
			return IDC_SIZEWE;
		case 6:
			return IDC_SIZENESW;
		case 7:
			return IDC_SIZENS;
		case 8:
			return IDC_SIZENWSE;
		default:
			return IDC_ARROW;
	}
}

void CScreenshotBoxUI::PaintPicture(HDC hDC)
{
	CPaintManagerUI* paintManager = this->GetManager();
	const TImageInfo* pImageInfo = paintManager->GetImageEx(m_pic.GetData(), 0, 0);
	if (pImageInfo)
	{
		Gdiplus::Bitmap bmp(pImageInfo->hBitmap, NULL);
		Gdiplus::Graphics graphics(hDC);
		graphics.DrawImage(&bmp, m_rcPic);
	}
}

void CScreenshotBoxUI::PaintBorder(HDC hDC)
{
	if (m_bFirst)
	{
		m_bFirst = FALSE;
		this->_InitData();
	}

	this->PaintPicture(hDC);

	for (int i = 0; i < 9; ++i)
		CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);

	RECT rc = { 0 };
	rc.left = m_rcClipPadding.left + m_rcInset.left;
	rc.top = m_rcClipPadding.top + m_rcInset.top;
	rc.right = m_rcClipPadding.right - m_rcInset.right;
	rc.bottom = m_rcClipPadding.bottom - m_rcInset.bottom;

	m_ClipBorderSize = (m_rcInset.left + m_rcInset.right + m_rcInset.top + m_rcInset.bottom) / 4;
	CRenderEngine::DrawRect(hDC, m_rcClipPadding, m_ClipBorderSize, m_dwBorderColor);

	this->DrawThumbnailBitmap(hDC);
}

RECT CScreenshotBoxUI::GetClipPadding() const
{
	return m_rcClipPadding;
}

void CScreenshotBoxUI::SetClipPadding(RECT rc)
{
	m_rcClipPadding.left	= m_rcItem.left + m_rcOffset.left;
	m_rcClipPadding.right	= m_rcItem.right - m_rcOffset.right;
	m_rcClipPadding.top		= m_rcItem.top + m_rcOffset.top;
	m_rcClipPadding.bottom	= m_rcItem.bottom - m_rcOffset.bottom;

	_stprintf_s(m_sClipDrawStringBuf
		, _T("file='shotboxbitmap' corner='%d,%d,%d,%d' hole='false'") \
		, m_rcOffset.left \
		, m_rcOffset.top \
		, m_rcOffset.right \
		, m_rcOffset.bottom);
	this->SetBkImage(m_sClipDrawStringBuf);
	this->Invalidate();
}

void CScreenshotBoxUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0) return;
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
		return;
	}
	if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
	{
		m_uButtonState |= UISTATE_CAPTURED;
		m_ptLastMouse = event.ptMouse;

		switch (m_iCursor) {
			case 0:
			case 1:
			case 3:
				m_ptClipBasePoint.x = m_rcItem.right - m_rcOffset.right;
				m_ptClipBasePoint.y = m_rcItem.bottom - m_rcOffset.bottom;
				break;
			case 2:
			case 5:
				m_ptClipBasePoint.x = m_rcItem.left + m_rcOffset.left;
				m_ptClipBasePoint.y = m_rcItem.bottom - m_rcOffset.bottom;
				break;
			case 6:
			case 7:
				m_ptClipBasePoint.x = m_rcItem.right - m_rcOffset.right;
				m_ptClipBasePoint.y = m_rcItem.top + m_rcOffset.top;
				break;
			case 8:
			default:
				m_ptClipBasePoint.x = m_rcItem.left + m_rcOffset.left;
				m_ptClipBasePoint.y = m_rcItem.top + m_rcOffset.top;
				break;
		}
		return;
	}
	if (event.Type == UIEVENT_BUTTONUP)
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
			m_uButtonState &= ~UISTATE_CAPTURED;
		}
		return;
	}
	else if (event.Type == UIEVENT_MOUSEENTER)
	{
		m_bMouseEnter = true;
	}
	else if (event.Type == UIEVENT_MOUSELEAVE)
	{
		m_bMouseEnter = false;
	}
	else if (event.Type == UIEVENT_MOUSEMOVE)
	{
		if (!m_bMouseEnter || (m_uButtonState & UISTATE_CAPTURED) == 0) return;
		
		//0	 1  2
		//3  4  5
		//6  7  8
		switch (m_iCursor) {
			//左上角顶点
			case 0:
			{
				long iDis = event.ptMouse.x - m_ptLastMouse.x;
				if (iDis > 0)	//向右拉
				{
					long iLimit = m_rcClipPadding.right - m_iMinWidth;
					if (m_rcClipPadding.left + iDis <= iLimit)
					{
						m_rcOffset.left += iDis;
						m_rcOffset.top += iDis;
					}
				}
				else
				{
					long iLimit = min(m_rcOffset.left, m_rcOffset.top);
					if (abs(iDis) < iLimit)
					{
						m_rcOffset.top += iDis;
						m_rcOffset.left += iDis;
					}
				}
			}break;
			
			//右上角顶点
			case 2:
			{
				long iDis = event.ptMouse.x - m_ptLastMouse.x;
				if (iDis > 0)	//向右拉
				{
					long iLimit = min(m_rcOffset.right, m_rcOffset.top);
					if (iDis < iLimit)
					{
						m_rcOffset.top -= iDis;
						m_rcOffset.right -= iDis;
					}
				}
				else
				{
					long iLimit = m_rcClipPadding.left + m_iMinWidth;
					if (m_rcClipPadding.right + iDis >= iLimit)
					{
						m_rcOffset.right -= iDis;
						m_rcOffset.top -= iDis;
					}
				}
			}break;
			
			//左下角顶点
			case 6:
			{
				long iDis = event.ptMouse.x - m_ptLastMouse.x;
				if (iDis > 0)	//向右拉
				{
					long iLimit = m_rcClipPadding.right - m_iMinWidth;
					if (m_rcClipPadding.left + iDis <= iLimit)
					{
						m_rcOffset.left += iDis;
						m_rcOffset.bottom += iDis;
					}
				}
				else
				{
					long iLimit = min(m_rcOffset.left, m_rcOffset.bottom);
					if (abs(iDis) < iLimit)
					{
						m_rcOffset.bottom += iDis;
						m_rcOffset.left += iDis;
					}
				}
			}break;
			
			//右下角顶点
			case 8:
			{
				long iDis = event.ptMouse.x - m_ptLastMouse.x;
				if (iDis > 0)	//向右拉
				{
					long iLimit = min(m_rcOffset.right, m_rcOffset.bottom);
					if (iDis < iLimit)
					{
						m_rcOffset.bottom -= iDis;
						m_rcOffset.right -= iDis;
					}
				}
				else
				{
					long iLimit = m_rcClipPadding.left + m_iMinWidth;
					if (m_rcClipPadding.right + iDis >= iLimit)
					{
						m_rcOffset.right -= iDis;
						m_rcOffset.bottom -= iDis;
					}
				}
			}break;
			
			//上边中点
			case 1:
			{
				long iDis = 2 * (event.ptMouse.y - m_ptLastMouse.y);
				if (iDis > 0)	//向下拉
				{
					long iLimit = m_rcClipPadding.bottom - m_iMinWidth;

					if (m_rcClipPadding.top + iDis <= iLimit)
					{
						m_rcOffset.top += iDis;
						m_rcOffset.left += iDis / 2.0;
						m_rcOffset.right += iDis / 2.0;
					}
				}
				else
				{
					long iLimit = min(m_rcOffset.left, m_rcOffset.right);
					iLimit = min(iLimit, m_rcOffset.top);
					if (abs(iDis) < iLimit)
					{
						m_rcOffset.top += iDis;
						m_rcOffset.left += iDis / 2.0;
						m_rcOffset.right += iDis / 2.0;
					}
				}
			}break;
			
			//下边中点
			case 7:
			{
				long iDis = 2 * (event.ptMouse.y - m_ptLastMouse.y);
				if (iDis > 0)	//向下拉
				{
					long iLimit = min(m_rcOffset.left, m_rcOffset.right);
					iLimit = min(iLimit, m_rcOffset.bottom);
					if (iDis < iLimit)
					{
						m_rcOffset.bottom -= iDis;
						m_rcOffset.left -= iDis / 2.0;
						m_rcOffset.right -= iDis / 2.0;
					}
				}
				else
				{
					long iLimit = m_rcClipPadding.top + m_iMinWidth;
					if (m_rcClipPadding.bottom + iDis >= iLimit)
					{
						m_rcOffset.bottom -= iDis;
						m_rcOffset.left -= iDis / 2.0;
						m_rcOffset.right -= iDis / 2.0;
					}
				}
			}break;
			
			//左边中点
			case 3:
			{
				long iDis = 2 * (event.ptMouse.x - m_ptLastMouse.x);
				if (iDis > 0)	//向右拉
				{
					long iLimit = m_rcClipPadding.right - m_iMinWidth;
					if (m_rcClipPadding.left + iDis <= iLimit)
					{
						m_rcOffset.left += iDis;
						m_rcOffset.top += iDis / 2;
						m_rcOffset.bottom += iDis / 2;
					}
				}
				else
				{
					long iLimit = min(m_rcOffset.top, m_rcOffset.bottom);
					iLimit = min(iLimit, m_rcOffset.left);
					if (abs(iDis) < iLimit)
					{
						m_rcOffset.left += iDis;
						m_rcOffset.top += iDis / 2;
						m_rcOffset.bottom += iDis / 2;
					}
				}
			}break;
			
			//右边中点
			case 5:
			{
				long iDis = 2 * (event.ptMouse.x - m_ptLastMouse.x);
				if (iDis > 0)	//向右拉
				{
					long iLimit = min(m_rcOffset.top, m_rcOffset.bottom);
					iLimit = min(iLimit, m_rcOffset.right);
					if (iDis < iLimit)
					{
						m_rcOffset.right -= iDis;
						m_rcOffset.top -= iDis / 2;
						m_rcOffset.bottom -= iDis / 2;
					}
				}
				else
				{
					long iLimit = m_rcClipPadding.left + m_iMinWidth;
					if (m_rcClipPadding.right + iDis >= iLimit)
					{
						m_rcOffset.right -= iDis;
						m_rcOffset.top -= iDis / 2;
						m_rcOffset.bottom -= iDis / 2;
					}
				}
			}break;
			
			//其他点(移动)
			default:
			{
				LONG cx = event.ptMouse.x - m_ptLastMouse.x;
				LONG cy = event.ptMouse.y - m_ptLastMouse.y;
				if (m_rcOffset.left + cx >= 0 && m_rcOffset.right - cx >= 0) {
					m_rcOffset.left += cx;
					m_rcOffset.right -= cx;
				}
				if (m_rcOffset.top + cy >= 0 && m_rcOffset.bottom - cy >= 0) {
					m_rcOffset.top += cy;
					m_rcOffset.bottom -= cy;
				}
			}
			break;
		}
		
		m_ptLastMouse = event.ptMouse;
		this->SetClipPadding(m_rcOffset);
	}
	CContainerUI::DoEvent(event);
}

} //namespace DuiLib
