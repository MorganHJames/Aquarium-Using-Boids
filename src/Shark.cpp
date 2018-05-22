#include "Shark.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BrainComponent.h"

typedef Entity PARENT;

Shark::Shark(glm::vec3 pos, float a_radius, float a_length, glm::vec4 a_colour)
{
	AddComponent(new ModelComponent(this, 0, a_radius, a_length, a_colour));
	AddComponent(new TransformComponent(this, pos));
	AddComponent(new BrainComponent(this));
}

Shark::~Shark()
{
}

void Shark::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);
}

void Shark::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	PARENT::Draw(a_uProgramID, a_uVBO, a_uIBO);
}