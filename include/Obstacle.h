#ifndef _OBSTACLE_H
#define _OBSTACLE_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Obstacle : public Entity
{
public:
	Obstacle(glm::vec3 a_pos, float a_radius, glm::vec4 a_colour);
	~Obstacle();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
};

#endif//_OBSTACLE_H