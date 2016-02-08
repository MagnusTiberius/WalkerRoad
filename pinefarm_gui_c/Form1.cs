using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using jacobsenroad;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace pinefarm_gui_c
{
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

    public partial class Form1 : Form
    {
        private Scheme3 client;

        public Form1()
        {
            InitializeComponent();
            client = new Scheme3();
            client.Start();
            client.Client.OnReceiveDataClient += OnReceiveDataHandler;
            Control.CheckForIllegalCrossThreadCalls = false;

            txtChat.ScrollBars = ScrollBars.Vertical;
        }

        private string OnReceiveDataHandler(string msg)
        {
            txtChat.AppendText( string.Format("{0}{1}{1}", msg, Environment.NewLine, Environment.NewLine));
            txtChat.Refresh();
            return msg;
        }
        private void btnSend_Click(object sender, EventArgs e)
        {

        }

        private void txtChat_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
