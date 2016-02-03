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
    /// <summary>
    /// 
    /// </summary>
    public class Scheme1
    {
        public Thread Start()
        {
            SynchronousSocketClient client = new SynchronousSocketClient();
            client.EnableDataGenerator = true;
            while (true)
            {
                Thread t = new Thread(new ThreadStart(client.StartClient));
                t.Start();
                //t.Join();
                return t;
            }
        }
    }
    
    /// <summary>
    /// 
    /// </summary>
    public class Scheme2 : SynchronousSocketClient
    {
        private Scheme2 client;

        public Scheme2()
        {
            OnReceiveData += OnReceiveDataHandler;
            OnSendData += OnSendDataHandler;
            OnClientLoop += OnClientLoopHandler;
        }

        private string OnReceiveDataHandler(string msg)
        {
            return msg;
        }

        private string OnSendDataHandler(string msg)
        {
            return msg;
        }

        private string OnClientLoopHandler()
        {
            //return Datagen.GetComment();
            return "My own handler.";
        }

        public Thread Start()
        {
            client = new Scheme2();
            client.EnableDataGenerator = true;
            client.TimerInterval = 200;
            while (true)
            {
                Thread t = new Thread(new ThreadStart(client.StartClient));
                t.Start();
                t.Join();
            }
        }
    }

    /// <summary>
    /// 
    /// </summary>
    class Program
    {

        static void Main(string[] args)
        {
            Scheme2 s = new Scheme2();
            Thread t = s.Start();
        }
    }
}
