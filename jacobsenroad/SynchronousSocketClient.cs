﻿using System;
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
        public event OnClientLoop OnClientLoop;

        private System.Timers.Timer _timer;
        private Socket _socket;
        private Guid guid;
        private Random rnd;
        public string ClientName { get; set; }
        public IPHostEntry ipHostInfo { get; set; }
        public string HostName { get; set; }
        public int PortNumber { get; set; }
        public bool IsConnected { get; set; }

        public SynchronousSocketClient()
        {
            double interval;
            interval = 200;
            PortNumber = 0;
            _timer = new System.Timers.Timer(interval);
            _timer.Elapsed += new ElapsedEventHandler(_timer_Elapsed);
            _timer.Enabled = false; // Enable it
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

        public double TimerInterval
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

        private void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            string msg;
            if (OnClientLoop != null)
            {
                msg = string.Format("{0}--{1}", ClientName, OnClientLoop());
            }
            else
            {
                msg = string.Format("{0}--{1}", ClientName, Datagen.GetComment());
            }
            string sendMsg = msg;
            if (OnSendData != null)
            {
                sendMsg = OnSendData(msg);
            }
            if (_socket != null)
            {
                if (_socket.Connected)
                {
                    _socket.Send(Encoding.ASCII.GetBytes(sendMsg));
                }
            }
        }

        public int Send(string msg)
        {
            int res = 0;
            string sendMsg = msg;
            if (OnSendData != null)
            {
                sendMsg = OnSendData(msg);
            }
            if (_socket != null)
            {
                if (_socket.Connected)
                {
                    res = _socket.Send(Encoding.ASCII.GetBytes(sendMsg));
                }
            }
            return res;
        }

        private void ClientLoop()
        {

        }

        public void StartClient() 
        {
            ClientName = Datagen.GetFirstName();


            rnd = new Random();
            guid = Guid.NewGuid();

            byte[] bytes = new byte[1024*16];

            StringBuilder sb = new StringBuilder();
            List<string> msgList = new List<string>();
            int bytesRec;
            IPEndPoint remoteEP;

            while (true)
            {

                try
                {
                    if (HostName == null)
                    {
                        ipHostInfo = Dns.Resolve(Dns.GetHostName());
                        //ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
                    }
                    else
                    {
                        ipHostInfo = Dns.Resolve(HostName);
                    }
                    IPAddress ipAddress = ipHostInfo.AddressList[0];
                    if (PortNumber == 0)
                    {
                        remoteEP = new IPEndPoint(ipAddress, Settings.PORTNUM);
                    }
                    else
                    {
                        remoteEP = new IPEndPoint(ipAddress, PortNumber);
                    }
                    _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    //_socket.ReceiveTimeout = 15000;
                    //_socket.NoDelay = true;
                    int id = Thread.CurrentThread.ManagedThreadId;

                    System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0} Send Start", id));

                    try
                    {
                        _socket.Connect(remoteEP);
                        Console.WriteLine("Socket connected to {0}", _socket.RemoteEndPoint.ToString());
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
                                    //Console.WriteLine(string.Format("{0}\n",recvMsg));
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
                        //Thread.CurrentThread.Abort();
                        //return;
                    }
                    catch (ArgumentNullException ane)
                    {
                        System.Diagnostics.Debug.WriteLine("ArgumentNullException : {0}", ane.ToString());
                        System.Diagnostics.Debug.WriteLine(string.Format("Error 12 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, ane.ToString()));
                        //Thread.CurrentThread.Abort();
                        //return;
                    }
                    catch (SocketException se)
                    {
                        System.Diagnostics.Debug.WriteLine("SocketException : {0}", se.ToString());
                        System.Diagnostics.Debug.WriteLine(string.Format("Error 13 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, se.ToString()));
                        //Thread.CurrentThread.Abort();
                        //return;
                    }
                    catch (Exception e)
                    {
                        System.Diagnostics.Debug.WriteLine("Unexpected exception : {0}", e.ToString());
                        System.Diagnostics.Debug.WriteLine(string.Format("Error 14 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
                        //Thread.CurrentThread.Abort();
                        //return;
                    }

                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine(e.ToString());
                    System.Diagnostics.Debug.WriteLine(string.Format("Error 14 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, e.ToString()));
                    //Thread.CurrentThread.Abort();
                    //return;
                }
            }
        }

    }
}
