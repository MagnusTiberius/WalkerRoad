#pragma once
#include "stdafx.h"


namespace SMTPL
{

	class SmtpAgent
	{
	public:
		SmtpAgent();
		~SmtpAgent();

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

		typedef vector<SmtpAgent::LP_MAILITEM> MAILITEMLIST, *LP_MAILITEMLIST;

		typedef struct SocketClient {
			SOCKET _socket;
			LP_MAILITEMLIST mailList;
			bool isConnected;
			LP_MAILITEM currentMail;
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
	};

}