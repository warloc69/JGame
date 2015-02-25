
#pragma once

#include "GameObject.h"
#include "DynamicObject.h"
#include <hash_map>
#include <iostream>
#include <boost\thread\thread.hpp>

#include "PacketHandler.h"
#include "Packets.h"

class GameController
{
	public:
		DLL_EXPORT static GameController* const getInstance();
		DLL_EXPORT void free();

		GameObject* spawnGameObject(uint16 resourceID, GHVECTOR v);
		DynamicObject* spawnDynamicObject(uint16 resourceID, GHVECTOR v);

		GameObject* findObject(uint32 id);
		GameObject* findObjectByClient(uint32 clientID);

	protected:
		GameController();
		virtual ~GameController() {}

	private:
		std::hash_map<uint32,GameObject*> m_gameObjects;
		void add(GameObject* go);
		uint32 generateKey();

		std::hash_map<uint32,DynamicObject*> m_dynamicObjects;
		void addDynamic(DynamicObject* go);
		uint32 generateDynamicKey();
};