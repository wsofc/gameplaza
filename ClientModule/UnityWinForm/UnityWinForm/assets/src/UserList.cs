using System;
using System.Drawing;
using System.Collections;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;

namespace UnityWinForm
{
    public partial class UnityWinForm : Form
    {
        ContextMenuStrip m_menu_strip           = null;                                         // 右键菜单
        MouseEventArgs m_menu_item_mouseevent   = null;                                         // 菜单子项鼠标事件
        UserModel m_menu_item_binduser          = null;                                         // 菜单子项绑定的用户信息

        bool m_gridcell_first_insertbody        = true;                                         // 是否首次插入body
        Image m_gridcell_headerbg_img           = null;                                         // 列表头背景图片
        Image m_gridcell_headersep_img          = null;                                         // 列表头分隔图片
        Color m_gridcell_back_color             = UnityWinForm.SiderBarBackColor;               // 列表项默认背景色
        Color m_gridcell_over_color             = Color.FromArgb(0xFF, 0x68, 0x9F, 0xFE);       // 列表项鼠标悬浮色
        
        /// <summary> 设置玩家列表侧栏显影 </summary>
        public void SetUserSideBarVisible(bool bVisible)
        {
            this.panel_sidebar.Visible = bVisible;
            this.panel_sidebar.Enabled = bVisible;

            // 若不可见,则设置游戏窗口全屏
            if (!bVisible)
            {
                this.panel_unity_game.Size = this.ClientSize;
            }
        }
        
        /// <summary> 清除列表UI </summary>
        public void Clear()
        {
            // 重置首次插入标志
            this.m_gridcell_first_insertbody = true;

            // 清理聊天框内容
            this.richtext_outview.Clear();
        }

