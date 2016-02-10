#include "ProtocolGame.h"
#include "ProtocolGameParser.h"

ProtocolGame::ProtocolGame()
{
	protocolGameParser = (Parser*)(new ProtocolGameParser());
}


ProtocolGame::~ProtocolGame()
{
}

void ProtocolGame::Connect()
{

}

void ProtocolGame::AddMessage(const CHAR* msg)
{
	if (msg != NULL)
	{
		messageList.push(_strdup(msg));
	}
}

LPVOID ProtocolGame::Evaluate(LPVOID refdata)
{
	Structs::LP_JOBREQUEST req = (Structs::LP_JOBREQUEST)refdata;
	return req;
}


LPVOID ProtocolGame::Parse()
{
	const CHAR* data = messageList.top();
	if (data != NULL)
	{
		messageList.pop();
		protocolGameParser->Input(data);
		LPVOID rv = protocolGameParser->Parse(data);
		return(rv);
	}
	return NULL;
}

int ProtocolGame::Send(SOCKET socket)
{
	return 0;
}

void ProtocolGame::SetSocket(SOCKET socket)
{
	_socket = socket;
}