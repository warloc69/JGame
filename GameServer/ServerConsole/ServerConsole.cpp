
#include "ServerConsole.h"

#pragma comment(lib, "GameController.lib")
#include "..\GameController\GameController.h"
#include "..\GameController\Macro.h"

#include "..\GameController\Packet.h"

int _tmain(int argc, _TCHAR* argv[])
{
// check for memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
// ----------------------

	ServerConsole* sc = new ServerConsole();
	sc->start();
	SAFE_DELETE_P(sc);
		
	return 0;
}

void ServerConsole::start()
{
	printf(" ::: ServerConsole started ::: \n");

	// test
	GameController* gc = new GameController();
	gc->spawnGameObject(GameObjectMasks::OBJ_TYPE_MASK_PLAYER, GHVECTOR(1,1,1));
	gc->spawnGameObject(GameObjectMasks::OBJ_TYPE_MASK_PLAYER, GHVECTOR(1,100,1));
	gc->spawnGameObject(GameObjectMasks::OBJ_TYPE_MASK_PLAYER, GHVECTOR(100,1,1));
	gc->spawnGameObject(GameObjectMasks::OBJ_TYPE_MASK_PLAYER, GHVECTOR(1,1,100));
	//

	std::string msg;
	std::getline(std::cin, msg);
	while(msg != "exit")
	{
		std::getline(std::cin, msg);
	}

	SAFE_DELETE_P(gc);

	printf(" ::: ServerConsole stopped ::: \n");
}

