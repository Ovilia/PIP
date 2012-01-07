using System;
using System.Drawing;

namespace PIP
{
  class ImageProcessor
  {
    private Bitmap bitmap;


    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="bitmap">Bitmap of the image to be processed</param>
    public ImageProcessor(Bitmap bitmap)
    {
      this.bitmap = bitmap;
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
      Bitmap grayBitmap = new Bitmap(bitmap.Width, bitmap.Height);
      Console.WriteLine("Width:" + bitmap.Size.Width + "\theight:" + bitmap.Size.Height);
      for (int i = 0; i < bitmap.Width; ++i)
      {
        for (int j = 0; j < bitmap.Height; ++j)
        {
          Color color = bitmap.GetPixel(i, j);
          int grayScale = (int)(color.R * 0.3 + color.G * 0.59
            + color.B * 0.11);
          Color grayColor = Color.FromArgb(grayScale, grayScale, grayScale);
          grayBitmap.SetPixel(i, j, grayColor);
        }
      }
      return grayBitmap;
    }
  }
}
