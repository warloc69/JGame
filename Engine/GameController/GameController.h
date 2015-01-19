
#pragma once

#define DLL_EXPORT   __declspec( dllexport )

#include "GameObject.h"
#include "Accessor.h"

class ServerConsole;

class GameController
{
	public:
		friend ServerConsole;

		static GameController* const getInstance();

		DLL_EXPORT void spawnGameObject(GameObjectMasks type, GHVECTOR v);
		void moveObject(uint32 objectID, GHVECTOR v);
		void rotateObject(uint32 objectID, GHVECTOR v);

	protected:
		DLL_EXPORT GameController();
		DLL_EXPORT ~GameController();

		DLL_EXPORT void Init();

	private:
		Accessor<uint32,GameObject>* pGameObjects;
};
