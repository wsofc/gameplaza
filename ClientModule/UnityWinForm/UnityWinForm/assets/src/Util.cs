using System;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Drawing;

public class Util
{
    private static StringBuilder StrBuffer = new StringBuilder(Define.MAX_PATH);

    /// <summary> byte数组转为结构体 </summary>
    public static object BytesToStruct<T>(byte[] bytes)
    {
        int size = Marshal.SizeOf(typeof(T));
        if (size > bytes.Length)
        {
            return null;
        }

        // 分配结构体内存空间
        IntPtr structPtr = Marshal.AllocHGlobal(size);

        // 将byte数组拷贝到分配好的内存空间
        Marshal.Copy(bytes, 0, structPtr, size);

        // 将内存空间转换为目标结构体
        object obj = Marshal.PtrToStructure(structPtr, typeof(T));

        // 释放内存空间
        Marshal.FreeHGlobal(structPtr);

        return obj;
    }

    /// <summary> 结构体转为byte数组 </summary>
    public static byte[] StructToBytes(object obj)
    {
        // 得到结构体的大小
        int size = Marshal.SizeOf(obj);

        // 创建byte数组
        byte[] bytes = new byte[size];

        // 分配结构体大小的内存空间
        IntPtr structPtr = Marshal.AllocHGlobal(size);

        // 将结构体拷到分配好的内存空间
        Marshal.StructureToPtr(obj, structPtr, false);

        // 从内存空间拷到byte数组
        Marshal.Copy(structPtr, bytes, 0, size);

        // 释放内存空间
        Marshal.FreeHGlobal(structPtr);

        // 返回byte数组
        return bytes;
    }

    /// <summary> byte数组转结构体数组 </summary>
    public static ArrayList BytesToStructArray<T>(byte[] bytes, ushort wSize)
    {
        ArrayList array = new ArrayList();
        if (wSize <= bytes.Length)
        {
            int tSize = Marshal.SizeOf(typeof(T));
            int iCount = wSize / tSize;

            byte[] tmpBuffer = new byte[tSize];
            int hSize = sizeof(byte) * tmpBuffer.Length;
            IntPtr pData = Marshal.AllocHGlobal(hSize);
            for (int i = 0; i < iCount; i++)
            {
                Array.Clear(tmpBuffer, 0, tmpBuffer.Length);
                Array.Copy(bytes, i * tSize, tmpBuffer, 0, tmpBuffer.Length);
                Marshal.Copy(tmpBuffer, 0, pData, hSize);

                object obj = Marshal.PtrToStructure((IntPtr)pData, typeof(T));
                if (obj != null)
                {
                    T t = (T)Marshal.PtrToStructure((IntPtr)pData, typeof(T));
                    array.Add(t);
                }
            }
            Marshal.FreeHGlobal(pData);
        }
        else
        {
            string sreError = String.Format("BytesToStructArray Failed [bytes_len = {0}, size = {1}]", bytes.Length, wSize);
            Debug.Assert(false, sreError);
        }

        return array;
    }

    /// <summary> C# String 转 C++ TCHAR 数组 </summary>
    public static void StringToTchar(ref byte[] cbOut, string strIn)
    {
        // 清空
        Array.Clear(cbOut, 0, cbOut.Length);

        // 转换
        byte[] cbTmp = System.Text.Encoding.Default.GetBytes(strIn);
        int iMinLen = Math.Min(cbOut.Length, cbTmp.Length);

        // 拷贝
        Buffer.BlockCopy(cbTmp, 0, cbOut, 0, iMinLen * sizeof(byte));
    }

    /// <summary> C++ TCHAR 数组 转 C# String </summary>
    public static string TcharToString(byte[] cbTchar)
    {
        int count = cbTchar.Length - 1;
        for (int i = 0; i < cbTchar.Length; ++i)
        {
            if (cbTchar[i] == '\0')
            {
                count = i;
                break;
            }
        }

        string str = Encoding.GetEncoding("GB2312").GetString(cbTchar, 0, count);//.TrimEnd('\0');
        return str;
    }

