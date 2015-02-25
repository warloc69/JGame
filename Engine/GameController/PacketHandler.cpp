
#include "PacketHandler.h"
#include "Macro.h"
#include "GameController.h"
#include "Packets.h"

void PacketHandler::handle(Packet& p)
{
	/// read opcode id
	uint16 opcodeID;
	p >> opcodeID;

	printf("Incoming packet :: opcodeID=%d\n", opcodeID);

	switch(opcodeID)
	{
		case C_PKT_AUTH_REQUEST:
			//m_authQueue.push(p);
			break;

		case C_PKT_GO_MOVE:
			handleGameObjectMovePacket(p);
			break;

		case C_PKT_GO_FIRE:
			handleGameObjectFirePacket(p);
			break;

		case C_PKT_GO_SPAWN:
			handleGameObjectSpawnPacket(p);
			break;

		default:
			printf("WARNING :: unhandled packet\n");
			break;
	}
}

void PacketHandler::handleGameObjectMovePacket(Packet& p)
{
	printf("PacketHandler::handleGameObjectMovePacket start\n");

	// parse packet
	uint32 clientID;
	float posX, posY, posZ, rotX, rotY, rotZ, rotW;
	p >> clientID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> rotW;

	assert(clientID);

	printf("clientID=%d \nposX=%.2f \nposY=%.2f \nposZ=%.2f \nrotX=%.2f \nrotY=%.2f \nrotZ=%.2f \nrotW=%.2f \n"
		, clientID, posX, posY, posZ, rotX, rotY, rotZ, rotW);

	GameController* gc = GameController::getInstance();

	// move object
	GameObject* go = gc->findObjectByClient(clientID);
	if(!go)
	{
		printf("WARNING! Object for %d client was not found.\n", clientID);
	}
	else
	{
		go->move(GHVECTOR(posX, posY, posZ));
		go->rotate(GHVECTOR4(rotX, rotY, rotZ, rotW));
	}

	printf("PacketHandler::handleGameObjectMovePacket end\n");
}

void PacketHandler::handleGameObjectSpawnPacket(Packet& p)
{
	printf("PacketHandler::handleGameObjectSpawnPacket start\n");

	// parse packet
	uint32 clientID;
	uint16 resourceID;
	float posX, posY, posZ, rotX, rotY, rotZ, rotW, velX, velY, velZ;
	p >> clientID >> resourceID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> rotW >> velX >> velY >> velZ;

	assert(clientID);

	printf("clientID=%d \nposX=%.2f \nposY=%.2f \nposZ=%.2f \nrotX=%.2f \nrotY=%.2f \nrotZ=%.2f \nrotW=%.2f \nvelX=%.2f \nvelY=%.2f \nvelZ=%.2f \n"
		, clientID, posX, posY, posZ, rotX, rotY, rotZ, rotW, velX, velY, velZ);

	GameController* gc = GameController::getInstance();
	if(gc->findObjectByClient(clientID))
	{
		printf("There is already spawned object for client=%d", clientID);
		return;
	}

	// spawn object
	GameObject* go = gc->spawnGameObject(resourceID, GHVECTOR(posX, posY, posZ));
	go->setClientID(clientID);
	go->rotate(GHVECTOR4(rotX, rotY, rotZ, rotW));
	go->setVelocity(GHVECTOR(velX, velY, velZ));

	// notify clients
	//gc->sendPacketToJavaServer(PacketHandler::gameObjectSpawn(clientID, go));

	printf("PacketHandler::handleGameObjectSpawnPacket end\n");
}

/// Client fires arms in a direction:
/// 1. Calculate spawn position by player position, resource and arms type
/// 2. Spawn internal arms in order to keep on track when damage should be sent to target
/// 3. Notify all clients that arms have been fired
void PacketHandler::handleGameObjectFirePacket(Packet& p)
{
	printf("PacketHandler::handleGameObjectFirePacket start\n");

	// parse packet
	uint32 clientID;
	uint16 armsID;
	float dirX, dirY, dirZ;
	p >> clientID >> armsID >> dirX >> dirY >> dirZ;

	assert(clientID);

	printf("clientID=%d \narmsID=%d \ndirX=%.2f \ndirY=%.2f \ndirZ=%.2f \n"
		, clientID, armsID, dirX, dirY, dirZ);

	GameController* gc = GameController::getInstance();

	/// 1. Calculate spawn position by player position, resource and arms type
	GameObject* player = gc->findObjectByClient(clientID);
	if(!player)
		return;
	GHVECTOR pos = player->getPosition();
//	GHVECTOR4 rot = player->getRotation();
	GHVECTOR dir = GHVECTOR(dirX, dirY, dirZ);
	uint16 resID = player->getResourceID();
	GHVECTOR armsStartPosition = pos + dir * 10;// = TODO: (pos, resID, armsID);
	GHVECTOR4 armsStartRotation = GHVECTOR4(0,0,0,1);// = TODO: (rot, resID, armsID);

	/// 2. Spawn internal arms in order to keep on track when damage should be sent to target
	////// - find start velocity by armsID
	float armsStartVelocityValue = 50;
	GHVECTOR armsStartVelocity = GHVECTOR(dirX, dirY, dirZ) * armsStartVelocityValue;
	GHVECTOR playerVelocity = player->getVelocity();
	GHVECTOR totalStartVelocity = armsStartVelocity + playerVelocity;
	////// - spawn internal dynamic object
	uint16 armsResourceID = 2;
	GameObject* armsObject = gc->spawnDynamicObject(armsResourceID, armsStartPosition);
	armsObject->rotate(armsStartRotation);
	armsObject->setVelocity(totalStartVelocity);

	/// 3. Notify all clients that arms have been fired
	////// - TODO: define all affected players
	Packet notify = PacketHandler::gameObjectSpawn(clientID, armsObject);
	//gc->sendPacketToJavaServer(notify);

	printf("PacketHandler::handleGameObjectFirePacket end\n");
}

Packet PacketHandler::gameObjectSpawn(uint32 clientID, GameObject* go)
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

Packet PacketHandler::gameObjectMove(uint32 clientID, GameObject* go)
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