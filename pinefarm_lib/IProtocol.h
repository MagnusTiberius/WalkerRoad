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

using namespace std;

class IProtocol
{
public:
	IProtocol();
	~IProtocol();

public:
	virtual void Connect() = 0;
	virtual LPVOID Evaluate(LPVOID refdata) = 0;
	virtual LPVOID Parse() = 0;
	virtual void AddMessage(const CHAR* msg) = 0;
	virtual int Send(SOCKET socket) = 0;
	virtual void SetSocket(SOCKET socket) = 0;
	virtual LPVOID Next() = 0;
};

