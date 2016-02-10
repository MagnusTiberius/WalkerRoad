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

#include "CompletionPortListener.h"

using namespace std;

class TestListener1
{
public:
	TestListener1();
	~TestListener1();

	void Start();

private:
	HANDLE ThreadHandle;
	DWORD ThreadID;
	static DWORD WINAPI TestListenerWorkerThread(LPVOID obj);
};

