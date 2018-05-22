#ifndef _BRAIN_COMPONENT_H
#define _BRAIN_COMPONENT_H

#include "Component.h"
#include "glm\glm.hpp"

enum BEHAVIOUR_TYPE
{
	SEEK,
	WANDER,
	FLEE,
	EVADE,
	PURSUE
};

class BrainComponent : public Component
{
public:
	BrainComponent(Entity* pOwnerEntity);
	~BrainComponent();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_IBO);

	inline glm::vec3 GetCurrentVelocity() const
	{
		return m_v3CurrentVelocity;
	}

private:
	//Steering behaviors.
	glm::vec3 CalculateSeekForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const;
	glm::vec3 CalculatePursuitForce(Entity* a_target, const glm::vec3& v3CurrentPos) const;

	glm::vec3 CalculateFleeForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const;
	glm::vec3 CalculateEvadeForce(Entity* a_target, const glm::vec3& v3CurrentPos) const;

	glm::vec3 CalculateWanderForce(const glm::vec3& v3CurrentPos, const glm::vec3& v3Forward);

	glm::vec3 CalculateInstinctForce(glm::vec3& v3CurrentPos);

	glm::vec3 CalculateContainmentForce(const glm::vec3& v3CurrentPos);

	//Collision Avoidance
	glm::vec3 CalculateCollisionAvoidanceForce();

	//Helper Functions.
	float Distance(glm::vec3 a_A, glm::vec3 a_B);
	bool LineIntersectsSphere(glm::vec3 a_ahead, glm::vec3 a_ahead2, Entity* a_Obstacle);
	Entity* FindMostThreateningObstacle(glm::vec3 a_ahead, glm::vec3 a_ahead2);

	//Flocking behaviors.
	glm::vec3 CalculateSeperationForce();
	glm::vec3 CalculateAlignmentForce();
	glm::vec3 CalculateCohesionForce();

	glm::vec3 m_v3CurrentVelocity;
	BEHAVIOUR_TYPE m_eCurrentBehaviour;

	float m_fContainmentTimer;

};

#endif//_BRAIN_COMPONENT_H