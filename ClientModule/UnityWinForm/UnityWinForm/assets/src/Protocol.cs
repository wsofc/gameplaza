using System;
using System.Runtime.InteropServices;

public class Protocol
{
    // ----------------------------------------------------------------------------------------------------------
    // 数据定义

    public enum MsgType
    {
        /// <summary> 无 </summary>
        MT_NONE = 0,

        /// <summary> 登录服务器发来的消息 </summary>
        MT_LOGIN_SERVER,

        /// <summary> 游戏服务器发来的消息 </summary>
        MT_GAME_SERVER,

        /// <summary> 游戏逻辑消息 </summary>
        //MT_GAME_LOGIC,

        /// <summary> 更新视图的消息(见下面视图消息类型) </summary>
        //MT_UPDATE_VIEW,

        /// <summary> 额外的预留项 </summary>
        MT_EXTRA
    };

    /// <summary> 加载游戏步骤 </summary>
    public enum StartGameStep
    {
        SGS_NONE = 0,               // 无
        SGS_LOAD_PROCESS,           // 加载游戏进程
        SGS_LOAD_HOOK,              // 挂载Winform钩子
        SGS_LOAD_GAME_HOOK,         // 挂载游戏钩子
        SGS_LOAD_USERLIST,          // 加载玩家列表
        SGS_LOAD_USERMENU,          // 加载游戏菜单
        SGS_LOAD_COMPLETE,          // 加载游戏完毕
        SGS_STEPS                   // 总步骤
    }

    public static IntPtr PlazaWnd                   = IntPtr.Zero;                  // 大厅窗口句柄
    public static IntPtr GamesWnd                   = IntPtr.Zero;                  // 游戏窗口句柄
    public static string GameName                   = "";                           // 游戏名
    public static string GameModelName              = "";                           // 游戏执行程序名
    public static string GameFileName               = "";                           // 游戏文件名
    public static string GameResPath                = "";                           // 游戏资源相对路径
    public static byte GameLoadingShow              = Define.TRUE;                  // 是否显示游戏加载进度条
    public static UInt32 GameLoadingColor           = 0xFF000000;                   // 游戏默认加载背景色
    public static string GameLoadingImg             = "";                           // 游戏默认加载背景
    public static tagGameConfig GameConfig          = new tagGameConfig(0);         // 游戏配置

    // IPC数据定义
    public const int IPC_VER                        = 0x0001;                       // IPC 版本
    public const int IPC_PACKAGE                    = 4096;                         // 数据缓存大小
    public const int IPC_BUFFER                     = (8 + IPC_PACKAGE);	        // 缓冲长度
    public const int IPC_FLAG_TO_SERVER             = 0x0000;                       // 数据发送给服务器
    public const int IPC_FLAG_TO_GAMEPLAZA          = 0x0001;                       // 数据发送给大厅
    public const int IPC_FLAG_TO_GAME               = 0x0002;                       // 数据发送给游戏
    public const int IPC_FLAG_TO_WINFROM            = 0x0003;                       // 数据发送给Winform
    
    // IPC主命令
    public const int IPC_MAIN_CONCTROL              = 0x0004;                       // 大厅交互主命令
    public const int IPC_MAIN_USER                  = 0x0003;						// 用户信息

    public const int IPC_SUB_USER_CONFIG            = 0x0000;						// 用户配置
    public const int IPC_SUB_USER_COME              = 0x0001;					    // 用户信息

    // IPC子命令
    public const int IPC_SUB_HEARTBEAT              = 0x1979;                       // 心跳消息
    public const int IPC_SUB_START_FINISH           = 0x0001;                       // 启动完成
    public const int IPC_SUB_CLOSE_FRAME            = 0x0002;                       // 游戏关闭
    public const int IPC_SUB_START_FAILED           = 0x0005;		                // 启动失败
    public const int IPC_SUB_CLOSE_FRAME_EX         = 0x0006;                       // 游戏强退
    public const int IPC_SUB_GF_USER_READY          = 0x0007;                       // 游戏用户同意
    public const int IPC_SUB_GF_RECONNECT           = 0x0008;			            // 游戏断线重连
    public const int IPC_SUB_KICK_USER              = 0x0009;					    // 踢出用户
    public const int IPC_SUB_SEND_CHAT              = 0x0010;					    // 发送聊天消息
    public const int IPC_USER_SUFFIX                = 0x0011;					    // 附加的玩家信息结构
    
