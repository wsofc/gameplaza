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

	/*@breif �ļ��򿪶Ի���
	*@param lpstrFilter	�����ַ���
	*@param hwndOwner	�����ھ��
	*@param fileNames	�����ļ�·��
	*@param isMulti		�Ƿ���ѡ�����ļ�
	*/
	bool openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti = false);

	/*@breif �ļ�����Ի���
	*@param hwndOwner		�����ھ��
	*@param lpstrFilter		�����ַ���
	*@param lpszDefExt		��չ��
	*@param lpszTitle		����
	*@param lpOutFilePath	������ļ�·��
	*/
	bool saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath);

protected:

	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const{ return _T("MainFrame"); };
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:

	// ���ñ༭���ı�
	void _SetEdtText(CRichEditUI* pEdt, std::vector<CDuiString>& vNames);

	// ���Ľ���(true ���Ľ���, false ���Ľ���)
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

	TCHAR				m_pzBuffer[MAX_PATH];	// �ַ���������
	CRichEditUI*		m_pEdtEncrypt;			// ���ܱ༭��
	CRichEditUI*		m_pEdtDecrypt;			// ���ܱ༭��
};
#endif //__MAINFRAME_H__