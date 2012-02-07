/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package pip;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Hashtable;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSlider;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.DatasetRenderingOrder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.SeriesRenderingOrder;
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

/**
 *
 * @author Ovilia
 */
public class HistogramPanel extends JPanel {

    private ValueMarker thresholdMarker;
    private JSlider slider;
    private JLabel thresholdLabel;
    private JRadioButton otsuButton;
    private JRadioButton entropyButton;
    private JRadioButton customButton;
    private static int thresholdValue;
    private int otsuThresholdValue;
    private int entropyThresholdValue;
    private ImageProcessor imageProcessor;

    public HistogramPanel(ImageProcessor imageProcessor) {
        this.imageProcessor = imageProcessor;
        int[] histogram = imageProcessor.getHistogram();
        int[][] rgbHistogram = imageProcessor.getRgbHistogram();
        otsuThresholdValue = -1;
        entropyThresholdValue = -1;

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

        // bind rgb with right axis
        NumberAxis rightAxis = new NumberAxis();
        rightAxis.setLabel("RGB Amount");
        plot.setRangeAxis(1, rightAxis);
        plot.setDataset(1, rgbCollection);
        plot.mapDatasetToRangeAxis(1, 1);
        plot.setDatasetRenderingOrder(DatasetRenderingOrder.FORWARD);

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

        // slider for threshold
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
        slider.addMouseListener(new SliderListener());

        JPanel northPanel = new JPanel();
        northPanel.setLayout(new FlowLayout());
        thresholdLabel = new JLabel("Threshold value: " + thresholdValue);
        northPanel.add(thresholdLabel);

        // add radio button to choose threshold value
        otsuButton = new JRadioButton("Otsu");
        entropyButton = new JRadioButton("Entropy");
        customButton = new JRadioButton("Customed");
        customButton.setSelected(true);
        otsuButton.addActionListener(new ButtonListener());
        entropyButton.addActionListener(new ButtonListener());
        customButton.addActionListener(new ButtonListener());

        ButtonGroup group = new ButtonGroup();
        group.add(otsuButton);
        group.add(entropyButton);
        group.add(customButton);
        northPanel.add(otsuButton);
        northPanel.add(entropyButton);
        northPanel.add(customButton);

        northPanel.add(slider);

        JButton thresholdButton = new JButton("Show binary image");
        thresholdButton.addActionListener(new ThresholdListener());
        northPanel.add(thresholdButton);

        this.add(northPanel, BorderLayout.NORTH);
    }

    /**
     * @return the thresholdValue
     */
    public static int getThresholdValue() {
        return thresholdValue;
    }

    /**
     * @return threshold value calculated using Otsu algorithm
     */
    public int getOtsuThreshold() {
        if (otsuThresholdValue < 0) {
            int[] histogram = imageProcessor.getHistogram();
            int[] accHistogram = imageProcessor.getAccHistogram();

            // Calculate when threshold value = 0,
            // then calculate on the base of former outcome
            // Sum of pixels with index below thresholdValue 
            int belowSum = 0;
            // Sum of pixels with index upper than threshold value 
            int upperSum = accHistogram[accHistogram.length - 1];

            // Sum of pixels multiplies index value below thresholdValue 
            int belowWeighted = 0;
            // Sum of pixels multiplies index value below threshold value 
            int upperWeighted = imageProcessor.getWeightedHistogramSum();

            // Average of pixels below thresholdValue
            double belowAverage = 0;
            // Average of pixels upper than thresholdValue
            double upperAverage;
            if (upperSum == 0) {
                upperAverage = 0;
            } else {
                upperAverage = (double) upperWeighted / upperSum;
            }

            // Max variance between two parts
            double maxVarianceBetween = belowSum * upperSum
                    * (belowAverage - upperAverage) * (belowAverage - upperAverage);
            otsuThresholdValue = 0;

            // Loop threshold value to get max variance between two parts
            for (int threshold = 0; threshold < accHistogram.length; ++threshold) {
                belowSum += histogram[threshold];
                upperSum -= histogram[threshold];

                if (belowSum == 0) {
                    continue;
                }
                if (upperSum == 0) {
                    break;
                }

                belowWeighted += histogram[threshold] * threshold;
                upperWeighted -= histogram[threshold] * threshold;

                belowAverage = (double) belowWeighted / belowSum;
                upperAverage = (double) upperWeighted / upperSum;

                // Variance between below and upper part
                double variaceBetween = (double) belowSum * upperSum
                        * (belowAverage - upperAverage) * (belowAverage - upperAverage);

                // Update threshold value if variance between two parts are
                // larger than maxVarianceBetween
                if (variaceBetween > maxVarianceBetween) {
                    maxVarianceBetween = variaceBetween;
                    otsuThresholdValue = threshold;
                }
            }
        }
        return otsuThresholdValue;
    }

