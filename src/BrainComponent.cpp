#include "BrainComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "Entity.h"
#include "Constants.h"
#include "Gizmos.h"

typedef Component PARENT;

BrainComponent::BrainComponent(Entity* pOwnerEntity, int a_iLeaderness)
	: PARENT(pOwnerEntity),
	m_v3CurrentVelocity(0.f,0.f,0.f),
	m_eCurrentBehaviour(WANDER)
{
	m_iLEADERNESS = a_iLeaderness;
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
	glm::vec3 v3Up = pTransformComp->GetUpDirection();


	//gram schmist orthonomalisation process
	v3Up = v3Up - (v3Forward * glm::dot(v3Forward, v3Up));
	v3Up = glm::normalize(v3Up);
	glm::vec3 v3Right = glm::cross(v3Up, v3Forward);
	v3Right = glm::normalize(v3Right);
	pTransformComp->SetUpDirection(v3Up);
	pTransformComp->SetRightDirection(v3Right);

	glm::vec3 v3CurrentPos = pTransformComp->GetCurrentPosition();

	AdaptColour(a_fDeltaTime);

	//Behavior Force Calculation
	glm::vec3 v3TotalForce(0.0f, 0.0f, 0.0f);

	glm::vec3 v3ContainmentForce = CalculateContainmentForce(v3CurrentPos);
	glm::vec3 v3WanderForce = CalculateWanderForce(v3CurrentPos, v3Forward);
	glm::vec3 v3InstinctiveForce = CalculateInstinctForce(v3CurrentPos);
	glm::vec3 v3SeperationForce = CalculateSeperationForce();
	glm::vec3 v3AlignmentForce = CalculateAlignmentForce();
	glm::vec3 v3CohesionForce = CalculateCohesionForce();
	glm::vec3 v3CollisionAvoidanceForce = CalculateCollisionAvoidanceForce();

	v3TotalForce = 
		(v3WanderForce * m_fWanderForce)
		+ (v3InstinctiveForce * m_fInstinctiveForce)
		+ (v3SeperationForce * m_fSeperationForce)
		+ (v3AlignmentForce * m_fAlignmentForce)
		+ (v3CohesionForce * m_fCohesionForce)
		+ (v3CollisionAvoidanceForce * m_fCollisionAvoidanceForce)
		+ (v3ContainmentForce * m_fContainmentForce);

	///////////////////////////////////////////////////
	
	m_v3CurrentVelocity += v3TotalForce;

	//Clamp velocity.
	m_v3CurrentVelocity = glm::clamp(m_v3CurrentVelocity, glm::vec3(-10.0f, -10.0f,-10.0f), glm::vec3(10.0f, 10.0f, 10.0f));

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
	glm::vec3 v3NewVelocity = v3TargetDir * m_fMAX_SPEED;
	glm::vec3 v3Force = v3NewVelocity - m_v3CurrentVelocity;

	//Add gizmos
	Gizmos::addCircle(v3Target, 2, 16, false, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));
	Gizmos::addLine(v3CurrentPos, v3Target, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));

	return v3Force;
}

glm::vec3 BrainComponent::CalculatePursuitForce(Entity* a_target, const glm::vec3& v3CurrentPos) const
{
	TransformComponent* aTransform = static_cast<TransformComponent*>(a_target->FindComponentOfType(TRANSFORM));
	BrainComponent* aBrain = static_cast<BrainComponent*>(a_target->FindComponentOfType(BRAIN));
	glm::vec3 distance = aTransform->GetCurrentPosition() - v3CurrentPos;
	float updatesAhead = distance.length() / m_fMAX_SPEED;
	glm::vec3 futurePosition = aTransform->GetCurrentPosition() + aBrain->GetCurrentVelocity() * updatesAhead;
	return CalculateSeekForce(futurePosition, v3CurrentPos);
}

glm::vec3 BrainComponent::CalculateFleeForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const
{
	//Apply force
	glm::vec3 v3TargetDir = glm::normalize(v3CurrentPos - v3Target);
	glm::vec3 v3NewVelocity = v3TargetDir * m_fMAX_SPEED;
	glm::vec3 v3Force = v3NewVelocity - m_v3CurrentVelocity;

	//Add gizmos
	Gizmos::addCircle(v3Target, 2, 16, false, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));
	Gizmos::addLine(v3CurrentPos, v3Target, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));

	return v3Force;
}

