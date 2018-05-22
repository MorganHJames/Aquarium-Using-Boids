#ifndef _MODEL_COMPONENT_H
#define _MODEL_COMPONENT_H

#include "Component.h"
#include <vector>
#include <glm/ext.hpp>

enum MODELTYPE
{
	FISH,
	SHARK,
	OBSTACLE,
	WALL
};

class ModelComponent : public Component
{
private:

public:
	ModelComponent(Entity* pOwnerEntity, float a_size, float a_radius, float a_length, glm::vec4 a_colour);

	~ModelComponent();

	float m_size;
	float m_radius;
	float m_length;

	MODELTYPE m_model;

	glm::vec4 m_colour;

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_IBO);
};

#endif//_MODEL_COMPONENT_H