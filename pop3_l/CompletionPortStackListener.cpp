#include "stdafx.h"
#include "CompletionPortStackListener.h"

namespace POP3L
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

		popAgent.Start();

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
		popAgent.SendCommand(s, _strdup(d));
		auto client = popAgent.GetClient(s);
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
						PopAgent::LP_SOCKETCLIENT  client = instance->popAgent.GetClient(job->socket);
						if (client != NULL)
						{

							if (client->commandList.size() > 0)
							{
								POP3L::Pop3Parser::LP_COMMANDSET &cmd = client->commandList[client->commandListPtr];
								if (strcmp(cmd->opcode, "USER") == 0)
								{
									char buf[1024];
									ZeroMemory(buf, 1024);
									auto s = cmd->params[0];
									sprintf_s(buf, "+OK User accepted");
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
									client->commandListPtr++;
								}
								if (strcmp(cmd->opcode, "PASS") == 0)
								{
									char buf[1024];
									ZeroMemory(buf, 1024);
									sprintf_s(buf, "+Ok Pass accepted");
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
									client->commandListPtr++;
								}
								
								if (strcmp(cmd->opcode, "LIST") == 0)
								{
									int n = client->mailList->size();

									char buf[1024];
									ZeroMemory(buf, 1024);
									sprintf_s(buf, "+Ok %d messages", n);
									string msg;
									msg.assign(buf);
									send(client->_socket, msg.c_str(), msg.length(), NULL);
									client->commandListPtr++;
								}

								if (strcmp(cmd->opcode, "RETR") == 0)
								{
									auto p = cmd->params[0];
									char* item = p[0];
									int i = atoi(item);
									PopAgent::MAILITEMLIST &ml = *client->mailList;
									PopAgent::LP_MAILITEM mi = ml[i];

									char buf[1024];
									ZeroMemory(buf, 1024);
									sprintf_s(buf, "+Ok xxx octets");
									string msg;
									msg.assign(buf);
									send(client->_socket, mi->data, strlen(mi->data), NULL);
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