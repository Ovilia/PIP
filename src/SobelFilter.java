package pip;

import java.awt.Color;
import java.awt.image.BufferedImage;

/**
 *
 * @author Ovilia
 */
public class SobelFilter extends Filter {

    public SobelFilter (BufferedImage originImage,
            BorderProcess borderProcess) {
        super(originImage, 1, borderProcess);
    }

    protected void createFilteredImage() {
        int width = originImage.getWidth();
        int height = originImage.getHeight();

        // do process
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int color1 = (new Color(
                        borderedImage.getRGB(i - 1 + radio, j - 1 + radio))).getRed();
                int color2 = (new Color(
                        borderedImage.getRGB(i + radio, j - 1 + radio))).getRed();
                int color3 = (new Color(
                        borderedImage.getRGB(i + 1 + radio, j - 1 + radio))).getRed();
                int color4 = (new Color(
                        borderedImage.getRGB(i - 1 + radio, j + radio))).getRed();
                int color6 = (new Color(
                        borderedImage.getRGB(i + 1 + radio, j + radio))).getRed();
                int color7 = (new Color(
                        borderedImage.getRGB(i - 1 + radio, j + 1 + radio))).getRed();
                int color8 = (new Color(
                        borderedImage.getRGB(i + radio, j + 1 + radio))).getRed();
                int color9 = (new Color(
                        borderedImage.getRGB(i + 1 + radio, j + 1 + radio))).getRed();

                int gx = (color7 + 2 * color8 + color9) - (color1 + 2 * color2 + color3);
                int gy = (color3 + 2 * color6 + color9) - (color1 + 2 * color4 + color7);
                double value = Math.sqrt((double) (gx * gx + gy * gy));
                if (value > ImageProcessor.RANGE_OF_8BITS - 1) {
                    value = ImageProcessor.RANGE_OF_8BITS - 1;
                }
                int color = (new Color((int) value, (int) value, (int) value)).getRGB();
                filteredImage.setRGB(i, j, color);
            }
        }
    }
}
