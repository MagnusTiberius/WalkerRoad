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
    public class CommunicationChannel
    {
        public string Name { get; set; }
        public string Description { get; set; }
        private Stack<string> listMessage = new Stack<string>();
        private int ctr1 = 0;
        private int tid;

        public CommunicationChannel()
        {
            tid = Thread.CurrentThread.ManagedThreadId;
        }

        public void Put(string msg)
        {
            lock (this)
            {
                int c = ctr1++;
                Console.WriteLine(string.Format("{0}:Put:{1}:{2}", tid, msg, c));
                //listMessage.Push(string.Format("{0}::{1}", msg, c));
                listMessage.Push(string.Format("{0}", msg));
            }
        }

        public string Pickup()
        {
            lock (this)
            {
                if (listMessage.Count == 0)
                {
                    return null;
                }
                string msg = listMessage.Pop();
                return string.Format("[{0}] {1}", Name, msg);
            }
        }
    }
}
