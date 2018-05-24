//\===========================================================================================
//\ File: Shark.h
//\ Author: Morgan James
//\ Brief: Allows for the creation of Sharks!
//\===========================================================================================

#ifndef _SHARK_H
#define _SHARK_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Shark : public Entity
{
public:
	Shark(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName);
	~Shark();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO);
};

#endif//_FISH_H