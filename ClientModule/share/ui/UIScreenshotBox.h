#ifndef __UISCREENSHOTBOX_H__
#define __UISCREENSHOTBOX_H__

#define  DUI_CTR_SCREENSHOTBOX			(_T("ScreenshotBox"))

namespace DuiLib {

class CScreenshotBoxUI : public CContainerUI
{
public:
	CScreenshotBoxUI(HWND hParentWnd);
	~CScreenshotBoxUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	// 0    1    2
	// 3    4    5
	// 6    7    8
	RECT GetSizerRect(int iIndex);
	LPTSTR GetSizerCursor(POINT& pt, int& iCursor);
	void PaintBorder(HDC hDC);
	void DoEvent(TEventUI& event);
	
	RECT GetClipPadding() const;
	void SetClipPadding(RECT rc);

	// 设置背景图片(lPic只能为图片路径,不能包含其他属性字符串)
	void SetPicture(LPCTSTR lPic);
	void PaintPicture(HDC hDC);

	//为指定控件创建一张位图
	void CreateThumbnailBitmap(LPCTSTR controlName, LPCTSTR bitmapName);

	//保存.bmp文件至指定目录输出
	BOOL SaveBitmapToFile(HBITMAP hBitmap, LPTSTR lpFileName);

	//绘制指定区域位图（拷贝位图至缩略框控件中）
	void DrawThumbnailBitmap(HDC hDC);

	//重置相关值等
	void Reset();

private:

	void _InitData();			// 初始化相关数据
	void _InitShotBoxBitmap();	// 初始化朦层位图
	void _InitPictrue();		// 适配背景图绘制坐标(用于裁剪)

protected:
	BOOL								m_bFirst;
	int									m_iCursor;
	bool								m_bMouseEnter;							// 鼠标是否在控件内
	int									m_iMinWidth;							// 基准匡最小宽度
	POINT								m_ptLastMouse;							// 鼠标上次记录的坐标
	UINT								m_uButtonState;
	POINT								m_ptClipBasePoint;						// 基准点
	RECT								m_rcClipPadding;						// 基准框
	RECT								m_rcOffset;								// 间距框(分别与对应边的距离)
	TCHAR								m_sClipDrawStringBuf[MAX_PATH];
	HWND								m_hParentWnd;
	unsigned char*						m_Bits;									//位图句柄数据缓冲区
	int									m_ClipBorderSize;						//基准框边框的宽度
	CDuiString							m_pic;
	Gdiplus::RectF						m_rcPic;
	std::map<LPCTSTR, TImageInfo*>		m_thumbnailMap;							//对应缩略框控件相关信息容器<控件名, 控件位图结构信息>
};

} // namespace DuiLib

#endif //__UISCREENSHOTBOX_H__
