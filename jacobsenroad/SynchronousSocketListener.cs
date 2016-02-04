using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Timers;

namespace jacobsenroad
{
    public class SynchronousSocketListener
    {
        public event OnReceiveData OnReceiveData;
        public event OnSendData OnSendData;

        private Socket handler;
        private List<Thread> threadList;
        public string HostName { get; set; }
        private IPHostEntry ipHostInfo;

        public void Close()
        {
            try
            {
                if (handler == null)
                {
                    return;
                }

                handler.Shutdown(SocketShutdown.Both);
                handler.Close();
            }
            catch(Exception)
            {

            }
        }

        public void StartListening()
        {
            byte[] bytes = new Byte[1024];
            threadList = new List<Thread>();

            if (HostName == null)
            {
                ipHostInfo = Dns.Resolve(Dns.GetHostName());
                //ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            }
            else
            {
                ipHostInfo = Dns.Resolve(HostName);
            }

            Console.WriteLine(string.Format("Host={0}", HostName));
            ipHostInfo = Dns.Resolve(HostName);
            IPAddress ipAddress = ipHostInfo.AddressList[0];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, Settings.PORTNUM);

            Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(10);
                while (true)
                {
                    Console.WriteLine("Waiting for a connection...");
                    
                    handler = listener.Accept();
                    //var pool = ListenerHandlerPool.Instance;
                    //pool.Add(handler);

                    ListenerHandler t = ListenerHandlerController.Next(handler);
                    t.OnReceiveData += OnReceiveDataHandler;
                    t.OnSendData += OnSendDataHandler;
                    Thread thread = new Thread(new ThreadStart(t.Start));
                    thread.Start();
                    threadList.Add(thread);
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            Console.WriteLine("\nPress ENTER to continue...");
            Console.Read();

        }

        private string OnReceiveDataHandler(string msg)
        {
            string msg2 = msg;
            if (OnReceiveData != null)
            {
                msg2 = OnReceiveData(msg);
            }
            return msg2;
        }

        private string OnSendDataHandler(string msg)
        {
            string msg2 = msg;
            if (OnSendData != null)
            {
                msg2 = OnSendData(msg);
            }
            return msg2;
        }
    }
}