glm::vec3 BrainComponent::CalculateEvadeForce(Entity* a_target, const glm::vec3& v3CurrentPos) const
{
	TransformComponent* aTransform = static_cast<TransformComponent*>(a_target->FindComponentOfType(TRANSFORM));
	BrainComponent* aBrain = static_cast<BrainComponent*>(a_target->FindComponentOfType(BRAIN));
	glm::vec3 distance = aTransform->GetCurrentPosition() - v3CurrentPos;
	float updatesAhead = distance.length() / m_fMAX_SPEED;
	glm::vec3 futurePosition = aTransform->GetCurrentPosition() + aBrain->GetCurrentVelocity() * updatesAhead; 
	return CalculateFleeForce(futurePosition, v3CurrentPos);
}

glm::vec3 BrainComponent::CalculateWanderForce(const glm::vec3& v3CurrentPos, const glm::vec3& v3Forward)
{
	glm::vec3 v3WanderSphereOrigin = v3CurrentPos + (v3Forward * m_fSPHERE_FORWARD_MULTIPLIER);
	glm::vec3 v3RandPoint = glm::sphericalRand(m_fWANDER_RADIUS);
	glm::vec3 v3RandPointInSphere = v3WanderSphereOrigin + v3RandPoint;

	//Use sphere rand to gen a random vector with magnitute of jitter.
	v3RandPoint = glm::sphericalRand(m_fJITTER);

	//Find a random in the sphere
	glm::vec3 v3RandTargetVec = v3RandPointInSphere + v3RandPoint;

	//Set the target.
	glm::vec3 v3Target = v3WanderSphereOrigin + (glm::normalize(v3RandTargetVec - v3WanderSphereOrigin) * m_fWANDER_RADIUS);

	
	//Apply force eqquation
	glm::vec3 v3TargetDir = glm::normalize(v3Target - v3CurrentPos);
	glm::vec3 v3NewVelocity = v3TargetDir * m_fMAX_SPEED;
	glm::vec3 v3Force = v3NewVelocity - m_v3CurrentVelocity;

	//Gizmos
	Gizmos::addLine(v3CurrentPos, v3WanderSphereOrigin, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Gizmos::addLine(v3WanderSphereOrigin, v3RandPointInSphere, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Gizmos::addLine(v3RandPointInSphere, v3RandTargetVec, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	Gizmos::addLine(v3WanderSphereOrigin, v3Target, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Gizmos::addCircle(v3WanderSphereOrigin, m_fWANDER_RADIUS, 16, false, glm::vec4(0.f, 0.8f, 0.3f, 1.0f));

	return v3Force;
}

glm::vec3 BrainComponent::CalculateInstinctForce(glm::vec3& v3CurrentPos)
{
	glm::vec3 v3InstinctForce(0.0f, 0.0f, 0.0f);
	//If shark and fish is close pursue fish

	if (GetOwnerEntity())
	{
		const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
		std::map<const unsigned int, Entity*>::const_iterator xIter;

		Entity* pClosest = nullptr;

		for (xIter = xEntityMap.begin(); xIter != xEntityMap.end(); ++xIter)
		{
			Entity* pTarget = xIter->second;
			//not same and not obstacle
			if (pTarget && pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID() && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model != static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model != OBSTACLE)
			{
				TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
				
				if (pTargetTransform)
				{
					if (pClosest != nullptr)
					{
						TransformComponent* pClosestTargetTransform = static_cast<TransformComponent*>(pClosest->FindComponentOfType(TRANSFORM));
						glm::vec3 v3TargetPos = pTargetTransform->GetCurrentPosition();
						float fDistanceBetween = glm::length(v3CurrentPos - v3TargetPos);
						float fDistanceBetweenClosest = glm::length(v3CurrentPos - pClosestTargetTransform->GetCurrentPosition());

						if (fDistanceBetween < fDistanceBetweenClosest)
						{
							pClosest = pTarget;
						}
					}
					else
					{
						pClosest = pTarget;
					}
				}
			}
		}

		if (pClosest != nullptr)
		{
			TransformComponent* pClosestTargetTransform = static_cast<TransformComponent*>(pClosest->FindComponentOfType(TRANSFORM));
			if (pClosestTargetTransform)
			{
				float fDistanceBetweenClosest = glm::length(v3CurrentPos - pClosestTargetTransform->GetCurrentPosition());
				if (fDistanceBetweenClosest < m_fINSTINCT_RANGE)
				{
					if (static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model == FISH)
					{
						v3InstinctForce += CalculateEvadeForce(pClosest, v3CurrentPos);

						if (fDistanceBetweenClosest < 1)
						{
							v3CurrentPos.x = glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE);
							v3CurrentPos.y = glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE);
							v3CurrentPos.z = glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE);
						}
					}
					if (static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model == SHARK)
					{
						v3InstinctForce += CalculatePursuitForce(pClosest, v3CurrentPos);
					}
				}
			}
		}
	}
	return v3InstinctForce;
}

glm::vec3 BrainComponent::CalculateContainmentForce(const glm::vec3& v3CurrentPos)
{
	glm::vec3 v3ContainmentForce(0.0f, 0.0f, 0.0f);

	//right wall
	if (glm::distance(v3CurrentPos, glm::vec3(g_fHALF_AQUARIUM_SIZE, v3CurrentPos.y, v3CurrentPos.z)) < m_fWall_CHECK_DISTANCE)
	{
		v3ContainmentForce += CalculateFleeForce(glm::vec3(g_fHALF_AQUARIUM_SIZE, v3CurrentPos.y, v3CurrentPos.z), v3CurrentPos);
	}

	//left wall
	if (glm::distance(v3CurrentPos, glm::vec3(-g_fAQUARIUM_SIZE, v3CurrentPos.y, v3CurrentPos.z)) < m_fWall_CHECK_DISTANCE)
	{
		v3ContainmentForce += CalculateFleeForce(glm::vec3(-g_fAQUARIUM_SIZE, v3CurrentPos.y, v3CurrentPos.z), v3CurrentPos);
	}

	//top wall
	if (glm::distance(v3CurrentPos, glm::vec3(v3CurrentPos.x, g_fAQUARIUM_SIZE, v3CurrentPos.z)) < m_fWall_CHECK_DISTANCE)
	{
		v3ContainmentForce += CalculateFleeForce(glm::vec3(v3CurrentPos.x, g_fAQUARIUM_SIZE, v3CurrentPos.z), v3CurrentPos);
	}

	//bottom wall
	if (glm::distance(v3CurrentPos, glm::vec3(v3CurrentPos.x, -g_fAQUARIUM_SIZE, v3CurrentPos.z)) < m_fWall_CHECK_DISTANCE)
	{
		v3ContainmentForce += CalculateFleeForce(glm::vec3(v3CurrentPos.x, -g_fAQUARIUM_SIZE, v3CurrentPos.z), v3CurrentPos);
	}

	//front wall
	if (glm::distance(v3CurrentPos, glm::vec3(v3CurrentPos.x, v3CurrentPos.y, g_fAQUARIUM_SIZE)) < m_fWall_CHECK_DISTANCE)
	{
		v3ContainmentForce += CalculateFleeForce(glm::vec3(v3CurrentPos.x, v3CurrentPos.y, g_fAQUARIUM_SIZE), v3CurrentPos);
	}

	//back wall
	if (glm::distance(v3CurrentPos, glm::vec3(v3CurrentPos.x, v3CurrentPos.y, -g_fAQUARIUM_SIZE)) < m_fWall_CHECK_DISTANCE)
	{
		v3ContainmentForce += CalculateFleeForce(glm::vec3(v3CurrentPos.x, v3CurrentPos.y, -g_fAQUARIUM_SIZE), v3CurrentPos);
	}

	return v3ContainmentForce;
}

glm::vec3 BrainComponent::CalculateSeperationForce()
{
	glm::vec3 v3SeparationForce(0.0f, 0.0f, 0.0f);	

	if (GetOwnerEntity())
	{
		TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
		if (pLocalTransform)
		{
			glm::vec3 v3LocalPos = pLocalTransform->GetCurrentPosition();

			const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
			std::map<const unsigned int, Entity*>::const_iterator xIter;

			for (xIter = xEntityMap.begin(); xIter != xEntityMap.end(); ++xIter)
			{
				Entity* pTarget = xIter->second;
				if (pTarget && pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID() && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model == static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model)
				{
					TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
					if (pTargetTransform)
					{
						glm::vec3 v3TargetPos = pTargetTransform->GetCurrentPosition();
						float fDistanceBetween = glm::length(v3LocalPos - v3TargetPos);
						if (fDistanceBetween < m_fNEIGHBOURHOOD_RADIUS)
						{
							v3SeparationForce += CalculateFleeForce(v3TargetPos, v3LocalPos);
							if (glm::length(v3SeparationForce) > 0.0f)
							{
								v3SeparationForce = glm::normalize(v3SeparationForce);
							}
						}
					}
				}
			}
		}
	}
	return v3SeparationForce;
}

glm::vec3 BrainComponent::CalculateAlignmentForce()
{
	glm::vec3 v3AverageVelocity(0.0f, 0.0f, 0.0f);
	glm::vec3 v3AlignmentForce(0.0f, 0.0f, 0.0f);
	unsigned int  uNeighbourCount = 0;

	if (GetOwnerEntity())
	{
		TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
		if (pLocalTransform)
		{
			glm::vec3 v3LocalPos = pLocalTransform->GetCurrentPosition();

			const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
			std::map<const unsigned int, Entity*>::const_iterator xIter;

			for (xIter = xEntityMap.begin(); xIter != xEntityMap.end(); ++xIter)
			{
				Entity* pTarget = xIter->second;
				if (pTarget && pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID() && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model == static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model)
				{
					TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
					BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));

					if (pTargetTransform && pTargetBrain)
					{
						glm::vec3 v3TargetPos = pTargetTransform->GetCurrentPosition();
						float fDistanceBetween = glm::length(v3LocalPos - v3TargetPos);
						if (fDistanceBetween < m_fNEIGHBOURHOOD_RADIUS)
						{
							for (int i = 0; i < pTargetBrain->m_iLEADERNESS; ++i)
							{
								v3AverageVelocity += pTargetBrain->GetCurrentVelocity();
								uNeighbourCount++;
							}
						}
					}
				}
			}

			if (glm::length(v3AverageVelocity) > 0.0f)
			{
				v3AverageVelocity /= uNeighbourCount;
				v3AlignmentForce = glm::normalize(v3AverageVelocity - m_v3CurrentVelocity);
			}

		}
	}
	return v3AlignmentForce;
}

