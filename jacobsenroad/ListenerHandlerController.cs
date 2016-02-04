using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Timers;

namespace jacobsenroad
{
    static public class ListenerHandlerController
    {
        static private List<ListenerHandler> list = new List<ListenerHandler>();
        static public int InstanceCount {get; set;}
        static public ListenerHandler Next(Socket socket)
        {
            foreach (ListenerHandler itm in list)
            {
                if (itm.IsAvailable)
                {
                    itm.SetSocket(socket);
                    return itm;
                }
            }
            ListenerHandler newHandler = new ListenerHandler(socket);
            list.Add(newHandler);
            InstanceCount++;
            return newHandler;
        }
    }
}
