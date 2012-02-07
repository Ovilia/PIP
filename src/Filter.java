package pip;

import java.awt.image.BufferedImage;

/**
 *
 * @author Ovilia
 */
public abstract class Filter {
    protected int radio;
    protected boolean isProcessed;
    protected BorderProcess borderProcess;
    protected BufferedImage originImage;
    protected BufferedImage filteredImage;
    
    public Filter (BufferedImage originImage, int radio, 
            BorderProcess borderProcess) {
        this.originImage = originImage;
        this.radio = radio;
        this.borderProcess = borderProcess;
        isProcessed = false;
    }
    
    /**
     * get filtered image
     * @return filtered image
     */
    public BufferedImage getFilteredImage() {
        if (!isProcessed) {
            doBorderProcess();
            createFilteredImage();
        }
        isProcessed = true;
        return filteredImage;
    }
    
    /**
     * init filteredImage, do border process according to borderProcess
     */
    protected void doBorderProcess() {
        int originWidth = originImage.getWidth();
        int originHeight = originImage.getHeight();
        int width = originWidth + 2 * radio;
        int height = originHeight + 2 * radio;
        filteredImage = new BufferedImage(width, height, originImage.getType());
        
        // copy image
        for (int i = 0; i < originWidth; ++i) {
            for (int j = 0; j < originHeight; ++j) {
                filteredImage.setRGB(i + radio, j + radio, 
                        originImage.getRGB(i, j));
            }
        }
        
        // do border process
        switch (borderProcess) {
            case BLACK:
                break;
            case NEAREST:
                // top-left corner
                int tlColor = originImage.getRGB(0, 0);
                // top-right corner
                int trColor = originImage.getRGB(originWidth - 1, 0);
                // bottom-left corner
                int blColor = originImage.getRGB(0, originHeight - 1);
                // bottom-right corner
                int brColor = originImage.getRGB(originWidth - 1, originHeight - 1);
                for (int i = 0; i < radio; ++i) {
                    for (int j = 0; j < radio; ++j) {
                        filteredImage.setRGB(i, j, tlColor);
                        filteredImage.setRGB(i + radio + originWidth, j, trColor);
                        filteredImage.setRGB(i, radio + originHeight + j, blColor);
                        filteredImage.setRGB(i + radio + originWidth,
                                radio + originHeight + j, brColor);
                    }
                }
                // top and bottom border
                for (int i = 0; i < originWidth; ++i) {
                    int topColor = originImage.getRGB(i, 0);
                    int bottomColor = originImage.getRGB(i, originHeight - 1);
                    for (int j = 0; j < radio; ++j) {
                        filteredImage.setRGB(i + radio, j, topColor);
                        filteredImage.setRGB(i + radio,
                                j + radio + originHeight, bottomColor);
                    }
                }
                // left and right border
                for (int i = 0; i < originHeight; ++i) {
                    int leftColor = originImage.getRGB(0, i);
                    int rightColor = originImage.getRGB(originWidth - 1, i);
                    for (int j = 0; j < radio; ++j) {
                        filteredImage.setRGB(j, i + radio, leftColor);
                        filteredImage.setRGB(j + radio + originWidth, i + radio,
                                rightColor);
                    }
                }
                break;
            case MIRROR:
                // mirror on left and right
                for (int i = 0; i < radio; ++i) {
                    for (int j = 0; j < originHeight; ++j) {
                        int leftColor = originImage.getRGB(radio - 1 - i, j);
                        int rightColor = originImage.getRGB(originWidth - 1 - i, j);
                        filteredImage.setRGB(i, j + radio, leftColor);
                        filteredImage.setRGB(i + radio + originWidth, j + radio,
                                rightColor);
                    }
                }
                // mirror on top and bottom
                for (int i = 0; i < radio; ++i) {
                    for (int j = 0; j < width; ++j) {
                        int topColor = filteredImage.getRGB(j, 2 * radio - i);
                        int bottomColor = filteredImage.getRGB(j, height - radio - i);
                        filteredImage.setRGB(j, i, topColor);
                        filteredImage.setRGB(j, i + radio + originHeight, bottomColor);
                    }
                }
                break;
            case PERIODIC:
                // periodic on left and right
                for (int i = 0; i < radio; ++i) {
                    for (int j = 0; j < originHeight; ++j) {
                        int leftColor = originImage.getRGB(originWidth - radio + i, j);
                        int rightColor = originImage.getRGB(i, j);
                        filteredImage.setRGB(i, j + radio, leftColor);
                        filteredImage.setRGB(i + radio + originWidth, j + radio,
                                rightColor);
                    }
                }
                // periodic on top and bottom
                for (int i = 0; i < radio; ++i) {
                    for (int j = 0; j < width; ++j) {
                        int topColor = filteredImage.getRGB(j, height - 2 * radio + i);
                        int bottomColor = filteredImage.getRGB(j, radio + i);
                        filteredImage.setRGB(j, i, topColor);
                        filteredImage.setRGB(j, i + radio + originHeight, bottomColor);
                    }
                }
                break;
            default:
                break;
        }
    }
    
    /**
     * create filtered image using given algorithm in child class
     */
    protected abstract void createFilteredImage();
    
    public enum BorderProcess {
        BLACK,
        NEAREST,
        MIRROR,
        PERIODIC
    }
}
