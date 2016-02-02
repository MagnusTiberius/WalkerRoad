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
            SynchronousSocketListener w = new SynchronousSocketListener();
            Thread t = new Thread(new ThreadStart(w.StartListening));
            t.Start();
            t.Join();
        }
    }
}
