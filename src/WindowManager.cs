using System;
using System.Windows.Forms;

namespace PIP
{
  class WindowManager
  {
    /// <summary>
    /// List of Window, e.g.: Toolkit window, history window...
    /// </summary>
    private System.Collections.ArrayList windowList;


    /// <summary>
    /// Construtor
    /// </summary>
    public WindowManager()
    {
      windowList = new System.Collections.ArrayList();
    }
    
    /// <summary>
    /// If windowList contains window of given type
    /// </summary>
    /// <param name="windowType">Type of window</param>
    /// <returns>If windowList contains window</returns>
    public bool hasWindow(Type windowType)
    {
      if (windowType == null || windowList == null)
      {
        return false;
      }
      foreach (Form window in windowList)
      {
        if (window.GetType() == windowType)
        {
          return true;
        }
      }
      return false;
    }

    /// <summary>
    /// Create window of given type
    /// </summary>
    /// <param name="windowType">Type of window</param>
    /// <returns>Window created</returns>
    public Form createWindow(Type windowType)
    {
      if (windowType == typeof(HistogramWindow))
      {
        return new HistogramWindow();
      }
      else if (windowType == typeof(ImageWindow))
      {
        return new ImageWindow();
      }
      else
      {
        return null;
      }
    }

    /// <summary>
    /// Add window to windowList, each type has only one instance in windowList
    /// </summary>
    /// <param name="windowType">Type of window to be added</param>
    public void addWindow(Type windowType)
    {
      if (windowType == null || hasWindow(windowType))
      {
        return;
      }
      Form window = createWindow(windowType);
      if (window != null)
      {
        windowList.Add(window);
        window.Show();
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
        windowList.Add(imageWindow);
        imageWindow.Show();
      }
    }

    /// <summary>
    /// Remove window from windowList
    /// </summary>
    /// <param name="window">Window to be removed</param>
    /// <returns>True if windowList contains window of given type</returns>
    public bool removeWindow(Type windowType)
    {
      if (windowType == null || windowList == null)
      {
        return false;
      }
      foreach (Form window in windowList)
      {
        if (window.GetType() == windowType)
        {
          windowList.Remove(window);
          window.Dispose();
          return true;
        }
      }
      return false;
    }

    /// <summary>
    /// Show all windows in windowList
    /// </summary>
    public void showAllWindows()
    {
      foreach (Form window in windowList)
      {
        window.Show();
      }
    }
  }
}
