#include "ProtocolChat.h"
#include "structs.h"

ProtocolChat::ProtocolChat()
{
	protocolChatParser = (Parser*)(new ProtocolChatParser());
}


ProtocolChat::~ProtocolChat()
{
}

void ProtocolChat::Connect()
{

}

void ProtocolChat::AddMessage(const CHAR* msg)
{
	messageList.push(_strdup(msg));
}

LPVOID ProtocolChat::Evaluate(LPVOID refdata)
{
	Structs::LP_JOBREQUEST req = (Structs::LP_JOBREQUEST)refdata;
	return req;
}


LPVOID ProtocolChat::Parse()
{
	const CHAR* data = messageList.top();
	messageList.pop();
	protocolChatParser->Input(data);
	LPVOID rv = protocolChatParser->Parse(data);
	return(rv);
}

int ProtocolChat::Send(SOCKET socket)
{
	return 0;
}

void ProtocolChat::SetSocket(SOCKET socket)
{
	_socket = socket;
}