        // -------------------------------------------------------------------------------------------------------------------
        // 右键菜单
        #region
        ArrayList UserMenu_Init_StaticItem(bool bSep)
        {
            ArrayList vMenuInfo = new ArrayList();
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_NAME, "昵称:", Define.TRUE));
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_GAMEID, "ID:", Define.TRUE));
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_GOLD, "金币:", Define.TRUE));
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_UNDERWRITE, "个性签名:", Define.TRUE));

            if (bSep)
            {
                vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_SEPARATOR, "", Define.TRUE));
            }

            return vMenuInfo;
        }

        public void UserMenu_Init(ArrayList vDynamicList, bool bOpenControl)
        {
            // 创建菜单根节点
            if (this.m_menu_strip == null)
            {
                this.m_menu_strip = new ContextMenuStrip();
                this.m_menu_strip.AutoSize = true;
                this.m_menu_strip.ShowImageMargin = false;
                this.m_menu_strip.ShowCheckMargin = false;
                this.m_menu_strip.BackColor = Color.Black;
                this.m_menu_strip.ForeColor = Color.White;
                this.m_menu_strip.RenderMode = ToolStripRenderMode.System;
                this.m_menu_strip.MouseClick += this.UserMenu_MouseClick;
                this.m_menu_strip.ItemClicked += this.UserMenu_ItemClick;
            }

            // 激活菜单
            this.m_menu_strip.Enabled = true;

            // 加载Winform默认部分
            bool bSep = vDynamicList != null && vDynamicList.Count > 0;
            ArrayList vStaticList = this.UserMenu_Init_StaticItem(bSep);

            // 拼接静态菜单
            ArrayList vMenuList = new ArrayList();
            for (int i = 0; i < vStaticList.Count; ++i)
            {
                vMenuList.Add(vStaticList[i]);
            }

            // 拼接动态菜单
            if (vDynamicList != null && bOpenControl)
            {
                for (int i = 0; i < vDynamicList.Count; ++i)
                {
                    vMenuList.Add(vDynamicList[i]);
                }
            }

            // 创建菜单子项
            UserModel user_self = DataManager.GetInstance().UserSelf;
            for (int i = 0; i < vMenuList.Count; ++i)
            {
                if (vMenuList[i] == null) continue;
                Protocol.tagUserMenuInfo tMenuInfo = (Protocol.tagUserMenuInfo)vMenuList[i];
                Protocol.eUserMenuType eType = (Protocol.eUserMenuType)tMenuInfo.dwMenuKey;

                // 核查是否重复插入
                bool bContinue = false;
                for (int j = 0; j < this.m_menu_strip.Items.Count; ++j)
                {
                    ToolStripItem tmpItem = this.m_menu_strip.Items[j];
                    Protocol.tagUserMenuInfo stu = (Protocol.tagUserMenuInfo)tmpItem.Tag;
                    if (stu.dwMenuKey == (UInt32)eType)
                    {
                        bContinue = true;
                        break;
                    }
                }
                if (bContinue) continue;
                
                ToolStripItem item = null;
                switch (eType)
                {
                    case Protocol.eUserMenuType.UMT_SEPARATOR:
                        {
                            item = new ToolStripSeparator();
                        } break;

                    default:
                        {
                            item = new ToolStripMenuItem();
                        } break;
                }

                if (item != null)
                {
                    item.Tag = tMenuInfo;
                    item.Enabled = tMenuInfo.cbActive == Define.TRUE;
                    this.m_menu_strip.Items.Add(item);
                }
            }
        }

        public void UserMenu_Show(int x, int y, UserModel user)
        {
            if (user == null || this.m_menu_strip == null) return;
            this.m_menu_strip.Show(x, y);

            for (int i = 0; i < this.m_menu_strip.Items.Count; ++i)
            {
                ToolStripItem item = this.m_menu_strip.Items[i];
                if (item == null) continue;
                
                Protocol.tagUserMenuInfo tMenuInfo = (Protocol.tagUserMenuInfo)item.Tag;
                Protocol.eUserMenuType eType = (Protocol.eUserMenuType)tMenuInfo.dwMenuKey;

                string strMenuValue = Util.TcharToString(tMenuInfo.pzMenuValue);
                switch (eType)
                {
                    // 无
                    case Protocol.eUserMenuType.UMT_NONE: break;

                    // 预留
                    case Protocol.eUserMenuType.UMT_EXTRA: break;

                    // 间隔线
                    case Protocol.eUserMenuType.UMT_SEPARATOR: break;

                    // 用户 昵称
                    case Protocol.eUserMenuType.UMT_USER_NAME:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.NickName);
                        } break;

                    // 用户 显示ID
                    case Protocol.eUserMenuType.UMT_USER_GAMEID:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.GameID);
                        } break;

                    // 用户 金币
                    case Protocol.eUserMenuType.UMT_USER_GOLD:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.GameGold);
                        } break;

                    // 用户 个性签名
                    case Protocol.eUserMenuType.UMT_USER_UNDERWRITE:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.UnderWrite);
                        } break;

                    default:
                        {
                            // 控制子项
                            item.Text = String.Format("{0}", strMenuValue);
                        } break;
                }

                // 按游戏配置显影/激活菜单子项
                item.Enabled = tMenuInfo.cbSelfActive == Define.TRUE ? tMenuInfo.cbActive == Define.TRUE : !user.IsSelf;
                item.Visible = tMenuInfo.cbSelfVisible == Define.TRUE ? tMenuInfo.cbVisible == Define.TRUE : !user.IsSelf;
            }
        }

        public void UserMenu_Close()
        {
            if (this.m_menu_strip == null) return;

            this.m_menu_strip.Close();
        }

        public void UserMenu_MouseClick(object sender, MouseEventArgs e)
        {
            this.m_menu_item_mouseevent = e;
        }

        public void UserMenu_ItemClick(object sender, ToolStripItemClickedEventArgs e)
        {
            ToolStripItem item = e.ClickedItem;
            Protocol.tagUserMenuInfo tMenuInfo = (Protocol.tagUserMenuInfo)item.Tag;
            Protocol.eUserMenuType eType = (Protocol.eUserMenuType)tMenuInfo.dwMenuKey;

            switch (eType)
            {
                case Protocol.eUserMenuType.UMT_NONE: break;
                case Protocol.eUserMenuType.UMT_USER_NAME: break;
                case Protocol.eUserMenuType.UMT_USER_GAMEID: break;
                case Protocol.eUserMenuType.UMT_USER_GOLD: break;
                case Protocol.eUserMenuType.UMT_USER_UNDERWRITE: break;
                case Protocol.eUserMenuType.UMT_SEPARATOR: break;
                case Protocol.eUserMenuType.UMT_EXTRA: break;
                default:
                    {
                        // 控制层
                        Protocol.tagUserMenuEvent tagEvent = new Protocol.tagUserMenuEvent();
                        tagEvent.dwMenuKey = (UInt32)eType;
                        tagEvent.dwMouseButtons = this.m_menu_item_mouseevent != null ? (UInt32)this.m_menu_item_mouseevent.Button : Define.INVALID_DWORD;
                        tagEvent.dwUserID = this.m_menu_item_binduser != null ? this.m_menu_item_binduser.UserID : Define.INVALID_DWORD;

                        Server server = Server.GetInstance();
                        server.SendDataToGame(Protocol.IPC_MAIN_CONCTROL, Protocol.IPC_SUB_WINGAME_MENU_EVENT, tagEvent);
                    } break;
            }
        }

        public void UserMenu_Remove()
        {
            if (this.m_menu_strip == null) return;

            this.UserMenu_Close();
            this.m_menu_strip.Items.Clear();
            this.m_menu_strip.Enabled = false;
        }
        #endregion

        // -------------------------------------------------------------------------------------------------------------------
        // 玩家列表
        #region
        public void UserGridView_Init(bool bDistinguish)
        {
            // 只读
            this.userlist_view.ReadOnly = true;

            // 不显示带星号的默认的第一行
            this.userlist_view.RowHeadersVisible = false;

            // 整行选中
            this.userlist_view.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            // 禁止多行选中
            this.userlist_view.MultiSelect = false;

            this.userlist_view.CurrentCell = null;

            // 禁止调整行的高度
            this.userlist_view.AllowUserToResizeRows = false;

            // 列表头禁止换行,自动省略号
            this.userlist_view.ColumnHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            // 单元格的边框线样式
            this.userlist_view.CellBorderStyle = DataGridViewCellBorderStyle.None;

            // 关闭自动添加一行
            this.userlist_view.AllowUserToAddRows = false;

            // 列表背景色(ARGB)
            this.userlist_view.BackgroundColor = UnityWinForm.SiderBarBackColor;

            // 列表边框风格
            this.userlist_view.BorderStyle = BorderStyle.None;
            //this.userlist_view.ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle.None;

            // 列表头高度调整模式
            this.userlist_view.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.DisableResizing;

            // 列表头高度
            this.userlist_view.ColumnHeadersHeight = 25;

            // 列表头 关闭可视样式(否则用户自定义表头文本等颜色无效)
            this.userlist_view.EnableHeadersVisualStyles = false;

            // 鼠标点击消息
            this.userlist_view.CellMouseDown += this.UserGridView_CellMouseClick;

            // 列表头重绘
            Resource res = Resource.GetInstance();
            this.m_gridcell_headerbg_img = res.LoadImgFromAssembly("common.list.bg.png");
            this.m_gridcell_headersep_img = res.LoadImgFromAssembly("common.list.sep.png");
            this.userlist_view.CellPainting += this.UserGridView_CellPainting;
        }

        public void UserGridView_InserHead(string text, int width, bool resizable = true, bool sortmode = false)
        {
            DataGridViewColumn column = new DataGridViewColumn();
            
            // 文本
            column.HeaderText = text;

            // 只读
            column.ReadOnly = true;

            // 表头宽
            column.Width = width;

            // 最小宽度(系统规定必须>=2)
            column.MinimumWidth = 5;

            // 单元格式(这里默认为文本框)
            //column.CellTemplate = new DataGridViewTextBoxCell();
            column.HeaderCell = new DataGridViewColumnHeaderCell();
            column.HeaderCell.Value = text;
            column.HeaderCell.Style.ForeColor = Color.FromArgb(0xFF, 0xE9, 0xC0, 0x5A);

            // 设置表头调整宽度模式
            column.AutoSizeMode = DataGridViewAutoSizeColumnMode.None;

            // 表头不分行(单行)
            column.DefaultCellStyle.WrapMode = DataGridViewTriState.False;

            // 表头居中
            column.HeaderCell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;

            // 是否可以调整表头宽度
            column.Resizable = resizable ? DataGridViewTriState.True : DataGridViewTriState.False;

            // 该列是否为可排序模式
            column.SortMode = sortmode ? DataGridViewColumnSortMode.Automatic : DataGridViewColumnSortMode.NotSortable;

            try
            {
                this.userlist_view.Columns.Add(column);
            }
            catch (System.Exception ex)
            {
                Debug.Print("UserGridView_InserHead Failed: ", ex.ToString());
            }
        }

        public void UserGridView_RemoveHead(int iIndex)
        {
            this.userlist_view.Columns.RemoveAt(iIndex);
        }

        public void UserGridView_RemoveHeadAll()
        {
            this.userlist_view.Columns.Clear();
        }

        public void UserGridView_InserBody(int iIndex, UserModel user, List<Protocol.tagColumnItem> list)
        {
            // 严格检测返回(下标超出范围 | 玩家对象不存在 | 还未创建列)
            if (iIndex < 0 || user == null || list == null || this.userlist_view.Columns.Count <= 0)
            {
                return;
            }

            Resource res = Resource.GetInstance();
            DataGridViewRow row = new DataGridViewRow();

            byte cbMemberOrder = user.MemberOrder;                      // 会员等级
            UInt32 dwTextColor = user.MemberOrderColor;                 // 字体颜色
            bool bGray = user.Status == Define.US_OFFLINE;              // 灰度显示

            // 真实玩家优先会员色，非会员则标记为红色
            UserModel user_self = DataManager.GetInstance().UserSelf;
            if (user_self.MasterOrder > 0 && !user.IsRobot && user.MemberOrder < 1)
            {
                dwTextColor = 0xFFFF0000;
            }

            // 离线文字颜色
            dwTextColor = bGray ? 0xFFA0A0A4 : dwTextColor;

            // 前景文字颜色
            Color ForeColor = Color.FromArgb((int)dwTextColor);

            // 选中背景颜色
            Color SelectionBackColor = Color.FromArgb(0xFF, 0x2B, 0x3B, 0x57);

            // 选中前景(文字)颜色
            Color SelectionForeColor = Color.FromArgb((int)dwTextColor);
            
            for (int i = 0; i < list.Count; ++i)
            {
                DataGridViewCell cell = null;
                switch (list[i].wDataDescribe)
                {
                    case Protocol.DTP_MEMBERORDER:
                        {
                            DataGridViewImageCell img_cell = new DataGridViewImageCell();
                            img_cell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
                            img_cell.ImageLayout = DataGridViewImageCellLayout.Zoom;
                            cell = img_cell;
                        } break;

                    case Protocol.DTP_USER_FACE:
                        {
                            DataGridViewImageCell img_cell = new DataGridViewImageCell();
                            img_cell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
                            img_cell.ImageLayout = DataGridViewImageCellLayout.Zoom;
                            cell = img_cell;
                        } break;

                    default:
                        {
                            cell = new DataGridViewTextBoxCell();
                            cell.Style.Alignment = DataGridViewContentAlignment.MiddleLeft;
                        } break;
                }

                if (cell != null)
                {
                    cell.Style.BackColor = UnityWinForm.SiderBarBackColor;
                    cell.Style.ForeColor = ForeColor;

                    cell.Style.SelectionBackColor = SelectionBackColor;
                    cell.Style.SelectionForeColor = SelectionForeColor;

                    row.Cells.Add(cell);
                    row.Height = UserList.GetInstance().BodyItemHeight;
                }
            }

            try
            {
                if (iIndex > this.userlist_view.Rows.Count)
                {
                    UserList user_list = UserList.GetInstance();
                    Debug.Print("UserGridView_InserBody Error - Test: index = {0}, count = {1}, headlist_count = {2}, bodylist_count = {3}"
                    , iIndex, this.userlist_view.Rows.Count, user_list.HeadListCount, user_list.BodyListCount);
                }

                iIndex = Math.Min(iIndex, this.userlist_view.Rows.Count);
                this.userlist_view.Rows.Insert(iIndex, row);
            }
            catch (Exception ex)
            {
                UserList user_list = UserList.GetInstance();
                Debug.Print("UserGridView_InserBody Error : index = {0}, count = {1}, headlist_count = {2}, bodylist_count = {3}"
                    , iIndex, this.userlist_view.Rows.Count, user_list.HeadListCount, user_list.BodyListCount);

                Debug.Print("UserGridView_InserBody Error Msg : " + ex.ToString());
            }
            this.UserGridView_UpdateBody(iIndex, user, list);

            // 初次不选中默认单元行
            if (this.m_gridcell_first_insertbody)
            {
                this.m_gridcell_first_insertbody = false;
                this.userlist_view.Rows[0].Selected = false;
            }
        }

        public void UserGridView_RemoveBody(int iIndex)
        {
            if (iIndex < 0 || iIndex > this.userlist_view.Rows.Count) return;
            this.userlist_view.Rows.RemoveAt(iIndex);
        }

        public void UserGridView_RemoveBodyAll()
        {
            this.userlist_view.Rows.Clear();
        }

        public void UserGridView_UpdateBody(int iIndex, UserModel user, List<Protocol.tagColumnItem> list)
        {
            if (iIndex < 0 || iIndex >= this.userlist_view.Rows.Count) return;

            Resource res = Resource.GetInstance();
            DataGridViewRow row = this.userlist_view.Rows[iIndex];
            row.Tag = user;

            for (int i = 0; i < list.Count; ++i)
            {
                DataGridViewCell cell = row.Cells[i];
                switch (list[i].wDataDescribe)
                {
                    // 用户ID
                    case Protocol.DTP_USER_ID:
                        {
                            cell.Value = user.UserID;
                        } break;

                    // 游戏ID
                    case Protocol.DTP_GAME_ID:
                        {
                            cell.Value = user.GameID;
                        } break;

                    // 用户状态
                    case Protocol.DTP_USER_STATUS:
                        {
                            string strDesc = "";
                            switch (user.Status)
                            {
                                case Define.US_NULL: strDesc = "无"; break;
                                case Define.US_FREE: strDesc = "站立"; break;
                                case Define.US_SIT: strDesc = "坐下"; break;
                                case Define.US_READY: strDesc = "准备"; break;
                                case Define.US_LOOKON: strDesc = "旁观"; break;
                                case Define.US_PLAY: strDesc = "游戏"; break;
                                case Define.US_OFFLINE: strDesc = "断线"; break;
                                default: strDesc = "无"; break;
                            }
                            cell.Value = String.Format("{0}-{1}", user.Status, strDesc);
                        } break;

                    // 会员等级
                    case Protocol.DTP_MEMBERORDER:
                        {
                            DataGridViewImageCell img_cell = cell as DataGridViewImageCell;
                            if (img_cell == null) return;

                            if (user.MemberOrder > 0)
                            {
                                string filename = String.Format("common.list.member_{0}.png", user.MemberOrder);
                                Image img = res.LoadImgFromAssembly(filename);
                                if (img != null)
                                {
                                    img_cell.Value = img;
                                }
                                else
                                {

                                    img_cell.Value = Resource.GetInstance().LoadImgFromAssembly("ui.member_default.png"); ;
                                }
                            }
                            else
                            {
                                img_cell.Value = Resource.GetInstance().LoadImgFromAssembly("ui.cell_default.png");
                            }
                        } break;

                    // 头像
                    case Protocol.DTP_USER_FACE:
                        {
                            DataGridViewImageCell img_cell = cell as DataGridViewImageCell;
                            if (img_cell == null) break;

                            string filename = String.Format("common.headPic.{0}.png", user.FaceID);
                            Image img = res.LoadImgFromAssembly(filename);
                            if (img != null)
                            {
                                img_cell.Value = img;
                            }
                            else
                            {
                                img_cell.Value = Resource.GetInstance().LoadImgFromAssembly("ui.headPic_default.png");
                            }
                        } break;


                    // 昵称
                    case Protocol.DTP_NICKNAME:
                        {
                            cell.Value = user.NickName;
                        } break;

                    // 金币
                    case Protocol.DTP_GAME_GOLD:
                        {
                            cell.Value = user.GameGold;
                        } break;

                    // 用户分数
                    case Protocol.DTP_USER_SCORE:
                        {
                            cell.Value = user.Score;
                        } break;

                    // 桌号
                    case Protocol.DTP_USER_TABLE:
                        {
                            cell.Value = user.TableID;
                        } break;

                    // 魅力
                    case Protocol.DTP_CHARM:
                        {
                            cell.Value = user.Charm;
                        } break;

                    // 游戏等级(称号)
                    case Protocol.DTP_GAME_LEVEL:
                        {
                            cell.Value = user.LevelDesc;
                        } break;

                    // 个性签名
                    case Protocol.DTP_UNDER_WRITE:
                        {
                            cell.Value = user.UnderWrite;
                        } break;

                    // 总局盘数
                    case Protocol.DTP_USER_PLAY_COUNT:
                        {
                            cell.Value = user.TotalCount;
                        } break;

                    // 用户经验
                    case Protocol.DTP_USER_EXPERIENCE:
                        {
                            cell.Value = user.Experience;
                        } break;

                    default:
                        {
                            cell.Value = "无";
                        } break;
                }

                //if (cell.ToolTipText.Equals(""))
                //{
                //    cell.ToolTipText = "";
                //}
                //else
                //{
                //    cell.ToolTipText = cell.Value.ToString();
                //}
            }
        }

        void UserGridView_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            // 点击的是鼠标右键，并且不是表头
            if (e.Button == MouseButtons.Right && e.ColumnIndex > -1 && e.RowIndex > -1)
            {
                // 右键选中行
                DataGridViewRow row = this.userlist_view.Rows[e.RowIndex];
                row.Selected = true;
                this.m_menu_item_binduser = row.Tag as UserModel;
                this.UserMenu_Show(MousePosition.X, MousePosition.Y, this.m_menu_item_binduser);
            }
        }

        void UserGridView_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (e.RowIndex == -1)
            {
                Util.Scale9Sprite(e.Graphics, this.m_gridcell_headerbg_img, e.CellBounds, new Rectangle(6, 6, 6, 6));

                Image sep_img = this.m_gridcell_headersep_img;
                e.Graphics.DrawImage(sep_img, new PointF(e.CellBounds.X + e.CellBounds.Width - sep_img.Width, e.CellBounds.Y + (e.CellBounds.Height - sep_img.Height) / 2));

                //bool bMouseOver = e.CellBounds.Contains(this.userlist_view.PointToClient(Control.MousePosition));
                //if (bMouseOver)
                //{
                //    Debug.Print("\n鼠标悬浮CellHeader上");
                //}

                e.PaintContent(e.CellBounds);
                e.Handled = true;
            }
        }
        #endregion
        
        // -------------------------------------------------------------------------------------------------------------------
        // 输入输出文本框
        #region
        void _SendOutTextMsg(string text, Color color)
        {
            // 先选中尾部,然后在设置选中颜色,最后在插入新文本
            if (text == null || text.Length <= 0) return;
            this.richtext_outview.Select(this.richtext_outview.TextLength, 0);
            this.richtext_outview.SelectionColor = color;
            this.richtext_outview.AppendText(text);

            // 滚动文本位置到最后一行 
            this.richtext_outview.ScrollToCaret();
        }

        // 消息过多则清理
        public void LimitCheckMsg()
        {
            if (this.richtext_outview.Text.Length > this.richtext_outview.MaxLength)
            {
                this.richtext_outview.Clear();
                this.SendOutTextMsg("消息过多,系统自动清理\n", Color.Red);
            }
        }

        public void SendOutTextMsg(string text, Color color)
        {
            if (text == null || text.Length <= 0) return;

            // 非UI线程调用
            if (this.richtext_outview.InvokeRequired)
            {
                this.richtext_outview.Invoke(new MethodInvoker(() =>
                {
                    this._SendOutTextMsg(text, color);
                }));
            }
            // UI线程调用
            else
            {
                this._SendOutTextMsg(text, color);
            }
        }
        #endregion
    }
}

