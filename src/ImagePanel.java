package pip;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.Hashtable;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.UIManager;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

/**
 *
 * @author Ovilia
 */
public class ImagePanel extends JPanel {

    private BufferedImage image;
    private JSlider slider;
    private JPanel centerPanel;

    /**
     * a JPanel that contains scalable image
     * @param image origin image
     */
    public ImagePanel(BufferedImage image) {
        this.image = image;

        this.setLayout(new BorderLayout());
        centerPanel = new JPanel();
        this.add(centerPanel, BorderLayout.CENTER);

        slider = new JSlider();
        slider.setMinimum(0);
        slider.setMaximum(200);
        Hashtable<Integer, JLabel> table = new Hashtable<Integer, JLabel>();
        int length = slider.getMaximum();
        for (int i = 0; i <= length; i += 50) {
            table.put(i, new JLabel(i + "%"));
        }
        slider.setLabelTable(table);
        slider.setPaintLabels(true);
        slider.setValue(100);
        slider.addChangeListener(new SliderListener());

        JPanel northPanel = new JPanel();
        northPanel.setLayout(new FlowLayout());
        JLabel label = new JLabel("Image scale: ");
        northPanel.add(label);
        northPanel.add(slider);

        this.add(northPanel, BorderLayout.NORTH);

        resizeImage(image.getWidth(), image.getHeight());
    }

    /**
     * set max size of the image
     * @param width
     * @param height 
     */
    public void setMaxSize(int width, int height) {
        double radioWidth = (double) width / image.getWidth();
        double radioHeight = (double) height / image.getHeight();
        if (radioWidth < radioHeight) {
            slider.setMaximum((int) (radioWidth * 100));
            slider.setValue((int) (radioWidth * 100));
        } else {
            slider.setMaximum((int) (radioHeight * 100));
            slider.setValue((int) (radioHeight * 100));
        }
    }

    /**
     * resize image with given width and height
     * @param width new width of image
     * @param height new height of image
     */
    public void resizeImage(int width, int height) {
        // zero size is not legal
        if (width <= 0) {
            width = 1;
        }
        if (height <= 0) {
            height = 1;
        }
        JLabel imageLabel = getLabel(width, height, image);
        centerPanel.removeAll();
        centerPanel.add(imageLabel);
        this.repaint();
        this.revalidate();
    }

    /**
     * create a JLabel that contains image with given size
     * @param w width of image
     * @param h height of image
     * @param image image to contain
     * @return a JLabel that contain image
     */
    private JLabel getLabel(int w, int h, BufferedImage image) {
        BufferedImage scaled = scale(image, w, h);
        JLabel label = new JLabel(new ImageIcon(scaled));
        label.setPreferredSize(new Dimension(w, h));
        label.setBorder(BorderFactory.createEtchedBorder());
        return label;
    }

    /**
     * scale image with given width and height
     * @param src source image
     * @param w width
     * @param h height
     * @return scaled image
     */
    private BufferedImage scale(BufferedImage src, int w, int h) {
        int type = BufferedImage.TYPE_INT_RGB;
        BufferedImage dst = new BufferedImage(w, h, type);
        Graphics2D g2 = dst.createGraphics();
        // Fill background for scale to fit.  
        g2.setBackground(UIManager.getColor("Panel.background"));
        g2.clearRect(0, 0, w, h);
        double xScale = (double) w / src.getWidth();
        double yScale = (double) h / src.getHeight();
        // Scaling options:  
        // Scale to fit - image just fits in label.  
        double scale = Math.min(xScale, yScale);
        // Scale to fill - image just fills label.  
        //double scale = Math.max(xScale, yScale);  
        int width = (int) (scale * src.getWidth());
        int height = (int) (scale * src.getHeight());
        int x = (w - width) / 2;
        int y = (h - height) / 2;
        g2.drawImage(src, x, y, width, height, null);
        g2.dispose();
        return dst;
    }

    /**
     * listener for slider changes state
     */
    class SliderListener implements ChangeListener {

        public void stateChanged(ChangeEvent changeEvent) {
            resizeImage(slider.getValue() * image.getWidth() / 100,
                    slider.getValue() * image.getHeight() / 100);
        }
    }
}