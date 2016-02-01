using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace walkerroadlib
{
    public class ClientBase : AsynchronousClient
    {
        public ClientBase()
        {
            OnReceiveCompleted += OnReceiveCompletedHandler;
        }

        private void OnReceiveCompletedHandler(string response)
        {
            Console.WriteLine(response);
            model.Header header = JsonConvert.DeserializeObject<model.Header>(response);
            if (header.Method.Equals("PRESENCE") && header.Status.Equals(model.Status.Presence.RECOGNIZED.ToString()))
            {
                Console.WriteLine("PRESENCE RECOGNIZED");
            }
            if (header.Method.Equals("LOGIN") && header.Status.Equals(model.Status.Access.GRANTED.ToString()))
            {
                Console.WriteLine("Approved");
            }
        }

        private string GetLogin()
        {
            model.Header h = new model.Header();
            h.Method = "LOGIN";
            h.Source = "CLIENT";
            h.Status = model.Status.Access.REQUEST.ToString();
            model.Login l = new model.Login();
            l.UserName = "Test";
            l.Password = "Test";
            h.Body = l;
            var json = JsonConvert.SerializeObject(h);
            return json;
        }

        private string GetPresence()
        {
            model.Header h = new model.Header();
            h.Method = "PRESENCE";
            h.Source = "CLIENT";
            h.Status = model.Status.Presence.UNKNOWN.ToString();
            model.Presence l = new model.Presence();
            Guid guid = Guid.NewGuid();
            l.GUID = guid.ToString();
            h.Body = l;
            var json = JsonConvert.SerializeObject(h);
            return json;
        }

        public void StartClient()
        {
            Start();
            Send(string.Format("{0}", GetPresence()));
            Send(string.Format("{0}", GetLogin()));
            CloseClient();
        }
    }
}
