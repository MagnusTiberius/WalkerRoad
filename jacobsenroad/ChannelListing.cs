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
    public class ChannelListing
    {
        static readonly ChannelListing _instance = new ChannelListing();
        public string Name { get; set; }
        public string Description { get; set; }
        private Dictionary<string, CommunicationChannel> list = new Dictionary<string, CommunicationChannel>();
        private int ctr1 = 0;
        private int tid = Thread.CurrentThread.ManagedThreadId;

        public static ChannelListing Instance
        {
            get
            {
                return _instance;
            }
        }

        public void CreateChannel(string key)
        {
            lock (this)
            {
                CommunicationChannel ch = new CommunicationChannel();
                list.Add(key, ch);
            }
        }

        public void Add(string key, CommunicationChannel ch)
        {
            lock (this)
            {
                list.Add(key, ch);
            }
        }

        public CommunicationChannel SwitchTo(string key)
        {
            lock (this)
            {
                if (key == null)
                {
                    if (list.ContainsKey("lobby"))
                    {
                        return list["lobby"];
                    }
                    else
                    {
                        CommunicationChannel ch = new CommunicationChannel();
                        ch.Name = "lobby";
                        list.Add("lobby", ch);
                        return list["lobby"];
                    }
                }

                if (!list.ContainsKey(key))
                {
                    return null;
                }
                return list[key];
            }
        }
    }
}
