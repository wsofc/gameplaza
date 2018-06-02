using System;

public class Define
{
    public const int    VER_GAME_LOW        = 10;               // 游戏版本低位
    public const int    VER_GAME_HIGH       = 10;               // 游戏版本高位

    // BOOL值
    public const int    TRUE                = 1;                // 真
    public const int    FALSE               = 0;                // 假

    // 无效数值
    public const byte   INVALID_BYTE        = 0xFF;             // 无效数值 8 bit
    public const UInt16 INVALID_WORD        = 0xFFFF;           // 无效数值 16 bit
    public const UInt32 INVALID_DWORD       = 0xFFFFFFFF;       // 无效数值 32 bit

    public const UInt16 INVALID_TABLE       = INVALID_WORD;     // 无效桌子
    public const UInt16 INVALID_CHAIR       = INVALID_WORD;     // 无效椅子

    // 字符数组长度定义
    public const int    NAME_LEN            = 32;               // 名字长度
    public const int    PWD_LEN             = 33;               // 密码长度
    public const int    GROUP_LEN           = 32;               // 社团长度
    public const int    UNDER_WRITE_LEN		= 32;               // 个性签名
    public const int    COMPUTER_ID_LEN     = 33;               // 机器码
    public const int    MAX_PATH            = 260;              // 路径长度
    public const int    MAX_CHAT_LEN        = 200;				// 聊天长度
    public const int    MAX_SYSMESSAGE_LEN  = 1024;             // 系统消息长度

    /// <summary> 用户状态定义 </summary>
    public const byte   US_NULL             = 0x00;             // 没有状态(用户离开房间)
    public const byte   US_FREE             = 0x01;             // 站立状态
    public const byte   US_SIT              = 0x02;             // 坐下状态
    public const byte   US_READY            = 0x03;             // 同意状态
    public const byte   US_LOOKON           = 0x04;             // 旁观状态
    public const byte   US_PLAY             = 0x05;             // 游戏状态
    public const byte   US_OFFLINE          = 0x06;             // 断线状态

    //消息类型
    public const int    SMT_INFO            = 0x0001;           // 信息消息
    public const int    SMT_EJECT           = 0x0002;           // 弹出消息
    public const int    SMT_GLOBAL          = 0x0004;           // 全局消息
    public const int    SMT_CLOSE_GAME      = 0x1000;           // 关闭游戏
    public const int    SMT_CLOSE_ROOM      = 0x1000;           // 关闭房间
    public const int    SMT_INTERMIT_LINE   = 0x4000;           // 中断连接
}
