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
        ContextMenuStrip m_menu_strip           = null;                                         // �Ҽ��˵�
        MouseEventArgs m_menu_item_mouseevent   = null;                                         // �˵���������¼�
        UserModel m_menu_item_binduser          = null;                                         // �˵�����󶨵��û���Ϣ

        bool m_gridcell_first_insertbody        = true;                                         // �Ƿ��״β���body
        Image m_gridcell_headerbg_img           = null;                                         // �б�ͷ����ͼƬ
        Image m_gridcell_headersep_img          = null;                                         // �б�ͷ�ָ�ͼƬ
        Color m_gridcell_back_color             = UnityWinForm.SiderBarBackColor;               // �б���Ĭ�ϱ���ɫ
        Color m_gridcell_over_color             = Color.FromArgb(0xFF, 0x68, 0x9F, 0xFE);       // �б����������ɫ
        
        /// <summary> ��������б������Ӱ </summary>
        public void SetUserSideBarVisible(bool bVisible)
        {
            this.panel_sidebar.Visible = bVisible;
            this.panel_sidebar.Enabled = bVisible;

            // �����ɼ�,��������Ϸ����ȫ��
            if (!bVisible)
            {
                this.panel_unity_game.Size = this.ClientSize;
            }
        }
        
        /// <summary> ����б�UI </summary>
        public void Clear()
        {
            // �����״β����־
            this.m_gridcell_first_insertbody = true;

            // �������������
            this.richtext_outview.Clear();
        }

        // -------------------------------------------------------------------------------------------------------------------
        // �Ҽ��˵�
        #region
        ArrayList UserMenu_Init_StaticItem(bool bSep)
        {
            ArrayList vMenuInfo = new ArrayList();
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_NAME, "�ǳ�:", Define.TRUE));
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_GAMEID, "ID:", Define.TRUE));
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_GOLD, "���:", Define.TRUE));
            vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_USER_UNDERWRITE, "����ǩ��:", Define.TRUE));

            if (bSep)
            {
                vMenuInfo.Add(new Protocol.tagUserMenuInfo((UInt32)Protocol.eUserMenuType.UMT_SEPARATOR, "", Define.TRUE));
            }

            return vMenuInfo;
        }

        public void UserMenu_Init(ArrayList vDynamicList, bool bOpenControl)
        {
            // �����˵����ڵ�
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

            // ����˵�
            this.m_menu_strip.Enabled = true;

            // ����WinformĬ�ϲ���
            bool bSep = vDynamicList != null && vDynamicList.Count > 0;
            ArrayList vStaticList = this.UserMenu_Init_StaticItem(bSep);

            // ƴ�Ӿ�̬�˵�
            ArrayList vMenuList = new ArrayList();
            for (int i = 0; i < vStaticList.Count; ++i)
            {
                vMenuList.Add(vStaticList[i]);
            }

            // ƴ�Ӷ�̬�˵�
            if (vDynamicList != null && bOpenControl)
            {
                for (int i = 0; i < vDynamicList.Count; ++i)
                {
                    vMenuList.Add(vDynamicList[i]);
                }
            }

            // �����˵�����
            UserModel user_self = DataManager.GetInstance().UserSelf;
            for (int i = 0; i < vMenuList.Count; ++i)
            {
                if (vMenuList[i] == null) continue;
                Protocol.tagUserMenuInfo tMenuInfo = (Protocol.tagUserMenuInfo)vMenuList[i];
                Protocol.eUserMenuType eType = (Protocol.eUserMenuType)tMenuInfo.dwMenuKey;

                // �˲��Ƿ��ظ�����
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
                    // ��
                    case Protocol.eUserMenuType.UMT_NONE: break;

                    // Ԥ��
                    case Protocol.eUserMenuType.UMT_EXTRA: break;

                    // �����
                    case Protocol.eUserMenuType.UMT_SEPARATOR: break;

                    // �û� �ǳ�
                    case Protocol.eUserMenuType.UMT_USER_NAME:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.NickName);
                        } break;

                    // �û� ��ʾID
                    case Protocol.eUserMenuType.UMT_USER_GAMEID:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.GameID);
                        } break;

                    // �û� ���
                    case Protocol.eUserMenuType.UMT_USER_GOLD:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.GameGold);
                        } break;

                    // �û� ����ǩ��
                    case Protocol.eUserMenuType.UMT_USER_UNDERWRITE:
                        {
                            item.Text = String.Format("{0}{1}", strMenuValue, user.UnderWrite);
                        } break;

                    default:
                        {
                            // ��������
                            item.Text = String.Format("{0}", strMenuValue);
                        } break;
                }

                // ����Ϸ������Ӱ/����˵�����
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
                        // ���Ʋ�
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
        // ����б�
        #region
        public void UserGridView_Init(bool bDistinguish)
        {
            // ֻ��
            this.userlist_view.ReadOnly = true;

            // ����ʾ���Ǻŵ�Ĭ�ϵĵ�һ��
            this.userlist_view.RowHeadersVisible = false;

            // ����ѡ��
            this.userlist_view.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            // ��ֹ����ѡ��
            this.userlist_view.MultiSelect = false;

            this.userlist_view.CurrentCell = null;

            // ��ֹ�����еĸ߶�
            this.userlist_view.AllowUserToResizeRows = false;

            // �б�ͷ��ֹ����,�Զ�ʡ�Ժ�
            this.userlist_view.ColumnHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            // ��Ԫ��ı߿�����ʽ
            this.userlist_view.CellBorderStyle = DataGridViewCellBorderStyle.None;

            // �ر��Զ����һ��
            this.userlist_view.AllowUserToAddRows = false;

            // �б���ɫ(ARGB)
            this.userlist_view.BackgroundColor = UnityWinForm.SiderBarBackColor;

            // �б�߿���
            this.userlist_view.BorderStyle = BorderStyle.None;
            //this.userlist_view.ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle.None;

            // �б�ͷ�߶ȵ���ģʽ
            this.userlist_view.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.DisableResizing;

            // �б�ͷ�߶�
            this.userlist_view.ColumnHeadersHeight = 25;

            // �б�ͷ �رտ�����ʽ(�����û��Զ����ͷ�ı�����ɫ��Ч)
            this.userlist_view.EnableHeadersVisualStyles = false;

            // �������Ϣ
            this.userlist_view.CellMouseDown += this.UserGridView_CellMouseClick;

            // �б�ͷ�ػ�
            Resource res = Resource.GetInstance();
            this.m_gridcell_headerbg_img = res.LoadImgFromAssembly("common.list.bg.png");
            this.m_gridcell_headersep_img = res.LoadImgFromAssembly("common.list.sep.png");
            this.userlist_view.CellPainting += this.UserGridView_CellPainting;
        }

        public void UserGridView_InserHead(string text, int width, bool resizable = true, bool sortmode = false)
        {
            DataGridViewColumn column = new DataGridViewColumn();
            
            // �ı�
            column.HeaderText = text;

            // ֻ��
            column.ReadOnly = true;

            // ��ͷ��
            column.Width = width;

            // ��С���(ϵͳ�涨����>=2)
            column.MinimumWidth = 5;

            // ��Ԫ��ʽ(����Ĭ��Ϊ�ı���)
            //column.CellTemplate = new DataGridViewTextBoxCell();
            column.HeaderCell = new DataGridViewColumnHeaderCell();
            column.HeaderCell.Value = text;
            column.HeaderCell.Style.ForeColor = Color.FromArgb(0xFF, 0xE9, 0xC0, 0x5A);

            // ���ñ�ͷ�������ģʽ
            column.AutoSizeMode = DataGridViewAutoSizeColumnMode.None;

            // ��ͷ������(����)
            column.DefaultCellStyle.WrapMode = DataGridViewTriState.False;

            // ��ͷ����
            column.HeaderCell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;

            // �Ƿ���Ե�����ͷ���
            column.Resizable = resizable ? DataGridViewTriState.True : DataGridViewTriState.False;

            // �����Ƿ�Ϊ������ģʽ
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
            // �ϸ��ⷵ��(�±곬����Χ | ��Ҷ��󲻴��� | ��δ������)
            if (iIndex < 0 || user == null || list == null || this.userlist_view.Columns.Count <= 0)
            {
                return;
            }

            Resource res = Resource.GetInstance();
            DataGridViewRow row = new DataGridViewRow();

            byte cbMemberOrder = user.MemberOrder;                      // ��Ա�ȼ�
            UInt32 dwTextColor = user.MemberOrderColor;                 // ������ɫ
            bool bGray = user.Status == Define.US_OFFLINE;              // �Ҷ���ʾ

            // ��ʵ������Ȼ�Աɫ���ǻ�Ա����Ϊ��ɫ
            UserModel user_self = DataManager.GetInstance().UserSelf;
            if (user_self.MasterOrder > 0 && !user.IsRobot && user.MemberOrder < 1)
            {
                dwTextColor = 0xFFFF0000;
            }

            // ����������ɫ
            dwTextColor = bGray ? 0xFFA0A0A4 : dwTextColor;

            // ǰ��������ɫ
            Color ForeColor = Color.FromArgb((int)dwTextColor);

            // ѡ�б�����ɫ
            Color SelectionBackColor = Color.FromArgb(0xFF, 0x2B, 0x3B, 0x57);

            // ѡ��ǰ��(����)��ɫ
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

            // ���β�ѡ��Ĭ�ϵ�Ԫ��
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
                    // �û�ID
                    case Protocol.DTP_USER_ID:
                        {
                            cell.Value = user.UserID;
                        } break;

                    // ��ϷID
                    case Protocol.DTP_GAME_ID:
                        {
                            cell.Value = user.GameID;
                        } break;

                    // �û�״̬
                    case Protocol.DTP_USER_STATUS:
                        {
                            string strDesc = "";
                            switch (user.Status)
                            {
                                case Define.US_NULL: strDesc = "��"; break;
                                case Define.US_FREE: strDesc = "վ��"; break;
                                case Define.US_SIT: strDesc = "����"; break;
                                case Define.US_READY: strDesc = "׼��"; break;
                                case Define.US_LOOKON: strDesc = "�Թ�"; break;
                                case Define.US_PLAY: strDesc = "��Ϸ"; break;
                                case Define.US_OFFLINE: strDesc = "����"; break;
                                default: strDesc = "��"; break;
                            }
                            cell.Value = String.Format("{0}-{1}", user.Status, strDesc);
                        } break;

                    // ��Ա�ȼ�
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

                    // ͷ��
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


                    // �ǳ�
                    case Protocol.DTP_NICKNAME:
                        {
                            cell.Value = user.NickName;
                        } break;

                    // ���
                    case Protocol.DTP_GAME_GOLD:
                        {
                            cell.Value = user.GameGold;
                        } break;

                    // �û�����
                    case Protocol.DTP_USER_SCORE:
                        {
                            cell.Value = user.Score;
                        } break;

                    // ����
                    case Protocol.DTP_USER_TABLE:
                        {
                            cell.Value = user.TableID;
                        } break;

                    // ����
                    case Protocol.DTP_CHARM:
                        {
                            cell.Value = user.Charm;
                        } break;

                    // ��Ϸ�ȼ�(�ƺ�)
                    case Protocol.DTP_GAME_LEVEL:
                        {
                            cell.Value = user.LevelDesc;
                        } break;

                    // ����ǩ��
                    case Protocol.DTP_UNDER_WRITE:
                        {
                            cell.Value = user.UnderWrite;
                        } break;

                    // �ܾ�����
                    case Protocol.DTP_USER_PLAY_COUNT:
                        {
                            cell.Value = user.TotalCount;
                        } break;

                    // �û�����
                    case Protocol.DTP_USER_EXPERIENCE:
                        {
                            cell.Value = user.Experience;
                        } break;

                    default:
                        {
                            cell.Value = "��";
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
            // �����������Ҽ������Ҳ��Ǳ�ͷ
            if (e.Button == MouseButtons.Right && e.ColumnIndex > -1 && e.RowIndex > -1)
            {
                // �Ҽ�ѡ����
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
                //    Debug.Print("\n�������CellHeader��");
                //}

                e.PaintContent(e.CellBounds);
                e.Handled = true;
            }
        }
        #endregion
        
        // -------------------------------------------------------------------------------------------------------------------
        // ��������ı���
        #region
        void _SendOutTextMsg(string text, Color color)
        {
            // ��ѡ��β��,Ȼ��������ѡ����ɫ,����ڲ������ı�
            if (text == null || text.Length <= 0) return;
            this.richtext_outview.Select(this.richtext_outview.TextLength, 0);
            this.richtext_outview.SelectionColor = color;
            this.richtext_outview.AppendText(text);

            // �����ı�λ�õ����һ�� 
            this.richtext_outview.ScrollToCaret();
        }

        // ��Ϣ����������
        public void LimitCheckMsg()
        {
            if (this.richtext_outview.Text.Length > this.richtext_outview.MaxLength)
            {
                this.richtext_outview.Clear();
                this.SendOutTextMsg("��Ϣ����,ϵͳ�Զ�����\n", Color.Red);
            }
        }

        public void SendOutTextMsg(string text, Color color)
        {
            if (text == null || text.Length <= 0) return;

            // ��UI�̵߳���
            if (this.richtext_outview.InvokeRequired)
            {
                this.richtext_outview.Invoke(new MethodInvoker(() =>
                {
                    this._SendOutTextMsg(text, color);
                }));
            }
            // UI�̵߳���
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
    private static UserList g_Instance                      = null;     // ����
    
    private bool m_bOpenUserList                            = true;     // �Ƿ���ʾ�б�
    private List<Protocol.tagColumnItem> m_HeadList         = null;     // �б�ͷ
    private List<UserModel> m_BodyList                      = null;     // ����б�

    /// <summary> �б�ÿһ�еĸ߶� </summary>
    public int BodyItemHeight { set; get; }

    /// <summary> ����ʵ�� </summary>
    public UnityWinForm.UnityWinForm unity_form { set; get; }

    /// <summary> �Ƿ�������б� </summary>
    public bool OpenUserList
    {
        set
        {
            this.m_bOpenUserList = value;
            this.SetVisible(this.m_bOpenUserList);
        }

        get { return this.m_bOpenUserList; }
    }

    /// <summary> �Ƿ��ӡ��ҽ���������Ϣ </summary>
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
        // ������б���
        if (bBodyClean)
        {
            this.RemoveBodyAll();
        }

        // ������б�ͷ
        if (bHeadClean)
        {
            this.RemoveHeadAll();
        }

        // ���UI��־��
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
        // �����ظ�����ͷ�ֶ�
        for (int i = 0; i < this.m_HeadList.Count; ++i)
        {
            if (this.m_HeadList[i].wDataDescribe == tColumItem.wDataDescribe)
            {
                return;
            }
        }

        this.m_HeadList.Add(tColumItem);
        
        // ���UI
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

                // ����UI
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

        // ����UI
        if (this.unity_form != null)
        {
            this.unity_form.UserGridView_RemoveHeadAll();
        }
    }
    
    public void InsertBody(UserModel user)
    {
        // ��Ҳ����� �� �б�ͷδ���� �򷵻�
        if (user == null || this.HeadListCount <= 0) return;

        UserModel user_self = DataManager.GetInstance().UserSelf;

        // �ȼ��һ��,�����ظ����(��Ҫ��Զ���ظ�����ʱ��Ч)
        for (int i = 0; i < this.m_BodyList.Count; ++i)
        {
            if (this.m_BodyList[i].UserID == user.UserID)
            {
                return;
            }
        }
        
        // �������
        bool bHide = false;
        byte cbUserMasterOrder = user.MasterOrder;
        byte cbUserMemberOrder = user.MemberOrder;

        byte cbSelfMasterOrder = user_self.MasterOrder;
        byte cbSelfMemberOrder = user_self.MemberOrder;

        byte cbBodyMasterOrder = 0;
        byte cbBodyMemberOrder = 0;

        // ����Ȩ�޼�����Լ��ߵ����
        if (user.UserID != user_self.UserID)
        {
            if (cbUserMasterOrder > 0 && cbUserMasterOrder > cbSelfMasterOrder)
            {
                bHide = true;
            }
        }
        if (bHide) return;

        // ����������
        bool bPushBack = true;
        int iIndex = 0;
        for (int i = 0; i < this.m_BodyList.Count; ++i)
        {
            iIndex = i;
            cbBodyMasterOrder = this.m_BodyList[i].MasterOrder;
            cbBodyMemberOrder = this.m_BodyList[i].MemberOrder;

            // 1.���Ȳ������Ա(����)
            if ((cbSelfMasterOrder > cbUserMasterOrder) && cbUserMasterOrder > cbBodyMasterOrder)
            {
                bPushBack = false;
                this.m_BodyList.Insert(i, user);
                break;
            }
            // 2.���Ȳ����Ա�ȼ��ߵ����(����Ա > ��� > ��Ա������ > ��ͨ������)
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
            // 3.����Աģʽ(���Ȳ�����ʵ���)
            else if (cbSelfMasterOrder > 0 && !user.IsRobot && this.m_BodyList[i].IsRobot)
            {
                bPushBack = false;
                this.m_BodyList.Insert(i, user);
                break;
            }

            // 4.���뵽�������֮ǰ
            //else if (this.m_BodyList[i].Status == Define.US_OFFLINE)
            //{
            //    bPushBack = false;
            //    break;
            //}
        }

        // ����������������,�����ĩβ
        if (bPushBack)
        {
            iIndex = this.m_BodyList.Count;
            this.m_BodyList.Insert(iIndex, user);
        }

        // ��һλ��ʾ�Լ�
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

            // ��������һλ
            if (pSelf != null)
            {
                this.m_BodyList.Insert(0, pSelf);
            }
        } while (false);

        DataManager dataMgr = DataManager.GetInstance();
        if (this.OpenPrintUserInOutMsg && dataMgr.SceneInited)
        {
            this.ParseTextMsg(String.Format("\n[0x00FFFFFF]{0}[-] [0xFFFFFFFF]������[-]", user.NickName));
        }

        // ����UI
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
                    this.ParseTextMsg(String.Format("\n[0x00FFFFFF]{0}[-] [0xFFFFFFFF]�뿪�˷���[-]", user.NickName));
                }
                
                // �Ƴ��б�����
                this.m_BodyList.RemoveAt(i);

                // ����UI
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

        // ����UI
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
        // ���²������������Ϣ���Ľ��
        if (user.IsSelf) this.unity_form.SetUserGold(user.GameGold);

        // ���²�������б��еĽ��
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

        // ����������ʽ����ƥ��(��ʽΪ:[argb]text[-])
        System.Text.RegularExpressions.Regex reg = new System.Text.RegularExpressions.Regex(@"\[[0x]*[0-9a-fA-F]+\].*?\[-\]");
        System.Text.RegularExpressions.MatchCollection matches = reg.Matches(text);

        // ����ƥ����ַ���
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