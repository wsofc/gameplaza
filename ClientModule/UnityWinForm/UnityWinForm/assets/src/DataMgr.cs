using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class DataManager : IEventListener
{
    private static DataManager g_Instance               = null;             // ����

    private UserModel m_self                            = null;             // ����Լ�
    private Dictionary<UInt32, UserModel> m_userMap     = null;             // �������
    private byte[] m_cbBuffer                           = null;             // ��������

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

        // ��ʼ����Ա����
        this.m_self = new UserModel();
        this.m_userMap = new Dictionary<UInt32, UserModel>();
        this.m_cbBuffer = new byte[Protocol.IPC_PACKAGE];

        // ��ʼ�����ݷ���ģ��
        Server server = Server.GetInstance();
        server.Init(form.Handle);

        // ��ʼ����Դ·��
        Resource res = Resource.GetInstance();
        res.Init();

        // ��������б�ģ��
        UserList userList = UserList.GetInstance();
        userList.unity_form = form;
        userList.Init();
        
        // �󶨼����¼�
        server.AttachEventListener((int)Protocol.MsgType.MT_GAME_SERVER, this);
    }

    public void Exit()
    {
        // ж�ؼ����¼�
        Server server = Server.GetInstance();
        server.DetachEventListener((int)Protocol.MsgType.MT_GAME_SERVER, this);
    }

    /// <summary> �������ṹ </summary>
    public UserModel UserSelf { get { return this.m_self; } }

    /// <summary> �����Ƿ��ѳ�ʼ�� </summary>
    public bool SceneInited { set; get; }

    /// <summary> ͨ���ṹ�崴����Ҷ��� </summary>
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

    /// <summary> ͨ�����ID����Ĭ����Ҷ��� </summary>
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

    /// <summary> ͨ�����ID��ȡ��Ҷ��� </summary>
    public UserModel GetUserByUserID(UInt32 dwUserID)
    {
        UserModel user = null;

        if (this.m_userMap.ContainsKey(dwUserID))
        {
            user = this.m_userMap[dwUserID];
        }

        return user;
    }

    /// <summary> ͨ������ID��ȡ��Ҷ��� </summary>
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

    /// <summary> ͨ�����IDɾ����Ҷ��� </summary>
    public bool RemoveUserByID(UInt32 dwUserID)
    {
        bool bRes = this.m_userMap.ContainsKey(dwUserID);

        if (bRes)
        {
            this.m_userMap.Remove(dwUserID);
        }

        return bRes;
    }

    /// <summary> ͨ����������� </summary>
    public int GetUserCount() { return m_userMap.Count; }

    /// <summary> �����û����� </summary>
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
    // ��������¼�

    /// <summary> ������Ϸ���� </summary>
    public void StartGamSteps(Protocol.StartGameStep eStep)
    {
        NestedWindow nest = NestedWindow.GetInstance();
        nest.StartGamSteps(eStep);
    }

    /// <summary> ���ؽ������,��ʼ��Ϸ,��ʼ����Ϸ���� </summary>
    public void StartGameInfo()
    {
        NestedWindow nest = NestedWindow.GetInstance();
        nest.StartGameInfo();
    }

    public void CloseGame()
    {
        // ֪ͨ��Ϸ�ر�
        Server server = Server.GetInstance();
        server.SendDataToGame(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_CLOSE_FRAME);

        // �����Ϸδ������ɾ͹رմ���,��ô��Ϸ�ͽ��ղ����ر���Ϣ,�Ӷ������Լ��ر���Ϸ����
        // ������Ҫ�ֶ��رս���
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

    /// <summary> ��Ϣ�ɷ����� </summary>
    public int EventProority { get { return 1; } }

    /// <summary> ��������¼� </summary>
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
                            // �����û���Ϣ
                            case Protocol.IPC_MAIN_USER:
                                {
                                    this.OnIPCUserData(package.CmdSubID, package.Buffer, package.DataSize);
                                } break;

                            // �û���Ϣ
                            case Protocol.MDM_GR_USER:
                                {
                                    this.OnUserData(package.CmdSubID, package.Buffer, package.DataSize);
                                } break;

                            // ϵͳ��Ϣ
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

                    // �������ӵ���ҽṹ
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
                                // ���ӵ������Ϣ�ṹ
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
                        // �����û�����
                        this.UpdateUserScore(stuUser);

                        // ����û����Ҳ��б���
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

            // ����״̬
            case Protocol.SUB_GR_USER_STATUS:
                {
                    object obj = Util.BytesToStruct<Protocol.UserStruct>(cbDataBuffer);
                    if (obj == null) return;

                    Protocol.UserStruct stuUser = (Protocol.UserStruct)obj;
                    UserModel user = this.GetUserByUserID(stuUser.dwUserID);
                    if (user == null) return;

                    // �����û�״̬
                    user.InitData(ref stuUser);

                    // �����û��б�
                    UserList userlist = UserList.GetInstance();
                    userlist.UpdateBody(user);

                    switch (user.Status)
                    {
                        case Define.US_NULL:        // û��״̬(�û��뿪����)
                            break;
                        case Define.US_READY:       // ͬ��״̬
                            break;
                        case Define.US_PLAY:        // ��Ϸ״̬
                            break;
                        case Define.US_LOOKON:      // �Թ�״̬
                            break;
                        case Define.US_OFFLINE:     // ����״̬
                            break;
                        case Define.US_FREE:		// վ��״̬
                            {
                                // �ȴ����UI�б����Ƴ�
                                userlist.RemoveBody(user);

                                // ����ٴ������б����Ƴ�
                                this.RemoveUserByID(user.UserID);
                            }
                            break;
                        case Define.US_SIT:		    // ����״̬
                            break;

                        default:
                            break;
                    }
                } break;

            // ���·���
            case Protocol.SUB_GR_USER_SCORE:
                {
                    object obj = Util.BytesToStruct<Protocol.UserStruct>(cbDataBuffer);
                    if (obj == null) return;

                    // ���������û�����
                    Protocol.UserStruct stuUser = (Protocol.UserStruct)obj;
                    this.UpdateUserScore(stuUser);

                    // �����û��б�
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

    // �����Ϣ
    private void OnGameFrame(ushort wSubCmdID, byte[] cbDataBuffer, ushort wDataSize)
    {
        switch (wSubCmdID)
        {
            // ��Ϸ����
            case Protocol.SUB_GF_OPTION:    break;

            // ������Ϣ
            case Protocol.SUB_GF_SCENE:     break;

            // �û�����
            case Protocol.SUB_GF_USER_CHAT: break;

            // ϵͳ��Ϣ
            case Protocol.SUB_GF_MESSAGE:
                {
                    object obj = Util.BytesToStruct<Protocol.tagGS_Message>(cbDataBuffer);
                    if (obj != null)
                    {
                        Protocol.tagGS_Message msg = (Protocol.tagGS_Message)obj;
                        string strMsg = Util.TcharToString(msg.szContent);

                        UserList userlist = UserList.GetInstance();

                        // ��ʾ��Ϣ
                        if (Define.SMT_INFO == (msg.wMessageType & Define.SMT_INFO))
                        {
                            Debug.Print("����:��ʾ��Ϣ - {0}", strMsg);
                            userlist.ParseTextMsg(String.Format("\n[0xFF000000]��ϵͳ��ʾ��[-]{0}", strMsg));
                        }

                        // ȫ����Ϣ
                        if (Define.SMT_GLOBAL == (msg.wMessageType & Define.SMT_GLOBAL))
                        {
                            Debug.Print("����:ȫ����Ϣ - {0}", strMsg);
                            userlist.ParseTextMsg(String.Format("\n[0xFF000000]��ϵͳ��ʾ��[-]{0}", strMsg));
                        }

                        // ������Ϣ
                        if (Define.SMT_EJECT == (msg.wMessageType & Define.SMT_EJECT))
                        {
                            Debug.Print("����:������Ϣ - {0}", strMsg);
                        }

                        // �رշ���
                        if (Define.SMT_CLOSE_ROOM == (msg.wMessageType & Define.SMT_CLOSE_ROOM))
                        {
                            Debug.Print("����:�رշ���");
                        }

                        // �ж�����
                        if (Define.SMT_INTERMIT_LINE == (msg.wMessageType & Define.SMT_INTERMIT_LINE))
                        {
                            Debug.Print("����:�ж�����");
                        }
                    }
                } break;

            default:
                break;
        }
    }
}