glm::vec3 BrainComponent::CalculateCohesionForce()
{
	glm::vec3 v3AveragePosition(0.0f, 0.0f, 0.0f);
	glm::vec3 v3CohesionForce(0.0f, 0.0f, 0.0f);
	unsigned int  uNeighbourCount = 0;

	if (GetOwnerEntity())
	{
		TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
		if (pLocalTransform)
		{
			glm::vec3 v3LocalPos = pLocalTransform->GetCurrentPosition();

			const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
			std::map<const unsigned int, Entity*>::const_iterator xIter;

			for (xIter = xEntityMap.begin(); xIter != xEntityMap.end(); ++xIter)
			{
				Entity* pTarget = xIter->second;
				if (pTarget && pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID() && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model == static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model)
				{
					TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
					
					if (pTargetTransform)
					{
						glm::vec3 v3TargetPos = pTargetTransform->GetCurrentPosition();
						float fDistanceBetween = glm::length(v3LocalPos - v3TargetPos);
						if (fDistanceBetween < m_fNEIGHBOURHOOD_RADIUS)
						{
							BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));
							if (pTargetBrain)
							{
								//for target leader ness
								for (int i = 0; i < pTargetBrain->m_iLEADERNESS; ++i)
								{
									v3AveragePosition += v3TargetPos;
									//add that amount also
									uNeighbourCount++;
								}
							}
						}
					}
				}
			}

			if (glm::length(v3AveragePosition) > 0.0f)
			{
				v3AveragePosition /= uNeighbourCount;
				v3CohesionForce = glm::normalize(v3AveragePosition - v3LocalPos);
			}

		}
	}
	return v3CohesionForce;
}

