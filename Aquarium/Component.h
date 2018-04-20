#ifndef _COMPONENT_H
#define _COMPONENT_H

enum COMPONENT_TYPE
{
	TRANSFORM,
	MODEL,
	BRAIN,
};

class Entity;

class Component
{
protected:
	COMPONENT_TYPE m_eComponentType;
	Entity* m_pOwnerEntity;
public:
	Component(Entity* a_pOwnerEntity);
	~Component();

	inline COMPONENT_TYPE GetComponentType() { return m_eComponentType; }

	Entity* GetOwnerEntity() { return m_pOwnerEntity; }
	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);

};

#endif//_COMPONENT_H