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

// ת�����ַ�
#define _TO_S(x)	(#x)
#define TO_S(x)		_TO_S(x)

// �����汾��Ϣ
#define GAMEPLAZA_VERSION_1(wServerHigh)		((wServerHigh & 0xFF) / 10)
#define GAMEPLAZA_VERSION_2(wServerHigh)		((wServerHigh & 0xFF) % 10)
#define GAMEPLAZA_VERSION_3(wServerLow)			((wServerLow & 0xFF) / 10)
#define GAMEPLAZA_VERSION_4(wServerLow)			((wServerLow & 0xFF) % 10)

#define GAMEPLAZA_VERSION_S_1(wServerHigh)		TO_S(GAMEPLAZA_VERSION_1(wServerHigh))
#define GAMEPLAZA_VERSION_S_2(wServerHigh)		TO_S(GAMEPLAZA_VERSION_2(wServerHigh))
#define GAMEPLAZA_VERSION_S_3(wServerLow)		TO_S(GAMEPLAZA_VERSION_3(wServerLow))
#define GAMEPLAZA_VERSION_S_4(wServerLow)		TO_S(GAMEPLAZA_VERSION_4(wServerLow))

#define MAKELONGLONG(l, h)			((LONGLONG)(((((DWORD_PTR)(l)) & 0xffffffff)) | (((ULONGLONG)(((DWORD_PTR)(h)) & 0xffffffff)) << 32)))

// �Զ�����Ϣ
#define CMW_CLOSE_SURE				0x00000100				// �رյ�ǰ����

#define	FACE_CTRL_SEL				WM_USER + 0x0001		// ���鴰��,����ѡ����Ϣ
#define	TRAY_MENU					WM_USER + 0x0002		// ���̲˵������Ϣ
#define DOWNLOAD_ENTER_ROOM			WM_USER + 0x0003		// ��Ϸ������ɽ��뷿��
#define WM_UNINSTALL_USERMENU		WM_USER + 0x0004		// ж����ҿ��Ʋ˵���Ϣ

#define IPC_SUB_HEARTBEAT			0x1979					// ������Ϣ
#define IPC_SUB_START_FAILED		0x0005					// ����ʧ��
#define	IPC_SUB_CLOSE_FRAME_EX		0x0006					// ��Ϸǿ�йر�
#define IPC_SUB_GF_USER_READY		0x0007					// ��Ϸ�û�ͬ��
#define IPC_SUB_GF_RECONNECT		0x0008					// ��Ϸ��������
#define IPC_SUB_KICK_USER			0x0009					// �߳��û�
#define IPC_SUB_SEND_CHAT			0x0010					// ����������Ϣ
#define IPC_USER_SUFFIX				0x0011					// ���ӵ������Ϣ�ṹ

#define IPC_FLAG_TO_SERVER			0x0000                  // ���ݷ��͸�������
#define IPC_FLAG_TO_GAMEPLAZA		0x0001                  // ���ݷ��͸�����
#define IPC_FLAG_TO_GAME			0x0002					// ���ݷ��͸���Ϸ
#define IPC_FLAG_TO_WINFROM			0x0003					// ���ݷ��͸�Winform

// Winform�����Ϣ
#define IPC_SUB_WINGAME_START_OK	0x1001					// WinForm�����ɹ�
#define IPC_SUB_WINGAME_START_NO	0x1002					// WinForm����ʧ��
#define IPC_SUB_WINGAME_GAMEPROCESS	0x1003					// ������Ϸ������Ϣ
#define IPC_SUB_WINGAME_START_INFO	0x1004					// ��Ϸ����������ʼ��
#define IPC_SUB_WINGAME_RESIZE		0x2001					// ��Ϸ���ڴ�С�����ı�
#define IPC_SUB_WINGAME_MENU_INIT	0x2002					// ��Ϸ�˵���ʼ��
#define IPC_SUB_WINGAME_MENU_CONFIG 0x2003					// ��Ϸ�˵�����

// new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

// �ַ���
#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

// ��ʱ��
// ʱ��
#define TIME_HEARTBEAT				2000			// ����Ϸ���̵�����ʱ����(��λ:ms)
#define TIME_LOCK_MACHINE			2000			// ��������������ť������(��λ:ms)
#define TIME_HEART_MAX_OUT			30				// ������ʱ������
#define TIME_HEART_MIN_OUT			3				// ������ʱ��С����
#define TIME_TOAST_PERFRAME			100				// toastÿ�μ��ʱ��
#define TIME_TOAST_OVER				1500			// toast�Զ���ʧʱ��
#define TIME_WAITTING_OVER			30000			// waitting ʱ��
#define TIME_GET_VERTCODE			60				// ��ȡ��֤����ʱ��(60��)
#define RECONNECT_INTERVAL			6000			// �����쳣����ʱ����(��λ:����)
#define TIME_START_GAME_FINISH		10				// ������Ϸ��Ϻ���ʱ�����б�����(��λ:����)

// ID
#define TIMER_ID_GET_VERTCODE		0x0001			// ��֤�붨ʱ�� I D
#define TIMER_ID_LOGINEND_SERVER	0x0100			// ����˵�¼���
#define TIMER_ID_CREATEHALL			0x0102			// ��������
#define TIMER_ID_LOGINEND_CLIENT	0x0200			// �ͻ��˵�¼���
#define	TIMER_ID_HALL_ROOL_TEXT		0x0300			// �������������ı�
#define TIMER_ID_ZOOMBOX			0x0400			// ����������ѡ��
#define	TIMER_ID_HALL_LOCK_MACHINE	0x0500			// ������������
#define TIMER_ID_RECONNECT			0x0600			// �����쳣������ʱ��
#define TIMER_ID_START_GAME_FINISH	0x0700			// ������Ϸ��϶�ʱ��
#define TIMER_ID_HEARTBEAT			0x1979			// ������ʱ�� I D 

// ����
#define DEFAULT_LOCAL_HEAD_COUNT	30				// Ĭ�ϱ���ͷ�����(��Ů��ռһ��,�±��0��ʼ)
#define SMS_VERTCODE_FIGURES		4				// ������֤��λ��
#define CANDIDATE_ACCOUNT_NUM		4				// ��ѡ�˺����޸���
#define DOWNLOAD_TASK_MAX_COUNT		5				// ͬʱ���ض�������������
#define RECONNECT_MAX_COUNT			10				// �����쳣����������


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

// ����LPCTSTR��TCHAR������(����С���)
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