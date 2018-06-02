using System;
using System.Runtime.InteropServices;

public class WinAPI
{
    public const int TRUE               = 1;
    public const int FALSE              = 0;
    public static IntPtr PTRUE          = new IntPtr(1);
    public static IntPtr PFALSE         = new IntPtr(0);

    // CreateWindow 窗口风格
    public const int WS_MAXIMIZEBOX     = 0x00010000;       // 最大化按钮风格
    public const int WS_CHILD           = 0x40000000;       // 创建一个子窗口,这个风格不能与WS_POPUP风格合用
    public const int WS_VISIBLE         = 0x10000000;       // 创建一个初始状态为可见的窗口
    public const int WS_POPUP           = int.MinValue;

    // CreateWindowEx 扩展风格的层叠式窗口、弹出式窗口或子窗口
    public const int WS_EX_MDICHILD     = 0x40;             // 创建一个MDI子窗口

    // SetWindowLong 改变指定窗口的属性 
    public const int GWL_STYLE          = (-16);            // 设定一个新的窗口风格

    // ShowWindow 窗口显示样式
    public const int SW_HIDE            = 0;                // 隐藏,并且任务栏也没有最小化图标
    public const int SW_SHOWNORMAL      = 1;                // 用最近的大小和位置显示,激活
    public const int SW_NORMAL          = 1;                // 同 SW_SHOWNORMAL
    public const int SW_SHOWMINIMIZED   = 2;                // 最小化,激活
    public const int SW_SHOWMAXIMIZED   = 3;                // 最大化,激活
    public const int SW_MAXIMIZE        = 3;                // 同 SW_SHOWMAXIMIZED
    public const int SW_SHOWNOACTIVATE  = 4;                // 用最近的大小和位置显示,不激活
    public const int SW_SHOW            = 5;                // 同 SW_SHOWNORMAL
    public const int SW_MINIMIZE        = 6;                // 最小化,不激活
    public const int SW_SHOWMINNOACTIVE = 7;                // 同 SW_MINIMIZE
    public const int SW_SHOWNA          = 8;                // 同 SW_SHOWNOACTIVATE
    public const int SW_RESTORE         = 9;                // 同 SW_SHOWNORMAL
    public const int SW_SHOWDEFAULT     = 10;               // 同 SW_SHOWNORMAL
    public const int SW_MAX             = 10;               // 同 SW_SHOWNORMAL

    // SetWindowPos 改变一个子窗口，弹出式窗口或顶层窗口的尺寸，位置和Z序
    public const int SWP_SHOWWINDOW     = 0x0040;           // 显示窗口
    public const int SWP_FRAMECHANGED   = 0x20;             // 给窗口发送WM_NCCALCSIZE消息，即使窗口尺寸没有改变也会发送该消息
    public const int SWP_ASYNCWINDOWPOS = 0x4000;           // 如果调用进程不拥有窗口，系统会向拥有窗口的线程发出需求。这就防止调用线程在其他线程处理需求的时候发生死锁
    public const int SWP_NOOWNERZORDER  = 0x200;            // 不改变z序中的所有者窗口的位置
    public const int SWP_NOREDRAW       = 0x8;              // 不重画变化(如果设置了这个标志，则不发生任何种类的变化)
    public const int SWP_NOZORDER       = 0x4;              // 保持当前的次序(忽略pWndInsertAfter)
    public const int SWP_NOACTIVATE     = 0x10;             // 不激活窗口
    public const int SWP_NOMOVE         = 0x2;              // 维持当前位置(忽略X和Y参数)
    public const int SWP_NOSIZE         = 0x1;              // 维持当前尺寸(忽略cx和Cy参数)