class UserList
{
    private static UserList g_Instance                      = null;     // 单例
    
    private bool m_bOpenUserList                            = true;     // 是否显示列表
    private List<Protocol.tagColumnItem> m_HeadList         = null;     // 列表头
    private List<UserModel> m_BodyList                      = null;     // 玩家列表

    /// <summary> 列表每一行的高度 </summary>
    public int BodyItemHeight { set; get; }

    /// <summary> 窗体实例 </summary>
    public UnityWinForm.UnityWinForm unity_form { set; get; }

    /// <summary> 是否开启玩家列表 </summary>
    public bool OpenUserList
    {
        set
        {
            this.m_bOpenUserList = value;
            this.SetVisible(this.m_bOpenUserList);
        }

        get { return this.m_bOpenUserList; }
    }

    /// <summary> 是否打印玩家进出房间消息 </summary>
    public bool OpenPrintUserInOutMsg { set; get; }

    public int HeadListCount { get { return this.m_HeadList.Count; } }

    public int BodyListCount { get { return this.m_BodyList.Count; } }

    public static UserList GetInstance()
    {
        if (UserList.g_Instance == null)
        {
            UserList.g_Instance = new UserList();
        }
        return UserList.g_Instance;
    }

    UserList()
    {
        this.m_HeadList = new List<Protocol.tagColumnItem>();
        this.m_BodyList = new List<UserModel>();
    }

