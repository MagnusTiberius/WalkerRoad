using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using jacobsenroad;

namespace smtp_c
{
    class Program
    {
        static void Main(string[] args)
        {
            bool inLoop = true;
            int ctrStep = 0;
            SmtpClient client = new SmtpClient();
            client.HostName = "127.0.0.1";
            client.EnableDataGenerator = false;
            client.TimerInterval = 200;
            client.PortNumber = 2626;
            Thread t = new Thread(new ThreadStart(client.StartClient));
            t.Start();
            t.Join();
            //Thread.Sleep(5000);
            //while (inLoop)
            //{
            //    if(ctrStep==0)
            //    {
            //        int a = 1;
            //        //string sendmsg = string.Format("LOGIN /lobby CHAT/1.0\nname={0}\ncontent-length:{1}\n\n{2}\n\n", nm, msg.Length, msg);
            //        //client.Send(sendmsg);
            //        //string msg = "HELO";
            //        //Console.WriteLine(string.Format("Sending {0}", msg));
            //        //client.Send(msg);
            //        //ctrStep++;
            //    }
            //}
        }
    }
}