    // SendMessage消息格式
    public const int WM_NULL            = 0x0000;           // 空消息，此消息将被接收窗口忽略
    public const int WM_CREATE          = 0x0001;           // 应用程序创建一个窗口
    public const int WM_DESTROY         = 0x0002;           // 一个窗口被销毁
    public const int WM_SETTEXT         = 0x000C;           // 设置窗体的标题
    public const int WM_COPYDATA        = 0x004A;           // 本机进程通信
    public const int WM_SYSCOMMAND      = 0x0112;           // 窗口系统消息
    public const int WM_NCLBUTTONDOWN   = 0x00A1;           // 窗口鼠标左键点击
    public const int WM_CLOSE           = 0x0010;           // 关闭窗口
    public const int WM_ACTIVATE        = 0x0006;           // 一个窗口被激活或失去激活状态
    public const int WM_MOUSEMOVE       = 0x0200;           // 鼠标移动
    public const int WM_LBUTTONDOWN     = 0x0201;           // 鼠标左键按下
    public const int WM_LBUTTONUP       = 0x0202;           // 鼠标左键弹起
    public const int WM_LBUTTONDBLCLK   = 0x0203;           // 鼠标左键双击
    public const int WM_RBUTTONDOWN     = 0x0204;           // 鼠标右键按下
    public const int WM_RBUTTONUP       = 0x0205;           // 鼠标右键弹起
    public const int WM_RBUTTONDBLCLK   = 0x0206;           // 鼠标右键双击
    public const int WM_MBUTTONDOWN     = 0x0207;           // 鼠标中键按下
    public const int WM_MBUTTONUP       = 0x0208;           // 鼠标中键弹起
    public const int WM_MBUTTONDBLCLK   = 0x0209;           // 鼠标中键双击
    public const int WM_MOUSEWHEEL      = 0x020A;           // 当鼠标滚轮转动时发送此消息给当前获得焦点的窗口
    public const int WM_PARENTNOTIFY    = 0x0210;           // 当MDI子窗口被创建或被销毁,或当光标位于子窗口上
                                                            // 且用户按了一下鼠标键时，发送此消息给它的父窗口
    // WM_SYSCOMMAND 子消息类型
    public const int SC_MINIMIZE        = 0xF020;           // 点击最小化
    public const int SC_MAXIMIZE        = 0xF030;           // 点击最大化
    public const int SC_RESTORE         = 0xF120;           // 点击复原

    // SetWindowsHookEx 钩子类型
    public const int WH_GETMESSAGE      = 3;                // 钩子类型(监视从GetMessage / PeekMessage函数返回的消息)
    public const int WH_CALLWNDPROC     = 4;                // 钩子类型(系统在消息发送到接收窗口过程之前调用此子程)
        
    // 建立钩子
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 SetWindowsHookEx(int idHook, HOOKPROC lpfn, IntPtr hMod, UInt32 dwThreadId);

    // 移除钩子
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int UnhookWindowsHookEx(IntPtr hhk);

    // 是否是窗口句柄
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int IsWindow(IntPtr hWnd);

    // 查找窗口
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    // Window MessageBox
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int MessageBox(IntPtr hWnd, string lpText, string lpCaption, uint uType);

    // 枚举窗口
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int EnumWindows(WNDENUMPROC lpEnumFunc, IntPtr lParam);

    // 获取窗口所在线程ID
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern UInt32 GetWindowThreadProcessId(IntPtr hWnd, ref IntPtr lpdwProcessId);

    // 将钩子信息传递到当前钩子链中的下一个子程
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 CallNextHookEx(IntPtr idHook, int nCode, IntPtr wParam, IntPtr lParam);

    // 获取当前窗口的父窗口
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr GetParent(IntPtr hWnd);

    // 将指定的消息发送到一个或多个窗口
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

