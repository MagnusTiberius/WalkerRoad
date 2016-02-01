using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace walkerroadlib.model
{
    public class Header
    {
        public string Method { get; set; }
        public string Source { get; set; }
        public object Body { get; set; }
        public string Status { get; set; }
        public string Presence { get; set; }
    }

    public delegate string OnReceiveData(string response);

    public class Status
    {
        public enum Access
        {
            REQUEST = 100001,
            GRANTED,
            DENIED
        }

        public enum Presence
        {
            UNKNOWN = 200001,
            RECOGNIZED,
            IGNORED
        }

    }
}
