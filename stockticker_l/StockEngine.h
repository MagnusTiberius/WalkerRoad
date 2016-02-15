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
	static DWORD WINAPI WorkerThread(LPVOID obj);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	HANDLE ghHasMessageEvent;
	int nThreads;
	HANDLE ghMutex;
	HANDLE ghMutex2;
	int ctr;
	stack<Structs::LP_JOBREQUEST> jobList;
	stack<HANDLE> threadHandles;
	bool enableNextStage;

	StockDef::LP_STOCKLISTING stockListing;

private:
	void Subscribe(Structs::LP_JOBREQUEST job);

};

