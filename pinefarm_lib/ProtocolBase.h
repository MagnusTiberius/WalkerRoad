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
#include "ProtocolChatParser.h"
#include "Parser.h"
#include "BaseParser.h"
#include "Protocol.h"

class ProtocolBase : public IProtocol
{
public:
	ProtocolBase();
	~ProtocolBase();

public:
	virtual void Connect();

	void AddMessage(const CHAR* msg);
	virtual LPVOID Evaluate(LPVOID refdata);
	virtual LPVOID Parse();
	virtual int Send(SOCKET socket);
	virtual void SetSocket(SOCKET socket);
	virtual LPVOID Next();
private:
	stack<const CHAR*> messageList;
	string inputData;
	Parser* protocolChatParser;
	SOCKET _socket;
	stack<Structs::LP_JOBREQUEST> jobreqList;
};

