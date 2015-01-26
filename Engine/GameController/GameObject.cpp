
#include "GameObject.h"

GameObject::GameObject(GameObjectMasks type)
{
	m_guid = 0;
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

