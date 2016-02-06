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

#include "Entity.h"

#define DEFAULT_BUFLEN 1024


using namespace std;

class Cosmos
{
public:
	Cosmos(string name);

	~Cosmos();

public:
	string GetName();

	void Add(Entity* entity);

	void Add(string m);

	void Update();

private:
	string _name;
	vector<Entity*> entityList;
	vector<Entity*>::iterator itEntity;

	stack<string> messageList;
};

