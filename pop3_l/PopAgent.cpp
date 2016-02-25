#include "stdafx.h"
#include "PopAgent.h"


namespace POP3L
{

	PopAgent::PopAgent()
	{
	}


	PopAgent::~PopAgent()
	{
	}


	void PopAgent::Start()
	{
		clientList = new CLIENTLIST();
	}

	PopAgent::LP_SOCKETCLIENT PopAgent::NewSocketClient()
	{
		PopAgent::LP_SOCKETCLIENT client = new PopAgent::SOCKETCLIENT();
		client->mailList = new MAILITEMLIST();
		client->isConnected = false;
		client->_socket = NULL;
		client->currentMail = NewMailItem();
		return client;
	}

	PopAgent::LP_MAILITEM PopAgent::NewMailItem()
	{
		PopAgent::LP_MAILITEM mail = new PopAgent::MAILITEM();
		::ZeroMemory(mail, sizeof(PopAgent::MAILITEM));
		return mail;
	}

	char* PopAgent::ReceiptTo(char* value, SOCKET s)
	{
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
				client->currentMail->recptList.push_back(value);
			}
		}
		return NULL;
	}


	void PopAgent::PurgeMailList(SOCKET s)
	{
		PopAgent::LP_SOCKETCLIENT lpclient = GetClient(s);
		PopAgent::SOCKETCLIENT &client = *lpclient;

		vector<PopAgent::LP_MAILITEM>* lpmailList = new vector<PopAgent::LP_MAILITEM>();
		vector<PopAgent::LP_MAILITEM> &mailList = *lpmailList;

		vector<PopAgent::LP_MAILITEM>::iterator it1;

		for (it1 = client.mailList->begin(); it1 != client.mailList->end(); it1++)
		{
			PopAgent::LP_MAILITEM lpmi = *it1;
			PopAgent::MAILITEM &mi = *lpmi;
			if (mi.isDeletedFlag == false)
			{
				mailList.push_back(lpmi);
			}
		}
		client.mailList = &mailList;

	}

	char* PopAgent::MailFrom(char* value, SOCKET s)
	{
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
				client->currentMail->fromList.push_back(value);
			}
		}
		return NULL;
	}

	void PopAgent::Connect(SOCKET s)
	{
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				found = true;
			}
		}

		if (!found)
		{
			PopAgent::LP_SOCKETCLIENT c = NewSocketClient();
			c->_socket = s;
			c->parser = new POP3L::Pop3Parser();
			c->commandListPtr = 0;
			c->isDataMode = false;
			c->mailList = new vector<PopAgent::LP_MAILITEM>();

			PopAgent::LP_MAILITEM mail = NewMailItem();
			mail->data = _strdup(mailFormat1);
			mail->fromList.push_back("<ben@test.org>");
			mail->isDeletedFlag = false;
			c->mailList->push_back(mail);

			mail = NewMailItem();
			mail->data = _strdup(mailFormat2);
			mail->fromList.push_back("<jack@test.org>");
			mail->isDeletedFlag = false;
			c->mailList->push_back(mail);

			clientList->push_back(c);
		}
	}

	PopAgent::LP_SOCKETCLIENT PopAgent::GetClient(SOCKET s)
	{
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				return *it;
			}
		}
		return NULL;
	}

	char* PopAgent::SendCommand(SOCKET s, char* command)
	{
		char* rv = NULL;
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
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
					POP3L::Pop3Parser::LP_COMMANDSET cmd = (POP3L::Pop3Parser::LP_COMMANDSET)client->parser->Next();
					while (cmd != NULL)
					{
						client->commandList.push_back(cmd);
						cmd = (POP3L::Pop3Parser::LP_COMMANDSET)client->parser->Next();
					}
				}
				return rv;
			}
		}

		return rv;
	}

	POP3L::Pop3Parser::LP_COMMANDSET PopAgent::NextCommand(SOCKET s)
	{
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				POP3L::Pop3Parser::LP_COMMANDSET ret = client->commandList[client->commandListPtr];
				return ret;
			}
		}
		return NULL;
	}

	void PopAgent::AddCommand(SOCKET s, POP3L::Pop3Parser::LP_COMMANDSET cmd)
	{
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
			if (client->_socket == s)
			{
				client->commandList.push_back(cmd);
				return;
			}
		}
	}

	char* PopAgent::Data(char* value, SOCKET s)
	{
		bool found = false;
		PopAgent::CLIENTLIST::iterator it;
		for (it = clientList->begin(); it != clientList->end(); it++)
		{
			PopAgent::LP_SOCKETCLIENT &client = *it;
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