#include "stdafx.h"
#include "CppUnitTest.h"

#include "CompletionPortListener.h"
#include "TestListener1.h"
#include "BaseParser.h"
#include "InputStage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pinefarm_test
{
	const char* gInput = "MOVE . GAME/1.0\nname = Casey\ncontent-length:24\ncoord-x : 1\n\nMOVING Casey X COORD : 1\n\n\
		SAY /lobby CHAT / 1.0\nname=Casey\ncontent-length : 24\n\nI seem to have found it.\n\n\
		MOVE . GAME/1.0\nname =Casey\ncontent-length:24\ncoord-x : 2\n\nMOVING Casey X COORD : 2\n\n\
		SAY /lobby CHAT /1.0\nname = Casey\ncontent-length: 45\n\nIt\'ll be just like Beggar\'s Canyon back home.\n\n\
		MOVE . GAME /1.0\nname= Casey\ncontent-length:24\ncoord-x : 3\n\nMOVING Casey X COORD : 3\n\n\
		SAY /lobby CHAT/ 1.0\nname = Casey\ncontent-length :56\n\nRemember, a Jedi can feel the Force flowing through him.\n\n\
		MOVE . GAME /1.0\nname = Casey\ncontent-length:24\ncoord-x : 4\n\nMOVING Casey X COORD : 4\n\n\
		SAY /lobby CHAT/ 1.0\nname=Casey\ncontent-length: 164\n\nI felt a great disturbance in the Force... as if millions of voices suddenly cried out in terror and were suddenly silenced.I fear something terrible has happened.\n\n\
		MOVE . GAME/1.0\nname = Casey\ncontent-length:24\ncoord-x : 5\n\n\MOVING Casey X COORD : 5\n\n\
		";


	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			BaseParser* p = new BaseParser();
			p->Parse(gInput);
			Structs::LP_JOBREQUEST item = (Structs::LP_JOBREQUEST)p->Next();
			while (item != NULL)
			{
				item = (Structs::LP_JOBREQUEST)p->Next();
				free(item);
			}
			delete(p);
		}

		TEST_METHOD(TestMethod2)
		{
			InputStage inputStage;
			inputStage.Start();

			BaseParser* p = new BaseParser();
			p->Parse(gInput);
			Structs::LP_JOBREQUEST item = (Structs::LP_JOBREQUEST)p->Next();
			while (item != NULL)
			{
				item = (Structs::LP_JOBREQUEST)p->Next();
				inputStage.AddMessage(item);
				//free(item);
			}
			delete(p);
			::Sleep(10000);
			//inputStage.Join();
			inputStage.Stop();
		}


	};
}