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
            if (txtChat.TextLength > 8000)
            {
                txtChat.Clear();
            }
            txtChat.AppendText( string.Format("{0}{1}{1}", msg, Environment.NewLine, Environment.NewLine));
            txtChat.Refresh();
            return msg;
        }
        private void btnSend_Click(object sender, EventArgs e)
        {
            string nm = client.Client.ClientName;
            string msg = txtInput.Text;
            string pack = string.Format("SAY . CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length, msg);
            client.Client.Send(pack);
            txtInput.Clear();
            txtInput.Refresh();
        }

        private void txtChat_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
