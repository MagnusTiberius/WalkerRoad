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
            Console.WriteLine(string.Format("\n\n{0}\n-----------------\n\n", msg));
            Thread.Sleep(500);
            return msg;
        }

        private string OnSendDataHandler(string msg)
        {
            return msg;
        }

        public void Start()
        {
            int stepCtr1 = 0;
            string nm = Datagen.GetFirstName();
            string msg = "";
            client = new Scheme3();
            //client.HostName = "74.208.133.86";
            client.HostName = "127.0.0.1";
            client.EnableDataGenerator = false;
            client.TimerInterval = 200;
            Thread t = new Thread(new ThreadStart(client.StartClient));
            t.Start();
            msg = nm;
            Thread.Sleep(1000);
            string sendmsg = string.Format("LOGIN /lobby CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length, msg);
            client.Send(sendmsg);
            while (inLoop)
            {
                Thread.Sleep(1000);
                msg = Datagen.GetComment();
                client.Send(string.Format("SAY /lobby CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length ,msg));
                Thread.Sleep(1000);
                string msg2 = GameStep(stepCtr1, nm);
                //Console.WriteLine(string.Format("=========================\nSending===>{0}\n\n", msg2));
                client.Send(msg2);
                stepCtr1++;
            }
            t.Join();
        }

        private string GameStep(int n, string nm)
        {
            string msg;
            string sendmsg = ""; ;
            if (n == 0)
            {
                msg = string.Format("SPAWNING {0}", nm);
                sendmsg = string.Format("SPAWN /portland GAME/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length, msg);
            }
            else
            {
                msg = string.Format("MOVING {0} X COORD : {1}", nm, n);
                sendmsg = string.Format("MOVE /portland GAME/1.0\nname={0}\ncontent-length:{1}\ncoord-x:{2}\n\n{3}\n\n", nm, msg.Length, n, msg);
            }
            return sendmsg;
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
