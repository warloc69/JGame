
#include "AuthorizationServer.h"

#include <tchar.h>

#define AUTHORIZATION_THREADS_NUMBER	3

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

	io_service service;
	AuthorizationServer* server = new AuthorizationServer(6669, service);

	thread_group tGroup;
	for(int i = 0; i < AUTHORIZATION_THREADS_NUMBER; i++)
		tGroup.create_thread(bind(&io_service::run, ref(service)));

	// read console input
	std::string msg;
	while(std::getline(std::cin, msg))
	{
		if(msg == "exit")
			break;
	}

	service.stop();
	tGroup.join_all();

	delete server;

	return 0;
}
