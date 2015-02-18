
#include "GameController.h"
#include "Macro.h"

#include <assert.h>
#include <Windows.h>

// global singleton instance of GameController
static GameController* pGameController = GameController::getInstance();

GameController::GameController() 
{
	m_read_queues = true;
}

GameController* const GameController::getInstance()
{
	if(!pGameController)
		pGameController = new GameController();

	return pGameController;
}

void GameController::free()
{
	stopQueues();

	SAFE_DELETE_HASH_MAP_V(m_gameObjects);
	SAFE_DELETE_P(pGameController);
}

// Spawns an object of (GameObjectMasks) type at (v) position
void GameController::spawnGameObject(GameObjectMasks type, GHVECTOR v)
{
	GameObject* go = new GameObject(type);
	go->move(v);
	add(go);

	// TODO: define which clients should receive spawn object information
	uint32 clientID = 0;

	Packet p = PacketHandler::spawnGameObject(clientID, go);
	sendPacketToJavaServer(p);
}

GameObject* GameController::findObject(uint32 id)
{
	std::hash_map<uint32,GameObject*>::iterator itr = m_gameObjects.find(id);
	if(itr != m_gameObjects.end())
		return itr->second;

	return NULL;
}

GameObject* GameController::findObjectByClient(uint32 clientID)
{
	std::hash_map<uint32,GameObject*>::iterator itr = m_gameObjects.begin();
	while(itr != m_gameObjects.end())
	{
		if(itr->second->getClientID() == clientID)
			return itr->second;
		itr++;
	}

	return NULL;
}

// auto-generate internal id for object
uint32 GameController::generateKey()
{
	uint32 i = 1;

	// todo: temp
	std::hash_map<uint32,GameObject*>::iterator itr = m_gameObjects.find(i);
	while(itr != m_gameObjects.end())
	{
		i++;
		itr = m_gameObjects.find(i);
	}

	return i;
}

void GameController::add(GameObject* o)
{
	assert(o);

	uint32 id = generateKey();
	o->setID(id);
	m_gameObjects.insert(std::pair<uint32,GameObject*>(id, o));
}

// method works in a separate thread
bool GameController::readQueue(QueueTypes type)
{
	try {
		// open shared memory
		shared_memory_object shm(open_only, SHARED_MEMORY_FILE, read_write);
		// map region
		mapped_region region(shm, read_write);
		// get region address
		void* addr = region.get_address();
		// cast memory to buffer
		shared_memory_buffer* data = static_cast<shared_memory_buffer*>(addr);

		// read loop
		while(m_read_queues)
		{
			Packet p;
			bool received = false;

			// queue is already threadsafe
			if(type == IN_QUEUE_MOVEMENT)
				received = data->inMovementQueue.pop(p);
			else if(type == IN_QUEUE_COLLISION)
				received = data->inCollisionQueue.pop(p);
			else if(type == OUT_QUEUE)
				received = data->outQueue.pop(p);

			if(!received)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				continue;
			}

			PacketHandler::handle(p);
		}
	} catch(interprocess_exception &ex) {
		MessageBoxA(NULL, ex.what(), "Error!", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		return false;
	}

	return true;
}

// sends packet to java server
void GameController::sendPacketToJavaServer(Packet out)
{
	try {
		// open shared memory
		shared_memory_object shm(open_only, SHARED_MEMORY_FILE, read_write);
		// map region
		mapped_region region(shm, read_write);
		// get region address
		void* addr = region.get_address();
		// cast memory to buffer
		shared_memory_buffer* data = static_cast<shared_memory_buffer*>(addr);
		// queue is already threadsafe
		data->outQueue.push(out);
	} catch(interprocess_exception &ex) {
		MessageBoxA(NULL, ex.what(), "Error!", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		return;
	}
}

void GameController::stopQueues()
{
	m_read_queues = false;
}

