using System.Windows.Forms;

namespace UnityWinForm
{
    partial class UnityWinForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.panel_unity_game = new System.Windows.Forms.Panel();
            this.panel_loading = new System.Windows.Forms.Panel();
            this.pictureBox_loading = new System.Windows.Forms.PictureBox();
            this.pictureBox_loadingText = new System.Windows.Forms.PictureBox();
            this.progressBar_loading = new System.Windows.Forms.ProgressBar();
            this.panel_userinfo = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lab_NickName = new System.Windows.Forms.Label();
            this.lab_GameID = new System.Windows.Forms.Label();
            this.lab_Gold = new System.Windows.Forms.Label();
            this.picbox_HeadPortrait = new System.Windows.Forms.PictureBox();
            this.richtext_outview = new System.Windows.Forms.RichTextBox();
            this.richtext_input = new System.Windows.Forms.RichTextBox();
            this.panel_sidebar = new System.Windows.Forms.Panel();
            this.picbox_chat_clear = new System.Windows.Forms.PictureBox();
            this.btn_send_msg = new System.Windows.Forms.Button();
            this.userlist_view = new System.Windows.Forms.DataGridViewEx();
            this.panel_loading.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_loading)).BeginInit();
            this.pictureBox_loading.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_loadingText)).BeginInit();
            this.panel_userinfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picbox_HeadPortrait)).BeginInit();
            this.panel_sidebar.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picbox_chat_clear)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.userlist_view)).BeginInit();
            this.SuspendLayout();
            // 
            // panel_unity_game
            // 
            this.panel_unity_game.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_unity_game.BackColor = System.Drawing.Color.Transparent;
            this.panel_unity_game.Location = new System.Drawing.Point(0, 0);
            this.panel_unity_game.Name = "panel_unity_game";
            this.panel_unity_game.Size = new System.Drawing.Size(1024, 682);
            this.panel_unity_game.TabIndex = 0;
            this.panel_unity_game.TabStop = true;
            // 
            // panel_loading
            // 
            this.panel_loading.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_loading.Controls.Add(this.pictureBox_loading);
            this.panel_loading.Location = new System.Drawing.Point(0, 0);
            this.panel_loading.Name = "panel_loading";
            this.panel_loading.Size = new System.Drawing.Size(1264, 682);
            this.panel_loading.TabIndex = 0;
            // 
            // pictureBox_loading
            // 
            this.pictureBox_loading.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox_loading.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox_loading.BackgroundImage = Resource.GetInstance().LoadImgFromAssembly("ui.loading_bg.jpg");
            this.pictureBox_loading.Controls.Add(this.pictureBox_loadingText);
            this.pictureBox_loading.Controls.Add(this.progressBar_loading);
            this.pictureBox_loading.Location = new System.Drawing.Point(0, 0);
            this.pictureBox_loading.Name = "pictureBox_loading";
            this.pictureBox_loading.Size = new System.Drawing.Size(1264, 682);
            this.pictureBox_loading.TabIndex = 2;
            this.pictureBox_loading.TabStop = false;
            // 
            // pictureBox_loadingText
            // 
            this.pictureBox_loadingText.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.pictureBox_loadingText.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox_loadingText.Image = Resource.GetInstance().LoadImgFromAssembly("ui.loading_text.png");
            this.pictureBox_loadingText.Location = new System.Drawing.Point(232, 560);
            this.pictureBox_loadingText.Name = "pictureBox_loadingText";
            this.pictureBox_loadingText.Size = new System.Drawing.Size(198, 18);
            this.pictureBox_loadingText.TabIndex = 1;
            this.pictureBox_loadingText.TabStop = false;
            // 
            // progressBar_loading
            // 
            this.progressBar_loading.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar_loading.Location = new System.Drawing.Point(232, 588);
            this.progressBar_loading.Name = "progressBar_loading";
            this.progressBar_loading.Size = new System.Drawing.Size(800, 50);
            this.progressBar_loading.Step = 1;
            this.progressBar_loading.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar_loading.TabIndex = 0;
            // 
            // panel_userinfo
            // 
            this.panel_userinfo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_userinfo.BackColor = System.Drawing.Color.Transparent;
            this.panel_userinfo.Controls.Add(this.label1);
            this.panel_userinfo.Controls.Add(this.label2);
            this.panel_userinfo.Controls.Add(this.label3);
            this.panel_userinfo.Controls.Add(this.lab_NickName);
            this.panel_userinfo.Controls.Add(this.lab_GameID);
            this.panel_userinfo.Controls.Add(this.lab_Gold);
            this.panel_userinfo.Controls.Add(this.picbox_HeadPortrait);
            this.panel_userinfo.Location = new System.Drawing.Point(0, 0);
            this.panel_userinfo.Name = "panel_userinfo";
            this.panel_userinfo.Size = new System.Drawing.Size(240, 88);
            this.panel_userinfo.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(253)))), ((int)(((byte)(209)))), ((int)(((byte)(96)))));
            this.label1.Location = new System.Drawing.Point(96, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "昵称:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(253)))), ((int)(((byte)(209)))), ((int)(((byte)(96)))));
            this.label2.Location = new System.Drawing.Point(96, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(23, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "ID:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(253)))), ((int)(((byte)(209)))), ((int)(((byte)(96)))));
            this.label3.Location = new System.Drawing.Point(96, 66);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "金币:";
            // 
            // lab_NickName
            // 
            this.lab_NickName.AutoEllipsis = true;
            this.lab_NickName.BackColor = System.Drawing.Color.Transparent;
            this.lab_NickName.ForeColor = System.Drawing.Color.White;
            this.lab_NickName.Location = new System.Drawing.Point(134, 13);
            this.lab_NickName.Name = "lab_NickName";
            this.lab_NickName.Size = new System.Drawing.Size(90, 12);
            this.lab_NickName.TabIndex = 2;
            // 
            // lab_GameID
            // 
            this.lab_GameID.AutoEllipsis = true;
            this.lab_GameID.BackColor = System.Drawing.Color.Transparent;
            this.lab_GameID.ForeColor = System.Drawing.Color.White;
            this.lab_GameID.Location = new System.Drawing.Point(134, 40);
            this.lab_GameID.Name = "lab_GameID";
            this.lab_GameID.Size = new System.Drawing.Size(90, 12);
            this.lab_GameID.TabIndex = 5;
            // 
            // lab_Gold
            // 
            this.lab_Gold.AutoEllipsis = true;
            this.lab_Gold.BackColor = System.Drawing.Color.Transparent;
            this.lab_Gold.ForeColor = System.Drawing.Color.White;
            this.lab_Gold.Location = new System.Drawing.Point(134, 66);
            this.lab_Gold.Name = "lab_Gold";
            this.lab_Gold.Size = new System.Drawing.Size(90, 12);
            this.lab_Gold.TabIndex = 6;
            // 
            // picbox_HeadPortrait
            // 
            this.picbox_HeadPortrait.BackColor = System.Drawing.Color.Transparent;
            this.picbox_HeadPortrait.Location = new System.Drawing.Point(5, 4);
            this.picbox_HeadPortrait.Name = "picbox_HeadPortrait";
            this.picbox_HeadPortrait.Size = new System.Drawing.Size(80, 80);
            this.picbox_HeadPortrait.TabIndex = 0;
            this.picbox_HeadPortrait.TabStop = false;
            // 
            // richtext_outview
            // 
            this.richtext_outview.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.richtext_outview.BackColor = UnityWinForm.SiderBarBackColor;
            this.richtext_outview.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richtext_outview.Location = new System.Drawing.Point(0, 456);
            this.richtext_outview.MaxLength = 2048;
            this.richtext_outview.Name = "richtext_outview";
            this.richtext_outview.ReadOnly = true;
            this.richtext_outview.Size = new System.Drawing.Size(240, 175);
            this.richtext_outview.TabIndex = 3;
            this.richtext_outview.Text = "";
            // 
            // richtext_input
            // 
            this.richtext_input.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.richtext_input.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.richtext_input.Font = new System.Drawing.Font("宋体", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.richtext_input.BackColor = System.Drawing.Color.White;
            this.richtext_input.ForeColor = System.Drawing.Color.Black;
            this.richtext_input.Location = new System.Drawing.Point(1, 658);
            this.richtext_input.MaxLength = 128;
            this.richtext_input.Multiline = false;
            this.richtext_input.Name = "richtext_input";
            this.richtext_input.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            this.richtext_input.Size = new System.Drawing.Size(188, 22);
            this.richtext_input.TabIndex = 4;
            this.richtext_input.Text = "";
            this.richtext_input.KeyDown += this.RichtextInput_KeyDown;
            // 
            // panel_sidebar
            // 
            this.panel_sidebar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel_sidebar.BackColor = UnityWinForm.SiderBarBackColor;
            this.panel_sidebar.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.panel_sidebar.Controls.Add(this.picbox_chat_clear);
            this.panel_sidebar.Controls.Add(this.richtext_outview);
            this.panel_sidebar.Controls.Add(this.btn_send_msg);
            this.panel_sidebar.Controls.Add(this.panel_userinfo);
            this.panel_sidebar.Controls.Add(this.richtext_input);
            this.panel_sidebar.Controls.Add(this.userlist_view);
            this.panel_sidebar.Location = new System.Drawing.Point(1024, 0);
            this.panel_sidebar.Name = "panel_sidebar";
            this.panel_sidebar.Size = new System.Drawing.Size(240, 682);
            this.panel_sidebar.TabIndex = 0;
            // 
            // picbox_chat_clear
            // 
            this.picbox_chat_clear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.picbox_chat_clear.BackColor = System.Drawing.Color.Transparent;
            this.picbox_chat_clear.BackgroundImage = Resource.GetInstance().LoadImgFromAssembly("ui.chat_clear.png");
            this.picbox_chat_clear.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.picbox_chat_clear.Location = new System.Drawing.Point(4, 631);
            this.picbox_chat_clear.Margin = new System.Windows.Forms.Padding(0);
            this.picbox_chat_clear.Name = "picbox_chat_clear";
            this.picbox_chat_clear.Size = new System.Drawing.Size(25, 25);
            this.picbox_chat_clear.TabIndex = 7;
            this.picbox_chat_clear.TabStop = false;
            this.picbox_chat_clear.MouseClick += new System.Windows.Forms.MouseEventHandler(this.LabChatClear_MouseClick);
            this.picbox_chat_clear.MouseDown += new System.Windows.Forms.MouseEventHandler(this.LabChatClear_MouseDown);
            this.picbox_chat_clear.MouseLeave += new System.EventHandler(this.LabChatClear_MouseLeave);
            this.picbox_chat_clear.MouseHover += new System.EventHandler(this.LabChatClear_MouseHover);
            this.picbox_chat_clear.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LabChatClear_MouseUp);
            // 
            // btn_send_msg
            // 
            this.btn_send_msg.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_send_msg.BackColor = System.Drawing.Color.Transparent;
            this.btn_send_msg.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.btn_send_msg.ForeColor = System.Drawing.Color.Black;
            //this.btn_send_msg.Image = Resource.GetInstance().LoadImgFromAssembly("common.button.btn_01_01.png");
            this.btn_send_msg.Location = new System.Drawing.Point(190, 656);
            this.btn_send_msg.Name = "btn_send_msg";
            this.btn_send_msg.Size = new System.Drawing.Size(49, 26);
            this.btn_send_msg.TabIndex = 5;
            this.btn_send_msg.Text = "发送";
            this.btn_send_msg.UseVisualStyleBackColor = false;
            this.btn_send_msg.Click += new System.EventHandler(this.SendMsg_BtnClick);
            // 
            // userlist_view
            // 
            this.userlist_view.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.userlist_view.Location = new System.Drawing.Point(0, 90);
            this.userlist_view.Name = "userlist_view";
            this.userlist_view.RowTemplate.Height = 23;
            this.userlist_view.Size = new System.Drawing.Size(240, 365);
            this.userlist_view.TabIndex = 2;
            // 
            // UnityWinForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 682);
            this.Controls.Add(this.panel_loading);
            this.Controls.Add(this.panel_sidebar);
            this.Controls.Add(this.panel_unity_game);
            this.Name = "UnityWinForm";
            this.Text = "ShellForm";
            this.panel_loading.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_loading)).EndInit();
            this.pictureBox_loading.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_loadingText)).EndInit();
            this.panel_userinfo.ResumeLayout(false);
            this.panel_userinfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picbox_HeadPortrait)).EndInit();
            this.panel_sidebar.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picbox_chat_clear)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.userlist_view)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Panel panel_loading;
        private Panel panel_unity_game;
        private Panel panel_sidebar;
        private Panel panel_userinfo;
        private Label label1;
        private Label lab_NickName;
        private Label label2;
        private Label label3;
        private Label lab_GameID;
        private Label lab_Gold;
        private DataGridViewEx userlist_view;
        private RichTextBox richtext_outview;
        private RichTextBox richtext_input;
        private Button btn_send_msg;
        private ProgressBar progressBar_loading;
        private PictureBox pictureBox_loadingText;
        private PictureBox pictureBox_loading;
        private PictureBox picbox_HeadPortrait;
        private PictureBox picbox_chat_clear;
    }
}