    // Winform子命令
    public const int IPC_SUB_WINGAME_START_OK       = 0x1001;					    // WinForm启动成功
    public const int IPC_SUB_WINGAME_START_NO	    = 0x1002;					    // WinForm启动失败
    public const int IPC_SUB_WINGAME_GAMEPROCESS    = 0x1003;					    // 保存游戏进程信息
    public const int IPC_SUB_WINGAME_START_INFO     = 0x1004;					    // 游戏启动完毕

    public const int IPC_SUB_WINGAME_RESIZE         = 0x2001;					    // 游戏窗口大小发生改变
    public const int IPC_SUB_WINGAME_MENU_INIT      = 0x2002;					    // 游戏菜单初始化
    public const int IPC_SUB_WINGAME_MENU_CONFIG    = 0x2003;					    // 游戏菜单配置
    public const int IPC_SUB_WINGAME_MENU_EVENT     = 0x2004;					    // 游戏菜单事件
    public const int IPC_SUB_WINGAME_REFRESH        = 0x3001;					    // 游戏UI刷新
    public const int IPC_SUB_WINGAME_TIPS_MSG       = 0x4001;                       // 游戏打印消息
    public const int IPC_SUB_WINGAME_FINISHED       = 0x5001;					    // 游戏配置完毕

    // 服务器主命令
    public const int MDM_GF_GAME                    = 100;                          // 游戏消息
    public const int MDM_GF_FRAME                   = 101;                          // 框架消息
    public const int MDM_GF_BANK                    = 103;							// 银行消息
    public const int MDM_GR_USER                    = 0x3100;                       // 用户信息
    public const int MAIN_GS_SYSTEM                 = 0x3500;		                // 系统信息
    public const int MAIN_GS_STATUS                 = 0x3300;                       // 状态信息

    // 服务器子命令
    public const int SUB_GS_MESSAGE                 = 300;		                    // 系统消息
    public const int SUB_GR_USER_COME               = 100;                          // 用户进入
    public const int SUB_GR_USER_STATUS             = 101;                          // 用户状态
    public const int SUB_GR_USER_SCORE              = 102;                          // 用户消息
    public const int SUB_GF_SCENE                   = 101;                          // 场景消息
    public const int SUB_GS_TABLE_INFO		        = 0x3301;						// 桌子信息
    public const int SUB_GS_TABLE_STATUS            = 0x3302;					    // 桌子状态

    public const int SUB_GF_OPTION                  = 100;				            // 游戏配置
    public const int SUB_GF_USER_READY              = 2;							// 用户同意
    public const int SUB_GF_LOOKON_CONTROL          = 3;							// 旁观控制
    public const int SUB_GF_USER_CHAT               = 200;				            // 用户聊天
    public const int SUB_GF_BET_RECORD_TIME         = 201;				            // 下注记录时间请求
    public const int SUB_GF_MESSAGE                 = 300;							// 系统消息

    public const int SUB_GF_BANK_WITHDRAW           = 451;							// 银行提取
    public const int SUB_GF_BANK_RESULT             = 456;                          // 银行操作失败
    public const int SUB_GF_BANK_INFO               = 457;                          // 银行操作成功
    public const int SUB_GF_BANK_GETINFO            = 458;							// 请求银行信息

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 默认信息
    public const int DTP_NULL				        = 0;                            // 无效数据

    // 基础信息
    public const int DTP_USER_ID                    = 1;							// 用户 I D
    public const int DTP_GAME_ID                    = 3;							// 游戏 I D
    public const int DTP_ACCOUNT			        = 5;							// 帐号
    public const int DTP_NICKNAME			        = 7;							// 昵称
    public const int DTP_USER_PWD		            = 9;							// 用户密码
    public const int DTP_SEX				        = 11;							// 用户性别
    public const int DTP_USER_FACE			        = 13;							// 用户头像
    public const int DTP_USER_RIGHT			        = 15;							// 用户权限
    public const int DTP_MASTER_RIGHT		        = 17;							// 管理权限
    public const int DTP_UNDER_WRITE                = 19;							// 个性签名
    public const int DTP_CHARM				        = 21;							// 用户魅力
    public const int DTP_MEMBERORDER                = 22;                           // 会员等级

