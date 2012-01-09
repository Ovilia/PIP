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
    private float[] histogram;
    private float[,] rgbHistogram;
    private float maxHistogram;
    private float maxRgbHistogram;


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
    public float[] getHistogram()
    {
      if (histogram != null)
      {
        return histogram;
      }
      histogram = new float[RANGE_OF_8BITS];
      Bitmap grayScaleBitmap = getGrayScaleBitmap();
      for (int i = 0; i < grayScaleBitmap.Width; ++i)
      {
        for (int j = 0; j < grayScaleBitmap.Height; ++j)
        {
          histogram[grayScaleBitmap.GetPixel(i, j).R]++;
        }
      }

      maxHistogram = -1;
      float imageSize = (float)(grayScaleBitmap.Height * grayScaleBitmap.Width);
      for (int i = 0; i < RANGE_OF_8BITS; ++i)
      {
        histogram[i] /= imageSize;
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
    public float[,] getRgbHistogram()
    {
      if (rgbHistogram != null)
      {
        return rgbHistogram;
      }
      rgbHistogram = new float[RANGE_OF_8BITS, 3];
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
      float imageSize = (float)(bitmap.Height * bitmap.Width);
      for (int i = 0; i < RANGE_OF_8BITS; ++i)
      {
        for (int j = 0; j < 3; ++j)
        {
          rgbHistogram[i, j] /= imageSize;
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
    public float getMaxHistogram()
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
    public float getMaxRgbHistogram()
    {
      if (maxRgbHistogram < 0)
      {
        getRgbHistogram();
      }
      return maxRgbHistogram;
    }
  }
}
