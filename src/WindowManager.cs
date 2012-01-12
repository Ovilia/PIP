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
    /// List of ImageWindow
    /// </summary>
    private System.Collections.ArrayList imageWindowList;


    /// <summary>
    /// Construtor
    /// </summary>
    public WindowManager()
    {
      windowList = new System.Collections.ArrayList();
      imageWindowList = new System.Collections.ArrayList();
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
    /// Get histogram if exist in windowList
    /// </summary>
    /// <returns>Histogram window</returns>
    public HistogramWindow getHistogramWindow()
    {
      if (windowList == null)
      {
        return null;
      }
      for (int i = 0; i < windowList.Count; ++i)
      {
        if (windowList[i].GetType() == typeof(HistogramWindow))
        {
          return (HistogramWindow)windowList[i];
        }
      }
      return null;
    }

    /// <summary>
    /// If windowList is empty
    /// </summary>
    /// <returns>True if windowList is empty</returns>
    public bool isWindowEmpty()
    {
      if (windowList.Count > 0)
      {
        return true;
      }
      else
      {
        return false;
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
      if (windowType == null)
      {
        return;
      }
      foreach (Form window in windowList)
      {
        if (window.GetType() == windowType)
        {
          window.Show();
          return;
        }
      }
      Form newWindow = createWindow(windowType);
      if (newWindow != null)
      {
        windowList.Add(newWindow);
        newWindow.Show();
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
      foreach (Form window in windowList)
      {
        window.Show();
      }
    }

    /// <summary>
    /// Dispose all windows in windowList
    /// </summary>
    public void disposeAllWindows()
    {
      Console.Write("Dispose");
      foreach (Form window in windowList)
      {
        window.Dispose();
      }
    }
  }
}
