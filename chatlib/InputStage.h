#pragma once
#include "stdafx.h"

class InputStage
{
public:
	InputStage();
	~InputStage();



	void Start();
	void AddMessage(Structs::LP_JOBREQUEST job);

private:
	static DWORD WINAPI WorkerThread(LPVOID obj);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	HANDLE ghHasMessageEvent;
	int nThreads;
	HANDLE ghMutex;

	stack<Structs::LP_JOBREQUEST> jobList;

};

