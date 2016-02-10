using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace jacobsenroad
{
    static public class DataGameGen
    {
        static public Dictionary<string, ProtocolCommand> CommandSet = new Dictionary<string, ProtocolCommand>
        {
            {"LOGIN", new ProtocolCommand {Name="LOGIN",Template="LOGIN {0}"} },
            {"SAY", new ProtocolCommand {Name="SAY",Template="SAY {0}"} },
            {"JOIN", new ProtocolCommand {Name="JOIN",Template="JOIN {0}"} },
        };

        
        static public string[] procedureData = {
                                                   string.Format("SPAWN . GAME/1.0\nname:{0}\n\n{0}\n\n", "John"),
                                                   string.Format("MOVE . GAME/1.0\nname:{0}\nx-coord:1\n\n{1)\n\n", "John", "x-coord:1;"),
                                            };
    }
}
