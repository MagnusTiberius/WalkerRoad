#include "ProtocolBase.h"


ProtocolBase::ProtocolBase()
{
	protocolChatParser = (Parser*)(new BaseParser());

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
}


ProtocolBase::~ProtocolBase()
{
}

void ProtocolBase::Connect()
{

}

LPVOID ProtocolBase::Next()
{
	Structs::LP_JOBREQUEST item = NULL;
	if (jobreqList.size() > 0)
	{
		item = (Structs::LP_JOBREQUEST)jobreqList.top();
		jobreqList.pop();
	}
	return item;
}

void ProtocolBase::AddMessage(const CHAR* msg)
{
	if (msg != NULL)
	{
		messageList.push(_strdup(msg));
	}
}

LPVOID ProtocolBase::Evaluate(LPVOID refdata)
{
	Structs::LP_JOBREQUEST req = (Structs::LP_JOBREQUEST)refdata;
	return req;
}


LPVOID ProtocolBase::Parse()
{
	::WaitForSingleObject(ghMutex, INFINITE);
	if (messageList.size() > 0)
	{
		const CHAR* data = messageList.top();
		if (data != NULL)
		{
			messageList.pop();
			::ReleaseMutex(ghMutex);
			protocolChatParser->Input(data);
			LPVOID rv = protocolChatParser->Parse(data);
			Structs::LP_JOBREQUEST item = (Structs::LP_JOBREQUEST)protocolChatParser->Next();
			while (item != NULL)
			{
				jobreqList.push(item);
				item = (Structs::LP_JOBREQUEST)protocolChatParser->Next();
			}
			return(rv);
		}
	}
	::ReleaseMutex(ghMutex);
	return NULL;
}

int ProtocolBase::Send(SOCKET socket)
{
	return 0;
}

void ProtocolBase::SetSocket(SOCKET socket)
{
	_socket = socket;
}