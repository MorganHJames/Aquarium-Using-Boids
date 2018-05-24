//\===========================================================================================
//\ File: Entity.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for Entity.h.
//\===========================================================================================

#include "Entity.h"

unsigned int Entity::m_suEntityIDCount;
std::map<const unsigned int, Entity*> Entity::s_xEntityList;

Entity::Entity()
	:m_uiEntityID(m_suEntityIDCount)
{
	m_suEntityIDCount++;
	s_xEntityList.insert(std::pair<const unsigned int, Entity*>(m_uiEntityID, this));
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

void Entity::Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO)
{
	for (std::vector<Component*>::iterator it = m_apComponentList.begin(); it != m_apComponentList.end(); ++it)
	{
		(*it)->Draw(a_uiProgramID, a_uiVBO, a_uiIBO);
	}
}

Component* Entity::FindComponentOfType(COMPONENT_TYPE a_eComponentType)
{
	for (std::vector<Component*>::iterator it = m_apComponentList.begin(); it != m_apComponentList.end(); ++it)
	{
		if ((*it)->GetComponentType() == a_eComponentType)
		{
			return *it;
		}
	}
	return NULL;
}

void Entity::RemoveEntity(Entity* a_pEntity)
{
	for (std::map<const unsigned int, Entity*>::iterator it = s_xEntityList.begin(); it != s_xEntityList.end(); it++)
	{
		if ((it->second) == a_pEntity)
		{
			s_xEntityList.erase(it);
			break;
		}
	}
}