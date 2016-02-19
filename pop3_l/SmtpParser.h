#pragma once
#include "stdafx.h"

namespace POP3L
{

	class SmtpParser
	{
	public:
		SmtpParser();
		~SmtpParser();

	private:
		const CHAR* _msg;
		ScannerA scanner;
		stack<char*> tokenList;

	public:
		void Input(const CHAR* msg);
		LPVOID Parse(const CHAR* msg);
		virtual LPVOID Next();

	public:
		int dataMode;

		typedef struct CommandSet {
			char* opcode;
			vector<char*>* params;
		} COMMANDSET, *LP_COMMANDSET;

	private:
		vector<SmtpParser::COMMANDSET> commandRequestList;
		LP_COMMANDSET cmd;
		int index;
	};


}