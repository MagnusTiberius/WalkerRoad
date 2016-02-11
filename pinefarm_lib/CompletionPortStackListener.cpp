#include "CompletionPortStackListener.h"


CompletionPortStackListener::CompletionPortStackListener()
{
	ghMutex = CreateMutex(
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
	jobList.push(job);


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
		
		if (instance->jobList.size() > 0)
		{
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
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
					Protocol* p = instance->_protocol; //*itr;
					if (p == NULL)
					{
						continue;
					}

					Structs::LP_JOBREQUEST jobresp = new Structs::JOBREQUEST();
					jobresp->data = job->data;
					jobresp->len = job->len;
					jobresp->socket = job->socket;

					p->AddMessage(_strdup(jobresp->data));
					LPVOID res = p->Parse();
					Structs::LP_JOBREQUEST nextJob = (Structs::LP_JOBREQUEST)p->Next();
					while (nextJob != NULL)
					{
						int bRes = send(_socket, nextJob->data, nextJob->len, 0);
						printf("Loop counter:===> %d; data=%s; len=%d; sent=%d\n", loopCtr1, nextJob->data, nextJob->len, bRes);
						if (bRes == SOCKET_ERROR)
						{
							printf("SOCKET_ERROR\n");
						}
						ZeroMemory(nextJob, sizeof(Structs::JOBREQUEST));
						free(nextJob);
						nextJob = (Structs::LP_JOBREQUEST)p->Next();
					}
					ZeroMemory(job, sizeof(Structs::JOBREQUEST));
					free(job);
				}
				catch (...)
				{
					int x = 1;
				}
			}
			::ReleaseMutex(instance->ghMutex);
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