using System;
using System.Text;
using System.Diagnostics;
using System.Collections;
using System.Runtime.InteropServices;

/// <summary> IPC通讯层(挂在到主场景中生效) </summary>
// 密封类(不允许继承)
public sealed class Server : EventNode
{
    private static Server g_Instance                    = null;                                 // 单例
    private static StringBuilder g_lpReturnedString     = new System.Text.StringBuilder(1024);  // 字符串缓冲区 

    private bool m_bHookSuccess                         = false;                                // 是否安装钩子成功
    private bool m_bHandled                             = false;                                // 是否拦截钩子消息(默认false,钩子获取到消息处理,不拦截,继续传递)
    private IntPtr m_idHook                             = IntPtr.Zero;                          // 钩子句柄
    private WinAPI.HOOKPROC m_lpHoolProc                = null;                                 // 钩子回调函数
    private IntPtr m_hWnd                               = IntPtr.Zero;                          // Form主窗口句柄

    private bool m_bFormComplete                        = false;                                // 框架初始化是否完毕
    private bool m_bGameComplete                        = false;                                // 游戏初始化是否完毕

    private ServerPackage m_pPackage                    = null;                                 // 事件缓冲包
    private IntPtr m_pBuffer                            = IntPtr.Zero;                          // IPC协议缓冲区
    private IntPtr m_pCopyData                          = IntPtr.Zero;                          // COPYDATA结构缓冲区
    private Protocol.IPC_Buffer m_stuIpcBuffer;                                                 // IPC协议数据缓冲结构体
    private WinAPI.COPYDATASTRUCT m_stuCopyData;                                                // 非托管进程通信结构体

    private UInt16 m_wLastErrorMainCmdID                = 0;                                    // 记录发生错误时上一次的主命令码
    private UInt16 m_wLastErrorSubCmdID                 = 0;                                    // 记录发生错误时上一次的子命令码
    
    Server()
    { }

    ~Server()
    {
        // 卸载钩子
        this.CloseHook();
        m_lpHoolProc = null;

        // 清理IPC缓冲区
        m_pPackage = null;
        m_stuIpcBuffer.bData = null;

        // 清理非托管资源
        try
        {
            Marshal.FreeHGlobal(m_pBuffer);
            Marshal.FreeHGlobal(m_pCopyData);
        }
        catch (ArgumentException aex)
        {
            Debug.Assert(false, String.Format("Server - IPC_Module FreeHGlobal Failed [Msg = {0}]", aex));
        }
        finally
        {
            Debug.Print("Server - IPC_Module FreeHGlobal Ok");
        }
    }

    /// <summary> Winform框架是否启动完毕 </summary>
    public bool IsFormComplete { get { return this.m_bFormComplete; } }

    /// <summary> 游戏框架是否启动完毕 </summary>
    public bool IsGameComplete { get { return this.m_bGameComplete; } }

    public static Server GetInstance()
    {
        if (Server.g_Instance == null)
        {
            Server.g_Instance = new Server();
        }
        return Server.g_Instance;
    }

    public void Init(IntPtr hWnd)
    {
        this.InitPlazaWindow();
        this.InitServer();
        this.LoadHood(hWnd);
    }

    /// <summary> 写入.ini文件
    /// <para> lpFileName   - 相对路径名     </para>
    /// <para> lpAppName    - 分类段落名     </para>
    /// <para> lpKeyName    - 键名           </para>
    static public void WriteINIConfig(string lpFileName, string lpAppName, string lpKeyName, string lpValue)
    {
        if (lpFileName.Equals("") == true || lpAppName.Equals("") == true || lpKeyName.Equals("") == true) return;

        // 文件路径
        string strFileName = String.Format("{0}\\{1}", System.Windows.Forms.Application.StartupPath, lpFileName);

        // 写入示例
        WinAPI.WritePrivateProfileString(lpAppName, lpKeyName, lpValue, strFileName);
    }

