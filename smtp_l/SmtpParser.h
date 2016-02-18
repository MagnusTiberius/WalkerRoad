#pragma once
#include "stdafx.h"

namespace SMTPL
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
		bool dataMode;

	public:
		void Input(const CHAR* msg);
		LPVOID Parse(const CHAR* msg);
		virtual LPVOID Next();

	public:
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