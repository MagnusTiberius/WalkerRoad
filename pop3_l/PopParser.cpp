#include "stdafx.h"
#include "PopParser.h"


namespace POP3L
{

	Pop3Parser::Pop3Parser()
	{
		dataMode = 0;
		cmd = NULL;
		index = 0;
	}


	Pop3Parser::~Pop3Parser()
	{
	}


	void Pop3Parser::Input(const CHAR* msg)
	{
		_msg = msg;
	}

	LPVOID Pop3Parser::Next()
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


	LPVOID Pop3Parser::Parse(const CHAR* msg)
	{
		Input(msg);
		scanner.Input(msg);
		CHAR* token;
		bool isDone = false;

		while (!isDone)
		{

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
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}

			if (strcmp(token, "STAT") == 0)
			{
				isDone = true;
				cmd = new COMMANDSET();
				cmd->opcode = _strdup(token);
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}

			if (strcmp(token, "LIST") == 0)
			{
				isDone = true;
				cmd = new COMMANDSET();
				cmd->opcode = _strdup(token);
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}


			if (strcmp(token, "RETR") == 0)
			{
				cmd = new COMMANDSET();
				cmd->params = new vector<char*>();
				cmd->opcode = _strdup(token);
				scanner.SkipEmpty();
				token = scanner.AcceptRun("1234567890");
				cmd->params->push_back(_strdup(token));
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}

			if (strcmp(token, "DELE") == 0)
			{
				cmd = new COMMANDSET();
				cmd->params = new vector<char*>();
				cmd->opcode = _strdup(token);
				scanner.SkipEmpty();
				token = scanner.AcceptRun("1234567890");
				cmd->params->push_back(_strdup(token));
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}

			if (strcmp(token, "USER") == 0)
			{
				cmd = new COMMANDSET();
				cmd->params = new vector<char*>();
				cmd->opcode = _strdup(token);
				scanner.SkipEmpty();
				token = scanner.AcceptRun("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
				cmd->params->push_back(_strdup(token));
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}

			if (strcmp(token, "PASS") == 0)
			{
				cmd = new COMMANDSET();
				cmd->params = new vector<char*>();
				cmd->opcode = _strdup(token);
				scanner.SkipEmpty();
				token = scanner.AcceptRun("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
				cmd->params->push_back(_strdup(token));
				token = scanner.AcceptRun("\n");
				commandRequestList.push_back(*cmd);
				continue;
			}


			//token = scanner.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-.");
		}
		return NULL;
	}
}