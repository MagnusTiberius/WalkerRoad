#pragma once
#include "stdafx.h"

class InputStage
{
public:
	InputStage();
	~InputStage();



	void Start();
	void Stop();
	void Join();
	void AddMessage(Structs::LP_JOBREQUEST job);

private:
	HANDLE ghEvents[THREAD_COUNT];
	static DWORD WINAPI WorkerThread(LPVOID obj);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	HANDLE ghHasMessageEvent;
	int nThreads;
	HANDLE ghMutex;
	int ctr;
	stack<Structs::LP_JOBREQUEST> jobList;
	stack<HANDLE> threadHandles;
};