    /// <summary> 读取.ini文件
    /// <para> lpFileName   - 相对路径名     </para>
    /// <para> lpAppName    - 分类段落名     </para>
    /// <para> lpKeyName    - 键名           </para>
    /// </summary>
    static public string ReadINIConfig(string lpFileName, string lpAppName, string lpKeyName)
    {
        // 返回值
        string strReturn = "";
        if (lpFileName.Equals("") == true || lpAppName.Equals("") == true || lpKeyName.Equals("") == true) return strReturn;

        // 文件路径
        string strFileName = String.Format("{0}\\{1}", System.Windows.Forms.Application.StartupPath, lpFileName);
        if (System.IO.File.Exists(strFileName))
        {
            Server.g_lpReturnedString.Length = 0;
            WinAPI.GetPrivateProfileString(lpAppName, lpKeyName, "", Server.g_lpReturnedString, (UInt32)Server.g_lpReturnedString.Capacity, strFileName);
            strReturn = Server.g_lpReturnedString.ToString();
        }

        return strReturn;
    }

    // 绑定大厅窗口
    void InitPlazaWindow()
    {
        // 编译器属性->条件编译->添加GAME_TEST_SWITCH字段
        // GAME_TEST_SWITCH 是否开启游戏测试模式(切记切记 发布时一定要去掉GAME_TEST_SWITCH条件编译字段)
#if GAME_TEST_SWITCH
        // 写入示例
        //Server.WriteINIConfig("ShellFormTest.ini", "Config", "PlazaWndName", "373游戏中心");
        //Server.WriteINIConfig("ShellFormTest.ini", "Config", "GameName", "百人牛牛");
        //Server.WriteINIConfig("ShellFormTest.ini", "Config", "GameModelName", "Ox100.exe");

        // 读取.INI配置文件
        string PlazaWndName     = Server.ReadINIConfig("ShellFormTest.ini", "Config", "PlazaWndName");
        string GameName         = Server.ReadINIConfig("ShellFormTest.ini", "Config", "GameName");
        string GameModelName    = Server.ReadINIConfig("ShellFormTest.ini", "Config", "GameModelName");

        // 初始化基础数据
        Protocol.PlazaWnd       = WinAPI.FindWindow(null, PlazaWndName); ;
        Protocol.GameName       = GameName;
        Protocol.GameModelName  = GameModelName;
        Protocol.GameFileName   = System.IO.Path.GetFileNameWithoutExtension(Protocol.GameModelName);

        // 读取默认玩家列表显示配置
        #region
        do 
        {
            string strOpenUserList = Server.ReadINIConfig("ShellFormTest.ini", "Config", "OpenUserList");
            string strOpenPrintUserInOutMsg = Server.ReadINIConfig("ShellFormTest.ini", "Config", "OpenPrintUserInOutMsg");

            byte cbOpenUserList = 0;
            byte cbOpenPrintUserInOutMsg = 0;
            byte.TryParse(strOpenUserList, out cbOpenUserList);
            byte.TryParse(strOpenPrintUserInOutMsg, out cbOpenPrintUserInOutMsg);

            Protocol.GameConfig.cbOpenUserList = cbOpenUserList;
            Protocol.GameConfig.cbOpenPrintUserInOutMsg = cbOpenPrintUserInOutMsg;
        } while (false);
        #endregion
#else
        string strCmdLine =  Environment.CommandLine;
        int iBegin = strCmdLine.IndexOf("[") + 1;
        int iEnd = strCmdLine.IndexOf("]");
        if (iBegin >= 0 && iEnd > 0)
        {
            string str = strCmdLine.Substring(iBegin, iEnd - iBegin);
            string[] strArray = str.Split(',');

            Protocol.PlazaWnd = (IntPtr)int.Parse(strArray[0]);
            Protocol.GameName = strArray[1];
            Protocol.GameModelName = strArray[2];
            Protocol.GameFileName = System.IO.Path.GetFileNameWithoutExtension(Protocol.GameModelName);
        }
        else
        {
            // 完全退出
            System.Environment.Exit(0);
        }
#endif
        // 设置游戏资源路径
        Protocol.GameResPath = String.Format("{0}\\{1}_Data\\Resources\\ShellForm\\", Protocol.GameFileName, Protocol.GameFileName);

        // 读取布局配置(是否显示加载进度条模式)
        #region
        do
        {
            string lpFileName = Protocol.GameResPath + "Config.ini";
            string lpFullFileName = String.Format("{0}\\{1}", System.Windows.Forms.Application.StartupPath, lpFileName);
            if (System.IO.File.Exists(lpFullFileName))
            {
                string strGameLoadingShow   = Server.ReadINIConfig(lpFileName, "Config", "GameLoadingShow");
                string strGameLoadingColor  = Server.ReadINIConfig(lpFileName, "Config", "GameLoadingColor");
                string strGameLoadingImg    = Server.ReadINIConfig(lpFileName, "Config", "GameLoadingImg");

                if (strGameLoadingShow.Equals("")) return;

                byte cbGameLoadingShow      = 0;
                UInt32 uGameLoadingColor    = 0xFF000000;
                byte.TryParse(strGameLoadingShow, out cbGameLoadingShow);
                try
                {
                    uGameLoadingColor = Convert.ToUInt32(strGameLoadingColor, 16);
                }
                catch (Exception ex)
                {
                    Debug.Print("Server - InitPlazaWindow, Read GameResConfig-LoadingBkColor Failed! Msg = {0}", ex.ToString());
                }

                Protocol.GameLoadingShow    = cbGameLoadingShow;
                Protocol.GameLoadingColor   = uGameLoadingColor;
                Protocol.GameLoadingImg     = strGameLoadingImg;

                // 无进度条模式,加载(实现秒开效果,则这里必须读取列表配置等,覆盖上面读取后的结果)
                if (Protocol.GameLoadingShow == Define.FALSE)
                {
                    string strOpenUserList = Server.ReadINIConfig(lpFileName, "Config", "OpenUserList");
                    string strOpenPrintUserInOutMsg = Server.ReadINIConfig(lpFileName, "Config", "OpenPrintUserInOutMsg");

                    byte cbOpenUserList = 0;
                    byte cbOpenPrintUserInOutMsg = 0;
                    byte.TryParse(strOpenUserList, out cbOpenUserList);
                    byte.TryParse(strOpenPrintUserInOutMsg, out cbOpenPrintUserInOutMsg);

                    Protocol.GameConfig.cbOpenUserList = cbOpenUserList;
                    Protocol.GameConfig.cbOpenPrintUserInOutMsg = cbOpenPrintUserInOutMsg;

                    // 右侧玩家列表显示配置生效
                    UserList userlist = UserList.GetInstance();
                    userlist.OpenUserList = Protocol.GameConfig.cbOpenUserList == Define.TRUE;
                    userlist.OpenPrintUserInOutMsg = Protocol.GameConfig.cbOpenPrintUserInOutMsg == Define.TRUE;
                }
            }
        } while (false);
        #endregion
    }

