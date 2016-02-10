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

#include "CompletionPortStackListener.h"
#include "ProtocolGame.h"

class CompletionPortListener
{
public:
	CompletionPortListener();
	~CompletionPortListener();

public:
	typedef struct
	{
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[DATA_BUFSIZE];
		DWORD BytesSEND;
		DWORD BytesRECV;
	} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

	// Structure definition
	typedef struct
	{
		SOCKET Socket;
	} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

	static DWORD WINAPI ServerWorkerThread(LPVOID obj);
	HANDLE CompletionPort;

private:
	SOCKADDR_IN InternetAddr;
	SOCKET Listen;
	HANDLE ThreadHandle;
	SOCKET Accept;
	SYSTEM_INFO SystemInfo;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	int i;
	DWORD RecvBytes;
	DWORD Flags;
	DWORD ThreadID;
	WSADATA wsaData;
	DWORD Ret;

	CompletionPortStackListener completionPortStackListener;

public:
	int Start();
};

//References:
//http://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5i.html
