#include "ModelComponent.h"
#include <glm/ext.hpp>
#include "aquarium.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Gizmos.h"

typedef Component PARENT;
glm::mat4 temp;
const glm::mat4& GetEntityMatrix() { return temp; }

ModelComponent::ModelComponent(Entity* pOwnerEntity, float a_size, float a_radius, float a_length, glm::vec4 a_colour) : PARENT(pOwnerEntity)
{
	m_eComponentType = MODEL;
	m_size = a_size;
	m_radius = a_radius;
	m_length = a_length;
	m_colour = a_colour;
}

ModelComponent::~ModelComponent()
{

}

void ModelComponent::Update(float a_fDeltaTime)
{
	// Apply the delta time
	static float ftime = 0.f;
	ftime += a_fDeltaTime * 0.2f;
}

void ModelComponent::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	//Get the entity owner
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	//Get transform component
	TransformComponent* pTransformComp = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));
	if (!pTransformComp) return;

	if (m_length > 0)
	{
		m_model = SHARK;
		Gizmos::addCylinder(pTransformComp->GetCurrentPosition(), m_radius, m_length * 0.5f, 10, true,m_colour, pTransformComp->GetEntityMatrix());
	}
	else if (m_radius > 0)
	{
		m_model = OBSTACLE;
		Gizmos::addSphere(pTransformComp->GetCurrentPosition(), 10, 10, m_radius, m_colour);
	}
	else
	{
		m_model = FISH;
		Gizmos::addBox(pTransformComp->GetCurrentPosition(), glm::vec3(m_size, m_size, m_size), true, m_colour, pTransformComp->GetEntityMatrix());
	}
}

