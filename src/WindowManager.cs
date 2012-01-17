using System;
using System.Windows.Forms;

namespace PIP
{
  class WindowManager
  {
    private HistogramWindow histogramWindow;
    /// <summary>
    /// List of ImageWindow
    /// </summary>
    private System.Collections.ArrayList imageWindowList;


    /// <summary>
    /// Construtor
    /// </summary>
    public WindowManager()
    {
      imageWindowList = new System.Collections.ArrayList();
    }
    
    /// <summary>
    /// Get histogram if exist in windowList
    /// </summary>
    /// <returns>Histogram window</returns>
    public HistogramWindow getHistogramWindow()
    {
      if (histogramWindow == null)
      {
        histogramWindow = new HistogramWindow();
      }
      return histogramWindow;
    }

    /// <summary>
    /// Show all initialized windows
    /// </summary>
    public void showAll()
    {
      if (histogramWindow != null)
      {
        histogramWindow.Show();
      }
    }

    /// <summary>
    /// Dispose all initialized windows
    /// </summary>
    public void disposeAll()
    {
      if (histogramWindow != null)
      {
        histogramWindow.Dispose();
      }
    }

    /// <summary>
    /// If imageWindowList is empty
    /// </summary>
    /// <returns>True if imageWindowList is empty</returns>
    public bool isImageWindowEmpty()
    {
      if (imageWindowList.Count > 0)
      {
        return false;
      }
      else
      {
        return true;
      }
    }

    /// <summary>
    /// Add ImageWindow to windowList, multi-ImageWindow supported
    /// </summary>
    /// <param name="imageWindow">ImageWindow to be added</param>
    public void addImageWindow(ImageWindow imageWindow)
    {
      if (imageWindow != null)
      {
        imageWindowList.Add(imageWindow);
        imageWindow.Show();
      }
    }

    /// <summary>
    /// Focus the given ImageWindow,
    /// set [+] before the focused ImageWindow
    /// </summary>
    /// <param name="imageWindow">ImageWindow to be focused</param>
    public void focusImageWindow(ImageWindow imageWindow)
    {
      if (imageWindow == null || imageWindowList == null)
      {
        return;
      }
      foreach (ImageWindow window in imageWindowList)
      {
        if (window == imageWindow)
        {
          // do nothing if was focused
          if (window.isFocused)
          {
            return;
          }
          window.isFocused = true;
          window.Text = "[+]" + window.Text;
          // change histogram to current image
          HistogramWindow histogramWindow = getHistogramWindow();
          if (histogramWindow != null && histogramWindow.Visible)
          {
            histogramWindow.updateImage();
          }
        }
        else
        {
          if (window.isFocused)
          {
            window.Text = window.Text.Substring(3);
          }
          window.isFocused = false;
        }
      }
    }

    /// <summary>
    /// Return the focused ImageWindow
    /// </summary>
    /// <returns>Focused ImageWindow</returns>
    public ImageWindow getFocusedImageWindow()
    {
      if (imageWindowList == null)
      {
        return null;
      }
      foreach (ImageWindow window in imageWindowList)
      {
        if (window.isFocused == true)
        {
          return window;
        }
      }
      return null;
    }

    /// <summary>
    /// Remove window from imageWindowList
    /// </summary>
    /// <param name="imageWindow">ImageWindow to be removed</param>
    /// <returns>True if removed</returns>
    public bool removeImageWindow(ImageWindow imageWindow)
    {
      if (imageWindow == null || imageWindowList == null)
      {
        return false;
      }
      imageWindowList.Remove(imageWindow);
      imageWindow.Dispose();
      return true;
    }

    /// <summary>
    /// Show all windows in windowList
    /// </summary>
    public void showAllWindows()
    {
      if (histogramWindow != null)
      {
        histogramWindow.Show();
      }
    }
  }
}
