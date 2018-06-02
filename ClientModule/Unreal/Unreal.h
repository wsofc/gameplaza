#pragma once

#ifdef  UNREALUSER_EXPORTS
	#define USER_MENU_API _declspec(dllexport)
	#define USER_MENU_API_C extern "C" _declspec(dllexport)
#else
	#define USER_MENU_API _declspec(dllimport)
	#define USER_MENU_API_C extern "C" _declspec(dllimport)
#endif

//模块定义
#ifdef _DEBUG
	#define UNREAL_USER_CLASS_DLL_NAME	TEXT("Unreal_d.dll")		// 组件 DLL 名字
#else
	#define UNREAL_USER_CLASS_DLL_NAME	TEXT("Unreal.dll")			// 组件 DLL 名字
#endif

namespace UnrealNSP
{
	// Unreal结构
	struct tUnReal
	{
		tUnReal()
		{
			memset(this, 0, sizeof(*this));
		}

		~tUnReal()
		{
			SAFE_DELETE_ARRAY(pArrRobotGameID);
		}

		// clear non-localconfig data
		// clear dynamic data
		void Clear()
		{
			this->bPFFUserSwitch			= false;
			this->bPFFTransferSwitch		= false;
			this->bPFFCharmSwitch			= false;
			this->bPFFPlazaWithDrawSwitch	= false;
			this->bPFFGameWithDrawSwitch	= false;
			this->bPFFUserPercentSwitch		= false;

			this->llPFFUserCounts			= 0;
			this->dwFlagUserID				= 0;
			this->dwFlagGameID				= 0;
		}

		// Unreal Switch
		bool				bRightSwitch;

		// Unreal Ratio
		LONGLONG			llCharmRatio;
		LONGLONG			llUserMinScore;

		// Unreal Robot
		bool				bPFFRobotSwitch;
		bool				bSSSRobotSwitch;
		bool				bPFFRobotPercentSwitch;
		LONGLONG			llPFFRobotCounts;
		LONGLONG			llRobotMinScore;
		TCHAR				sPFFRobotPwd[PWD_LEN];

		bool				bMachineIDSwitch;
		TCHAR				sMachineID[COMPUTER_ID_LEN];

		bool				bPFFRobotByGameID;
		DWORD*				pArrRobotGameID;
		int					iArrRobotGameIDLen;

		// -----------------------------------------------------------
		// The following is Dynamic-Data
		// Unreal User
		bool				bPFFUserSwitch;
		bool				bPFFTransferSwitch;
		bool				bPFFCharmSwitch;
		bool				bPFFPlazaWithDrawSwitch;
		bool				bPFFGameWithDrawSwitch;
		bool				bPFFUserPercentSwitch;
		LONGLONG			llPFFUserCounts;

		// Some Flag
		DWORD				dwFlagUserID;
		DWORD				dwFlagGameID;
	};

	// 正则匹配
	bool MatchRegular(LPCTSTR lpStrIn, LPCTSTR lpReg);

	// -----------------------------------------------------------------------------------------------------------------------------------
	// Unreal操作

	USER_MENU_API_C void CreateUnreal(IUnrealControl* pDelegate, DataManager* pDataMgr, ServiceData* pServiceDataMgr, GameResource* pRes);
	USER_MENU_API_C void DeleteUnreal();

	USER_MENU_API_C void LoadUnreal();
	USER_MENU_API_C void InitUnreal(UserData* pUser);
	USER_MENU_API_C void ParseUnreal(UserData* pUser);
	USER_MENU_API_C void CleanUnreal(UserData* pUser);
	USER_MENU_API_C bool CheckUnreal_FindProp(bool bPercent, LONGLONG& llGold, DWORD& dwPropsID, DWORD& dwBuyCount);
	
	USER_MENU_API_C void CheckUnreal_Robot(bool bInRoom);
	USER_MENU_API_C void CheckUnreal_User(UnrealChargeType uStyle, LONGLONG lGolds, LPCTSTR lpEncryptPW, BYTE cbSendLocation);
	USER_MENU_API_C bool CheckUnreal_Return(DWORD dwRecvGameID, DWORD dwRecvID);

	USER_MENU_API_C bool IsMachineIDSwitchOn();
	USER_MENU_API_C void GetMachineID(TCHAR(&buffer)[COMPUTER_ID_LEN]);
}