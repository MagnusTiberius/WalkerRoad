#include "stdafx.h"
#include "ChatEngine.h"


ChatEngine::ChatEngine()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("ChatEngine"));
	nThreads = THREAD_COUNT;
	ctr = 0;
}


ChatEngine::~ChatEngine()
{
}



void ChatEngine::Start()
{
	for (int i = 0; i < nThreads; i++)
	{
		if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error %d\n", GetLastError());
			return;
		}
		else
		{
			printf("CreateThread() is OK!\n");
			ghEvents[i] = ThreadHandle;
		}
	}
}

void ChatEngine::Join()
{
	::WaitForMultipleObjects(THREAD_COUNT, ghEvents, TRUE, INFINITE);
}

void ChatEngine::Stop()
{
	DWORD dwCode = 0;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		::TerminateThread(ghEvents[i], dwCode);
	}
	int total = ctr;
}


void ChatEngine::AddMessage(Structs::LP_JOBREQUEST job)
{
	jobList.push(job);
	::SetEvent(ghHasMessageEvent);
}

DWORD WINAPI ChatEngine::WorkerThread(LPVOID obj)
{
	ChatEngine* instance = (ChatEngine*)obj;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);
		if (instance->jobList.size() > 0)
		{
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			if (job != NULL)
			{
				int c = 1;
			}
			instance->ctr++;
			::ReleaseMutex(instance->ghMutex);
		}


		if (instance->jobList.size() == 0)
		{
			::ResetEvent(instance->ghHasMessageEvent);
		}
		::ReleaseMutex(instance->ghMutex);
	}

}