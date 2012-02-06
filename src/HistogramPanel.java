/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package pip;

import java.awt.Color;
import javax.swing.JPanel;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.StandardXYItemRenderer;
import org.jfree.chart.renderer.xy.XYBarRenderer;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.data.xy.IntervalXYDataset;
import org.jfree.data.xy.XYBarDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

/**
 *
 * @author Ovilia
 */
public class HistogramPanel extends JPanel {

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

        ChartPanel panel = new ChartPanel(jFreeChart);
        this.add(panel);
    }
}
