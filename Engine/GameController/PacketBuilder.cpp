
#include "PacketBuilder.h"
#include "Packets.h"
#include "Macro.h"

Packet PacketBuilder::gameObjectSpawn(uint32 clientID, GameObject* go)
{
	//	Packet format:
	//		id		- clientID	- objectID	- resource  - position		- rotation
	//		uint16	- uint32	- uint32	- uint16	- 3 x float		- 4 x float
	//  Total = 2 + 4 + 34 bytes

	Packet p;

	p << (uint8) 46; // data size
	
	p << (uint16) E_PKT_GO_SPAWN; // id

	p << clientID; // client id

	p << go->getID(); // object id
	
	p << go->getResourceID(); // resource id
	
	p << go->getPosition().x;
	p << go->getPosition().y;
	p << go->getPosition().z;

	p << go->getRotation().x;
	p << go->getRotation().y;
	p << go->getRotation().z;
	p << go->getRotation().w;

	p << go->getVelocity().x;
	p << go->getVelocity().y;
	p << go->getVelocity().z;

	p.finalize();

	return p;
}

Packet PacketBuilder::gameObjectMove(uint32 clientID, GameObject* go)
{
	//	Packet format:
	//		id		- clientID	- objectID	- position		- rotation
	//		uint16	- uint32	- uint32	- 3 x float		- 4 x float
	//  Total = 2 + 4 + 32 bytes

	Packet p;

	p << (uint8) 32; // data size
	
	p << (uint16) E_PKT_GO_MOVE; // id

	p << clientID; // client id

	p << go->getID(); // object id
	
	p << go->getPosition().x;
	p << go->getPosition().y;
	p << go->getPosition().z;

	p << go->getRotation().x;
	p << go->getRotation().y;
	p << go->getRotation().z;
	p << go->getRotation().w;

	p.finalize();

	return p;
}