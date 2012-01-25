using System;
using System.Drawing;
using System.Windows.Forms;

namespace PIP
{
  class MainForm : Form
  {
    [STAThread]
    public static void Main()
    {
      MainForm mainForm = new MainForm();
      Application.Run(mainForm);
    }

    public static WindowManager windowManager;

    private ToolStripMenuItem filesToolStripMenuItem;
    private ToolStripMenuItem editToolStripMenuItem;
    private ToolStripMenuItem windowToolStripMenuItem;
    private ToolStripMenuItem helpToolStripMenuItem;
    private ToolStripMenuItem aboutToolStripMenuItem;
    private ToolStripMenuItem histogramWindowToolStripMenuItem;
    private ToolStripMenuItem openToolStripMenuItem;
    private ToolStripMenuItem saveToolStripMenuItem;
    private ToolStripSeparator toolStripSeparator1;
    private ToolStripMenuItem exitToolStripMenuItem;
    private OpenFileDialog openFileDialog;
    private ToolStripMenuItem grayScaleWindowToolStripMenuItem;
    private ToolStripMenuItem convolutionAndImageFilterWindowToolStripMenuItem;
    private MenuStrip menuStrip;
  
    /// <summary>
    /// Constructor
    /// </summary>
    public MainForm()
    {
      InitializeComponent();
      this.Height = 0;
      this.Width = SystemInformation.VirtualScreen.Width;

      windowManager = new WindowManager();
      windowManager.showAllWindows();
    }

