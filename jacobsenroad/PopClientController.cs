using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Timers;
using System.Configuration;

namespace jacobsenroad
{
    public class PopClientController
    {
        Thread worker = null;
        Thread client = null;
        SmtpClient smtpClient = null;
        Stack<string> commandList = new Stack<string>();

        public PopClientController()
        {
            smtpClient = new SmtpClient();
            smtpClient.SmtpOnReceiveData += OnReceiveDataHandler;
            smtpClient.SmtpOnSendData += OnSendDataHandler;
        }

        public PopClientController(string environmentTams, string environmentEpicor)
        {
            smtpClient = new SmtpClient();
            smtpClient.SmtpOnReceiveData += OnReceiveDataHandler;
            smtpClient.SmtpOnSendData += OnSendDataHandler;
        }

        public void Init(string[] args)
        {

        }

        public void Run()
        {
            commandList.Push("LIST");

            smtpClient.HostName = "127.0.0.1";
            smtpClient.PortNumber = 9110;
            client = new Thread(new ThreadStart(smtpClient.StartClient));
            client.Start();

            worker = new Thread(new ThreadStart(WorkerThread));
            worker.Start();
            worker.Join();
        }

        public void DeInit()
        {

        }

        public void WorkerThread()
        {
            while (true)
            {
                Thread.Sleep(30 * 1000);
                if (smtpClient.IsConnected)
                {
                    if (commandList.Count > 0)
                    {
                        string cmd = commandList.Pop();
                        smtpClient.Send(cmd);
                    }
                }
            }

        }

        private string OnReceiveDataHandler(string msg)
        {
            commandList.Push("RETR 1");
            return null;
        }

        private string OnSendDataHandler(string msg)
        {
            return null;
        }
    }

}
