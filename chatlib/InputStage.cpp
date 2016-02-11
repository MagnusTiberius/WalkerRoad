#include "stdafx.h"
#include "InputStage.h"


InputStage::InputStage()
{

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("InputStage"));
	nThreads = THREAD_COUNT;
}


InputStage::~InputStage()
{
}

void InputStage::Start()
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


void InputStage::AddMessage(Structs::LP_JOBREQUEST job)
{
	jobList.push(job);
}

DWORD WINAPI InputStage::WorkerThread(LPVOID obj)
{
	InputStage* instance = (InputStage*)obj;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);
		if (instance->jobList.size() > 0)
		{
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			//==================




			//==================
			::ReleaseMutex(instance->ghMutex);
		}
		

		if (instance->jobList.size() == 0)
		{
			::ResetEvent(instance->ghHasMessageEvent);
		}
		::ReleaseMutex(instance->ghMutex);
	}

}