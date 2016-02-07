#include "Entity.h"

Entity::Entity(SOCKET socket)
{
	_socket = socket;
	_isAlive = true;
	ghEntityHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD dwThreadId = GetCurrentThreadId();
	if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
	{
		fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
		return;
	}
	else
		fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

	return;
}


SOCKET Entity::GetSocket()
{
	return _socket;
}

//void Entity::SetCosmos(Cosmos* cosmos)
//{
//	_cosmos = cosmos;
//}

void Entity::AddMessage(string m)
{
	messageList.push(m);
	SetEvent(ghEntityHasMessageEvent);
}


void Entity::SetCosmosName(string nm)
{
	_cosmosName.assign(nm);
}

string Entity::GetCosmosName()
{
	return _cosmosName;
}

void Entity::SetAliveStatus(bool bStat)
{
	_isAlive = bStat;
}

bool Entity::IsAlive()
{
	return _isAlive;
}

DWORD WINAPI Entity::ServerWorkerThread(LPVOID lpObject)
{
	HANDLE ghEntityHasMessageEvent;
	Entity *obj = (Entity*)lpObject;

	ghEntityHasMessageEvent = obj->ghEntityHasMessageEvent;

	bool isLooping = true;
	do {
		WaitForSingleObject(ghEntityHasMessageEvent, INFINITE);
		string msg = obj->messageList.top();
		obj->messageList.pop();
		ResetEvent(ghEntityHasMessageEvent);
		obj->protocolChat.AddMessage(msg);
		obj->protocolChat.Evaluate();
	} while (isLooping);
	return 1;
}