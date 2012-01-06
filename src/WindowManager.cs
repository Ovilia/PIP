using System;

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
    /// Add window to windowList
    /// </summary>
    /// <param name="window">Window to be added</param>
    public void addWindow(Window window)
    {
      if (window == null) {
        return;
      }
      if (windowList == null) {
        windowList = new System.Collections.ArrayList();
      }
      if (!windowList.Contains(window)) {
        windowList.Add(window);
        window.Show();
      }
    }

    /// <summary>
    /// Remove window from windowList
    /// </summary>
    /// <param name="window">Window to be removed</param>
    public void removeWindow(Window window)
    {
      if (window == null) {
        return;
      }
      if (windowList != null) {
        if (windowList.Contains(window)) {
          windowList.Remove(window);
        }
      }
    }
  }
}
