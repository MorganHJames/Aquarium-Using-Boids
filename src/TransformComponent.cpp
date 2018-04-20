#include "TransformComponent.h"

typedef Component PARENT;

TransformComponent::TransformComponent(Entity* pOwner)
	: PARENT(pOwner), m_m4EntityMatrix(glm::mat4())
{
}

TransformComponent::~TransformComponent()
{
}