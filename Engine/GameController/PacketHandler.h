
#pragma once

#include "Packet.h"
#include "Packets.h"
#include "GameObject.h"

class PacketHandler
{
	public:
		// main
		static void handle(Packet& p);

		// builders
		static Packet gameObjectSpawn(uint32 clientID, GameObject* go);
		static Packet gameObjectMove(uint32 clientID, GameObject* go);

		// handlers
		static void handleGameObjectMovePacket(Packet& p);
		static void handleGameObjectFirePacket(Packet& p);
		static void handleGameObjectSpawnPacket(Packet& p);
};

