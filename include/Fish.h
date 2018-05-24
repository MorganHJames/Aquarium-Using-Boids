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
	Fish(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v3Colour, std::string a_sName);
	~Fish();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO);
};

#endif//_FISH_H