
#pragma once

#define MAIN_GS_MATCH			0x3F00			//比赛信息


enum emSubMatchID
{
	SUB_MATCH_ID_BASE		= 0x1000,
	SUB_GS_SIGN_UP			,			//报名
	SUB_GS_SIGN_UP_SUCCESS	,			//报名成功
	SUB_GS_SIGN_UP_FAIL		,			//报名失败

	SUB_GS_MATCH_NOT_OPEN	,
};


//报名
struct tagGS_Match_SignUp
{
	tagGS_Match_SignUp()
	{
		memset(this,0,sizeof(*this));
	}
};

//报名结果
struct tagGS_Match_SignUpSuccess
{
	tagGS_Match_SignUpSuccess()
	{
		memset(this,0,sizeof(*this));
	}
};
struct tagGS_Match_SignUpFail
{
	tagGS_Match_SignUpFail()
	{
		memset(this,0,sizeof(*this));
	}
	LONG		lErrorCode;						//错误代码
	TCHAR		szErrorDescribe[128];			//错误消息
};

//比赛未开放
struct tagGS_Match_MatchNotOpen
{
	tagGS_Match_MatchNotOpen()
	{
		memset(this,0,sizeof(*this));
	}

	DWORD	dwUserID;
	WORD	wStaySecondsCnt;	//停留时间
	WORD	wCommentLen;
	TCHAR	szComment[128];		//消息
};



//-----------------------------------------------
//					the end
//-----------------------------------------------
