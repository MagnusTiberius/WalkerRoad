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
        public static string data = null;
        public WorldDimension WorldDimension { get; set; }
        public ChannelListing ChannelListing { get; set; }
        
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
            if (_socket.Connected)
            {
                _socket.Send(Encoding.ASCII.GetBytes("Notice: Hey this is the server ping.\n"));
            }
        }

        public void Update(string msg)
        {
            if (_socket.Connected)
            {
                _socket.Send(Encoding.ASCII.GetBytes(msg));
            }
        }

        public void Start()
        {
            _timer = new System.Timers.Timer(1000);
            _timer.Elapsed += new ElapsedEventHandler(_timer_Elapsed);
            _timer.Enabled = true; // Enable it

            //TargetLocation target = TargetLocation.Instance;
            WorldDimension.Register(this);

            // An incoming connection needs to be processed.
            IsAvailable = false;
            int n = 0;
            bool isLooping = true;
            _socket.ReceiveTimeout = 15000;
            _socket.NoDelay = true;
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
                            //data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                            string d = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                            WorldDimension.CurrentCommunicationChannel.Put(d);
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
