#include "stdafx.h"
#include "CompletionPortStackListener.h"

namespace SMTPL
{

	CompletionPortStackListener::CompletionPortStackListener()
	{
		ghMutex = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);
		ghMutex2 = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);
		ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("CompletionPortStackListener"));
		nThreads = THREAD_COUNT;
	}


	CompletionPortStackListener::~CompletionPortStackListener()
	{
	}

	void CompletionPortStackListener::Start()
	{
		inputStage = new InputStage();
		inputStage->Start();

		smtpAgent.Start();

		for (int i = 0; i < nThreads; i++)
		{
			if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
			{
				printf("CreateThread() failed with error %d\n", GetLastError());
				return;
			}
			else
				printf("CreateThread() is OK!\n");
		}
	}


	void CompletionPortStackListener::AddJobRequest(SOCKET s, const char* d, DWORD n)
	{
		Structs::LP_JOBREQUEST job = new Structs::JOBREQUEST;
		ZeroMemory(job, sizeof(Structs::JOBREQUEST));
		job->socket = s;

		string data(d);
		int dataLen = data.length();

		char* str = _strdup(data.c_str());
		job->data = _strdup(data.c_str());
		job->len = dataLen;
		job->socket = s;
		::WaitForSingleObject(ghMutex2, INFINITE);
		smtpAgent.SendCommand(s, _strdup(d));
		auto client = smtpAgent.GetClient(s);
		if (client->parser->dataMode == 2)
		{
			string msg = "354 End data with <CR><LF>.<CR><LF>\n";
			job->data = _strdup(msg.c_str());
			job->len = msg.length();
		}
		if (client->parser->dataMode == 3)
		{
			string msg = "...\n";
			job->data = _strdup(msg.c_str());
			job->len = msg.length();
		}
		if (client->parser->dataMode == 99)
		{
			string msg = "250 Ok: queued as 12345\n";
			job->data = _strdup(msg.c_str());
			job->len = msg.length();
		}
		jobList.push(job);
		::ReleaseMutex(ghMutex2);

		SetEvent(ghHasMessageEvent);
	}

	DWORD WINAPI CompletionPortStackListener::WorkerThread(LPVOID obj)
	{
		CompletionPortStackListener* instance = (CompletionPortStackListener*)obj;
		int loopCtr1 = 0;
		stack<Structs::LP_JOBREQUEST> _jobList = instance->jobList;
		SOCKET _socket;

		while (true)
		{
			::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
			::WaitForSingleObject(instance->ghMutex, INFINITE);
			loopCtr1++;

			::WaitForSingleObject(instance->ghMutex, INFINITE);
			int sz = instance->jobList.size();
			if (sz > 0)
			{
				::WaitForSingleObject(instance->ghMutex2, INFINITE);
				Structs::LP_JOBREQUEST job = instance->jobList.top();
				instance->jobList.pop();
				::ReleaseMutex(instance->ghMutex2);
				//
				//
				if (job == NULL)
				{
					continue;
				}
				_socket = job->socket;

				{
					try
					{

						SmtpAgent::LP_SOCKETCLIENT  client = instance->smtpAgent.GetClient(job->socket);
						if (client != NULL)
						{
							if (client->parser->dataMode == 2)
							{
								send(client->_socket, job->data, job->len, NULL);
								client->parser->dataMode = 3;
								continue;
							}

							if (client->parser->dataMode == 3)
							{
								send(client->_socket, job->data, job->len, NULL);
								continue;
							}

							if (client->parser->dataMode == 99)
							{
								send(client->_socket, job->data, job->len, NULL);
								client->parser->dataMode = 0;
								continue;
							}

							if (client->commandList.size() > 0)
							{
								SMTPL::SmtpParser::LP_COMMANDSET &cmd = client->commandList[client->commandListPtr];
								if (strcmp(cmd->opcode, "HELO") == 0)
								{
									char buf[1024];
									ZeroMemory(buf, 1024);
									auto s = cmd->params[0];
									sprintf_s(buf, "250 Hello %s", s[0]);
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
									int b = 1;
									client->commandListPtr++;
								}
								if (strcmp(cmd->opcode, "RCPTTO") == 0)
								{
									char buf[1024];
									ZeroMemory(buf, 1024);
									sprintf_s(buf, "250 Ok");
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
									client->commandListPtr++;
								}
								
								if (strcmp(cmd->opcode, "MAILFROM") == 0)
								{
									char buf[1024];
									ZeroMemory(buf, 1024);
									sprintf_s(buf, "250 Ok");
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
									client->commandListPtr++;
								}

								if (strcmp(cmd->opcode, "DATA") == 0)
								{
									char buf[1024];
									ZeroMemory(buf, 1024);
									sprintf_s(buf, "250 Ok: vvvv ooo xxxx");
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
								}
							}

							int b = 1;

						}
					}
					catch (...)
					{
						int x = 1;
					}
				}
			}

			if (instance->jobList.size() == 0)
			{
				::ResetEvent(instance->ghHasMessageEvent);
			}
			::ReleaseMutex(instance->ghMutex);

		}
		return 1;
	}

	void CompletionPortStackListener::SetParserHandler(Parser* p)
	{
		_parser = p;
	}

	void CompletionPortStackListener::SetProtocol(Protocol* p)
	{
		_protocol = p;
	}

	void CompletionPortStackListener::AddProtocol(Protocol* p)
	{
		protocolList.push_back(p);
	}
}