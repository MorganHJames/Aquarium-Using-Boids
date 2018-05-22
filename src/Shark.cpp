#include "Shark.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BrainComponent.h"

typedef Entity PARENT;

Shark::Shark(glm::vec3 pos, float a_fLeaderness, glm::vec4 a_colour)
{
	AddComponent(new ModelComponent(this, SHARK, a_colour, 0.0f));
	AddComponent(new TransformComponent(this, pos));
	AddComponent(new BrainComponent(this, a_fLeaderness));
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