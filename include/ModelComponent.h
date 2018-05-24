//\===========================================================================================
//\ File: Model Component.h
//\ Author: Morgan James
//\ Brief: Contains color, model and size information for the entities.
//\===========================================================================================

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
	ModelComponent(Entity* pOwnerEntity, MODELTYPE a_modelType, glm::vec4 a_colour, float a_fRadius);

	~ModelComponent();

	MODELTYPE m_model;

	glm::vec4 m_colour;

	float m_fRadius;

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_IBO);
};

#endif//_MODEL_COMPONENT_H