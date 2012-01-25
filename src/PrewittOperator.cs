using System;
using System.Drawing;

namespace PIP
{
  class PrewittOperator : Filter
  {
    public PrewittOperator(ImageProcessor imageProcessor)
      : base(imageProcessor)
    {
      // Last and last column and row remains as origin one
      int width = filteredBitmap.Width - 1;
      int height = filteredBitmap.Height - 1;
      for (int i = 1; i < width; ++i)
      {
        for (int j = 1; j < height; ++j)
        {
          int[] neighbor = new int[9];
          for (int s = 0; s < 3; ++s)
          {
            for (int t = 0; t < 3; ++t)
            {
              neighbor[3 * s + t] = 
                filteredBitmap.GetPixel(i - 1 + s, j - 1 + t).R;
            }
          }
          int gx = (neighbor[6] + neighbor[7] + neighbor[8])
            - (neighbor[0] + neighbor[1] + neighbor[2]);
          int gy = (neighbor[2] + neighbor[5] + neighbor[8])
            - (neighbor[0] + neighbor[3] + neighbor[6]);

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
