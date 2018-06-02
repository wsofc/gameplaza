using System;

public class UserModel
{
    public UserModel()
    {
        this.TableID = Define.INVALID_TABLE;
        this.ChairID = Define.INVALID_CHAIR;
    }

    ~UserModel()
    { }

    private byte m_cbMemberOrder    = 0;            // 会员等级
    private UInt32 m_dwMemberColor  = 0xFFFFFFFF;   // 对应会员列表字体颜色

    /// <summary> 是否是自己本身 </summary>
    public bool IsSelf { set; get; }

    /// <summary> 是否是机器人 </summary>
    public bool IsRobot { set; get; }

    /// <summary> 用户ID </summary>
    public UInt32 UserID { set; get; }

    /// <summary> 用户显示的游戏ID </summary>
    public UInt32 GameID { set; get; }

    /// <summary> 用户头像ID </summary>
    public UInt32 FaceID { set; get; }

    /// <summary> 用户会员等级 </summary>
    public byte MemberOrder
    {
        set
        {
            this.m_cbMemberOrder = value;
            switch (this.m_cbMemberOrder)
            {
                case 0:     this.m_dwMemberColor = 0xFFFFFFFF; break;
                case 1:     this.m_dwMemberColor = 0xFFFF0024; break;
                case 2:     this.m_dwMemberColor = 0xFFFFC600; break;
                case 3:     this.m_dwMemberColor = 0xFFFF00FC; break;
                default:    this.m_dwMemberColor = 0xFFFFFFFF; break;
            }
        }

        get { return this.m_cbMemberOrder; }
    }

    /// <summary> 会员等级颜色 </summary>
    public UInt32 MemberOrderColor { get { return m_dwMemberColor; } }

    /// <summary> 用户权限等级 </summary>
    public byte MasterOrder { set; get; }

    /// <summary> 用户状态 </summary>
    public byte Status { set; get; }

    /// <summary> 用户金币 </summary>
    public Int64 GameGold { set; get; }

    /// <summary> 用户银行金币 </summary>
    public Int64 BankGold { set; get; }

    /// <summary> 用户魅力值 </summary>
    public Int64 Charm { set; get; }

    /// <summary> 用户分数 </summary>
    public Int64 Score { set; get; }

    /// <summary> 用户成绩 </summary>
    public Int64 Grade { set; get; }

    /// <summary> 当前桌子ID </summary>
    public UInt32 TableID { set; get; }

    /// <summary> 当前椅子ID </summary>
    public UInt32 ChairID { set; get; }
    
    /// <summary> 昵称 </summary>
    public string NickName { set; get; }

    /// <summary> 账号 </summary>
    public string Account { set; get; }

    /// <summary> 头像路径 </summary>
    public string FacePath { set; get; }

    /// <summary> 用户性别 </summary>
    public byte Gender { set; get; }

    /// <summary> 社团索引 </summary>
    public UInt32 GroupID { set; get; }

    /// <summary> 社团名字 </summary>
    public string GroupName { set; get; }

    /// <summary> 个性签名 </summary>
    public string UnderWrite { set; get; }

    /// <summary> 等级描述 </summary>
    public string LevelDesc { set; get; }

    /// <summary> 机器码 </summary>
    public string MachineID { set; get; }

    /// <summary> 用户经验 </summary>
    public UInt32 Experience { set; get; }

    /// <summary> 胜利盘数 </summary>
    public Int32 WinCount { set; get; }

    /// <summary> 失败盘数 </summary>
    public Int32 LostCount { set; get; }

    /// <summary> 和局盘数 </summary>
    public Int32 DrawCount { set; get; }

    /// <summary> 总盘数 </summary>
    public Int32 TotalCount { get { return this.WinCount + this.LostCount + this.DrawCount; } }

    /// <summary> 断线数目 </summary>
    public Int32 FleeCount { set; get; }

    /// <summary> 消费金币 </summary>
    public Int64 InsureScore { set; get; }


    public void InitData(ref Protocol.UserStruct stuUser, object objUserSuffix = null)
    {
        this.UserID         = stuUser.dwUserID;
        this.GameID         = stuUser.dwGameID;
        this.FaceID         = stuUser.dwFaceID;
        this.TableID        = stuUser.dwTableID;
        this.ChairID        = stuUser.dwChairID;
        this.Status         = stuUser.cbStatus;
        this.IsSelf         = stuUser.cbSelf == Define.TRUE;
        this.IsRobot        = stuUser.cbRobot == Define.TRUE;
        this.MemberOrder    = stuUser.cbMemberOrder;
        this.MasterOrder    = stuUser.cbMasterOrder;
        this.GameGold       = stuUser.llGold;
        this.BankGold       = stuUser.llBankGold;
        this.Score          = stuUser.llScore;
        this.Charm          = stuUser.llCharm;
        this.NickName       = Util.TcharToString(stuUser.pzNickName);
        this.Account        = Util.TcharToString(stuUser.pzAccount);
        this.FacePath       = Util.TcharToString(stuUser.pzFacePath);

        // 附加属性
        if (objUserSuffix != null)
        {
            Protocol.UserStructSuffix stuUserSuffix = (Protocol.UserStructSuffix)objUserSuffix;
            this.Gender         = stuUserSuffix.cbGender;
            this.GroupID        = stuUserSuffix.dwGroupID;
            this.Experience     = stuUserSuffix.dwExperience;
            this.WinCount       = stuUserSuffix.lWinCount;
            this.LostCount      = stuUserSuffix.lLostCount;
            this.DrawCount      = stuUserSuffix.lDrawCount;
            this.FleeCount      = stuUserSuffix.lFleeCount;
            this.InsureScore    = stuUserSuffix.llInsureScore;
            this.GroupName      = Util.TcharToString(stuUserSuffix.pzGroupName);
            this.UnderWrite     = Util.TcharToString(stuUserSuffix.pzUnderWrite);
            this.LevelDesc      = Util.TcharToString(stuUserSuffix.pzLevelDesc);
            this.MachineID      = Util.TcharToString(stuUserSuffix.pzMachineID);
        }
    }
}