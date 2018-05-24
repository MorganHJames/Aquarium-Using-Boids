//\===========================================================================================
//\ File: TransformCompnent.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for TransformCompnent.h.
//\===========================================================================================

#include "TransformComponent.h"

typedef Component PARENT;

TransformComponent::TransformComponent(Entity* pOwner, glm::vec3 pos, std::string a_sName)
	: PARENT(pOwner), m_m4EntityMatrix(glm::mat4())
{
	m_sName = a_sName;
	SetCurrentPosition(pos);
}

TransformComponent::~TransformComponent()
{
}