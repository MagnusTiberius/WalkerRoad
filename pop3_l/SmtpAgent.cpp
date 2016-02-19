#include "stdafx.h"
#include "SmtpAgent.h"

namespace POP3L
{

	SmtpAgent::SmtpAgent()
	{
	}


	SmtpAgent::~SmtpAgent()
	{
	}


	void SmtpAgent::Start()
	{
		clientList = new CLIENTLIST();
	}

	SmtpAgent::LP_SOCKETCLIENT SmtpAgent::NewSocketClient()
	{
		SmtpAgent::LP_SOCKETCLIENT client = new SmtpAgent::SOCKETCLIENT();
		client->mailList = new MAILITEMLIST();
		client->isConnected = false;
		client->_socket = NULL;
		client->currentMail = NewMailItem();
		return client;
	}

	SmtpAgent::LP_MAILITEM SmtpAgent::NewMailItem()
	{
		SmtpAgent::LP_MAILITEM mail = new SmtpAgent::MAILITEM();
		ZeroMemory(mail, sizeof(SmtpAgent::MAILITEM));
		return mail;
	}

	char* SmtpAgent::ReceiptTo(char* value, SOCKET s)
	{
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
				client->currentMail->recptList.push_back(value);
			}
		}
		return NULL;
	}


	char* SmtpAgent::MailFrom(char* value, SOCKET s)
	{
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
				client->currentMail->fromList.push_back(value);
			}
		}
		return NULL;
	}

	void SmtpAgent::Connect(SOCKET s)
	{
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
			}
		}

		if (!found)
		{
			SmtpAgent::LP_SOCKETCLIENT c = NewSocketClient();
			c->_socket = s;
			c->parser = new POP3L::SmtpParser();
			c->commandListPtr = 0;
			c->isDataMode = false;
			clientList->push_back(c);
		}
	}

	SmtpAgent::LP_SOCKETCLIENT SmtpAgent::GetClient(SOCKET s)
	{
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				return *it;
			}
		}
		return NULL;
	}

	char* SmtpAgent::SendCommand(SOCKET s, char* command)
	{
		char* rv = NULL;
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				client->parser->Parse(command);
				int dataMode = client->parser->dataMode;
				if (client->parser->dataMode == 1)
				{
					client->parser->dataMode = 2;
				}

				if (dataMode == 0)
				{
					POP3L::SmtpParser::LP_COMMANDSET cmd = (POP3L::SmtpParser::LP_COMMANDSET)client->parser->Next();
					while (cmd != NULL)
					{
						client->commandList.push_back(cmd);
						cmd = (POP3L::SmtpParser::LP_COMMANDSET)client->parser->Next();
					}
				}
				return rv;
			}
		}

		return rv;
	}

	POP3L::SmtpParser::LP_COMMANDSET SmtpAgent::NextCommand(SOCKET s)
	{
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				POP3L::SmtpParser::LP_COMMANDSET ret = client->commandList[client->commandListPtr];
				return ret;
			}
		}
		return NULL;
	}

	void SmtpAgent::AddCommand(SOCKET s, POP3L::SmtpParser::LP_COMMANDSET cmd)
	{
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				client->commandList.push_back(cmd);
				return;
			}
		}
	}

	char* SmtpAgent::Data(char* value, SOCKET s)
	{
		bool found = false;
		SmtpAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			SmtpAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
				string str;
				if (client->currentMail->data == NULL)
				{
					str = "";
				}
				else
				{
					str.assign(client->currentMail->data);
				}
				str.append(value);
				client->currentMail->data = _strdup(str.c_str());
			}
		}
		return NULL;
	}

}

