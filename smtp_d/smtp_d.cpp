// smtp_d.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CompletionPortListener.h"

class Application
{
public:
	void Start()
	{
		CompletionPortListener listener;
		listener.Start();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Application app;
	app.Start();
	return 0;
}

