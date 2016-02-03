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

    public class ListenerHandler
    {
        private System.Timers.Timer _timer;
        private Socket _socket;
        private byte[] bytes = new Byte[1024];
        public WorldDimension WorldDimension { get; set; }
        public ChannelListing ChannelListing { get; set; }

        public event OnReceiveData OnReceiveData;
        public event OnSendData OnSendData;

        public bool IsAvailable { get; set; }

        public ListenerHandler(Socket socket)
        {
            _socket = socket;
            IsAvailable = true;
            WorldDimension = WorldDimension.Instance;
        }

        public ListenerHandler(Socket socket, WorldDimension wd)
        {
            _socket = socket;
            IsAvailable = true;
            WorldDimension = wd;
        }

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

        public double DataGeneratorInterval
        {
            get
            {
                return _timer.Interval;
            }

            set
            {
                _timer.Interval = value;
            }
        }

        public void SetSocket(Socket socket)
        {
            _socket = socket;
        }

        public void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (_socket.Connected)
            {
                string msg = "Notice: Hey this is the server ping.\n";
                string msg2 = msg;
                if (OnSendData != null)
                {
                    msg2 = OnSendData(msg);
                }
                _socket.Send(Encoding.ASCII.GetBytes(msg2));
            }
        }

        public void Update(string msg)
        {
            if (_socket.Connected)
            {
                string msg2 = msg;
                if (OnSendData != null)
                {
                    msg2 = OnSendData(msg);
                }
                _socket.Send(Encoding.ASCII.GetBytes(msg2));
            }
        }

        public void Start()
        {
            _timer = new System.Timers.Timer(5000);
            _timer.Elapsed += new ElapsedEventHandler(_timer_Elapsed);
            _timer.Enabled = true; // Enable it

            WorldDimension.Register(this);

            IsAvailable = false;
            int n = 0;
            bool isLooping = true;
            //_socket.ReceiveTimeout = 15000;
            //_socket.NoDelay = true;
            while (isLooping)
            {
                bytes = new byte[1024 * 4];
                try
                {
                    lock (this)
                    {
                        int bytesRec = _socket.Receive(bytes);
                        if (bytesRec > 0)
                        {
                            string recvMsg = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                            WorldDimension.CurrentCommunicationChannel.Put(recvMsg);
                            System.Diagnostics.Debug.WriteLine(string.Format("ListenerHandler 22 Thread:{0} Send", Thread.CurrentThread.ManagedThreadId));
                            Console.WriteLine(string.Format("ListenerHandler 22 Thread:{0} Send", Thread.CurrentThread.ManagedThreadId));
                            OnReceiveData(recvMsg);
                        }
                        else
                        {
                            isLooping = false;
                        }
                    }
                }
                catch (Exception e)
                {
                    isLooping = false;
                    System.Diagnostics.Debug.WriteLine(string.Format("Error ListenerHandler 22 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
                    _timer.Enabled = false;
                }
            }
            IsAvailable = true;
        }
    }

}
