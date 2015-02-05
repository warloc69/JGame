
#pragma once

#include "GameObject.h"
#include <hash_map>
#include <iostream>
#include <boost\thread\thread.hpp>

#include "PacketHandler.h"

#include "..\Integration\Packets.h"
#include "..\Integration\Shared.h"

class GameController
{
	public:
		DLL_EXPORT static GameController* const getInstance();
		DLL_EXPORT void free();

		void spawnGameObject(GameObjectMasks type, GHVECTOR v);

		DLL_EXPORT bool readQueue(QueueTypes type);
		DLL_EXPORT void stopQueues();

		GameObject* findObject(uint32 id);

	protected:
		GameController();
		virtual ~GameController() {}

		void sendPacketToJavaServer(Packet p);
		
	private:
		bool m_read_queues;

		void add(GameObject* go);
		uint32 generateKey();

		std::hash_map<uint32,GameObject*> m_gameObjects;
};