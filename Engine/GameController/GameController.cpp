
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

	SAFE_DELETE_HASH_MAP_V(m_dynamicObjects);
	SAFE_DELETE_HASH_MAP_V(m_gameObjects);
	SAFE_DELETE_P(pGameController);
}

// Spawns an object with model from resource at (v) position
GameObject* GameController::spawnGameObject(uint16 resourceID, GHVECTOR v)
{
	GameObject* go = new GameObject(resourceID);
	go->move(v);
	add(go);
	return go;
}

// Spawns dynamic object with model from resource at (v) position
DynamicObject* GameController::spawnDynamicObject(uint16 resourceID, GHVECTOR v)
{
	DynamicObject* go = new DynamicObject(resourceID);
	go->move(v);
	addDynamic(go);
	return go;
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

uint32 GameController::generateDynamicKey()
{
	uint32 i = 1;

	// todo: temp
	std::hash_map<uint32,DynamicObject*>::iterator itr = m_dynamicObjects.find(i);
	while(itr != m_dynamicObjects.end())
	{
		i++;
		itr = m_dynamicObjects.find(i);
	}

	return i;
}

void GameController::addDynamic(DynamicObject* o)
{
	assert(o);

	uint32 id = generateDynamicKey();
	o->setID(id);
	m_dynamicObjects.insert(std::pair<uint32,DynamicObject*>(id, o));
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

void GameController::update(float dt)
{
	m_mutex.lock();


	m_mutex.unlock();
}