    //状态信息
    public const int DTP_USER_STATUS                = 100;							// 用户状态
    public const int DTP_USER_TABLE			        = 101;							// 游戏桌号
    public const int DTP_USER_CHAIR			        = 102;							// 椅子号码
    public const int DTP_USER_PING			        = 103;							// 网络延时
    public const int DTP_USER_INDEX			        = 104;							// 用户索引
    public const int DTP_USER_ROUND			        = 105;							// 循环计数
    public const int DTP_LOOK_ON_USER		        = 106;							// 旁观玩家

    //积分信息
    public const int DTP_USER_WIN		            = 200;							// 胜局盘数
    public const int DTP_USER_LOST		            = 201;							// 输局盘数
    public const int DTP_USER_DRAW		            = 202;							// 和局盘数
    public const int DTP_USER_FLEE		            = 203;							// 逃局盘数
    public const int DTP_WIN_RATE		            = 204;							// 用户胜率
    public const int DTP_LOST_RATE		            = 205;							// 用户输率
    public const int DTP_DRAW_RATE		            = 206;							// 用户和率
    public const int DTP_FLEE_RATE		            = 207;							// 用户逃率
    public const int DTP_USER_TAX		            = 208;							// 用户税收
    public const int DTP_INSURE_SCORE		        = 209;							// 存储金币
    public const int DTP_GAME_GOLD		            = 210;							// 游戏金币
    public const int DTP_USER_SCORE		            = 211;							// 用户分数
    public const int DTP_USER_PLAY_COUNT	        = 212;							// 总局盘数
    public const int DTP_USER_EXPERIENCE	        = 213;							// 用户经验
    public const int DTP_GAME_LEVEL		            = 214;							// 游戏等级
    public const int DTP_COMMUNITY_LEVEL	        = 215;							// 社区等级

    //扩展信息
    public const int DTP_USER_GROUP_ID		        = 300;							// 社团 I D
    public const int DTP_USER_GROUP_NAME	        = 301;							// 社团名字
    public const int DTP_USER_NOTE		            = 302;							// 用户备注
    public const int DTP_USER_DESCRIBE		        = 303;							// 用户描述
    public const int DTP_USER_COMPANION		        = 304;							// 用户关系

    //系统信息
    public const int DTP_COMPUTER_ID		        = 1000;							// 机器序列
    public const int DTP_STATION_PAGE		        = 1001;			    			// 站点页面
    public const int DTP_OPTION_BUFFER              = 1002;				    		// 配置信息
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ----------------------------------------------------------------------------------------------------------
    // 基本结构体

    // 网络命令
    [StructLayout(LayoutKind.Sequential)]
    public struct CMD_Command
    {
        public ushort   wMainCmdID;                                         // 主命令码
        public ushort   wSubCmdID;                                          // 子命令码
    }

    // IPC结构
    // Head
    [StructLayout(LayoutKind.Sequential)]
    public struct IPC_Head
    {
        public ushort   wVersion;                                           // IPC 版本
        public ushort   wDataSize;                                          // 数据大小
        public ushort   wMainCmdID;                                         // 主命令码
        public ushort   wSubCmdID;                                          // 子命令码
    }
    
    // Body
    [StructLayout(LayoutKind.Sequential)]
    public struct IPC_Buffer
    {
        public IPC_Buffer(int i)
        {
            this.Head   = new IPC_Head();
            this.bData  = new byte[Protocol.IPC_PACKAGE];
        }

