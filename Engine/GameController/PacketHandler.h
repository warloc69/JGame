
#pragma once

#include "..\Integration\Packet.h"
#include "GameObject.h"

class PacketHandler
{
	public:
		// main
		static void handle(Packet p);

		// builders
		static Packet spawnGameObject(uint32 clientID, GameObject* go);

	private:
		// handlers
		static void handleMoveAndRotatePacket(Packet p);
};

