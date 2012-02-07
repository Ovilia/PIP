package pip;

import java.awt.image.BufferedImage;
import java.util.Arrays;

/**
 *
 * @author Ovilia
 */
public class MedianFilter extends Filter {

    public MedianFilter(BufferedImage originImage, int radio,
            BorderProcess borderProcess) {
        super(originImage, radio, borderProcess);
    }

    protected void createFilteredImage() {
        int width = originImage.getWidth();
        int height = originImage.getHeight();

        // do process
        int length = (2 * radio + 1) * (2 * radio + 1) - 1;
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int[] colors = new int[length];
                int amt = 0;
                for (int m = 0; m <= 2 * radio; ++m) {
                    for (int n = 0; n <= 2 * radio; ++n) {
                        if (m != radio || n != radio) {
                            colors[amt] = borderedImage.getRGB(i + m, j + n);
                            amt++;
                        }
                    }
                }
                Arrays.sort(colors);
                filteredImage.setRGB(i, j, colors[length / 2]);
            }
        }
    }
}
