using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace jacobsenroad
{
    public static class EventLog
    {

        public static void Write(string message)
        {
            string sSource = "Listener Service";
            string sLog = "Application";
            string sEvent = "Event";

            if (!System.Diagnostics.EventLog.SourceExists(sSource))
                System.Diagnostics.EventLog.CreateEventSource(sSource, sLog);

            System.Diagnostics.EventLog.WriteEntry(sSource, message);
        }
    }
}
