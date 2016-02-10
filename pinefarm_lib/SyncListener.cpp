#include "SyncListener.h"


SyncListener::SyncListener()
{
}


SyncListener::~SyncListener()
{
}

int SyncListener::Init()
{
	DWORD dwThreadId = GetCurrentThreadId();
	m_PortNum = DEFAULT_PORT_INT;


	DWORD Ret;

	ghHasMessageEvent = CreateEvent(NULL, TRUE,	FALSE,	TEXT("HasMessageEvent") );

	if (ghHasMessageEvent == NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		return 1;
	}

	engine = &ExistenceEngine::GetInstance();
	engine->Start();

	do
	{

		if ((Ret = WSAStartup((2, 2), &wsaData)) != 0)
		{
			fprintf(stderr, "%d::WSAStartup() failed with error %d\n", dwThreadId, Ret);
			return 1;
		}
		else
			fprintf(stderr, "%d::WSAStartup() is OK!\n", dwThreadId);

		InternetAddr.sin_family = AF_INET;
		InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		InternetAddr.sin_port = htons(m_PortNum);



		if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
		{
			fprintf(stderr, "%d::WSASocket() failed with error %d\n", dwThreadId, WSAGetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::WSASocket() is OK!\n", dwThreadId);

		int bRes = bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
		while (bRes == SOCKET_ERROR)
		{
			fprintf(stderr, "%d::bind() failed with error %d\nLooking for next port...\n", dwThreadId, WSAGetLastError());
			InternetAddr.sin_port = htons(++m_PortNum);
			bRes = bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
		}
		fprintf(stderr, "%d::bind() is fine! Port number at  %d\n", dwThreadId, m_PortNum);

		if (listen(ListenSocket, 5) == SOCKET_ERROR)
		{
			fprintf(stderr, "%d::listen() failed with error %d\n", dwThreadId, WSAGetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::listen() is working...\n", dwThreadId);

		try
		{
			Loop();
		}
		catch (...)
		{
			printf("Error in Init() --> dying\n");
		}
	} while (true);

	closesocket(ListenSocket);
}



int SyncListener::Loop()
{
	DWORD ThreadID;
	HANDLE ThreadHandle;
	DWORD dwThreadId = GetCurrentThreadId();
	try
	{
		do {
			ClientSocket = accept(ListenSocket, NULL, NULL);
			if (ClientSocket == INVALID_SOCKET) {
				printf("accept failed with error: %d\n", WSAGetLastError());
				int erno = WSAGetLastError();

				if (erno == WSANOTINITIALISED)
				{
					printf("WSANOTINITIALISED \n");
				}
				if (erno == WSAECONNRESET)
				{
					printf("WSAECONNRESET \n");
				}
				if (erno == WSAEFAULT)
				{
					printf("WSAEFAULT \n");
				}
				if (erno == WSAEINTR)
				{
					printf("WSAEINTR \n");
				}
				if (erno == WSAEINVAL)
				{
					printf("WSAEINVAL \n");
				}
				if (erno == WSAEINPROGRESS)
				{
					printf("WSAEINPROGRESS \n");
				}
				if (erno == WSAEMFILE)
				{
					printf("WSAEMFILE \n");
				}
				if (erno == WSAENETDOWN)
				{
					printf("WSAENETDOWN \n");
				}
				if (erno == WSAENOBUFS)
				{
					printf("WSAENOBUFS \n");
				}
				if (erno == WSAENOBUFS)
				{
					printf("WSAENOTSOCK \n");
				}
				if (erno == WSAENOTSOCK)
				{
					printf("WSAENOBUFS \n");
				}
				if (erno == WSAEOPNOTSUPP)
				{
					printf("WSAEOPNOTSUPP \n");
				}
				shutdown(ListenSocket, SD_BOTH);
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}

			//engine.Add(new Entity(ClientSocket));

			if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
			{
				fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
				return 1;
			}
			else
				fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

		} while (true);
	}
	catch (...)
	{
		printf("Error in Loop() --> dying\n");
	}

}


DWORD WINAPI SyncListener::ServerWorkerThread(LPVOID lpObject)
{
	int iResult;
	char recvbufloc[DEFAULT_BUFLEN];
	SyncListener *obj = (SyncListener*)lpObject;

	SOCKET socketloc = obj->ClientSocket;
	Entity* entity = new Entity(socketloc);
	obj->engine->Add(entity);

	do {
		ZeroMemory(recvbufloc, DEFAULT_BUFLEN);
		::Sleep(10);
		iResult = recv(socketloc, recvbufloc, obj->recvbuflen, 0);
		if (iResult > 0) 
		{
			string str(recvbufloc);
			const CHAR* inputbuf = _strdup(recvbufloc);
			//printf("Bytes received: %d : %s\n", iResult, str.c_str());
			string cosmosName = entity->GetCosmosName();
			//obj->engine->AddMessage(cosmosName, str.c_str());
			entity->AddMessage(inputbuf);
		}
		else if (iResult == 0)
		{
			printf("Connection closing...\n");
		}
		else  
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			shutdown(socketloc, SD_BOTH);
			closesocket(socketloc);
			//WSACleanup();
			return 1;
		}

	} while (iResult > 0);
	return 1;
}


int SyncListener::Start()
{

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else  {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
	return 1;
}
