package pip;

import java.awt.Color;
import java.awt.image.BufferedImage;

/**
 *
 * @author Ovilia
 */
public class GaussianFilter extends Filter {

    double sigma;

    public GaussianFilter(BufferedImage originImage, int radio, double sigma,
            BorderProcess borderProcess) {
        super(originImage, radio, borderProcess);
        this.sigma = sigma;
    }

    protected void createFilteredImage() {
        double coefficient = 0.5 / Math.PI / sigma / sigma;
        double[][] kernel = new double[2 * radio + 1][2 * radio + 1];
        double kernelSum = 0;
        for (int m = 0; m <= 2 * radio; ++m) {
            for (int n = 0; n <= 2 * radio; ++n) {
                kernel[m][n] = coefficient * Math.exp(
                        -(m * m + n * n) / 2 / sigma / sigma);
                kernelSum += kernel[m][n];
                //System.out.println(kernel[m][n]);
            }
        }
        // make sure brightness doesn't change
        for (int m = 0; m <= 2 * radio; ++m) {
            for (int n = 0; n <= 2 * radio; ++n) {
                kernel[m][n] /= kernelSum;
            }
        }

        int width = filteredImage.getWidth();
        int height = filteredImage.getHeight();
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                double value = 0;
                for (int m = 0; m <= 2 * radio; ++m) {
                    for (int n = 0; n <= 2 * radio; ++n) {
                        int gray = (new Color(borderedImage.getRGB(i + m, j + n))).getRed();
                        value += gray * kernel[m][n];
                    }
                }
                int color = (int) value;
                if (color > ImageProcessor.RANGE_OF_8BITS - 1) {
                    color = ImageProcessor.RANGE_OF_8BITS - 1;
                }
                filteredImage.setRGB(i, j, (new Color(color, color, color)).getRGB());
            }
        }
    }
}
