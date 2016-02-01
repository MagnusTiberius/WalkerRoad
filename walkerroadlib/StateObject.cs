using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using walkerroadlib.Controller;

namespace walkerroadlib
{
    public class Const
    {
        public const int PORT_NUM = 8092;
    }

    // State object for receiving data from remote device.
    public class StateObject
    {
        // Client socket.
        public Socket workSocket = null;
        // Size of receive buffer.
        public const int BufferSize = 1024;
        // Receive buffer.
        public byte[] buffer = new byte[BufferSize];
        // Received data string.
        public StringBuilder sb = new StringBuilder();

        private SocketManager socketManager;

        public StateObject()
        {
            socketManager = new SocketManager(this);
        }

        public void ReadCallback(IAsyncResult ar)
        {
            socketManager.ReadCallback(ar);
        }

    }
}
