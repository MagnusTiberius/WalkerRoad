using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics.Tracing;
using System.Timers;

namespace jacobsenroad
{


    public class SynchronousSocketClient
    {
        public event OnReceiveData OnReceiveData;
        public event OnSendData OnSendData;

        private System.Timers.Timer _timer;
        private Socket _socket;
        private Guid guid;
        private Random rnd;
        public string ClientName { get; set; }

        public bool EnableDataGenerator
        {
            get
            {
                return _timer.Enabled;
            }

            set
            {
                _timer.Enabled = value;
            }
        }

        public void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            string msg = string.Format("{0}--{1}", ClientName, Datagen.GetComment());
            string sendMsg = msg;
            if (OnSendData != null)
            {
                sendMsg = OnSendData(msg);
            }
            _socket.Send(Encoding.ASCII.GetBytes(sendMsg));
        }

        public void Send(string msg)
        {
            string sendMsg = msg;
            if (OnSendData != null)
            {
                sendMsg = OnSendData(msg);
            }
            _socket.Send(Encoding.ASCII.GetBytes(sendMsg));
        }

        public void StartClient() 
        {
            ClientName = Datagen.GetFirstName();

            double interval; 
            interval = 2000;
            _timer = new System.Timers.Timer(interval);
            _timer.Elapsed += new ElapsedEventHandler(_timer_Elapsed);
            _timer.Enabled = true; // Enable it

            rnd = new Random();
            guid = Guid.NewGuid();

            byte[] bytes = new byte[1024*4];

            StringBuilder sb = new StringBuilder();
            List<string> msgList = new List<string>();
            int bytesRec;

            try {

                IPHostEntry ipHostInfo = Dns.Resolve(Dns.GetHostName());
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, Settings.PORTNUM);
                _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp );
                _socket.ReceiveTimeout = 15000;
                _socket.NoDelay = true;
                int id = Thread.CurrentThread.ManagedThreadId;

                System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0} Send Start", id));

                try {
                    _socket.Connect(remoteEP);
                    Console.WriteLine("Socket connected to {0}",  _socket.RemoteEndPoint.ToString());
                    byte[] msg = Encoding.ASCII.GetBytes("This is a test<EOF>\n");
                    System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}", id));
                    try
                    {
                        lock (this)
                        {
                            bytesRec = _socket.Receive(bytes);
                            while (bytesRec > 0)
                            {
                                string recvMsg = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                                if (OnReceiveData != null)
                                {
                                    OnReceiveData(recvMsg);
                                }
                                Console.WriteLine(string.Format("{0}\n",recvMsg));
                                bytesRec = _socket.Receive(bytes);
                            }
                        }
                    }
                    catch (SocketException se)
                    {
                        System.Diagnostics.Debug.WriteLine(string.Format("Error 11 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, se.ToString()));
                    }
                    System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}", Thread.CurrentThread.ManagedThreadId));
                    _socket.Shutdown(SocketShutdown.Both);
                    _socket.Close();
                } 
                catch (ArgumentNullException ane) 
                {
                    Console.WriteLine("ArgumentNullException : {0}",ane.ToString());
                    System.Diagnostics.Debug.WriteLine(string.Format("Error 12 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, ane.ToString()));
                } 
                catch (SocketException se) 
                {
                    Console.WriteLine("SocketException : {0}",se.ToString());
                    System.Diagnostics.Debug.WriteLine(string.Format("Error 13 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, se.ToString()));
                } 
                catch (Exception e) 
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                    System.Diagnostics.Debug.WriteLine(string.Format("Error 14 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
                }

            } catch (Exception e) {
                Console.WriteLine( e.ToString());
                System.Diagnostics.Debug.WriteLine(string.Format("Error 14 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
            }
        }

    }
}
