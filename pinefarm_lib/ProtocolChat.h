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

#include "IProtocol.h"

class ProtocolChat : public IProtocol
{
public:
	ProtocolChat();
	~ProtocolChat();

public:
	virtual void Connect();

	void AddMessage(string msg);
	void Evaluate();
private:
	stack<string> messageList;
};

