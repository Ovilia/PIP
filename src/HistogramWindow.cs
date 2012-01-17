using System;
using System.Drawing;
using System.Windows.Forms;

namespace PIP
{
  class HistogramWindow : Form
  {
    private SplitContainer splitContainer;
    private TrackBar thresholdTrackBar;
    private TextBox thresholdTextBox;
    private System.Windows.Forms.DataVisualization.Charting.Chart histogramChart;
    private System.Windows.Forms.DataVisualization.Charting.Series histogramSeries;
    private System.Windows.Forms.DataVisualization.Charting.Series redSeries;
    private System.Windows.Forms.DataVisualization.Charting.Series greenSeries;
    private System.Windows.Forms.DataVisualization.Charting.Series blueSeries;
    private System.Windows.Forms.DataVisualization.Charting.Series thresholdSeries;
    private CheckBox rgbCheckBox;
    private CheckBox logCheckBox;
    private Label thresholdLabel_128;
    private Label thresholdLabel_255;
    private Label thresholdLabel_0;

    private int[] histogram;
    private int[,] rgbHistogram;

    private int maxHistogram;
    private Label thresholdLabel;
    private GroupBox thresholdGroupBox;
    private RadioButton entropyRadioButton;
    private RadioButton otsuRadioButton;
    private RadioButton customRadioButton;
    private CheckBox originCheckBox;
    private int maxRGBHistogram;

    public HistogramWindow()
    {
      InitializeComponent();
      updateImage();
    }

    /// <summary>
    /// Init histogram
    /// </summary>
    public void initHistogram()
    {
      histogram = MainForm.windowManager.getFocusedImageWindow().imageProcessor.getHistogram();
      maxHistogram = MainForm.windowManager.getFocusedImageWindow().imageProcessor.getMaxHistogram();
    }

    /// <summary>
    /// Get threshold value
    /// </summary>
    /// <returns>Threshold value</returns>
    public int getThresholdValue()
    {
      return thresholdTrackBar.Value;
    }

    /// <summary>
    /// Init RGB histogram
    /// </summary>
    public void initRgbHistogram()
    {
      rgbHistogram = MainForm.windowManager.getFocusedImageWindow().imageProcessor.getRgbHistogram();
      maxRGBHistogram = MainForm.windowManager.getFocusedImageWindow().imageProcessor.getMaxRgbHistogram();
    }

    /// <summary>
    /// Redraw histogram chart
    /// </summary>
    public void updateGraySeries()
    {
      initHistogram();

      histogramSeries = new System.Windows.Forms.DataVisualization.Charting.Series();
      histogramSeries.Name = "Gray Histogram";
      histogramSeries.Color = Color.Gray;

      for (int i = 0; i < ImageProcessor.RANGE_OF_8BITS; ++i)
      {
        histogramSeries.Points.Add(histogram[i]);
      }

      this.histogramChart.Series.Clear();
      this.histogramChart.Series.Add(histogramSeries);
      this.histogramChart.ChartAreas[0].AxisY.Maximum = maxHistogram;
      this.histogramChart.ChartAreas[0].AxisY.Title = "Gray Histogram";
      this.histogramChart.ChartAreas[0].AxisX.Maximum = ImageProcessor.RANGE_OF_8BITS - 1;
      this.histogramChart.ChartAreas[0].AxisX.Minimum = 0;
      this.histogramChart.ChartAreas[0].AxisX.Interval = ImageProcessor.RANGE_OF_8BITS / 8;

      updateThresholdSeries();
    }

    /// <summary>
    /// Redraw RGB Histogram
    /// </summary>
    public void updateRgbSeries()
    {
      if (redSeries == null || greenSeries == null || blueSeries == null)
      {
        initRgbHistogram();

        redSeries = new System.Windows.Forms.DataVisualization.Charting.Series();
        redSeries.Name = "Red Histogram";
        redSeries.Color = Color.Red;
        redSeries.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;

        greenSeries = new System.Windows.Forms.DataVisualization.Charting.Series();
        greenSeries.Name = "Green Histogram";
        greenSeries.Color = Color.Green;
        greenSeries.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;

        blueSeries = new System.Windows.Forms.DataVisualization.Charting.Series();
        blueSeries.Name = "Blue Histogram";
        blueSeries.Color = Color.Blue;
        blueSeries.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;

        for (int i = 0; i < ImageProcessor.RANGE_OF_8BITS; ++i)
        {
          redSeries.Points.AddXY(i, rgbHistogram[i, 0]);
          greenSeries.Points.AddXY(i, rgbHistogram[i, 1]);
          blueSeries.Points.AddXY(i, rgbHistogram[i, 2]);
        }
      }

      this.histogramChart.Series.Add(redSeries);
      this.histogramChart.Series.Add(greenSeries);
      this.histogramChart.Series.Add(blueSeries);

      redSeries.YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
      greenSeries.YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
      blueSeries.YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
      this.histogramChart.ChartAreas[0].AxisY2.Maximum = maxRGBHistogram;
      this.histogramChart.ChartAreas[0].AxisY2.Title = "RGB histogram";
    }

