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

ChatParseTree* ProtocolChat::Evaluate()
{
	string s = messageList.top();
	messageList.pop();
	inputData.assign(s);
	protocolChatParser.Input(inputData);
	return( protocolChatParser.Parse(inputData) );
}