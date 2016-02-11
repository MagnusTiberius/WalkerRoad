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
#include <list>
#include "Shlwapi.h"


#include "Existence.h"
#include "ProtocolChat.h"
#include "Parser.h"
#include "Protocol.h"
#include "structs.h"

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "11000"
#define DEFAULT_PORT_INT 11000
#define DEFAULT_PORT "11000"
#define DEFAULT_PORT_INT 11000
#define DATA_BUFSIZE 8192
#define THREAD_COUNT 5

using namespace std;

class CompletionPortStackListener
{
public:
	CompletionPortStackListener();
	~CompletionPortStackListener();

	//typedef struct
	//{
	//	char* method;
	//	char* contentType;
	//	map<char*, char*> properties;
	//} HEADER, *LP_HEADER;

	//typedef struct
	//{
	//	SOCKET socket;
	//	HEADER header;
	//	const char* data;
	//	DWORD len;
	//} JOBREQUEST, *LP_JOBREQUEST;

private:
	stack<Structs::LP_JOBREQUEST> jobList;
	vector<Protocol*> protocolList;
	//vector<Protocol*>::iterator itrProtocolList;
	HANDLE ghHasMessageEvent;
	int nThreads;
	HANDLE ghMutex;
public:
	void AddJobRequest(SOCKET s, const char* data, DWORD len);
	void Start();
	void SetParserHandler(Parser* p);
	void SetProtocol(Protocol* p);
	void AddProtocol(Protocol* p);
private:
	static DWORD WINAPI CompletionPortStackListener::WorkerThread(LPVOID obj);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	Parser* _parser;
	Protocol* _protocol;
};

