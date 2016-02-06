#include "Entity.h"

Entity::Entity(SOCKET socket)
{
	_socket = socket;
}


SOCKET Entity::GetSocket()
{
	return _socket;
}

