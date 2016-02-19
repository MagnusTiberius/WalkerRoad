#pragma once
#include "CompletionPortListener.h"

using namespace POP3L;

class Application
{
public:
	Application();
	~Application();

	void Start();

	CompletionPortListener listener;
};