    /**
     * @return threshold value calculated using Entropy algorithm
     */
    public int getEntropyThreshold() {
        if (entropyThresholdValue < 0) {
            int[] histogram = imageProcessor.getHistogram();
            int[] accHistogram = imageProcessor.getAccHistogram();
            // Sum of x * log(x) with index below thresholdValue
            double belowSumXLogX = 0;
            // Sum of x * log(x) with index upper than threshold value
            double upperSumXLogX = 0;
            for (int i = 0; i < histogram.length; ++i) {
                if (histogram[i] != 0) {
                    // Log on base e
                    upperSumXLogX += histogram[i] * Math.log((double) histogram[i]);
                }
            }

            double maxEntropy = 0;

            for (int threshold = 0; threshold < histogram.length; ++threshold) {
                if (histogram[threshold] == 0) {
                    continue;
                }

                int upperSum = accHistogram[histogram.length - 1]
                        - accHistogram[threshold];
                if (upperSum == 0) {
                    break;
                }

                double xLogX = histogram[threshold] * Math.log(histogram[threshold]);
                belowSumXLogX += xLogX;
                upperSumXLogX -= xLogX;

                double belowEntropy = Math.log(accHistogram[threshold])
                        - belowSumXLogX / accHistogram[threshold];
                double upperEntropy = Math.log(upperSum)
                        - upperSumXLogX / upperSum;
                double entropy = belowEntropy + upperEntropy;

                if (entropy > maxEntropy) {
                    entropyThresholdValue = threshold;
                    maxEntropy = entropy;
                }
            }
        }
        return entropyThresholdValue;
    }

    class SliderListener implements MouseListener {

        public void mousePressed(MouseEvent e) {
        }

        public void mouseReleased(MouseEvent e) {
            if (thresholdMarker != null) {
                customButton.setSelected(true);
                thresholdValue = slider.getValue();
                thresholdMarker.setValue(thresholdValue);
                thresholdMarker.setLabel("Threshold value: " + thresholdValue);
                thresholdLabel.setText("Threshold value: " + thresholdValue);
            }
        }

        public void mouseEntered(MouseEvent e) {
        }

        public void mouseExited(MouseEvent e) {
        }

        public void mouseClicked(MouseEvent e) {
        }
    }

    public class ThresholdListener implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            // show a frame with given threshold value
            int threshold = HistogramPanel.getThresholdValue();
            JFrame frame = new JFrame("Binary image with threshold value "
                    + threshold);
            ImagePanel binaryPanel = new ImagePanel(
                    PIPView.getImageProcessor().getThresholdImage(threshold));
            frame.add(binaryPanel);
            frame.setSize(600, 400);
            frame.show();
        }
    }

    public class ButtonListener implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            if (otsuButton.isSelected()) {
                // get otsu threshold value
                thresholdValue = getOtsuThreshold();
                thresholdMarker.setValue(thresholdValue);
                slider.setValue(thresholdValue);
                thresholdMarker.setLabel("Threshold value: " + thresholdValue);
                thresholdLabel.setText("Threshold value: " + thresholdValue);
            } else if (entropyButton.isSelected()) {
                // get entropy threshold value
                thresholdValue = getEntropyThreshold();
                thresholdMarker.setValue(thresholdValue);
                slider.setValue(thresholdValue);
                thresholdMarker.setLabel("Threshold value: " + thresholdValue);
                thresholdLabel.setText("Threshold value: " + thresholdValue);
            }
        }
    }
}
