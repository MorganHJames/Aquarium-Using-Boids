#include "FishModelComponent.h"
#include <glm/ext.hpp>
#include "aquarium.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Gizmos.h"

typedef Component PARENT;
glm::mat4 temp;
const glm::mat4& GetEntityMatrix() { return temp; }

FishModelComponent::FishModelComponent(Entity* pOwnerEntity) : PARENT(pOwnerEntity)
{
	m_eComponentType = MODEL;
}

FishModelComponent::~FishModelComponent()
{

}

void FishModelComponent::Update(float a_fDeltaTime)
{
	// Apply the delta time
	static float ftime = 0.f;
	ftime += a_fDeltaTime * 0.2f;
}

void FishModelComponent::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	//Get the entity owner
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	//Get transform component
	TransformComponent* pTransformComp = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));
	if (!pTransformComp) return;



	Gizmos::addBox(pTransformComp->GetCurrentPosition(), glm::vec3(1.f, 1.f, 1.f), true, glm::vec4(1.f, 0.0f, 0.f, 1.f), pTransformComp->GetEntityMatrix());
}

