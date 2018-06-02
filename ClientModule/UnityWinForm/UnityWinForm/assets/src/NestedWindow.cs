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

    UnityWinForm.UnityWinForm m_WinForm     = null;             // Winform������
    Control m_ParentCtl                     = null;             // Ƕ�״��������ĸ��ؼ�
    Process m_NestedProcess                 = null;             // Ƕ�״��ڽ���
    EventHandler m_StartCompleteCallBack    = null;             // ������ɻص�
    string m_strVersion                     = "";               // ��Ϸ�汾��
    string m_strWndTitle                    = "";               // ��Ϸ���ڱ���

    SendOrPostCallback m_ExitCallBack       = null;             // Ƕ�״��ڽ����˳��¼��ص�
    SynchronizationContext m_SyncContext    = null;             // UI�̵߳�ͬ��������

    IntPtr m_hNestedWnd                     = IntPtr.Zero;      // Ƕ�״��ھ��
    bool m_bDaemonThreadFlag                = false;            // �ػ��̱߳�־

    Rectangle m_LastWinRect                 = new Rectangle();  // ��һ�δ���λ��

    NestedWindow()
    { }

    ~NestedWindow()
    { }

    /// <summary> �汾�� </summary>
    public string Version { get { return this.m_strVersion; } }

    /// <summary> ���ڱ��� </summary>
    public string WindowTitle { get { return this.m_strWndTitle; } }

    /// <summary> ��Ϸ�Ƿ���ȫ�������</summary>
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
                Debug.Print("�첽������Ϸ���� start...");

                // �󶨻ص�
                this.m_StartCompleteCallBack = e;

                // ��ȡ��Ϸ�汾
                this.m_strVersion = this.GetVersionInfo(filename);

                // ��ʼ��ָ�������Ľ���
                this.m_NestedProcess = new Process();
                this.m_NestedProcess.StartInfo.FileName = filename;
                this.m_NestedProcess.StartInfo.UseShellExecute = false;
                this.m_NestedProcess.StartInfo.CreateNoWindow = true;
                this.m_NestedProcess.StartInfo.Arguments = String.Format("-parentHWND {0} {1}", hParentWnd, strComLine);
                this.m_NestedProcess.StartInfo.RedirectStandardInput = true;
                this.m_NestedProcess.StartInfo.RedirectStandardOutput = false;

                // ����ָ������
                this.m_NestedProcess.Start();
                
                // ����ֱ���������
                this.m_NestedProcess.WaitForInputIdle();
                this.OnStartCallBack(hParentWnd);
                
                Debug.Print("�첽������Ϸ���� end...");
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

        // ע��
        // ����������StartInfo.Arguments = parentHWNDʱ,������Ƕ�봰��,Ч��������,���޷�ͨ�����̻�ȡ�����ھ��,�����Ǻ͸��ؼ��ں���(Ƕ��unity������)
        // ���������StartInfo.Arguments = parentHWNDʱ,Ƕ�봰�ڲ�������Ч��,������ô�����ܻ�����һ�¼������ֵĴ���,Ȼ�����SetParent,SetWindowLongʵ��Ƕ��
        // Ч���ܲ�,������������ȡ�����ھ��,���ǶԴ������õȲ�������Ч
        // ��������,������õ�һ��,����Arguments����

        // ��ȡ��Ӧ�Ӵ��ھ��
        this.m_hNestedWnd = WinAPI.GetChildWindowByPid(hParentWnd, this.m_NestedProcess.Id);

        // �����Ӵ��ڱ�������
        this.m_strWndTitle = String.Format("{0} v{1}", Protocol.GameName, this.m_strVersion);
        
        // UI�߳���ִ��
        if (this.m_SyncContext != null)
        {
            this.m_SyncContext.Post(new SendOrPostCallback((object state) =>
            {
                // �ص�
                this.m_StartCompleteCallBack(this, null);

                // ��Ϸ��������:������Ϸ�������
                this.StartGamSteps(Protocol.StartGameStep.SGS_LOAD_PROCESS);
            }), null);
        }
    }

    /// <summary> ������Ϸ����ID������ </summary>
    public void SendGameProcessInfoToPlaza()
    {
        // ������Ϸ����ID������
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
            // ����Ӧ��Ϸ���ڴ�С
            this.AdaptGameWndSize();

            // �������
            fPercent = 100.0f;
            this.m_WinForm.ShowLoadPercent((int)fPercent);
        }
    }

    public void AdaptGameWndSize()
    {
        if (this.m_WinForm == null) return;

        // ������ɺ�����ˢ������Ϸ����λ��,����������������ʾ����,���޷�������ʾ����(��ʵ�Ǵ���λ�ò���)
        System.Drawing.Rectangle rect = this.m_WinForm.GetGameClientRect();
        if (rect.Width <= 0 || rect.Height <= 0 || this.m_LastWinRect == rect) return;

        // �·���Ϣ,������Ϸ�ڲ��ֱ���
        do
        {
            WinAPI.SIZE win_size = new WinAPI.SIZE();
            win_size.cx = rect.Width;
            win_size.cy = rect.Height;
            Server server = Server.GetInstance();
            server.SendDataToGame(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_RESIZE, win_size);
        } while (false);

        // ˢ��Winform��Ϸ����
        WinAPI.ShowWindow(this.m_hNestedWnd, WinAPI.SW_SHOW);
        WinAPI.MoveWindow(this.m_hNestedWnd, rect.X, rect.Y, rect.Width, rect.Height, WinAPI.TRUE);
        
        // �����ϴδ���λ��
        m_LastWinRect.X = rect.X;
        m_LastWinRect.Y = rect.Y;
        m_LastWinRect.Width = rect.Width;
        m_LastWinRect.Height = rect.Height;

        // ��ʱˢ���Ҳ��б�UI
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

        // ������Ϸ������ʼ��
        Server server = Server.GetInstance();
        server.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_START_INFO, Protocol.IPC_FLAG_TO_GAMEPLAZA);

        // ���ó�����ʼ���ֶ�
        DataManager dataMgr = DataManager.GetInstance();
        dataMgr.SceneInited = true;

        // ��Ϸ�������
        this.StartCompleteFinished = true;
    }

    /// <summary> ����Unity���� </summary>
    internal void ActivateNestedWindow()
    {
        if (WinAPI.FALSE == WinAPI.IsWindow(this.m_hNestedWnd)) return;
        WinAPI.SendMessage(this.m_hNestedWnd, (uint)WinAPI.WM_ACTIVATE, WinAPI.PTRUE, IntPtr.Zero);
    }

    /// <summary> ʧȥ����Unity���� </summary>
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

            //�汾����ʾΪ"���汾��.�ΰ汾��.�ڲ��汾��.ר�ò�����"
            strVersions = String.Format("{0}.{1}.{2}.{3}", file.FileMajorPart, file.FileMinorPart, file.FileBuildPart, file.FilePrivatePart);
        }
        catch (System.IO.FileNotFoundException ex)
        {
            //Debug.Assert(false, String.Format("GetVersionInfo Failed [path = {0}, emsg = {1}]", path, ex.Message));
            Debug.Print("GetVersionInfo Failed [path = {0}, emsg = {1}]", path, ex.Message);
        }

        return strVersions;
    }

    /// <summary> ����һ���ػ��߳�(���ڼ���Ƕ�׽���) </summary>
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
        Debug.Print("�ػ��߳�: Start...");
        while (this.m_bDaemonThreadFlag)
        {
            // ����Ϸ���̲����ڻ����˳�
            if (this.m_NestedProcess == null || this.m_NestedProcess.HasExited)
            {
                if (this.m_SyncContext != null && this.m_ExitCallBack != null)
                {
                    this.m_SyncContext.Post(this.m_ExitCallBack, this);
                }

                Debug.Print("�ػ��߳�: Abnormal Exit...");
                this.StopDaemonThread();
                break;
            }

            Thread.Sleep(1000);
        }
        Debug.Print("�ػ��߳�: End...");
    }

    /// <summary> ��Ϸ�����Ƿ���� </summary>
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
        // �ر��ػ��߳�
        this.StopDaemonThread();

        // ֪ͨWinform�رմ���
        if (this.m_WinForm != null)
        {
            WinAPI.ShowWindow(this.m_WinForm.Handle, WinAPI.SW_HIDE);
            this.m_WinForm.CloseForm(true);
        }
    }

    public void KillNestedProcess()
    {
        // �ر��ػ��߳�
        this.StopDaemonThread();

        // �����̴���,��رս���
        if (this.m_NestedProcess != null && !this.m_NestedProcess.HasExited)
        {
            this.m_NestedProcess.Kill();
            this.m_NestedProcess.Close();
            this.m_NestedProcess = null;
        }
    }
}