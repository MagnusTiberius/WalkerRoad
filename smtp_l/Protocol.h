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

#include "IProtocol.h"

using namespace std;

class Protocol : public IProtocol
{
public:
	Protocol();
	~Protocol();

public:
	virtual void Connect();
	virtual LPVOID Evaluate(LPVOID refdata);
	virtual LPVOID Parse();
	virtual void AddMessage(const CHAR* msg);
	virtual int Send(SOCKET socket);
	virtual void SetSocket(SOCKET socket);
	virtual LPVOID Next();
};

