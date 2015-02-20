
#include "GameObject.h"

GameObject::GameObject(uint16 resourceID)
{
	m_guid = 0;
	m_resource_id = resourceID;
	m_position = GHVECTOR();
	m_rotation = GHVECTOR4();
	m_velocity = GHVECTOR4();
}

GameObject::~GameObject()
{
}

uint16 GameObject::getResourceID()
{
	return m_resource_id;
}

void GameObject::setResourceID(uint16 resourceID)
{
	m_resource_id = resourceID;
}

uint32 GameObject::getID()
{
	return m_guid;
}

uint32 GameObject::getClientID()
{
	return m_client_id;
}

void GameObject::setID(uint32 id)
{
	m_guid = id;
}

void GameObject::setClientID(uint32 id)
{
	m_client_id = id;
}

GHVECTOR GameObject::getPosition()
{
	return m_position;
}

GHVECTOR4 GameObject::getRotation()
{
	return m_rotation;
}

void GameObject::move(GHVECTOR v)
{
	m_position = v;
}

void GameObject::rotate(GHVECTOR4 v)
{
	m_rotation = v;
}

GHVECTOR4 GameObject::getVelocity()
{
	return m_velocity;
}

void GameObject::setVelocity(GHVECTOR4 v)
{
	m_velocity = v;
}
