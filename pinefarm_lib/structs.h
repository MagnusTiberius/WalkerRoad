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

class Structs
{
public:
	typedef struct
	{
		char* method;
		char* contentType;
		char* contentLen;
		char* name;
		map<char*, char*> properties;
	} HEADER, *LP_HEADER;

	typedef struct
	{
		SOCKET socket;
		HEADER header;
		const char* data;
		DWORD len;
		bool sendResponse;
	} JOBREQUEST, *LP_JOBREQUEST, JOBRESPONSE, *LP_JOBRESPONSE;

};