    ~UserList() { }

    public void Init()
    {
        this.BodyItemHeight = 25;
        if (this.unity_form != null)
        {
            this.unity_form.UserGridView_Init(false);
            this.SetVisible(this.OpenUserList);
        }
    }

    public void InitSelf(UserModel user)
    {
        if (this.unity_form != null)
        {
            this.unity_form.SetUserNickName(user.NickName);
            this.unity_form.SetUserGameID(user.GameID);
            this.unity_form.SetUserGold(user.GameGold);
            this.unity_form.SetUserHeadPortrait(user.FaceID);
        }
    }

    public void Clear(bool bBodyClean = true, bool bHeadClean = true)
    {
        // 清除所有表项
        if (bBodyClean)
        {
            this.RemoveBodyAll();
        }

        // 清除所有表头
        if (bHeadClean)
        {
            this.RemoveHeadAll();
        }

        // 清除UI标志灯
        if(this.unity_form != null)
        {
            this.unity_form.Clear();
        }
    }

    public void SetVisible(bool bVisible)
    {
        if (this.unity_form != null)
        {
            this.unity_form.SetUserSideBarVisible(bVisible);
        }
    }
    
    public void CreateUserMenu(ArrayList vMenuInfo, bool bOpenControl)
    {
        if (this.unity_form != null)
        {
            this.unity_form.UserMenu_Init(vMenuInfo, bOpenControl);
        }
    }
    
