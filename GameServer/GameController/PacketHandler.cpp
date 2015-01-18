
#include "PacketHandler.h"
#include "Macro.h"
#include "GameController.h"
#include "Packets.h"

void PacketHandler::handleMoveAndRotatePacket(Packet p)
{
	Logger::debug(L"PacketHandler::handleMoveAndRotatePacket start");

	p.resetRead();

	uint16 opcodeID;
	uint32 objectID;
	float posX, posY, posZ, rotX, rotY, rotZ;

	p >> opcodeID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ;

	GameController* gc = GameController::getInstance();
	gc->moveObject(objectID, GHVECTOR(posX, posY, posZ));
	gc->rotateObject(objectID, GHVECTOR(rotX, rotY, rotZ));

	Logger::debug(L"PacketHandler::handleMoveAndRotatePacket end");
}

Packet PacketHandler::spawnGameObject(GameObject* go)
{
	//	Packet format:
	//		id		- type		- position		- rotation
	//		uint16	- uint16	- 3 x float		- 3 x float
	//  Total = 12 bytes

	Packet p = Packet(0xd8);
	
	p << (uint16) PACKET_SPAWN_GAME_OBJECT;
	
	p << go->getType();
	
	p << go->getPosition().x;
	p << go->getPosition().y;
	p << go->getPosition().z;

	p << go->getRotation().x;
	p << go->getRotation().y;
	p << go->getRotation().z;

	return p;
}