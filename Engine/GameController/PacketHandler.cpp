
#include "PacketHandler.h"
#include "Macro.h"
#include "GameController.h"
#include "Packets.h"

void PacketHandler::handleMoveAndRotatePacket(Packet p)
{
	try {
		Logger::debug(L"PacketHandler::handleMoveAndRotatePacket start", true);

		p.resetRead();

		uint16 opcodeID;
		uint32 objectID;
		float posX, posY, posZ, rotX, rotY, rotZ;

		p >> opcodeID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ;

		Logger::trace(L"objectID="+toWString(objectID), true);
		Logger::trace(L"posX="+toWString(posX), true);
		Logger::trace(L"posY="+toWString(posY), true);
		Logger::trace(L"posZ="+toWString(posZ), true);
		Logger::trace(L"rotX="+toWString(rotX), true);
		Logger::trace(L"rotY="+toWString(rotY), true);
		Logger::trace(L"rotZ="+toWString(rotZ), true);

		GameController* gc = GameController::getInstance();
		GameObject* go = gc->findObject(objectID);
		std::wstring oldP;
		if(go)
		{
			GHVECTOR pos = go->getPosition();
			oldP = L"old_position X="+toWString(pos.x);
		}

		gc->moveObject(objectID, GHVECTOR(posX, posY, posZ));
		gc->rotateObject(objectID, GHVECTOR(rotX, rotY, rotZ));

		gc->spawnGameObject(GameObjectMasks::OBJ_TYPE_MASK_CREATURE, GHVECTOR(1000,1000,77777));


		// test movement
		go = gc->findObject(objectID);
		if(go)
		{
			GHVECTOR pos = go->getPosition();
			Logger::debug(oldP + L" :: new_position X="+toWString(pos.x), true);
		}

		Logger::debug(L"PacketHandler::handleMoveAndRotatePacket end", true);
	} catch(Exception e) {
		Logger::error(e.getMessage(), true);
	}
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