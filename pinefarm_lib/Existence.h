#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stack>


using namespace std;


class Existence
{
public:
	Existence();
	~Existence();

private:
	SOCKET currentSocket;
	stack <SOCKET> socketList;

public:
	void AddSocket(SOCKET socket)
	{
		socketList.push(socket);
	}
};
