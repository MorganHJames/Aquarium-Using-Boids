//\===========================================================================================
//\ File: Entity.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for Entity.h.
//\===========================================================================================

#include "Entity.h"

unsigned int Entity::s_uEntityIDCount;
std::map<const unsigned int, Entity*> Entity::s_xEntityList;

Entity::Entity()
	:m_uEntityID(s_uEntityIDCount)
{
	s_uEntityIDCount++;
	s_xEntityList.insert(std::pair<const unsigned int, Entity*>(m_uEntityID, this));
}

Entity::~Entity()
{
}

void Entity::Update(float a_fDeltaTime)
{
	for (std::vector<Component*>::iterator it = m_apComponentList.begin(); it != m_apComponentList.end(); ++it)
	{
		(*it)->Update(a_fDeltaTime);
	
	}
}

void Entity::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	for (std::vector<Component*>::iterator it = m_apComponentList.begin(); it != m_apComponentList.end(); ++it)
	{
		(*it)->Draw(a_uProgramID, a_uVBO, a_uIBO);
	}
}

Component* Entity::FindComponentOfType(COMPONENT_TYPE eComponentType)
{
	for (std::vector<Component*>::iterator it = m_apComponentList.begin(); it != m_apComponentList.end(); ++it)
	{
		if ((*it)->GetComponentType() == eComponentType)
		{
			return *it;
		}
	}
	return NULL;
}

void Entity::RemoveEntity(Entity* a_entity)
{
	for (std::map<const unsigned int, Entity*>::iterator it = s_xEntityList.begin(); it != s_xEntityList.end(); it++)
	{
		if ((it->second) == a_entity)
		{
			s_xEntityList.erase(it);
			break;
		}
	}
}