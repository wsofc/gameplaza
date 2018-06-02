using System;
using System.Text;
using System.Drawing;
using System.Threading;
using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;

public class NestedWindow
{
    static NestedWindow g_Instance  = null;

    UnityWinForm.UnityWinForm m_WinForm     = null;             // Winform主窗口
    Control m_ParentCtl                     = null;             // 嵌套窗口依附的父控件
    Process m_NestedProcess                 = null;             // 嵌套窗口进程
    EventHandler m_StartCompleteCallBack    = null;             // 启动完成回调
    string m_strVersion                     = "";               // 游戏版本号
    string m_strWndTitle                    = "";               // 游戏窗口标题

    SendOrPostCallback m_ExitCallBack       = null;             // 嵌套窗口进程退出事件回调
    SynchronizationContext m_SyncContext    = null;             // UI线程的同步上下文

    IntPtr m_hNestedWnd                     = IntPtr.Zero;      // 嵌套窗口句柄
    bool m_bDaemonThreadFlag                = false;            // 守护线程标志

    Rectangle m_LastWinRect                 = new Rectangle();  // 上一次窗口位置

    NestedWindow()
    { }

    ~NestedWindow()
    { }

    /// <summary> 版本号 </summary>
    public string Version { get { return this.m_strVersion; } }

    /// <summary> 窗口标题 </summary>
    public string WindowTitle { get { return this.m_strWndTitle; } }

    /// <summary> 游戏是否完全启动完毕</summary>
    public bool StartCompleteFinished { set; get; }

    public static NestedWindow GetInstance()
    {
        if(NestedWindow.g_Instance == null)
        {
            NestedWindow.g_Instance = new NestedWindow();
        }
        return NestedWindow.g_Instance;
    }

    public Process GetNestedProcess() { return this.m_NestedProcess; }

    public void Init(UnityWinForm.UnityWinForm form, Control control)
    {
        this.m_WinForm = form;
        this.m_ParentCtl = control;
    }

    public IntPtr GetNestedWindow() { return this.m_hNestedWnd; }

    public void Start(string filename, EventHandler e, string strComLine, SynchronizationContext context)
    {
        this.m_SyncContext = context;
        if (this.m_NestedProcess != null)
        {
            this.Stop();
        }
        try
        {
            IntPtr hParentWnd = this.m_ParentCtl.Handle;
            Thread t = new Thread(() =>
            {
                Debug.Print("异步创建游戏进程 start...");

                // 绑定回调
                this.m_StartCompleteCallBack = e;

                // 读取游戏版本
                this.m_strVersion = this.GetVersionInfo(filename);

                // 初始化指定启动的进程
                this.m_NestedProcess = new Process();
                this.m_NestedProcess.StartInfo.FileName = filename;
                this.m_NestedProcess.StartInfo.UseShellExecute = false;
                this.m_NestedProcess.StartInfo.CreateNoWindow = true;
                this.m_NestedProcess.StartInfo.Arguments = String.Format("-parentHWND {0} {1}", hParentWnd, strComLine);
                this.m_NestedProcess.StartInfo.RedirectStandardInput = true;
                this.m_NestedProcess.StartInfo.RedirectStandardOutput = false;

                // 启动指定进程
                this.m_NestedProcess.Start();
                
                // 阻塞直到加载完毕
                this.m_NestedProcess.WaitForInputIdle();
                this.OnStartCallBack(hParentWnd);
                
                Debug.Print("异步创建游戏进程 end...");
            });
            t.Start();
        }
        catch
        {
            if (this.m_NestedProcess != null)
            {
                if (!this.m_NestedProcess.HasExited)
                {
                    this.m_NestedProcess.Kill();
                }
                this.m_NestedProcess = null;
            }
        }
    }

    public void Stop()
    {
        if (this.m_NestedProcess != null)
        {
            try
            {
                if (!this.m_NestedProcess.HasExited)
                {
                    this.m_NestedProcess.Kill();
                }
            }
            catch (Exception ex)
            {
                Debug.Assert(false, "NestedWindow Stop Failed");
                Debug.Print("NestedWindow Stop Failed [Msg:{0}]", ex.Message);
            }

            this.m_NestedProcess = null;
        }
    }

