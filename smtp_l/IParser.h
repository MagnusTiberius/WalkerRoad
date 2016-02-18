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

namespace SMTPL
{

	class IParser
	{
	public:
		IParser();
		~IParser();

	public:
		virtual void Input(const CHAR* msg) = 0;
		virtual LPVOID Parse(const CHAR* msg) = 0;
		virtual LPVOID Next() = 0;
	};

}