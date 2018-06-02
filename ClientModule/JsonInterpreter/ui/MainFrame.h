#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

class MainFrame : public WindowImplBase
{
public:
	MainFrame();
	~MainFrame();
	BOOL CreateFrame();
	
public:
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

	void InitData(TNotifyUI& msg);
	void Encrypt();
	void Decrypt();

	/*@breif 文件打开对话框
	*@param lpstrFilter	过滤字符串
	*@param hwndOwner	父窗口句柄
	*@param fileNames	完整文件路径
	*@param isMulti		是否标记选择多个文件
	*/
	bool openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti = false);

	/*@breif 文件保存对话框
	*@param hwndOwner		父窗口句柄
	*@param lpstrFilter		过滤字符串
	*@param lpszDefExt		扩展名
	*@param lpszTitle		标题
	*@param lpOutFilePath	输出的文件路径
	*/
	bool saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath);

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("MainFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	// 设置编辑框文本
	void _SetEdtText(CRichEditUI* pEdt, std::vector<CDuiString>& vNames);

	// 密文解析(true 明文解析, false 密文解析)
	void _Parse(bool bExpress);

public:
	static LPCTSTR g_kBtn_Close;
	static LPCTSTR g_kEdt_Encrypt;
	static LPCTSTR g_kBtn_Encrypt;
	static LPCTSTR g_kEdt_Decrypt;
	static LPCTSTR g_kBtn_Decrypt;
	static LPCTSTR g_kBtn_Encrypt_Look;
	static LPCTSTR g_kBtn_Decrypt_Look;

private:

	TCHAR				m_pzBuffer[MAX_PATH];	// 字符串缓冲区
	CRichEditUI*		m_pEdtEncrypt;			// 加密编辑框
	CRichEditUI*		m_pEdtDecrypt;			// 解密编辑框
};
#endif //__MAINFRAME_H__