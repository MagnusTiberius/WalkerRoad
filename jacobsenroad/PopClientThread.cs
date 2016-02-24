using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Timers;
using System.Configuration;

namespace jacobsenroad
{
    public class PopClientThread
    {
        PopClientController controller = null;
        string[] pargs;
        private string m_environmentTams;
        private string m_environmentEpicor;

        public PopClientThread(string[] args)
        {
            pargs = args;
            controller = new PopClientController();
            controller.Init(pargs);
        }

        public PopClientThread(string environmentTams, string environmentEpicor)
        {
            m_environmentTams = environmentTams;
            m_environmentEpicor = environmentEpicor;
            controller = new PopClientController(environmentTams, environmentEpicor);
            controller.Init(pargs);
        }

        ~PopClientThread()
        {
            Stop();
        }

        public void Run()
        {
            controller.Run();
        }

        public void Stop()
        {
            Console.WriteLine("Stopping...\n");
            controller.DeInit();
        }
    }
}
