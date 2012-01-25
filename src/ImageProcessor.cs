using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace PIP
{
  class ImageProcessor
  {
    public const int RANGE_OF_8BITS = 256;

    private Bitmap bitmap;
    private Bitmap grayScaleBitmap;
    private int[] histogram;
    private int[] accumulatedHistogram;
    private int[,] rgbHistogram;

    private int weightedHistogram;

    private int maxHistogram;
    private int maxRgbHistogram;

    private int thresholdValue;
    private int otsuThresholdValue;
    private int entropyThresholdValue;

    private Bitmap binaryBitmap;

    public enum ImageType{
      ORIGIN_IMAGE = 0,
      GRAY_IMAGE,
      BINARY_IMAGE
    }


    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="bitmap">Bitmap of the image to be processed</param>
    public ImageProcessor(Bitmap bitmap)
    {
      setBitmap(bitmap);
    }

    /// <summary>
    /// Get pixel of given position
    /// </summary>
    /// <param name="x">X position of the pixel</param>
    /// <param name="y">Y position of the pixel</param>
    /// <returns>Color of the pixel</returns>
    public Color getPixel(int x, int y)
    {
      return bitmap.GetPixel(x, y);
    }

    /// <summary>
    /// Get width of the bitmap
    /// </summary>
    /// <returns>Width of the bitmap</returns>
    public int getWidth()
    {
      return bitmap.Width;
    }

    /// <summary>
    /// Get height of the bitmap
    /// </summary>
    /// <returns>Height of the bitmap</returns>
    public int getHeight()
    {
      return bitmap.Height;
    }

    /// <summary>
    /// Get bitmap of original image
    /// </summary>
    /// <returns>Bitmap</returns>
    public Bitmap getBitmap()
    {
      return bitmap;
    }

    /// <summary>
    /// Set bitmap of original image,
    /// histogram and other value is set to be uninitialized
    /// </summary>
    /// <param name="bitmap"></param>
    public void setBitmap(Bitmap bitmap)
    {
      if (bitmap != this.bitmap)
      {
        this.bitmap = bitmap;
        // -1 is the default flag to notify the uninitialized variables
        maxHistogram = -1;
        maxRgbHistogram = -1;
        otsuThresholdValue = -1;
        entropyThresholdValue = -1;
        weightedHistogram = -1;
      }
    }

    /// <summary>
    /// Get gray scale bitmap
    /// </summary>
    /// <returns>Gray scale bitmap</returns>
    public Bitmap getGrayScaleBitmap()
    {
      if (grayScaleBitmap != null)
      {
        return grayScaleBitmap;
      }
      grayScaleBitmap = new Bitmap(bitmap.Width, bitmap.Height);
      /*
      for (int i = 0; i < bitmap.Width; ++i)
      {
        for (int j = 0; j < bitmap.Height; ++j)
        {
          Color color = bitmap.GetPixel(i, j);
          int grayScale = (int)(color.R * 0.3 + color.G * 0.59
            + color.B * 0.11);
          Color grayColor = Color.FromArgb(grayScale, grayScale, grayScale);
          grayScaleBitmap.SetPixel(i, j, grayColor);
        }
      }
      */
      Graphics g = Graphics.FromImage(grayScaleBitmap);
      ColorMatrix colorMatrix = new ColorMatrix(
        new float[][]
        {
          new float[] {0.3f, 0.3f, 0.3f, 0, 0},
          new float[] {0.59f, 0.59f, 0.59f, 0, 0},
          new float[] {0.11f, 0.11f, 0.11f, 0, 0},
          new float[] {0, 0, 0, 1, 0},
          new float[] {0, 0, 0, 0, 1}
        });
      ImageAttributes attributes = new ImageAttributes();
      attributes.SetColorMatrix(colorMatrix);
      g.DrawImage(bitmap, new Rectangle(0, 0, bitmap.Width, bitmap.Height),
        0, 0, bitmap.Width, bitmap.Height, GraphicsUnit.Pixel, attributes);
      g.Dispose();
    
      return grayScaleBitmap;
    }

    /// <summary>
    /// Get histogram array
    /// </summary>
    /// <returns>Histogram array</returns>
    public int[] getHistogram()
    {
      if (histogram != null)
      {
        return histogram;
      }
      histogram = new int[RANGE_OF_8BITS];
      accumulatedHistogram = new int[RANGE_OF_8BITS];

      Bitmap grayScaleBitmap = getGrayScaleBitmap();
      for (int i = 0; i < grayScaleBitmap.Width; ++i)
      {
        for (int j = 0; j < grayScaleBitmap.Height; ++j)
        {
          histogram[grayScaleBitmap.GetPixel(i, j).R]++;
        }
      }

      maxHistogram = histogram[0];
      accumulatedHistogram[0] = histogram[0];
      for (int i = 1; i < RANGE_OF_8BITS; ++i)
      {
        if (histogram[i] > maxHistogram)
        {
          maxHistogram = histogram[i];
        }
        accumulatedHistogram[i] = accumulatedHistogram[i - 1] + histogram[i];
      }
      return histogram;
    }

    /// <summary>
    /// Get accumulated histogram value
    /// </summary>
    /// <returns>Accumulated histogram value</returns>
    public int[] getAccumulatedHistogram()
    {
      if (accumulatedHistogram == null)
      {
        getHistogram();
      }
      return accumulatedHistogram;
    }

    /// <summary>
    /// Get sum(i * histogram[i]) of histogram array
    /// </summary>
    /// <returns>Weighted sum of histogram array</returns>
    int getWeightedHistogram()
    {
      if (weightedHistogram != -1)
      {
        return weightedHistogram;
      }
      // Make sure histogram is calculate
      getHistogram();

      weightedHistogram = 0;
      for (int i = 0; i < RANGE_OF_8BITS; ++i)
      {
        weightedHistogram += i * histogram[i];
      }
      return weightedHistogram;
    }

    /// <summary>
    /// Get RGB hitogram array
    /// </summary>
    /// <returns>RGB hitogram array</returns>
    public int[,] getRgbHistogram()
    {
      if (rgbHistogram != null)
      {
        return rgbHistogram;
      }
      rgbHistogram = new int[RANGE_OF_8BITS, 3];
      for (int i = 0; i < bitmap.Width; ++i)
      {
        for (int j = 0; j < bitmap.Height; ++j)
        {
          rgbHistogram[bitmap.GetPixel(i, j).R, 0]++;
          rgbHistogram[bitmap.GetPixel(i, j).G, 1]++;
          rgbHistogram[bitmap.GetPixel(i, j).B, 2]++;
        }
      }

      maxRgbHistogram = -1;
      int imageSize = bitmap.Height * bitmap.Width;
      for (int i = 0; i < RANGE_OF_8BITS; ++i)
      {
        for (int j = 0; j < 3; ++j)
        {
          if (rgbHistogram[i, j] > maxRgbHistogram)
          {
            maxRgbHistogram = rgbHistogram[i, j];
          }
        }
      }
      return rgbHistogram;
    }

    /// <summary>
    /// Get max histogram value
    /// </summary>
    /// <returns>Max histogram value</returns>
    public int getMaxHistogram()
    {
      if (maxHistogram < 0)
      {
        getHistogram();
      }
      return maxHistogram;
    }

    /// <summary>
    /// Get max RGB histogram value
    /// </summary>
    /// <returns>Max RGB histogram value</returns>
    public int getMaxRgbHistogram()
    {
      if (maxRgbHistogram < 0)
      {
        getRgbHistogram();
      }
      return maxRgbHistogram;
    }

    /// <summary>
    /// Get threshold value calculated by Otsu Algorithm
    /// </summary>
    /// <returns>Threshold value</returns>
    public int getOtsuThresholdValue()
    {
      if (otsuThresholdValue != -1)
      {
        return otsuThresholdValue;
      }
      // Make sure histogram is calculate
      getHistogram();

      // Calculate when threshold value = 0,
      // then calculate on the base of former outcome

      // Sum of pixels with index below thresholdValue 
      int belowSum = 0;
      // Sum of pixels with index upper than threshold value 
      int upperSum = accumulatedHistogram[RANGE_OF_8BITS - 1];

      // Sum of pixels multiplies index value below thresholdValue 
      int belowWeighted = 0;
      // Sum of pixels multiplies index value below threshold value 
      int upperWeighted = getWeightedHistogram();

      // Average of pixels below thresholdValue
      double belowAverage = 0;
      // Average of pixels upper than thresholdValue
      double upperAverage;
      if (upperSum == 0)
      {
        upperAverage = 0;
      }
      else
      {
        upperAverage = (double)upperWeighted / upperSum;
      }

      // Max variance between two parts
      double maxVarianceBetween = belowSum * upperSum
        * (belowAverage - upperAverage) * (belowAverage - upperAverage);
      otsuThresholdValue = 0;

      // Loop threshold value to get max variance between two parts
      for (int thresholdValue = 0; thresholdValue < RANGE_OF_8BITS; ++thresholdValue)
      {
        belowSum += histogram[thresholdValue];
        upperSum -= histogram[thresholdValue];

        if (belowSum == 0)
        {
          continue;
        }
        if (upperSum == 0)
        {
          break;
        }

        belowWeighted += histogram[thresholdValue] * thresholdValue;
        upperWeighted -= histogram[thresholdValue] * thresholdValue;

        belowAverage = (double)belowWeighted / belowSum;
        upperAverage = (double)upperWeighted / upperSum;

        // Variance between below and upper part
        double variaceBetween = (double)belowSum * upperSum
          * (belowAverage - upperAverage) * (belowAverage - upperAverage);

        // Update threshold value if variance between two parts are
        // larger than maxVarianceBetween
        if (variaceBetween > maxVarianceBetween)
        {
          maxVarianceBetween = variaceBetween;
          otsuThresholdValue = thresholdValue;
        }
      }
      return otsuThresholdValue;
    }

    /// <summary>
    /// Get threshold value calculated by Entropy Algorithm
    /// </summary>
    /// <returns>Threshold value</returns>
    public int getEntropyThresholdValue()
    {
      if (entropyThresholdValue != -1)
      {
        return entropyThresholdValue;
      }
      // Make sure histogram is calculate
      getHistogram();

      // Sum of x * log(x) with index below thresholdValue
      double belowSumXLogX = 0;
      // Sum of x * log(x) with index upper than threshold value
      double upperSumXLogX = 0;
      for (int i = 0; i < RANGE_OF_8BITS; ++i)
      {
        if (histogram[i] != 0)
        {
          // Log on base e
          upperSumXLogX += histogram[i] * Math.Log((double)histogram[i]);
        }
      }

      double maxEntropy = 0;

      for (int thresholdValue = 0; thresholdValue < RANGE_OF_8BITS; ++thresholdValue)
      {
        if (histogram[thresholdValue] == 0)
        {
          continue;
        }

        int upperSum = accumulatedHistogram[RANGE_OF_8BITS - 1]
                       - accumulatedHistogram[thresholdValue];
        if (upperSum == 0)
        {
          break;
        }

        double xLogX = histogram[thresholdValue] * Math.Log(histogram[thresholdValue]);
        belowSumXLogX += xLogX;
        upperSumXLogX -= xLogX;

        double belowEntropy = Math.Log(accumulatedHistogram[thresholdValue])
                              - belowSumXLogX / accumulatedHistogram[thresholdValue];
        double upperEntropy = Math.Log(upperSum)
                              - upperSumXLogX / upperSum;
        double entropy = belowEntropy + upperEntropy;

        if (entropy > maxEntropy)
        {
          entropyThresholdValue = thresholdValue;
          maxEntropy = entropy;
        }
      }
      return entropyThresholdValue;
    }

    /// <summary>
    /// Get binary bitmap with given threshold value,
    /// pixels with equal or less value than threshold value will be set 
    /// to be 0 and those with more value to be 255
    /// </summary>
    /// <param name="thresholdValue">Threshold value, -1 for former value</param>
    /// <returns></returns>
    public Bitmap getBinaryBitmap(int thresholdValue = -1)
    {
      if (binaryBitmap != null && thresholdValue == -1)
      {
        return binaryBitmap;
      }

      // If no former threshold value exist, set it to be 0
      if (thresholdValue == -1)
      {
        thresholdValue = 0;
      }
      this.thresholdValue = thresholdValue;

      binaryBitmap = new Bitmap(bitmap);
      Bitmap grayScaleBitmap = getGrayScaleBitmap();

      // Set pixels with value less than threshold value to be 0,
      // and more with 255
      for (int i = 0; i < grayScaleBitmap.Width; ++i)
      {
        for (int j = 0; j < grayScaleBitmap.Height; ++j)
        {
          Color color = grayScaleBitmap.GetPixel(i, j);
          if (color.R <= thresholdValue)
          {
            binaryBitmap.SetPixel(i, j, Color.Black);
          }
          else
          {
            binaryBitmap.SetPixel(i, j, Color.White);
          }
        }
      }
      return binaryBitmap;
    }
  }
}
