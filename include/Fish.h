#ifndef _FISH_H
#define _FISH_H

// Includes
#include "Entity.h"
#include "glm\glm.hpp"

class Fish : public Entity
{
public:
	Fish(glm::vec3 pos);
	~Fish();

	virtual void Update(float a_fDeltaTime);
	void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);

private:

protected:
	float m_fScale;
};

#endif //_ENEMY_H