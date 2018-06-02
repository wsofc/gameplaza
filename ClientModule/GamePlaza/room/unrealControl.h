#ifndef _UNREAL_
#define _UNREAL_

//#include <atlstr.h>
#include "..\\share\\control\\IUnrealControl.h"
typedef void(*ProcCreateUnreal)(IUnrealControl*, DataManager*, ServiceData*, GameResource*);
typedef void(*ProcDeleteUnreal)();
typedef void(*ProcLoadUnreal)();
typedef void(*ProcInitUnreal)(UserData*);
typedef void(*ProcParseUnreal)(UserData*);
typedef void(*ProcCleanUnreal)(UserData*);
typedef void(*ProcCheckUnrealRobot)(bool);
typedef void(*ProcCheckUnrealUser)(UnrealNSP::UnrealChargeType, LONGLONG, LPCTSTR, BYTE);
typedef bool(*ProcCheckUnrealReturn)(DWORD, DWORD);
typedef bool(*ProcIsMachineIDSwitchOn)();
typedef void(*ProcGetMachineID)(TCHAR(&buffer)[COMPUTER_ID_LEN]);

class UserData;

class UnrealControl : public IUnrealControl
{
public:
	static UnrealControl* GetInstance();

	// �Ƿ��Ѿ�������DLL
	bool IsLoadUnrealDll(){ return m_bLoadSuccess; }

	// ����DLL
	bool LoadUnrealDll();

	// ж��DLL
	void UnLoadUnrealDll();

	// Unreal Check
	void LoadUnreal();
	void InitUnreal(UserData* pUser);
	void ParseUnreal(UserData* pUser);
	void CleanUnreal(UserData* pUser);
	void CheckUnreal_Robot();
	void CheckUnreal_User(UnrealNSP::UnrealChargeType uStyle, LONGLONG lGolds, LPCTSTR lpEncryptPW, BYTE cbSendLocation);
	bool CheckUnreal_Return(DWORD dwRecvGameID, DWORD dwRecvID);
	bool IsMachineIDSwitchOn();
	void GetMachineID(TCHAR(&buffer)[COMPUTER_ID_LEN]);

public:
	void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
	DWORD SendData(BYTE cbSendLocation, WORD wMainCmdID, WORD wSubCmdID);
	DWORD SendData(BYTE cbSendLocation, WORD wMainCmdID, WORD wSubCmdID, void * const pData, WORD wDataSize);

private:
	UnrealControl();
	~UnrealControl();
	static UnrealControl*			g_instance;					// ����

	bool							m_bLoadSuccess;				// �Ƿ���سɹ�
	TCHAR							m_pzBuffer[MAX_PATH];		// �ַ���������
	HINSTANCE						m_hInst;					// ��̬����
	
	ProcCreateUnreal				m_pFuncCreateUnreal;
	ProcDeleteUnreal				m_pFuncDeleteUnreal;
	ProcLoadUnreal					m_pFuncLoadUnreal;
	ProcInitUnreal					m_pFuncInitUnreal;
	ProcParseUnreal					m_pFuncParseUnreal;
	ProcCleanUnreal					m_pFuncCleanUnreal;
	ProcCheckUnrealRobot			m_pFuncCheckUnrealRobot;
	ProcCheckUnrealUser				m_pFuncCheckUnrealUser;
	ProcCheckUnrealReturn			m_pFuncCheckUnrealReturn;
	ProcIsMachineIDSwitchOn			m_pFuncIsMachineIDSwitchOn;
	ProcGetMachineID				m_pFuncGetMachineID;
};

#endif // _USER_CONTROL_