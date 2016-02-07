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

ChatParseTree* ProtocolChatParser::Parse(string msg)
{
	Input(msg);
	scanner.Input(msg.c_str());
	CHAR* token;
	CHAR* contentLen = "0";

	const CHAR* c;

	token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	if (strcmp(token, "SAY") == 0 || strcmp(token, "LOGIN") == 0)
	{
		method.assign(token);
		scanner.SkipEmpty();
		token = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._");

		if (strlen(token) > 0)
		{
			scanner.SkipEmpty();
			token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
			token = scanner.AcceptRun("\/0123456789.");
			token = scanner.AcceptRun("\n");
		}


		token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		while (token != NULL)
		{
			if (strcmp(token, "name") == 0)
			{
				token = scanner.AcceptRun("=:");
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				name.assign(token);
			}
			if (strcmp(token, "content-length") == 0)
			{
				token = scanner.AcceptRun("=:");
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				contentLen = _strdup(token);
			}
			token = scanner.AcceptRun("\n");
			if (strcmp(token, "\n") == 0)
			{
				int a = 1;
			}
			if (strcmp(token, "\n\n") == 0)
			{
				content = scanner.GetByContentLength(atoi(contentLen));
				token = scanner.AcceptRun("\n");
				if (strcmp(token, "\n\n") == 0)
				{
					printf("method=%s; name=%s\n", method.c_str(), name.c_str());
					//printf("method=%s\n", method.c_str());
					ChatParseTree* tree = new ChatParseTree();
					tree->method = method;
					tree->name = name;
					tree->content = content;
					return tree;
				}
			}
			token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		}
		
	}
}