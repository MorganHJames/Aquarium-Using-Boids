//\===========================================================================================
//\ File: TransformCompnent.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for TransformCompnent.h.
//\===========================================================================================

#include "TransformComponent.h"

typedef Component PARENT;

TransformComponent::TransformComponent(Entity* a_pOwner, glm::vec3 a_v3Pos, std::string a_sName)
	: PARENT(a_pOwner), m_m4EntityMatrix(glm::mat4())
{
	m_sName = a_sName;
	SetCurrentPosition(a_v3Pos);
}

TransformComponent::~TransformComponent()
{
}