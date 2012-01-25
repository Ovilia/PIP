using System;
using System.Drawing;

namespace PIP
{
  class Filter
  {
    protected ImageProcessor imageProcessor;

    protected Bitmap filteredBitmap;
    public Bitmap FilteredBitmap
    {
      get
      {
        return filteredBitmap;
      }
    }


    /// <summary>
    /// Constructor of Filter
    /// </summary>
    /// <param name="imageProcessor">ImageProcessor to detail with 
    /// basic image processing work</param>
    public Filter(ImageProcessor imageProcessor)
    {
      this.imageProcessor = imageProcessor;
      filteredBitmap = imageProcessor.getGrayScaleBitmap();
    }
  }
}
