#include "ProtocolGameParser.h"


ProtocolGameParser::ProtocolGameParser()
{
}


ProtocolGameParser::~ProtocolGameParser()
{
}

void ProtocolGameParser::Input(const CHAR* msg)
{
	_msg = msg;
}

LPVOID ProtocolGameParser::Parse(const CHAR* msg)
{
	Input(msg);
	scanner.Input(msg);
	CHAR* token;
	CHAR* contentLen = "0";
	CHAR* contentType = "0";
	CHAR* url;
	CHAR* app;
	CHAR* coordx = "";
	const CHAR* c;

	Structs::LP_JOBREQUEST jobreq = new Structs::JOBREQUEST();


	token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	if (strcmp(token, "SPAWN") == 0 || strcmp(token, "MOVE") == 0)
	{
		method = _strdup(token);
		jobreq->header.method = _strdup(token);

		if (strcmp(method, "SPAWN") == 0)
		{
			jobreq->sendResponse = true;
		}
		else if (strcmp(method, "MOVE") == 0)
		{
			jobreq->sendResponse = true;
		}
		else
		{
			return NULL;
		}

		scanner.SkipEmpty();
		token = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._");
		if (strlen(token) == 0)	return NULL;
		url = token;
		jobreq->header.url = _strdup(url);

		if (strlen(token) > 0)
		{
			scanner.SkipEmpty();
			token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
			if (strlen(token) == 0)	return NULL;
			if (strcmp(token, "GAME") == 0)
			{
				app = _strdup(token);
			}
			else
			{
				return NULL;
			}
			token = scanner.AcceptRun("\/0123456789.");
			if (strlen(token) == 0)	return NULL;
			token = scanner.AcceptRun("\n");
			if (strlen(token) == 0)	return NULL;
		}


		token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		while (token != NULL)
		{
			if (strcmp(token, "name") == 0)
			{
				token = scanner.AcceptRun("=:");
				if (strlen(token) == 0)	return NULL;
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				if (strlen(token) == 0)	return NULL;
				name = _strdup(token);
				jobreq->header.name = _strdup(token);
			}
			else if (strcmp(token, "content-length") == 0)
			{
				token = scanner.AcceptRun("=:");
				if (strlen(token) == 0)	return NULL;
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				if (strlen(token) == 0)	return NULL;
				contentLen = _strdup(token);
				jobreq->header.contentLen = _strdup(token);
				jobreq->len = atoi(contentLen);
			}
			else if (strcmp(token, "content-type") == 0)
			{
				token = scanner.AcceptRun("=:");
				if (strlen(token) == 0)	return NULL;
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				if (strlen(token) == 0)	return NULL;
				contentType = _strdup(token);
				jobreq->header.contentType = _strdup(contentType);
			}
			else if (strcmp(token, "coord-x") == 0)
			{
				token = scanner.AcceptRun("=:");
				if (strlen(token) == 0)	return NULL;
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
				if (strlen(token) == 0)	return NULL;
				coordx = _strdup(token);
				//printf("method=%s; coordx=%s\n", method, coordx);
				//jobreq->header.contentType = _strdup(contentType);
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
					printf("method=%s; name=%s; coordx=%s\n", method, name, coordx);
					return jobreq;
				}
			}
			token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
		}

	}
	return NULL;
}