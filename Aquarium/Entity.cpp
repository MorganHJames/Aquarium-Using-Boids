#include "Entity.h"

Entity::Entity()
{

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
