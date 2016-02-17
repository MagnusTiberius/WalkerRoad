#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <vector>

#define DEFAULT_BUFLEN 1024

using namespace std;

//#include "stdafx.h"


class ScannerA
{
public:
	ScannerA();
	~ScannerA();

	void Input(const CHAR* str);
	bool Accept(CHAR* str);
	CHAR* AcceptRun(CHAR* str);
	void Backup();
	const CHAR* Peek();
	const CHAR* Next();
	bool IsNumeric();
	bool IsAlphabetic();
	bool IsAlphanumeric();
	bool IsEmpty();
	void SkipEmpty();
	CHAR* AcceptUntil(CHAR *str);
	bool IsEOS();
	int GetCurrentPosition();
	string GetByContentLength(int length);
protected:
	const CHAR* m_str;
	int m_start;
	int m_end;
	int m_pos;
	const CHAR *m_c;
	CHAR m_token[DEFAULT_BUFLEN];
};

/*
References:

1) http://cuddle.googlecode.com/hg/talk/lex.html#slide-31


*/