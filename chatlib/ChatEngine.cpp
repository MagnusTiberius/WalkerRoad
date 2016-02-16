#include "stdafx.h"
#include "ChatEngine.h"


ChatEngine::ChatEngine()
{
	ghMutex = CreateMutex(NULL, FALSE,  NULL);
	ghMutex2 = CreateMutex(NULL, FALSE, NULL);
	ghMutex3 = CreateMutex(NULL, FALSE, NULL);
	ghMutex4 = CreateMutex(NULL, FALSE, NULL);
	ghMutex5 = CreateMutex(NULL, FALSE, NULL);

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
	::WaitForSingleObject(ghMutex3, INFINITE);
	conv->push(job);
	urlObject->conversation = conv;
	::ReleaseMutex(ghMutex3);
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
		::WaitForSingleObject(ghMutex5, INFINITE);
		urlObject->memberList->push_back(job->socket);
		conversationList[url] = urlObject;
		::ReleaseMutex(ghMutex5);
	}
	::WaitForSingleObject(ghMutex2, INFINITE);
	ctr2++;
	::ReleaseMutex(ghMutex2);
	::SetEvent(ghHasMessageEvent2);
}

void ChatEngine::AddMessage(Structs::LP_JOBREQUEST job)
{
	::WaitForSingleObject(ghMutex4, INFINITE);
	jobList.push(job);
	::ReleaseMutex(ghMutex4);
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

		stack<Structs::LP_JOBREQUEST>* stk = urlObject->conversation;
		int sz = stk->size();
		if (sz > 0)
		{
			::WaitForSingleObject(instance->ghMutex3, INFINITE);
			Structs::LP_JOBREQUEST item = stk->top();
			stk->pop();
			::ReleaseMutex(instance->ghMutex3);
			instance->ctr2--;

			char* msg = _strdup(item->data);
			vector<SOCKET>* mbr = urlObject->memberList;
			vector<SOCKET>::iterator it2;
			char hdr[1024];
			ZeroMemory(hdr, 1024);
			sprintf(hdr, "SAY %s CHAT/1.0\ncontent-length:%d\ncontent-type:text\n\n%s\n\n", item->header.url, strlen(item->data), item->data);

			::WaitForSingleObject(instance->ghMutex5, INFINITE);
			for (it2 = mbr->begin(); it2 != mbr->end(); it2++)
			{
				SOCKET _socket = *it2;
#ifdef PRODUCTION
				int bRes = send(_socket, hdr, strlen(hdr), 0);
				//printf("Loop counter:===> %d; data=%s; len=%d; sent=%d\n", loopCtr1, nextJob->data, nextJob->len, bRes);
				if (bRes == SOCKET_ERROR)
				{
					printf("SOCKET_ERROR in ChatEngine::WorkerThread2\n");
				}
#endif
			}
			::ReleaseMutex(instance->ghMutex5);
		}

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
			::WaitForSingleObject(instance->ghMutex4, INFINITE);
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			::ReleaseMutex(instance->ghMutex4);
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