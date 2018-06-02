
#pragma once

#define MAIN_GS_MATCH			0x3F00			//������Ϣ


enum emSubMatchID
{
	SUB_MATCH_ID_BASE		= 0x1000,
	SUB_GS_SIGN_UP			,			//����
	SUB_GS_SIGN_UP_SUCCESS	,			//�����ɹ�
	SUB_GS_SIGN_UP_FAIL		,			//����ʧ��

	SUB_GS_MATCH_NOT_OPEN	,
};


//����
struct tagGS_Match_SignUp
{
	tagGS_Match_SignUp()
	{
		memset(this,0,sizeof(*this));
	}
};

//�������
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
	LONG		lErrorCode;						//�������
	TCHAR		szErrorDescribe[128];			//������Ϣ
};

//����δ����
struct tagGS_Match_MatchNotOpen
{
	tagGS_Match_MatchNotOpen()
	{
		memset(this,0,sizeof(*this));
	}

	DWORD	dwUserID;
	WORD	wStaySecondsCnt;	//ͣ��ʱ��
	WORD	wCommentLen;
	TCHAR	szComment[128];		//��Ϣ
};



//-----------------------------------------------
//					the end
//-----------------------------------------------
