#ifndef _ENTITY_H
#define _ENTITY_H

// Includes
#include <vector>
#include <map>
#include "Component.h"

class Entity
{
private:
	unsigned int m_uEntityID;
	

	static unsigned int s_uEntityIDCount;
	static std::map<const unsigned int, Entity*> s_xEntityList;

protected:
	std::vector<Component*>m_apComponentList;


public:
	Entity();
	~Entity();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);

	void AddComponent(Component* a_pComponent) { m_apComponentList.push_back(a_pComponent); }
	Component* FindComponentOfType(COMPONENT_TYPE eComponentType);

	inline static unsigned int GetEntityID() { return s_uEntityIDCount; }

};

#endif // _ENTITY_H