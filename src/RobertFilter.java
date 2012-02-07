package pip;

import java.awt.image.BufferedImage;

/**
 *
 * @author Ovilia
 */
public class RobertFilter extends Filter {
    public RobertFilter (BufferedImage originImage, 
            BorderProcess borderProcess) {
        super(originImage, 50, borderProcess);
    }
    
    protected void createFilteredImage() {
        int width = originImage.getWidth();
        int height = originImage.getHeight();
        
        // do process
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                
            }
        }
    }
}
