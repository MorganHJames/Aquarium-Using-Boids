//\===========================================================================================
//\ File: Component.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for Component.h.
//\===========================================================================================

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

void Component::Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO)
{

}