    // 加载化钩子
    void InitServer()
    {
        // 初始化钩子
        m_lpHoolProc    = new WinAPI.HOOKPROC(this.OnHookProc);

        // 初始化事件对象
        m_pPackage      = new ServerPackage();

        // 初始化IPC缓冲区
        m_stuIpcBuffer  = new Protocol.IPC_Buffer(0);
        m_pBuffer       = Marshal.AllocHGlobal(Protocol.IPC_BUFFER);

        // 初始化COPYDATA缓冲区
        m_stuCopyData   = new WinAPI.COPYDATASTRUCT();
        m_pCopyData     = Marshal.AllocHGlobal(Marshal.SizeOf(m_stuCopyData));
    }

    // 加载结束回调
    public void OnLoadResult(bool bRes)
    {
        this.m_bFormComplete = bRes;

        if (bRes)
        {
            // 启动框架完成
            this.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_START_OK, Protocol.IPC_FLAG_TO_GAMEPLAZA);

            // 游戏启动步骤:挂载Winform钩子完毕
            DataManager dataMgr = DataManager.GetInstance();
            dataMgr.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_HOOK);
        }
        else
        {
            // 启动框架失败
            this.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_START_NO, Protocol.IPC_FLAG_TO_GAMEPLAZA);
        }
    }

    // 设置窗口标题(加入版本号等)
    public void SetGameWindowText(IntPtr hWnd)
    {
#if !UNITY_EDITOR && UNITY_STANDALONE_WIN
                if (Define.TRUE == WinAPI.IsWindow(hWnd))
                {
                    // 获取窗口标题
                    System.Text.StringBuilder strTitle = new System.Text.StringBuilder(Define.MAX_SYSMESSAGE_LEN);
                    WinAPI.GetWindowText(m_hWnd, strTitle, strTitle.Capacity);

                    // 格式化版本号
                    System.Text.StringBuilder strVersion = new System.Text.StringBuilder(Define.MAX_PATH);
                    strVersion.AppendFormat("   v{0:D2}{1:D2}", Define.VER_GAME_HIGH, Define.VER_GAME_LOW);
                    int iStart = 4;
                    int iEnd = strVersion.Length;
                    for (int i = iStart; i <= iEnd; ++i)
                    {
                        strVersion.Insert(++i, '.');
                    }

                    // 追加字符
                    strTitle.Append(strVersion);
                    strTitle.Append("   按F1键全屏/退出全屏, Esc键退出全屏");

                    // 设置窗口标题
                    WinAPI.SetWindowText(m_hWnd, strTitle.ToString());
                }
#endif
    }

    // ------------------------------------------------------------------------------------------------------------------------------
    // 初始化钩子

    // 安装钩子
    void LoadHood(IntPtr hWnd)
    {
        Debug.Print("Server - Start Load Hook...");

        // 获取窗口句柄
        this.m_hWnd = hWnd;
        if (Define.TRUE == WinAPI.IsWindow(m_hWnd) && m_lpHoolProc != null)
        {
            // 设置窗口标题
            this.SetGameWindowText(this.m_hWnd);

            // 获取当前线程ID
            IntPtr pid = IntPtr.Zero;
            uint tid = WinAPI.GetWindowThreadProcessId(m_hWnd, ref pid);

            // 安装钩子
            IntPtr hMod = IntPtr.Zero;
            m_idHook = new IntPtr(WinAPI.SetWindowsHookEx(WinAPI.WH_CALLWNDPROC, m_lpHoolProc, hMod, tid));
            if (m_idHook != IntPtr.Zero)
            {
                Debug.Print("Server - Load Hook Success");

                // 安装钩子成功
                m_bHookSuccess = true;

                //保持活动 避免 回调过程 被垃圾回收
                GCHandle.Alloc(m_lpHoolProc);

                this.OnLoadResult(true);
            }
            else
            {
                // 钩子安装失败
                m_bHookSuccess = false;
                WinAPI.UnhookWindowsHookEx(m_idHook);

                this.OnLoadResult(false);
            }
        }
        else
        {
            m_bHookSuccess = false;
        }

        if (!m_bHookSuccess)
        {
            Debug.Print("Server - Load Hook Failed [ecode = {0}, hwnd = {1}, lpHoolProc = {2}, idHook = {3}]"
                , WinAPI.GetLastError(), m_hWnd, m_lpHoolProc, m_idHook);

            m_hWnd = IntPtr.Zero;
            m_bHookSuccess = false;
            this.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_START_FAILED, Protocol.IPC_FLAG_TO_GAMEPLAZA);
        }
    }

    // 卸载钩子 
    void CloseHook()
    {
        Debug.Print("Server - Close Hook");

        if (m_bHookSuccess)
        {
            WinAPI.UnhookWindowsHookEx(m_idHook);
        }

        m_bHookSuccess = false;
        m_bHandled = false;
        m_idHook = IntPtr.Zero;
        m_lpHoolProc = null;
    }

    // 钩子监听
    Int32 OnHookProc(int code, IntPtr wParam, IntPtr lParam)
    {
        try
        {
            m_bHandled = false;

            // 提取钩子结构体
            WinAPI.CWPSTRUCT cwStruct = (WinAPI.CWPSTRUCT)Marshal.PtrToStructure(lParam, typeof(WinAPI.CWPSTRUCT));
            switch (cwStruct.message)
            {
                case WinAPI.WM_SYSCOMMAND:
                    {
                        // 若要拦截原始未处理过的消息,请用WH_GETMESSAGE钩子模式
                        // 具体的请查阅API详情

                        // 最小化
                        if (WinAPI.SC_MINIMIZE == (cwStruct.wParam.ToInt32() & 0xFFF0))
                        { }
                        // 最大化
                        else if (WinAPI.SC_MAXIMIZE == (cwStruct.wParam.ToInt32() & 0xFFF0))
                        { }
                    } break;

                case WinAPI.WM_COPYDATA:
                    {
                        WinAPI.COPYDATASTRUCT cdStruct = (WinAPI.COPYDATASTRUCT)Marshal.PtrToStructure((IntPtr)cwStruct.lParam
                            , typeof(WinAPI.COPYDATASTRUCT));

                        Protocol.IPC_Buffer buff = (Protocol.IPC_Buffer)Marshal.PtrToStructure((IntPtr)cdStruct.lpData
                            , typeof(Protocol.IPC_Buffer));

                        this.m_wLastErrorMainCmdID = buff.Head.wMainCmdID;
                        this.m_wLastErrorSubCmdID = buff.Head.wSubCmdID;

                        UInt32 dwFlag = (UInt32)cdStruct.dwData;
                        switch (dwFlag)
                        {
                            case Protocol.IPC_FLAG_TO_SERVER:
                            case Protocol.IPC_FLAG_TO_GAMEPLAZA:
                                {
                                    // 拦截游戏发给大厅的控制消息
                                    switch (buff.Head.wMainCmdID)
                                    {
                                        case Protocol.IPC_MAIN_CONCTROL:
                                            {
                                                this.OnIPCMainControl(buff.Head.wSubCmdID, buff.bData, buff.Head.wDataSize);
                                            }break;
                                        default:
                                            break;
                                    }

                                    this.SendDataToPlaza(buff.Head.wMainCmdID, buff.Head.wSubCmdID, dwFlag, buff.bData, buff.Head.wDataSize);
                                } break;

                            case Protocol.IPC_FLAG_TO_WINFROM:
                                {
                                    this.DispatchMsg(ref buff);
                                    this.SendDataToGame(buff.Head.wMainCmdID, buff.Head.wSubCmdID, buff.bData, buff.Head.wDataSize);
                                } break;

                            case Protocol.IPC_FLAG_TO_GAME: break;
                            default: break;
                        }
                    } break;

                default:
                    break;
            }

            // 拦截
            if (m_bHandled)
            {
                return Define.TRUE;
            }
            // 继续传递
            else
            {
                return WinAPI.CallNextHookEx(m_idHook, code, wParam, lParam);
            }
        }
        catch (Exception ex)
        {
            Debug.Print("Server - OnHookProc Failed [Msg = {0}]", ex.ToString());
            Debug.Print("Server - OnHookProc Failed [e_mid = {0}, e_sid = {1}]", this.m_wLastErrorMainCmdID, this.m_wLastErrorSubCmdID);
            return Define.TRUE;
        }
    }

    // ------------------------------------------------------------------------------------------------------------------------------
    // 处理IPC消息

    bool SendData(IntPtr hWnd, ushort wMainCmdID, ushort wSubCmdID, UInt32 dwFlag = Protocol.IPC_FLAG_TO_SERVER, object stuData = null, UInt16 wDataSize = 0)
    {
        //Debug.Assert(WinAPI.IsWindow(hWnd) == Define.TRUE, String.Format("Server - SendMsg Failed, Not Fount the Target Window [{0}]", hWnd));
        if (WinAPI.IsWindow(hWnd) == Define.FALSE)
        {
            Debug.Print("Server - SendMsg Failed, Not Fount the Target Window [{0}]", hWnd);
            return false;
        }

        // 初始化Protocol.LIPC_Buffer结构对象
        m_stuIpcBuffer.Head.wVersion = Protocol.IPC_VER;
        m_stuIpcBuffer.Head.wMainCmdID = wMainCmdID;
        m_stuIpcBuffer.Head.wSubCmdID = wSubCmdID;
        m_stuIpcBuffer.Head.wDataSize = 0;
        Array.Clear(m_stuIpcBuffer.bData, 0, m_stuIpcBuffer.bData.Length);

        try
        {
            // 填充ipc缓冲数据
            if (stuData != null)
            {
                if (stuData.GetType() == typeof(byte[]))
                {
                    byte[] cbData = stuData as byte[];
                    Debug.Assert(cbData.Length <= Protocol.IPC_PACKAGE);
                    if (cbData.Length > Protocol.IPC_PACKAGE)
                    {
                        Debug.Print("Server - SendMsg Failed, Too large data [real_size = {0}, spec_size={1}]"
                            , cbData.Length, Protocol.IPC_PACKAGE);
                        return false;
                    }

                    m_stuIpcBuffer.Head.wDataSize = wDataSize;
                    Buffer.BlockCopy(cbData, 0, m_stuIpcBuffer.bData, 0, m_stuIpcBuffer.Head.wDataSize);
                }
                else
                {
                    Debug.Assert(Marshal.SizeOf(stuData) <= Protocol.IPC_PACKAGE);
                    if (Marshal.SizeOf(stuData) > Protocol.IPC_PACKAGE)
                    {
                        Debug.Print("Server - SendMsg Failed, Too large data [real_size = {0}, spec_size={1}]"
                            , Marshal.SizeOf(stuData), Protocol.IPC_PACKAGE);
                        return false;
                    }

                    m_stuIpcBuffer.Head.wDataSize = (ushort)Marshal.SizeOf(stuData);
                    Marshal.StructureToPtr(stuData, m_pBuffer, false);
                    Marshal.Copy(m_pBuffer, m_stuIpcBuffer.bData, 0, m_stuIpcBuffer.Head.wDataSize);
                }
            }

            // 结构体序列化
            Marshal.StructureToPtr(m_stuIpcBuffer, m_pBuffer, false);

            // COPYDATA赋值
            m_stuCopyData.lpData = m_pBuffer;
            m_stuCopyData.dwData = (IntPtr)dwFlag;
            m_stuCopyData.cbData = Protocol.IPC_BUFFER;

            // COPYDATA序列化
            Marshal.StructureToPtr(m_stuCopyData, m_pCopyData, false);

            //发送msg
            WinAPI.SendMessage(hWnd, WinAPI.WM_COPYDATA, IntPtr.Zero, m_pCopyData);
        }
        catch (OutOfMemoryException oex)
        {
            Debug.Print("Server - SendMsg AllocHGlobal Failed [Msg = {0}]", oex);
        }

        return true;
    }

    public bool SendDataToGame(ushort wMainCmdID, ushort wSubCmdID, object stuData = null, UInt16 wDataSize = 0)
    {
        return this.SendData(Protocol.GamesWnd, wMainCmdID, wSubCmdID, Protocol.IPC_FLAG_TO_GAME, stuData, wDataSize);
    }

    public bool SendDataToPlaza(ushort wMainCmdID, ushort wSubCmdID, UInt32 dwFlag, object stuData = null, UInt16 wDataSize = 0)
    {
        return this.SendData(Protocol.PlazaWnd, wMainCmdID, wSubCmdID, dwFlag, stuData, wDataSize);
    }

    void DispatchMsg(ref Protocol.IPC_Buffer buff)
    {
        // 此处拦截有效的消息
        switch (buff.Head.wMainCmdID)
        {
            // 大厅控制
            case Protocol.IPC_MAIN_CONCTROL:
                {
                    this.OnIPCMainControl(buff.Head.wSubCmdID, buff.bData, buff.Head.wDataSize);
                } break;

            // 其他消息都发送至对应的监听者
            default:
                {
                    m_pPackage.Version = buff.Head.wVersion;
                    m_pPackage.DataSize = buff.Head.wDataSize;
                    m_pPackage.CmdMainID = buff.Head.wMainCmdID;
                    m_pPackage.CmdSubID = buff.Head.wSubCmdID;
                    m_pPackage.Buffer = buff.bData;

                    this.SendEvent((int)Protocol.MsgType.MT_GAME_SERVER, m_pPackage);
                } break;
        }
    }

    // 处理大厅下发给Winform的IPC_MAIN_CONCTROL消息
    void OnIPCMainControl(ushort wSubCmdID, byte[] cbDataBuffer, ushort wDataSize)
    {
        switch (wSubCmdID)
        {
            // 游戏窗口启动完毕
            case Protocol.IPC_SUB_START_FINISH:
                {
                    this.m_bGameComplete = true;

                    // 游戏启动步骤:游戏数据加载完毕
                    DataManager dataMgr = DataManager.GetInstance();
                    dataMgr.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_GAME_HOOK);

                    // 无进度条模式,则游戏钩子挂在完毕后,立即适配游戏大小(实现秒开效果)
                    if (Protocol.GameLoadingShow == Define.FALSE)
                    {
                        NestedWindow nest = NestedWindow.GetInstance();
                        nest.AdaptGameWndSize();
                    }
                } break;
            
            // 游戏菜单初始化
            case Protocol.IPC_SUB_WINGAME_MENU_INIT:
                {
                    // 传递至游戏中处理,处理完毕后返回至Winform配置UI
                    // Protocol.IPC_SUB_WINGAME_MENU_CONFIG返回

                    // 游戏启动步骤:玩家列表加载完毕
                    DataManager dataMgr = DataManager.GetInstance();
                    dataMgr.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_USERLIST);
                } break;

            // 游戏菜单配置
            case Protocol.IPC_SUB_WINGAME_MENU_CONFIG:
                {
                    // 解析菜单数据,可以为空
                    ArrayList obj_array = Util.BytesToStructArray<Protocol.tagUserMenuInfo>(cbDataBuffer, wDataSize);

                    // 创建菜单信息
                    UserModel SelfData = DataManager.GetInstance().UserSelf;
                    bool bOpenControl = SelfData.MasterOrder > 0;
                    UserList userlist = UserList.GetInstance();
                    userlist.CreateUserMenu(obj_array, bOpenControl);
                    
                    // 游戏启动步骤:游戏菜单加载完毕
                    DataManager dataMgr = DataManager.GetInstance();
                    dataMgr.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_USERMENU);
                } break;

            // 游戏初始化完毕
            case Protocol.IPC_SUB_WINGAME_FINISHED:
                {
                    // 配置右侧玩家列表栏
                    object obj = Util.BytesToStruct<Protocol.tagGameConfig>(cbDataBuffer);
                    if (obj != null)
                    {
                        Protocol.tagGameConfig stu = (Protocol.tagGameConfig)obj;
                        Protocol.GameConfig = stu;
                    }

                    // 右侧玩家列表显示配置
                    // 只有加载界面有进度条才有显示列表的资格,若无进度条则看游戏文件配置
                    if (Protocol.GameLoadingShow == Define.TRUE)
                    {
                        UserList userlist = UserList.GetInstance();
                        userlist.OpenUserList = Protocol.GameConfig.cbOpenUserList == Define.TRUE;
                        userlist.OpenPrintUserInOutMsg = Protocol.GameConfig.cbOpenPrintUserInOutMsg == Define.TRUE;
                    }

                    // 游戏启动步骤:配置完成
                    DataManager dataMgr = DataManager.GetInstance();
                    dataMgr.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_COMPLETE);
                } break;

            // 游戏打印消息
            case Protocol.IPC_SUB_WINGAME_TIPS_MSG:
                {
                    object obj = Util.BytesToStruct<Protocol.tagGS_Message>(cbDataBuffer);
                    if (obj == null) return;

                    Protocol.tagGS_Message msg = (Protocol.tagGS_Message)obj;
                    string strTxt = Util.TcharToString(msg.szContent);

                    UserList userlist = UserList.GetInstance();
                    userlist.ParseTextMsg("\n");

                    // 提示消息
                    if (Define.SMT_INFO == (msg.wMessageType & Define.SMT_INFO))
                    {
                        userlist.ParseTextMsg(strTxt);
                    }
                    // 全局消息
                    if (Define.SMT_GLOBAL == (msg.wMessageType & Define.SMT_GLOBAL))
                    { }

                    // 弹框消息
                    if (Define.SMT_EJECT == (msg.wMessageType & Define.SMT_EJECT))
                    { }

                    // 关闭房间
                    if (Define.SMT_CLOSE_ROOM == (msg.wMessageType & Define.SMT_CLOSE_ROOM))
                    { }

                    // 中断连接
                    if (Define.SMT_INTERMIT_LINE == (msg.wMessageType & Define.SMT_INTERMIT_LINE))
                    { }
                } break;

            // 心跳检测
            case Protocol.IPC_SUB_HEARTBEAT:
                {
                    this.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_HEARTBEAT, Protocol.IPC_FLAG_TO_GAMEPLAZA);
                } break;

            // 断线重连
            case Protocol.IPC_SUB_GF_RECONNECT:
                {
                    object obj = Util.BytesToStruct<Protocol.tagGameFrameReconnect>(cbDataBuffer);
                    if (obj == null)
                    {
                        this.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_CLOSE_FRAME_EX, Protocol.IPC_FLAG_TO_GAMEPLAZA);
                    }
                    else
                    {
                        Protocol.tagGameFrameReconnect stu = (Protocol.tagGameFrameReconnect)obj;
                        switch (stu.cbStatus)
                        {
                            case Define.TRUE:
                                {
                                    // 重置场景初始化与否字段
                                    DataManager dataMgr = DataManager.GetInstance();
                                    dataMgr.SceneInited = false;
                                } break;

                            case Define.FALSE:
                                {
                                    // 清理玩家列表UI等
                                    UserList userlist = UserList.GetInstance();
                                    userlist.Clear(true, false);
                                } break;
                        }
                    }
                } break;

            // 游戏强退
            case Protocol.IPC_SUB_CLOSE_FRAME:
            case Protocol.IPC_SUB_CLOSE_FRAME_EX:
                {
                    DataManager dataMgr = DataManager.GetInstance();
                    dataMgr.OnCloseGame();
                } break;

            default:
                break;
        }
    }
}
