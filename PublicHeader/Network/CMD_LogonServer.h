/*

REQ -- request 请求
ANW -- answer  应答

*/

#pragma once


//广场版本
#define VER_PLAZA_LOW					2								//广场版本
#define VER_PLAZA_HIGH					16								//广场版本
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)


//-----------------------------------------------
//登录错误标识

#define ERC_GP_LOGON_SUCCESS			0								//登陆成功
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//帐号不存在
#define ERC_GP_LONG_NULLITY				2								//禁止登录
#define ERC_GP_PASSWORD_ERCOR			3								//密码错误


//-----------------------------------------------
//登录命令码

#define MAIN_LS_LOGON					0x2000								//广场登录

#define SUB_LS_LOGON_ACCOUNT			0x2001							//帐号登录
#define SUB_LS_LOGON_GAME_ID				0x2002						//I D 登录
#define SUB_LS_REGISTER_ACCOUNT			0x2003						//注册帐号

#define SUB_LS_LOGON_SUCCESS			0x2005							//登陆成功
#define SUB_LS_LOGON_ERROR				0x2006							//登陆失败
#define SUB_LS_LOGON_FINISH				0x2007							//登陆完成

#define SUB_LS_REGISTER_SUCCESS			0x2008							//注册成功
#define SUB_LS_REGISTER_ERROR			0x2009							//注册失败

#define SUB_C2LS_CHECKINGSMS						0x2101								// 核查短信验证
#define SUB_C2LS_GETSMS									0x2102								// 获取短信验证
#define SUB_LS2C_NEEDSMSCODE						0x2103								// 需要核查短信验证
#define SUB_LS2C_SENDSMSSUCCESS					0x2104								// 已发送sms成功
#define SUB_LS2C_SMS_ERROR							0x2105								// 短信失败

#define SUB_LS2C_PASS										0x2106								// 已发送短信验证码

#define SUB_C2LS_GETPASSWORD						0x2110								//  找回密码
#define SUB_LS2C_GETPASSWORD_SSUCCESS		0x2111								//  找回密码成功
#define SUB_LS2C_GETPASSWORD_ERROR			0x2112								//  找回密码失败

#define SUB_C2LS_CHECKACCOUNT						0x2115								//  申请核查账号是否可用
#define SUB_C2LS_CHECKACCOUNT_SSUCCESS		0x2116								//  申请核查账号是否可用成功
#define SUB_C2LS_CHECKACCOUNT_ERROR			0x2117								//  申请核查账号是否可用失败

#define SUB_C2LS_GETSERVERINFO_REQ				0x2118								//  申请更新服务器信息

#define SUB_C2LS_GETUSERINFO_REQ					0x2119								//  申请获取玩家信息
#define SUB_LS2C_GETUSERINFO							0x2120								//  玩家信息内容
#define SUB_LS2C_GETUSERINFO_ERROR				0x2121								//  玩家信息内容失败

#define SUB_C2LS_GETUSERGOLD_REQ					0x3000								//  申请获取玩家金币信息
#define SUB_LS2C_GETUSERGOLD							0x3001								//  玩家金币信息内容
#define SUB_LS2C_GETUSERGOLD_ERROR				0x3002								//  玩家信息金币内容失败

// 
struct tag_C_GetSMS
{
	tag_C_GetSMS()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE					btType;										// 操作类型	1注册；2修改密码；3修改手机；4登陆；5取款；6转账; 7解绑定;8登陆
	TCHAR				szPhone[PHONE_LEN];				// 手机号
	TCHAR				szAccount[NAME_LEN];				//登录帐号
};

// 刷新 玩家金币信息
struct tag_S_UserGold
{
	tag_S_UserGold()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD						dwUserID;					// 玩家USERID
	LONGLONG					lGameGold;				// 游戏金币
	LONGLONG					lBankGold;				// 银行金币
	LONGLONG					lCharm;						// 魅力
};

