using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Timers;
using System.Configuration;
using System.ServiceProcess;

namespace jacobsenroad
{
    public class PopClientController
    {
        int sleepCtr = 0;

        const string sSource = "KaseyaEpicor Listener Service";
        const string sLog = "Application";
        const string sEvent = "Event";

        const string HOST = "smtp.company.com";
        const string LOCALHOST = "127.0.0.1";

        const string ENTER_USER = "USER test";
        const string ENTER_PWD = "PASS werewr";

        const int POP3PORTNUM = 110;
        const int POP3PORTNUM_TEST = 9110;

        private string status = "READY";

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
            smtpClient.HostName = HOST;
            smtpClient.PortNumber = POP3PORTNUM;
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
                            int res = smtpClient.Send(string.Format("{0}\r\n", cmd));
                            //KaseyaEpicorLib.EventLog.Write(string.Format("SEND: {0}", cmd));
                            Console.WriteLine(string.Format("SEND: {0}", cmd));
                            Console.WriteLine(string.Format("BYTES SENT: {0}", res));
                        }
                        else
                        {
                            commandList.Push("QUIT");
                        }
                    }
                }
            }
        }


        private string OnReceiveDataHandler(string msg)
        {
            //KaseyaEpicorLib.EventLog.Write(string.Format("STATUS: {0}; RECV: {1}", status, msg));
            Console.WriteLine(string.Format("STATUS: {0}; RECV: {1}", status, msg));
            //Console.WriteLine(string.Format("RECV: {0}", msg));
            IsBusy = true;
            string sEpicorCaseNum = "";
            int nEpicorCaseNum = 0;
            bool isClosed = false;
            int disp = msg.IndexOf("POP3 service is ready");

            if (disp > 0)
            {
                Console.WriteLine("0001");
                commandList.Push(string.Format("{0}", ENTER_USER));
                status = "ENTER_USER";
            }
            else if (msg.IndexOf("-ERR") >= 0 && status.Equals("ENTER_USER"))
            {
                Console.WriteLine("0002");
                commandList.Push(string.Format("{0}", ENTER_USER));
                status = "ENTER_USER";
            }
            else if (msg.IndexOf("+OK") >= 0 && status.Equals("ENTER_USER"))
            {
                Console.WriteLine("0003");
                commandList.Push(string.Format("{0}", ENTER_PWD));
                status = "ENTER_PWD";
            }
            else if (msg.IndexOf("-ERR") >= 0 && status.Equals("ENTER_PWD"))
            {
                Console.WriteLine("0004");
                commandList.Push(string.Format("{0}", ENTER_PWD));
                status = "ENTER_PWD";
            }
            else if (msg.IndexOf("+OK") >= 0 && status.Equals("ENTER_PWD"))
            {
                Console.WriteLine("0005");
                commandList.Push(string.Format("{0}", "LIST"));
                status = "LIST";
            }
            else if (msg.IndexOf("+OK") >= 0 && status.Equals("LIST"))
            {
                Console.WriteLine("0006");
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
                    sleepCtr = 0;
                    commandList.Push("RETR 1");
                    status = "RETR";
                }
                else
                {
                    if (sleepCtr > 10)
                    {
                        sleepCtr = 0;
                    }
                    sleepCtr++;
                    int nSleep = sleepCtr * 30;
                    string evmsg = string.Format("Sleeping for {0} sec.", nSleep);
                    Console.WriteLine(evmsg);
                    Thread.Sleep(nSleep * 1000);
                    //commandList.Push(string.Format("{0}", "LIST"));
                    //status = "LIST";
                    commandList.Push(string.Format("{0}", "QUIT"));
                    status = "QUIT";
                    //KaseyaEpicorLib.EventLog.Write(evmsg);
                }
            }
            else if (status.Equals("RETR"))
            {
                bool isDone = false;
                Console.WriteLine("RETR==> {0}", msg);
                Console.WriteLine("0007 x={0}, y={1}", msg.IndexOf("text/plain"), msg.IndexOf("k2e@sureid.com"));
                if (msg.IndexOf("text/plain") >= 0 || msg.IndexOf("k2e@sureid.com") >= 0)
                {
                    Console.WriteLine("0007-A1");
                    List<Attr> attrs = ParseIt2(ASCIIEncoding.ASCII.GetBytes(msg));
                    Console.WriteLine("0007-A2");

                    foreach (Attr atr in attrs)
                    {
                        string evmsg = string.Format("Attribute name={0}, value={1}", atr.name, atr.value);
                        Console.WriteLine(evmsg);
                        if (atr.name.ToUpper().Trim().Equals("STATUS"))
                        {
                            if (atr.value.ToUpper().Trim().Equals("COMPLETED") || atr.value.ToUpper().Trim().Equals("CLOSED"))
                            {
                                isClosed = true;
                            }
                        }
                        if (atr.name.ToUpper().Trim().Equals("EPICORCASENUM"))
                        {
                            sEpicorCaseNum = atr.value;
                        }
                    }
                    Console.WriteLine("0007-A3");

                    if (isClosed)
                    {
                        if (sEpicorCaseNum.Length > 0)
                        {
                            if (int.TryParse(sEpicorCaseNum, out nEpicorCaseNum))
                            {
                                jacobsenroad.EventLog.Write(string.Format("RETR: CLOSING THE TICKET:\n{0}", msg));
                                Console.WriteLine("Closing the ticket...");
                                //
                                // Call Epicor UpdateCase() and close the case;
                                //
                                isDone = true;
                            }
                        }
                        else
                        {
                            isDone = true;
                        }
                    }
                    else
                    {
                        jacobsenroad.EventLog.Write(string.Format("NO CLOSING NAME/VALUE PAIR: INVALID EMAIL:\r\n{0}", msg));
                        isDone = true;
                    }

                    if (isDone)
                    {
                        commandList.Push("DELE 1");
                        status = "DELETE";
                    }
                }
                else
                {
                    Console.WriteLine("0007-B");
                    //KaseyaEpicorLib.EventLog.Write(string.Format("INVALID FORMAT:\r\n{0}",msg));
                    //commandList.Push("DELE 1");
                    //status = "DELETE";
                }
            }
            else if (msg.IndexOf("+OK") >= 0 && status.Equals("DELETE"))
            {
                commandList.Push(string.Format("{0}", "QUIT"));
                status = "QUIT";
            }

            IsBusy = false;

            return null;
        }

        public List<Attr> ParseIt2(byte[] inputStr)
        {
            Scanner scanner = new Scanner();

            scanner.Input(inputStr);

            List<Attr> props = new List<Attr>();
            List<byte> token;
            string tok1;
            Attr atr;

            token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(".0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
            tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
            Console.WriteLine("1. token={0}", tok1);
            if (tok1.Equals("Received"))
            {
                token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                Console.WriteLine("2. token={0}", tok1);

                bool found = false;
                while (!found)
                {
                    scanner.SkipEmpty();

                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    Console.WriteLine("3. token={0}", tok1);
                    if (tok1.Equals("From"))
                    {
                        found = true;
                    }
                    else
                    {
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        string tok2 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    }
                }

                token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                Console.WriteLine("4. token={0}", tok1);
                if (tok1.Equals(":"))
                {
                    scanner.SkipEmpty();
                    token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("<"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("<"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("@.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    Console.WriteLine("token={0}", tok1);
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(">"));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    scanner.SkipEmpty();
                }
                bool inLoop = true;
                while (inLoop)
                {
                    scanner.SkipEmpty();
                    token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz "));
                    tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                    if (tok1.Equals("To"))
                    {
                        atr = new Attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("<"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("<"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes("@.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        Console.WriteLine("token={0}", tok1);
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(">"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                    }
                    else if (tok1.Equals("Subject"))
                    {
                        atr = new Attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
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
                    else if (tok1.Equals("MIME-Version"))
                    {
                        atr = new Attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                        //inLoop = false;
                    }
                    else if (tok1.Equals("Content-Type"))
                    {
                        atr = new Attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        Console.WriteLine("token={0}", tok1);
                        atr.value = tok1;
                        props.Add(atr);
                        scanner.SkipEmpty();
                    }
                    else if (tok1.Length == 0)
                    {
                        inLoop = false;
                    }
                    else
                    {
                        atr = new Attr();
                        string tokenName = tok1;
                        atr.name = tokenName;
                        scanner.SkipEmpty();
                        token = scanner.AcceptRun(ASCIIEncoding.ASCII.GetBytes(":"));
                        tok1 = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        scanner.SkipEmpty();
                        token = scanner.AcceptUntil(ASCIIEncoding.ASCII.GetBytes("\r\n"));
                        string tokenValue = ASCIIEncoding.ASCII.GetString(token.ToArray());
                        atr.value = tokenValue;
                        scanner.SkipEmpty();
                        props.Add(atr);
                        Console.WriteLine("Attribute: {0}={1}", atr.name, atr.value);
                    }
                }
            }
            return props;
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
