#include "ProtocolChatParser.h"
#include "structs.h"

ProtocolChatParser::ProtocolChatParser()
{
}


ProtocolChatParser::~ProtocolChatParser()
{
}


void ProtocolChatParser::Input(const CHAR* msg)
{
	_msg = msg;
}

LPVOID ProtocolChatParser::Parse(const CHAR* msg)
{
	Input(msg);
	scanner.Input(msg);
	CHAR* token;
	CHAR* contentLen = "0";

	const CHAR* c;

	Structs::LP_JOBREQUEST jobreq = new Structs::JOBREQUEST();


	token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	if (strcmp(token, "SAY") == 0 || strcmp(token, "LOGIN") == 0)
	{
		method = _strdup(token);
		jobreq->header.method = _strdup(token);
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
				name = _strdup(token);
				jobreq->header.name = _strdup(token);
			}
			if (strcmp(token, "content-length") == 0)
			{
				token = scanner.AcceptRun("=:");
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				contentLen = _strdup(token);
				jobreq->header.contentLen = _strdup(token);
				jobreq->len = atoi(contentLen);
			}
			token = scanner.AcceptRun("\n");
			if (strcmp(token, "\n") == 0)
			{
				int a = 1;
			}
			if (strcmp(token, "\n\n") == 0)
			{
				string cl = scanner.GetByContentLength(atoi(contentLen));
				content = cl.c_str();
				jobreq->data = _strdup(content);
				token = scanner.AcceptRun("\n");
				if (strcmp(token, "\n\n") == 0)
				{
					printf("method=%s; name=%s\n", method, name);
					return jobreq;
				}
			}
			token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		}
		
	}
	return NULL;
}