// 请求玩家金币信息
struct tag_C_GetUserGold_REQ
{
	tag_C_GetUserGold_REQ()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD						dwUserID;					// 玩家USERID
};

struct tag_S_ErrorMsg
{
	tag_S_ErrorMsg()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE					btErrorCode;											// 错误码
	TCHAR				szMsg[RESULT_CARD_TYPE];				//描述
};

//帐号登录
struct tagLS_LogonByAccount
{
	tagLS_LogonByAccount()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD			dwPlazaVersion;					//广场版本
	TCHAR				szSMS[SMS_LEN];					// 短信认证码
	TCHAR				szAccount[NAME_LEN];			//登录帐号
	TCHAR				szPwd[PWD_LEN];					//登录密码
};

//  找回密码
struct tagLS_GetPassword
{
	tagLS_GetPassword()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR				szSMS[SMS_LEN];					// 短信认证码
	TCHAR				szAccount[NAME_LEN];			//登录帐号
	TCHAR				szNewPwd[PWD_LEN];			//新登录密码
};

// 获取服务器列表
struct tagLS_GetServerInfo
{
	tagLS_GetServerInfo()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE				btPlatform;					// 机器设备
};

//I D 登录
struct tagLS_LogonByGameID
{
	tagLS_LogonByGameID()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwPlazaVersion;					//广场版本
	DWORD				dwGameID;							//游戏 I D
	TCHAR					szPwd[PWD_LEN];					//登录密码
};

struct tagLS_CheckAccount
{
	tagLS_CheckAccount()
	{
		memset(this, 0,sizeof(*this));
	}
	BYTE					btType;		// 1:账号， 2：昵称
	TCHAR				szAccount[NAME_LEN];					//帐号
};


struct tagLS_GetUserInfoByGameID
{
	tagLS_GetUserInfoByGameID()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD			dwGameID;
	DWORD			dwUserID;
	TCHAR				szAccount[NAME_LEN];
	TCHAR				szNickname[NAME_LEN];
};

//注册帐号
struct tagLS_RegisterAccount
{
	tagLS_RegisterAccount()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wFaceID;										//头像标识
	BYTE					cbGender;										//用户性别
	DWORD			dwPlazaVersion;							//广场版本
	TCHAR				szSpreader[NAME_LEN];				//推广人名
	TCHAR				szAccount[NAME_LEN];					//帐号
	TCHAR				szNickname[NAME_LEN];				//昵称
	TCHAR				szPwd[PWD_LEN];							//登录密码
	TCHAR				szPhoneNumber[PHONE_LEN];		//手机号

	TCHAR				szQQNumber[15];						//QQ号码
	TCHAR				szRealName[NAME_LEN];			//真实姓名
	TCHAR				szIDCard[IDCARD_LEN];			// 身份证号码
	TCHAR				szEmail[22];								//邮箱
	TCHAR				szPwdProtectQuestion[32];		//问题
	TCHAR				szPwdProtectAnswer[32];			//答案
	TCHAR				szSMS[SMS_LEN];					//短信认证码
};


//登陆成功
struct tagLS_LogonSuccess
{
	tagLS_LogonSuccess()
	{
		memset(this,0,sizeof(*this));
	}
	WORD					wFaceID;							//头像索引
	BYTE						cbGender;							//用户性别
	BYTE						cbMember;						//会员等级
	DWORD				dwUserID;							//用户 I D
	DWORD				dwGameID;						//游戏 I D
	DWORD				dwExperience;					//用户经验
	LONGLONG			dwCharm;							 //魅力

	LONGLONG            lGold;								//金币
	LONGLONG            lBankGold;						//银行金币

	//扩展信息
	DWORD				dwCustomFaceVer;				//头像版本
	BYTE						cbMoorMachine;					//锁定机器
	BYTE						cbMasterOrder;						//管理等级
};

//登陆失败
struct tagLS_LogonError
{
	tagLS_LogonError()
	{
		memset(this,0,sizeof(*this));
	}
	LONG				lErrorCode;						//错误代码
	TCHAR				szErrorDescribe[128];			//错误消息
};



