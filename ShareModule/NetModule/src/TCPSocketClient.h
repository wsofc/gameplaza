
#pragma once

#include "NetModule.h"
#include "NetPacketParserXorMap.h"


const int PACKET_BUFFER_SIZE	= TCP_PACKET_MAXSIZE + TCP_PACKET_HEAD_SIZE;
const int SEND_BUFFER_SIZE		= TCP_PACKET_BUFFER_SIZE;
const int RECV_BUFFER_SIZE		= (TCP_PACKET_BUFFER_SIZE * 10);


//////////////////////////////////////////////////////////////////////////
/// TCP网络客户端
//////////////////////////////////////////////////////////////////////////
class CTCPSocketClient : public CWnd, public ITCPSocketClient
{
public:
	CTCPSocketClient();
	virtual ~CTCPSocketClient();

public:
	//释放对象
	virtual VOID __cdecl Release() { delete this; }
	//接口查询
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

public:
	//设置接口
	virtual bool __cdecl SetTCPSocketSink(IUnknownEx * pIUnknownEx);
	//获取接口
	virtual VOID * __cdecl GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer);

public:
	//获取状态
	virtual BYTE __cdecl GetSocketStatus() { return m_cbSocketStatus; }
	//发送间隔
	virtual DWORD __cdecl GetLastSendTick() { return m_dwSendTickCount; }
	//接收间隔
	virtual DWORD __cdecl GetLastRecvTick() { return m_dwRecvTickCount; }
	//发送数目
	virtual DWORD __cdecl GetSendPacketCount() { return m_dwSendPacketCount; }
	//接收数目
	virtual DWORD __cdecl GetRecvPacketCount() { return m_dwRecvPacketCount; }

public:
	//代理测试
	virtual DWORD __cdecl ProxyServerTesting();
	//设置代理
	virtual bool __cdecl SetProxyServerInfo(emProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo);

	//操作接口
public:
	//关闭连接
	virtual VOID __cdecl CloseSocket();
	//连接操作
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort);
	//连接操作
	virtual DWORD __cdecl Connect(LPCTSTR szServerIP, WORD wPort);
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	virtual void __cdecl SetDebugOutputHelper(const tagDebugOutputHelper & DebugOutputHelper);

protected:
	//连接代理
	DWORD ConnectProxyServer();
	//地址解释
	DWORD TranslateAddr(LPCTSTR pszServerAddr);

protected:
	//关闭连接
	VOID CloseSocket(BYTE cbShutReason);
	//缓冲数据
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);

	//发送数据
	DWORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);

protected:
	DECLARE_MESSAGE_MAP()
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//网络消息
	afx_msg LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);
	//网络读取
	afx_msg LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//网络发送
	afx_msg LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//网络关闭
	afx_msg LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//网络连接
	afx_msg LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

private:
	//处理心跳检测
	void OnTickDetect(void);

protected:
	BYTE							m_cbSocketStatus;					//网络状态

protected:
	SOCKET							m_hSocket;							//连接句柄
	ITCPSocketClientSink*			m_pITCPSocketSink;					//回调接口

protected:
	emProxyServerType				m_ProxyServerType;					//代理类型
	tagProxyServerInfo				m_ProxyServerInfo;					//代理信息

protected:
	WORD							m_wRecvSize;						//接收长度
	BYTE							m_cbRecvBuf[TCP_PACKET_MAXSIZE*10];		//接收缓冲

protected:
	bool							m_bNeedBuffer;						//缓冲状态
	DWORD							m_dwBufferData;						//缓冲数据
	DWORD							m_dwBufferSize;						//缓冲大小
	LPBYTE							m_pcbDataBuffer;					//缓冲数据

protected:
	DWORD							m_dwSendTickCount;					//发送时间
	DWORD							m_dwRecvTickCount;					//接收时间
	DWORD							m_dwSendPacketCount;				//发送计数
	DWORD							m_dwRecvPacketCount;				//接受计数

private:
	tagDebugOutputHelper			m_DebugOutputHelper;
	tagClientTickDetect				m_ClientTickDetect;

private:
	CNetPacketParserXorMap			m_pPacketParser;					///< 网络数据解析器
};


//-----------------------------------------------
//					the end
//-----------------------------------------------