
#pragma once

#include "GameObject.h"
#include <hash_map>

class GameController
{
	public:
		DLL_EXPORT static GameController* const getInstance();
		DLL_EXPORT void free();

		void spawnGameObject(GameObjectMasks type, GHVECTOR v);
		//void moveObject(uint32 objectID, GHVECTOR v);
		//void rotateObject(uint32 objectID, GHVECTOR v);

		DLL_EXPORT bool readQueue();
		DLL_EXPORT void stopQueue();

		GameObject* findObject(uint32 id);

	protected:
		GameController();
		virtual ~GameController() {}

	private:
		uint32 m_read_pos;
		bool m_read_controller;

		void add(GameObject* go);
		uint32 generateKey();

		std::hash_map<uint32,GameObject*> m_gameObjects;
};