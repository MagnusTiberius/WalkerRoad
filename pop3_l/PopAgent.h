#pragma once
#include "stdafx.h"

namespace POP3L
{


	class PopAgent
	{
	public:
		PopAgent();
		~PopAgent();

	public:
		typedef struct MailItem {
			vector<char*> fromList;
			vector<char*> recptList;
			char* data;
			DWORD dwStatus;
			bool rcptToFlag;
			bool mailFromFlag;
			bool dataStartFlag;
			bool dataEndFlag;
		} MAILITEM, *LP_MAILITEM;

		typedef vector<PopAgent::LP_MAILITEM> MAILITEMLIST, *LP_MAILITEMLIST;

		typedef struct SocketClient {
			SOCKET _socket;
			POP3L::Pop3Parser* parser;
			LP_MAILITEMLIST mailList;
			bool isConnected;
			LP_MAILITEM currentMail;
			vector<POP3L::Pop3Parser::LP_COMMANDSET> commandList;
			int commandListPtr;
			bool isDataMode;
			char* user;
			char* passwd;
		} SOCKETCLIENT, *LP_SOCKETCLIENT;

		typedef vector<LP_SOCKETCLIENT> CLIENTLIST, *LP_CLIENTLIST;

		LP_CLIENTLIST clientList;

	public:
		void Start();
		LP_SOCKETCLIENT NewSocketClient();
		LP_MAILITEM NewMailItem();
		char* ReceiptTo(char* value, SOCKET s);
		char* MailFrom(char* value, SOCKET s);
		char* Data(char* value, SOCKET s);
		void Connect(SOCKET s);
		char* SendCommand(SOCKET s, char* command);
		PopAgent::LP_SOCKETCLIENT GetClient(SOCKET s);
		POP3L::Pop3Parser::LP_COMMANDSET NextCommand(SOCKET s);
	private:
		//SmtpParser parser;
		void AddCommand(SOCKET s, POP3L::Pop3Parser::LP_COMMANDSET cmd);


	private:
		const char* mailFormat1 = R"(Planck cautiously insisted that this was simply an aspect of the processes 
of absorption and emission of radiation and had nothing to do with the physical 
reality of the radiation itself.[6] In fact, he considered his quantum hypothesis 
a mathematical trick to get the right answer rather than a sizable discovery.[7] However, 
in 1905 Albert Einstein interpreted Planck's quantum hypothesis realistically and 
used it to explain the photoelectric effect, in which shining light on certain materials 
can eject electrons from the material. He won the 1921 Nobel Prize in Physics for this work.
)";

		const char* mailFormat2 = R"(By 1930, quantum mechanics had been further unified 
and formalized by the work of David Hilbert, Paul Dirac and 
John von Neumann[9] with greater emphasis on measurement, the statistical 
nature of our knowledge of reality, and philosophical speculation about 
the 'observer'. It has since permeated many disciplines including quantum 
chemistry, quantum electronics, quantum optics, and quantum information 
science. Its speculative modern developments include string theory and 
quantum gravity theories. It also provides a useful framework for many 
features of the modern periodic table of elements, and describes the 
behaviors of atoms during chemical bonding and the flow of electrons in 
computer semiconductors, and therefore plays a crucial role in many 
modern technologies.
)";



	};

}