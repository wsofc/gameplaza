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

	// ���ñ���ͼƬ(lPicֻ��ΪͼƬ·��,���ܰ������������ַ���)
	void SetPicture(LPCTSTR lPic);
	void PaintPicture(HDC hDC);

	//Ϊָ���ؼ�����һ��λͼ
	void CreateThumbnailBitmap(LPCTSTR controlName, LPCTSTR bitmapName);

	//����.bmp�ļ���ָ��Ŀ¼���
	BOOL SaveBitmapToFile(HBITMAP hBitmap, LPTSTR lpFileName);

	//����ָ������λͼ������λͼ�����Կ�ؼ��У�
	void DrawThumbnailBitmap(HDC hDC);

	//�������ֵ��
	void Reset();

private:

	void _InitData();			// ��ʼ���������
	void _InitShotBoxBitmap();	// ��ʼ������λͼ
	void _InitPictrue();		// ���䱳��ͼ��������(���ڲü�)

protected:
	BOOL								m_bFirst;
	int									m_iCursor;
	bool								m_bMouseEnter;							// ����Ƿ��ڿؼ���
	int									m_iMinWidth;							// ��׼����С���
	POINT								m_ptLastMouse;							// ����ϴμ�¼������
	UINT								m_uButtonState;
	POINT								m_ptClipBasePoint;						// ��׼��
	RECT								m_rcClipPadding;						// ��׼��
	RECT								m_rcOffset;								// ����(�ֱ����Ӧ�ߵľ���)
	TCHAR								m_sClipDrawStringBuf[MAX_PATH];
	HWND								m_hParentWnd;
	unsigned char*						m_Bits;									//λͼ������ݻ�����
	int									m_ClipBorderSize;						//��׼��߿�Ŀ��
	CDuiString							m_pic;
	Gdiplus::RectF						m_rcPic;
	std::map<LPCTSTR, TImageInfo*>		m_thumbnailMap;							//��Ӧ���Կ�ؼ������Ϣ����<�ؼ���, �ؼ�λͼ�ṹ��Ϣ>
};

} // namespace DuiLib

#endif //__UISCREENSHOTBOX_H__
