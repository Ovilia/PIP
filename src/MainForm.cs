using System;
using System.Windows.Forms;

namespace PIP
{
  class MainForm : Form
  {
    private WindowManager windowManager;

    private ToolStripMenuItem filesToolStripMenuItem;
    private ToolStripMenuItem editToolStripMenuItem;
    private ToolStripMenuItem windowToolStripMenuItem;
    private ToolStripMenuItem helpToolStripMenuItem;
    private ToolStripMenuItem aboutToolStripMenuItem;
    private ToolStripMenuItem testWindowToolStripMenuItem;
    private MenuStrip menuStrip;
  
    /// <summary>
    /// Constructor
    /// </summary>
    public MainForm()
    {
      windowManager = new WindowManager();

      Text = "PIP Image Processor";
      this.AutoSize = true;

      InitializeComponent();
    }

    public static void Main()
    {
      MainForm mainForm = new MainForm();
      Application.Run(mainForm);
    }

    private void InitializeComponent()
    {
      this.menuStrip = new System.Windows.Forms.MenuStrip();
      this.filesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.testWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
      this.filesToolStripMenuItem.Name = "filesToolStripMenuItem";
      this.filesToolStripMenuItem.Size = new System.Drawing.Size(45, 21);
      this.filesToolStripMenuItem.Text = "Files";
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
            this.testWindowToolStripMenuItem});
      this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
      this.windowToolStripMenuItem.Size = new System.Drawing.Size(67, 21);
      this.windowToolStripMenuItem.Text = "Window";
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
      // testWindowToolStripMenuItem
      // 
      this.testWindowToolStripMenuItem.Name = "testWindowToolStripMenuItem";
      this.testWindowToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.testWindowToolStripMenuItem.Text = "test window";
      this.testWindowToolStripMenuItem.Click += new System.EventHandler(this.testWindowToolStripMenuItem_Click);
      // 
      // MainForm
      // 
      this.ClientSize = new System.Drawing.Size(284, 262);
      this.Controls.Add(this.menuStrip);
      this.MainMenuStrip = this.menuStrip;
      this.Name = "MainForm";
      this.menuStrip.ResumeLayout(false);
      this.menuStrip.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    private void testWindowToolStripMenuItem_Click(object sender, EventArgs e)
    {
      windowManager.addWindow(new Window());
    }

  }
}
