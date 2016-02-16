#pragma once
#include "stdafx.h"

#define PRODUCTION

class ChatEngine
{
public:
	ChatEngine();
	~ChatEngine();

	void Start();
	void Stop();
	void Join();
	void AddMessage(Structs::LP_JOBREQUEST job);
	void SendJobMessage(Structs::LP_JOBREQUEST job);


private:
	HANDLE ghEvents[THREAD_COUNT];
	HANDLE ghEvents2[THREAD_COUNT];
	static DWORD WINAPI WorkerThread(LPVOID obj);
	static DWORD WINAPI WorkerThread2(LPVOID obj);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	HANDLE ghHasMessageEvent;
	int nThreads;
	HANDLE ghMutex;
	int ctr;
	int ctr2;
	stack<Structs::LP_JOBREQUEST> jobList;
	stack<HANDLE> threadHandles;

	HANDLE ghHasMessageEvent2;
	HANDLE ghMutex2;
	HANDLE ghMutex3;
	HANDLE ghMutex4;
	HANDLE ghMutex5;
private:
	typedef struct urlObject{
		bool isBusy;
		stack<Structs::LP_JOBREQUEST>* conversation;
		vector<SOCKET>* memberList;
	} URLOBJECT;

private:
	typedef stack<Structs::LP_JOBREQUEST> CONVERSATION, *LP_CONVERSATION;
	map<string, URLOBJECT*> conversationList;
	vector<SOCKET> disconnectedSockets;

};

