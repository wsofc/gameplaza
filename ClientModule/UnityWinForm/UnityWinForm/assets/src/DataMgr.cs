using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class DataManager : IEventListener
{
    private static DataManager g_Instance               = null;             // 单例

    private UserModel m_self                            = null;             // 玩家自己
    private Dictionary<UInt32, UserModel> m_userMap     = null;             // 玩家容器
    private byte[] m_cbBuffer                           = null;             // 缓冲数组

    DataManager()
    { }

    ~DataManager()
    {
        this.Exit();
    }

    public static DataManager GetInstance()
    {
        if (DataManager.g_Instance == null)
        {
            DataManager.g_Instance = new DataManager();
        }
        return DataManager.g_Instance;
    }

    public void Init(UnityWinForm.UnityWinForm form)
    {
        Debug.Assert(form != null, "Init DataManager Failed, the WinForm is null");
        if (form == null) return;

        // 初始化成员变量
        this.m_self = new UserModel();
        this.m_userMap = new Dictionary<UInt32, UserModel>();
        this.m_cbBuffer = new byte[Protocol.IPC_PACKAGE];

        // 初始化数据服务模块
        Server server = Server.GetInstance();
        server.Init(form.Handle);

        // 初始化资源路径
        Resource res = Resource.GetInstance();
        res.Init();

        // 加载玩家列表模块
        UserList userList = UserList.GetInstance();
        userList.unity_form = form;
        userList.Init();
        
        // 绑定监听事件
        server.AttachEventListener((int)Protocol.MsgType.MT_GAME_SERVER, this);
    }

    public void Exit()
    {
        // 卸载监听事件
        Server server = Server.GetInstance();
        server.DetachEventListener((int)Protocol.MsgType.MT_GAME_SERVER, this);
    }

    /// <summary> 玩家自身结构 </summary>
    public UserModel UserSelf { get { return this.m_self; } }

    /// <summary> 场景是否已初始化 </summary>
    public bool SceneInited { set; get; }

    /// <summary> 通过结构体创建玩家对象 </summary>
    public UserModel CreatUserByStruct(Protocol.UserStruct stuUser, object objUserSuffix = null)
    {
        UserModel user = null;
        if (this.m_userMap.ContainsKey(stuUser.dwUserID))
        {
            user = this.m_userMap[stuUser.dwUserID];
        }
        else
        {
            user = new UserModel();
            user.InitData(ref stuUser, objUserSuffix);
            this.m_userMap[stuUser.dwUserID] = user;
        }

        if (user.IsSelf)
        {
            this.m_self = user;
        }

        return user;
    }

    /// <summary> 通过玩家ID创建默认玩家对象 </summary>
    public UserModel CreatUserByID(UInt32 dwUserID)
    {
        UserModel user = null;
        if (this.m_userMap.ContainsKey(dwUserID))
        {
            user = this.m_userMap[dwUserID];
        }
        else
        {
            user = new UserModel();
            this.m_userMap[dwUserID] = user;
        }

        if (user.IsSelf)
        {
            this.m_self = user;
        }

        return user;
    }

    /// <summary> 通过玩家ID获取玩家对象 </summary>
    public UserModel GetUserByUserID(UInt32 dwUserID)
    {
        UserModel user = null;

        if (this.m_userMap.ContainsKey(dwUserID))
        {
            user = this.m_userMap[dwUserID];
        }

        return user;
    }

    /// <summary> 通过椅子ID获取玩家对象 </summary>
    public UserModel GetUserByChairID(UInt32 dwChairID)
    {
        UserModel user = null;

        foreach (var it in this.m_userMap)
        {
            if (it.Value.ChairID == dwChairID)
            {
                user = it.Value;
                break;
            }
        }

        return user;
    }

    /// <summary> 通过玩家ID删除玩家对象 </summary>
    public bool RemoveUserByID(UInt32 dwUserID)
    {
        bool bRes = this.m_userMap.ContainsKey(dwUserID);

        if (bRes)
        {
            this.m_userMap.Remove(dwUserID);
        }

        return bRes;
    }

    /// <summary> 通过玩家总数量 </summary>
    public int GetUserCount() { return m_userMap.Count; }

    /// <summary> 更新用户分数 </summary>
    public void UpdateUserScore(Protocol.UserStruct stuUser)
    {
        if (this.m_userMap.ContainsKey(stuUser.dwUserID))
        {
            UserModel user = this.m_userMap[stuUser.dwUserID];
            user.GameGold = stuUser.llGold;
            user.BankGold = stuUser.llBankGold;
            user.Score = stuUser.llScore;
            user.Charm = stuUser.llCharm;
        }
    }

    // ---------------------------------------------------------------------------------------------------------------------------------
    // 处理接收事件

    /// <summary> 加载游戏进度 </summary>
    public void StartGamSteps(Protocol.StartGameStep eStep)
    {
        NestedWindow nest = NestedWindow.GetInstance();
        nest.StartGamSteps(eStep);
    }

    /// <summary> 加载进度完毕,开始游戏,初始化游戏场景 </summary>
    public void StartGameInfo()
    {
        NestedWindow nest = NestedWindow.GetInstance();
        nest.StartGameInfo();
    }

    public void CloseGame()
    {
        // 通知游戏关闭
        Server server = Server.GetInstance();
        server.SendDataToGame(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_CLOSE_FRAME);

        // 如果游戏未启动完成就关闭窗口,那么游戏就接收不到关闭消息,从而不能自己关闭游戏进程
        // 所以需要手动关闭进程
        if (!server.IsFormComplete || !server.IsGameComplete)
        {
            this.OnCloseGame(true);
        }
    }

    public void OnCloseGame(bool bKillNestedProcess = false)
    {
        NestedWindow nest = NestedWindow.GetInstance();
        nest.NextWaitForExit();
        
        if (bKillNestedProcess)
        {
            nest.KillNestedProcess();
        }
    }

    /// <summary> 消息派发级别 </summary>
    public int EventProority { get { return 1; } }

    /// <summary> 处理接收事件 </summary>
    public bool HandleEvent(int id, object param1 = null, object param2 = null)
    {
        switch (id)
        {
            case (int)Protocol.MsgType.MT_GAME_SERVER:
                {
                    ServerPackage package = param1 as ServerPackage;
                    if (package != null)
                    {
                        switch (package.CmdMainID)
                        {
                            // 大厅用户消息
                            case Protocol.IPC_MAIN_USER:
                                {
                                    this.OnIPCUserData(package.CmdSubID, package.Buffer, package.DataSize);
                                } break;

                            // 用户消息
                            case Protocol.MDM_GR_USER:
                                {
                                    this.OnUserData(package.CmdSubID, package.Buffer, package.DataSize);
                                } break;

                            // 系统消息
                            case Protocol.MDM_GF_FRAME:
                                {
                                    this.OnGameFrame(package.CmdSubID, package.Buffer, package.DataSize);
                                }break;

                            default:
                                break;
                        }
                    }
                } break;

            default:
                break;
        }
        return false;
    }
    
    private void OnIPCUserData(ushort wSubCmdID, byte[] cbDataBuffer, ushort wDataSize)
    {
        switch (wSubCmdID)
        {
            case Protocol.IPC_SUB_USER_CONFIG:
                {
                    System.Collections.ArrayList array = Util.BytesToStructArray<Protocol.tagColumnItem>(cbDataBuffer, wDataSize);
                    if (array == null) return;

                    for (int i = 0; i < array.Count; ++i)
                    {
                        Protocol.tagColumnItem headItem = (Protocol.tagColumnItem)array[i];
                        UserList userlist = UserList.GetInstance();
                        userlist.InsertHead(headItem);
                    }
                }break;
            
            case Protocol.IPC_SUB_USER_COME:
                {
                    object obj = Util.BytesToStruct<Protocol.UserStruct>(cbDataBuffer);
                    if (obj == null) return;

                    Protocol.UserStruct stuUser = (Protocol.UserStruct)obj;

                    object objStructSuffix = null;
                    Array.Clear(this.m_cbBuffer, 0, this.m_cbBuffer.Length);

                    // 解析附加的玩家结构
                    #region
                    if (true)
                    {
                        int wOffset = Marshal.SizeOf(typeof(Protocol.UserStruct));
                        RecvPacketHelper RecvPacket = new RecvPacketHelper(cbDataBuffer, wDataSize, wOffset);
                        tagDataDescribe DataDescribe = new tagDataDescribe();
                        while (RecvPacket.GetData(this.m_cbBuffer, ref DataDescribe))
                        {
                            switch (DataDescribe.wDataDescribe)
                            {
                                // 附加的玩家信息结构
                                case Protocol.IPC_USER_SUFFIX:
                                    {
                                        objStructSuffix = Util.BytesToStruct<Protocol.UserStructSuffix>(this.m_cbBuffer);
                                    } break;
                                default:
                                    break;
                            }
                        }
                    }
                    #endregion

                    UserModel user = this.CreatUserByStruct(stuUser, objStructSuffix);
                    if (user != null)
                    {
                        // 更新用户分数
                        this.UpdateUserScore(stuUser);

                        // 添加用户至右侧列表中
                        UserList userlist = UserList.GetInstance();
                        if (user.IsSelf)
                        {
                            userlist.InitSelf(user);
                        }
                        userlist.InsertBody(user);
                    }
                } break;

            default:
                break;
        }
    }

    private void OnUserData(ushort wSubCmdID, byte[] cbDataBuffer, ushort wDataSize)
    {
        switch (wSubCmdID)
        {
            case Protocol.SUB_GR_USER_COME:
                break;

            // 更新状态
            case Protocol.SUB_GR_USER_STATUS:
                {
                    object obj = Util.BytesToStruct<Protocol.UserStruct>(cbDataBuffer);
                    if (obj == null) return;

                    Protocol.UserStruct stuUser = (Protocol.UserStruct)obj;
                    UserModel user = this.GetUserByUserID(stuUser.dwUserID);
                    if (user == null) return;

                    // 更新用户状态
                    user.InitData(ref stuUser);

                    // 更新用户列表
                    UserList userlist = UserList.GetInstance();
                    userlist.UpdateBody(user);

                    switch (user.Status)
                    {
                        case Define.US_NULL:        // 没有状态(用户离开房间)
                            break;
                        case Define.US_READY:       // 同意状态
                            break;
                        case Define.US_PLAY:        // 游戏状态
                            break;
                        case Define.US_LOOKON:      // 旁观状态
                            break;
                        case Define.US_OFFLINE:     // 断线状态
                            break;
                        case Define.US_FREE:		// 站立状态
                            {
                                // 先从玩家UI列表中移除
                                userlist.RemoveBody(user);

                                // 最后再从数据列表中移除
                                this.RemoveUserByID(user.UserID);
                            }
                            break;
                        case Define.US_SIT:		    // 坐下状态
                            break;

                        default:
                            break;
                    }
                } break;

            // 更新分数
            case Protocol.SUB_GR_USER_SCORE:
                {
                    object obj = Util.BytesToStruct<Protocol.UserStruct>(cbDataBuffer);
                    if (obj == null) return;

                    // 更新所有用户分数
                    Protocol.UserStruct stuUser = (Protocol.UserStruct)obj;
                    this.UpdateUserScore(stuUser);

                    // 更新用户列表
                    UserModel user = this.GetUserByUserID(stuUser.dwUserID);
                    if (user != null)
                    {
                        UserList userlist = UserList.GetInstance();
                        userlist.UpdateBody(user);
                    }
                } break;

            default:
                break;
        }
    }

    // 框架消息
    private void OnGameFrame(ushort wSubCmdID, byte[] cbDataBuffer, ushort wDataSize)
    {
        switch (wSubCmdID)
        {
            // 游戏配置
            case Protocol.SUB_GF_OPTION:    break;

            // 场景消息
            case Protocol.SUB_GF_SCENE:     break;

            // 用户聊天
            case Protocol.SUB_GF_USER_CHAT: break;

            // 系统消息
            case Protocol.SUB_GF_MESSAGE:
                {
                    object obj = Util.BytesToStruct<Protocol.tagGS_Message>(cbDataBuffer);
                    if (obj != null)
                    {
                        Protocol.tagGS_Message msg = (Protocol.tagGS_Message)obj;
                        string strMsg = Util.TcharToString(msg.szContent);

                        UserList userlist = UserList.GetInstance();

                        // 提示消息
                        if (Define.SMT_INFO == (msg.wMessageType & Define.SMT_INFO))
                        {
                            Debug.Print("类型:提示消息 - {0}", strMsg);
                            userlist.ParseTextMsg(String.Format("\n[0xFF000000]【系统提示】[-]{0}", strMsg));
                        }

                        // 全局消息
                        if (Define.SMT_GLOBAL == (msg.wMessageType & Define.SMT_GLOBAL))
                        {
                            Debug.Print("类型:全局消息 - {0}", strMsg);
                            userlist.ParseTextMsg(String.Format("\n[0xFF000000]【系统提示】[-]{0}", strMsg));
                        }

                        // 弹框消息
                        if (Define.SMT_EJECT == (msg.wMessageType & Define.SMT_EJECT))
                        {
                            Debug.Print("类型:弹框消息 - {0}", strMsg);
                        }

                        // 关闭房间
                        if (Define.SMT_CLOSE_ROOM == (msg.wMessageType & Define.SMT_CLOSE_ROOM))
                        {
                            Debug.Print("类型:关闭房间");
                        }

                        // 中断连接
                        if (Define.SMT_INTERMIT_LINE == (msg.wMessageType & Define.SMT_INTERMIT_LINE))
                        {
                            Debug.Print("类型:中断连接");
                        }
                    }
                } break;

            default:
                break;
        }
    }
}