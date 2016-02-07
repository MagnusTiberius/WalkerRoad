#include "ProtocolChat.h"


ProtocolChat::ProtocolChat()
{
}


ProtocolChat::~ProtocolChat()
{
}

void ProtocolChat::Connect()
{

}

void ProtocolChat::AddMessage(string msg)
{
	messageList.push(msg);
}

void ProtocolChat::Evaluate()
{
	string s = messageList.top();
	messageList.pop();
	inputData.append(s);
}