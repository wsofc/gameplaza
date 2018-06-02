
#pragma once

#include "NetModule.h"
#include "NetPacketParserXorMap.h"


const int PACKET_BUFFER_SIZE	= TCP_PACKET_MAXSIZE + TCP_PACKET_HEAD_SIZE;
const int SEND_BUFFER_SIZE		= TCP_PACKET_BUFFER_SIZE;
const int RECV_BUFFER_SIZE		= (TCP_PACKET_BUFFER_SIZE * 10);


//////////////////////////////////////////////////////////////////////////
/// TCP����ͻ���
//////////////////////////////////////////////////////////////////////////
class CTCPSocketClient : public CWnd, public ITCPSocketClient
{
public:
	CTCPSocketClient();
	virtual ~CTCPSocketClient();

public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

public:
	//���ýӿ�
	virtual bool __cdecl SetTCPSocketSink(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual VOID * __cdecl GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer);

public:
	//��ȡ״̬
	virtual BYTE __cdecl GetSocketStatus() { return m_cbSocketStatus; }
	//���ͼ��
	virtual DWORD __cdecl GetLastSendTick() { return m_dwSendTickCount; }
	//���ռ��
	virtual DWORD __cdecl GetLastRecvTick() { return m_dwRecvTickCount; }
	//������Ŀ
	virtual DWORD __cdecl GetSendPacketCount() { return m_dwSendPacketCount; }
	//������Ŀ
	virtual DWORD __cdecl GetRecvPacketCount() { return m_dwRecvPacketCount; }

public:
	//�������
	virtual DWORD __cdecl ProxyServerTesting();
	//���ô���
	virtual bool __cdecl SetProxyServerInfo(emProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo);

	//�����ӿ�
public:
	//�ر�����
	virtual VOID __cdecl CloseSocket();
	//���Ӳ���
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort);
	//���Ӳ���
	virtual DWORD __cdecl Connect(LPCTSTR szServerIP, WORD wPort);
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	virtual void __cdecl SetDebugOutputHelper(const tagDebugOutputHelper & DebugOutputHelper);

protected:
	//���Ӵ���
	DWORD ConnectProxyServer();
	//��ַ����
	DWORD TranslateAddr(LPCTSTR pszServerAddr);

protected:
	//�ر�����
	VOID CloseSocket(BYTE cbShutReason);
	//��������
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);

	//��������
	DWORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);

protected:
	DECLARE_MESSAGE_MAP()
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);
	//�����ȡ
	afx_msg LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//���緢��
	afx_msg LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//����ر�
	afx_msg LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//��������
	afx_msg LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

private:
	//�����������
	void OnTickDetect(void);

protected:
	BYTE							m_cbSocketStatus;					//����״̬

protected:
	SOCKET							m_hSocket;							//���Ӿ��
	ITCPSocketClientSink*			m_pITCPSocketSink;					//�ص��ӿ�

protected:
	emProxyServerType				m_ProxyServerType;					//��������
	tagProxyServerInfo				m_ProxyServerInfo;					//������Ϣ

protected:
	WORD							m_wRecvSize;						//���ճ���
	BYTE							m_cbRecvBuf[TCP_PACKET_MAXSIZE*10];		//���ջ���

protected:
	bool							m_bNeedBuffer;						//����״̬
	DWORD							m_dwBufferData;						//��������
	DWORD							m_dwBufferSize;						//�����С
	LPBYTE							m_pcbDataBuffer;					//��������

protected:
	DWORD							m_dwSendTickCount;					//����ʱ��
	DWORD							m_dwRecvTickCount;					//����ʱ��
	DWORD							m_dwSendPacketCount;				//���ͼ���
	DWORD							m_dwRecvPacketCount;				//���ܼ���

private:
	tagDebugOutputHelper			m_DebugOutputHelper;
	tagClientTickDetect				m_ClientTickDetect;

private:
	CNetPacketParserXorMap			m_pPacketParser;					///< �������ݽ�����
};


//-----------------------------------------------
//					the end
//-----------------------------------------------