using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using jacobsenroad;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text.RegularExpressions;

namespace smtp_c
{
    public class SmtpClient :  SynchronousSocketClient
    {

        public SmtpClient()
        {
            OnReceiveData += OnReceiveDataHandler;
            OnSendData += OnSendDataHandler;
        }

        private string OnReceiveDataHandler(string msg)
        {
            Console.WriteLine(string.Format("{0}", msg));
            Thread.Sleep(500);
            Regex regex;

            regex = new Regex(@"^[0-9][0-9][0-9]");
            Match match = regex.Match(msg);

            if (match.Success)
            {
                if (match.Value.Equals("220"))
                {
                    Send("HELO localmachine");
                }
            }

            return msg;
        }

        private string OnSendDataHandler(string msg)
        {
            return msg;
        }
    }
}
