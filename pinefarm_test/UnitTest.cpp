#include "stdafx.h"
#include "CppUnitTest.h"

#include "CompletionPortListener.h"
#include "TestListener1.h"
#include "BaseParser.h"
#include "InputStage.h"
#include "StockEngine.h"
#include "StockRepository.h"
#include "SmtpAgent.h"
#include "SmtpParser.h"

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
		MOVE . GAME/1.0\nname = Casey\ncontent-length:24\ncoord-x : 1\n\nMOVING Casey X COORD : 1\n\n\
		SAY /kitchen CHAT / 1.0\nname=Casey\ncontent-length : 24\n\nI seem to have found it.\n\n\
		MOVE . GAME/1.0\nname =Casey\ncontent-length:24\ncoord-x : 2\n\nMOVING Casey X COORD : 2\n\n\
		SAY /garden CHAT /1.0\nname = Casey\ncontent-length: 45\n\nIt\'ll be just like Beggar\'s Canyon back home.\n\n\
		MOVE . GAME /1.0\nname= Casey\ncontent-length:24\ncoord-x : 3\n\nMOVING Casey X COORD : 3\n\n\
		SAY /kitchen CHAT/ 1.0\nname = Casey\ncontent-length :56\n\nRemember, a Jedi can feel the Force flowing through him.\n\n\
		MOVE . GAME /1.0\nname = Casey\ncontent-length:24\ncoord-x : 4\n\nMOVING Casey X COORD : 4\n\n\
		SAY /garden CHAT/ 1.0\nname=Casey\ncontent-length: 164\n\nI felt a great disturbance in the Force... as if millions of voices suddenly cried out in terror and were suddenly silenced.I fear something terrible has happened.\n\n\
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
			inputStage.EnableNextStage(false);
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

		TEST_METHOD(TestMethod3)
		{
			InputStage inputStage;
			inputStage.EnableNextStage(true);
			inputStage.Start();

			BaseParser* p = new BaseParser();
			p->Parse(gInput);
			Structs::LP_JOBREQUEST item = (Structs::LP_JOBREQUEST)p->Next();
			while (item != NULL)
			{
				item = (Structs::LP_JOBREQUEST)p->Next();
				item->socket = 1;
				inputStage.AddMessage(item);
				//free(item);
			}
			delete(p);
			::Sleep(10000);
			//inputStage.Join();
			inputStage.Stop();
		}


		TEST_METHOD(TestStockEngine)
		{
			StockEngine stockEngine;
			stockEngine.Start();
			//"MOVE . GAME/1.0\nname = Casey\ncontent-length:24\ncoord-x : 1\n\nMOVING Casey X COORD : 1\n\n\";
			Structs::LP_JOBREQUEST job = new Structs::JOBREQUEST();
			job->header.method = "SUBSCRIBE";
			job->header.protocol = "STOCK";
			job->header.url = "INTC";
			job->header.name = "SubscriberA";
			stockEngine.AddMessage(job);
			Structs::LP_JOBREQUEST job2 = new Structs::JOBREQUEST();
			job2->header.method = "PRICEUPDATE";
			job2->header.protocol = "STOCK";
			job2->header.url = "INTC";
			job2->data = "25.45";
			stockEngine.PriceUpdate(job2);
			::Sleep(5000);
			stockEngine.AddMessage(job2);
			::Sleep(15000);
		}

		TEST_METHOD(TestRepoStock)
		{
			StockRepository repo;
			repo.Start();
			repo.CreateStock("INTC", "Intel Inc", "50.50");
			StockDef::LP_STOCKLISTING listing = repo.GetListing();
		}

		TEST_METHOD(TestSmtpAgent)
		{
			SMTPL::SmtpAgent agent;
			agent.Start();
			agent.Connect(512);
			agent.Connect(200);
			agent.Connect(300);
			agent.Data("The following example shows a namespace declaration and three ways that code outside the namespace can access their members.", 512);
		}

		TEST_METHOD(TestSmtpParser)
		{
			SMTPL::SmtpParser parser;
			parser.Parse("HELO relay.example.org\nMAIL FROM:<bob@example.org>\nRCPT TO:<alice@example.com>\nRCPT TO:<theboss@example.com>\nDATA\nFrom: \"Bob Example\" <bob@example.org>\nTo: \"Alice Example\" <alice@example.com>\nCc: theboss@example.com\nDate: Tue, 15 January 2008 16:02:43 -0500\nSubject: Test message\n\nHello Alice.\nThis is a test message with 5 header fields and 4 lines in the message body.\nYour friend,\nBob\n.\nQUIT\n");
			SMTPL::SmtpParser::LP_COMMANDSET cmd = (SMTPL::SmtpParser::LP_COMMANDSET)parser.Next();
			while (strcmp(cmd->opcode, "QUIT") != 0)
			{
				cmd = (SMTPL::SmtpParser::LP_COMMANDSET)parser.Next();
			}
		}


	};
}