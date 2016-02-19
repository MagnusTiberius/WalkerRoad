#include "stdafx.h"
#include "SmtpParser.h"


namespace POP3L
{

	SmtpParser::SmtpParser()
	{
		dataMode = 0;
		cmd = NULL;
		index = 0;
	}


	SmtpParser::~SmtpParser()
	{
	}


	void SmtpParser::Input(const CHAR* msg)
	{
		_msg = msg;
	}

	LPVOID SmtpParser::Next()
	{
		if (commandRequestList.size() > 0 && index < commandRequestList.size())
		{
			COMMANDSET &item = commandRequestList[index];
			index++;

			LP_COMMANDSET pItem = &item;

			return (LPVOID)pItem;
		}
		return NULL;
	}


	LPVOID SmtpParser::Parse(const CHAR* msg)
	{
		Input(msg);
		scanner.Input(msg);
		CHAR* token;
		bool isDone = false;

		while (!isDone)
		{
			if (dataMode > 0)
			{
				token = scanner.AcceptRun("\n.");
				if (token == NULL)
				{
					token = scanner.AcceptUntil("\n");
					if (strlen(token) > 0)
					{
						cmd->params->push_back(_strdup(token));
					}
				}

				if (strcmp(token, ".\n") == 0)
				{
					commandRequestList.push_back(*cmd);
					dataMode = 99;
					continue;
				}
				if (strcmp(token, "") == 0)
				{
					isDone = true;
					break;
				}

				token = scanner.AcceptUntil("\n");
				if (strlen(token) > 0)
				{
					if (strcmp(token, "\n") != 0)
					{
						cmd->params->push_back(_strdup(token));
					}
				}
				continue;
			}

			scanner.SkipEmpty();
			token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

			if (token == NULL)
			{
				break;
			}

			if (strcmp(token, "QUIT") == 0)
			{
				isDone = true;
				cmd = new COMMANDSET();
				cmd->opcode = _strdup(token);
				commandRequestList.push_back(*cmd);
				continue;
			}

			if (strcmp(token, "HELO") == 0)
			{
				cmd = new COMMANDSET();
				cmd->params = new vector<char*>();
				cmd->opcode = _strdup(token);
				scanner.SkipEmpty();
				token = scanner.AcceptRun("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-.");
				cmd->params->push_back(_strdup(token));
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
			}

			if (strcmp(token, "MAIL") == 0)
			{
				scanner.SkipEmpty();
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
				if (strcmp(token, "FROM") == 0)
				{
					cmd = new COMMANDSET();
					cmd->params = new vector<char*>();
					cmd->opcode = _strdup("MAILFROM");
					token = scanner.AcceptRun(":");
					token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>@.");
					cmd->params->push_back(_strdup(token));
					commandRequestList.push_back(*cmd);
				}
			}

			if (strcmp(token, "RCPT") == 0)
			{
				scanner.SkipEmpty();
				token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
				if (strcmp(token, "TO") == 0)
				{
					cmd = new COMMANDSET();
					cmd->params = new vector<char*>();
					cmd->opcode = _strdup("RCPTTO");
					token = scanner.AcceptRun(":");
					token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>@.");
					cmd->params->push_back(_strdup(token));
					commandRequestList.push_back(*cmd);
				}
			}

			if (strcmp(token, "DATA") == 0)
			{
				cmd = new COMMANDSET();
				cmd->params = new vector<char*>();
				cmd->opcode = _strdup(token);
				scanner.SkipEmpty();
				token = scanner.AcceptRun("\n");
				dataMode = 1;
			}

			//token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-.");
		}
		return NULL;
	}
}