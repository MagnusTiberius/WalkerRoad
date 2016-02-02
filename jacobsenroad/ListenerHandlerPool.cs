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
    public class ListenerHandlerPool
    {
        static readonly ListenerHandlerPool _instance = new ListenerHandlerPool();
        public static ListenerHandlerPool Instance
        {
            get
            {
                return _instance;
            }
        }

        private ListenerHandlerPool()
        {
            list = new List<ListenerHandler>();
        }

        private List<ListenerHandler> list;

        public void Add(Socket socket)
        {
            try
            {
                foreach (ListenerHandler itm in list)
                {
                    if (itm.IsAvailable)
                    {
                        itm.SetSocket(socket);
                        Thread thread1 = new Thread(new ThreadStart(itm.Start));
                        thread1.Start();
                        return;
                    }
                }
                ListenerHandler h = new ListenerHandler(socket);
                list.Add(h);
                Thread thread = new Thread(new ThreadStart(h.Start));
                thread.Start();
            }
            catch (Exception e)
            {
                System.Diagnostics.Debug.WriteLine(string.Format("Error ListenerHandler 23 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
            }
            return;
        }
    }
}