    private void InitializeComponent()
    {
      this.menuStrip = new System.Windows.Forms.MenuStrip();
      this.filesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.histogramWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.convolutionAndImageFilterWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.grayScaleWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
      this.menuStrip.SuspendLayout();
      this.SuspendLayout();
      // 
      // menuStrip
      // 
      this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filesToolStripMenuItem,
            this.editToolStripMenuItem,
            this.windowToolStripMenuItem,
            this.helpToolStripMenuItem});
      this.menuStrip.Location = new System.Drawing.Point(0, 0);
      this.menuStrip.Name = "menuStrip";
      this.menuStrip.Size = new System.Drawing.Size(284, 25);
      this.menuStrip.TabIndex = 0;
      this.menuStrip.Text = "menuStrip";
      // 
      // filesToolStripMenuItem
      // 
      this.filesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
      this.filesToolStripMenuItem.Name = "filesToolStripMenuItem";
      this.filesToolStripMenuItem.Size = new System.Drawing.Size(45, 21);
      this.filesToolStripMenuItem.Text = "Files";
      // 
      // openToolStripMenuItem
      // 
      this.openToolStripMenuItem.Name = "openToolStripMenuItem";
      this.openToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
      this.openToolStripMenuItem.Text = "Open";
      this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
      // 
      // saveToolStripMenuItem
      // 
      this.saveToolStripMenuItem.Enabled = false;
      this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
      this.saveToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
      this.saveToolStripMenuItem.Text = "Save";
      // 
      // toolStripSeparator1
      // 
      this.toolStripSeparator1.Name = "toolStripSeparator1";
      this.toolStripSeparator1.Size = new System.Drawing.Size(105, 6);
      // 
      // exitToolStripMenuItem
      // 
      this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
      this.exitToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
      this.exitToolStripMenuItem.Text = "Exit";
      this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
      // 
      // editToolStripMenuItem
      // 
      this.editToolStripMenuItem.Name = "editToolStripMenuItem";
      this.editToolStripMenuItem.Size = new System.Drawing.Size(42, 21);
      this.editToolStripMenuItem.Text = "Edit";
      // 
      // windowToolStripMenuItem
      // 
      this.windowToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.histogramWindowToolStripMenuItem,
            this.convolutionAndImageFilterWindowToolStripMenuItem,
            this.grayScaleWindowToolStripMenuItem});
      this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
      this.windowToolStripMenuItem.Size = new System.Drawing.Size(67, 21);
      this.windowToolStripMenuItem.Text = "Window";
      this.windowToolStripMenuItem.Click += new System.EventHandler(this.windowToolStripMenuItem_Click);
      // 
      // histogramWindowToolStripMenuItem
      // 
      this.histogramWindowToolStripMenuItem.CheckOnClick = true;
      this.histogramWindowToolStripMenuItem.Enabled = false;
      this.histogramWindowToolStripMenuItem.Name = "histogramWindowToolStripMenuItem";
      this.histogramWindowToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
      this.histogramWindowToolStripMenuItem.Text = "Histogram window";
      this.histogramWindowToolStripMenuItem.Click += new System.EventHandler(this.histogramWindowToolStripMenuItem_Click);
      // 
      // convolutionAndImageFilterWindowToolStripMenuItem
      // 
      this.convolutionAndImageFilterWindowToolStripMenuItem.CheckOnClick = true;
      this.convolutionAndImageFilterWindowToolStripMenuItem.Enabled = false;
      this.convolutionAndImageFilterWindowToolStripMenuItem.Name = "convolutionAndImageFilterWindowToolStripMenuItem";
      this.convolutionAndImageFilterWindowToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
      this.convolutionAndImageFilterWindowToolStripMenuItem.Text = "Convolution and image filter window";
      this.convolutionAndImageFilterWindowToolStripMenuItem.Click += new System.EventHandler(this.convolutionAndImageFilterWindowToolStripMenuItem_Click);
      // 
      // grayScaleWindowToolStripMenuItem
      // 
      this.grayScaleWindowToolStripMenuItem.Enabled = false;
      this.grayScaleWindowToolStripMenuItem.Name = "grayScaleWindowToolStripMenuItem";
      this.grayScaleWindowToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
      this.grayScaleWindowToolStripMenuItem.Text = "Gray scale window";
      this.grayScaleWindowToolStripMenuItem.Click += new System.EventHandler(this.grayScaleWindowToolStripMenuItem_Click);
      // 
      // helpToolStripMenuItem
      // 
      this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
      this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
      this.helpToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
      this.helpToolStripMenuItem.Text = "Help";
      // 
      // aboutToolStripMenuItem
      // 
      this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
      this.aboutToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
      this.aboutToolStripMenuItem.Text = "About";
      // 
      // openFileDialog
      // 
      this.openFileDialog.Filter = "Image Files(*.jpg; *.jpeg; *.gif; *.bmp)|*.png; *.jpg; *.jpeg; *.gif; *.bmp";
      this.openFileDialog.Title = "Open an image file";
      // 
      // MainForm
      // 
      this.AutoSize = true;
      this.ClientSize = new System.Drawing.Size(284, 262);
      this.Controls.Add(this.menuStrip);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
      this.MainMenuStrip = this.menuStrip;
      this.Name = "MainForm";
      this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
      this.Text = "PIP Image Processor";
      this.menuStrip.ResumeLayout(false);
      this.menuStrip.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }
    
    private void histogramWindowToolStripMenuItem_Click(object sender, EventArgs e)
    {
      windowManager.getHistogramWindow().Visible =
        histogramWindowToolStripMenuItem.Checked;
    }

    private void exitToolStripMenuItem_Click(object sender, EventArgs e)
    {
      this.Dispose();
    }

    private void openToolStripMenuItem_Click(object sender, EventArgs e)
    {
      DialogResult dialogResult = openFileDialog.ShowDialog();
      if (dialogResult == DialogResult.OK)
      {
        string fileName = openFileDialog.FileName;
        ImageWindow imageWindow = new ImageWindow(fileName);
        windowManager.addImageWindow(imageWindow);
      }
    }

    private void grayScaleWindowToolStripMenuItem_Click(object sender, EventArgs e)
    {
      ImageProcessor imageProcesser = windowManager.getFocusedImageWindow().imageProcessor;
      ImageWindow window = new ImageWindow(imageProcesser.getGrayScaleBitmap());
      window.Text = "Gray scale image";
      windowManager.addImageWindow(window);
    }

    private void windowToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if (windowManager.isImageWindowEmpty() != true)
      {
        this.histogramWindowToolStripMenuItem.Enabled = true;
        this.grayScaleWindowToolStripMenuItem.Enabled = true;
        this.convolutionAndImageFilterWindowToolStripMenuItem.Enabled = true;
        histogramWindowToolStripMenuItem.Checked =
          windowManager.getHistogramWindow().Visible;
        convolutionAndImageFilterWindowToolStripMenuItem.Checked =
          windowManager.getFilterWindow().Visible;
      }
      else
      {
        this.histogramWindowToolStripMenuItem.Enabled = false;
        this.grayScaleWindowToolStripMenuItem.Enabled = false;
        this.convolutionAndImageFilterWindowToolStripMenuItem.Enabled = false;
        this.histogramWindowToolStripMenuItem.Checked = false;
        this.grayScaleWindowToolStripMenuItem.Checked = false;
        this.convolutionAndImageFilterWindowToolStripMenuItem.Checked = false;
        windowManager.disposeAllWindows();
      }
    }

    private void convolutionAndImageFilterWindowToolStripMenuItem_Click(object sender, EventArgs e)
    {
      windowManager.getFilterWindow().Visible = 
        convolutionAndImageFilterWindowToolStripMenuItem.Checked;
    }
  }
}
