
#pragma once

#include "Packet.h"
#include "GameObject.h"

class PacketHandler
{
	public:
		// handlers
		static void handleMoveAndRotatePacket(Packet p);

		// builders
		static Packet spawnGameObject(GameObject* go);
};

