// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <stack>
#include <vector>

#include "Entity.h"
#include "Cosmos.h"

#define DEFAULT_BUFLEN 1024

#define BUFSIZMAX		8*1024
#define BUFSIZMIN		1024
#define DATA_BUFSIZE	8192
#define DATASIZMIN		128
#define THREAD_COUNT 3

using namespace std;

#include "ChatEngine.h"
#include "GameEngine.h"

// TODO: reference additional headers your program requires here
