
#pragma once

#include "Vectors.h"

enum GameObjectMasks
{
	OBJ_TYPE_MASK_PLAYER		= 0x0001,
	OBJ_TYPE_MASK_CREATURE		= 0x0002,
	OBJ_TYPE_MASK_GAMEOBJECT	= 0x0004,
	OBJ_TYPE_MASK_UNIT			= OBJ_TYPE_MASK_PLAYER | OBJ_TYPE_MASK_CREATURE,
	OBJ_TYPE_MASK_OBJECT_3D		= OBJ_TYPE_MASK_UNIT | OBJ_TYPE_MASK_GAMEOBJECT,
};

class GameObject
{
	public:
		GameObject(GameObjectMasks type);
		~GameObject();

		uint32 getID();
		void setID(uint32 id);
		uint16 getType();
		uint64 getGUID();

		GHVECTOR getPosition();
		GHVECTOR getRotation();

		void move(GHVECTOR v);
		void rotate(GHVECTOR v);

	private:
		uint64 m_guid;
		void setType(uint16 type);

		GHVECTOR m_position;
		GHVECTOR m_rotation;
};