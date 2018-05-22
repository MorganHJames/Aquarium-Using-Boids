#include "Fish.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BrainComponent.h"

typedef Entity PARENT;

Fish::Fish(glm::vec3 pos, float a_size, glm::vec4 a_colour)
{
	AddComponent(new ModelComponent(this, a_size, 0, 0, a_colour));
	AddComponent(new TransformComponent(this, pos));
	AddComponent(new BrainComponent(this));
}

Fish::~Fish()
{
}

void Fish::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);
}

void Fish::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	PARENT::Draw(a_uProgramID, a_uVBO, a_uIBO);
}