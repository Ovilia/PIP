using System;
using System.Drawing;

namespace PIP
{
  class RobertsOperator : Filter
  {
    public RobertsOperator(ImageProcessor imageProcessor)
      : base(imageProcessor)
    {
      // Last column and last row remains as origin one
      int width = filteredBitmap.Width - 1;
      int height = filteredBitmap.Height - 1;
      for (int i = 0; i < width; ++i)
      {
        for (int j = 0; j < height; ++j)
        {
          int color5 = filteredBitmap.GetPixel(i, j).R;
          int color9 = filteredBitmap.GetPixel(i + 1, j + 1).R;
          int gx = color9 - color5;

          int color8 = filteredBitmap.GetPixel(i, j + 1).R;
          int color6 = filteredBitmap.GetPixel(i + 1, j).R;
          int gy = color8 - color6;

          int mag = (int)Math.Sqrt(gx * gx + gy * gy);
          if (mag >= ImageProcessor.RANGE_OF_8BITS)
          {
            mag = ImageProcessor.RANGE_OF_8BITS - 1;
          }
          Color color = Color.FromArgb(mag, mag, mag);

          filteredBitmap.SetPixel(i, j, color);
        }
      }
    }
  }
}
