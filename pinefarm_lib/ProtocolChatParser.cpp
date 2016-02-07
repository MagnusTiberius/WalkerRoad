#include "ProtocolChatParser.h"


ProtocolChatParser::ProtocolChatParser()
{
}


ProtocolChatParser::~ProtocolChatParser()
{
}


void ProtocolChatParser::Input(string msg)
{
	_msg = msg;
}

void ProtocolChatParser::Parse(string msg)
{
	Input(msg);
	scanner.Input(msg.c_str());
	CHAR* token;
	token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	if (strcmp(token, "SAY") == 0)
	{
		scanner.SkipEmpty();
		token = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._");
		if (strlen(token) > 0)
		{
			scanner.SkipEmpty();
			token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
			token = scanner.AcceptRun("\/0123456789.");
			token = scanner.AcceptRun("\n");
		}
		token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		token = scanner.AcceptRun("=:");
		token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
		token = scanner.AcceptRun("\n");
		token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		token = scanner.AcceptRun("=:");
		token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
		token = scanner.AcceptRun("\n");
	}
}