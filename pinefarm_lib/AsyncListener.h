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

class AsyncListener
{
public:
	AsyncListener();
	~AsyncListener();


public:
	void Start(int PortNum);

	typedef struct
	{
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[DEFAULT_BUFLEN];
		byte *LPBuffer;
		vector<byte> byteBuffer;
		DWORD BytesSEND;
		DWORD BytesRECV;
	} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

	typedef struct
	{
		SOCKET Socket;
	} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

	// Typedef definition
	typedef struct _SOCKET_INFORMATION {
		OVERLAPPED Overlapped;
		SOCKET Socket;
		CHAR Buffer[DATA_BUFSIZE];
		WSABUF DataBuf;
		DWORD BytesSEND;
		DWORD BytesRECV;
	} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

	HANDLE GetCompletionPort();

private:
	int Start();
	static void CALLBACK AsyncListener::WorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);
	static DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);
	HANDLE CompletionPort;
	int m_PortNum = DEFAULT_PORT_INT;
};

