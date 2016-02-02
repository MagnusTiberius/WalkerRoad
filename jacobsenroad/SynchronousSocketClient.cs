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

namespace jacobsenroad
{
    public class SynchronousSocketClient
    {

        public void StartClient() 
        {
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
                IPEndPoint remoteEP = new IPEndPoint(ipAddress,11000);

                // Create a TCP/IP  socket.
                Socket sender = new Socket(AddressFamily.InterNetwork, 
                    SocketType.Stream, ProtocolType.Tcp );

                sender.ReceiveTimeout = 15000;
                sender.NoDelay = true;
                int id = Thread.CurrentThread.ManagedThreadId;

                System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0} Send Start", id));

                // Connect the socket to the remote endpoint. Catch any errors.
                try {
                    sender.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}",
                        sender.RemoteEndPoint.ToString());

                    // Encode the data string into a byte array.
                    byte[] msg = Encoding.ASCII.GetBytes("This is a test<EOF>\n");

                    for (int j = 0; j < 5; j++)
                    {
                        lock (this)
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                // Send the data through the socket.
                                int bytesSent = sender.Send(Encoding.ASCII.GetBytes(string.Format("{0}{1}:{2};", j, i, id)));
                            }
                            // Receive the response from the remote device.
                            bytesRec = sender.Receive(bytes);
                            string msg1 = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                            sb.Append(msg1);
                            msgList.Add(msg1);
                            System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}:{1}", id, msg1));
                            Thread.Sleep(20);
                            System.Diagnostics.Debug.WriteLine("{0} Echoed test = {1}", id, Encoding.ASCII.GetString(bytes, 0, bytesRec));
                        }
                    }
                    System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}", id));
                    try
                    {
                        lock (this)
                        {
                            bytesRec = sender.Receive(bytes);
                            while (bytesRec > 0)
                            {
                                string msg1 = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                                sb.Append(msg1);
                                msgList.Add(msg1);
                                System.Diagnostics.Debug.WriteLine("{0} Echoed test = {1}", id, Encoding.ASCII.GetString(bytes, 0, bytesRec));
                                int bytesSent = sender.Send(Encoding.ASCII.GetBytes(string.Format("{0}{1}:{2};", id, id, id)));
                                bytesRec = sender.Receive(bytes);
                            }
                        }
                    }
                    catch (SocketException se)
                    {
                        System.Diagnostics.Debug.WriteLine(string.Format("Error 11 Thread:{0} {1}", Thread.CurrentThread.ManagedThreadId, se.ToString()));
                    }
                    // Release the socket.
                    System.Diagnostics.Debug.WriteLine(string.Format("Thread:{0}", Thread.CurrentThread.ManagedThreadId));
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();

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
