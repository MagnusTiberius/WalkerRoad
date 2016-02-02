using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using jacobsenroad;

namespace walkerroad_c
{
    class Program
    {
        static void Main(string[] args)
        {
            SynchronousSocketClient client = new SynchronousSocketClient();
            while (true)
            {
                Thread t = new Thread(new ThreadStart(client.StartClient));
                t.Start();
                t.Join();
            }
        }
    }
}