glm::vec3 BrainComponent::CalculateCollisionAvoidanceForce()
{
	glm::vec3 v3CollisionAvoidanceForce(0.0f, 0.0f, 0.0f);
	
	if (GetOwnerEntity())
	{
		TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
	
		if (pLocalTransform)
		{
			glm::vec3 ahead = pLocalTransform->GetCurrentPosition() + glm::normalize(m_v3CurrentVelocity) * m_fMAX_SEE_AHEAD;
			glm::vec3 ahead2 = pLocalTransform->GetCurrentPosition() + glm::normalize(m_v3CurrentVelocity) * m_fMAX_SEE_AHEAD * 0.5f;
	
			Entity* mostThreatening = FindMostThreateningObstacle(ahead, ahead2);
	
			if (mostThreatening != nullptr)
			{
				TransformComponent* aTransform = static_cast<TransformComponent*>(mostThreatening->FindComponentOfType(TRANSFORM));
	
				v3CollisionAvoidanceForce.x = ahead.x - aTransform->GetCurrentPosition().x;
				v3CollisionAvoidanceForce.y = ahead.y - aTransform->GetCurrentPosition().y;
				v3CollisionAvoidanceForce.z = ahead.z - aTransform->GetCurrentPosition().z;
				glm::normalize(v3CollisionAvoidanceForce);
			}
			else
			{
				v3CollisionAvoidanceForce.x = 0.0f;
				v3CollisionAvoidanceForce.y = 0.0f;
				v3CollisionAvoidanceForce.z = 0.0f;
			}
		}
	}

	return v3CollisionAvoidanceForce;
}

