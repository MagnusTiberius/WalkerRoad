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

	};

}