    /// <summary>
    /// Update threshold series
    /// </summary>
    public void updateThresholdSeries()
    {
      if (thresholdSeries == null)
      {
        thresholdSeries = new System.Windows.Forms.DataVisualization.Charting.Series();
        thresholdSeries.Name = "Threshold value";
        thresholdSeries.Color = Color.DarkOrange;
        thresholdSeries.BorderWidth = 2;
        thresholdSeries.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
        this.histogramChart.Series.Add(thresholdSeries);
      }
      else
      {
        // Remove former threshold value
        thresholdSeries.Points.RemoveAt(0);
        thresholdSeries.Points.RemoveAt(0);
      }
      if (!this.histogramChart.Series.Contains(thresholdSeries))
      {
        this.histogramChart.Series.Add(thresholdSeries);
      }
      thresholdSeries.Points.AddXY(thresholdTrackBar.Value, 0);
      thresholdSeries.Points.AddXY(thresholdTrackBar.Value, maxHistogram);
    }

    public void updateBinaryBitmap()
    {
      ImageWindow imageWindow = MainForm.windowManager.getFocusedImageWindow();
      imageWindow.updateImage(ImageProcessor.ImageType.BINARY_IMAGE);
    }

    /// <summary>
    /// Update histogram or RGB histogram using or not using log
    /// </summary>
    /// <param name="isLog">If to use log</param>
    public void updateLogAxis(bool isLog)
    {
      this.histogramChart.ChartAreas[0].AxisY.LogarithmBase = 2.0;
      this.histogramChart.ChartAreas[0].AxisY.IsLogarithmic = isLog;
    }

    /// <summary>
    /// Remove RGB Histogram
    /// </summary>
    public void removeRgbSeries()
    {
      this.histogramChart.Series.Remove(redSeries);
      this.histogramChart.Series.Remove(greenSeries);
      this.histogramChart.Series.Remove(blueSeries); 
    }

    /// <summary>
    /// Called when new ImageWindow is focused
    /// </summary>
    public void updateImage()
    {
      rgbCheckBox.Checked = false;
      customRadioButton.Checked = true;
      thresholdTextBox.Text = "0";
      thresholdTrackBar.Value = 0;
      updateGraySeries();
    }