float BrainComponent::Distance(glm::vec3 a_A, glm::vec3 a_B)
{
	return (float)sqrt(
		  pow((a_B.x - a_A.x), 2)
		+ pow((a_B.y - a_A.y), 2)
		+ pow((a_B.z - a_A.z), 2));
}

bool BrainComponent::LineIntersectsSphere(glm::vec3 a_ahead, glm::vec3 a_ahead2, Entity* a_Obstacle)
{
	ModelComponent* aModel = static_cast<ModelComponent*>(a_Obstacle->FindComponentOfType(MODEL));
	TransformComponent* aTransform = static_cast<TransformComponent*>(a_Obstacle->FindComponentOfType(TRANSFORM));
	float radius = aModel->m_fRadius;
	return Distance(aTransform->GetCurrentPosition(), a_ahead) <= radius || Distance(aTransform->GetCurrentPosition(), a_ahead2) <= radius;
}

Entity* BrainComponent::FindMostThreateningObstacle(glm::vec3 a_ahead, glm::vec3 a_ahead2)
{
	Entity* mostThreatening = nullptr;

	if (GetOwnerEntity())
	{
		TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
		if (pLocalTransform)
		{
			glm::vec3 v3LocalPos = pLocalTransform->GetCurrentPosition();
	
			const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
			std::map<const unsigned int, Entity*>::const_iterator xIter;
			
			for (xIter = xEntityMap.begin(); xIter != xEntityMap.end(); ++xIter)
			{
				Entity* pTarget = xIter->second;

				if (pTarget && pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID() && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model == OBSTACLE)
				{
					bool collision = LineIntersectsSphere(a_ahead, a_ahead2, pTarget);
	
					TransformComponent* aTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
					
					if (mostThreatening != nullptr)
					{
						TransformComponent* aMostThreateningTransform = static_cast<TransformComponent*>(mostThreatening->FindComponentOfType(TRANSFORM));

						if (collision && Distance(v3LocalPos, aTransform->GetCurrentPosition()) < Distance(v3LocalPos, aMostThreateningTransform->GetCurrentPosition()))
						{
							mostThreatening = pTarget;
						}
					}
					else if (collision && mostThreatening == nullptr)
					{
						mostThreatening = pTarget;
					}
				}
			}
		}
	}

	return mostThreatening;
}

