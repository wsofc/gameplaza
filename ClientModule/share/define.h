#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <tchar.h>

#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif // ASSERT

#ifndef ArrayCount
#define	ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#endif // ArrayCount

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ try { delete p; } catch (...) { } p = nullptr; } 
#endif // SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(pArr) { if (pArr) { delete [] (pArr);  (pArr) = nullptr; } }
#endif // SAFE_DELETE_ARRAY

// 转换宏字符
#define _TO_S(x)	(#x)
#define TO_S(x)		_TO_S(x)

// 大厅版本信息
#define GAMEPLAZA_VERSION_1(wServerHigh)		((wServerHigh & 0xFF) / 10)
#define GAMEPLAZA_VERSION_2(wServerHigh)		((wServerHigh & 0xFF) % 10)
#define GAMEPLAZA_VERSION_3(wServerLow)			((wServerLow & 0xFF) / 10)
#define GAMEPLAZA_VERSION_4(wServerLow)			((wServerLow & 0xFF) % 10)

#define GAMEPLAZA_VERSION_S_1(wServerHigh)		TO_S(GAMEPLAZA_VERSION_1(wServerHigh))
#define GAMEPLAZA_VERSION_S_2(wServerHigh)		TO_S(GAMEPLAZA_VERSION_2(wServerHigh))
#define GAMEPLAZA_VERSION_S_3(wServerLow)		TO_S(GAMEPLAZA_VERSION_3(wServerLow))
#define GAMEPLAZA_VERSION_S_4(wServerLow)		TO_S(GAMEPLAZA_VERSION_4(wServerLow))

#define MAKELONGLONG(l, h)			((LONGLONG)(((((DWORD_PTR)(l)) & 0xffffffff)) | (((ULONGLONG)(((DWORD_PTR)(h)) & 0xffffffff)) << 32)))

// 自定义消息
#define CMW_CLOSE_SURE				0x00000100				// 关闭当前窗口

#define	FACE_CTRL_SEL				WM_USER + 0x0001		// 表情窗口,表情选择消息
#define	TRAY_MENU					WM_USER + 0x0002		// 托盘菜单鼠标消息
#define DOWNLOAD_ENTER_ROOM			WM_USER + 0x0003		// 游戏更新完成进入房间
#define WM_UNINSTALL_USERMENU		WM_USER + 0x0004		// 卸载玩家控制菜单消息

#define IPC_SUB_HEARTBEAT			0x1979					// 心跳消息
#define IPC_SUB_START_FAILED		0x0005					// 启动失败
#define	IPC_SUB_CLOSE_FRAME_EX		0x0006					// 游戏强行关闭
#define IPC_SUB_GF_USER_READY		0x0007					// 游戏用户同意
#define IPC_SUB_GF_RECONNECT		0x0008					// 游戏断线重连
#define IPC_SUB_KICK_USER			0x0009					// 踢出用户
#define IPC_SUB_SEND_CHAT			0x0010					// 发送聊天消息
#define IPC_USER_SUFFIX				0x0011					// 附加的玩家信息结构

#define IPC_FLAG_TO_SERVER			0x0000                  // 数据发送给服务器
#define IPC_FLAG_TO_GAMEPLAZA		0x0001                  // 数据发送给大厅
#define IPC_FLAG_TO_GAME			0x0002					// 数据发送给游戏
#define IPC_FLAG_TO_WINFROM			0x0003					// 数据发送给Winform

// Winform框架消息
#define IPC_SUB_WINGAME_START_OK	0x1001					// WinForm启动成功
#define IPC_SUB_WINGAME_START_NO	0x1002					// WinForm启动失败
#define IPC_SUB_WINGAME_GAMEPROCESS	0x1003					// 保存游戏进程信息
#define IPC_SUB_WINGAME_START_INFO	0x1004					// 游戏场景初请求始化
#define IPC_SUB_WINGAME_RESIZE		0x2001					// 游戏窗口大小发生改变
#define IPC_SUB_WINGAME_MENU_INIT	0x2002					// 游戏菜单初始化
#define IPC_SUB_WINGAME_MENU_CONFIG 0x2003					// 游戏菜单配置

// new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

// 字符串
#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

// 定时器
// 时间
#define TIME_HEARTBEAT				2000			// 与游戏进程的心跳时间间隔(单位:ms)
#define TIME_LOCK_MACHINE			2000			// 大厅锁定本机按钮点击间隔(单位:ms)
#define TIME_HEART_MAX_OUT			30				// 心跳超时最大次数
#define TIME_HEART_MIN_OUT			3				// 心跳超时最小次数
#define TIME_TOAST_PERFRAME			100				// toast每次间隔时间
#define TIME_TOAST_OVER				1500			// toast自动消失时间
#define TIME_WAITTING_OVER			30000			// waitting 时间
#define TIME_GET_VERTCODE			60				// 获取验证码间隔时间(60秒)
#define RECONNECT_INTERVAL			6000			// 网络异常重连时间间隔(单位:毫秒)
#define TIME_START_GAME_FINISH		10				// 启动游戏完毕后延时发送列表数据(单位:毫秒)

// ID
#define TIMER_ID_GET_VERTCODE		0x0001			// 验证码定时器 I D
#define TIMER_ID_LOGINEND_SERVER	0x0100			// 服务端登录完成
#define TIMER_ID_CREATEHALL			0x0102			// 创建大厅
#define TIMER_ID_LOGINEND_CLIENT	0x0200			// 客户端登录完成
#define	TIMER_ID_HALL_ROOL_TEXT		0x0300			// 大厅底栏滚动文本
#define TIMER_ID_ZOOMBOX			0x0400			// 桌子面板快速选桌
#define	TIMER_ID_HALL_LOCK_MACHINE	0x0500			// 大厅锁定本机
#define TIMER_ID_RECONNECT			0x0600			// 网络异常重连定时器
#define TIMER_ID_START_GAME_FINISH	0x0700			// 启动游戏完毕定时器
#define TIMER_ID_HEARTBEAT			0x1979			// 心跳定时器 I D 

// 其他
#define DEFAULT_LOCAL_HEAD_COUNT	30				// 默认本地头像个数(男女各占一半,下标从0开始)
#define SMS_VERTCODE_FIGURES		4				// 短信验证码位数
#define CANDIDATE_ACCOUNT_NUM		4				// 候选账号上限个数
#define DOWNLOAD_TASK_MAX_COUNT		5				// 同时下载多个任务的最大个数
#define RECONNECT_MAX_COUNT			10				// 网络异常重连最大次数


struct stuMsgBoxFlag
{
	stuMsgBoxFlag()
	{
		memset(this, 0, sizeof(*this));
	}

	stuMsgBoxFlag(bool bRes, unsigned int uMsgID)
	{
		this->bRes = bRes;
		this->uMsgID = uMsgID;
	}

	bool			bRes;
	unsigned int	uMsgID;
};

// 拷贝LPCTSTR至TCHAR数组中(含大小检测)
template <size_t size>
int StrToCharArray(TCHAR(&buffer)[size], LPCTSTR lpStr)
{
	if (!lpStr) return -1;

	int iSize = _tcslen(lpStr) + 1;
	iSize = min(ArrayCount(buffer) - 1, iSize);
	memcpy(buffer, lpStr, iSize * sizeof(TCHAR));

	return iSize;
}

#endif //__DEFINE_H__