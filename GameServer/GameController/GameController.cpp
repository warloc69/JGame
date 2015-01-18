
#include "GameController.h"
#include "Macro.h"

#include "PacketHandler.h"

static GameController* pGameController = NULL;

GameController::GameController()
{
	pGameObjects = new Accessor<uint32,GameObject>();
}

GameController::~GameController()
{
	SAFE_DELETE_P(pGameObjects);
}

GameController* const GameController::getInstance()
{
	if(!pGameController)
		pGameController = new GameController();

	return pGameController;
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


	//Java_org_jgame_server_core_SimpleServer_sendPacket(
}

void GameController::moveObject(uint32 objectID, GHVECTOR v)
{
	Logger::debug(L"objectID="+objectID, true);

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