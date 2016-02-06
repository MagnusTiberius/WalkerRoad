#pragma once


#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <vector>

#include "Entity.h"
#include "Cosmos.h"

#define DEFAULT_BUFLEN 1024


using namespace std;



class ExistenceEngine
{
public:
	ExistenceEngine();
	~ExistenceEngine();

private:
	SOCKET currentSocket;
	vector<Cosmos*> cosmosList;

	HANDLE ThreadHandle;
	DWORD ThreadID;

	Cosmos* mainLobby;

public:

	Entity* Add(Entity* entity);
	Entity* Add(Entity* entity, Cosmos* c);
	Cosmos* Add(Cosmos* c);



public:
	int Start();

private:
	static DWORD WINAPI ExistenceEngine::ServerWorkerThread(LPVOID lpObject);

};





