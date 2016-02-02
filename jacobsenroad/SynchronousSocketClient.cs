using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Diagnostics.Tracing;
using System.Timers;

namespace jacobsenroad
{
    public class Settings
    {
        public const int PORTNUM = 11000;
    }

    public class SynchronousSocketClient
    {
        private System.Timers.Timer _timer;
        private Socket _socket;
        private Guid guid;
        private Random rnd;

        public void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            Thread.Sleep(rnd.Next(6000, 10000));

            _socket.Send(Encoding.ASCII.GetBytes(guid.ToString()));
        }

        public void StartClient() 
        {
            _timer = new System.Timers.Timer(1000);
            _timer.Elapsed += new ElapsedEventHandler(_timer_Elapsed);
            _timer.Enabled = true; // Enable it

            rnd = new Random();
            guid = Guid.NewGuid();

            // Data buffer for incoming data.
            byte[] bytes = new byte[1024*4];

            StringBuilder sb = new StringBuilder();
            List<string> msgList = new List<string>();
            int bytesRec;
            // Connect to a remote device.
            try {
                // Establish the remote endpoint for the socket.
                // This example uses port 11000 on the local computer.
                IPHostEntry ipHostInfo = Dns.Resolve(Dns.GetHostName());
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, Settings.PORTNUM);

                // Create a TCP/IP  socket.
                _socket = new Socket(AddressFamily.InterNetwork, 
                    SocketType.Stream, ProtocolType.Tcp );

                _socket.ReceiveTimeout = 15000;
                _socket.NoDelay = true;
                int id = Thread.CurrentThread.ManagedThreadId;

                System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0} Send Start", id));

                // Connect the socket to the remote endpoint. Catch any errors.
                try {
                    _socket.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}",
                        _socket.RemoteEndPoint.ToString());

                    // Encode the data string into a byte array.
                    byte[] msg = Encoding.ASCII.GetBytes("This is a test<EOF>\n");

                    //for (int j = 0; j < 5; j++)
                    //{
                    //    lock (this)
                    //    {
                    //        for (int i = 0; i < 2; i++)
                    //        {
                    //            // Send the data through the socket.
                    //            int bytesSent = _socket.Send(Encoding.ASCII.GetBytes(string.Format("{0}{1}:{2};", j, i, id)));
                    //        }
                    //        // Receive the response from the remote device.
                    //        bytesRec = _socket.Receive(bytes);
                    //        string msg1 = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                    //        sb.Append(msg1);
                    //        msgList.Add(msg1);
                    //        System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}:{1}", id, msg1));
                    //        Thread.Sleep(20);
                    //        System.Diagnostics.Debug.WriteLine("{0} Echoed test = {1}", id, Encoding.ASCII.GetString(bytes, 0, bytesRec));
                    //    }
                    //}
                    System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}", id));
                    try
                    {
                        lock (this)
                        {
                            bytesRec = _socket.Receive(bytes);
                            while (bytesRec > 0)
                            {
                                string msg1 = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                                //sb.Append(msg1);
                                //msgList.Add(msg1);
                                //System.Diagnostics.Debug.WriteLine("{0} Echoed test = {1}", id, Encoding.ASCII.GetString(bytes, 0, bytesRec));
                                //int bytesSent = _socket.Send(Encoding.ASCII.GetBytes(string.Format("{0}{1}:{2};", id, id, id)));
                                //Console.WriteLine("{0}:{1}:{2}", id, Encoding.ASCII.GetString(bytes, 0, bytesRec), msg1);
                                Console.WriteLine(msg1);
                                bytesRec = _socket.Receive(bytes);
                            }
                        }
                    }
                    catch (SocketException se)
                    {
                        System.Diagnostics.Debug.WriteLine(string.Format("Error 11 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, se.ToString()));
                    }
                    // Release the socket.
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
