
#include "GameController.h"
#include "Macro.h"

#include <assert.h>
#include <Windows.h>

// global singleton instance of GameController
static GameController* pGameController = GameController::getInstance();

GameController::GameController() 
{
}

GameController* const GameController::getInstance()
{
	if(!pGameController)
		pGameController = new GameController();

	return pGameController;
}

void GameController::free()
{
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
