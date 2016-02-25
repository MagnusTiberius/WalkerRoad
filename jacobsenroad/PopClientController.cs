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
        public bool IsBusy { get; set; }
        private List<Attr> _props = new List<Attr>();

        public struct Attr
        {
            public string name;
            public string value;
        }

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
            IsBusy = false;
            smtpClient.HostName = "127.0.0.1";
            smtpClient.PortNumber = 9110;
            client = new Thread(new ThreadStart(smtpClient.StartClient));
            client.Start();

            worker = new Thread(new ThreadStart(WorkerThread));
            worker.Start();
            worker.Join();
            client.Join();
        }

        public void DeInit()
        {

        }


        public void WorkerThread()
        {
            while (true)
            {
                Thread.Sleep(5 * 1000);
                if (smtpClient.IsConnected)
                {
                    if (!IsBusy)
                    {
                        if (commandList.Count > 0)
                        {
                            string cmd = commandList.Pop();
                            smtpClient.Send(cmd);
                        }
                        else
                        {
                            commandList.Push("LIST\n");
                        }
                    }
                }
            }
        }


        private string OnReceiveDataHandler(string msg)
        {
            IsBusy = true;

            int disp = msg.IndexOf("POP3 server ready");

            if (msg.IndexOf("POP3 server ready") > 0)
            {
                commandList.Push("LIST\n");
            }

            if (msg.IndexOf("messages") >= 0 && msg.IndexOf("+Ok") >= 0)
            {
                Scanner scanner = new Scanner();
                scanner.Input(msg);
                byte[] btoken = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-")).ToArray();
                string stoken = ASCIIEncoding.ASCII.GetString(btoken);
                scanner.SkipEmpty();
                btoken = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("0123456789+-")).ToArray();
                stoken = ASCIIEncoding.ASCII.GetString(btoken);

                int n = int.Parse(stoken);

                if (n > 0)
                {
                    commandList.Push("RETR 1\n");
                }
            }

            if (msg.IndexOf("[mailto:kaseya@rgsupport.com]") >= 0)
            {
                List<Attr> attrs = ParseIt(ASCIIEncoding.ASCII.GetBytes(msg));
                commandList.Push("DELE 1\n");
                int j = 1;
            }

            IsBusy = false;

            return null;
        }

        public List<Attr> ParseIt(byte[] inputStr)
        {
            Scanner scanner = new Scanner();
            scanner.Input(inputStr);
            List<Attr> props = new List<Attr>();
            List<byte> token;
            string tok1;
            Attr atr;

            for (int i = 0; i < 4; i++)
            {
                atr = new Attr();

                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                atr.name = tok1;

                scanner.SkipEmpty();
                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                scanner.SkipEmpty();
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                atr.value = tok1;

                props.Add(atr);

                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\r"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                scanner.SkipEmpty();
            }
            token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\r"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("\n"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            //token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;_$% "));
            token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            scanner.SkipEmpty();

            //------------------

            for (int i = 0; i < 9; i++)
            {
                atr = new Attr();
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                atr.name = tok1;

                scanner.SkipEmpty();
                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());

                scanner.SkipEmpty();
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                atr.value = tok1;

                props.Add(atr);
                scanner.SkipEmpty();
            }
            return props;
        }

        private string OnSendDataHandler(string msg)
        {
            return null;
        }
    }

}
