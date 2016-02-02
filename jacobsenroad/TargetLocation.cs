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
    public class TargetLocation
    {
        static readonly TargetLocation _instance = new TargetLocation();
        private int initv = 0;
        private int tid;
        private int ctr1 = 0;
        public static TargetLocation Instance
        {
            get
            {
                return _instance;
            }
        }

        Stack<string> listMessage = new Stack<string>();
        List<ListenerHandler> listConnection = new List<ListenerHandler>();

        private void TargetLocationInit()
        {
            tid = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("TargetLocation ctor()");
            Thread threadServer = new Thread(new ThreadStart(Looper));
            threadServer.Start();
        }

        private void Looper()
        {
            while (true)
            {
                lock (this)
                {
                    if (listMessage.Count == 0)
                    {
                        continue;
                    }
                    string msg = listMessage.Pop();
                    Console.WriteLine(string.Format("{0}:Update:{1}", tid, msg));
                    foreach (ListenerHandler h in listConnection)
                    {
                        if (h != null)
                        {
                            try
                            {
                                h.Update(string.Format("{0}::{1}", msg, ctr1));
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine(ex.Message);
                            }
                        }
                    }
                    Console.WriteLine("==========================================");
                }
                //foreach (string msg in listMessage)
                //{
                //    foreach (ListenerHandler h in listConnection)
                //    {
                //        //h.Update(msg);
                //        Console.WriteLine(string.Format("Update:{0}", msg));
                //    }
                //}
            }
        }

        public void Register(ListenerHandler h)
        {
            lock (this)
            {
                if (initv == 0)
                {
                    TargetLocationInit();
                }

                listConnection.Add(h);
            }
        }

        public void Put(string msg)
        {
            lock (this)
            {
                int c = ctr1++;
                Console.WriteLine(string.Format("{0}:Put:{1}:{2}", tid, msg, c));
                listMessage.Push(string.Format("{0}{1}", msg, c));
            }
        }
    }
}
