#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <cassert>
#include <assert.h> 
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Shlwapi.h"


#include "Existence.h"
#include "ProtocolChat.h"

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "11000"
#define DEFAULT_PORT_INT 11000
#define DEFAULT_PORT "11000"
#define DEFAULT_PORT_INT 11000
#define DATA_BUFSIZE 8192

class CompletionPortStackListener
{
public:
	CompletionPortStackListener();
	~CompletionPortStackListener();

	typedef struct
	{
		SOCKET socket;
		const char* data;
		DWORD len;
	} JOBREQUEST, *LP_JOBREQUEST;

private:
	static stack<LP_JOBREQUEST> jobList;
	HANDLE ghHasMessageEvent;
public:
	void AddJobRequest(SOCKET s, const char* data, DWORD len);

private:
	static DWORD WINAPI CompletionPortStackListener::WorkerThread(LPVOID obj);

};

