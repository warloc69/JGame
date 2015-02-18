
#include <tchar.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <boost\thread\thread.hpp>

#include "..\Integration\Shared.h"

#pragma comment(lib, "GameController.lib")
#include "..\GameController\GameController.h"

#include "ServerConsole.h"

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

	ServerConsole* sc = new ServerConsole();
	sc->start();
	delete sc;

	return 0;
}

// starts reading queue by game controller in a separate thread
void readMovementQueue(GameController* gc)
{
	gc->readQueue(QueueTypes::IN_QUEUE_MOVEMENT);
	boost::this_thread::yield();
}

// starts reading queue by game controller in a separate thread
void readCollisionQueue(GameController* gc)
{
	gc->readQueue(QueueTypes::IN_QUEUE_COLLISION);
	boost::this_thread::yield();
}

// initializes shared memory
bool ServerConsole::initSharedMemory()
{
	shared_memory_object::remove(SHARED_MEMORY_FILE);

	try {
		// opens shared memory file or creates if it not exists
		shared_memory_object shm(create_only, SHARED_MEMORY_FILE, read_write);
		// allocate memory by size of shared buffer
		shm.truncate(sizeof(shared_memory_buffer));
		// map the allocated region
		mapped_region region(shm, read_write);
	} catch(interprocess_exception &ex) {
		shared_memory_object::remove(SHARED_MEMORY_FILE);
		MessageBoxA(NULL, ex.what(), "Error!", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		return false;
	}

	return true;
}

void ServerConsole::start()
{
	printf(" ::: ServerConsole started ::: \n\n\n");

	if(!initSharedMemory())
	{
		printf("ERROR: cannot allocate shared memory. Exit from console.\n");
		return;
	}

	// start reading packets queues in separate threads
	GameController* gc = GameController::getInstance();
	boost::thread m_threadMovementQueue = boost::thread(&readMovementQueue, gc);
	boost::thread m_threadCollisionQueue = boost::thread(&readCollisionQueue, gc);

	// read console input
	std::string msg;
	while(std::getline(std::cin, msg))
	{
		if(msg == "exit")
			break;
		if(msg == "spawn")
		{
			gc->spawnGameObject(GameObjectMasks::OBJ_TYPE_MASK_CREATURE, GHVECTOR(10,50,10));
		}
	}

	// stop all threads
	m_threadMovementQueue.interrupt();
	m_threadCollisionQueue.interrupt();

	// free resources
	gc->free();

	// free shared memory
	shared_memory_object::remove(SHARED_MEMORY_FILE);

	printf(" ::: ServerConsole stopped ::: \n");
}

