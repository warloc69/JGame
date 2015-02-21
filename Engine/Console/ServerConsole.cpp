
#include <tchar.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <boost/thread.hpp>

#include "..\Integration\Shared.h"

#pragma comment(lib, "GameController.lib")
#include "..\GameController\GameController.h"

#include "ServerConsole.h"

// for memory leaks detect
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/*const int qSize = 1000;
const int pushPackets = 5000;
const int popPackets = pushPackets*4;
void testPopQueue(Queue<int,qSize> *q)
{
    boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();

	int k = 0;
	int j;
	while(true) {
		if(q->pop(j))
			k++;
		if(k >= pushPackets)
			break;
	}


    boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration msdiff = t2 - t1;
	printf("POP :: time = %d\n", msdiff.total_milliseconds());
	printf("POP :: success = %d\n", k);

	while(true) {}
	boost::this_thread::yield();
}

void testPushQueue(Queue<int,qSize> *q)
{
    boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();

	int k = 0;
	for(int i = 0; i <= pushPackets; i++)
	{
		boost::this_thread::sleep(boost::posix_time::microsec(250));
		if(!(q->push(i)))
			k++;
	}

    boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration msdiff = t2 - t1;
	printf("PUSH :: time = %d\n", msdiff.total_milliseconds());
	printf("PUSH :: errors = %d\n", k);

	while(true) {}
	boost::this_thread::yield();
}

void testQueue()
{
	Queue<int,qSize> q;
	boost::thread popThread = boost::thread(&testPopQueue, &q);
	boost::thread pushThread = boost::thread(&testPushQueue, &q);

	while(true) {}
}*/

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

	//testQueue();

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

// starts reading queue by game controller in a separate thread
void readSpawnQueue(GameController* gc)
{
	gc->readQueue(QueueTypes::IN_QUEUE_SPAWN);
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
	boost::thread m_threadSpawnQueue = boost::thread(&readSpawnQueue, gc);

	// read console input
	std::string msg;
	while(std::getline(std::cin, msg))
	{
		if(msg == "exit")
			break;
		if(msg == "spawn")
		{
			gc->spawnGameObject(0, GHVECTOR(10,50,10));
		}
	}

	// stop all threads
	m_threadMovementQueue.interrupt();
	m_threadCollisionQueue.interrupt();
	m_threadSpawnQueue.interrupt();

	// free resources
	gc->free();

	// free shared memory
	shared_memory_object::remove(SHARED_MEMORY_FILE);

	printf(" ::: ServerConsole stopped ::: \n");
}

