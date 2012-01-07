using System;
using System.Drawing;
using System.Windows.Forms;

namespace PIP
{
  class HistogramWindow : Form
  {
    private SplitContainer splitContainer;
    private Label thresholdLabel;
    private TrackBar thresholdTrackBar;
    private System.Windows.Forms.DataVisualization.Charting.Chart histogramChart;


    public HistogramWindow()
    {
      InitializeComponent();

      Bitmap bitmap = MainForm.windowManager.getFocusedImageWindow().ImageBitmap;

    }

    private void InitializeComponent()
    {
      System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 
        = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
      System.Windows.Forms.DataVisualization.Charting.Legend legend1 
        = new System.Windows.Forms.DataVisualization.Charting.Legend();
      System.Windows.Forms.DataVisualization.Charting.Series series1 
        = new System.Windows.Forms.DataVisualization.Charting.Series();
      this.splitContainer = new System.Windows.Forms.SplitContainer();
      this.thresholdLabel = new System.Windows.Forms.Label();
      this.thresholdTrackBar = new System.Windows.Forms.TrackBar();
      this.histogramChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).BeginInit();
      this.splitContainer.Panel1.SuspendLayout();
      this.splitContainer.Panel2.SuspendLayout();
      this.splitContainer.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.thresholdTrackBar)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.histogramChart)).BeginInit();
      this.SuspendLayout();
      // 
      // splitContainer
      // 
      this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer.Location = new System.Drawing.Point(0, 0);
      this.splitContainer.Name = "splitContainer";
      this.splitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
      // 
      // splitContainer.Panel1
      // 
      this.splitContainer.Panel1.Controls.Add(this.thresholdLabel);
      this.splitContainer.Panel1.Controls.Add(this.thresholdTrackBar);
      // 
      // splitContainer.Panel2
      // 
      this.splitContainer.Panel2.Controls.Add(this.histogramChart);
      this.splitContainer.Size = new System.Drawing.Size(484, 295);
      this.splitContainer.SplitterDistance = 37;
      this.splitContainer.TabIndex = 0;
      // 
      // thresholdLabel
      // 
      this.thresholdLabel.AutoSize = true;
      this.thresholdLabel.Font = new System.Drawing.Font("SimSun", 9F);
      this.thresholdLabel.Location = new System.Drawing.Point(12, 9);
      this.thresholdLabel.Name = "thresholdLabel";
      this.thresholdLabel.Size = new System.Drawing.Size(95, 12);
      this.thresholdLabel.TabIndex = 1;
      this.thresholdLabel.Text = "Threshold value";
      // 
      // thresholdTrackBar
      // 
      this.thresholdTrackBar.Location = new System.Drawing.Point(113, 3);
      this.thresholdTrackBar.Name = "thresholdTrackBar";
      this.thresholdTrackBar.Size = new System.Drawing.Size(368, 45);
      this.thresholdTrackBar.TabIndex = 0;
      // 
      // histogramChart
      // 
      chartArea1.Name = "ChartArea1";
      this.histogramChart.ChartAreas.Add(chartArea1);
      legend1.Name = "Legend1";
      this.histogramChart.Legends.Add(legend1);
      this.histogramChart.Location = new System.Drawing.Point(0, 3);
      this.histogramChart.Name = "histogramChart";
      this.histogramChart.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.Pastel;
      series1.ChartArea = "ChartArea1";
      series1.Legend = "Legend1";
      series1.Name = "Series1";
      this.histogramChart.Series.Add(series1);
      this.histogramChart.Size = new System.Drawing.Size(481, 252);
      this.histogramChart.TabIndex = 0;
      this.histogramChart.Text = "histogramChart";
      // 
      // HistogramWindow
      // 
      this.ClientSize = new System.Drawing.Size(484, 295);
      this.Controls.Add(this.splitContainer);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
      this.Name = "HistogramWindow";
      this.Text = "Histogram Window";
      this.splitContainer.Panel1.ResumeLayout(false);
      this.splitContainer.Panel1.PerformLayout();
      this.splitContainer.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).EndInit();
      this.splitContainer.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.thresholdTrackBar)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.histogramChart)).EndInit();
      this.ResumeLayout(false);

    }

  }
}
