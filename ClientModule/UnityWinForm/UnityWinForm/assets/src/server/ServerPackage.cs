using System;

public class ServerPackage
{
    public ServerPackage()
    { }

    ~ServerPackage()
    { }

    // IPC版本
    public UInt16 Version   { set; get; }

    // 包体数据大小
    public UInt16 DataSize  { set; get; }

    // 主ID
    public UInt16 CmdMainID { set; get; }

    // 子ID
    public UInt16 CmdSubID  { set; get; }
        
    // 数据缓冲
    public byte[] Buffer    { set; get; }
}
