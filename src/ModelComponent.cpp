#include "ModelComponent.h"
#include <glm/ext.hpp>
#include "aquarium.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "BrainComponent.h"
#include "Gizmos.h"

typedef Component PARENT;
glm::mat4 temp;
const glm::mat4& GetEntityMatrix() { return temp; }

ModelComponent::ModelComponent(Entity* pOwnerEntity, MODELTYPE a_modelType, glm::vec4 a_colour, float a_fRadius) : PARENT(pOwnerEntity)
{
	m_eComponentType = MODEL;
	m_model = a_modelType;
	m_colour = a_colour;
	m_fRadius = a_fRadius;
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

	

	if (m_model == SHARK)
	{
		//Get brain component
		BrainComponent* pBrainComp = static_cast<BrainComponent*>(pEntity->FindComponentOfType(BRAIN));
		Gizmos::addCylinder(pTransformComp->GetCurrentPosition(), pBrainComp->m_iLEADERNESS, pBrainComp->m_iLEADERNESS * 0.5f, 10, true,m_colour, pTransformComp->GetEntityMatrix());
	}
	else if (m_model == OBSTACLE)
	{
		Gizmos::addSphere(pTransformComp->GetCurrentPosition(), 10, 10, m_fRadius, m_colour);
	}
	else
	{
		//Get brain component
		BrainComponent* pBrainComp = static_cast<BrainComponent*>(pEntity->FindComponentOfType(BRAIN));
		Gizmos::addBox(pTransformComp->GetCurrentPosition(), glm::vec3(pBrainComp->m_iLEADERNESS, pBrainComp->m_iLEADERNESS, pBrainComp->m_iLEADERNESS), true, m_colour, pTransformComp->GetEntityMatrix());
	}
}

