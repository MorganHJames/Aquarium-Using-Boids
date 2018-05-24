//\===========================================================================================
//\ File: Entity.h
//\ Author: Morgan James
//\ Brief: The base class for all entities.
//\===========================================================================================

#ifndef _ENTITY_H
#define _ENTITY_H

//Includes
#include <vector>
#include <map>
#include "Component.h"

class Entity
{
private:
	const unsigned int m_uiEntityID;
	static unsigned int m_suEntityIDCount;
	static std::map<const unsigned int, Entity*> s_xEntityList;

protected:
	std::vector<Component*>m_apComponentList;

public:
	Entity();
	~Entity();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO);

	void AddComponent(Component* a_pComponent) { m_apComponentList.push_back(a_pComponent); }
	Component* FindComponentOfType(COMPONENT_TYPE a_eComponentType);

	void RemoveEntity(Entity* a_pEntity);

	inline const unsigned int GetEntityID() { return m_uiEntityID; }
	inline static const std::map<const unsigned int, Entity*> GetEntityList() { return s_xEntityList; }
};

#endif//_ENTITY_H