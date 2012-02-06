/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package pip;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.util.Hashtable;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.ValueMarker;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.StandardXYItemRenderer;
import org.jfree.chart.renderer.xy.XYBarRenderer;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.data.xy.IntervalXYDataset;
import org.jfree.data.xy.XYBarDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.ui.RectangleInsets;
import pip.PIPView.ThresholdListener;

/**
 *
 * @author Ovilia
 */
public class HistogramPanel extends JPanel {
    private ValueMarker thresholdMarker;
    private JSlider slider;
    private JLabel thresholdLabel;
    private static int thresholdValue;

    public HistogramPanel(ImageProcessor imageProcessor) {
        int[] histogram = imageProcessor.getHistogram();
        int[][] rgbHistogram = imageProcessor.getRgbHistogram();

        XYSeries graySeries = new XYSeries("Gray");
        XYSeries redSeries = new XYSeries("Red");
        XYSeries greenSeries = new XYSeries("Green");
        XYSeries blueSeries = new XYSeries("Blue");
        for (int i = 0; i < histogram.length; ++i) {
            graySeries.add(i, histogram[i]);
            redSeries.add(i, rgbHistogram[i][0]);
            greenSeries.add(i, rgbHistogram[i][1]);
            blueSeries.add(i, rgbHistogram[i][2]);
        }

        XYSeriesCollection grayCollection = new XYSeriesCollection();
        XYSeriesCollection rgbCollection = new XYSeriesCollection();
        grayCollection.addSeries(graySeries);
        rgbCollection.addSeries(redSeries);
        rgbCollection.addSeries(greenSeries);
        rgbCollection.addSeries(blueSeries);
        IntervalXYDataset dataset = new XYBarDataset(grayCollection, 0.9D);

        JFreeChart jFreeChart = ChartFactory.createXYBarChart(
                "Histogram", "Value", false, "Gray Amount", dataset,
                PlotOrientation.VERTICAL, true, true, false);

        XYPlot plot = jFreeChart.getXYPlot();
        plot.setBackgroundPaint(Color.WHITE);

        XYItemRenderer grayRender = new XYBarRenderer();
        XYItemRenderer rgbRender = new StandardXYItemRenderer();
        plot.setRenderer(0, grayRender);
        plot.setRenderer(1, rgbRender);
        grayRender.setSeriesPaint(0, Color.GRAY);
        rgbRender.setSeriesPaint(0, Color.RED);
        rgbRender.setSeriesPaint(1, Color.GREEN);
        rgbRender.setSeriesPaint(2, Color.BLUE);

        NumberAxis rightAxis = new NumberAxis();
        rightAxis.setLabel("RGB Amount");
        plot.setRangeAxis(1, rightAxis);
        plot.setDataset(1, rgbCollection);
        plot.mapDatasetToRangeAxis(1, 1);

        // Threshold value
        thresholdValue = 0;
        thresholdMarker = new ValueMarker(0);
        thresholdMarker.setLabel("Threshold value: " + 0);
        thresholdMarker.setPaint(Color.ORANGE);
        thresholdMarker.setStroke(new BasicStroke(3.0F));
        thresholdMarker.setLabelOffset(
                new RectangleInsets(10, 40, 5, 4));
        plot.addDomainMarker(thresholdMarker);

        ChartPanel panel = new ChartPanel(jFreeChart);
        this.setLayout(new BorderLayout());
        this.add(panel, BorderLayout.CENTER);

        slider = new JSlider();
        slider.setMinimum(0);
        slider.setMaximum(histogram.length);
        slider.setValue(thresholdValue);
        Hashtable<Integer, JLabel> table = new Hashtable<Integer, JLabel>();
        for (int i = 0; i <= histogram.length; i += 64) {
            table.put(i, new JLabel(i + ""));
        }
        slider.setLabelTable(table);
        slider.setPaintLabels(true);
        slider.addChangeListener(new SliderListener());

        JPanel southPanel = new JPanel();
        southPanel.setLayout(new FlowLayout());
        thresholdLabel = new JLabel("Threshold value: " + thresholdValue);
        southPanel.add(thresholdLabel);

        southPanel.add(slider);

        JButton thresholdButton = new JButton("Update binary image");
        thresholdButton.addActionListener(new ThresholdListener());
        southPanel.add(thresholdButton);

        this.add(southPanel, BorderLayout.SOUTH);
    }

    /**
     * @return the thresholdValue
     */
    public static int getThresholdValue() {
        return thresholdValue;
    }
    
    class SliderListener implements ChangeListener {

        public void stateChanged(ChangeEvent changeEvent) {
            if (thresholdMarker != null) {
                thresholdValue = slider.getValue();
                thresholdMarker.setValue(thresholdValue);
                thresholdMarker.setLabel("Threshold value: " + thresholdValue);
                thresholdLabel.setText("Threshold value: " + thresholdValue);
            }
        }
    }
}
