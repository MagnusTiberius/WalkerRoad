#include "stdafx.h"
#include "InputStage.h"

namespace SMTPL
{

	InputStage::InputStage()
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


	InputStage::~InputStage()
	{
	}

	void InputStage::EnableNextStage(bool v)
	{
		enableNextStage = v;
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
			{
				printf("CreateThread() is OK!\n");
				ghEvents[i] = ThreadHandle;
			}
		}
		//chatEngine = new ChatEngine();
		//chatEngine->Start();
		//gameEngine = new GameEngine();
		//gameEngine->Start();
		//stockEngine = new StockEngine();
		//stockEngine->Start();
	}

	void InputStage::Join()
	{
		::WaitForMultipleObjects(THREAD_COUNT, ghEvents, TRUE, INFINITE);
	}

	void InputStage::Stop()
	{
		DWORD dwCode = 0;

		for (int i = 0; i < THREAD_COUNT; i++)
		{
			::TerminateThread(ghEvents[i], dwCode);
		}
		int total = ctr;
		//chatEngine->Stop();
		//stockEngine->Stop();
	}


	void InputStage::AddMessage(Structs::LP_JOBREQUEST job)
	{
		::WaitForSingleObject(ghMutex2, INFINITE);
		jobList.push(job);
		::ReleaseMutex(ghMutex2);
		::SetEvent(ghHasMessageEvent);
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
				::WaitForSingleObject(instance->ghMutex2, INFINITE);
				Structs::LP_JOBREQUEST job = instance->jobList.top();
				instance->jobList.pop();
				::ReleaseMutex(instance->ghMutex2);
				if (job != NULL)
				{
					if (instance->enableNextStage)
					{
						if (strcmp(job->header.protocol, "GAME") == 0)
						{
							//instance->gameEngine->AddMessage(job);
						}

						if (strcmp(job->header.protocol, "CHAT") == 0)
						{
							//instance->chatEngine->AddMessage(job);
						}

						if (strcmp(job->header.protocol, "STOCK") == 0)
						{
							//instance->stockEngine->AddMessage(job);
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

}