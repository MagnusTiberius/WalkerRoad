#pragma once
#include "CompletionPortListener.h"

using namespace SMTPL;

class Application
{
public:
	Application();
	~Application();

	void Start();

	CompletionPortListener listener;
};

