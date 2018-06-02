using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using System.Diagnostics;
using System.Threading;

namespace UnityWinForm
{
    public partial class UnityWinForm : Form
    {
        public static  Color SiderBarBackColor          = Color.FromArgb(0xFF, 0x4A, 0x6C, 0xA6);   // 右侧栏底色
        private SynchronizationContext m_SyncContext    = null;                                     // UI线程的同步上下文
        private bool m_bImmediatelyExit                 = false;                                    // 是否立即退出

        public UnityWinForm()
        {
            InitializeComponent();

            this.Init();
            this.ShowLoading();
            this.LoadGameWindow();
        }

        void Init()
        {
            // 初始化UI线程同步句柄
            this.m_SyncContext = SynchronizationContext.Current;

            // 绑定窗口大小改变回调
            this.panel_unity_game.Resize += this.NestedWindowResize;

            // 开启双缓冲
            this.SetStyle(ControlStyles.DoubleBuffer | ControlStyles.AllPaintingInWmPaint, true);

            // 固定窗口边框,不允许拉伸
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;

            // 初始化数据模块
            DataManager dataMgr = DataManager.GetInstance();
            dataMgr.Init(this);

            // 显示窗口对应图标
            this.ShowIcon = true;
            Icon tmpIcon = null;
            try
            {
                Resource res = Resource.GetInstance();
                string icon_path = String.Format("{0}icon.ico", res.GameRespath);
                tmpIcon = new Icon(icon_path);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
            finally
            {
                if (tmpIcon == null)
                {
                    System.Reflection.Assembly asm = System.Reflection.Assembly.GetExecutingAssembly();
                    System.IO.Stream stream = asm.GetManifestResourceStream("UnityWinForm.assets.img.icon.icon.ico");
                    if (stream != null)
                    {
                        tmpIcon = new Icon(stream);
                    }
                }
                this.Icon = tmpIcon;
            }
        }

        // 显示初始加载进度界面
        void ShowLoading()
        {
            Resource res = Resource.GetInstance();
            if (Protocol.GameLoadingShow == Define.TRUE)
            {
                Image img = res.LoadImgFromGameRes("loading_bg.jpg");
                if (img == null) img = Resource.GetInstance().LoadImgFromAssembly("ui.loading_bg.jpg");
                this.pictureBox_loading.Image = img;
                this.pictureBox_loading.SizeMode = PictureBoxSizeMode.StretchImage;
                this.progressBar_loading.Value = 0;
            }
            else
            {
                // 隐藏进度条加载面板
                this.panel_loading.Visible = false;

                // 设置游戏窗口背景色
                this.panel_unity_game.BackColor = Color.FromArgb((int)Protocol.GameLoadingColor);
            }
        }

        // 加载"游戏进程"
        void LoadGameWindow()
        {
            Size ClientSize = this.ClientSize;
            if (Protocol.GameConfig.cbOpenUserList == Define.TRUE)
            {
                ClientSize = this.panel_unity_game.ClientSize;
            }

            // 嵌入Unity程序
            string strFileName = String.Format("{0}\\{1}\\{2}", System.Windows.Forms.Application.StartupPath, Protocol.GameFileName, Protocol.GameModelName);
            string strComLine = String.Format("[{0},{1},{2}]", this.Handle, ClientSize.Width, ClientSize.Height);
            NestedWindow nsWnd = NestedWindow.GetInstance();
            nsWnd.Init(this, this.panel_unity_game);
            nsWnd.Start(strFileName, this._OnLoadGameWindowComplete, strComLine, this.m_SyncContext);
        }
        
        // 加载"游戏进程"回调
        void _OnLoadGameWindowComplete(object sender, EventArgs e)
        {
            NestedWindow nsWnd = sender as NestedWindow;
            if (nsWnd == null) nsWnd = NestedWindow.GetInstance();

            // 启动守护线程
            nsWnd.StartDaemonThread(this.OnNestedAbnormalExit);

            // 发送游戏进程ID至大厅
            nsWnd.SendGameProcessInfoToPlaza();

            // 设置游戏窗口句柄
            Protocol.GamesWnd = nsWnd.GetNestedWindow();

            // 设置主窗口标题
#if !GAME_TEST_SWITCH
            this.Text = nsWnd.WindowTitle;
#endif
        }

        // 刷新列表UI
        public void RefreshListUI()
        {
            this.panel_sidebar.Refresh();
        }

        // 异步统计,UI线程显示加载进度
        public void ShowLoadPercent(int iValue)
        {
            // 设置进度条百分比
            iValue = Math.Abs(iValue);
            iValue = iValue > 100 ? 100 : iValue;
            this.progressBar_loading.Value = iValue;

            if (iValue >= 100)
            {
                // 开始游戏(发送初始化场景消息)
                DataManager dataMgr = DataManager.GetInstance();
                dataMgr.StartGameInfo();

                // 延时隐藏Loading界面
                System.Timers.Timer t = new System.Timers.Timer(100);
                t.Enabled = true;
                t.AutoReset = false;
                t.Elapsed += new System.Timers.ElapsedEventHandler((object sender, System.Timers.ElapsedEventArgs ea) =>
                {
                    this.m_SyncContext.Post((object state) =>
                    {
                        t.Enabled = false;
                        this.panel_loading.Visible = false;
                    }, null);
                });
            }
        }

        // 读取exe内嵌资源中的json配置
        public void ReadLocalResStreamToJson(string fileName)
        {
            // 发布模式下,读取嵌入资源中的json配置文件
            System.Reflection.Assembly myAssem = System.Reflection.Assembly.GetExecutingAssembly();
            System.IO.Stream sm = myAssem.GetManifestResourceStream("UnityWinForm.assets.Config.json");
            if (sm != null)
            {
                byte[] cbData = new byte[sm.Length];
                sm.Read(cbData, 0, (int)sm.Length);
                sm.Close();
                string str = Encoding.GetEncoding("GB2312").GetString(cbData, 0, cbData.Length);

                UserList userlist = UserList.GetInstance();
                Newtonsoft.Json.Linq.JObject json = Newtonsoft.Json.Linq.JObject.Parse(str);
                Debug.Print("ReadLocalResStreamToJson - [{0}]", json != null);
            }
        }

        public Rectangle GetGameClientRect()
        {
            return this.panel_unity_game.ClientRectangle;
        }

        public void SetUserNickName(string value)
        {
            this.lab_NickName.Text = value;
        }

        public void SetUserGameID(UInt32 value)
        {
            this.lab_GameID.Text = value.ToString();
        }

        public void SetUserGold(Int64 value)
        {
            this.lab_Gold.Text = Util.NumericStringSplit(value, false);
        }

        public void SetUserHeadPortrait(UInt32 value)
        {
            Resource res = Resource.GetInstance();
            Image img = res.LoadImgFromAssembly(String.Format("common.headPic.{0}.png", value));
            if (img == null)
            {
                img = res.LoadImgFromAssembly(String.Format("common.headPic.{0}.png", 0));
            }

            this.picbox_HeadPortrait.Image = img;
        }

        // 输入文本按键消息
        private void RichtextInput_KeyDown(object sender, KeyEventArgs e)
        {
            // 组合键 CTRL + V
            if (e.Modifiers == Keys.Control && e.KeyCode == Keys.V)
            {
                // 拦截掉此消息
                e.Handled = true;

                string pasteText = Clipboard.GetText();
                if (this.richtext_input.CanPaste(DataFormats.GetFormat(DataFormats.Text)))
                {
                    this.richtext_input.SelectedText = pasteText;
                }
            }
        }

        private void SendMsg_BtnClick(object sender, EventArgs e)
        {
            string text = this.richtext_input.Text;
            if (text.Length <= 0) return;

            this.richtext_input.Text = "";
            this.richtext_input.Focus();

            string strTmp = "抱歉，本游戏房不允许公聊!";
            Protocol.tagGS_Message stu = new Protocol.tagGS_Message(0);
            stu.wMessageType = Define.SMT_EJECT;
            Util.StringToTchar(ref stu.szContent, strTmp);
            stu.wMessageLength = (UInt16)strTmp.Length;

            Server server = Server.GetInstance();
            server.SendDataToGame(Protocol.MDM_GF_FRAME, Protocol.SUB_GF_MESSAGE, stu);
        }

        /// <summary> 嵌套窗口大小改变 </summary>
        public void NestedWindowResize(object sender, EventArgs e)
        {
            // 激活游戏窗口
            NestedWindow nsWnd = NestedWindow.GetInstance();
            nsWnd.ActivateNestedWindow();

            // 刷新游戏窗口
            nsWnd.AdaptGameWndSize();
        }

        /// <summary> 拦截并处理系统消息和自定义消息 </summary>
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WinAPI.WM_PARENTNOTIFY:
                    {
                        UInt16 loworder = WinAPI.LOWORD(m.WParam.ToInt32());
                        switch (loworder)
                        {
                            case WinAPI.WM_CREATE:
                            case WinAPI.WM_DESTROY:
                                {
                                    // 聚焦嵌套的Unity窗口
                                    NestedWindow nsWnd = NestedWindow.GetInstance();
                                    nsWnd.ActivateNestedWindow();
                                } break;
                            case WinAPI.WM_LBUTTONDOWN:
                            case WinAPI.WM_MBUTTONDOWN:
                            case WinAPI.WM_RBUTTONDOWN:
                                {
                                    // 聚焦嵌套的Unity窗口
                                    int x = WinAPI.GET_X_LPARAM(m.LParam.ToInt32());
                                    int y = WinAPI.GET_Y_LPARAM(m.LParam.ToInt32());
                                    Point p = new Point(x, y);
                                    if (this.panel_unity_game.ClientRectangle.Contains(p))
                                    {
                                        // 关闭菜单显示
                                        this.UserMenu_Close();

                                        // 激活游戏窗口
                                        NestedWindow nsWnd = NestedWindow.GetInstance();
                                        nsWnd.ActivateNestedWindow();
                                    }
                                } break;
                            default: break;
                        }
                    } break;
                default:
                    break;
            }

