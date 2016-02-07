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
#include "ProtocolChat.h"
#include "ChatParseTree.h"

#define DEFAULT_BUFLEN 1024

using namespace std;


class Entity
{
public:
	Entity(SOCKET socket);

	~Entity();

	SOCKET GetSocket();
	//void SetCosmos(Cosmos* cosmos);
	void AddMessage(string m);
	void SetCosmosName(string nm);
	string GetCosmosName();
	void SetAliveStatus(bool bStat);
	bool IsAlive();

private:
	SOCKET _socket;
	string name;
	bool _isAlive;
	ProtocolChat protocolChat;
	static DWORD WINAPI ServerWorkerThread(LPVOID lpObject);
	HANDLE ghEntityHasMessageEvent;
	DWORD ThreadID;
	HANDLE ThreadHandle;
	stack<string> messageList;
	
public:
	string _cosmosName;
};