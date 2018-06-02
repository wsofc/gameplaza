#pragma once


//�������ݶ���
#define TCP_PROTOCOL_VER				0x05								//����汾
#define TCP_PACKET_MAXSIZE				8192								//���绺��
#define TCP_PACKET_BUFFER_SIZE			(TCP_PACKET_MAXSIZE - TCP_PACKET_HEAD_SIZE - 2 * sizeof(DWORD))
#define TCP_PACKET_HEAD_SIZE			sizeof(CMD_Head)


/////////////////////////////////////////////////////////////////////////////////////////

//�ں�������
#define MDM_KN_COMMAND					6									//�ں�����
#define SUB_KN_DETECT_SOCKET			7									//�������
#define SUB_KN_SHUT_DOWN_SOCKET			8									//�ж�����

//�����ں�
struct CMD_Info
{
	BYTE								cbMessageVer;						//�汾��ʶ
	BYTE								cbCheckCode;						//Ч���ֶ�
	WORD								wDataSize;							//���ݴ�С
};

//��������
struct CMD_Command
{
	WORD								wMainCmdID;							//��������
	WORD								wSubCmdID;							//��������
};

//�����ͷ
struct CMD_Head
{
	CMD_Info							CmdInfo;							//�����ṹ
	CMD_Command							CommandInfo;						//������Ϣ
};

//���������
struct CMD_Buffer
{
	CMD_Head							Head;								//���ݰ�ͷ
	BYTE								cbBuffer[TCP_PACKET_BUFFER_SIZE];	//���ݻ���
};

//���ṹ��Ϣ
struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;					//����ʱ��
	DWORD								dwRecvTickCount;					//����ʱ��
};

//�������кŽṹ
struct tagClientSerial
{
	DWORD							dwSystemVer;							//ϵͳ��ʶ;0:����,1��IOS��2����׿
	TCHAR								szComputerID[33];					//��������
};
