
#include "GameController.h"
#include "Macro.h"

#include "PacketHandler.h"
#include "Packets.h"


GameController* GameController::pGameController = NULL;
Accessor<uint32,GameObject>* GameController::pGameObjects = NULL;
int GameController::m_counter = 0;

GameController* const GameController::getInstance()
{
	if(!m_counter)
	{
		pGameController = new GameController();
		pGameObjects = new Accessor<uint32,GameObject>();
	}

	m_counter++;
	return pGameController;
}

void GameController::free()
{
	m_counter--;

	if(!m_counter)
	{
		SAFE_DELETE_P(pGameObjects);

		delete this;
		pGameController = NULL;
	}
}

void GameController::spawnGameObject(GameObjectMasks type, GHVECTOR v)
{
	GameObject* go = new GameObject(type);
	go->move(v);
	pGameObjects->add(go);

	Packet p = PacketHandler::spawnGameObject(go);

	uint8 id;
	uint16 gType;
	float px;
	float py;
	float pz;

	p >> id >> gType >> px >> py >> pz;

	Logger::debug(L"Packet send :: " + toWString(id) + L"," + toWString(gType) + L"," + toWString(px) + L"," + toWString(py) + L"," + toWString(pz));

	/**
		TODO: send packet to Java server here
	*/

	// p >> opcodeID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ;
	//          2          4          4       4       4       4       4       4

	/*uint8* data = new uint8[8];
    data[0] = 0x00;
    data[1] = 0x03;
    data[2] = 0x50;
    data[3] = 0x53;
    data[4] = 0x54;
    data[5] = 0x55;
    data[6] = 0x56;
    data[7] = 0x57;

	std::vector<uint8> vv = ARRAY_TO_VECTOR(data, 10);

	p = Packet(10);
	p << vv;
	p.resetRead();


	uint16 opcodeID;
	p >> opcodeID;

	Logger::debug(L"opcodeID="+toWString(opcodeID), true);

	switch(opcodeID)
	{
		case PACKET_MOVE_GAME_OBJECT:
			PacketHandler::handleMoveAndRotatePacket(p);
			break;

		default:
			break;
	}*/



	//Java_org_jgame_server_core_SimpleServer_sendPacket(
}

void GameController::moveObject(uint32 objectID, GHVECTOR v)
{
	GameObject* go = pGameObjects->getValue(objectID);
	if(!go)
		return;
	go->move(v);
}

void GameController::rotateObject(uint32 objectID, GHVECTOR v)
{
	GameObject* go = pGameObjects->getValue(objectID);
	if(!go)
		return;
	go->rotate(v);
}

GameObject* GameController::findObject(uint32 id)
{
	return pGameObjects->getValue(id);
}

int GameController::getSize()
{
	return pGameObjects->getMap().size();
}

/*JNIEnv* createJVM(JavaVM** jvm) 
{
    JNIEnv *env;
    JavaVMInitArgs vm_args;

    JavaVMOption options; 
    options.optionString = "-Djava.class.path=E:\\_Programming\\_Z&B\\_TW\\JGame\\Server\\src"; 
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = &options;
    vm_args.ignoreUnrecognized = 0;
    
    int ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
    if(ret < 0)
        printf("\nUnable to Launch JVM\n");       
    return env;
}*/