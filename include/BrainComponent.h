//\===========================================================================================
//\ File: BrainComponent.h
//\ Author: Morgan James
//\ Brief: Contains functions for figuring out what forces should be applied to entites and then doing so.
//\===========================================================================================

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
	BrainComponent(Entity* a_pOwnerEntity, int a_iLeaderness);
	~BrainComponent();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO);

	inline glm::vec3 GetCurrentVelocity() const
	{
		return m_v3CurrentVelocity;
	}

	float m_fMAX_SPEED = 5.5f;
	float m_fJITTER = 0.9f;
	float m_fWANDER_RADIUS = 2.0f;
	float m_fSPHERE_FORWARD_MULTIPLIER = 10.0f;
	float m_fNEIGHBOURHOOD_RADIUS = 40.0f;
	float m_fMAX_SEE_AHEAD = 15.0f;
	float m_fWall_CHECK_DISTANCE = 1.0f;
	float m_fINSTINCT_RANGE = 50.0f;
	float m_fCOLOUR_CHANGE_AMOUNT = 0.001f;
	float m_fCOLOUR_CHANGE_TIME = 0.0000001f;
	float m_fCOLOUR_CHANGE_CURRENT_TIME = 0.1f;
	int m_iLEADERNESS = 1;
	bool m_bGizmos = false;

	//Total Force:
	float m_fWanderForce = 0.3f;
	float m_fInstinctiveForce = 0.3f;
	float m_fSeperationForce = 2.0f;
	float m_fAlignmentForce = 0.8f;
	float m_fCohesionForce = 1.5f;
	float m_fCollisionAvoidanceForce = 10.0f;
	float m_fContainmentForce = 100.0f;

private:
	//Steering behaviors.
	glm::vec3 CalculateSeekForce(const glm::vec3& a_v3Target, const glm::vec3& a_v3CurrentPos) const;
	glm::vec3 CalculatePursuitForce(Entity* a_pTarget, const glm::vec3& a_v3CurrentPos) const;

	glm::vec3 CalculateFleeForce(const glm::vec3& a_v3Target, const glm::vec3& a_v3CurrentPos) const;
	glm::vec3 CalculateEvadeForce(Entity* a_pTarget, const glm::vec3& a_v3CurrentPos) const;

	glm::vec3 CalculateWanderForce(const glm::vec3& a_v3CurrentPos, const glm::vec3& a_v3Forward);

	glm::vec3 CalculateInstinctForce(glm::vec3& a_v3CurrentPos);

	glm::vec3 CalculateContainmentForce(const glm::vec3& a_v3CurrentPos);

	//Collision Avoidance
	glm::vec3 CalculateCollisionAvoidanceForce();

	//Helper Functions.
	float Distance(glm::vec3 a_v3A, glm::vec3 a_v3B);
	bool LineIntersectsSphere(glm::vec3 a_v3Ahead, glm::vec3 a_v3Ahead2, Entity* a_obstacle);
	Entity* FindMostThreateningObstacle(glm::vec3 a_v3Ahead, glm::vec3 a_v3Ahead2);

	//Color Adaption.
	void AdaptColour(float a_fDeltaTime);
	glm::vec4 CalculateAverageColour();

	//Flocking behaviors.
	glm::vec3 CalculateSeperationForce();
	glm::vec3 CalculateAlignmentForce();
	glm::vec3 CalculateCohesionForce();

	glm::vec3 m_v3CurrentVelocity;
	BEHAVIOUR_TYPE m_eCurrentBehaviour;
};

#endif//_BRAIN_COMPONENT_H