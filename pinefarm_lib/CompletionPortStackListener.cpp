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
	job->socket = s;

	string data(d);
	int dataLen = data.length();

	job->data = _strdup(data.c_str());
	job->len = dataLen;
	jobList.push(job);


	SetEvent(ghHasMessageEvent);
}

DWORD WINAPI CompletionPortStackListener::WorkerThread(LPVOID obj)
{
	CompletionPortStackListener* instance = (CompletionPortStackListener*)obj;

	stack<Structs::LP_JOBREQUEST> _jobList = instance->jobList;
	SOCKET _socket;
	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);
		if (instance->jobList.size() > 0)
		{
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			::ReleaseMutex(instance->ghMutex);
			//
			//
			_socket = job->socket;
			instance->_protocol->SetSocket(_socket);
			instance->_protocol->AddMessage(job->data);
			LPVOID res = instance->_protocol->Parse();
			Structs::LP_JOBREQUEST jr = (Structs::LP_JOBREQUEST)res;
			job = (Structs::LP_JOBREQUEST)instance->_protocol->Evaluate(res);
			if (job->sendResponse)
			{
				int bRes = send(_socket, job->data, job->len, 0);
				if (bRes == SOCKET_ERROR)
				{
					printf("SOCKET_ERROR\n");
				}
			}
			continue;
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