//\===========================================================================================
//\ File: Shark.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for Obstacle.h.
//\===========================================================================================

#include "Shark.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BrainComponent.h"

typedef Entity PARENT;

Shark::Shark(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName)
{
	AddComponent(new ModelComponent(this, SHARK, a_v4Colour, 0.0f));
	AddComponent(new TransformComponent(this, a_v3Pos, a_sName));
	AddComponent(new BrainComponent(this, a_iLeaderness));
}

Shark::~Shark()
{
}

void Shark::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);
}

void Shark::Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO)
{
	PARENT::Draw(a_uiProgramID, a_uiVBO, a_uiIBO);
}