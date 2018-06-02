#include "stdafx.h"
#include "UserMenu.h"

namespace UserMenuNSP {

	extern MenuDataStruct 	g_tMenuData;
	extern IUserControl* 	g_pDelegate;

	bool MatchRegular(CDuiString& strIn, LPCTSTR lpReg)
	{
		bool bResult = false;
		if (strIn.GetLength() <= 0 || lpReg == nullptr)
		{
			return bResult;
		}

		std::wregex pattern;
		LPWSTR cstr_words = nullptr;
#ifdef UNICODE || _UNICODE
		pattern = lpReg;
		cstr_words = strIn.GetData();
		bResult = std::regex_match(cstr_words, pattern);
#else
		LPWSTR cstr_reg = StringConvertor::AnsiToWide(lpReg);
		pattern = cstr_reg;

		cstr_words = StringConvertor::AnsiToWide(strIn.GetData());
		bResult = std::regex_match(cstr_words, pattern);

		StringConvertor::StringFree(cstr_words);
		StringConvertor::StringFree(cstr_reg);

#endif // UNICODE || _UNICODE

		return bResult;
	}

	USER_MENU_API_C void CreatUserMenu(CPaintManagerUI* pManager, ResInfo* pResInfo, UserMenuStruct* pUserStruct, IUserControl* pDelegate)
	{
		// 拷贝资源信息
		memcpy(&g_tMenuData.tResInfo, pResInfo, sizeof(ResInfo));

		// 拷贝默认显示的玩家数据
		g_tMenuData.bSelf = pUserStruct->bSelf;
		g_tMenuData.dwUserID = pUserStruct->dwUserID;
		g_tMenuData.dwGameID = pUserStruct->dwGameID;
		g_tMenuData.lGameGold = pUserStruct->lGameGold;
		g_tMenuData.lScore = pUserStruct->lScore;
		memcpy(g_tMenuData.szNickname, pUserStruct->szNickname, ArrayCount(pUserStruct->szNickname) * sizeof(TCHAR));
		memcpy(g_tMenuData.szUnderWrite, pUserStruct->szUnderWrite, ArrayCount(pUserStruct->szUnderWrite) * sizeof(TCHAR));
		g_pDelegate = pDelegate;

		// 创建菜单
		bool bCreatFrame = false;
		bCreatFrame = !UserMenuNSP::UserMenu::IsExistInstance();
		UserMenu* pUserMenu = UserMenuNSP::UserMenu::GetInstance();
		if (bCreatFrame)
		{
			pUserMenu->CreatFrame(pManager);
		}
		else
		{
			pUserMenu->CreatUserMenu(pManager);
		}
	}

	USER_MENU_API_C void DeleteUserMenu(HWND hWnd)
	{
		if (UserMenuNSP::UserMenu::IsExistInstance())
		{
			UserMenu* pUserMenu = UserMenuNSP::UserMenu::GetInstance();
			pUserMenu->Release();
		}
		else
		{
			// 发送卸载Dll消息
			::PostMessage(hWnd, WM_UNINSTALL_USERMENU, NULL, NULL);
		}
	}

	USER_MENU_API_C void ReciveWinScore(LONGLONG lCurWinScore)
	{
		g_tMenuData.lCurWinScore = lCurWinScore;
		if (PanelScore::IsExistInstance())
		{
			PanelScore* pInstance = PanelScore::GetInstance();
			pInstance->UpdateLastData();
		}
	}

	USER_MENU_API_C void ReciveWinCount(LONGLONG lCurWinCount)
	{
		g_tMenuData.lCurWinCount = lCurWinCount;
		if (WinCount::IsExistInstance())
		{
			WinCount* pInstance = WinCount::GetInstance();
			pInstance->UpdateLastData();
		}
	}

	USER_MENU_API_C void ReciveLostCount(LONGLONG lCurLostCount)
	{
		g_tMenuData.lCurLostCount = lCurLostCount;
		if (LostCount::IsExistInstance())
		{
			LostCount* pInstance = LostCount::GetInstance();
			pInstance->UpdateLastData();
		}
	}

	USER_MENU_API_C void ReciveRoomSetting(RoomSetData& tRoomSetData)
	{
		memcpy(&g_tMenuData.tRoomSetData, &tRoomSetData, sizeof(RoomSetData));
		if (RoomSetting::IsExistInstance())
		{
			RoomSetting* pInstance = RoomSetting::GetInstance();
			pInstance->UpdateLastData();
		}
	}

	USER_MENU_API_C void ReciveStokSetting(LONGLONG lCurStokValue)
	{
		g_tMenuData.lCurStokValue = lCurStokValue;
		if (StokSetting::IsExistInstance())
		{
			StokSetting* pInstance = StokSetting::GetInstance();
			pInstance->UpdateLastData();
		}
	}

	USER_MENU_API_C void ReciveRobotCount(LONGLONG lCurRobotCount)
	{
		g_tMenuData.lCurRobotCount = lCurRobotCount;
		if (RobotCount::IsExistInstance())
		{
			RobotCount* pInstance = RobotCount::GetInstance();
			pInstance->UpdateLastData();
		}
	}

	USER_MENU_API_C void ReciveRobotScore(LONGLONG lCurRobotMax, LONGLONG lCurRobotMin)
	{
		g_tMenuData.lCurRobotMax = lCurRobotMax;
		g_tMenuData.lCurRobotMin = lCurRobotMin;
		if (RobotScore::IsExistInstance())
		{
			RobotScore* pInstance = RobotScore::GetInstance();
			pInstance->UpdateLastData();
		}
	}
}