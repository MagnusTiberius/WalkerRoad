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

#include "ScannerA.h"
#include "ChatParseTree.h"

class ProtocolChatParser 
{
public:
	ProtocolChatParser();
	~ProtocolChatParser();

private:
	string _msg;

	ScannerA scanner;
	string  name;
	string content;
	string method;

public:
	void Input(string msg);
	ChatParseTree* Parse(string msg);
};

