#pragma  once


//平台文件
#include "Template.h"
#include "GlobalDef.h"
#include "BasicTypes.h"


//导出定义
#ifndef NET_MODULE_CLASS
#	ifdef  NET_MODULE_DLL
#		define NET_MODULE_CLASS _declspec(dllexport)
#	else
#		define NET_MODULE_CLASS _declspec(dllimport)
#	endif
#endif

//模块定义
#ifndef _DEBUG
#	define NET_MODULE_DLL_NAME	TEXT("NetModule.dll")		//组件 DLL 名字
#else
#	define NET_MODULE_DLL_NAME	TEXT("NetModuleD.dll")		//组件 DLL 名字
#endif

//
// 自动链接库.
//
#if defined(_MSC_VER)
#	if !defined(NO_AUTOMATIC_LIBS) && !defined(NET_MODULE_DLL)
#		if defined(_DEBUG)
#			pragma comment(lib, "NetModuleD.lib")
#		else
#			pragma comment(lib, "NetModule.lib")
#		endif
#	endif
#endif




//关闭原因
#define SHUT_REASON_INSIDE			0									//内部原因
#define SHUT_REASON_NORMAL			1									//正常关闭
#define SHUT_REASON_REMOTE			2									//远程关闭
#define SHUT_REASON_TIME_OUT		3									//网络超时
#define SHUT_REASON_EXCEPTION		4									//异常关闭

//网络状态
#define SOCKET_STATUS_IDLE			0									//空闲状态
#define SOCKET_STATUS_WAIT			1									//等待状态
#define SOCKET_STATUS_CONNECT		2									//连接状态

//连接错误
#define CONNECT_SUCCESS				0									//连接成功
#define CONNECT_FAILURE				1									//连接失败
#define CONNECT_EXCEPTION			2									//参数异常
#define CONNECT_PROXY_FAILURE		3									//连接成功
#define CONNECT_PROXY_USER_INVALID	4									//用户无效


//////////////////////////////////////////////////////////////////////////
//代理类型
//////////////////////////////////////////////////////////////////////////
enum emProxyServerType
{
	PROXY_TYPE_NONE		,			//没有代理
	PROXY_TYPE_HTTP		,			//HTTP 代理
	PROXY_TYPE_SOCKS4	,			//SOCKS4 代理
	PROXY_TYPE_SOCKS5	,			//SOCKS5 代理
};


//////////////////////////////////////////////////////////////////////////
//代理信息
//////////////////////////////////////////////////////////////////////////
struct tagProxyServerInfo
{
	tagProxyServerInfo()
	{
		memset(this,0,sizeof(*this));
	}

	WORD							wProxyPort;							//代理端口
	TCHAR							szUserName[32];						//代理用户
	TCHAR							szPassword[32];						//代理密码
	TCHAR							szHttpDomain[32];					//代理用户
	TCHAR							szProxyServer[32];					//代理地址
};


//////////////////////////////////////////////////////////////////////////
//代理描述
//////////////////////////////////////////////////////////////////////////
struct tagProxyTypeDescribe
{
	TCHAR							szDescribe[32];						//描述字符
	emProxyServerType				ProxyServerType;					//代理类型
};


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
struct tagDebugOutputHelper
{
	tagDebugOutputHelper()
	{
		memset(this,0,sizeof(*this));
	}
	bool	bEnable;
	TCHAR	szOutputPrefix[256];	//输出前缀
};


//////////////////////////////////////////////////////////////////////////
/// 客户端心跳包检测记录
//////////////////////////////////////////////////////////////////////////
struct tagClientTickDetect
{
	tagClientTickDetect(){this->Init();}
	void Init(void)
	{
		memset(this,0,sizeof(*this));
		dwMaxDisconnectTickTimes = 6;
	}

	DWORD	dwOldTickCount;
	DWORD	dwNewTickCount;

	DWORD	dwTickTimeSpace;			//心跳时间间隔
	DWORD   dwDisconnectTickTimes;		//断开的心跳次数
	DWORD	dwMaxDisconnectTickTimes;	//最大的断开心跳次数,超过了,认为断线了
};


#ifdef _UNICODE
#	define VER_ITCPSocketClient INTERFACE_VERSION(1,1)
static const GUID IID_ITCPSocketClient = {0x91446684,0x621c,0x4546,0x86,0x59,0x22,0xa9,0x5a,0x7c,0x20,0xb9};
#else
#	define VER_ITCPSocketClient INTERFACE_VERSION(1,1)
static const GUID IID_ITCPSocketClient = {0xc8a0b91a,0xcb6f,0x4749,0xa3,0x44,0xa7,0x06,0x32,0x11,0xbf,0x79};
#endif

#ifdef _UNICODE
#	define VER_ITCPSocketClientSink INTERFACE_VERSION(1,1)
static const GUID IID_ITCPSocketClientSink = {0xdb0f09dd,0x65e4,0x441f,0x0094,0xb1,0xfe,0x80,0x8c,0x1d,0xf2,0x65};
#else
#	define VER_ITCPSocketClientSink INTERFACE_VERSION(1,1)
static const GUID IID_ITCPSocketClientSink = {0x231416e8,0x74b0,0x472d,0x009e,0xc3,0x4a,0x22,0xe5,0x21,0x09,0x37};
#endif


//////////////////////////////////////////////////////////////////////////
/// TCP网络客户端接口
//////////////////////////////////////////////////////////////////////////
interface ITCPSocketClient : public IUnknownEx
{
public:
	//设置接口
	virtual bool __cdecl SetTCPSocketSink(IUnknownEx * pIUnknownEx)=NULL;
	//获取接口
	virtual VOID * __cdecl GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)=NULL;

public:
	//网络状态
	virtual BYTE __cdecl GetSocketStatus()=NULL;
	//发送间隔
	virtual DWORD __cdecl GetLastSendTick()=NULL;
	//接收间隔
	virtual DWORD __cdecl GetLastRecvTick()=NULL;
	//发送数目
	virtual DWORD __cdecl GetSendPacketCount()=NULL;
	//接收数目
	virtual DWORD __cdecl GetRecvPacketCount()=NULL;

public:
	//代理测试
	virtual DWORD __cdecl ProxyServerTesting()=NULL;
	//设置代理
	virtual bool __cdecl SetProxyServerInfo(emProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)=NULL;

public:
	//关闭连接
	virtual VOID __cdecl CloseSocket()=NULL;
	//连接操作
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//连接操作
	virtual DWORD __cdecl Connect(LPCTSTR szServerIP, WORD wPort)=NULL;
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;

public:
	virtual void __cdecl SetDebugOutputHelper(const tagDebugOutputHelper & DebugOutputHelper)=NULL;
};


//////////////////////////////////////////////////////////////////////////
/// TCP网络客户端回调接口
//////////////////////////////////////////////////////////////////////////
interface ITCPSocketClientSink : public IUnknownEx
{
public:
	/// 连接事件
	virtual bool __cdecl OnSocketConnect(INT nErrorCode, ITCPSocketClient* pITCPSocketClient) = NULL;

	/// 关闭事件
	virtual bool __cdecl OnSocketClose(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)=NULL;

	/// 读取事件
	virtual bool __cdecl OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient) = NULL;
};
DECLARE_MODULE_HELPER(TCPSocketClient, NET_MODULE_DLL_NAME, "CreateTCPSocketClient")



#include "NetPacketParserXorMap.h"

