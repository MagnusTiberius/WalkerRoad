#include "stdafx.h"
#include "StockEngine.h"


StockEngine::StockEngine()
{

	ghMutex = CreateMutex(NULL,FALSE, NULL);
	ghMutex2 = CreateMutex(NULL,  FALSE,  NULL);

	ghMutex1 = CreateMutex(NULL, FALSE, NULL);
	ghMutex12 = CreateMutex(NULL, FALSE, NULL);

	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("StockEngine001"));

	ghHasMessageEvent2 = CreateEvent(NULL, TRUE, FALSE, TEXT("StockEngine002"));

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

		if ((ThreadHandle = CreateThread(NULL, 0, PublishWorkerThread, this, 0, &ThreadID)) == NULL)
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
	StockDef::STOCKLISTING &listing = *stockListing;

	char* stockCode = job->header.url;
	map<string, StockDef::LP_STOCKPRICE> &stockList = *listing.stockList;
	StockDef::LP_STOCKPRICE lpItem = stockList[stockCode];
	StockDef::STOCKPRICE &item = *lpItem;
	StockDef::LP_SUBSCRIBER subscriber = new StockDef::SUBSCRIBER();
	subscriber->name = _strdup(job->header.name);
	subscriber->_socket = job->socket;
	map<string, StockDef::LP_SUBSCRIBER> &subscriberList = *item.subscriberList;
	string subscriberName(subscriber->name);
	subscriberList[subscriberName] = subscriber;
	::SetEvent(ghHasMessageEvent2);
}

void StockEngine::PriceUpdate(Structs::LP_JOBREQUEST job)
{
	StockDef::STOCKLISTING &listing = *stockListing;

	char* stockCode = job->header.url;
	map<string, StockDef::LP_STOCKPRICE> &stockList = *listing.stockList;
	StockDef::LP_STOCKPRICE lpItem = stockList[stockCode];
	StockDef::STOCKPRICE &item = *lpItem;

	item.currentPrice = job->data;
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
		::WaitForSingleObject(instance->ghMutex1, INFINITE);
		if (instance->jobList.size() > 0)
		{
			::WaitForSingleObject(instance->ghMutex12, INFINITE);
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			::ReleaseMutex(instance->ghMutex12);
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
						if (strcmp(job->header.method, "PRICEUPDATE") == 0)
						{
							instance->PriceUpdate(job);
						}
					}
				}
			}
			instance->ctr++;
			::ReleaseMutex(instance->ghMutex1);
		}
		

		if (instance->jobList.size() == 0)
		{
			::ResetEvent(instance->ghHasMessageEvent);
		}
		::ReleaseMutex(instance->ghMutex1);
	}

}

