#pragma  once


//ƽ̨�ļ�
#include "Template.h"
#include "GlobalDef.h"
#include "BasicTypes.h"


//��������
#ifndef NET_MODULE_CLASS
#	ifdef  NET_MODULE_DLL
#		define NET_MODULE_CLASS _declspec(dllexport)
#	else
#		define NET_MODULE_CLASS _declspec(dllimport)
#	endif
#endif

//ģ�鶨��
#ifndef _DEBUG
#	define NET_MODULE_DLL_NAME	TEXT("NetModule.dll")		//��� DLL ����
#else
#	define NET_MODULE_DLL_NAME	TEXT("NetModuleD.dll")		//��� DLL ����
#endif

//
// �Զ����ӿ�.
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




//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣
#define CONNECT_PROXY_FAILURE		3									//���ӳɹ�
#define CONNECT_PROXY_USER_INVALID	4									//�û���Ч


//////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////
enum emProxyServerType
{
	PROXY_TYPE_NONE		,			//û�д���
	PROXY_TYPE_HTTP		,			//HTTP ����
	PROXY_TYPE_SOCKS4	,			//SOCKS4 ����
	PROXY_TYPE_SOCKS5	,			//SOCKS5 ����
};


//////////////////////////////////////////////////////////////////////////
//������Ϣ
//////////////////////////////////////////////////////////////////////////
struct tagProxyServerInfo
{
	tagProxyServerInfo()
	{
		memset(this,0,sizeof(*this));
	}

	WORD							wProxyPort;							//����˿�
	TCHAR							szUserName[32];						//�����û�
	TCHAR							szPassword[32];						//��������
	TCHAR							szHttpDomain[32];					//�����û�
	TCHAR							szProxyServer[32];					//�����ַ
};


//////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////
struct tagProxyTypeDescribe
{
	TCHAR							szDescribe[32];						//�����ַ�
	emProxyServerType				ProxyServerType;					//��������
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
	TCHAR	szOutputPrefix[256];	//���ǰ׺
};


//////////////////////////////////////////////////////////////////////////
/// �ͻ�������������¼
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

	DWORD	dwTickTimeSpace;			//����ʱ����
	DWORD   dwDisconnectTickTimes;		//�Ͽ�����������
	DWORD	dwMaxDisconnectTickTimes;	//���ĶϿ���������,������,��Ϊ������
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
/// TCP����ͻ��˽ӿ�
//////////////////////////////////////////////////////////////////////////
interface ITCPSocketClient : public IUnknownEx
{
public:
	//���ýӿ�
	virtual bool __cdecl SetTCPSocketSink(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�ӿ�
	virtual VOID * __cdecl GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)=NULL;

public:
	//����״̬
	virtual BYTE __cdecl GetSocketStatus()=NULL;
	//���ͼ��
	virtual DWORD __cdecl GetLastSendTick()=NULL;
	//���ռ��
	virtual DWORD __cdecl GetLastRecvTick()=NULL;
	//������Ŀ
	virtual DWORD __cdecl GetSendPacketCount()=NULL;
	//������Ŀ
	virtual DWORD __cdecl GetRecvPacketCount()=NULL;

public:
	//�������
	virtual DWORD __cdecl ProxyServerTesting()=NULL;
	//���ô���
	virtual bool __cdecl SetProxyServerInfo(emProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)=NULL;

public:
	//�ر�����
	virtual VOID __cdecl CloseSocket()=NULL;
	//���Ӳ���
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//���Ӳ���
	virtual DWORD __cdecl Connect(LPCTSTR szServerIP, WORD wPort)=NULL;
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;

public:
	virtual void __cdecl SetDebugOutputHelper(const tagDebugOutputHelper & DebugOutputHelper)=NULL;
};


//////////////////////////////////////////////////////////////////////////
/// TCP����ͻ��˻ص��ӿ�
//////////////////////////////////////////////////////////////////////////
interface ITCPSocketClientSink : public IUnknownEx
{
public:
	/// �����¼�
	virtual bool __cdecl OnSocketConnect(INT nErrorCode, ITCPSocketClient* pITCPSocketClient) = NULL;

	/// �ر��¼�
	virtual bool __cdecl OnSocketClose(BYTE cbShutReason, ITCPSocketClient* pITCPSocketClient)=NULL;

	/// ��ȡ�¼�
	virtual bool __cdecl OnSocketRead(const CMD_Command& cmd, VOID* pDataBuffer, WORD wDataSize, ITCPSocketClient* pITCPSocketClient) = NULL;
};
DECLARE_MODULE_HELPER(TCPSocketClient, NET_MODULE_DLL_NAME, "CreateTCPSocketClient")



#include "NetPacketParserXorMap.h"