glm::vec4 BrainComponent::CalculateAverageColour()
{
	glm::vec4 v4AverageColour(0.0f, 0.0f, 0.0f, 0.0f);

	unsigned int  uNeighbourCount = 0;

	if (GetOwnerEntity())
	{
		TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
		if (pLocalTransform)
		{
			glm::vec3 v3LocalPos = pLocalTransform->GetCurrentPosition();

			const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
			std::map<const unsigned int, Entity*>::const_iterator xIter;

			for (xIter = xEntityMap.begin(); xIter != xEntityMap.end(); ++xIter)
			{
				Entity* pTarget = xIter->second;
				if (pTarget && pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID() && static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL))->m_model == static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL))->m_model)
				{
					TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
					BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));

					if (pTargetTransform && pTargetBrain)
					{
						glm::vec3 v3TargetPos = pTargetTransform->GetCurrentPosition();
						float fDistanceBetween = glm::length(v3LocalPos - v3TargetPos);
						if (fDistanceBetween < m_fNEIGHBOURHOOD_RADIUS)
						{
							ModelComponent* pTargetModel = static_cast<ModelComponent*>(pTarget->FindComponentOfType(MODEL));
							if (pTargetModel)
							{
								for (int i = 0; i < pTargetBrain->m_iLEADERNESS; ++i)
								{
									v4AverageColour += pTargetModel->m_colour;
									uNeighbourCount++;
								}
							}
						}
					}
				}
			}

			if (glm::length(v4AverageColour) > 0.0f)
			{
				if (uNeighbourCount == 0)
				{
					ModelComponent* pLocalModel = static_cast<ModelComponent*>(GetOwnerEntity()->FindComponentOfType(MODEL));
					if (pLocalModel)
					{
						v4AverageColour = pLocalModel->m_colour;
					}
				}
				else
				{
					v4AverageColour /= uNeighbourCount;
				}
			}

		}
	}

	return v4AverageColour;
}

void BrainComponent::AdaptColour(float a_fDeltaTime)
{
	m_fCOLOUR_CHANGE_CURRENT_TIME -= a_fDeltaTime * 0.1f;

	if (m_fCOLOUR_CHANGE_CURRENT_TIME <= 0)
	{
		//Get the entity owner
		Entity* pEntity = GetOwnerEntity();
		if (!pEntity) return;

		//Get model component
		ModelComponent* pModelComp = static_cast<ModelComponent*>(pEntity->FindComponentOfType(MODEL));
		if (!pModelComp) return;

		//Change Color.
		//Red
		glm::vec4 v4AverageColour = CalculateAverageColour();

		if (pModelComp->m_colour.x > v4AverageColour.x)
		{
			pModelComp->m_colour.x -= m_fCOLOUR_CHANGE_AMOUNT;
		}
		if (pModelComp->m_colour.x < v4AverageColour.x)
		{
			pModelComp->m_colour.x += m_fCOLOUR_CHANGE_AMOUNT;
		}
		//Green
		if (pModelComp->m_colour.y > v4AverageColour.y)
		{
			pModelComp->m_colour.y -= m_fCOLOUR_CHANGE_AMOUNT;
		}
		if (pModelComp->m_colour.y < v4AverageColour.y)
		{
			pModelComp->m_colour.y += m_fCOLOUR_CHANGE_AMOUNT;
		}
		//Blue
		if (pModelComp->m_colour.z > v4AverageColour.z)
		{
			pModelComp->m_colour.z -= m_fCOLOUR_CHANGE_AMOUNT;
		}
		if (pModelComp->m_colour.z < v4AverageColour.z)
		{
			pModelComp->m_colour.z += m_fCOLOUR_CHANGE_AMOUNT;
		}
		//Alpha
		if (pModelComp->m_colour.w > v4AverageColour.w)
		{
			pModelComp->m_colour.w -= m_fCOLOUR_CHANGE_AMOUNT;
		}
		if (pModelComp->m_colour.w < v4AverageColour.w)
		{
			pModelComp->m_colour.w += m_fCOLOUR_CHANGE_AMOUNT;
		}
		m_fCOLOUR_CHANGE_CURRENT_TIME = m_fCOLOUR_CHANGE_TIME;
	}
}