#include "stdafx.h"
#include "TestListener1.h"


TestListener1::TestListener1()
{
}


TestListener1::~TestListener1()
{
}

void TestListener1::Start()
{
	if ((ThreadHandle = CreateThread(NULL, 0, TestListenerWorkerThread, this, 0, &ThreadID)) == NULL)
	{
		printf("CreateThread() failed with error %d\n", GetLastError());
		return;
	}
	else
		printf("CreateThread() is OK!\n");
}


DWORD WINAPI TestListener1::TestListenerWorkerThread(LPVOID obj)
{
	TestListener1* inst = (TestListener1*)obj;
	CompletionPortListener server;
	server.Start();
	WaitForSingleObject(inst->ThreadHandle, INFINITE);

	return 1;
}