// pinefarm_d.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SyncListener.h"


class Application
{
public:
	void Start()
	{
		SyncListener listener;
		listener.Init();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Application app;
	app.Start();

	return 0;
}

