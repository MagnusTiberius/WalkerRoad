#include "Entity.h"

Entity::Entity(SOCKET socket)
{
	_socket = socket;
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
	int a = 1;
}


void Entity::SetCosmosName(string nm)
{
	_cosmosName.assign(nm);
}

string Entity::GetCosmosName()
{
	return _cosmosName;
}