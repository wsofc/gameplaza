#pragma once

#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
#include "gameResource.h"						// 游戏资源集
#include "trayMenu.h"							// 托盘菜单
#include "gifImage.h"							// gif图支持
#include "richEditUtil.h"						// 编辑框辅助接口

#include "..\\data\\dataManager.h"				// 大厅数据管理器
#include "..\\ipc\\ipc.h"						// ipc通信
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

	tstring		fileName;	//文件全名(文件名 + 后缀名)
	tstring		name;		//文件名
	tstring		exName;		//后缀名
	int			size;		//大小
};

enum MatchWords_Type
{
	REG_TYPE_NULL			//无
	, REG_TYPE_ACCOUNT		//账号
	, REG_TYPE_NICKNAME		//昵称
	, REG_TYPE_PASSWORD		//密码
	, REG_TYPE_TELPHONE		//手机号
	, REG_TYPE_VERTCODE		//验证码
	, REG_TYPE_REALNAME		//真实姓名
	, REG_TYPE_EMAIL		//邮箱
	, REG_TYPE_QQ_NUMBER	//QQ号码
	, REG_TYPE_QUESTION		//密保问题
	, REG_TYPE_ANSWER		//密保答案
	, REG_TYPE_IDCARD		//身份证
	, REG_TYPE_UNDERWRITE	//个人说明
	, REG_TYPE_UNREAL		//unreal
	, REG_TYPE_EXTRA		//预留
};

//面板枚举
enum Tls_Panel
{
	TLS_NONE,				//无
	TLS_PANEL_HALL,			//主面板
	TLS_PANEL_ROOM_LIST,	//房间列表面板
	TLS_PANEL_ROOM,			//房间面板
	TLS_PANEL_EXTRA			//预留项
};

//侧栏枚举
enum Tls_SideBar
{
	TLS_SIDEBAR_HALL,		//主侧栏
	TLS_SIDEBAR_ROOM_LIST,	//房间侧栏
	TLS_SIDEBAR_ROOM,		//桌子侧栏
	TLS_SIDEBAR_EXTRA		//预留项
};

struct stuUnFoucsWnd
{
	stuUnFoucsWnd()
	{
		memset(this, 0, sizeof(*this));
	}

	HWND	hWnd;			//失去焦点的窗口 
	DWORD	dwActOffset;	//运动幅度
	DWORD	dwActCount;		//运动计数
	DWORD	dwActTimes;		//运动次数
	RECT	rectSrc;		//起始位置
};

class GameUtil
{
public:

	static GameUtil* GetInstance();
	static int WINAPI WriteWebBrowserRegKey(LPCTSTR lpKey, DWORD dwValue, tstring& strOutInfo);

	~GameUtil();

	bool SetClipBoardText(LPCSTR text, HWND hWnd);								//设置剪贴板文本内容
	tstring GetClipBoardText(HWND hWnd);										//获取剪贴板文本内容

	bool MatchWordByType(CDuiString& strIn, DWORD dwType);						//正则匹配注册信息
	bool MatchRegular(CDuiString& strIn, LPCTSTR lpReg);

	void MinimizeTraySaveControl(CControlUI* control = NULL);					//保存最小化至托盘时要恢复状态的控件

	void MinimizeTray(HWND hWnd);												//最小化(临时关闭) 至托盘

	void SetFrozenEnable(CPaintManagerUI* pManager, bool bEnable);				//冻结根节点鼠标键盘消息

	void ShowTopWindow(HWND hWnd, bool bShow = true, bool bTakeFocus = true);	//置顶显示窗口

	void RestoreTopWindow(HWND hWnd);											//置顶复原窗口

	void OpenWebByName(LPCTSTR lpName);											//打开指定名称的网页

	void OpenWebByURL(LPCTSTR lpName);											//打开指定的网页

	LPCTSTR GetWebByName(LPCTSTR lpName);										//获取指定名称的网页url

	void CreatTrayMenu(CPaintManagerUI* pParentManager);						//创建托盘菜单
	
	//窗口失去焦点动画
	void RunWndUnFocusAction(HWND hWnd, DWORD dwActTimes = 3, DWORD dwActOffset = 5);

	//窗口失去焦点动画回调
	void ProFuncWndUnFocus(UINT_PTR idEvent, DWORD dwTime);

	//停止动画
	void StopWndUnFocusAction(HWND hWnd);
	
	//更换面板和侧栏布局
	void SetCurPanel(CPaintManagerUI* pManager
		, Tls_Panel tls_panel = Tls_Panel::TLS_PANEL_HALL
		, Tls_SideBar tls_sidebar = Tls_SideBar::TLS_SIDEBAR_HALL);

	Tls_Panel GetCurPanel();

	//转换ARGB至RichEdit控件的RGB
	DWORD TransARGBToRGB(DWORD dwValue);

	//转换RichEdit控件的RGB至ARGB
	DWORD TransRGBToARGB(DWORD dwValue);

	//转换ARGB至RGBA
	DWORD TransARGBToRGBA(DWORD dwValue);

	///////////////////////////////////////////////////////// 文件操作 /////////////////////////////////////////////////////////

	// 文件操作接口

