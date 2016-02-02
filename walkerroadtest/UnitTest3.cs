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
        [TestMethod]
        public void TestMethod1()
        {
            SynchronousSocketListener listener = new SynchronousSocketListener();
            SynchronousSocketClient client = new SynchronousSocketClient();
            SynchronousSocketClient client2 = new SynchronousSocketClient();
            SynchronousSocketClient client3 = new SynchronousSocketClient();

            Thread threadServer = new Thread(new ThreadStart(listener.StartListening));
            Thread threadClient1 = new Thread(new ThreadStart(client.StartClient));
            Thread threadClient2 = new Thread(new ThreadStart(client2.StartClient));
            Thread threadClient3 = new Thread(new ThreadStart(client3.StartClient));

            threadServer.Start();
            Thread.Sleep(100);

            threadClient1.Start();
            threadClient2.Start();
            threadClient3.Start();

            threadClient1.Join();
            threadClient2.Join();
            threadClient3.Join();
            threadServer.Abort();
        }
    }
}
