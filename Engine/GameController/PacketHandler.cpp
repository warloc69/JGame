
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
		case CE_PKT_GO_MOVE:
			PacketHandler::handleGameObjectMovePacket(p);
			break;

		case CE_PKT_GO_FIRE:
			PacketHandler::handleGameObjectFirePacket(p);
			break;

		default:
			printf("WARNING :: unhandled packet\n");
			break;
	}
}

void PacketHandler::handleGameObjectMovePacket(Packet p)
{
	printf("PacketHandler::handleGameObjectMovePacket start\n");

	// parse packet
	uint32 clientID, objectID;
	float posX, posY, posZ, rotX, rotY, rotZ, rotW;
	p >> clientID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> rotW;

	assert(objectID);

	printf("clientID=%d \nobjectID=%d \nposX=%.2f \nposY=%.2f \nposZ=%.2f \nrotX=%.2f \nrotY=%.2f \nrotZ=%.2f \nrotW=%.2f \n"
		, clientID, objectID, posX, posY, posZ, rotX, rotY, rotZ, rotW);

	GameController* gc = GameController::getInstance();

	// move object
	GameObject* go = gc->findObject(objectID);
	if(!go)
	{
		printf("WARNING! Object with %d ID was not found.\n", objectID);
	}
	else
	{
		go->move(GHVECTOR(posX, posY, posZ));
		go->rotate(GHVECTOR4(rotX, rotY, rotZ, rotW));
	}

	printf("PacketHandler::handleGameObjectMovePacket end\n");
}

/// Client fires arms in a direction:
/// 1. Calculate spawn position by player position, resource and arms type
/// 2. Spawn internal arms in order to keep on track when damage should be sent to target
/// 3. Notify all clients that arms have been fired
void PacketHandler::handleGameObjectFirePacket(Packet p)
{
	printf("PacketHandler::handleGameObjectFirePacket start\n");

	// parse packet
	uint32 clientID, objectID;
	uint16 armsID;
	float dirX, dirY, dirZ;
	p >> clientID >> objectID >> armsID >> dirX >> dirY >> dirZ;

	assert(objectID);

	printf("clientID=%d \nobjectID=%d \narmsID=%d \ndirX=%.2f \ndirY=%.2f \ndirZ=%.2f \n"
		, clientID, objectID, armsID, dirX, dirY, dirZ);

	GameController* gc = GameController::getInstance();

	/// 1. Calculate spawn position by player position, resource and arms type
	GameObject* player = gc->findObject(objectID);
	if(!player)
		return;
	GHVECTOR pos = player->getPosition();
	GHVECTOR4 rot = player->getRotation();
	uint16 resID = player->getResourceID();
	GHVECTOR armsStartPosition;// = TODO: (pos, resID, armsID);
	GHVECTOR4 armsStartRotation;// = TODO: (rot, resID, armsID);

	/// 2. Spawn internal arms in order to keep on track when damage should be sent to target
	////// - find start velocity by armsID
	float armsStartVelocityValue = 50;
	GHVECTOR4 armsStartVelocity = GHVECTOR4(dirX, dirY, dirZ, armsStartVelocityValue);
	GHVECTOR4 playerVelocity = player->getVelocity();
	GHVECTOR4 totalStartVelocity = GHVector4NormalizeW(armsStartVelocity + playerVelocity);
	////// - spawn internal dynamic object
	uint16 armsResourceID = 0;
	GameObject* armsObject = gc->spawnDynamicObject(armsResourceID, armsStartPosition);
	armsObject->rotate(armsStartRotation);
	armsObject->setVelocity(totalStartVelocity);

	/// 3. Notify all clients that arms have been fired
	////// - define all affected players
	uint32 client1 = 1;
	Packet notify = PacketHandler::gameObjectSpawn(client1, armsObject);
	gc->sendPacketToJavaServer(notify);

	printf("PacketHandler::handleGameObjectFirePacket end\n");
}

Packet PacketHandler::gameObjectSpawn(uint32 clientID, GameObject* go)
{
	//	Packet format:
	//		id		- clientID	- objectID	- type		- position		- rotation
	//		uint16	- uint32	- uint32	- uint16	- 3 x float		- 4 x float
	//  Total = 2 + 4 + 34 bytes

	Packet p;
	
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

	p.finalize();

	return p;
}

Packet PacketHandler::gameObjectMove(uint32 clientID, GameObject* go)
{
	//	Packet format:
	//		id		- clientID	- objectID	- position		- rotation
	//		uint16	- uint32	- uint32	- 3 x float		- 4 x float
	//  Total = 2 + 4 + 32 bytes

	Packet p;
	
	p << (uint16) CE_PKT_GO_MOVE; // id

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