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

    private byte m_cbMemberOrder    = 0;            // ��Ա�ȼ�
    private UInt32 m_dwMemberColor  = 0xFFFFFFFF;   // ��Ӧ��Ա�б�������ɫ

    /// <summary> �Ƿ����Լ����� </summary>
    public bool IsSelf { set; get; }

    /// <summary> �Ƿ��ǻ����� </summary>
    public bool IsRobot { set; get; }

    /// <summary> �û�ID </summary>
    public UInt32 UserID { set; get; }

    /// <summary> �û���ʾ����ϷID </summary>
    public UInt32 GameID { set; get; }

    /// <summary> �û�ͷ��ID </summary>
    public UInt32 FaceID { set; get; }

    /// <summary> �û���Ա�ȼ� </summary>
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

    /// <summary> ��Ա�ȼ���ɫ </summary>
    public UInt32 MemberOrderColor { get { return m_dwMemberColor; } }

    /// <summary> �û�Ȩ�޵ȼ� </summary>
    public byte MasterOrder { set; get; }

    /// <summary> �û�״̬ </summary>
    public byte Status { set; get; }

    /// <summary> �û���� </summary>
    public Int64 GameGold { set; get; }

    /// <summary> �û����н�� </summary>
    public Int64 BankGold { set; get; }

    /// <summary> �û�����ֵ </summary>
    public Int64 Charm { set; get; }

    /// <summary> �û����� </summary>
    public Int64 Score { set; get; }

    /// <summary> �û��ɼ� </summary>
    public Int64 Grade { set; get; }

    /// <summary> ��ǰ����ID </summary>
    public UInt32 TableID { set; get; }

    /// <summary> ��ǰ����ID </summary>
    public UInt32 ChairID { set; get; }
    
    /// <summary> �ǳ� </summary>
    public string NickName { set; get; }

    /// <summary> �˺� </summary>
    public string Account { set; get; }

    /// <summary> ͷ��·�� </summary>
    public string FacePath { set; get; }

    /// <summary> �û��Ա� </summary>
    public byte Gender { set; get; }

    /// <summary> �������� </summary>
    public UInt32 GroupID { set; get; }

    /// <summary> �������� </summary>
    public string GroupName { set; get; }

    /// <summary> ����ǩ�� </summary>
    public string UnderWrite { set; get; }

    /// <summary> �ȼ����� </summary>
    public string LevelDesc { set; get; }

    /// <summary> ������ </summary>
    public string MachineID { set; get; }

    /// <summary> �û����� </summary>
    public UInt32 Experience { set; get; }

    /// <summary> ʤ������ </summary>
    public Int32 WinCount { set; get; }

    /// <summary> ʧ������ </summary>
    public Int32 LostCount { set; get; }

    /// <summary> �;����� </summary>
    public Int32 DrawCount { set; get; }

    /// <summary> ������ </summary>
    public Int32 TotalCount { get { return this.WinCount + this.LostCount + this.DrawCount; } }

    /// <summary> ������Ŀ </summary>
    public Int32 FleeCount { set; get; }

    /// <summary> ���ѽ�� </summary>
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

        // ��������
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