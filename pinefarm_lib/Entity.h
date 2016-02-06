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

class Entity
{
public:
	Entity(SOCKET socket);

	~Entity();

	SOCKET GetSocket();

private:
	SOCKET _socket;
	string name;
};