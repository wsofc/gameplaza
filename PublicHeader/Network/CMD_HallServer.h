/*

REQ -- request 请求
ANW -- answer  应答

*/

#pragma once


//-----------------------------------------------
//登录命令码

#define MAIN_HS_LOGON					0x4000							//广场登录

#define SUB_HS_LOGON_USER_ID			0x4002							//I D 登录

#define SUB_HS_LOGON_SUCCESS			0x4005							//登陆成功
#define SUB_HS_LOGON_ERROR				0x4006							//登陆失败
#define SUB_HS_LOGON_FINISH				0x4007							//登陆完成


//I D 登录
struct tagHS_LogonByUserID
{
	tagHS_LogonByUserID()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;						//用户I D
	TCHAR		szPwd[PWD_LEN];					//登录密码
};

//登陆成功
struct tagHS_LogonSuccess
{
	tagHS_LogonSuccess()
	{
		memset(this,0,sizeof(*this));
	}
};

//登陆失败
struct tagHS_LogonError
{
	tagHS_LogonError()
	{
		memset(this,0,sizeof(*this));
	}
	LONG		lErrorCode;						//错误代码
	TCHAR		szErrorDescribe[128];			//错误消息
};


//-----------------------------------------------
//
#define MAIN_HS_USER					0x4100			//用户信息

#define SUB_HS_MODIFY_BASE_INFO			0x4103			//修改个人信息
#define SUB_HS_MODIFY_PWD				0x4106			//修改密码
#define SUB_HS_MODIFY_USER_INFO_RESULT	0x4107			//修改结果

#define SUB_HS_SAFE_BIND				0x4110			//玩家绑定
#define SUB_HS_SAFE_UNBIND				0x4111			//解除绑定

#define SUB_HS_GET_MAIL_COUNT			0x4120			//用户未读邮件数量
#define SUB_HS_GET_SERVER_TIMES			0x4121			//用户请求服务器时间

#define	SUB_HS_GET_GAME_SCORE_COUNT			0x4122			//用户请求刷新金币数量
//个人资料
struct tagHS_ModifyBaseInfo
{
	tagHS_ModifyBaseInfo()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;				//玩家 ID

	TCHAR		szNickname[NAME_LEN];	//玩家昵称
	BYTE		cbGender;				//玩家性别
	WORD		wFaceID;				
	TCHAR		szUnderWrite[UNDER_WRITE_LEN];		//个性签名
};

//变长数据包
//如果只修改一个密码,那么只发送需要的
//如果修改两个密码,那么发送所有的
struct tagHS_ModifyPwd
{
	tagHS_ModifyPwd()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;		//玩家 ID
	BYTE		cbPwdType;

	TCHAR		szOldPwd0[PWD_LEN];	
	TCHAR		szNewPwd0[PWD_LEN];

	TCHAR		szOldPwd1[PWD_LEN];	
	TCHAR		szNewPwd1[PWD_LEN];	
};

////密码类型
//enum emPwdType
//{
//	PWD_TYPE_ERROR	,
//	PWD_TYPE_LOGON	= 0x01,	//登录密码
//	PWD_TYPE_BANK	= 0x02,	//银行密码
//};
#define	MODIFY_TYPE_BASE_INFO	0x04

//修改结果
struct tagHS_ModifyUserInfoResult
{
	tagHS_ModifyUserInfoResult()
	{
		memset(this,0,sizeof(*this));
	}
	bool		bSuccess;				//成功标识
	BYTE		cbModifyType;			//修改类型,PWD_TYPE_LOGON,PWD_TYPE_LOGON,MODIFY_TYPE_BASE_INFO
	TCHAR		szErrorDescribe[128];	//描述信息
};

//
//

//绑定帐号
struct tagHS_SafeBind
{
	tagHS_SafeBind()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD				dwUserID;						//玩家ID
	tagClientSerial		ClientSerial;					//机器序列
	TCHAR				szPwd[PWD_LEN];					//登录密码
};

//解除绑定
struct tagHS_SafeUnbind
{
	tagHS_SafeUnbind()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD	dwUserID;						//玩家ID
	TCHAR	szPwd[PWD_LEN];					//登录密码
};

//解除绑定
struct tagHS_BindResult
{
	tagHS_BindResult()
	{
		memset(this,0,sizeof(*this));
	}
	BYTE	cbIsBind;
	BYTE	cbSuccess;
	TCHAR	szErrorDescribe[128];			//错误消息
};

//得到邮件数量
struct tagHS_GetMailCount
{
	tagHS_GetMailCount()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;						//用户I D
	UINT		nMailCount;						//用户未读邮件数量
	bool		bSuccess;							//成功标识
};
//得到金币数量
struct tagHS_GetGameScore
{
	tagHS_GetGameScore()
	{
		memset(this,0,sizeof(*this));
	}
	DWORD		dwUserID;						//用户I D
	LONGLONG	lGameScore;						//用户金币数量
};

//得到服务器时间
struct tagHS_GetServerTimes
{
	tagHS_GetServerTimes()
	{
		memset(this,0,sizeof(*this));
	}
	LONGLONG	lServerMilliseconds;				//服务器返回的总微秒数
	TCHAR		szKey[37];							//GUID key
	WORD		wButtonFlag;						//按钮标志
};


//-----------------------------------------------
//					the end
//-----------------------------------------------
