using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using jacobsenroad;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace pinefarm_gui_c
{
    class NetClient
    {
    }

    /// <summary>
    /// 
    /// </summary>
    public class Scheme3 : SynchronousSocketClient
    {
        public event OnReceiveData OnReceiveDataClient;

        public Scheme3 Client { get; set; }
        private bool inLoop = true;
        public Scheme3()
        {
            OnReceiveData += OnReceiveDataHandler;
            OnSendData += OnSendDataHandler;
        }

        private string OnReceiveDataHandler(string msg)
        {
            if (OnReceiveDataClient != null)
            {
                OnReceiveDataClient(msg);
            }
            return msg;
        }

        private string OnSendDataHandler(string msg)
        {
            return msg;
        }

        public void Start()
        {
            string nm = Datagen.GetFirstName();
            string msg = "";
            Client = new Scheme3();
            //client.HostName = "74.208.133.86";
            Client.HostName = "127.0.0.1";
            Client.EnableDataGenerator = false;
            Client.TimerInterval = 200;
            Thread t = new Thread(new ThreadStart(Client.StartClient));
            t.Start();
            msg = nm;
            Thread.Sleep(1000);
            string sendmsg = string.Format("LOGIN . CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length, msg);
            Client.Send(sendmsg);
            //while (inLoop)
            //{
            //    Thread.Sleep(1000);
            //    msg = Datagen.GetComment();
            //    client.Send(string.Format("SAY . CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length, msg));
            //}
            //t.Join();
        }
    }
}
