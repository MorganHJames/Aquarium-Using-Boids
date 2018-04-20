#include "Component.h"

Component::Component(Entity* a_pOwnerEntity)
{
	m_pOwnerEntity = a_pOwnerEntity;
}

Component::~Component()
{

}

void Component::Update(float a_fDeltaTime)
{

}

void Component::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{

}