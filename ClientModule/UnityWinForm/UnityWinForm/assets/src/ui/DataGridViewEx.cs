using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace System.Windows.Forms
{
    public class DataGridViewEx : DataGridView
    {
        private Color _BackCellsColor = UnityWinForm.UnityWinForm.SiderBarBackColor;
        private Color _MouseOverColor = Color.FromArgb(0xFF, 0x68, 0x9F, 0xFE);

        public DataGridViewEx()
            : base()
        {
            // 开启双缓冲
            this.SetStyle(ControlStyles.DoubleBuffer | ControlStyles.AllPaintingInWmPaint, true);
        }

        protected override void OnCellMouseEnter(DataGridViewCellEventArgs e)
        {
            base.OnCellMouseEnter(e);

            if (e.RowIndex > -1 && e.RowIndex < this.Rows.Count)
            {
                DataGridViewRow row = this.Rows[e.RowIndex];
                for (int i = 0; i < row.Cells.Count; ++i)
                {
                    row.Cells[i].Style.BackColor = this._MouseOverColor;
                }
            }
        }

        protected override void OnCellMouseLeave(DataGridViewCellEventArgs e)
        {
            base.OnCellMouseLeave(e);

            if (e.RowIndex > -1 && e.RowIndex < this.Rows.Count)
            {
                DataGridViewRow row = this.Rows[e.RowIndex];
                for (int i = 0; i < row.Cells.Count; ++i)
                {
                    row.Cells[i].Style.BackColor = this._BackCellsColor;
                }
            }
        }

        //protected override void PaintBackground(Graphics graphics, Rectangle clipBounds, Rectangle gridBounds)
        //{
        //    Image bg_Img = global::UnityWinForm.Properties.Resources.siderbar_bg;
        //    Util.Scale9Sprite(graphics, bg_Img, this.ClientRectangle, new Rectangle(4, 4, 4, 4));
        //}
    }

    // 加载自定义滚动条示例
    //public class DataGridViewEx : DataGridView
    //{
    //    private Color _BeforInCellsColor = Color.White;
    //    private Color _MouseOverHighColor = Color.FromArgb(0xFF, 0x68, 0x9F, 0xFE);

    //    // ------------------------------------------------------
    //    private int _displayRowCount = 0;

    //    private int _displayWidth = 0;

    //    //private ScrollBarEx.HScrollBarEx _hScrollBarEx;

    //    //private ScrollBarEx.VScrollBarEx _vScrollBarEx;

    //    private CustomControls.CustomScrollbar _hScrollBarEx;

    //    private CustomControls.CustomScrollbar _vScrollBarEx;

    //    private bool dgvScroll = false;

    //    public DataGridViewEx()
    //        : base()
    //    {
    //        _hScrollBarEx = new CustomControls.CustomScrollbar();

    //        _vScrollBarEx = new CustomControls.CustomScrollbar();

    //        this.Controls.Add(_hScrollBarEx);

    //        this.Controls.Add(_vScrollBarEx);

    //        base.SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);

    //        this.HorizontalScrollBar.VisibleChanged += new EventHandler(ScrollBar_VisibleChanged);

    //        this.VerticalScrollBar.VisibleChanged += new EventHandler(ScrollBar_VisibleChanged);

    //        this.SizeChanged += new EventHandler(ScrollBar_VisibleChanged);

    //        this._vScrollBarEx.ValueChanged += new EventHandler(VScrollBarEx_ValueChanged);

    //        this._hScrollBarEx.ValueChanged += new EventHandler(HScrollBarEx_ValueChanged);

    //        this.Scroll += new ScrollEventHandler(DataGridViewEx_Scroll);

    //        this.ColumnHeadersHeightChanged += new EventHandler(ScrollBar_VisibleChanged);

    //        this.ColumnWidthChanged += new DataGridViewColumnEventHandler(ScrollBar_VisibleChanged);

    //        this.RowHeadersWidthChanged += new EventHandler(ScrollBar_VisibleChanged);

    //        this.RowHeightChanged += new DataGridViewRowEventHandler(ScrollBar_VisibleChanged);

    //        this.RowsAdded += new DataGridViewRowsAddedEventHandler(ScrollBar_VisibleChanged);

    //        this.RowsRemoved += new DataGridViewRowsRemovedEventHandler(ScrollBar_VisibleChanged);

    //        this.ColumnAdded += new DataGridViewColumnEventHandler(ScrollBar_VisibleChanged);

    //        this.ColumnRemoved += new DataGridViewColumnEventHandler(ScrollBar_VisibleChanged);

    //        this.DataSourceChanged += new EventHandler(ScrollBar_VisibleChanged);

    //        SetScrollBarEx();

    //    }

    //    public bool m_bMouseEnter = false;
    //    public Rectangle m_rcCell = new Rectangle();
    //    public int m_iCurRowIndex = 0;

    //    protected override void OnCellMouseEnter(DataGridViewCellEventArgs e)
    //    {
    //        base.OnCellMouseEnter(e);
            
    //        if (e.RowIndex > -1 && e.RowIndex < this.Rows.Count)
    //        {
    //            DataGridViewRow row = this.Rows[e.RowIndex];
    //            for (int i = 0; i < row.Cells.Count; ++i)
    //            {
    //                this._BeforInCellsColor = row.Cells[i].Style.BackColor;
    //                row.Cells[i].Style.BackColor = this._MouseOverHighColor;
    //            }
    //        }
    //    }

    //    protected override void OnCellMouseLeave(DataGridViewCellEventArgs e)
    //    {
    //        base.OnCellMouseLeave(e);

    //        if (e.RowIndex > -1 && e.RowIndex < this.Rows.Count)
    //        {
    //            DataGridViewRow row = this.Rows[e.RowIndex];
    //            for (int i = 0; i < row.Cells.Count; ++i)
    //            {
    //                row.Cells[i].Style.BackColor = this._BeforInCellsColor;
    //            }
    //        }
    //    }

    //    protected override void PaintBackground(Graphics graphics, Rectangle clipBounds, Rectangle gridBounds)
    //    {
    //        Image bg_Img = global::UnityWinForm.Properties.Resources.siderbar_bg;
    //        Util.Scale9Sprite(graphics, bg_Img, this.ClientRectangle, new Rectangle(4, 4, 4, 4));
    //    }

    //    private void VScrollBarEx_ValueChanged(object sender, EventArgs e)
    //    {
    //        if (!this.dgvScroll)
    //        {

    //            this.FirstDisplayedScrollingRowIndex = _vScrollBarEx.Value;

    //            Application.DoEvents();

    //        }
    //    }
        
    //    private void HScrollBarEx_ValueChanged(object sender, EventArgs e)
    //    {
    //        if (!this.dgvScroll)
    //        {

    //            this.HorizontalScrollingOffset = _hScrollBarEx.Value;

    //            GetDisplayWidth();

    //            Application.DoEvents();

    //        }
    //    }
        
    //    private void DataGridViewEx_Scroll(object sender, ScrollEventArgs e)
    //    {
    //        this.dgvScroll = true;

    //        if (e.ScrollOrientation == ScrollOrientation.VerticalScroll)
    //        {
    //            _vScrollBarEx.Value = this.FirstDisplayedScrollingRowIndex;
    //        }
    //        else
    //        {
    //            _hScrollBarEx.Value = this.HorizontalScrollingOffset;
    //        }

    //        this.dgvScroll = false;
    //    }
        
    //    private void ScrollBar_VisibleChanged(object sender, EventArgs e)
    //    {

    //        SetScrollBarEx();

    //    }



    //    private void SetScrollBarEx()
    //    {

    //        if (this.VerticalScrollBar.Visible)
    //        {

    //            _vScrollBarEx.Visible = true;

    //            _vScrollBarEx.Location = new Point(this.DisplayRectangle.Width - 10, 0);

    //            this.VerticalScrollBar.SendToBack();

    //            _vScrollBarEx.Height = this.DisplayRectangle.Height;



    //            GetDisplayRowCount();

    //        }

    //        else
    //        {

    //            _vScrollBarEx.Visible = false;

    //        }



    //        if (this.HorizontalScrollBar.Visible)
    //        {

    //            _hScrollBarEx.Visible = true;

    //            _hScrollBarEx.Location = new Point(0, this.DisplayRectangle.Height);

    //            this.HorizontalScrollBar.SendToBack();

    //            _hScrollBarEx.Width = this.DisplayRectangle.Width;



    //            GetDisplayWidth();

    //            _hScrollBarEx.Value = this.HorizontalScrollingOffset;

    //        }

    //        else
    //        {

    //            _hScrollBarEx.Visible = false;

    //        }

    //    }



    //    public int GetDisplayWidth()
    //    {

    //        int width = 0;

    //        for (int i = 0; i < this.Columns.Count; i++)
    //        {

    //            width += this.Columns[i].Width;

    //        }

    //        _displayWidth = width;

    //        this._hScrollBarEx.Maximum = width;

    //        this._hScrollBarEx.LargeChange = _hScrollBarEx.Maximum / _hScrollBarEx.Width + this.DisplayRectangle.Width - this.RowHeadersWidth;

    //        return width;

    //    }



    //    public int GetDisplayRowCount()
    //    {

    //        int j = 0;

    //        int height = 0;

    //        int i = this.FirstDisplayedScrollingRowIndex;

    //        if (i < 0)
    //        {

    //            i = 0;

    //        }

    //        for (; i < this.Rows.Count; i++)
    //        {

    //            height += this.Rows[i].Height;

    //            if (height < this.DisplayRectangle.Height - this.ColumnHeadersHeight)
    //            {

    //                j++;

    //            }

    //            else
    //            {

    //                break;

    //            }



    //        }

    //        j = this.Rows.Count - j;

    //        if (j < 0)
    //        {

    //            j = 0;

    //        }

    //        if (_displayRowCount != j)
    //        {

    //            _displayRowCount = j;

    //            _vScrollBarEx.Maximum = j + _vScrollBarEx.Minimum + _vScrollBarEx.LargeChange - 1;

    //            if (this.FirstDisplayedScrollingRowIndex < 0)
    //            {

    //                _vScrollBarEx.Value = 0;

    //            }

    //            else if (this.FirstDisplayedScrollingRowIndex > _vScrollBarEx.Maximum)
    //            {

    //                _vScrollBarEx.Value = _vScrollBarEx.Maximum;

    //            }

    //            else
    //            {

    //                _vScrollBarEx.Value = this.FirstDisplayedScrollingRowIndex;

    //            }

    //        }

    //        return j;

    //    }

    //}
}