#include "Existence.h"


ExistenceEngine::ExistenceEngine()
{

}

ExistenceEngine::~ExistenceEngine()
{

}

Entity* ExistenceEngine::Add(Entity* entity)
{
	Add(entity, mainLobby);
	//entity->SetCosmos(mainLobby);
	return entity;
}

Entity* ExistenceEngine::Add(Entity* entity, Cosmos* c)
{
	if (c == NULL)
	{
		mainLobby->Add(entity);
		entity->SetCosmosName(mainLobby->GetName());
	}
	else
	{
		c->Add(entity);
		string nm = c->GetName();
		entity->SetCosmosName(nm);
	}

	return entity;
}

Cosmos* ExistenceEngine::Add(Cosmos* c)
{
	cosmosList.push_back(c);
	return c;
}

void ExistenceEngine::AddMessage(string cosmosName, const char* msg)
{
	vector<Cosmos*>::iterator itCosmos;
	for (itCosmos = cosmosList.begin(); itCosmos != cosmosList.end(); itCosmos++)
	{
		Cosmos* cosmos = *itCosmos;
		string name = cosmos->GetName();
		if (name == cosmosName)
		{
			cosmos->AddMessage(msg);
		}
	}

}

int ExistenceEngine::Start()
{
	DWORD dwThreadId = GetCurrentThreadId();

	mainLobby = Add(new Cosmos("lobby"));

	if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
	{
		fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
		return 1;
	}
}

DWORD WINAPI ExistenceEngine::ServerWorkerThread(LPVOID lpObject)
{
	vector<Cosmos*>::iterator itCosmos;

	ExistenceEngine *obj = (ExistenceEngine*)lpObject;

	bool isLooping = true;
	do {
		for (itCosmos = obj->cosmosList.begin(); itCosmos != obj->cosmosList.end(); itCosmos++)
		{
			Cosmos* cosmos = *itCosmos;
			cosmos->Update();
		}
	} while (isLooping);
	return 1;
}