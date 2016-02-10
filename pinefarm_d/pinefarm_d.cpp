// pinefarm_d.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SyncListener.h"
#include "CompletionPortListener.h"
#define OPTION2


#ifdef OPTION1

class Application
{
public:
	void Start()
	{
		SyncListener listener;
		listener.Init();
	}
};

#endif

#ifdef OPTION2

class Application
{
public:
	void Start()
	{
		CompletionPortListener listener;
		listener.Start();
	}
};

#endif


int _tmain(int argc, _TCHAR* argv[])
{
	Application app;
	app.Start();

	return 0;
}