	/*@breif 遍历指定文件夹
	*@param	lpPath		文件目录名
	*@param files		所有子文件的结构体
	*@param	isCircle	是否循环遍历子目录(默认:FALSE)
	*@param lpExName	要筛选的文件后缀名(例如:dll,则返回的数组里面只包含dll文件,为null或_T("")则返回全部文件)
	*/
	void traverseFolder(LPCTSTR lpstrFilter, std::vector<FILE_STRUCT>& files, BOOL isCircle = FALSE, LPCTSTR lpExName = nullptr);

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
	static bool saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath);

	// 文件路径接口

	static BOOL IsDirectory(LPCTSTR lpszPath);														// 检测指定路径是否目录
	static BOOL IsFileExist(LPCTSTR lpszFileName);													// 检测指定文件是否存在
	static BOOL IsDirectoryExist(LPCTSTR lpszPath);													// 检测指定目录是否存在
	static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);	// 创建指定文件目录
	static BOOL DeleteDirectory(LPCTSTR lpPathName);												// 删除文件夹所有内容

	static tstring GetAppPath();																	// 获取应用程序执行路径
	static tstring GetCurDir();																		// 获取应用程序当前目录
	static tstring GetTempPath();																	// 获取当前系统的临时文件夹的路径
	static tstring GetTempFileName(LPCTSTR lpszFileName);											// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
	static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);						// 获取随机文件名(全路径)
	static tstring GetPathRoot(LPCTSTR lpszPath);													// 获取指定路径的根目录信息
	static tstring GetDirectoryName(LPCTSTR lpszPath);												// 返回指定路径字符串的目录信息
	static tstring GetFileName(LPCTSTR lpszPath);													// 返回指定路径字符串的文件名和扩展名
	static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);									// 返回不具有扩展名的路径字符串的文件名
	static tstring GetExtension(LPCTSTR lpszPath);													// 返回指定的路径字符串的扩展名
	static tstring GetFullPath(LPCTSTR lpszPath);													// 根据指定的相对路径获取绝对路径
	static GUID GetFileExtensionGuid(LPCTSTR lpExtension);											// 获取文件扩展名的GUID

	///////////////////////////////////////////////////////// 获取硬件信息 ///////////////////////////////////////////////////////
	
	BOOL GetPhyDriveSerial(LPTSTR pModelNo, LPTSTR pSerialNo);								// 获取硬盘序列号
	void ToLittleEndian(PUSHORT pWords, int nFirstIndex, int nLastIndex, LPTSTR pBuf);		// 把WORD数组调整字节序为little-endian，并滤除字符串结尾的空格
	void TrimStart(LPTSTR pBuf);															// 滤除字符串起始位置的空格
	BOOL GetActiveMac(LPTSTR pBuf);															// 获取MAC地址
	BOOL GetCPU(LPTSTR pBuf);																// 获取CPU名称
	tstring GetMachineString();																// 获取机器串
	

	///////////////////////////////////////////////////////// 字符串操作 /////////////////////////////////////////////////////////

	CString newGUID();																// 用来返回一个系统自动生成的32位字符串
	const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);							// 返回当前系统时间 格式为: L"%Y-%m-%d %H:%M:%S"
	BOOL IsDigit(const TCHAR * lpStr);												// 判断字符串是否为数字
	void Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr);	// 替换指定字符串

	// 获取两个字符串之间的整数/字符串
	int GetBetweenInt(const TCHAR* pStr, UINT cStart, UINT cEnd, int nDefValue = 0);
	int GetBetweenInt(const TCHAR* pStr, const TCHAR* pStart, const TCHAR* pEnd, int nDefValue = 0);
	tstring GetBetweenString(const TCHAR * pStr, UINT cStart, UINT cEnd);
	tstring GetBetweenString(const TCHAR * pStr, const TCHAR* pStart, const TCHAR* pEnd);

	///////////////////////////////////////////////////////// 定时器操作 /////////////////////////////////////////////////////////
	void SetTimer(HWND hWnd, TimerCallBack pCallBack, UINT_PTR nIDEvent, UINT uElapse);
	void KillTimer(HWND hWnd, UINT_PTR nIDEvent);
	void RemoveAllTimers();

	///////////////////////////////////////////////////////// 编辑框发送文本 /////////////////////////////////////////////////////
	void SendSystemMsgText(CRichEditUI* pEdit, LPCTSTR pzText);												// 发送系统消息
	void SendChatMsgText(CRichEditUI* pEdit, UserData& tUser, const LPCTSTR pzContent);						// 发送聊天消息

	void SendBuyPropsMsgText(CRichEditUI* pEdit, CMD_GF_PropsNotify* pStuOk);								// 发送购买道具消息
	void SendGivePropsMsgText(CRichEditUI* pEdit, CMD_GF_PropsNotify* pStuOk);								// 发送赠送道具消息

	void SendUserLeaveText(CRichEditUI* pEdit, LPCTSTR pzName, LPCTSTR pzText);								// 发送用户离开消息

	bool LoadSensitiveVocabulary();																			// 加载敏感词汇表(txt)
	bool CheckSensitiveVocabulary(CDuiString& pzOut, bool bReplace = false);								// 检测敏感词汇

	///////////////////////////////////////////////////////// 换肤 ///////////////////////////////////////////////////////////////
	bool GetSkinResPath(LPCTSTR lpFolderName, CDuiString& pzOutPath);
	int ReadSkinConfig();
	void SaveSkinConfig(LPCTSTR lpAccount);
	bool LoadSkinResFromDll(LPCTSTR lpFileName);

	///////////////////////////////////////////////////////// 退出大厅 //////////////////////////////////////////////////////////
	void EndHall();
	
private:

	TCHAR									m_pzBuffer[MAX_PATH];		// 字符串缓冲区
	Tls_Panel								m_cur_panel;				// 当前面板
	std::map<UINT_PTR, stuUnFoucsWnd*>		m_vUnFocusWndMap;			// 失去焦点动画容器
	WndTimmer								m_WndTimmer;				// 定时器对象
	std::wstring							m_tstrSensitiveVocabulary;	// 敏感词汇表(这里必须用宽字符串去解析,否则检索中文时,ASCII会重复冲突)

private:

	static GameUtil* g_instance;
	GameUtil();
};

#endif // __GAMEUTIL_H__