using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Timers;

namespace jacobsenroad
{

    public class ListenerHandler
    {
        private System.Timers.Timer _timer;
        private Socket _socket;
        private byte[] bytes = new Byte[1024];
        public static string data = null;
        public TargetLocation Target { get; set; }
        public bool IsAvailable { get; set; }

        public ListenerHandler(Socket socket)
        {
            _socket = socket;
            IsAvailable = true;
        }

        public void SetSocket(Socket socket)
        {
            _socket = socket;
        }

        public void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            _socket.Send(Encoding.ASCII.GetBytes("Notice: Hey this is the server ping.\n"));
        }

        public void Update(string msg)
        {
            _socket.Send(Encoding.ASCII.GetBytes(msg));
        }

        public void Start()
        {
            _timer = new System.Timers.Timer(1000);
            _timer.Elapsed += new ElapsedEventHandler(_timer_Elapsed);
            _timer.Enabled = true; // Enable it

            //TargetLocation target = TargetLocation.Instance;
            Target.Register(this);

            // An incoming connection needs to be processed.
            IsAvailable = false;
            int n = 0;
            bool isLooping = true;
            _socket.ReceiveTimeout = 15000;
            _socket.NoDelay = true;
            while (isLooping)
            {
                bytes = new byte[1024*4];
                try
                {
                    lock (this)
                    {
                        int bytesRec = _socket.Receive(bytes);
                        if (bytesRec > 0)
                        {
                            data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                            string d = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                            Target.Put(d);
                            //_socket.Send(Encoding.ASCII.GetBytes(string.Format("Server:{0}:{1}:{2}", n++, Thread.CurrentThread.ManagedThreadId, d)));
                            System.Diagnostics.Debug.WriteLine(string.Format("ListenerHandler 22 Thread:{0} Send", Thread.CurrentThread.ManagedThreadId));
                            Console.WriteLine(string.Format("ListenerHandler 22 Thread:{0} Send", Thread.CurrentThread.ManagedThreadId));
                            //Thread.Sleep(500);
                        }
                        else
                        {
                            isLooping = false;
                        }
                    }
                }
                catch(Exception e)
                {
                    isLooping = false;
                    System.Diagnostics.Debug.WriteLine(string.Format("Error ListenerHandler 22 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
                    _timer.Enabled = false;
                }
            }
            IsAvailable = true;
        }
    }

    public class TargetLocation
    {
        static readonly TargetLocation _instance = new TargetLocation();
        private int initv = 0;
        private int tid;
        private int ctr1 = 0;
        public static TargetLocation Instance
        {
            get
            {
                return _instance;
            }
        }

        Stack<string> listMessage = new Stack<string>();
        List<ListenerHandler> listConnection = new List<ListenerHandler>();

        private void TargetLocationInit()
        {
            tid = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("TargetLocation ctor()");
            Thread threadServer = new Thread(new ThreadStart(Looper));
            threadServer.Start();
        }

        private void Looper()
        {
            while (true)
            {
                lock (this)
                {
                    if (listMessage.Count == 0)
                    {
                        continue;
                    }
                    string msg = listMessage.Pop();
                    Console.WriteLine(string.Format("{0}:Update:{1}", tid, msg));
                    foreach (ListenerHandler h in listConnection)
                    {
                        if (h != null)
                        {
                            try
                            {
                                h.Update(string.Format("{0}::{1}", msg, ctr1));
                            }
                            catch( Exception ex )
                            {
                                Console.WriteLine(ex.Message);
                            }
                        }
                    }
                    Console.WriteLine("==========================================");
                }
                //foreach (string msg in listMessage)
                //{
                //    foreach (ListenerHandler h in listConnection)
                //    {
                //        //h.Update(msg);
                //        Console.WriteLine(string.Format("Update:{0}", msg));
                //    }
                //}
            }
        }

        public void Register(ListenerHandler h)
        {
            lock (this)
            {
                if (initv == 0)
                {
                    TargetLocationInit();
                }

                listConnection.Add(h);
            }
        }

        public void Put(string msg)
        {
            lock (this)
            {
                int c = ctr1++;
                Console.WriteLine(string.Format("{0}:Put:{1}:{2}", tid, msg, c));
                listMessage.Push(string.Format("{0}{1}",msg,c));
            }
        }
    }

    public class ListenerHandlerPool
    {
        static readonly ListenerHandlerPool _instance = new ListenerHandlerPool();
        public static ListenerHandlerPool Instance
        {
            get
            {
                return _instance;
            }
        }

        private ListenerHandlerPool()
        {
            list = new List<ListenerHandler>();
        }

        private List<ListenerHandler> list;

        public void Add(Socket socket)
        {
            try
            {
                foreach (ListenerHandler itm in list)
                {
                    if (itm.IsAvailable)
                    {
                        itm.SetSocket(socket);
                        Thread thread1 = new Thread(new ThreadStart(itm.Start));
                        thread1.Start();
                        return;
                    }
                }
                ListenerHandler h = new ListenerHandler(socket);
                list.Add(h);
                Thread thread = new Thread(new ThreadStart(h.Start));
                thread.Start();
            }
            catch(Exception e)
            {
                System.Diagnostics.Debug.WriteLine(string.Format("Error ListenerHandler 23 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
            }
            return;
        }
    }

    public class SynchronousSocketListener
    {

        // Incoming data from the client.
        public string data = null;

        private List<Thread> threadList;

        public void StartListening()
        {
            // Data buffer for incoming data.
            byte[] bytes = new Byte[1024];
            threadList = new List<Thread>();
            // Establish the local endpoint for the socket.
            // Dns.GetHostName returns the name of the 
            // host running the application.
            IPHostEntry ipHostInfo = Dns.Resolve(Dns.GetHostName());
            IPAddress ipAddress = ipHostInfo.AddressList[0];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, Settings.PORTNUM);

            // Create a TCP/IP socket.
            Socket listener = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and 
            // listen for incoming connections.
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(10);

                // Start listening for connections.
                while (true)
                {
                    Console.WriteLine("Waiting for a connection...");
                    // Program is suspended while waiting for an incoming connection.
                    Socket handler = listener.Accept();
                    data = null;

                    //var pool = ListenerHandlerPool.Instance;
                    //pool.Add(handler);

                    
                    ListenerHandler t = new ListenerHandler(handler);
                    t.Target = TargetLocation.Instance;
                    Thread thread = new Thread(new ThreadStart(t.Start));
                    thread.Start();
                    threadList.Add(thread);
                    

                    //// An incoming connection needs to be processed.
                    //while (true)
                    //{
                    //    bytes = new byte[1024];
                    //    int bytesRec = handler.Receive(bytes);
                    //    data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                    //    if (data.IndexOf("<EOF>") > -1)
                    //    {
                    //        break;
                    //    }
                    //}

                    //// Show the data on the console.
                    //Console.WriteLine("Text received : {0}", data);

                    //// Echo the data back to the client.
                    //byte[] msg = Encoding.ASCII.GetBytes(data);

                    //handler.Send(msg);
                    //handler.Shutdown(SocketShutdown.Both);
                    //handler.Close();
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            Console.WriteLine("\nPress ENTER to continue...");
            Console.Read();

        }

    }
}
