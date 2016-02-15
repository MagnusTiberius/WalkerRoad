#pragma once
#include "stdafx.h"
#include "StocksDefinition.h"

class StockEngine
{
public:
	StockEngine();
	~StockEngine();



	void Start();
	void Stop();
	void Join();
	void AddMessage(Structs::LP_JOBREQUEST job);
	void EnableNextStage(bool v);
private:
	HANDLE ghEvents[THREAD_COUNT];
	HANDLE ghEvents2[THREAD_COUNT];

	static DWORD WINAPI WorkerThread(LPVOID obj);
	static DWORD WINAPI PublishWorkerThread(LPVOID obj);
	
	HANDLE ThreadHandle;
	DWORD ThreadID;
	HANDLE ghHasMessageEvent;
	HANDLE ghHasMessageEvent2;
	int nThreads;
	HANDLE ghMutex;
	HANDLE ghMutex2;
	HANDLE ghMutex1;
	HANDLE ghMutex12;
	int ctr;
	stack<Structs::LP_JOBREQUEST> jobList;
	stack<HANDLE> threadHandles;
	bool enableNextStage;

	StockDef::LP_STOCKLISTING stockListing;

private:
	void Subscribe(Structs::LP_JOBREQUEST job);

};

