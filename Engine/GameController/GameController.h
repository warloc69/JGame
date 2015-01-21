
#pragma once

#include "GameObject.h"
#include "Accessor.h"

class GameController
{
	public:
		DLL_EXPORT static GameController* const getInstance();
		DLL_EXPORT void free();

		DLL_EXPORT void spawnGameObject(GameObjectMasks type, GHVECTOR v);
		DLL_EXPORT void moveObject(uint32 objectID, GHVECTOR v);
		DLL_EXPORT void rotateObject(uint32 objectID, GHVECTOR v);

		DLL_EXPORT GameObject* findObject(uint32 id);

		DLL_EXPORT int getSize();

	protected:
		GameController() {}
		virtual ~GameController() {}

	private:
		static Accessor<uint32,GameObject>* pGameObjects;
		static GameController* pGameController;
		static int m_counter;
};