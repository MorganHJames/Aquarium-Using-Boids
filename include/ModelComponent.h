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
	ModelComponent(Entity* a_pOwnerEntity, MODELTYPE a_eModelType, glm::vec4 a_v4Colour, float a_fRadius);

	~ModelComponent();

	MODELTYPE m_eModel;

	glm::vec4 m_v4Colour;

	float m_fRadius;

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO);
};

#endif//_MODEL_COMPONENT_H