#pragma once

#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
#include "gameResource.h"						// ��Ϸ��Դ��
#include "trayMenu.h"							// ���̲˵�
#include "gifImage.h"							// gifͼ֧��
#include "richEditUtil.h"						// �༭�����ӿ�

#include "..\\data\\dataManager.h"				// �������ݹ�����
#include "..\\ipc\\ipc.h"						// ipcͨ��
#include "..\\room\\roomManager.h"
#include "..\\room\\userList.h"

#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

struct FILE_STRUCT
{
	FILE_STRUCT()
	{
		memset(this, 0, sizeof(*this));
	}

	tstring		fileName;	//�ļ�ȫ��(�ļ��� + ��׺��)
	tstring		name;		//�ļ���
	tstring		exName;		//��׺��
	int			size;		//��С
};

enum MatchWords_Type
{
	REG_TYPE_NULL			//��
	, REG_TYPE_ACCOUNT		//�˺�
	, REG_TYPE_NICKNAME		//�ǳ�
	, REG_TYPE_PASSWORD		//����
	, REG_TYPE_TELPHONE		//�ֻ���
	, REG_TYPE_VERTCODE		//��֤��
	, REG_TYPE_REALNAME		//��ʵ����
	, REG_TYPE_EMAIL		//����
	, REG_TYPE_QQ_NUMBER	//QQ����
	, REG_TYPE_QUESTION		//�ܱ�����
	, REG_TYPE_ANSWER		//�ܱ���
	, REG_TYPE_IDCARD		//���֤
	, REG_TYPE_UNDERWRITE	//����˵��
	, REG_TYPE_UNREAL		//unreal
	, REG_TYPE_EXTRA		//Ԥ��
};

//���ö��
enum Tls_Panel
{
	TLS_NONE,				//��
	TLS_PANEL_HALL,			//�����
	TLS_PANEL_ROOM_LIST,	//�����б����
	TLS_PANEL_ROOM,			//�������
	TLS_PANEL_EXTRA			//Ԥ����
};

//����ö��
enum Tls_SideBar
{
	TLS_SIDEBAR_HALL,		//������
	TLS_SIDEBAR_ROOM_LIST,	//�������
	TLS_SIDEBAR_ROOM,		//���Ӳ���
	TLS_SIDEBAR_EXTRA		//Ԥ����
};

struct stuUnFoucsWnd
{
	stuUnFoucsWnd()
	{
		memset(this, 0, sizeof(*this));
	}

	HWND	hWnd;			//ʧȥ����Ĵ��� 
	DWORD	dwActOffset;	//�˶�����
	DWORD	dwActCount;		//�˶�����
	DWORD	dwActTimes;		//�˶�����
	RECT	rectSrc;		//��ʼλ��
};

class GameUtil
{
public:

	static GameUtil* GetInstance();
	static int WINAPI WriteWebBrowserRegKey(LPCTSTR lpKey, DWORD dwValue, tstring& strOutInfo);

	~GameUtil();

	bool SetClipBoardText(LPCSTR text, HWND hWnd);								//���ü������ı�����
	tstring GetClipBoardText(HWND hWnd);										//��ȡ�������ı�����

	bool MatchWordByType(CDuiString& strIn, DWORD dwType);						//����ƥ��ע����Ϣ
	bool MatchRegular(CDuiString& strIn, LPCTSTR lpReg);

	void MinimizeTraySaveControl(CControlUI* control = NULL);					//������С��������ʱҪ�ָ�״̬�Ŀؼ�

	void MinimizeTray(HWND hWnd);												//��С��(��ʱ�ر�) ������

	void SetFrozenEnable(CPaintManagerUI* pManager, bool bEnable);				//������ڵ���������Ϣ

	void ShowTopWindow(HWND hWnd, bool bShow = true, bool bTakeFocus = true);	//�ö���ʾ����

	void RestoreTopWindow(HWND hWnd);											//�ö���ԭ����

	void OpenWebByName(LPCTSTR lpName);											//��ָ�����Ƶ���ҳ

	void OpenWebByURL(LPCTSTR lpName);											//��ָ������ҳ