            base.WndProc(ref m);

            // 窗口复原之后发出刷新消息
            switch (m.Msg)
            {
                case WinAPI.WM_SYSCOMMAND:
                    {
                        UInt16 loworder = WinAPI.LOWORD(m.WParam.ToInt32());
                        switch (loworder)
                        {
                            // 最小化
                            case WinAPI.SC_MINIMIZE:
                                { } break;

                            // 最大化
                            case WinAPI.SC_MAXIMIZE:
                                { } break;

                            // 复原
                            case WinAPI.SC_RESTORE:
                                {
                                    // 恢复窗口大小位置时下发游戏，刷新UI
                                    Server server = Server.GetInstance();
                                    server.SendDataToGame(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_REFRESH);
                                } break;
                        }
                    } break;
            }
        }

        // 游戏进程异常退出
        void OnNestedAbnormalExit(object sender)
        {
            Debug.Print("游戏异常关闭");

            // 退出Winform
            NestedWindow nest = NestedWindow.GetInstance();
            nest.KillNestedProcess();
            this.CloseForm(true);
        }

        // 关闭Winform
        public void CloseForm(bool bImmediately)
        {
            this.m_bImmediatelyExit = bImmediately;
            this.Close();
        }

        // 关闭Winform前回调
        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            Debug.Print("关闭Winform操作:{0}", this.m_bImmediatelyExit);