    void OnStartCallBack(IntPtr hParentWnd)
    {
        //WinAPI.SetParent(this.m_hNestedWnd, this.m_ParentCtl.Handle);
        //WinAPI.SetWindowLong(this.m_hNestedWnd, WinAPI.GWL_STYLE, WinAPI.WS_VISIBLE);

        // 注意
        // 上述设置了StartInfo.Arguments = parentHWND时,会立马嵌入窗口,效果很完美,但无法通过进程获取主窗口句柄,估计是和父控件融合了(嵌入unity是这样)
        // 如果不设置StartInfo.Arguments = parentHWND时,嵌入窗口不是立刻效果,无论怎么设置总会先闪一下即将出现的窗口,然后采用SetParent,SetWindowLong实现嵌套
        // 效果很差,但可以立即获取主窗口句柄,但是对窗口设置等操作会无效
        // 综上所述,这里采用第一种,设置Arguments参数

        // 获取对应子窗口句柄
        this.m_hNestedWnd = WinAPI.GetChildWindowByPid(hParentWnd, this.m_NestedProcess.Id);

        // 设置子窗口标题内容
        this.m_strWndTitle = String.Format("{0} v{1}", Protocol.GameName, this.m_strVersion);
        
        // UI线程中执行
        if (this.m_SyncContext != null)
        {
            this.m_SyncContext.Post(new SendOrPostCallback((object state) =>
            {
                // 回调
                this.m_StartCompleteCallBack(this, null);

                // 游戏启动步骤:加载游戏进程完毕
                this.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_PROCESS);
            }), null);
        }
    }

    /// <summary> 发送游戏进程ID至大厅 </summary>
    public void SendGameProcessInfoToPlaza()
    {
        // 发送游戏进程ID至大厅
        Process gameProcess = this.GetNestedProcess();
        WinAPI.PROCESS_INFORMATION tProcessInfo = new WinAPI.PROCESS_INFORMATION();
        tProcessInfo.hProcess = 0;
        tProcessInfo.hThread = 0;
        tProcessInfo.dwProcessId = (UInt32)gameProcess.Id;
        tProcessInfo.dwThreadId = (UInt32)System.Threading.Thread.CurrentThread.ManagedThreadId;

        Server server = Server.GetInstance();
        server.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_GAMEPROCESS, Protocol.IPC_FLAG_TO_GAMEPLAZA, tProcessInfo);
    }

    public void StartGamSteps(Protocol.StartGameStep eStep)
    {
        if (this.m_WinForm == null) return;

        float fPercent = (float)eStep / ((float)Protocol.StartGameStep.SGS_STEPS);
        fPercent = fPercent > 1.0f ? 1.0f : fPercent;
        fPercent *= 100;
        this.m_WinForm.ShowLoadPercent((int)fPercent);

        if (eStep == Protocol.StartGameStep.SGS_LOAD_COMPLETE)
        {
            // 自适应游戏窗口大小
            this.AdaptGameWndSize();

            // 加载完毕
            fPercent = 100.0f;
            this.m_WinForm.ShowLoadPercent((int)fPercent);
        }
    }

    public void AdaptGameWndSize()
    {
        if (this.m_WinForm == null) return;

        // 启动完成后重新刷新下游戏窗口位置,否则在其他多屏显示器下,会无法正常显示窗口(其实是窗口位置不对)
        System.Drawing.Rectangle rect = this.m_WinForm.GetGameClientRect();
        if (rect.Width <= 0 || rect.Height <= 0 || this.m_LastWinRect == rect) return;

        // 下发消息,适配游戏内部分辨率
        do
        {
            WinAPI.SIZE win_size = new WinAPI.SIZE();
            win_size.cx = rect.Width;
            win_size.cy = rect.Height;
            Server server = Server.GetInstance();
            server.SendDataToGame(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_RESIZE, win_size);
        } while (false);

        // 刷新Winform游戏窗口
        WinAPI.ShowWindow(this.m_hNestedWnd, WinAPI.SW_SHOW);
        WinAPI.MoveWindow(this.m_hNestedWnd, rect.X, rect.Y, rect.Width, rect.Height, WinAPI.TRUE);
        
        // 保存上次窗口位置
        m_LastWinRect.X = rect.X;
        m_LastWinRect.Y = rect.Y;
        m_LastWinRect.Width = rect.Width;
        m_LastWinRect.Height = rect.Height;

        // 延时刷新右侧列表UI
        System.Timers.Timer t = new System.Timers.Timer(100);
        t.Enabled = false;
        t.AutoReset = false;
        t.Elapsed += new System.Timers.ElapsedEventHandler((object sender, System.Timers.ElapsedEventArgs ea) =>
        {
            this.m_SyncContext.Post((object state) =>
            {
                this.m_WinForm.RefreshListUI();
            }, null);
        });
    }

    public void StartGameInfo()
    {
        if (this.m_WinForm == null) return;

        // 请求游戏场景初始化
        Server server = Server.GetInstance();
        server.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_START_INFO, Protocol.IPC_FLAG_TO_GAMEPLAZA);

        // 设置场景初始化字段
        DataManager dataMgr = DataManager.GetInstance();
        dataMgr.SceneInited = true;

        // 游戏启动完毕
        this.StartCompleteFinished = true;
    }

    /// <summary> 激活Unity窗口 </summary>
    internal void ActivateNestedWindow()
    {
        if (WinAPI.FALSE == WinAPI.IsWindow(this.m_hNestedWnd)) return;
        WinAPI.SendMessage(this.m_hNestedWnd, (uint)WinAPI.WM_ACTIVATE, WinAPI.PTRUE, IntPtr.Zero);
    }

    /// <summary> 失去激活Unity窗口 </summary>
    internal void DeactivateNestedWindow()
    {
        WinAPI.SendMessage(this.m_hNestedWnd, (uint)WinAPI.WM_ACTIVATE, WinAPI.PFALSE, IntPtr.Zero);
    }

    public void SetWindText(string strWndTitle)
    {
        WinAPI.SetWindowText(this.m_hNestedWnd, strWndTitle);
    }

    public string GetVersionInfo(string path)
    {
        string strVersions = "";

        try
        {
            FileVersionInfo file = System.Diagnostics.FileVersionInfo.GetVersionInfo(path);

            //版本号显示为"主版本号.次版本号.内部版本号.专用部件号"
            strVersions = String.Format("{0}.{1}.{2}.{3}", file.FileMajorPart, file.FileMinorPart, file.FileBuildPart, file.FilePrivatePart);
        }
        catch (System.IO.FileNotFoundException ex)
        {
            //Debug.Assert(false, String.Format("GetVersionInfo Failed [path = {0}, emsg = {1}]", path, ex.Message));
            Debug.Print("GetVersionInfo Failed [path = {0}, emsg = {1}]", path, ex.Message);
        }

        return strVersions;
    }

    /// <summary> 创建一个守护线程(用于监听嵌套进程) </summary>
    public void StartDaemonThread(SendOrPostCallback cb)
    {
        this.m_ExitCallBack = cb;
        this.m_bDaemonThreadFlag = true;
        Thread t = new Thread(this.OnDaemonThread);
        t.Start();
    }

    public void StopDaemonThread()
    {
        this.m_bDaemonThreadFlag = false;
    }

    void OnDaemonThread()
    {
        Debug.Print("守护线程: Start...");
        while (this.m_bDaemonThreadFlag)
        {
            // 若游戏进程不存在或已退出
            if (this.m_NestedProcess == null || this.m_NestedProcess.HasExited)
            {
                if (this.m_SyncContext != null && this.m_ExitCallBack != null)
                {
                    this.m_SyncContext.Post(this.m_ExitCallBack, this);
                }

                Debug.Print("守护线程: Abnormal Exit...");
                this.StopDaemonThread();
                break;
            }

            Thread.Sleep(1000);
        }
        Debug.Print("守护线程: End...");
    }

    /// <summary> 游戏进程是否存在 </summary>
    public bool IsNestedThreadExist()
    {
        return this.m_NestedProcess != null && !this.m_NestedProcess.HasExited;
    }

    public void NextWaitForExit()
    {
        this.StopDaemonThread();
        if (this.m_SyncContext != null)
        {
            this.m_SyncContext.Post((object obj) => { this.WaitForExit(); }, this);
        }
    }

    public void WaitForExit()
    {
        // 关闭守护线程
        this.StopDaemonThread();

        // 通知Winform关闭窗口
        if (this.m_WinForm != null)
        {
            WinAPI.ShowWindow(this.m_WinForm.Handle, WinAPI.SW_HIDE);
            this.m_WinForm.CloseForm(true);
        }
    }

    public void KillNestedProcess()
    {
        // 关闭守护线程
        this.StopDaemonThread();

        // 若进程存在,则关闭进程
        if (this.m_NestedProcess != null && !this.m_NestedProcess.HasExited)
        {
            this.m_NestedProcess.Kill();
            this.m_NestedProcess.Close();
            this.m_NestedProcess = null;
        }
    }
}