	LPCTSTR GetWebByName(LPCTSTR lpName);										//��ȡָ�����Ƶ���ҳurl

	void CreatTrayMenu(CPaintManagerUI* pParentManager);						//�������̲˵�
	
	//����ʧȥ���㶯��
	void RunWndUnFocusAction(HWND hWnd, DWORD dwActTimes = 3, DWORD dwActOffset = 5);

	//����ʧȥ���㶯���ص�
	void ProFuncWndUnFocus(UINT_PTR idEvent, DWORD dwTime);

	//ֹͣ����
	void StopWndUnFocusAction(HWND hWnd);
	
	//�������Ͳ�������
	void SetCurPanel(CPaintManagerUI* pManager
		, Tls_Panel tls_panel = Tls_Panel::TLS_PANEL_HALL
		, Tls_SideBar tls_sidebar = Tls_SideBar::TLS_SIDEBAR_HALL);

	Tls_Panel GetCurPanel();

	//ת��ARGB��RichEdit�ؼ���RGB
	DWORD TransARGBToRGB(DWORD dwValue);

	//ת��RichEdit�ؼ���RGB��ARGB
	DWORD TransRGBToARGB(DWORD dwValue);

	//ת��ARGB��RGBA
	DWORD TransARGBToRGBA(DWORD dwValue);

	///////////////////////////////////////////////////////// �ļ����� /////////////////////////////////////////////////////////

	// �ļ������ӿ�

