#include "CompletionPortStackListener.h"


CompletionPortStackListener::CompletionPortStackListener()
{
	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("HasMessageEvent_CompletionPortStackListener"));
}


CompletionPortStackListener::~CompletionPortStackListener()
{
}


void CompletionPortStackListener::AddJobRequest(SOCKET s, const char* d, DWORD n)
{
	LP_JOBREQUEST job = new JOBREQUEST;
	job->socket = s;
	job->data = d;
	job->len = n;
	jobList.push(job);
	SetEvent(ghHasMessageEvent);
}

DWORD WINAPI CompletionPortStackListener::WorkerThread(LPVOID obj)
{
	CompletionPortStackListener* instance = (CompletionPortStackListener*)obj;

	stack<LP_JOBREQUEST> _jobList = instance->jobList;

	while (true)
	{
		WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		if (_jobList.size() > 0)
		{
			LP_JOBREQUEST job = _jobList.top();
			jobList.pop();
		}
		else
		{
			ResetEvent(instance->ghHasMessageEvent);
		}
	}
	return 1;
}