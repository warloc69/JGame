
#pragma once

#include "Packet.h"

class PacketHandler
{
	public:
		static void handle(Packet& p);

	private:
		static void handleGameObjectMovePacket(Packet& p);
		static void handleGameObjectFirePacket(Packet& p);
		static void handleGameObjectSpawnPacket(Packet& p);
};

