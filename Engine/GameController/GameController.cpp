
#include "GameController.h"
#include "Macro.h"
#include "PacketHandler.h"
#include "..\Integration\Packets.h"
#include "..\Integration\Shared.h"

#include <assert.h>
#include <Windows.h>

// global singleton instance of GameController
static GameController* pGameController = GameController::getInstance();

GameController::GameController() 
{
	m_read_pos = 0;
	m_read_controller = true;
}

GameController* const GameController::getInstance()
{
	if(!pGameController)
		pGameController = new GameController();

	return pGameController;
}

void GameController::free()
{
	stopQueue();

	SAFE_DELETE_HASH_MAP_V(m_gameObjects);
	SAFE_DELETE_P(pGameController);
}

// Spawns an object of (GameObjectMasks) type at (v) position
void GameController::spawnGameObject(GameObjectMasks type, GHVECTOR v)
{
	GameObject* go = new GameObject(type);
	go->move(v);
	add(go);

	Packet p = PacketHandler::spawnGameObject(go);
	// TODO: send packet back to Java server
}

GameObject* GameController::findObject(uint32 id)
{
	std::hash_map<uint32,GameObject*>::iterator itr = m_gameObjects.begin();
	for(; itr != m_gameObjects.end(); itr++)
	{
		if(itr->first == id)
			return itr->second;
	}

	return NULL;
}

// auto-generate internal id for object
uint32 GameController::generateKey()
{
	uint32 i = 1;

	std::hash_map<uint32,GameObject*>::iterator itr = m_gameObjects.begin();
	for(; itr != m_gameObjects.end(); itr++)
	{
		if(itr->first == i)
		{
			i++;
			continue;
		}
		break;
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

/*JNIEnv* createJVM(JavaVM** jvm) 
{
    JNIEnv *env;
    JavaVMInitArgs vm_args;

    JavaVMOption options; 
    options.optionString = "-Djava.class.path=E:\\_Programming\\_Z&B\\_TW\\JGame\\Server\\src"; 
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = &options;
    vm_args.ignoreUnrecognized = 0;
    
    int ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
    if(ret < 0)
        printf("\nUnable to Launch JVM\n");       
    return env;
}*/

// method works in a separate thread
bool GameController::readQueue()
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
		while(m_read_controller)
		{
			Packet p;
			if(!data->queue.pop(p))
				continue;

			PacketHandler::handle(p);
		}
	} catch(interprocess_exception &ex) {
		MessageBoxA(NULL, ex.what(), "Error!", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		return false;
	}

	return true;
}

void GameController::stopQueue()
{
	m_read_controller = false;
}