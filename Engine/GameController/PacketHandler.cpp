
#include "PacketHandler.h"
#include "Macro.h"
#include "GameController.h"
#include "..\Integration\Packets.h"

void PacketHandler::handle(Packet p)
{
	p.resetRead();

	// parse packet ID
	uint16 opcodeID;
	p >> opcodeID;
	printf("Incoming packet :: opcodeID=%d\n", opcodeID);

	// handle packet by ID
	switch(opcodeID)
	{
		case PACKET_MOVE_GAME_OBJECT:
			PacketHandler::handleMoveAndRotatePacket(p);
			break;

		default:
			printf("WARNING :: unhandled packet\n");
			break;
	}
}

void PacketHandler::handleMoveAndRotatePacket(Packet p)
{
	printf("PacketHandler::handleMoveAndRotatePacket start\n");

	// parse packet
	uint32 clientID, objectID;
	float posX, posY, posZ, rotX, rotY, rotZ;
	p >> clientID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ;

	assert(clientID);

	printf("clientID=%d \nobjectID=%d \nposX=%.2f \nposY=%.2f \nposZ=%.2f \nrotX=%.2f \nrotY=%.2f \nrotZ=%.2f \n"
		, clientID, objectID, posX, posY, posZ, rotX, rotY, rotZ);

	// move object
	GameController* gc = GameController::getInstance();
	GameObject* go = gc->findObjectByClient(clientID);
	//assert(go);
	if(go)
	{
		// validation
		if(go->getID() != objectID)
		{
			printf("WARNING! Client's object id is not equal to engine's");
		}

		go->move(GHVECTOR(posX, posY, posZ));
		go->rotate(GHVECTOR(rotX, rotY, rotZ));
	}
	else
	{
		GameObject* go = gc->findObject(objectID);
		if(!go)
		{
			printf("WARNING! Object with %d ID was not found.\n", objectID);
		}
		else
		{
			go->move(GHVECTOR(posX, posY, posZ));
			go->rotate(GHVECTOR(rotX, rotY, rotZ));
		}
	}

	printf("PacketHandler::handleMoveAndRotatePacket end\n");
}

Packet PacketHandler::spawnGameObject(uint32 clientID, GameObject* go)
{
	//	Packet format:
	//		id		- clientID	- objectID	- type		- position		- rotation
	//		uint16	- uint32	- uint32	- uint16	- 3 x float		- 3 x float
	//  Total = 36 bytes

	Packet p;
	
	p << (uint16) PACKET_SPAWN_GAME_OBJECT; // id

	p << clientID; // client id

	p << go->getID(); // object id
	
	p << go->getType(); // spawn object type
	
	p << go->getPosition().x;
	p << go->getPosition().y;
	p << go->getPosition().z;

	p << go->getRotation().x;
	p << go->getRotation().y;
	p << go->getRotation().z;

	p.finalize();

	return p;
}