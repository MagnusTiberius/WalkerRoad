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
        private bool inLoop = true;
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
            return Datagen.GetComment();
            //return "My own handler.";
        }

        public void Start()
        {
            string nm = Datagen.GetFirstName();
            client = new Scheme2();
            client.EnableDataGenerator = false;
            client.TimerInterval = 200;
            Thread t = new Thread(new ThreadStart(client.StartClient));
            t.Start();
            t.Join();
        }
    }


    /// <summary>
    /// 
    /// </summary>
    public class Scheme3 : SynchronousSocketClient
    {
        private Scheme3 client;
        private bool inLoop = true;
        public Scheme3()
        {
            OnReceiveData += OnReceiveDataHandler;
            OnSendData += OnSendDataHandler;
        }

        private string OnReceiveDataHandler(string msg)
        {
            return msg;
        }

        private string OnSendDataHandler(string msg)
        {
            return msg;
        }

        public void Start()
        {
            string nm = Datagen.GetFirstName();
            client = new Scheme3();
            //client.HostName = "74.208.133.86";
            client.HostName = "127.0.0.1";
            client.EnableDataGenerator = false;
            client.TimerInterval = 200;
            Thread t = new Thread(new ThreadStart(client.StartClient));
            t.Start();
            while (inLoop)
            {
                Thread.Sleep(1000);
                string msg = Datagen.GetComment();
                client.Send(string.Format("SAY . CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length ,msg));
            }
            t.Join();
        }
    }
    
    /// <summary>
    /// 
    /// </summary>
    class Program
    {

        static void Main(string[] args)
        {
            int v = 3;

            if (v == 1)
            {
                Scheme2 s = new Scheme2();
                s.Start();
            }
            if (v == 2)
            {
                Scheme1 s = new Scheme1();
                Thread t = s.Start();
                t.Join();
            }
            if (v == 3)
            {
                Scheme3 s = new Scheme3();
                s.Start();
            }
        }
    }
}
