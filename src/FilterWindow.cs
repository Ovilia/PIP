using System;
using System.Windows.Forms;

namespace PIP
{
  class FilterWindow : Form
  {
    RobertsOperator robertsOperator;
    PrewittOperator prewittOperator;

    private Label filterLabel;
    private Label convolutionLabel;
    private RadioButton sobelButton;
    private RadioButton prewittButton;
    private RadioButton robertsRadioButton;
    private RadioButton medianButton;
    private RadioButton gaussianButton;
    private Button applyButton;
    private TextBox textBox1;
    private Label ratioLabel;
    private GroupBox groupBox;

  
    public FilterWindow()
    {
      InitializeComponent();
    }

    private void InitializeComponent()
    {
      this.groupBox = new System.Windows.Forms.GroupBox();
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.ratioLabel = new System.Windows.Forms.Label();
      this.medianButton = new System.Windows.Forms.RadioButton();
      this.gaussianButton = new System.Windows.Forms.RadioButton();
      this.filterLabel = new System.Windows.Forms.Label();
      this.convolutionLabel = new System.Windows.Forms.Label();
      this.sobelButton = new System.Windows.Forms.RadioButton();
      this.prewittButton = new System.Windows.Forms.RadioButton();
      this.robertsRadioButton = new System.Windows.Forms.RadioButton();
      this.applyButton = new System.Windows.Forms.Button();
      this.groupBox.SuspendLayout();
      this.SuspendLayout();
      // 
      // groupBox
      // 
      this.groupBox.Controls.Add(this.textBox1);
      this.groupBox.Controls.Add(this.ratioLabel);
      this.groupBox.Controls.Add(this.medianButton);
      this.groupBox.Controls.Add(this.gaussianButton);
      this.groupBox.Controls.Add(this.filterLabel);
      this.groupBox.Controls.Add(this.convolutionLabel);
      this.groupBox.Controls.Add(this.sobelButton);
      this.groupBox.Controls.Add(this.prewittButton);
      this.groupBox.Controls.Add(this.robertsRadioButton);
      this.groupBox.Location = new System.Drawing.Point(12, 12);
      this.groupBox.Name = "groupBox";
      this.groupBox.Size = new System.Drawing.Size(306, 105);
      this.groupBox.TabIndex = 0;
      this.groupBox.TabStop = false;
      // 
      // textBox1
      // 
      this.textBox1.Enabled = false;
      this.textBox1.Location = new System.Drawing.Point(217, 57);
      this.textBox1.Name = "textBox1";
      this.textBox1.Size = new System.Drawing.Size(44, 21);
      this.textBox1.TabIndex = 8;
      this.textBox1.Text = "1";
      this.textBox1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // ratioLabel
      // 
      this.ratioLabel.AutoSize = true;
      this.ratioLabel.Location = new System.Drawing.Point(176, 60);
      this.ratioLabel.Name = "ratioLabel";
      this.ratioLabel.Size = new System.Drawing.Size(47, 12);
      this.ratioLabel.TabIndex = 7;
      this.ratioLabel.Text = "ratio: ";
      // 
      // medianButton
      // 
      this.medianButton.AutoSize = true;
      this.medianButton.Location = new System.Drawing.Point(160, 80);
      this.medianButton.Name = "medianButton";
      this.medianButton.Size = new System.Drawing.Size(101, 16);
      this.medianButton.TabIndex = 6;
      this.medianButton.TabStop = true;
      this.medianButton.Text = "Median filter";
      this.medianButton.UseVisualStyleBackColor = true;
      // 
      // gaussianButton
      // 
      this.gaussianButton.AutoSize = true;
      this.gaussianButton.Location = new System.Drawing.Point(160, 36);
      this.gaussianButton.Name = "gaussianButton";
      this.gaussianButton.Size = new System.Drawing.Size(113, 16);
      this.gaussianButton.TabIndex = 5;
      this.gaussianButton.TabStop = true;
      this.gaussianButton.Text = "Gaussian filter";
      this.gaussianButton.UseVisualStyleBackColor = true;
      // 
      // filterLabel
      // 
      this.filterLabel.AutoSize = true;
      this.filterLabel.Location = new System.Drawing.Point(149, 17);
      this.filterLabel.Name = "filterLabel";
      this.filterLabel.Size = new System.Drawing.Size(41, 12);
      this.filterLabel.TabIndex = 4;
      this.filterLabel.Text = "Filter";
      // 
      // convolutionLabel
      // 
      this.convolutionLabel.AutoSize = true;
      this.convolutionLabel.Location = new System.Drawing.Point(6, 17);
      this.convolutionLabel.Name = "convolutionLabel";
      this.convolutionLabel.Size = new System.Drawing.Size(71, 12);
      this.convolutionLabel.TabIndex = 3;
      this.convolutionLabel.Text = "Convolution";
      // 
      // sobelButton
      // 
      this.sobelButton.AutoSize = true;
      this.sobelButton.Location = new System.Drawing.Point(19, 80);
      this.sobelButton.Name = "sobelButton";
      this.sobelButton.Size = new System.Drawing.Size(107, 16);
      this.sobelButton.TabIndex = 2;
      this.sobelButton.TabStop = true;
      this.sobelButton.Text = "Sobel operator";
      this.sobelButton.UseVisualStyleBackColor = true;
      // 
      // prewittButton
      // 
      this.prewittButton.AutoSize = true;
      this.prewittButton.Location = new System.Drawing.Point(19, 58);
      this.prewittButton.Name = "prewittButton";
      this.prewittButton.Size = new System.Drawing.Size(119, 16);
      this.prewittButton.TabIndex = 1;
      this.prewittButton.TabStop = true;
      this.prewittButton.Text = "Prewitt operator";
      this.prewittButton.UseVisualStyleBackColor = true;
      this.prewittButton.CheckedChanged += new System.EventHandler(this.prewittButton_CheckedChanged);
      // 
      // robertsRadioButton
      // 
      this.robertsRadioButton.AutoSize = true;
      this.robertsRadioButton.Location = new System.Drawing.Point(19, 36);
      this.robertsRadioButton.Name = "robertsRadioButton";
      this.robertsRadioButton.Size = new System.Drawing.Size(119, 16);
      this.robertsRadioButton.TabIndex = 0;
      this.robertsRadioButton.TabStop = true;
      this.robertsRadioButton.Text = "Roberts operator";
      this.robertsRadioButton.UseVisualStyleBackColor = true;
      this.robertsRadioButton.CheckedChanged += new System.EventHandler(this.robertsRadioButton_CheckedChanged);
      // 
      // applyButton
      // 
      this.applyButton.Location = new System.Drawing.Point(243, 123);
      this.applyButton.Name = "applyButton";
      this.applyButton.Size = new System.Drawing.Size(75, 23);
      this.applyButton.TabIndex = 1;
      this.applyButton.Text = "Apply";
      this.applyButton.UseVisualStyleBackColor = true;
      // 
      // FilterWindow
      // 
      this.ClientSize = new System.Drawing.Size(330, 157);
      this.Controls.Add(this.applyButton);
      this.Controls.Add(this.groupBox);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
      this.Name = "FilterWindow";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
      this.Text = "Convolution and Image Filters Window";
      this.groupBox.ResumeLayout(false);
      this.groupBox.PerformLayout();
      this.ResumeLayout(false);

    }

    private void robertsRadioButton_CheckedChanged(object sender, EventArgs e)
    {
      if (robertsRadioButton.Checked)
      {
        if (robertsOperator == null)
        {
          robertsOperator = new RobertsOperator(
            MainForm.windowManager.getFocusedImageWindow().imageProcessor);
        }
        MainForm.windowManager.addImageWindow(
          new ImageWindow(robertsOperator.FilteredBitmap));
      }
    }

    private void prewittButton_CheckedChanged(object sender, EventArgs e)
    {
      if (prewittButton.Checked)
      {
        if (prewittOperator == null)
        {
          prewittOperator = new PrewittOperator(
            MainForm.windowManager.getFocusedImageWindow().imageProcessor);
        }
        MainForm.windowManager.addImageWindow(
          new ImageWindow(prewittOperator.FilteredBitmap));
      }
    }
  }
}
