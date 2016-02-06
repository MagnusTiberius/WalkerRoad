#include "Cosmos.h"

Cosmos::Cosmos(string name)
{
	_name = name;
}


void Cosmos::Update()
{
	int iSendResult;

	if (!messageList.empty())
	{
		string m = messageList.top();
		for (itEntity = entityList.begin(); itEntity != entityList.end(); itEntity++)
		{
			Entity* e = *itEntity;
			SOCKET s = e->GetSocket();

			iSendResult = send(s, m.c_str(), m.length(), 0);
		}
		messageList.pop();
	}
}

string Cosmos::GetName()
{
	return _name;
}

void Cosmos::Add(Entity* entity)
{
	entityList.push_back(entity);
}

void Cosmos::Add(string m)
{
	messageList.push(m);
}