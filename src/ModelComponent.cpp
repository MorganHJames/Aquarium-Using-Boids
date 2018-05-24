//\===========================================================================================
//\ File: ModelComponent.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for ModelComponent.h.
//\===========================================================================================

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

ModelComponent::ModelComponent(Entity* a_pOwnerEntity, MODELTYPE a_eModelType, glm::vec4 a_v4Colour, float a_fRadius) : PARENT(a_pOwnerEntity)
{
	m_eComponentType = MODEL;
	m_eModel = a_eModelType;
	m_v4Colour = a_v4Colour;
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

void ModelComponent::Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO)
{
	//Get the entity owner
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	//Get transform component
	TransformComponent* pTransformComp = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));

	if (!pTransformComp) return;

	

	if (m_eModel == SHARK)
	{
		//Get brain component
		BrainComponent* pBrainComp = static_cast<BrainComponent*>(pEntity->FindComponentOfType(BRAIN));
		Gizmos::addCylinder(pTransformComp->GetCurrentPosition(), (float)pBrainComp->m_iLEADERNESS, pBrainComp->m_iLEADERNESS * 0.5f, 10, true,m_v4Colour, pTransformComp->GetEntityMatrix());
	}
	else if (m_eModel == OBSTACLE)
	{
		Gizmos::addSphere(pTransformComp->GetCurrentPosition(), 10, 10, m_fRadius, m_v4Colour);
	}
	else
	{
		//Get brain component
		BrainComponent* pBrainComp = static_cast<BrainComponent*>(pEntity->FindComponentOfType(BRAIN));
		Gizmos::addBox(pTransformComp->GetCurrentPosition(), glm::vec3(pBrainComp->m_iLEADERNESS, pBrainComp->m_iLEADERNESS, pBrainComp->m_iLEADERNESS), true, m_v4Colour, pTransformComp->GetEntityMatrix());
	}
}

