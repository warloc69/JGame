
#pragma once

#include "Vectors.h"

class GameObject
{
	public:
		GameObject(uint16 resourceID);
		virtual ~GameObject();

		DLL_EXPORT uint32 getID();
		DLL_EXPORT uint32 getClientID();
		DLL_EXPORT uint16 getResourceID();
		void setID(uint32 id);
		void setClientID(uint32 id);
		void setResourceID(uint16 resourceID);

		DLL_EXPORT GHVECTOR getPosition();
		DLL_EXPORT GHVECTOR4 getRotation();
		DLL_EXPORT GHVECTOR getVelocity();

		DLL_EXPORT void move(GHVECTOR v);
		DLL_EXPORT void rotate(GHVECTOR4 v);
		DLL_EXPORT void setVelocity(GHVECTOR v);

	protected:
		uint32 m_guid;
		uint32 m_client_id;
		uint16 m_resource_id;

		GHVECTOR m_position;
		GHVECTOR4 m_rotation;
		GHVECTOR m_velocity;
};