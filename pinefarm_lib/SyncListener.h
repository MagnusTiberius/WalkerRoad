#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Existence.h"

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "11000"
#define DEFAULT_PORT_INT 11000



class SyncListener
{
public:
	SyncListener();
	~SyncListener();

private:
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	SOCKADDR_IN InternetAddr;
	int m_PortNum;

	ExistenceEngine engine;
	HANDLE ghHasMessageEvent;
public:

	int Init();

	int Loop();

	static DWORD WINAPI SyncListener::ServerWorkerThread(LPVOID lpObject);

	int Start();
};