            NestedWindow nest = NestedWindow.GetInstance();
            if (this.m_bImmediatelyExit || !nest.StartCompleteFinished || !nest.IsNestedThreadExist())
            {
                e.Cancel = false;
                if (nest.IsNestedThreadExist())
                {
                    nest.KillNestedProcess();
                }

                // 发送退出游戏消息
                Server server = Server.GetInstance();
                server.SendDataToPlaza(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_CLOSE_FRAME_EX, Protocol.IPC_FLAG_TO_GAMEPLAZA);
            }
            else
            {
                e.Cancel = true;
                DataManager dataMgr = DataManager.GetInstance();
                dataMgr.CloseGame();
            }
        }
        
        private void LabChatClear_MouseHover(object sender, EventArgs e)
        {
            //this.picbox_chat_clear.Image = Resource.GetInstance().LoadImgFromAssembly("common.button.btn_hot.png");
            //int w = this.picbox_chat_clear.Width;
            //int h = this.picbox_chat_clear.Height;

            //Image img = this.picbox_chat_clear.Image;
            //Bitmap bmp = new Bitmap(w, h);
            //Graphics g = Graphics.FromImage(bmp);
            //Util.Scale9Sprite(g, img, new Rectangle(0, 0, w, h), new Rectangle(5, 5, 5, 5));
            //this.picbox_chat_clear.Image = bmp;
            //g.Dispose();
        }

        private void LabChatClear_MouseLeave(object sender, EventArgs e)
        {
            this.picbox_chat_clear.Image = null;
        }

        private void LabChatClear_MouseDown(object sender, MouseEventArgs e)
        {
            //this.picbox_chat_clear.Image = Resource.GetInstance().LoadImgFromAssembly("common.button.btn_push.png");
            //int w = this.picbox_chat_clear.Width;
            //int h = this.picbox_chat_clear.Height;

            //Image img = this.picbox_chat_clear.Image;
            //Bitmap bmp = new Bitmap(w, h);
            //Graphics g = Graphics.FromImage(bmp);
            //Util.Scale9Sprite(g, img, new Rectangle(0, 0, w, h), new Rectangle(5, 5, 5, 5));
            //this.picbox_chat_clear.Image = bmp;
            //g.Dispose();
        }

        private void LabChatClear_MouseUp(object sender, MouseEventArgs e)
        {
            //this.picbox_chat_clear.Image = Resource.GetInstance().LoadImgFromAssembly("common.button.btn_hot.png");
            //int w = this.picbox_chat_clear.Width;
            //int h = this.picbox_chat_clear.Height;

            //Image img = this.picbox_chat_clear.Image;
            //Bitmap bmp = new Bitmap(w, h);
            //Graphics g = Graphics.FromImage(bmp);
            //Util.Scale9Sprite(g, img, new Rectangle(0, 0, w, h), new Rectangle(5, 5, 5, 5));
            //this.picbox_chat_clear.Image = bmp;
            //g.Dispose();
        }

        private void LabChatClear_MouseClick(object sender, MouseEventArgs e)
        {
            this.richtext_outview.Clear();
        }
    }
}
