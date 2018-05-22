#ifndef _SHARK_H
#define _SHARK_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Shark : public Entity
{
public:
	Shark(glm::vec3 pos, float a_radius, float a_length, glm::vec4 a_colour);
	~Shark();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
};

#endif//_FISH_H