    /// <summary> 排列(阶乘 循环算法) </summary>  
    static public Int64 A1(int N, int R)
    {
        if (R > N || R <= 0 || N <= 0) throw new ArgumentException("params invalid!");
        Int64 t = 1;
        int i = N;

        while (i != N - R)
        {
            try
            {
                checked
                {
                    t *= i;
                }
            }
            catch
            {
                throw new OverflowException("overflow happens!");
            }
            --i;
        }
        return t;
    }

    /// <summary> 排列(阶乘 堆栈算法) </summary>  
    static public Int64 A2(int N, int R)
    {
        if (R > N || R <= 0 || N <= 0) throw new ArgumentException("arguments invalid!");
        System.Collections.Generic.Stack<int> s = new System.Collections.Generic.Stack<int>();
        Int64 iRlt = 1;
        int t = 0;
        s.Push(N);
        while ((t = s.Peek()) != N - R)
        {
            try
            {
                checked
                {
                    iRlt *= t;
                }
            }
            catch
            {
                throw new OverflowException("overflow happens!");
            }
            s.Pop();
            s.Push(t - 1);
        }
        return iRlt;
    }

    /// <summary> 组合 </summary>  
    static public Int64 C(int N, int R)
    {
        return A1(N, R) / A2(R, R);
    }

    /// <summary> 泛型冒泡排序 </summary>
    public delegate bool Contrast<T>(T t1, T t2);
    static public void BubbleSort<T>(T[] R, Contrast<T> contrast)
    {
        //交换标志
        T temp;
        bool exchange = false;

        for (int i = 0; i < R.Length; i++) //最多做R.Length-1趟排序 
        {
            exchange = false; //本趟排序开始前，交换标志应为假
            for (int j = R.Length - 2; j >= i; j--)
            {
                if (contrast(R[j + 1], R[j])) //交换条件
                //if (R[j + 1] < R[j]) 
                {
                    temp = R[j + 1];
                    R[j + 1] = R[j];
                    R[j] = temp;
                    exchange = true; //发生了交换，故将交换标志置为真 
                }
            }
            if (!exchange) //本趟排序未发生交换，提前终止算法 
            {
                break;
            }
        }
    }

    /// <summary> 标准仿C memcpy函数 </summary>
    unsafe public static void* CopyMemory(void* dst, void* src, uint count)
    {
        System.Diagnostics.Debug.Assert(dst != null);
        System.Diagnostics.Debug.Assert(src != null);

        void* ret = dst;

        /*
         * copy from lower addresses to higher addresses
         */
        while (count-- > 0)
        {
            *(char*)dst = *(char*)src;
            dst = (char*)dst + 1;
            src = (char*)src + 1;
        }

        return (ret);
    }

    /// <summary> 标准仿C memmove函数 </summary>  
    unsafe public static void* MyMemMove(void* dst, void* src, uint count)
    {
        System.Diagnostics.Debug.Assert(dst != null);
        System.Diagnostics.Debug.Assert(src != null);

        void* ret = dst;

        if (dst <= src || (char*)dst >= ((char*)src + count))
        {
            while (count-- > 0)
            {
                *(char*)dst = *(char*)src;
                dst = (char*)dst + 1;
                src = (char*)src + 1;
            }
        }
        else
        {
            dst = (char*)dst + count - 1;
            src = (char*)src + count - 1;
            while (count-- > 0)
            {
                *(char*)dst = *(char*)src;
                dst = (char*)dst - 1;
                src = (char*)src - 1;
            }
        }

        return (ret);
    }

    /// <summary> 标准仿C memset函数 </summary>
    unsafe public static void* memset(void* s, int c, uint n)
    {
        byte* p = (byte*)s;

        while (n > 0)
        {
            *p++ = (byte)c;
            --n;
        }

        return s;
    }

    /// <summary> 字符串转换MD5串(不可逆) </summary>  
    public static string MD5EncryptData(string input, bool bCapital = false)
    {
        // Use input string to calculate MD5 hash
        String hex_format = bCapital ? "X2" : "x2";
        System.Security.Cryptography.MD5 md5 = System.Security.Cryptography.MD5.Create();
        byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
        byte[] hashBytes = md5.ComputeHash(inputBytes);

        // Convert the byte array to hexadecimal string
        System.Text.StringBuilder sb = new System.Text.StringBuilder();
        for (int i = 0; i < hashBytes.Length; i++)
        {
            sb.Append(hashBytes[i].ToString(hex_format));
        }
        return sb.ToString();
    }
    
