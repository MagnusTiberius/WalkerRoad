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

#define DEFAULT_BUFLEN 1024


using namespace std;

class Entity
{
public:
	Entity(SOCKET socket)
	{
		_socket = socket;
	}

	~Entity();

	SOCKET GetSocket()
	{
		return _socket;
	}

private:
	SOCKET _socket;
	string name;
};





class Cosmos
{
public:
	Cosmos(string name);

	~Cosmos();

public:
	string GetName()
	{
		return _name;
	}

	void Add(Entity* entity)
	{
		entityList.push_back(entity);
	}

	void Add(string m)
	{
		messageList.push(m);
	}

	void Update()
	{
		int iResult;
		int iSendResult;
		char recvbufloc[DEFAULT_BUFLEN];

		if (!messageList.empty())
		{
			string m = messageList.top();
			for (itEntity = entityList.begin(); itEntity != entityList.end(); itEntity++)
			{
				Entity* e = *itEntity;
				SOCKET s = e->GetSocket();
				iSendResult = send(s, recvbufloc, iResult, 0);
			}
			messageList.pop();
		}
	}

private:
	string _name;
	vector<Entity*> entityList;
	vector<Entity*>::iterator itEntity;

	stack<string> messageList;
};

Cosmos::Cosmos(string name)
{
	_name = name;
}



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

	Entity* Add(Entity* entity)
	{
		Add(entity, mainLobby);
		return entity;
	}

	Entity* Add(Entity* entity, Cosmos* c)
	{
		if (c == NULL)
		{
			mainLobby->Add(entity);
		}
		else
		{
			c->Add(entity);
		}

		return entity;
	}

	Cosmos* Add(Cosmos* c)
	{
		cosmosList.push_back(c);
		return c;
	}


public:
	int Start()
	{
		DWORD dwThreadId = GetCurrentThreadId();

		mainLobby = Add(new Cosmos("lobby"));

		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
			return 1;
		}
	}

private:
	static DWORD WINAPI ExistenceEngine::ServerWorkerThread(LPVOID lpObject)
	{
		vector<Cosmos*>::iterator itCosmos;

		ExistenceEngine *obj = (ExistenceEngine*)lpObject;

		bool isLooping = true;
		do {
			for (itCosmos = obj->cosmosList.begin(); itCosmos != obj->cosmosList.end(); itCosmos++)
			{
				Cosmos* cosmos = *itCosmos;
				cosmos->Update();
			}
		} while (isLooping);
	}

};