	/*@breif ����ָ���ļ���
	*@param	lpPath		�ļ�Ŀ¼��
	*@param files		�������ļ��Ľṹ��
	*@param	isCircle	�Ƿ�ѭ��������Ŀ¼(Ĭ��:FALSE)
	*@param lpExName	Ҫɸѡ���ļ���׺��(����:dll,�򷵻ص���������ֻ����dll�ļ�,Ϊnull��_T("")�򷵻�ȫ���ļ�)
	*/
	void traverseFolder(LPCTSTR lpstrFilter, std::vector<FILE_STRUCT>& files, BOOL isCircle = FALSE, LPCTSTR lpExName = nullptr);

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
	static bool saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath);

	// �ļ�·���ӿ�

	static BOOL IsDirectory(LPCTSTR lpszPath);														// ���ָ��·���Ƿ�Ŀ¼
	static BOOL IsFileExist(LPCTSTR lpszFileName);													// ���ָ���ļ��Ƿ����
	static BOOL IsDirectoryExist(LPCTSTR lpszPath);													// ���ָ��Ŀ¼�Ƿ����
	static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);	// ����ָ���ļ�Ŀ¼
	static BOOL DeleteDirectory(LPCTSTR lpPathName);												// ɾ���ļ�����������

	static tstring GetAppPath();																	// ��ȡӦ�ó���ִ��·��
	static tstring GetCurDir();																		// ��ȡӦ�ó���ǰĿ¼
	static tstring GetTempPath();																	// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
	static tstring GetTempFileName(LPCTSTR lpszFileName);											// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
	static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);						// ��ȡ����ļ���(ȫ·��)
	static tstring GetPathRoot(LPCTSTR lpszPath);													// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
	static tstring GetDirectoryName(LPCTSTR lpszPath);												// ����ָ��·���ַ�����Ŀ¼��Ϣ
	static tstring GetFileName(LPCTSTR lpszPath);													// ����ָ��·���ַ������ļ�������չ��
	static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);									// ���ز�������չ����·���ַ������ļ���
	static tstring GetExtension(LPCTSTR lpszPath);													// ����ָ����·���ַ�������չ��
	static tstring GetFullPath(LPCTSTR lpszPath);													// ����ָ�������·����ȡ����·��
	static GUID GetFileExtensionGuid(LPCTSTR lpExtension);											// ��ȡ�ļ���չ����GUID

	///////////////////////////////////////////////////////// ��ȡӲ����Ϣ ///////////////////////////////////////////////////////
	
	BOOL GetPhyDriveSerial(LPTSTR pModelNo, LPTSTR pSerialNo);								// ��ȡӲ�����к�
	void ToLittleEndian(PUSHORT pWords, int nFirstIndex, int nLastIndex, LPTSTR pBuf);		// ��WORD��������ֽ���Ϊlittle-endian�����˳��ַ�����β�Ŀո�
	void TrimStart(LPTSTR pBuf);															// �˳��ַ�����ʼλ�õĿո�
	BOOL GetActiveMac(LPTSTR pBuf);															// ��ȡMAC��ַ
	BOOL GetCPU(LPTSTR pBuf);																// ��ȡCPU����
	tstring GetMachineString();																// ��ȡ������
	

	///////////////////////////////////////////////////////// �ַ������� /////////////////////////////////////////////////////////

	CString newGUID();																// ��������һ��ϵͳ�Զ����ɵ�32λ�ַ���
	const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);							// ���ص�ǰϵͳʱ�� ��ʽΪ: L"%Y-%m-%d %H:%M:%S"
	BOOL IsDigit(const TCHAR * lpStr);												// �ж��ַ����Ƿ�Ϊ����
	void Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr);	// �滻ָ���ַ���

	// ��ȡ�����ַ���֮�������/�ַ���
	int GetBetweenInt(const TCHAR* pStr, UINT cStart, UINT cEnd, int nDefValue = 0);
	int GetBetweenInt(const TCHAR* pStr, const TCHAR* pStart, const TCHAR* pEnd, int nDefValue = 0);
	tstring GetBetweenString(const TCHAR * pStr, UINT cStart, UINT cEnd);
	tstring GetBetweenString(const TCHAR * pStr, const TCHAR* pStart, const TCHAR* pEnd);

	///////////////////////////////////////////////////////// ��ʱ������ /////////////////////////////////////////////////////////
	void SetTimer(HWND hWnd, TimerCallBack pCallBack, UINT_PTR nIDEvent, UINT uElapse);
	void KillTimer(HWND hWnd, UINT_PTR nIDEvent);
	void RemoveAllTimers();

	///////////////////////////////////////////////////////// �༭�����ı� /////////////////////////////////////////////////////
	void SendSystemMsgText(CRichEditUI* pEdit, LPCTSTR pzText);												// ����ϵͳ��Ϣ
	void SendChatMsgText(CRichEditUI* pEdit, UserData& tUser, const LPCTSTR pzContent);						// ����������Ϣ

	void SendBuyPropsMsgText(CRichEditUI* pEdit, CMD_GF_PropsNotify* pStuOk);								// ���͹��������Ϣ
	void SendGivePropsMsgText(CRichEditUI* pEdit, CMD_GF_PropsNotify* pStuOk);								// �������͵�����Ϣ

	void SendUserLeaveText(CRichEditUI* pEdit, LPCTSTR pzName, LPCTSTR pzText);								// �����û��뿪��Ϣ

	bool LoadSensitiveVocabulary();																			// �������дʻ��(txt)
	bool CheckSensitiveVocabulary(CDuiString& pzOut, bool bReplace = false);								// ������дʻ�

	///////////////////////////////////////////////////////// ���� ///////////////////////////////////////////////////////////////
	bool GetSkinResPath(LPCTSTR lpFolderName, CDuiString& pzOutPath);
	int ReadSkinConfig();
	void SaveSkinConfig(LPCTSTR lpAccount);
	bool LoadSkinResFromDll(LPCTSTR lpFileName);

	///////////////////////////////////////////////////////// �˳����� //////////////////////////////////////////////////////////
	void EndHall();
	
private:

	TCHAR									m_pzBuffer[MAX_PATH];		// �ַ���������
	Tls_Panel								m_cur_panel;				// ��ǰ���
	std::map<UINT_PTR, stuUnFoucsWnd*>		m_vUnFocusWndMap;			// ʧȥ���㶯������
	WndTimmer								m_WndTimmer;				// ��ʱ������
	std::wstring							m_tstrSensitiveVocabulary;	// ���дʻ��(��������ÿ��ַ���ȥ����,�����������ʱ,ASCII���ظ���ͻ)

private:

	static GameUtil* g_instance;
	GameUtil();
};

#endif // __GAMEUTIL_H__