    /// <summary> 数字字符串按指定符号分隔 </summary>
    public static string NumericStringSplit(Int64 llValue, bool bShowPlusSign = true, char cSign = ',', int iOffset = 4)
    {
        StrBuffer.Length = 0;
        StrBuffer.AppendFormat("{0:}", Math.Abs(llValue));

        int j = 0;
        int iCount = StrBuffer.Length - 1;
        for (int i = iCount; i > 0; --i)
        {
            if (++j % iOffset == 0)
            {
                j = 0;
                StrBuffer.Insert(i, cSign);
            }
        }

        if (bShowPlusSign)
        {
            if (llValue > 0)
            {
                StrBuffer.Insert(0, '+');
            }
            else if (llValue < 0)
            {
                StrBuffer.Insert(0, '-');
            }
        }

        return StrBuffer.ToString();
    }

    /// <summary> 九宫格绘图 /// </summary>
    public static void Scale9Sprite(System.Drawing.Graphics g, System.Drawing.Image img, System.Drawing.Rectangle destRect, System.Drawing.Rectangle insetRect)
    {
        if (img == null) return;

        //// 填充四角
        // 左上角
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.X, destRect.Y, insetRect.X, insetRect.Y)
            , new System.Drawing.Rectangle(0, 0, insetRect.X, insetRect.Y), System.Drawing.GraphicsUnit.Pixel);

        // 右上角
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.Right - insetRect.Width, destRect.Y, insetRect.Width, insetRect.Y)
            , new System.Drawing.Rectangle(img.Width - insetRect.Width, 0, insetRect.Width, insetRect.Y), System.Drawing.GraphicsUnit.Pixel);

        // 左下角
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.X, destRect.Bottom - insetRect.Height, insetRect.X, insetRect.Height)
            , new System.Drawing.Rectangle(0, img.Height - insetRect.Height, insetRect.X, insetRect.Height), System.Drawing.GraphicsUnit.Pixel);

        // 右下角
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.Right - insetRect.Width, destRect.Bottom - insetRect.Height, insetRect.Width, insetRect.Height)
            , new System.Drawing.Rectangle(img.Width - insetRect.Width, img.Height - insetRect.Height, insetRect.Width, insetRect.Height), System.Drawing.GraphicsUnit.Pixel);

        // 填充四边
        // 左
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.X, destRect.Y + insetRect.Y, insetRect.X, destRect.Height - (insetRect.Y + insetRect.Height))
            , new System.Drawing.Rectangle(0, insetRect.Y, insetRect.X, img.Height - (insetRect.Y + insetRect.Height)), System.Drawing.GraphicsUnit.Pixel);

        // 上
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.X + insetRect.X, destRect.Y, destRect.Width - (insetRect.X + insetRect.Width), insetRect.Y)
            , new System.Drawing.Rectangle(insetRect.X, 0, img.Width - (insetRect.X + insetRect.Width), insetRect.Y), System.Drawing.GraphicsUnit.Pixel);

        // 右
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.Right - insetRect.Right, destRect.Y + insetRect.Y, insetRect.Right, destRect.Height - (insetRect.Y + insetRect.Height))
            , new System.Drawing.Rectangle(img.Width - insetRect.Right, insetRect.Y, insetRect.Right, img.Height - (insetRect.Y + insetRect.Height)), System.Drawing.GraphicsUnit.Pixel);

        // 下
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.X + insetRect.X, destRect.Bottom - insetRect.Height, destRect.Width - (insetRect.X + insetRect.Width), insetRect.Height)
            , new System.Drawing.Rectangle(insetRect.X, img.Height - insetRect.Height, img.Width - (insetRect.X + insetRect.Width), insetRect.Height), System.Drawing.GraphicsUnit.Pixel);

        // 填充中间
        g.DrawImage(img, new System.Drawing.Rectangle(destRect.X + insetRect.X, destRect.Y + insetRect.Y, destRect.Width - (insetRect.X + insetRect.Width), destRect.Height - (insetRect.Y + insetRect.Height))
            , new System.Drawing.Rectangle(insetRect.X, insetRect.Y, img.Width - (insetRect.X + insetRect.Width), img.Height - (insetRect.Y + insetRect.Height)), System.Drawing.GraphicsUnit.Pixel);
    }
}
