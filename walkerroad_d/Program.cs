using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.Threading;
using jacobsenroad;

namespace walkerroad_d
{
    class Program
    {
        static void Main(string[] args)
        {
            SynchronousSocketListener listener = new SynchronousSocketListener();
            listener.HostName = "127.0.0.1";
            Thread threadServer = new Thread(new ThreadStart(listener.StartListening));
            threadServer.Start();
            threadServer.Join();
        }
    }
}
