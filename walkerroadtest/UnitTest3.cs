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

            Thread threadServer = new Thread(new ThreadStart(listener.StartListening));
            Thread threadClient = new Thread(new ThreadStart(client.StartClient));

            threadServer.Start();
            Thread.Sleep(100);
            threadClient.Start();
            threadClient.Join();
            threadServer.Abort();
        }
    }
}
