#pragma once


//网络数据定义
#define TCP_PROTOCOL_VER				0x05								//网络版本
#define TCP_PACKET_MAXSIZE				8192								//网络缓冲
#define TCP_PACKET_BUFFER_SIZE			(TCP_PACKET_MAXSIZE - TCP_PACKET_HEAD_SIZE - 2 * sizeof(DWORD))
#define TCP_PACKET_HEAD_SIZE			sizeof(CMD_Head)


/////////////////////////////////////////////////////////////////////////////////////////

//内核命令码
#define MDM_KN_COMMAND					6									//内核命令
#define SUB_KN_DETECT_SOCKET			7									//检测命令
#define SUB_KN_SHUT_DOWN_SOCKET			8									//中断网络

//网络内核
struct CMD_Info
{
	BYTE								cbMessageVer;						//版本标识
	BYTE								cbCheckCode;						//效验字段
	WORD								wDataSize;							//数据大小
};

//网络命令
struct CMD_Command
{
	WORD								wMainCmdID;							//主命令码
	WORD								wSubCmdID;							//子命令码
};

//网络包头
struct CMD_Head
{
	CMD_Info							CmdInfo;							//基础结构
	CMD_Command							CommandInfo;						//命令信息
};

//网络包缓冲
struct CMD_Buffer
{
	CMD_Head							Head;								//数据包头
	BYTE								cbBuffer[TCP_PACKET_BUFFER_SIZE];	//数据缓冲
};

//检测结构信息
struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;					//发送时间
	DWORD								dwRecvTickCount;					//接收时间
};

//机器序列号结构
struct tagClientSerial
{
	DWORD							dwSystemVer;							//系统标识;0:电脑,1：IOS，2：安卓
	TCHAR								szComputerID[33];					//机器序列
};
