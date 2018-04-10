#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp>

// Derived application class that wraps up all globals neatly
class aquarium : public Application
{
public:

	aquarium();
	virtual ~aquarium();

protected:

	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
};

#endif // __Application_H_