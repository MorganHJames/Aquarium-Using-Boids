#include "TransformComponent.h"

typedef Component PARENT;

TransformComponent::TransformComponent(Entity* pOwner, glm::vec3 pos)
	: PARENT(pOwner), m_m4EntityMatrix(glm::mat4())
{
	SetCurrentPosition(pos);
}

TransformComponent::~TransformComponent()
{
}