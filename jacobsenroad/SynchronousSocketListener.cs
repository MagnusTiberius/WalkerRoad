using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace jacobsenroad
{

    public class ListenerHandler
    {
        private Socket _socket;
        private byte[] bytes = new Byte[1024];
        public static string data = null;

        public ListenerHandler(Socket socket)
        {
            _socket = socket;
        }

        public void Start()
        {
            // An incoming connection needs to be processed.
            int n = 0;
            bool isLooping = true;
            while (isLooping)
            {
                bytes = new byte[1024];
                try
                {
                    int bytesRec = _socket.Receive(bytes);
                    if (bytesRec > 0)
                    {
                        data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                        string d = Encoding.ASCII.GetString(bytes, 0, bytesRec);

                        _socket.Send(Encoding.ASCII.GetBytes(string.Format("This is a reply {0}\n {1}", n++, d)));
                    }
                    else
                    {
                        isLooping = false;
                    }
                }
                catch(Exception)
                {
                    isLooping = false;
                }
            }
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
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);

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
                    var t = new ListenerHandler(handler);
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
