package pip;

import java.awt.Color;
import java.awt.image.BufferedImage;

/**
 *
 * @author Ovilia
 */
public class RobertFilter extends Filter {
    public RobertFilter (BufferedImage originImage, 
            BorderProcess borderProcess) {
        super(originImage, 1, borderProcess);
    }
    
    protected void createFilteredImage() {
        int width = originImage.getWidth();
        int height = originImage.getHeight();
        
        // do process
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int color5 = (new Color(
                        borderedImage.getRGB(i + radio, j + radio))).getRed();
                int color9 = (new Color(
                        borderedImage.getRGB(i + 1 + radio, j + 1 + radio))).getRed();
                int color8 = (new Color(
                        borderedImage.getRGB(i + radio, j + 1 + radio))).getRed();
                int color6 = (new Color(
                        borderedImage.getRGB(i + 1 + radio, j + radio))).getRed();
                
                int gx = color9 - color5;
                int gy = color8 - color6;
                double value = Math.sqrt((double)(gx * gx + gy * gy));
                if (value > ImageProcessor.RANGE_OF_8BITS - 1) {
                    value = ImageProcessor.RANGE_OF_8BITS - 1;
                }
                int color = (new Color((int)value, (int)value, (int)value)).getRGB();
                filteredImage.setRGB(i, j, color);
            }
        }
    }
}
