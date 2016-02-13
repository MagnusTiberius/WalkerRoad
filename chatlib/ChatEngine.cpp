#include "stdafx.h"
#include "ChatEngine.h"


ChatEngine::ChatEngine()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
	ghMutex2 = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("ChatEngine"));
	ghHasMessageEvent2 = CreateEvent(NULL, TRUE, FALSE, TEXT("ChatEngine2"));

	nThreads = THREAD_COUNT;
	ctr = 0;
	ctr2 = 0;
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
		if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread2, this, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error %d\n", GetLastError());
			return;
		}
		else
		{
			printf("CreateThread() is OK!\n");
			ghEvents2[i] = ThreadHandle;
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

void ChatEngine::SendJobMessage(Structs::LP_JOBREQUEST job)
{
	URLOBJECT* urlObject;
	stack<Structs::LP_JOBREQUEST>* conv;
	char* url = job->header.url;
	if (conversationList.find(url) != conversationList.end())
	{
		urlObject = conversationList[url];
		conv = urlObject->conversation;
	}
	else
	{
		urlObject = new URLOBJECT();
		urlObject->isBusy = false;
		urlObject->conversation = new stack<Structs::LP_JOBREQUEST>();
		urlObject->memberList = new vector<SOCKET>();
		conv = urlObject->conversation;
		conversationList[url] = urlObject;
	}
	conv->push(job);
	urlObject->conversation = conv;
	bool exists = false;
	for (auto &item : *urlObject->memberList)
	{
		if (item == job->socket)
		{
			exists = true;
			break;
		}
	}
	if (!exists)
	{
		urlObject->memberList->push_back(job->socket);
		conversationList[url] = urlObject;
	}
	::WaitForSingleObject(ghMutex2, INFINITE);
	ctr2++;
	::ReleaseMutex(ghMutex2);
	::SetEvent(ghHasMessageEvent2);
}

void ChatEngine::AddMessage(Structs::LP_JOBREQUEST job)
{
	jobList.push(job);
	::SetEvent(ghHasMessageEvent);
}

DWORD WINAPI ChatEngine::WorkerThread2(LPVOID obj)
{
	ChatEngine* instance = (ChatEngine*)obj;
	URLOBJECT* urlObject = NULL;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent2, INFINITE);
		::WaitForSingleObject(instance->ghMutex2, INFINITE);
		map<string, URLOBJECT*>::iterator itr;
		for (itr = instance->conversationList.begin(); itr != instance->conversationList.end(); itr++)
		{
			urlObject = itr->second;
			if (!urlObject->isBusy)
			{
				urlObject->isBusy = true;
				break;
			}
		}
		::ReleaseMutex(instance->ghMutex2);

		::WaitForSingleObject(instance->ghMutex2, INFINITE);
		stack<Structs::LP_JOBREQUEST>* stk = urlObject->conversation;
		if (stk->size() > 0)
		{
			Structs::LP_JOBREQUEST item = stk->top();
			stk->pop();
			instance->ctr2--;
			::ReleaseMutex(instance->ghMutex2);
			char* msg = _strdup(item->data);
			vector<SOCKET>* mbr = urlObject->memberList;
			vector<SOCKET>::iterator it2;
			string replyMsg;
			replyMsg.append(item->header.name);
			replyMsg.append(": ");
			replyMsg.append(item->data);
			for (it2 = mbr->begin(); it2 != mbr->end(); it2++)
			{
				SOCKET _socket = *it2;
#ifdef PRODUCTION
				int bRes = send(_socket, replyMsg.c_str(), replyMsg.length(), 0);
				//printf("Loop counter:===> %d; data=%s; len=%d; sent=%d\n", loopCtr1, nextJob->data, nextJob->len, bRes);
				if (bRes == SOCKET_ERROR)
				{
					printf("SOCKET_ERROR\n");
				}
#endif
			}
		}
		::ReleaseMutex(instance->ghMutex2);

		::WaitForSingleObject(instance->ghMutex2, INFINITE);
		urlObject->isBusy = false;
		if (instance->ctr2 <= 0)
		{
			::ResetEvent(instance->ghHasMessageEvent2);
		}
		::ReleaseMutex(instance->ghMutex2);
	}

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
				instance->SendJobMessage(job);
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