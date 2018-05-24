//\===========================================================================================
//\ File: Obstacle.h
//\ Author: Morgan James
//\ Brief: Allows for the creation of obstacles!
//\===========================================================================================

#ifndef _OBSTACLE_H
#define _OBSTACLE_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Obstacle : public Entity
{
public:
	Obstacle(glm::vec3 a_v3Pos, float a_fRadius, glm::vec4 a_v4Colour, std::string a_sName);
	~Obstacle();
	
	float m_size = 1;

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO);
};

#endif//_OBSTACLE_H