    private void InitializeComponent()
    {
      System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
      System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
      this.splitContainer = new System.Windows.Forms.SplitContainer();
      this.originCheckBox = new System.Windows.Forms.CheckBox();
      this.logCheckBox = new System.Windows.Forms.CheckBox();
      this.rgbCheckBox = new System.Windows.Forms.CheckBox();
      this.thresholdGroupBox = new System.Windows.Forms.GroupBox();
      this.customRadioButton = new System.Windows.Forms.RadioButton();
      this.entropyRadioButton = new System.Windows.Forms.RadioButton();
      this.otsuRadioButton = new System.Windows.Forms.RadioButton();
      this.thresholdLabel_255 = new System.Windows.Forms.Label();
      this.thresholdLabel_128 = new System.Windows.Forms.Label();
      this.thresholdLabel = new System.Windows.Forms.Label();
      this.thresholdTextBox = new System.Windows.Forms.TextBox();
      this.thresholdLabel_0 = new System.Windows.Forms.Label();
      this.thresholdTrackBar = new System.Windows.Forms.TrackBar();
      this.histogramChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).BeginInit();
      this.splitContainer.Panel1.SuspendLayout();
      this.splitContainer.Panel2.SuspendLayout();
      this.splitContainer.SuspendLayout();
      this.thresholdGroupBox.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.thresholdTrackBar)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.histogramChart)).BeginInit();
      this.SuspendLayout();
      // 
      // splitContainer
      // 
      this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer.Location = new System.Drawing.Point(0, 0);
      this.splitContainer.Name = "splitContainer";
      // 
      // splitContainer.Panel1
      // 
      this.splitContainer.Panel1.Controls.Add(this.originCheckBox);
      this.splitContainer.Panel1.Controls.Add(this.logCheckBox);
      this.splitContainer.Panel1.Controls.Add(this.rgbCheckBox);
      this.splitContainer.Panel1.Controls.Add(this.thresholdGroupBox);
      // 
      // splitContainer.Panel2
      // 
      this.splitContainer.Panel2.BackColor = System.Drawing.SystemColors.Control;
      this.splitContainer.Panel2.Controls.Add(this.histogramChart);
      this.splitContainer.Size = new System.Drawing.Size(694, 388);
      this.splitContainer.SplitterDistance = 205;
      this.splitContainer.TabIndex = 0;
      // 
      // originCheckBox
      // 
      this.originCheckBox.AutoSize = true;
      this.originCheckBox.Location = new System.Drawing.Point(19, 187);
      this.originCheckBox.Name = "originCheckBox";
      this.originCheckBox.Size = new System.Drawing.Size(126, 16);
      this.originCheckBox.TabIndex = 9;
      this.originCheckBox.Text = "Show origin image";
      this.originCheckBox.UseVisualStyleBackColor = true;
      this.originCheckBox.CheckedChanged += new System.EventHandler(this.originCheckBox_CheckedChanged);
      // 
      // logCheckBox
      // 
      this.logCheckBox.AutoSize = true;
      this.logCheckBox.Location = new System.Drawing.Point(19, 231);
      this.logCheckBox.Name = "logCheckBox";
      this.logCheckBox.Size = new System.Drawing.Size(102, 16);
      this.logCheckBox.TabIndex = 4;
      this.logCheckBox.Text = "With log axis";
      this.logCheckBox.UseVisualStyleBackColor = true;
      this.logCheckBox.Visible = false;
      this.logCheckBox.CheckedChanged += new System.EventHandler(this.logCheckBox_CheckedChanged);
      // 
      // rgbCheckBox
      // 
      this.rgbCheckBox.AutoSize = true;
      this.rgbCheckBox.Location = new System.Drawing.Point(19, 209);
      this.rgbCheckBox.Name = "rgbCheckBox";
      this.rgbCheckBox.Size = new System.Drawing.Size(102, 16);
      this.rgbCheckBox.TabIndex = 3;
      this.rgbCheckBox.Text = "RGB Histogram";
      this.rgbCheckBox.UseVisualStyleBackColor = true;
      this.rgbCheckBox.CheckedChanged += new System.EventHandler(this.rgbCheckBox_CheckedChanged);
      // 
      // thresholdGroupBox
      // 
      this.thresholdGroupBox.Controls.Add(this.customRadioButton);
      this.thresholdGroupBox.Controls.Add(this.entropyRadioButton);
      this.thresholdGroupBox.Controls.Add(this.otsuRadioButton);
      this.thresholdGroupBox.Controls.Add(this.thresholdLabel_255);
      this.thresholdGroupBox.Controls.Add(this.thresholdLabel_128);
      this.thresholdGroupBox.Controls.Add(this.thresholdLabel);
      this.thresholdGroupBox.Controls.Add(this.thresholdTextBox);
      this.thresholdGroupBox.Controls.Add(this.thresholdLabel_0);
      this.thresholdGroupBox.Controls.Add(this.thresholdTrackBar);
      this.thresholdGroupBox.Location = new System.Drawing.Point(13, 12);
      this.thresholdGroupBox.Name = "thresholdGroupBox";
      this.thresholdGroupBox.Size = new System.Drawing.Size(178, 169);
      this.thresholdGroupBox.TabIndex = 8;
      this.thresholdGroupBox.TabStop = false;
      this.thresholdGroupBox.Text = "Threshold";
      // 
      // customRadioButton
      // 
      this.customRadioButton.AutoSize = true;
      this.customRadioButton.Checked = true;
      this.customRadioButton.Location = new System.Drawing.Point(12, 66);
      this.customRadioButton.Name = "customRadioButton";
      this.customRadioButton.Size = new System.Drawing.Size(107, 16);
      this.customRadioButton.TabIndex = 10;
      this.customRadioButton.TabStop = true;
      this.customRadioButton.Text = "Customed value";
      this.customRadioButton.UseVisualStyleBackColor = true;
      // 
      // entropyRadioButton
      // 
      this.entropyRadioButton.AutoSize = true;
      this.entropyRadioButton.Location = new System.Drawing.Point(12, 44);
      this.entropyRadioButton.Name = "entropyRadioButton";
      this.entropyRadioButton.Size = new System.Drawing.Size(125, 16);
      this.entropyRadioButton.TabIndex = 9;
      this.entropyRadioButton.TabStop = true;
      this.entropyRadioButton.Text = "Entropy Algorithm";
      this.entropyRadioButton.UseVisualStyleBackColor = true;
      this.entropyRadioButton.CheckedChanged += new System.EventHandler(this.entropyRadioButton_CheckedChanged);
      // 
      // otsuRadioButton
      // 
      this.otsuRadioButton.AutoSize = true;
      this.otsuRadioButton.Location = new System.Drawing.Point(12, 20);
      this.otsuRadioButton.Name = "otsuRadioButton";
      this.otsuRadioButton.Size = new System.Drawing.Size(107, 16);
      this.otsuRadioButton.TabIndex = 8;
      this.otsuRadioButton.Text = "Otsu Algorithm";
      this.otsuRadioButton.UseVisualStyleBackColor = true;
      this.otsuRadioButton.CheckedChanged += new System.EventHandler(this.otsuRadioButton_CheckedChanged);
      // 
      // thresholdLabel_255
      // 
      this.thresholdLabel_255.AutoSize = true;
      this.thresholdLabel_255.Location = new System.Drawing.Point(149, 146);
      this.thresholdLabel_255.Name = "thresholdLabel_255";
      this.thresholdLabel_255.Size = new System.Drawing.Size(23, 12);
      this.thresholdLabel_255.TabIndex = 6;
      this.thresholdLabel_255.Text = "255";
      // 
      // thresholdLabel_128
      // 
      this.thresholdLabel_128.AutoSize = true;
      this.thresholdLabel_128.Location = new System.Drawing.Point(78, 146);
      this.thresholdLabel_128.Name = "thresholdLabel_128";
      this.thresholdLabel_128.Size = new System.Drawing.Size(23, 12);
      this.thresholdLabel_128.TabIndex = 7;
      this.thresholdLabel_128.Text = "128";
      // 
      // thresholdLabel
      // 
      this.thresholdLabel.AutoSize = true;
      this.thresholdLabel.Font = new System.Drawing.Font("SimSun", 9F);
      this.thresholdLabel.Location = new System.Drawing.Point(10, 94);
      this.thresholdLabel.Name = "thresholdLabel";
      this.thresholdLabel.Size = new System.Drawing.Size(101, 12);
      this.thresholdLabel.TabIndex = 1;
      this.thresholdLabel.Text = "Threshold value:";
      // 
      // thresholdTextBox
      // 
      this.thresholdTextBox.Location = new System.Drawing.Point(113, 91);
      this.thresholdTextBox.Name = "thresholdTextBox";
      this.thresholdTextBox.Size = new System.Drawing.Size(49, 21);
      this.thresholdTextBox.TabIndex = 2;
      this.thresholdTextBox.Text = "0";
      this.thresholdTextBox.MouseClick += new System.Windows.Forms.MouseEventHandler(this.thresholdTextBox_MouseClick);
      this.thresholdTextBox.TextChanged += new System.EventHandler(this.thresholdTextBox_TextChanged);
      // 
      // thresholdLabel_0
      // 
      this.thresholdLabel_0.AutoSize = true;
      this.thresholdLabel_0.Location = new System.Drawing.Point(15, 146);
      this.thresholdLabel_0.Name = "thresholdLabel_0";
      this.thresholdLabel_0.Size = new System.Drawing.Size(11, 12);
      this.thresholdLabel_0.TabIndex = 5;
      this.thresholdLabel_0.Text = "0";
      // 
      // thresholdTrackBar
      // 
      this.thresholdTrackBar.AccessibleRole = System.Windows.Forms.AccessibleRole.ScrollBar;
      this.thresholdTrackBar.Cursor = System.Windows.Forms.Cursors.Arrow;
      this.thresholdTrackBar.Location = new System.Drawing.Point(6, 118);
      this.thresholdTrackBar.Maximum = 255;
      this.thresholdTrackBar.Name = "thresholdTrackBar";
      this.thresholdTrackBar.Size = new System.Drawing.Size(166, 45);
      this.thresholdTrackBar.TabIndex = 0;
      this.thresholdTrackBar.TickFrequency = 32;
      this.thresholdTrackBar.Scroll += new System.EventHandler(this.thresholdTrackBar_Scroll);
      this.thresholdTrackBar.MouseUp += new System.Windows.Forms.MouseEventHandler(this.thresholdTrackBar_MouseUp);
      // 
      // histogramChart
      // 
      chartArea1.Name = "ChartArea1";
      this.histogramChart.ChartAreas.Add(chartArea1);
      legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
      legend1.Name = "HistogramLegend";
      this.histogramChart.Legends.Add(legend1);
      this.histogramChart.Location = new System.Drawing.Point(3, 0);
      this.histogramChart.Name = "histogramChart";
      this.histogramChart.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.Pastel;
      this.histogramChart.Size = new System.Drawing.Size(482, 388);
      this.histogramChart.TabIndex = 0;
      this.histogramChart.Text = "Histogram Chart";
      // 
      // HistogramWindow
      // 
      this.ClientSize = new System.Drawing.Size(694, 388);
      this.Controls.Add(this.splitContainer);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
      this.Name = "HistogramWindow";
      this.Text = "Histogram Window";
      this.splitContainer.Panel1.ResumeLayout(false);
      this.splitContainer.Panel1.PerformLayout();
      this.splitContainer.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).EndInit();
      this.splitContainer.ResumeLayout(false);
      this.thresholdGroupBox.ResumeLayout(false);
      this.thresholdGroupBox.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.thresholdTrackBar)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.histogramChart)).EndInit();
      this.ResumeLayout(false);

    }

    private void thresholdTrackBar_Scroll(object sender, EventArgs e)
    {
      customRadioButton.Checked = true;
      thresholdTextBox.Text = thresholdTrackBar.Value.ToString();
      updateThresholdSeries();
    }

    private void thresholdTextBox_TextChanged(object sender, EventArgs e)
    {
      int value;
      try
      {
        value = int.Parse(thresholdTextBox.Text);
      }
      catch (Exception)
      {
        value = 0;
        thresholdTextBox.Text = "0";
      }
      if (value > 255 || value < 0)
      {
        value = 0;
        thresholdTextBox.Text = "0";
      }
      thresholdTrackBar.Value = value;
      updateThresholdSeries();
      updateBinaryBitmap();
    }

    private void rgbCheckBox_CheckedChanged(object sender, EventArgs e)
    {
      if (rgbCheckBox.Checked)
      {
        updateRgbSeries();
      }
      else
      {
        removeRgbSeries();
      }
    }

    private void logCheckBox_CheckedChanged(object sender, EventArgs e)
    {
      //updateLogAxis(logCheckBox.Checked);
    }

    private void thresholdTrackBar_MouseUp(object sender, MouseEventArgs e)
    {
      updateBinaryBitmap();
    }

    private void otsuRadioButton_CheckedChanged(object sender, EventArgs e)
    {
      if (otsuRadioButton.Checked)
      {
        thresholdTextBox.Text =
          MainForm.windowManager.getFocusedImageWindow().imageProcessor.getOtsuThresholdValue().ToString();
        updateBinaryBitmap();
      }
    }

    private void entropyRadioButton_CheckedChanged(object sender, EventArgs e)
    {
      if (entropyRadioButton.Checked)
      {
        thresholdTextBox.Text =
          MainForm.windowManager.getFocusedImageWindow().imageProcessor.getEntropyThresholdValue().ToString();
        updateBinaryBitmap();
      }
    }

    private void thresholdTextBox_MouseClick(object sender, MouseEventArgs e)
    {
      customRadioButton.Checked = true;
    }

    private void originCheckBox_CheckedChanged(object sender, EventArgs e)
    {
      if (originCheckBox.Checked)
      {
        MainForm.windowManager.getFocusedImageWindow().updateImage(
          ImageProcessor.ImageType.ORIGIN_IMAGE, false);
      }
      else
      {
        MainForm.windowManager.getFocusedImageWindow().updateImage(
          ImageProcessor.ImageType.BINARY_IMAGE, false);
      }
    }

  }
}
