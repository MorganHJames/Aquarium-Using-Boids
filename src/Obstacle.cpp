#include "Obstacle.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

typedef Entity PARENT;


Obstacle::Obstacle(glm::vec3 a_pos, float a_radius, glm::vec4 a_colour)
{
	AddComponent(new ModelComponent(this, 0, a_radius, 0, a_colour));
	AddComponent(new TransformComponent(this, a_pos));
}

Obstacle::~Obstacle()
{
}

void Obstacle::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);
}

void Obstacle::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	PARENT::Draw(a_uProgramID, a_uVBO, a_uIBO);
}