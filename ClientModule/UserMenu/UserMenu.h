#pragma once

#ifdef  USERMENU_EXPORTS
	#define USER_MENU_API _declspec(dllexport)
	#define USER_MENU_API_C extern "C" _declspec(dllexport)
#else
	#define USER_MENU_API _declspec(dllimport)
	#define USER_MENU_API_C extern "C" _declspec(dllimport)
#endif

//ģ�鶨��
#ifdef _DEBUG
#	define USER_MENU_CLASS_DLL_NAME	TEXT("UserMenu_d.dll")			// ��� DLL ����
#else
#	define USER_MENU_CLASS_DLL_NAME	TEXT("UserMenu.dll")			// ��� DLL ����
#endif

namespace UserMenuNSP {
	
	// �Ի���IDö��
	enum MsgEventID
	{
		MSGID_NONE	= 0,									// ��
		MSGID_USER_GAMEID,									// ��ҵ���ϷID
		MSGID_SCORE_SETTING,								// ������Ӯ����
		MSGID_WIN_COUNT,									// Ӯ�ľ���
		MSGID_LOST_COUNT,									// ��ľ���
		MSGID_KICK_ROOM,									// �߳�����
		MSGID_SEND_SYSMSG,									// ����ϵͳ��Ϣ
		MSGID_FORBID_ACCOUNT,								// �߳�����
		MSGID_SEND_SYSMSG_RANG,								// ����ϵͳ��Ϣ��ѡ��Χ
		MSGID_ROOM_SETTING,									// ��������
		MSGID_STOKE_SETTING,								// �������
		MSGID_ROBOT_COUNT,									// ����������
		MSGID_ROBOT_SCORE_MAX,								// ��������������Χ
		MSGID_ROBOT_SCORE_MIN,								// ��������С������Χ
		MSGID_EXTRA											// ����Ԥ����
	};
	
	// ���ݽṹ
	struct MenuDataStruct
	{
		bool			bSelf;								// �Ƿ����Լ�
		DWORD			dwUserID;							// �û�I D
		DWORD			dwGameID;							// ��ϷI D
		LONGLONG		lGameGold;							// ���
		LONGLONG		lScore;								// �û�����
		TCHAR			szNickname[NAME_LEN];				// �ǳ�
		TCHAR			szUnderWrite[UNDER_WRITE_LEN];		// ����ǩ��
		
		LONGLONG		lCurWinScore;						// ��ǰ���õķ���
		LONGLONG		lCurWinCount;						// ��ǰӮ����
		LONGLONG		lCurLostCount;						// ��ǰ�伸��

		LONGLONG		lCurStokValue;						// ��ǰ���ֵ

		LONGLONG		lCurRobotCount;						// ��ǰ����������
		LONGLONG		lCurRobotMax;						// ��ǰ������������
		LONGLONG		lCurRobotMin;						// ��ǰ��������С����

		RoomSetData		tRoomSetData;						// ��������
		ResInfo			tResInfo;							// ��Դ��Ϣ

		MenuDataStruct()
		{
			memset(this, 0, sizeof(this));
		}
	};
	
	// ����ƥ��
	bool MatchRegular(CDuiString& strIn, LPCTSTR lpReg);

	// �����˵�
	USER_MENU_API_C void CreatUserMenu(CPaintManagerUI* pManager, ResInfo* pResInfo, UserMenuStruct* pUserStruct, IUserControl* pDelegate);
	
	// ж�ز˵�
	USER_MENU_API_C void DeleteUserMenu(HWND hWnd);

	// ���յ�ǰ��Ӯ����
	USER_MENU_API_C void ReciveWinScore(LONGLONG lCurWinScore);

	// ���յ�ǰ���Ӯ����
	USER_MENU_API_C void ReciveWinCount(LONGLONG lCurWinCount);
	
	// ���յ�ǰ����伸��
	USER_MENU_API_C void ReciveLostCount(LONGLONG lCurLostCount);

	// ���յ�ǰ��������
	USER_MENU_API_C void ReciveRoomSetting(RoomSetData& tRoomSetData);

	// ���յ�ǰ�������
	USER_MENU_API_C void ReciveStokSetting(LONGLONG lCurStokValue);
	
	// ���յ�ǰ����������
	USER_MENU_API_C void ReciveRobotCount(LONGLONG lCurRobotCount);
	
	// ���յ�ǰ�����˷���
	USER_MENU_API_C void ReciveRobotScore(LONGLONG lCurRobotMax, LONGLONG lCurRobotMin);
}

#include "frame\\PanelScore.h"
#include "frame\\WinCount.h"
#include "frame\\LostCount.h"
#include "frame\\SendSysMsg.h"
#include "frame\\RoomSetting.h"
#include "frame\\StokSetting.h"
#include "frame\\RobotCount.h"
#include "frame\\RobotScore.h"

#include "frame\\UserMenuFrame.h"