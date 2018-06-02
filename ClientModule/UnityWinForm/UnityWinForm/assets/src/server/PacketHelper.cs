using System;
using System.Diagnostics;
using System.Collections;
using System.Runtime.InteropServices;

//数据描述头
public struct tagDataDescribe
{
    public ushort wDataSize;						//数据大小
    public ushort wDataDescribe;					//数据描述
};

//数据包辅助类
unsafe public class SendPacketHelper
{
    protected ushort m_wDataSize;					//数据大小
    protected ushort m_wBufferSize;					//缓冲大小
    protected byte* m_pcbBuffer;					//缓冲指针

    //构造函数
    unsafe public SendPacketHelper(byte[] cbData, ref byte[] cbOutData)
    {
        //效验大小
        Debug.Assert(cbData.Length <= cbOutData.Length);

        // 如果溢出则最多只会拷贝cbBuffer.Length大小的数据
        int iMinSize = Math.Min(cbData.Length, cbOutData.Length);
        Buffer.BlockCopy(cbData, 0, cbOutData, 0, iMinSize);

        fixed (byte* pcbBuffer = cbOutData)
        {
            this._Init(pcbBuffer + iMinSize, (ushort)(cbOutData.Length - iMinSize));
        }
    }

    //析构函数
    ~SendPacketHelper()
    {
        // 这里是清理非托管资源的用户代码段
    }
    
    //清理数据
    public void CleanData() { m_wDataSize = 0; }

    //获取大小
    public ushort GetDataSize() { return m_wDataSize; }

    //获取缓冲
    unsafe public void* GetDataBuffer() { return m_pcbBuffer; }

    //插入数据
    unsafe public bool AddPacket(byte[] cbData, ushort wDataType)
    {
        bool bRes = false;
        fixed (byte* pcbBuffer = cbData)
        {
            _AddPacket(pcbBuffer, (ushort)cbData.Length, wDataType);
        }
        return bRes;
    }

    //初始数据
    unsafe private void _Init(void* pcbBuffer, ushort wBufferSize)
    {
        m_wDataSize = 0;
        m_wBufferSize = wBufferSize;
        m_pcbBuffer = (byte*)pcbBuffer;
    }
    
    //插入数据
    unsafe private bool _AddPacket(void* pData, ushort wDataSize, ushort wDataType)
    {
        //效验大小
        uint iDescribeSize = (uint)Marshal.SizeOf(typeof(tagDataDescribe));
        Debug.Assert((wDataSize + iDescribeSize + m_wDataSize) <= m_wBufferSize);
        if ((wDataSize + iDescribeSize + m_wDataSize) > m_wBufferSize) return false;

        //插入数据
        Debug.Assert(m_pcbBuffer != null);
        tagDataDescribe* pDataDescribe = (tagDataDescribe*)(m_pcbBuffer + m_wDataSize);
        pDataDescribe->wDataSize = wDataSize;
        pDataDescribe->wDataDescribe = wDataType;

        //插入数据
        if (wDataSize > 0)
        {
            Debug.Assert(pData != null);
            Util.CopyMemory(pDataDescribe + 1, pData, wDataSize);
        }

        //设置数据
        m_wDataSize += (ushort)(iDescribeSize + wDataSize);

        return true;
    }
};

//数据包辅助类
unsafe public class RecvPacketHelper
{
    protected ushort    m_wDataPos  = 0;        // 数据点
    protected ushort    m_wDataSize = 0;        // 数据大小
    protected byte*     m_pcbBuffer = null;     // 缓冲指针
    public const int    DTP_NULL    = 0;        // 无效数据

    //构造函数
    unsafe public RecvPacketHelper(byte[] cbData, int wDataSize, int wOffset)
    {
        Debug.Assert(wDataSize < cbData.Length && wDataSize >= wOffset);
        fixed (byte* pcbData = cbData)
        {
            _Init(pcbData + wOffset, (ushort)(wDataSize - wOffset));
        }
    }

    //析构函数
    ~RecvPacketHelper()
    {
        // 这里是清理非托管资源的用户代码段
    }

    //获取数据
    unsafe public bool GetData(byte[] cbOutData, ref tagDataDescribe DataDescribe)
    {
        Array.Clear(cbOutData, 0, cbOutData.Length);
        void* pGetData = this._GetData(ref DataDescribe);
        if (DataDescribe.wDataDescribe == RecvPacketHelper.DTP_NULL)
        {
            cbOutData = null;
            return false;
        }
        else
        {
            // 考虑到字符串'\0',所以长度应该 + 1
            if (cbOutData.Length < DataDescribe.wDataSize + 1)
            {
                Debug.Print("RecvPacketHelper GetData Error! Recv_Size Too Big(total_size={0}, real_size={1})", cbOutData.Length, DataDescribe.wDataSize);
                return false;
            }

            // 拷贝数据
            fixed (byte* pOutData = cbOutData)
            {
                Util.CopyMemory(pOutData, pGetData, (uint)DataDescribe.wDataSize);
            }
        }

        return true;
    }

    //初始数据
    unsafe private void _Init(void* pcbBuffer, ushort wDataSize)
    {
        m_wDataPos = 0;
        m_wDataSize = wDataSize;
        m_pcbBuffer = (byte*)pcbBuffer;
    }

    //获取数据
    unsafe private void* _GetData(ref tagDataDescribe DataDescribe)
    {
        //效验数据
        if (m_wDataPos >= m_wDataSize)
        {
            Debug.Assert(m_wDataPos == m_wDataSize);
            DataDescribe.wDataSize = 0;
            DataDescribe.wDataDescribe = RecvPacketHelper.DTP_NULL;
            return null;
        }

        //获取数据
        uint iDescribeSize = (uint)Marshal.SizeOf(typeof(tagDataDescribe));
        Debug.Assert((m_wDataPos + iDescribeSize) <= m_wDataSize);
        fixed (void* pDataDescribe = &DataDescribe)
        {
            Util.CopyMemory(pDataDescribe, m_pcbBuffer + m_wDataPos, iDescribeSize);
        }
        Debug.Assert((m_wDataPos + iDescribeSize+ DataDescribe.wDataSize) <= m_wDataSize);

        //效验数据
        if ((m_wDataPos + iDescribeSize + DataDescribe.wDataSize) > m_wDataSize)
        {
            DataDescribe.wDataSize = 0;
            DataDescribe.wDataDescribe = RecvPacketHelper.DTP_NULL;
            return null;
        }

        //设置数据
        void* pData = null;
        if (DataDescribe.wDataSize > 0) pData = m_pcbBuffer + m_wDataPos + iDescribeSize;
        m_wDataPos += (ushort)(iDescribeSize + DataDescribe.wDataSize);
        return pData;
    }
};
