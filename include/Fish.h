//\===========================================================================================
//\ File: Fish.h
//\ Author: Morgan James
//\ Brief: Allows for the creation of fish!
//\===========================================================================================

#ifndef _FISH_H
#define _FISH_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Fish : public Entity
{
public:
	Fish(glm::vec3 pos, int a_iLeaderness, glm::vec4 a_colour, std::string a_sName);
	~Fish();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
};

#endif//_FISH_H