    public void InsertHead(Protocol.tagColumnItem tColumItem)
    {
        // 避免重复插入头字段
        for (int i = 0; i < this.m_HeadList.Count; ++i)
        {
            if (this.m_HeadList[i].wDataDescribe == tColumItem.wDataDescribe)
            {
                return;
            }
        }

        this.m_HeadList.Add(tColumItem);
        
        // 添加UI
        if (this.unity_form != null)
        {
            string head_text = Util.TcharToString(tColumItem.szColumnName);
            int head_width = (int)tColumItem.wColumnWidth;

            bool disable = false;
            disable |= tColumItem.wDataDescribe == Protocol.DTP_USER_STATUS;
            disable |= tColumItem.wDataDescribe == Protocol.DTP_MEMBERORDER;
            disable |= tColumItem.wDataDescribe == Protocol.DTP_USER_FACE;
            this.unity_form.UserGridView_InserHead(head_text, head_width, !disable);
        }
    }

    public void RemoveHead(UInt16 wDataDescribe)
    {
        for (int i = 0; i < this.m_HeadList.Count; ++i)
        {
            if (this.m_HeadList[i].wDataDescribe == wDataDescribe)
            {
                this.m_HeadList.RemoveAt(i);

                // 更新UI
                if (this.unity_form != null)
                {
                    this.unity_form.UserGridView_RemoveHead(i);
                }

                break;
            }
        }
    }