    // 将指定的消息发送到一个或多个窗口
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, string lParam);

    // 获取一个前台窗口的句柄
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr GetForegroundWindow();

    // 改变指定窗口的属性
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 SetWindowLong(IntPtr hWnd, int nIndex, Int32 dwNewLong);

    // 改变指定窗口的属性32位
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "SetWindowLong", CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr SetWindowLongPtr32(HandleRef hWnd, int nIndex, int dwNewLong);

    // 改变指定窗口的属性64位
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, EntryPoint = "SetWindowLongPtr", CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr SetWindowLongPtr64(HandleRef hWnd, int nIndex, int dwNewLong);

    // 改变指定窗口的位置和大小
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, int bRepaint);

    // 向指定的窗体更新区域添加一个矩形，然后窗口客户区域的这一部分将被重新绘制
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int InvalidateRect(IntPtr hWnd, ref RECT lpRect, int bErase);

    // 更新指定窗口的客户区
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int UpdateWindow(IntPtr hWnd);

    // 设置指定窗口的显示状态
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int ShowWindow(IntPtr hWnd, int nCmdShow);

    // 获得指定窗口的有关信息，函数也获得在额外窗口内存中指定偏移位地址的32位度整型值
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 GetWindowLong(IntPtr hWnd, int nIndex);

    // 返回指定窗口的边框矩形的尺寸
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 GetWindowRect(IntPtr hWnd, out RECT lpRect);

    // 改变指定窗口的标题栏的文本内容
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 SetWindowText(IntPtr hWnd, string lpString);

    // 获取指定窗口的标题栏的文本内容
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 GetWindowText(IntPtr hWnd, System.Text.StringBuilder lpString, int nMaxCount);

    // 设置弹出式窗口，层叠窗口或子窗口的父窗口。新的窗口与窗口必须属于同一应用程序
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndParent);

    // 用于枚举一个父窗口的所有子窗口
    [DllImport("user32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int EnumChildWindows(IntPtr hWndParent, WNDENUMPROC lpEnumFunc, IntPtr lParam);

    // 获取一个应用程序或动态链接库的模块句柄
    [DllImport("kernel32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern IntPtr GetModuleHandle(string lpModuleName);

    // 返回上一次的错误结果(0则表示无错误)
    [DllImport("kernel32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern UInt32 GetLastError();

    // 写入.ini文件(返回0表示失败，非0为成功)
    [DllImport("kernel32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern Int32 WritePrivateProfileString(string lpAppName,string lpKeyName,string lpString,string lpFileName);

    // 读取.ini文件(返回取得字符串缓冲区的长度)
    [DllImport("kernel32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto, SetLastError = true)]
    public static extern UInt32 GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, System.Text.StringBuilder lpReturnedString, UInt32 nSize, string lpFileName);
    
    // 钩子委托
    public delegate Int32 HOOKPROC(int code, IntPtr wParam, IntPtr lParam);

    // 枚举窗口委托
    public delegate int WNDENUMPROC(IntPtr hwnd, IntPtr lParam);

    // 从指定的32位值, 取出低8位
    public static byte LOBYTE(int w)
    {
        return ((byte)(((UInt32)(w)) & 0xff));
    }

    // 从指定的32位值, 取出高8位
    public static byte HIBYTE(int w)
    {
        return ((byte)((((UInt32)(w)) >> 8) & 0xff));
    }

    // 从指定的32位值, 取出低16位
    public static UInt16 LOWORD(int _dw)
    {
        return ((UInt16)(((UInt32)(_dw)) & 0xffff));
    }

    // 从指定的32位值, 取出高16位
    public static UInt16 HIWORD(int _dw)
    {
        return ((UInt16)((((UInt32)(_dw)) >> 16) & 0xffff));
    }

    // 从指定的64位值, 取出低32位
    public static UInt32 LODWORD(Int64 _qw)
    {
        return ((UInt32)(_qw));
    }

    // 从指定的64位值, 取出高32位
    public static UInt32 HIDWORD(Int64 _qw)
    {
        return ((UInt32)(((_qw) >> 32) & 0xffffffff));
    }

    // 将两个8位的数联合成一个的16位的UInt16型的数
    public static UInt16 MAKEWORD(int a, int b)
    {
        return ((UInt16)(((byte)(((UInt32)(a)) & 0xff)) | ((UInt16)((byte)(((UInt32)(b)) & 0xff))) << 8));
    }

    // 将两个16位的数联合成一个的32位的Int32型的数
    public static Int32 MAKELONG(int a, int b)
    {
        return ((Int32)(((UInt16)(((UInt32)(a)) & 0xffff)) | ((UInt32)((UInt16)(((UInt32)(b)) & 0xffff))) << 16));
    }

    // 将32位的Int32型的数分解成两个16位的数
    public static void PARSELONG(int uValue, ref UInt16 wLow, ref UInt16 wHigh)
    {
        wLow = (UInt16)((UInt32)uValue & 0x0000FFFF);
        wHigh = (UInt16)(((UInt32)uValue & 0xFFFF0000) >> 16);
    }

    /// <summary> 获取鼠标x坐标 </summary>
    public static int GET_X_LPARAM(int lp)
    {
        return ((int)(short)WinAPI.LOWORD(lp));
    }

    /// <summary> 获取鼠标y坐标 </summary>
    public static int GET_Y_LPARAM(int lp)
    {
        return ((int)(short)WinAPI.HIWORD(lp));
    }

    /// <summary> 改变指定窗口的属性 </summary>
    public static IntPtr SetWindowLong(HandleRef hWnd, int nIndex, int dwNewLong)
    {
        if (IntPtr.Size == 4)
        {
            return SetWindowLongPtr32(hWnd, nIndex, dwNewLong);
        }
        return SetWindowLongPtr64(hWnd, nIndex, dwNewLong);
    }

    /// <summary> 通过进程ID枚举获取对应窗口 </summary>
    public static IntPtr GetWindowByPid(int ipid)
    {
        IntPtr hWnd = IntPtr.Zero;
        IntPtr iTagPid = new IntPtr(ipid);
        WinAPI.EnumWindows(new WinAPI.WNDENUMPROC(delegate(IntPtr hwnd, IntPtr lParam)
        {
            IntPtr pid = IntPtr.Zero;
            WinAPI.GetWindowThreadProcessId(hwnd, ref pid);
            if (pid == lParam)
            {
                hWnd = hwnd;
                return WinAPI.FALSE;        // 返回 0 终止枚举
            }
            return WinAPI.TRUE;             // 返回 1 继续枚举
        }), iTagPid);

        return hWnd;
    }

    /// <summary> 通过进程ID枚举获取对应子窗口 </summary>
    public static IntPtr GetChildWindowByPid(IntPtr hParentWnd, int ipid)
    {
        IntPtr hWnd = IntPtr.Zero;
        IntPtr iTagPid = new IntPtr(ipid);

        if (WinAPI.TRUE == WinAPI.IsWindow(hParentWnd))
        {
            WinAPI.EnumChildWindows(hParentWnd, new WinAPI.WNDENUMPROC(delegate(IntPtr hwnd, IntPtr lParam)
            {
                IntPtr pid = IntPtr.Zero;
                WinAPI.GetWindowThreadProcessId(hwnd, ref pid);
                if (pid == lParam)
                {
                    hWnd = hwnd;
                    return WinAPI.FALSE;        // 返回 0 终止枚举
                }
                return WinAPI.TRUE;             // 返回 1 继续枚举
            }), iTagPid);
        }

        return hWnd;
    }

    /// <summary> 窗口大小 </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct SIZE
    {
        public Int32    cx;
        public Int32    cy;
    }

    /// <summary> 矩形 </summary>
    public struct RECT
    {
        public RECT(Int32 left, Int32 top, Int32 right, Int32 bottom)
        {
            this.left   = left;
            this.top    = top;
            this.right  = right;
            this.bottom = bottom;
        }

        public Int32    left;
        public Int32    top;
        public Int32    right;
        public Int32    bottom;
    }

    /// <summary> WM_COPYDATA结构 </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct COPYDATASTRUCT
    {
        public IntPtr   dwData;                                             // 保存一个数值, 可以用来作标志等
        public int      cbData;                                             // 待发送的数据的长度
        public IntPtr   lpData;                                             // 待发送的数据的起始地址(可以为NULL)
    }

    /// <summary> 钩子接收消息的结构 </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct CWPSTRUCT
    {
        public IntPtr   lParam;
        public IntPtr   wParam;
        public uint     message;
        public IntPtr   hwnd;
    }

    /// <summary> 枚举窗口结构体 </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct WNDINFO
    {
        public UInt32   dwProcessID;
        public IntPtr   hWnd;
    }

    /// <summary> 进程信息结构体 </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct PROCESS_INFORMATION
    {
        public Int32    hProcess;
        public Int32    hThread;
        public UInt32   dwProcessId;
        public UInt32   dwThreadId;
    }
}
