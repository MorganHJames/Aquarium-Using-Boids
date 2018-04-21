#include "BrainComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Constants.h"

#include "Gizmos.h"

typedef Component PARENT;

//Constants
static const float fMAX_SPEED = 5.5f;
static const float fJITTER = 0.95f;
static const float fWANDER_RADIUS = 5.0f;
static const float fSPHERE_FORWARD_MULTIPLIER = 100.0f;
static const float fCONTAINMENT_TIMER_MAX = 5.0f;

BrainComponent::BrainComponent(Entity* pOwnerEntity)
	: PARENT(pOwnerEntity),
	m_v3CurrentVelocity(0.f,0.f,0.f),
	m_eCurrentBehaviour(WANDER),
	m_fContainmentTimer(0.0f)
{
	m_eComponentType = BRAIN;
}

BrainComponent::~BrainComponent()
{

}

void BrainComponent::Update(float a_fDeltaTime)
{
	//Get the entity owner
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	//Get transform component
	TransformComponent* pTransformComp = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));
	if (!pTransformComp) return;

	//Get forward and pos
	glm::vec3 v3Forward = pTransformComp->GetFacingDirection();
	glm::vec3 v3CurrentPos = pTransformComp->GetCurrentPosition();

	glm::vec3 v3TotalForce(0.0f, 0.0f, 0.0f);

	glm::vec3 v3ContainmentForce = CalculateContainmentForce(v3CurrentPos);

	if (glm::length(v3ContainmentForce) <= 0.0f && m_fContainmentTimer <= 0.0f)
	{
		glm::vec3 v3WanderForce = CalculateWanderForce(v3CurrentPos, v3Forward);
		v3TotalForce = v3WanderForce;
	}
	else
	{
		m_fContainmentTimer -= a_fDeltaTime;
		v3TotalForce = v3ContainmentForce;
	}

	m_v3CurrentVelocity += v3TotalForce;

	//Apply the velocity to position
	v3CurrentPos += m_v3CurrentVelocity * a_fDeltaTime;

	if (glm::length(m_v3CurrentVelocity) > 0.0f)
	{
		v3Forward = glm::normalize(m_v3CurrentVelocity);
	}

	//Initialize target vector
	glm::vec3 v3NewTarget = { 0.f,0.f,0.f };

	glm::vec3 up = pTransformComp->GetUpDirection();
	glm::vec3 right = glm::cross(up, v3Forward);

	pTransformComp->SetRightDirection(right);
	pTransformComp->SetFacingDirection(v3Forward);
	pTransformComp->SetUpDirection(up);
	pTransformComp->SetCurrentPosition(v3CurrentPos);
}

void BrainComponent::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{

}


glm::vec3 BrainComponent::CalculateSeekForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const
{
	//Apply force
	glm::vec3 v3TargetDir = glm::normalize(v3Target - v3CurrentPos);
	glm::vec3 v3NewVelocity = v3TargetDir * fMAX_SPEED;
	glm::vec3 v3Force = v3NewVelocity - m_v3CurrentVelocity;

	//Add gizmos
	Gizmos::addCircle(v3Target, 2, 16, false, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));
	Gizmos::addLine(v3CurrentPos, v3Target, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));

	return v3Force;
}

glm::vec3 BrainComponent::CalculateEvadeForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const
{
	//Apply force
	glm::vec3 v3TargetDir = glm::normalize(v3CurrentPos - v3Target);
	glm::vec3 v3NewVelocity = v3TargetDir * fMAX_SPEED;
	glm::vec3 v3Force = v3NewVelocity - m_v3CurrentVelocity;

	//Add gizmos
	Gizmos::addCircle(v3Target, 2, 16, false, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));
	Gizmos::addLine(v3CurrentPos, v3Target, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));

	return v3Force;
}

glm::vec3 BrainComponent::CalculateWanderForce(const glm::vec3& v3CurrentPos, const glm::vec3& v3Forward)
{
	glm::vec3 v3WanderSphereOrigin = v3CurrentPos + (v3Forward * fSPHERE_FORWARD_MULTIPLIER);
	glm::vec3 v3RandPoint = glm::sphericalRand(fWANDER_RADIUS);
	glm::vec3 v3RandPointInSphere = v3WanderSphereOrigin + v3RandPoint;

	//Use sphere rand to gen a random vector with magnitute of jitter.
	v3RandPoint = glm::sphericalRand(fJITTER);

	//Find a random in the sphere
	glm::vec3 v3RandTargetVec = v3RandPointInSphere + v3RandPoint;

	//Set the target.
	glm::vec3 v3Target = v3WanderSphereOrigin + (glm::normalize(v3RandTargetVec - v3WanderSphereOrigin) * fWANDER_RADIUS);

	
	//Apply force eqquation
	glm::vec3 v3TargetDir = glm::normalize(v3Target - v3CurrentPos);
	glm::vec3 v3NewVelocity = v3TargetDir * fMAX_SPEED;
	glm::vec3 v3Force = v3NewVelocity - m_v3CurrentVelocity;

	//Gizmos
	Gizmos::addLine(v3CurrentPos, v3WanderSphereOrigin, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Gizmos::addLine(v3WanderSphereOrigin, v3RandPointInSphere, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Gizmos::addLine(v3RandPointInSphere, v3RandTargetVec, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	Gizmos::addLine(v3WanderSphereOrigin, v3Target, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Gizmos::addCircle(v3WanderSphereOrigin, fWANDER_RADIUS, 16, false, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));

	return v3Force;
}

glm::vec3 BrainComponent::CalculateContainmentForce(const glm::vec3& v3CurrentPos)
{
	glm::vec3 v3ContainmentForce(0.0f, 0.0f, 0.0f);

	if (v3CurrentPos.x > g_fAQUARIUM_SIZE
		|| v3CurrentPos.x < -g_fAQUARIUM_SIZE
		|| v3CurrentPos.z > g_fAQUARIUM_SIZE
		|| v3CurrentPos.z < -g_fAQUARIUM_SIZE)
	{
		v3ContainmentForce = CalculateSeekForce(glm::vec3(0.0f, 0.0f, 0.0f), v3CurrentPos);
		m_fContainmentTimer = fCONTAINMENT_TIMER_MAX;
	}

	return v3ContainmentForce;
}


