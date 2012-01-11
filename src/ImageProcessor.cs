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
    private int[,] rgbHistogram;
    private int maxHistogram;
    private int maxRgbHistogram;

    private int thresholdValue;
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
      this.bitmap = bitmap;
      maxHistogram = -1;
      maxRgbHistogram = -1;
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
      
      /*Graphics g = Graphics.FromImage(grayScaleBitmap);
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
    */
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
      Bitmap grayScaleBitmap = getGrayScaleBitmap();
      for (int i = 0; i < grayScaleBitmap.Width; ++i)
      {
        for (int j = 0; j < grayScaleBitmap.Height; ++j)
        {
          histogram[grayScaleBitmap.GetPixel(i, j).R]++;
        }
      }

      maxHistogram = -1;
      int imageSize = grayScaleBitmap.Height * grayScaleBitmap.Width;
      for (int i = 0; i < RANGE_OF_8BITS; ++i)
      {
        if (histogram[i] > maxHistogram)
        {
          maxHistogram = histogram[i];
        }
      }
      return histogram;
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
            color = Color.Black;
          }
          else
          {
            color = Color.White;
          }
          binaryBitmap.SetPixel(i, j, color);
        }
      }
      return binaryBitmap;
    }
  }
}
