
#include "GameObject.h"
#include "Accessor.h"

GameObject::GameObject(GameObjectMasks type)
{
	setType(type);
	m_position = GHVECTOR();
}

GameObject::~GameObject()
{
}

uint16 GameObject::getType()
{
	return (uint16) (m_guid & 0xFFFF);
}

void GameObject::setType(uint16 type)
{
	m_guid = m_guid | type;
}

uint32 GameObject::getID()
{
	return (uint32) m_guid >> 16;
}

void GameObject::setID(uint32 id)
{
	m_guid = m_guid | (id << 16);
}

uint64 GameObject::getGUID()
{
	return m_guid;
}

GHVECTOR GameObject::getPosition()
{
	return m_position;
}

GHVECTOR GameObject::getRotation()
{
	return m_rotation;
}

void GameObject::move(GHVECTOR v)
{
	m_position = v;
}

void GameObject::rotate(GHVECTOR v)
{
	m_rotation = v;
}

uint32 Accessor<uint32,GameObject>::generateKey()
{
	uint32 i = 1;
	std::hash_map<K,C*>::iterator itr = m_values.begin();
	for(; itr != m_values.end(); itr++)
	{
		if(itr->first == i)
		{
			i++;
			continue;
		}
	}

	return i;
}

void Accessor<uint32,GameObject>::add(GameObject* o)
{
	if(o)
	{
		uint32 id = generateKey();
		o->setID(id);
		m_values.insert(std::pair<uint32,GameObject*>(id, o));
	}
}