        public IPC_Head Head;                                               // 数据包头      
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = IPC_PACKAGE)]
        public byte[]   bData;                			                    // 数据缓冲
    }

    // 大厅传入游戏的用户结构体
    [StructLayout(LayoutKind.Sequential)]
    public struct UserStruct
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.NAME_LEN)]
        public byte[] pzNickName;                                           // 用户昵称
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.NAME_LEN)]
        public byte[] pzAccount;                                            // 用户账号
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.MAX_PATH)]
        public byte[] pzFacePath;	                                        // 头像路径

        public byte     cbStatus;                                           // 玩家状态
        public byte     cbSelf;                                             // 是否是自己
        public byte     cbRobot;		                                    // 是否是机器人
        public byte     cbMemberOrder;		                                // 会员等级
        public byte     cbMasterOrder;			                            // 管理员等级
        public UInt32   dwUserID;                                           // 用户 I D
        public UInt32   dwFaceID;			                                // 头像 I D
        public UInt32   dwGameID;                                           // 游戏 I D(非游戏真正的ID)
        public UInt32   dwTableID;                                          // 桌子 I D
        public UInt32   dwChairID;                                          // 椅子 I D
        public Int64    llGold;                                             // 金币
        public Int64    llBankGold;                                         // 银行金币
        public Int64    llScore;				                            // 用户分数
        public Int64    llCharm;                                            // 魅力
    };

    // 大厅传入游戏的附加用户结构体
    [StructLayout(LayoutKind.Sequential)]
    public struct UserStructSuffix
    {
        public UserStructSuffix(int i)
        {
            this.cbGender       = 0;
            this.dwGroupID      = 0;
            this.dwExperience   = 0;
            this.lWinCount      = 0;
            this.lLostCount     = 0;
            this.lDrawCount     = 0;
            this.lFleeCount     = 0;
            this.llInsureScore  = 0;
            this.pzGroupName    = new byte[Define.GROUP_LEN];
            this.pzUnderWrite   = new byte[Define.UNDER_WRITE_LEN];
            this.pzLevelDesc    = new byte[Define.NAME_LEN];
            this.pzMachineID    = new byte[Define.COMPUTER_ID_LEN];
        }

        public byte     cbGender;								            // 用户性别

	    public UInt32   dwGroupID;								            // 社团索引
	    public UInt32   dwExperience;							            // 用户经验
	
	    public Int32    lWinCount;								            // 胜利盘数
	    public Int32    lLostCount;								            // 失败盘数
	    public Int32    lDrawCount;								            // 和局盘数
	    public Int32    lFleeCount;								            // 断线数目
	    public Int64    llInsureScore;							            // 消费金币
	        
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.GROUP_LEN)]
	    public byte[]   pzGroupName;					                    // 社团名字

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.UNDER_WRITE_LEN)]
	    public byte[]   pzUnderWrite;			                            // 个性签名

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.NAME_LEN)]
	    public byte[]   pzLevelDesc;					                    // 等级描述

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.COMPUTER_ID_LEN)]
        public byte[]   pzMachineID;					                    // 机器码
    }

    // 大厅重连结构
    [StructLayout(LayoutKind.Sequential)]
    public struct tagGameFrameReconnect
    {
        public byte     cbMaxCount;	                                        // 重连上限次数
        public byte     cbInterval;	                                        // 每次的间隔时间
        public byte     cbStatus;		                                    // 断线重连状态(1 - 开始, 0 - 结束)
    };

    // 桌子状态信息
    [StructLayout(LayoutKind.Sequential)]
    public struct tagGS_TableStatus
    {
        public UInt16   wTableID;                                           // 桌子号码
        public byte     bTableLock;						                    // 锁定状态
        public byte     bPlayStatus;						                // 游戏状态
    };

    //游戏配置
    [StructLayout(LayoutKind.Sequential)]
    public struct CMD_GF_Option
    {
        public byte     bGameStatus;					                    // 游戏状态
        public byte     bAllowLookon;					                    // 允许旁观
    };

    //消息数据包
    [StructLayout(LayoutKind.Sequential)]
    public struct tagGS_Message
    {
        public tagGS_Message(int i)
        {
            this.wMessageType   = 0;
            this.wMessageLength = 0;
            this.szContent      = new byte[Define.MAX_SYSMESSAGE_LEN];
        }

	    public UInt16   wMessageType;						                // 消息类型
	    public UInt16   wMessageLength;						                // 消息长度

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.MAX_SYSMESSAGE_LEN)]
	    public byte[]   szContent;						                    // 消息内容
    };

    //踢出用户
    [StructLayout(LayoutKind.Sequential)]
    public struct CMD_GR_KillUser
    {
        public UInt32   dwTargetUserID;						                // 目标用户
    };

    // 房间聊天结构
    [StructLayout(LayoutKind.Sequential)]
    public struct CMD_GR_UserChat
    {
        public CMD_GR_UserChat(int i)
        {
            this.wChatLength    = 0;
            this.crFontColor    = 0xFFFFFF;
            this.dwSendUserID   = 0;
            this.dwTargetUserID = 0;
            this.szChatMessage  = new byte[Define.MAX_CHAT_LEN];
        }

	    public UInt16   wChatLength;						                // 信息长度
	    public UInt32   crFontColor;						                // 信息颜色
	    public UInt32   dwSendUserID;						                // 发送用户
	    public UInt32   dwTargetUserID;						                // 目标用户
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.MAX_CHAT_LEN)]
	    public byte[]   szChatMessage;		                                // 聊天信息
    };

    // 桌子聊天结构
    [StructLayout(LayoutKind.Sequential)]
    public struct CMD_GF_UserChat
    {
        public CMD_GF_UserChat(int i)
        {
            this.wChatLength    = 0;
            this.crFontColor    = 0xFFFFFFFF;
            this.dwSendUserID   = 0;
            this.dwTargetUserID = 0;
            this.szChatMessage  = new byte[Define.MAX_CHAT_LEN];
        }

        public UInt16   wChatLength;						                // 信息长度
        public UInt32   crFontColor;						                // 信息颜色
        public UInt32   dwSendUserID;						                // 发送用户
        public UInt32   dwTargetUserID;						                // 目标用户
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.MAX_CHAT_LEN)]
        public byte[]   szChatMessage;		                                // 聊天信息
    };
    
    //游戏配置
    [StructLayout(LayoutKind.Sequential)]
    public struct tagGameConfig
    {
        public tagGameConfig(int i)
        {
            this.cbOpenUserList = Define.FALSE;
            this.cbOpenPrintUserInOutMsg = Define.FALSE;
        }

        public byte     cbOpenUserList;                                     // 是否显示右侧玩家列表栏
        public byte     cbOpenPrintUserInOutMsg;                            // 是否打印玩家进出提示消息
    }

    //列表项描述结构
    [StructLayout(LayoutKind.Sequential)]
    public struct tagColumnItem
    {
        public tagColumnItem(int i)
        {
            this.wColumnWidth   = 0;
            this.wDataDescribe  = 0;
            this.szColumnName   = new byte[16];
        }

	    public UInt16   wColumnWidth;						                // 列表宽度
	    public UInt16   wDataDescribe;						                // 字段类型
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	    public byte[]   szColumnName;					                    // 列表名字
    };

    //菜单键值类型
    public enum eUserMenuType
    {
        UMT_NONE = 0,           // 无
        UMT_USER_NAME,          // 用户 昵称
        UMT_USER_GAMEID,        // 用户 显示ID
        UMT_USER_GOLD,          // 用户 金币
        UMT_USER_UNDERWRITE,    // 用户 个性签名
        UMT_SEPARATOR,          // 间隔线
        UMT_EXTRA               // 预留位
    }
    
    //菜单键值信息
    [StructLayout(LayoutKind.Sequential)]
    public struct tagUserMenuInfo
    {
        public tagUserMenuInfo(UInt32 dwMenuKey, string strMenuValue, byte cbActive = Define.TRUE, byte cbVisible = Define.TRUE
            , byte cbSelfActive = Define.TRUE, byte cbSelfVisible = Define.TRUE)
        {
            this.dwMenuKey          = dwMenuKey;
            this.pzMenuValue        = new byte[Define.NAME_LEN];
            this.cbActive           = cbActive;
            this.cbVisible          = cbVisible;
            this.cbSelfActive       = cbSelfActive;
            this.cbSelfVisible      = cbSelfVisible;
            Util.StringToTchar(ref this.pzMenuValue, strMenuValue);
        }

        public byte     cbActive;           // 是否激活(默认true)
        public byte     cbVisible;          // 是否显示(默认true)
        public byte     cbSelfActive;       // 若是自己是否激活(默认true)
        public byte     cbSelfVisible;      // 若是自己是否显示(默认true)
        public UInt32   dwMenuKey;          // 菜单键

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.NAME_LEN)]
        public byte[]   pzMenuValue;        // 菜单值
    }

    //菜单事件信息
    [StructLayout(LayoutKind.Sequential)]
    public struct tagUserMenuEvent
    {
        public UInt32   dwMouseButtons;     // 鼠标键值(请参照 System.Windows.Forms.MouseButtons 枚举)
        public UInt32   dwMenuKey;          // 菜单键值
        public UInt32   dwUserID;           // 用户ID
    }
}
