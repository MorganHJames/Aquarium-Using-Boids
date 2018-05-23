#ifndef _SHARK_H
#define _SHARK_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Shark : public Entity
{
public:
	Shark(glm::vec3 pos, int a_iLeaderness, glm::vec4 a_colour, std::string a_sName);
	~Shark();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
};

#endif//_FISH_H