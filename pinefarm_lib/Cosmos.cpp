#include "Cosmos.h"

Cosmos::Cosmos(string name)
{
	_name = name;
	ghHasMessageEvent = OpenEvent(EVENT_ALL_ACCESS | EVENT_MODIFY_STATE, FALSE, TEXT("HasMessageEvent"));
}


void Cosmos::Update()
{
	int iSendResult;
	dwThreadId = GetCurrentThreadId();
	if (!messageList.empty())
	{
		string m = messageList.top();
		messageList.pop();
		for (itEntity = entityList.begin(); itEntity != entityList.end(); itEntity++)
		{
			Entity* e = *itEntity;
			SOCKET s = e->GetSocket();
			if (e->IsAlive())
			{
				//printf("server: %d: %s \n\n", dwThreadId, m.c_str());
				iSendResult = send(s, m.c_str(), m.length(), 0);
				if (iSendResult > 0)
				{
					int a = 1;
				}
				if (iSendResult == -1)
				{
					// remove entity from entityList
					e->SetAliveStatus(false);
				}
			}
		}
		
		SetEvent(ghHasMessageEvent);
	}
	else
	{
		ResetEvent(ghHasMessageEvent);
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

void Cosmos::AddMessage(string m)
{
	messageList.push(m);
}