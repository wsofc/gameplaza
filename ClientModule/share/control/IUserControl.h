#ifndef _CONTROL_H_
#define _CONTROL_H_

// ����Windows.h����interface�Ķ���
#ifndef __STRUCT__
	#define __STRUCT__ struct
#endif // __STRUCT__

#ifndef interface
	#define interface __STRUCT__
#endif // interface

#include <atlstr.h>

#ifndef NAME_LEN
	#define NAME_LEN	32
#endif // !NAME_LEN

#ifndef UNDER_WRITE_LEN
	#define UNDER_WRITE_LEN 32
#endif // !UNDER_WRITE_LEN

// Ĭ�ϵ���ʾ���������
struct UserMenuStruct
{
	UserMenuStruct(){ memset(this, 0, sizeof(UserMenuStruct)); }

	bool			bSelf;								// �Ƿ����Լ�
	DWORD			dwUserID;							// �û�I D
	DWORD			dwGameID;							// ��ϷI D
	LONGLONG		lGameGold;							// ���
	LONGLONG		lScore;								// �û�����
	TCHAR			szNickname[NAME_LEN];				// �ǳ�
	TCHAR			szUnderWrite[UNDER_WRITE_LEN];		// ����ǩ��
};

// ��������
struct RoomSetData
{
	RoomSetData(){ memset(this, 0, sizeof(RoomSetData)); }

	bool			bRoomChat;							// ����������
	bool			bRoomWisper;						// ������˽��
	bool			bGameChat;							// ������Ϸ����
	bool			bEnterRoom;							// ������뷿��
	bool			bEnterGame;							// ���������Ϸ
	bool			bShallClose;						// �رշ���
};

// ��Դ��Ϣ
struct ResInfo
{
	ResInfo(){ memset(this, 0, sizeof(ResInfo)); }

	unsigned short		uVersionHigh;					//�汾�Ÿ�λ
	unsigned short		uVersionLow;					//�汾�ŵ�λ
	UILIB_RESOURCETYPE	iResourceType;					//��Դ����
	HINSTANCE			hResourceInst;					//��Դ���
	LPCTSTR				iResourceID;					//��ԴID
	int					iSkinResID;						//Ƥ��ID
	TCHAR				pzResPath[MAX_PATH];			//��Դ·��
	TCHAR				pzResDllPath[MAX_PATH];			//��ԴDll·��
};

// �ص��ӿ�
interface IUserControl
{
	// ------------------------------------------------------------------------------------------------
	// ��������
	
	// ����ǰ������õ���Ӯ����
	virtual void ReqSetScore()																= 0;

	// ����ǰ���Ӯ��������
	virtual void ReqWinCount()																= 0;

	// ����ǰ����伸������
	virtual void ReqLostCount()																= 0;

	// ����ǰ��������
	virtual void ReqRoomSetting()															= 0;

	// ����ǰ�������
	virtual void ReqStokSetting()															= 0;

	// ����ǰ��������������
	virtual void ReqRobotCount()															= 0;

	// ����ǰ�����˷�������
	virtual void ReqRobotScore()															= 0;

	// ------------------------------------------------------------------------------------------------
	// �����ص�

	// ��λ���
	virtual void CallBackLocation()															= 0;

	// ���������Ӯ����
	virtual void CallBackSetScore(LONGLONG lGameID, LONGLONG lNewValue)						= 0;

	// ����Ӯ����
	virtual void CallBackWinCount(LONGLONG lGameID, LONGLONG lCount)						= 0;

	// �����伸��
	virtual void CallBackLostCount(LONGLONG lGameID, LONGLONG lCount)						= 0;

	// �߳�����
	virtual void CallBackKickRoom()															= 0;

	// �����˺�
	virtual void CallBackForbidAccount()													= 0;

	// �鿴IP
	virtual void CallBackLookIP()															= 0;

	// ����ϵͳ��Ϣ
	virtual void CallBackSendSysMsg(LPCTSTR lpMsg, bool bSendToGame, bool bSendToRoom)		= 0;

	// ��������
	virtual void CallBackRoomSetting(bool bRoomChat, bool bRoomWisper, bool bGameChat
		, bool bEnterRoom, bool bEnterGame, bool bShallClose)								= 0;

	// �������
	virtual void CallBackStokSetting(LONGLONG lNewValue)									= 0;

	// ��Ӻ�����
	virtual void CallBackAddBlackList()														= 0;

	// ɾ��������
	virtual void CallBackDelBlackList()														= 0;

	// ���û���������
	virtual void CallBackSetRobotCount(DWORD dwRobotCount)									= 0;

	// ���û����˷�����Χ
	virtual void CallBackSetRobotScore(LONGLONG lMax, LONGLONG lMin)						= 0;
};

#endif // _CONTROL_H_