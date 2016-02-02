using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using walkerroadlib;

using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using jacobsenroad;

namespace walkerroadtest
{
    [TestClass]
    public class UnitTest3
    {
        const int SZ = 1;

        [TestMethod]
        public void TestMethod198()
        {
            List<Thread> list = new List<Thread>();

            SynchronousSocketListener listener = new SynchronousSocketListener();
            Thread threadServer = new Thread(new ThreadStart(listener.StartListening));

            for (int i = 0; i < SZ; i++ )
            {
                SynchronousSocketClient client = new SynchronousSocketClient();
                Thread threadClient1 = new Thread(new ThreadStart(client.StartClient));
                threadClient1.Start();
                list.Add(threadClient1);
            }

            threadServer.Start();
            Thread.Sleep(100);

            foreach(Thread t in list)
            {
                t.Join();
            }
            threadServer.Abort();
        }
    }
}
