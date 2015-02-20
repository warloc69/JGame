
#pragma once

#include "GameObject.h"

class DynamicObject : public GameObject
{
	public:
		DynamicObject(uint16 resourceID) : GameObject(resourceID)
		{
			m_life_time = 5.0f;
		}

		float getLifeTime()
		{
			return m_life_time;
		}

		void update(float dt)
		{
			m_life_time -= dt;
		}

		void setLifeTime(float t)
		{
			m_life_time = t;
		}

	private:
		float m_life_time;
};