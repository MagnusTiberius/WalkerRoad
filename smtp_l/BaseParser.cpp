#include "stdafx.h"
#include "BaseParser.h"

namespace SMTPL
{

	BaseParser::BaseParser()
	{
	}


	BaseParser::~BaseParser()
	{
	}


	void BaseParser::Input(const CHAR* msg)
	{
		_msg = msg;
	}

	LPVOID BaseParser::Next()
	{
		Structs::JOBREQUEST* item = NULL;
		if (jobreqList.size() > 0)
		{
			item = jobreqList.top();
			jobreqList.pop();
		}
		return (item);
	}

	LPVOID BaseParser::Parse(const CHAR* msg)
	{
		Input(msg);
		scanner.Input(msg);
		CHAR* token;
		CHAR* contentLen = "0";
		CHAR* contentType = "0";
		CHAR* url;
		CHAR* app;
		CHAR* protocol;
		CHAR* version;
		CHAR* propertyName;
		CHAR* propertyValue;
		const CHAR* c;


		Structs::LP_JOBREQUEST jobreq;



		while (true)
		{
			scanner.SkipEmpty();
			token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

			if (token == NULL)
			{
				break;
			}

			jobreq = new Structs::JOBREQUEST();
			ZeroMemory(jobreq, sizeof(Structs::JOBREQUEST));
			method = _strdup(token);
			jobreq->header.method = _strdup(token);


			scanner.SkipEmpty();
			token = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._");
			if (strlen(token) == 0)	break;;
			url = token;
			jobreq->header.url = _strdup(url);

			if (strlen(token) > 0)
			{
				scanner.SkipEmpty();
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
				if (strlen(token) == 0)	break;;
				if (strlen(token) > 0)
				{
					protocol = _strdup(token);
					jobreq->header.protocol = _strdup(protocol);
				}
				else
				{
					break;
				}
				scanner.SkipEmpty();
				token = scanner.AcceptRun("\/");
				if (strlen(token) == 0)	break;
				scanner.SkipEmpty();
				token = scanner.AcceptRun("0123456789.");
				if (strlen(token) == 0)	break;
				version = _strdup(token);
				jobreq->header.version = _strdup(version);
				token = scanner.AcceptRun("\n");
				if (strlen(token) == 0)	break;
			}


			token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
			while (token != NULL)
			{
				if (strcmp(token, "name") == 0)
				{
					token = scanner.AcceptRun("=: ");
					if (strlen(token) == 0)	break;
					token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
					if (strlen(token) == 0)	break;
					name = _strdup(token);
					jobreq->header.name = _strdup(token);
				}
				else if (strcmp(token, "content-length") == 0)
				{
					token = scanner.AcceptRun("=: ");
					if (strlen(token) == 0)	break;
					token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
					if (strlen(token) == 0)	break;
					contentLen = _strdup(token);
					jobreq->header.contentLen = _strdup(token);
					jobreq->len = atoi(contentLen);
				}
				else if (strcmp(token, "content-type") == 0)
				{
					token = scanner.AcceptRun("=: ");
					if (strlen(token) == 0)	break;
					token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
					if (strlen(token) == 0)	break;
					contentType = _strdup(token);
					jobreq->header.contentType = _strdup(contentType);
				}
				else
				{
					propertyName = _strdup(token);
					token = scanner.AcceptRun("=: ");
					if (strlen(token) == 0)	break;
					token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-01234567890");
					if (strlen(token) == 0)	break;
					propertyValue = _strdup(token);
					if (jobreq->header.properties == NULL)
					{
						jobreq->header.properties = new map<string, char*>();
					}
					map<string, char*> &prp = *jobreq->header.properties;
					prp[string(propertyName)] = _strdup(propertyValue);
					//jobreq->header.properties = &prp;
					//map<string, char*> *prp = &jobreq->header.properties;
					//prp->emplace(make_pair(string(propertyName), propertyValue));
					//int sz = prp->size();
					//*prp[string(propertyName)] = _strdup(propertyValue);
					//jobreq->header.properties[_strdup(propertyName)] = _strdup(propertyValue);
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
					jobreqList.push(jobreq);
					break;
				}
				token = scanner.AcceptRun("\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-");
			}
		}
		return NULL;
	}
}