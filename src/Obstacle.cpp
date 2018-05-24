//\===========================================================================================
//\ File: Obstacle.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for Obstacle.h.
//\===========================================================================================

#include "Obstacle.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

typedef Entity PARENT;

Obstacle::Obstacle(glm::vec3 a_v3Pos, float a_fRadius, glm::vec4 a_v4Colour, std::string a_sName)
{
	AddComponent(new ModelComponent(this, OBSTACLE, a_v4Colour, a_fRadius));
	AddComponent(new TransformComponent(this, a_v3Pos, a_sName));
}

Obstacle::~Obstacle()
{
}

void Obstacle::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);
}

void Obstacle::Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO)
{
	PARENT::Draw(a_uiProgramID, a_uiVBO, a_uiIBO);
}