    public void RemoveHeadAll()
    {
        this.m_HeadList.Clear();

        // 更新UI
        if (this.unity_form != null)
        {
            this.unity_form.UserGridView_RemoveHeadAll();
        }
    }
    
    public void InsertBody(UserModel user)
    {
        // 玩家不存在 或 列表头未创建 则返回
        if (user == null || this.HeadListCount <= 0) return;

        UserModel user_self = DataManager.GetInstance().UserSelf;

        // 先检测一遍,避免重复添加(主要针对多次重复调用时生效)
        for (int i = 0; i < this.m_BodyList.Count; ++i)
        {
            if (this.m_BodyList[i].UserID == user.UserID)
            {
                return;
            }
        }
        
        // 插入玩家
        bool bHide = false;
        byte cbUserMasterOrder = user.MasterOrder;
        byte cbUserMemberOrder = user.MemberOrder;

        byte cbSelfMasterOrder = user_self.MasterOrder;
        byte cbSelfMemberOrder = user_self.MemberOrder;

        byte cbBodyMasterOrder = 0;
        byte cbBodyMemberOrder = 0;

        // 隐藏权限级别比自己高的玩家
        if (user.UserID != user_self.UserID)
        {
            if (cbUserMasterOrder > 0 && cbUserMasterOrder > cbSelfMasterOrder)
            {
                bHide = true;
            }
        }
        if (bHide) return;

        // 添加玩家数据
        bool bPushBack = true;
        int iIndex = 0;
        for (int i = 0; i < this.m_BodyList.Count; ++i)
        {
            iIndex = i;
            cbBodyMasterOrder = this.m_BodyList[i].MasterOrder;
            cbBodyMemberOrder = this.m_BodyList[i].MemberOrder;

            // 1.优先插入管理员(降序)
            if ((cbSelfMasterOrder > cbUserMasterOrder) && cbUserMasterOrder > cbBodyMasterOrder)
            {
                bPushBack = false;
                this.m_BodyList.Insert(i, user);
                break;
            }
            // 2.优先插入会员等级高的玩家(管理员 > 玩家 > 会员机器人 > 普通机器人)
            else if (cbUserMemberOrder > cbBodyMemberOrder && !this.m_BodyList[i].IsSelf)
            {
                if (cbUserMasterOrder < cbBodyMasterOrder || (user.IsRobot && !this.m_BodyList[i].IsRobot))
                {
                    continue;
                }
                else if (cbSelfMasterOrder > 0 && (user.IsRobot && !this.m_BodyList[i].IsRobot))
                {
                    continue;
                }

                bPushBack = false;
                this.m_BodyList.Insert(i, user);
                break;
            }
            // 3.管理员模式(优先插入真实玩家)
            else if (cbSelfMasterOrder > 0 && !user.IsRobot && this.m_BodyList[i].IsRobot)
            {
                bPushBack = false;
                this.m_BodyList.Insert(i, user);
                break;
            }

            // 4.插入到离线玩家之前
            //else if (this.m_BodyList[i].Status == Define.US_OFFLINE)
            //{
            //    bPushBack = false;
            //    break;
            //}
        }

        // 以上条件都不满足,则插入末尾
        if (bPushBack)
        {
            iIndex = this.m_BodyList.Count;
            this.m_BodyList.Insert(iIndex, user);
        }

        // 第一位显示自己
        do
        {
            UserModel pSelf = null;
            for (int i = 0; i < this.m_BodyList.Count; ++i)
            {
                if (this.m_BodyList[i].IsSelf && i != 0)
                {
                    pSelf = this.m_BodyList[i];
                    this.m_BodyList.RemoveAt(i);
                    break;
                }
            }

            // 自身插入第一位
            if (pSelf != null)
            {
                this.m_BodyList.Insert(0, pSelf);
            }
        } while (false);

        DataManager dataMgr = DataManager.GetInstance();
        if (this.OpenPrintUserInOutMsg && dataMgr.SceneInited)
        {
            this.ParseTextMsg(String.Format("\n[0x00FFFFFF]{0}[-] [0xFFFFFFFF]进来了[-]", user.NickName));
        }

        // 更新UI
        if (this.unity_form != null)
        {
            this.unity_form.UserGridView_InserBody(iIndex, user, this.m_HeadList);
        }
    }