//-----------------------------------------------
//游戏列表命令码

#define MAIN_LS_SERVER_LIST				0x2100							//列表信息

#define SUB_LS_LIST_TYPE				0x2101							//类型列表
#define SUB_LS_LIST_KIND				0x2102							//种类列表
#define SUB_LS_LIST_STATION				0x2103							//站点列表
#define SUB_LS_LIST_GAME_SERVER			0x2104							//房间列表
#define	SUB_LS_LIST_HALL_SERVER			0x2105				
#define SUB_LS_LIST_FINISH				0x2106							//发送完成
#define SUB_LS_LIST_CONFIG				0x2107							//列表配置

//列表配置
struct tagLS_ListConfig
{
	tagLS_ListConfig()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE				bShowOnlineCount;				//显示人数
};


//-----------------------------------------------
//系统命令码

#define MAIN_LS_SYSTEM					0x2200								//系统命令

#define SUB_LS_VERSION						0x2201								//版本通知
#define SUB_LS_MAINTENANCE			0x2202								//系统维护
#define SUB_LS_VERSION_REQ				0x2203								// 版本检测申请

//版本通知
struct tagLS_Version
{
	tagLS_Version()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE					bNewVersion;					//更新版本
	BYTE					bAllowConnect;					//允许连接
	WORD				wServerLow;
	WORD				wServerHigh;

};
//版本检测申请
struct tagLS_VersionReq
{
	tagLS_VersionReq()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD			dwPlazaVersion;					//广场版本
};


//系统维护
struct tagLS_Maintenance
{
	tagLS_Maintenance()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wHintMessageLen;
	TCHAR				szHintMessage[256];
};


//-----------------------------------------------
//
#define MAIN_LS_USER						0x2300					// 用户信息

#define SUB_LS_USER_UPLOAD_FACE				0x2301					// 上传头像
#define SUB_LS_USER_DOWNLOAD_FACE			0x2302					// 下载头像
#define SUB_LS_UPLOAD_FACE_RESULT			0x2303					// 上传结果
#define SUB_LS_DELETE_FACE_RESULT			0x2304					// 删除结果
#define SUB_LS_CUSTOM_FACE_DELETE			0x2305					// 删除头像

#define SUB_LS_USERINFO_GET					0x2310					// 获取个人信息
#define SUB_LS_USERINFO_GET_OK				0x2311					// 获取个人信息成功
#define SUB_LS_USERINFO_GET_ERROR			0x2312					// 获取个人信息失败

#define SUB_LS_USERINFO_UPDATE				0x2320					// 修改个人信息
#define SUB_LS_USERINFO_UPDATE_OK			0x2321					// 修改个人信息成功
#define SUB_LS_USERINFO_UPDATE_ERROR		0x2322					// 修改个人信息失败

#define SUB_LS_USERINFO_MODTEL				0x2330					// 修改手机号
#define SUB_LS_USERINFO_MODTEL_OK			0x2331					// 修改手机号成功
#define SUB_LS_USERINFO_MODTEL_ERROR		0x2332					// 修改手机号失败

#define SUB_LS_USERINFO_MODPW				0x2340					// 修改密码
#define SUB_LS_USERINFO_MODPW_OK			0x2341					// 修改密码成功
#define SUB_LS_USERINFO_MODPW_ERROR			0x2342					// 修改密码失败

#define SUB_LS_USER_LOCK_MACHINE			0x2350					// 锁定本机
#define SUB_LS_USER_LOCK_MACHINE_OK			0x2351					// 锁定本机成功
#define SUB_LS_USER_LOCK_MACHINE_ERROR		0x2352					// 锁定本机失败

#define SUB_LS_USER_UNLOCK_MACHINE			0x2360					// 解锁本机
#define SUB_LS_USER_UNLOCK_MACHINE_OK		0x2361					// 解锁本机成功
#define SUB_LS_USER_UNLOCK_MACHINE_ERROR	0x2362					// 解锁本机失败

