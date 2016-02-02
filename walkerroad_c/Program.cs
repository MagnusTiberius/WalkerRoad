﻿using System;
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
                Thread threadClient1 = new Thread(new ThreadStart(client.StartClient));
                threadClient1.Start();
                threadClient1.Join();
            }
        }
    }
}