    public void RemoveBody(UserModel user)
    {
        if (user == null) return;
        for (int i = 0; i < this.m_BodyList.Count; ++i)
        {
            if (this.m_BodyList[i].UserID == user.UserID)
            {
                if (this.OpenPrintUserInOutMsg)
                {
                    this.ParseTextMsg(String.Format("\n[0x00FFFFFF]{0}[-] [0xFFFFFFFF]离开了房间[-]", user.NickName));
                }
                
                // 移除列表数据
                this.m_BodyList.RemoveAt(i);

                // 更新UI
                if (this.unity_form != null)
                {
                    this.unity_form.UserGridView_RemoveBody(i);
                }

                break;
            }
        }
    }

    public void RemoveBodyAll()
    {
        this.m_BodyList.Clear();

        // 更新UI
        if (this.unity_form != null)
        {
            this.unity_form.UserGridView_RemoveBodyAll();
        }
    }

    public void RemoveMenu()
    {
        if (this.unity_form != null)
        {
            this.unity_form.UserMenu_Remove();
        }
    }

    public void RemoveAll()
    {
        this.RemoveBodyAll();
        this.RemoveHeadAll();
        this.RemoveMenu();
    }

    public void UpdateBody(UserModel user)
    {
        // 更新侧栏玩家自身信息栏的金币
        if (user.IsSelf) this.unity_form.SetUserGold(user.GameGold);

        // 更新侧栏玩家列表中的金币
        for (int i = 0; i < this.m_BodyList.Count; ++i)
        {
            if (user.UserID == this.m_BodyList[i].UserID)
            {
                if (this.unity_form != null)
                {
                    this.unity_form.UserGridView_UpdateBody(i, user, this.m_HeadList);
                }
                break;
            }
        }
    }

