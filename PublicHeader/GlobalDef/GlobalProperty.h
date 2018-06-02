#pragma once


//-----------------------------------------------
//-----------------------------------------------
//�ʻ�����
#define FLOWER_1					0
#define FLOWER_2					1
#define FLOWER_3					2
#define FLOWER_4					3
#define FLOWER_5					4
#define FLOWER_6					5
#define FLOWER_7					6
#define FLOWER_8					7
#define FLOWER_9					8
#define FLOWER_10					9
#define FLOWER_11					10

const BYTE g_FlowerTypeList[]=
{
	FLOWER_1	,
	FLOWER_2	,
	FLOWER_3	,
	FLOWER_4	,
	FLOWER_5	,
	FLOWER_6	,
	FLOWER_7	,
	FLOWER_8	,
	FLOWER_9	,
	FLOWER_10	,
	FLOWER_11	,
};

//�ʻ���Ŀ
#define FLOWER_COUNT		(sizeof(g_FlowerTypeList)/sizeof(g_FlowerTypeList[0]))//������Ŀ

//////////////////////////////////////////////////////////////////////////
//�궨��
#define CHARM_EXCHANGE_RATE			50									//�һ�����
#define MAX_FLOWER_COUNT				100								//�����Ŀ

//�ʻ��ṹ
struct tagGiftInfo
{
	TCHAR		szName[64];							//�ʻ�����
	TCHAR		szAction[128];						//��������
	TCHAR		szQuantifier1[20];					//������
	TCHAR		szResult[128];						//�������
	TCHAR		szQuantifier2[6];					//������
	TCHAR		szCharmResult[128];				//�������
};

//�ʻ�����
tagGiftInfo const g_FlowerDescribe[FLOWER_COUNT]=
{
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("�ʻ�"),TEXT(" ��������� "),TEXT(" �ʻ�1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("ơ��"),TEXT(" ��������� "),TEXT(" ơ��1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("���"),TEXT(" ��������� "),TEXT(" ���1ö"),TEXT(",��ϲ "),TEXT("ö "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("�γ�"),TEXT(" ��������� "),TEXT(" �γ�1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT(",��ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("����"),TEXT(" �ݺݵ����� "),TEXT(" ����1��"),TEXT(",��Ǹ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("שͷ"),TEXT(" �ݺݵ����� "),TEXT(" שͷ1��"),TEXT(",��Ǹ "),TEXT("�� "),TEXT(" ����ֵ���� %ld ��!")},
	{TEXT("ը��"),TEXT(" �ݺݵ����� "),TEXT(" ը��1ö"),TEXT(",��Ǹ "),TEXT("ö "),TEXT(" ����ֵ���� %ld ��!")}
};


 
 
//���߶���
#define MAX_PROPERTY_COUNT				100									//�����Ŀ
#define BUGLE_MAX_CHAR						120									//���ȸ���
#define BULESSING_MAX_CHAR				100									//ף������
#define PROPERTY_COUNT				10									//����
 

//������Ϣ�ṹ
struct tagPropsInfo
{
	tagPropsInfo()
	{
		memset(this,0,sizeof(*this));
	}
	WORD				wPropsID;							//����ID
	TCHAR				szName[20];						// ��������
	LONG				lPrice;								//�ʻ��۸�
	LONG				lVIPPrice;							//��Ա�۸�
	WORD				wType;								// ��������
	LONGLONG		lSrcCharm;						//��������
	LONGLONG		lRcvCharm;						//��������
	TCHAR				szDescribe[200];				// ����˵��
	LONG				lAvailableTime;					// ��Чʱ��
	bool					bEnabled;							// �Ƿ����
};



//-----------------------------------------------
//					the end
//-----------------------------------------------