
#include "AuthorizationServer.h"
#include "WorldServer.h"

#include <tchar.h>

#define AUTHORIZATION_THREADS_NUMBER	3
#define WORLD_THREADS_NUMBER			8

#define AUTHORIZATION_PORT	6669
#define WORLD_PORT			6671

// for memory leaks detect
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// entry point
int _tmain(int argc, _TCHAR* argv[])
{
	// check for memory leaks
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	printf("\n------------------------------------\n");
	printf("------------------------------------\n");
	printf("---     ENGINE v0.1 START    -------\n");
	printf("------------------------------------\n");
	printf("------------------------------------\n\n");

	io_service service;

	AuthorizationServer* authServer = new AuthorizationServer(AUTHORIZATION_PORT, service);
	printf("Loaded Authorization(%d) server.\n", AUTHORIZATION_PORT);

	WorldServer* worldServer = new WorldServer(WORLD_PORT, service);
	printf("Loaded World(%d) server.\n", WORLD_PORT);

	thread_group tAuthorizationGroup;
	for(int i = 0; i < AUTHORIZATION_THREADS_NUMBER; i++)
		tAuthorizationGroup.create_thread(bind(&io_service::run, ref(service)));
	printf("Authorization(%d) server started(%d threads).\n", AUTHORIZATION_PORT, AUTHORIZATION_THREADS_NUMBER);

	thread_group tWorldGroup;
	for(int i = 0; i < WORLD_THREADS_NUMBER; i++)
		tWorldGroup.create_thread(bind(&io_service::run, ref(service)));
	printf("World(%d) server started(%d threads).\n", WORLD_PORT, WORLD_THREADS_NUMBER);

	// read console input
	std::string msg;
	while(std::getline(std::cin, msg))
	{
		if(msg == "exit")
			break;
	}

	printf("\n---     ENGINE v0.1 SHUTDOWNING  ---\n\n");

	service.stop();

	tAuthorizationGroup.join_all();
	tWorldGroup.join_all();

	delete authServer;
	delete worldServer;

	return 0;
}
