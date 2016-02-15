#include "stdafx.h"
#include "StockEngine.h"


StockEngine::StockEngine()
{

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	ghMutex2 = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("InputStage"));
	nThreads = THREAD_COUNT;
	ctr = 0;
	enableNextStage = true;
}


StockEngine::~StockEngine()
{
}

void StockEngine::EnableNextStage(bool v)
{
	enableNextStage = v;
}

void StockEngine::Start()
{
	stockListing = StockDef::Init();

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

void StockEngine::Join()
{
	::WaitForMultipleObjects(THREAD_COUNT, ghEvents, TRUE, INFINITE);
}

void StockEngine::Stop()
{
	DWORD dwCode = 0;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		::TerminateThread(ghEvents[i], dwCode);
	}
	int total = ctr;
}

void StockEngine::Subscribe(Structs::LP_JOBREQUEST job)
{
	int a = 1;
}

void StockEngine::AddMessage(Structs::LP_JOBREQUEST job)
{
	::WaitForSingleObject(ghMutex2, INFINITE);
	jobList.push(job);
	::ReleaseMutex(ghMutex2);
	::SetEvent(ghHasMessageEvent);
}

DWORD WINAPI StockEngine::WorkerThread(LPVOID obj)
{
	StockEngine* instance = (StockEngine*)obj;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);
		if (instance->jobList.size() > 0)
		{
			::WaitForSingleObject(instance->ghMutex2, INFINITE);
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			::ReleaseMutex(instance->ghMutex2);
			if (job != NULL)
			{
				if (instance->enableNextStage)
				{
					if (strcmp(job->header.protocol, "STOCK") == 0)
					{
						if (strcmp(job->header.method, "SUBSCRIBE") == 0)
						{
							instance->Subscribe(job);
						}
					}
				}
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