#define MAIN_LS_SMS							0x3000					// 短信


//修改玩家信息
struct tagLS_UpdateUserInfo
{
	tagLS_UpdateUserInfo()
	{
		memset(this, 0, sizeof(*this));
	}
	WORD				wFaceID;										// 头像标识
	BYTE					cbGender;										// 用户性别
	TCHAR				szAccount[NAME_LEN];					// 帐号
	TCHAR				szPwd[PWD_LEN];							// 登录密码

	TCHAR				szNickname[NAME_LEN];				// 昵称
	TCHAR				szRealname[NAME_LEN];				// 真实姓名
	TCHAR				szEmail[EMAIL_LEN];						// 邮箱
	TCHAR				szTelphone[PHONE_LEN];				// 电话号码
	TCHAR				szQQNumber[PHONE_LEN];			// QQ号码
	TCHAR				szIDCard[IDCARD_LEN];				// 身份证
	TCHAR				szUnderWrite[UNDER_WRITE_LEN];			// 个性签名
};

//修改手机号
struct tagLS_ModTelphone
{
	tagLS_ModTelphone()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR				szAccount[NAME_LEN];			//登录帐号
	TCHAR				szPwd[PWD_LEN];					//登录密码
	TCHAR				szSMS[SMS_LEN];					//短信认证码
	TCHAR				szOldTel[PHONE_LEN];			//旧手机号
	TCHAR				szNewTel[PHONE_LEN];			//新手机号
};

//修改密码
struct tagLS_ModPassword
{
	tagLS_ModPassword()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR				szAccount[NAME_LEN];			//登录帐号
	TCHAR				szOldPwd[PWD_LEN];				//旧登录密码
	TCHAR				szNewPwd[PWD_LEN];				//新登录密码
};

//锁定本机
struct tagLS_LockMachine
{
	tagLS_LockMachine()
	{
		memset(this, 0, sizeof(*this));
	}

	bool					bLock;														// 是否锁定(true:锁定，false:解锁)
	TCHAR				szSMS[SMS_LEN];										//短信认证码
	TCHAR				szAccount[NAME_LEN];								// 帐号
	TCHAR				szBankPwd[PWD_LEN];								// 银行密码
	TCHAR				szComputerID[COMPUTER_ID_LEN];			// 机器码
};

//定义头像
struct tagLS_UploadCustomFace
{
	tagLS_UploadCustomFace()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;						//玩家 ID
	WORD				wCurrentSize;					//当前大小
	bool				bLastPacket;					//最后标识
	bool				bFirstPacket;					//第一个标识
	BYTE				bFaceData[2048];				//头像数据
};

//下载头像
struct tagLS_DownloadFace
{
	tagLS_DownloadFace()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;							//玩家 ID
};

//上传结果
struct tagLS_UploadFaceResult
{
	tagLS_UploadFaceResult()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR				szDescribeMsg[128];					//描述信息
	DWORD				dwFaceVer;							//;
	bool				bOperateSuccess;					//成功标识
};

//下载成功
struct tagLS_DownloadFaceSuccess
{
	tagLS_DownloadFaceSuccess()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwToltalSize;						//总共大小
	DWORD				dwCurrentSize;						//当前大小
	DWORD				dwUserID;							//玩家 ID
	BYTE				bFaceData[2048];					//头像数据
};

//删除结果
struct tagLS_DeleteFaceResult
{
	tagLS_DeleteFaceResult()
	{
		memset(this,0,sizeof(*this));
	}
	TCHAR				szDescribeMsg[128];					//描述信息
	DWORD				dwFaceVer;							//头像版本
	bool				bOperateSuccess;					//成功标识
};

//删除消息
struct tagLS_CustomFaceDelete
{
	tagLS_CustomFaceDelete()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;							//玩家 ID
	DWORD				dwFaceVer;							//头像版本
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