    public void SendOutTextMsg(string text, Color color)
    {
        if (this.unity_form != null)
        {
            this.unity_form.SendOutTextMsg(text, color);
        }
    }

    public void ParseTextMsg(string text)
    {
        if (this.unity_form == null || text.Length <= 0) return;

        // 定义正则表达式用来匹配(格式为:[argb]text[-])
        System.Text.RegularExpressions.Regex reg = new System.Text.RegularExpressions.Regex(@"\[[0x]*[0-9a-fA-F]+\].*?\[-\]");
        System.Text.RegularExpressions.MatchCollection matches = reg.Matches(text);

        // 搜索匹配的字符串
        int iLastEndIndex = 0;
        for (int i = 0; i < matches.Count; ++i)
        {
            int index = matches[i].Index;
            string value = matches[i].Value;
            
            if(iLastEndIndex != index && index != 0)
            {
                string strText = text.Substring(iLastEndIndex, index - iLastEndIndex);
                this.SendOutTextMsg(strText, Color.White);
            }

            int ipos1 = value.IndexOf('[', 0);
            int ipos2 = value.IndexOf(']', 0);
            int ipos3 = value.Length - "[-]".Length;
            int iColor = 0;

            string sColor = value.Substring(ipos1 + 1, ipos2 - ipos1 - 1);
            string sValue = value.Substring(ipos2 + 1, ipos3 - ipos2 - 1);
            Color color = Color.Red;

            try
            {
                iColor = Convert.ToInt32(sColor, 16);

                int r = WinAPI.LOBYTE((iColor) >> 24);
                int g = WinAPI.LOBYTE(WinAPI.HIWORD(iColor));
                int b = WinAPI.LOBYTE(((UInt16)(iColor)) >> 8);
                int a = WinAPI.LOBYTE(iColor);
                color = Color.FromArgb(a, r, g, b);
            }
            catch (Exception ex)
            {
                Debug.Print("UserList - ParseTextMsg Color Error: value = {0}", sColor);
                Debug.Print("UserList - ParseTextMsg Color Error: msg = {0}", ex.ToString());
            }

            this.SendOutTextMsg(sValue, color);
            iLastEndIndex = index + value.Length;
        }

        if (iLastEndIndex != text.Length)
        {
            string strText = text.Substring(iLastEndIndex, text.Length - iLastEndIndex);
            this.SendOutTextMsg(strText, Color.White);
        }

        this.unity_form.LimitCheckMsg();
    }
}