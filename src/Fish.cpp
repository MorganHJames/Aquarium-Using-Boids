//\===========================================================================================
//\ File: Fish.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for Fish.h.
//\===========================================================================================

#include "Fish.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BrainComponent.h"

typedef Entity PARENT;

Fish::Fish(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName)
{
	AddComponent(new ModelComponent(this, FISH, a_v4Colour, 0.0f));
	AddComponent(new TransformComponent(this, a_v3Pos, a_sName));
	AddComponent(new BrainComponent(this, a_iLeaderness));
}

Fish::~Fish()
{
}

void Fish::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);
}

void Fish::Draw(unsigned int a_uiProgramID, unsigned int a_uIVBO, unsigned int a_uiIBO)
{
	PARENT::Draw(a_uiProgramID, a_uIVBO, a_uiIBO);
}