DWORD WINAPI StockEngine::PublishWorkerThread(LPVOID obj)
{
	StockEngine* instance = (StockEngine*)obj;
	StockDef::STOCKLISTING &listing = *instance->stockListing;

	struct sSubscriberPack {
		string name;
		SOCKET _socket;
		string data;
	} SUBSCRIBERPACK, *LP_SUBSCRIBERPACK;

	map<string, sSubscriberPack*>* publishList = new map<string, sSubscriberPack*>();
	map<string, sSubscriberPack*>::iterator itPublishList;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent2, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);
		if (listing.stockList->size() > 0)
		{
			StockDef::STOCKLIST &stockList = *listing.stockList;
			StockDef::STOCKLIST::iterator it1;
			for (it1 = stockList.begin(); it1 != stockList.end(); it1++)
			{
				StockDef::STOCKPRICE &item = *it1->second;
				StockDef::SUBSCRIBERLIST &subscriberList = *item.subscriberList;
				if (subscriberList.size() > 0)
				{
					StockDef::SUBSCRIBERLIST::iterator it2;
					StockDef::SUBSCRIBERLIST::iterator itInvalid;
					for (itInvalid = instance->disconnectedUsers.begin(); itInvalid != instance->disconnectedUsers.end(); itInvalid++)
					{
						StockDef::SUBSCRIBER &inv = *itInvalid->second;
						it2 = subscriberList.find(inv.name);
						if (it2 != subscriberList.end())
						{
							StockDef::LP_SUBSCRIBER &item = it2->second;
							//free(item->name);
							//delete item;
							//subscriberList.erase(it2);
							if (item->_socket == inv._socket)
							{
								item->isValid = false;
							}
						}
					}
					for (it2 = subscriberList.begin(); it2 != subscriberList.end(); it2++)
					{
						string name = it2->first;
						StockDef::SUBSCRIBER &subscriber = *it2->second;
						if (!subscriber.isValid)
						{
							continue;
						}
						char* stockCode = _strdup(item.stock->code);
						char* stockPrice = _strdup(item.currentPrice.c_str());
						char hdr[1024 * 16];
						ZeroMemory(hdr, 1024 * 16);
						sprintf_s(hdr, "%s=%s;", stockCode, stockPrice);
						itPublishList = publishList->find(name);
						if (itPublishList != publishList->end())
						{
							sSubscriberPack &subsbr = *itPublishList->second;
							subsbr.data.append(hdr);
						}
						else
						{
							sSubscriberPack* psubsbr = new sSubscriberPack();
							psubsbr->name = name;
							psubsbr->_socket = subscriber._socket;
							psubsbr->data.assign(hdr);
							map<string, sSubscriberPack*> &pubList = *publishList;
							pubList[name] = psubsbr;
						}

					}
				}
			}


			//if (instance->disconnectedUsers.size() > 0)
			//{
			//	vector<StockDef::SUBSCRIBER>::iterator itInvalid;
			//	for (itInvalid = instance->disconnectedUsers.begin(); itInvalid != instance->disconnectedUsers.end(); itInvalid++)
			//	{
			//		StockDef::SUBSCRIBER inv = *itInvalid;
			//		map<string, sSubscriberPack*>::iterator &it2 = publishList->find(inv.name);
			//		if (it2 != publishList->end())
			//		{
			//			sSubscriberPack* item = it2->second;
			//			delete item;
			//			publishList->erase(it2);
			//			instance->disconnectedUsers.erase(itInvalid);
			//		}
			//	}
			//}
			//instance->disconnectedUsers.clear();
			for (itPublishList = publishList->begin(); itPublishList != publishList->end(); itPublishList++)
			{
				sSubscriberPack &subscriber = *itPublishList->second;

				string strBody = "";
				strBody.append(subscriber.name);
				strBody.append(":");
				strBody.append(subscriber.data);


				char buf[1024 * 16];
				ZeroMemory(buf, 1024 * 16);

				sprintf_s(buf, "STOCK/1.0 200 OK\ncontent-type:text\ncontent-length:%d\n\n%s\n\n", strBody.length(), strBody.c_str());

				string strSend(buf);

				int bRes = send(subscriber._socket, strSend.c_str(), strSend.length(), 0);
				if (bRes == SOCKET_ERROR)
				{
					StockDef::SUBSCRIBERLIST::iterator itInv;
					itInv = instance->disconnectedUsers.find(subscriber.name);
					if (itInv == instance->disconnectedUsers.end())
					{
						StockDef::SUBSCRIBER* invsubsc = new StockDef::SUBSCRIBER();
						invsubsc->name.assign(subscriber.name);
						invsubsc->_socket = subscriber._socket;
						instance->disconnectedUsers[subscriber.name] = (invsubsc);
						printf("SOCKET_ERROR in PublishWorkerThread name=%s, socket=%d\n", subscriber.name.c_str(), subscriber._socket);
					}
				}
				subscriber.data.clear();
			}
			::Sleep(1500);
		}


		if (listing.stockList->size() == 0)
		{
			::ResetEvent(instance->ghHasMessageEvent2);
		}
		::ReleaseMutex(instance->ghMutex);
	}

}