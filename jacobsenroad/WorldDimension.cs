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

    public class CommunicationChannel
    {
        public string Name { get; set; }
        public string Description { get; set; }
        private Stack<string> listMessage = new Stack<string>();
        private int ctr1 = 0;
        private int tid;

        public CommunicationChannel()
        {
            tid = Thread.CurrentThread.ManagedThreadId;
        }

        public void Put(string msg)
        {
            lock (this)
            {
                int c = ctr1++;
                Console.WriteLine(string.Format("{0}:Put:{1}:{2}", tid, msg, c));
                listMessage.Push(string.Format("{0}", msg));
            }
        }

        public string Pickup()
        {
            lock (this)
            {
                if (listMessage.Count == 0)
                {
                    return null;
                }
                string msg = listMessage.Pop();
                return string.Format("[{0}] {1}", Name, msg);
            }
        }
    }

    public class WorldDimension
    {
        static readonly WorldDimension _instance = new WorldDimension();
        private int initv = 0;
        private int tid;
        private int ctr1 = 0;
        public static WorldDimension Instance
        {
            get
            {
                return _instance;
            }
        }
        public ChannelListing ChannelListing { get; set; }

        private CommunicationChannel _communicationChannel = null;

        private Stack<string> listMessage = new Stack<string>();
        private List<ListenerHandler> listConnection = new List<ListenerHandler>();

        private void WorldDimensionInit()
        {
            tid = Thread.CurrentThread.ManagedThreadId;
            Console.WriteLine("WorldDimension ctor()");
            Thread threadServer = new Thread(new ThreadStart(Looper));
            threadServer.Start();
        }

        public CommunicationChannel CurrentCommunicationChannel 
        {
            get
            {
                if (_communicationChannel == null)
                {
                    ChannelListing.Instance.SwitchTo(null);
                    _communicationChannel = ChannelListing.Instance.SwitchTo("lobby");
                }
                return _communicationChannel;
            }

            set
            {
                _communicationChannel = value;
            } 
        }


        private void Looper()
        {
            while (true)
            {
                lock (this)
                {
                    string msg = CurrentCommunicationChannel.Pickup();  
                    if (msg == null)
                    {
                        continue;
                    }

                    Console.WriteLine(string.Format("{0}:Update:{1}", tid, msg));
                    foreach (ListenerHandler h in listConnection)
                    {
                        if (h != null)
                        {
                            try
                            {
                                h.Update(string.Format("{0}", msg));
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine(ex.Message);
                            }
                        }
                    }
                    Console.WriteLine("==========================================");
                }

            }
        }

        public void Register(ListenerHandler h)
        {
            lock (this)
            {
                if (initv == 0)
                {
                    WorldDimensionInit();
                }

                listConnection.Add(h);
            }
        }

        public void Put(string msg)
        {
            lock (this)
            {
                int c = ctr1++;
                Console.WriteLine(string.Format("{0}:Put:{1}:{2}", tid, msg, c));
                listMessage.Push(string.Format("{0}", msg));
            }
        }
    }
}
