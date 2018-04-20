#ifndef _FISH_MODEL_COMPONENT_H
#define _FISH_MODEL_COMPONENT_H

#include "Component.h"
#include <vector>

class FishModelComponent : public Component
{
private:

public:
	FishModelComponent(Entity* pOwnerEntity);
	~FishModelComponent();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_IBO);
};

#endif//_FISH_MODEL_COMPONENT_H