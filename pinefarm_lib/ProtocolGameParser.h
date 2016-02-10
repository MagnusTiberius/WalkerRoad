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
#include "IParser.h"
#include "Parser.h"
#include "structs.h"

class ProtocolGameParser : public IParser
{
public:
	ProtocolGameParser();
	~ProtocolGameParser();

private:
	const CHAR* _msg;

	ScannerA scanner;
	const CHAR*  name;
	const CHAR* content;
	const CHAR* method;

public:
	void Input(const CHAR* msg);
	LPVOID Parse(const CHAR* msg);

};

