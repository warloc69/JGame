
#pragma once

#include "Packet.h"
#include "GameObject.h"

class PacketBuilder
{
	public:
		static Packet gameObjectSpawn(uint32 clientID, GameObject* go);
		static Packet gameObjectMove(uint32 clientID, GameObject* go);
};

