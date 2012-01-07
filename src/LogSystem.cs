using System;
using System.Collections;

namespace PIP
{
  class LogSystem
  {
    static private LogSystem logSystem;

    private ArrayList logList;


    /// <summary>
    /// Private constructor for singleton
    /// </summary>
    private LogSystem()
    {
      logList = new ArrayList();
    }

    /// <summary>
    /// Get LogSystem singleton
    /// </summary>
    /// <returns>LogSystem instance</returns>
    static public LogSystem getInstance()
    {
      if (logSystem == null)
      {
        logSystem = new LogSystem();
      }
      return logSystem;
    }

    /// <summary>
    /// Write into LogSystem
    /// </summary>
    /// <param name="actionType">Action to be written</param>
    public void writeLog(ActionType actionType)
    {
      logList.Add(actionType);
    }

    /// <summary>
    /// Read last n log items
    /// </summary>
    /// <param name="count">Count of log items to be read</param>
    /// <returns>ArrayList of log items</returns>
    public ArrayList readLastNLogs(int count)
    {
      return logList.GetRange(logList.Count - count, count);
    }

    public enum ActionType
    {
      OPEN_FILE = 0,
      SAVE_FILE
    }

  }
}
