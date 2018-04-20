#ifndef __AQUARIUM_H_
#define __AQUARIUM_H_

// Includes
#include "Application.h"
#include "glm/ext.hpp"
#include <vector>

// Forward Declarations
class Fish;

// Derived application class that wraps up all globals neatly
class aquarium : public Application
{
public:
	aquarium();
	~aquarium();

protected:
	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	inline unsigned int GetIBO() { return m_ibo; }
	inline unsigned int GetVBO() { return m_vbo; }
	inline unsigned int GetVAO() { return m_vao; }
	inline unsigned int GetProgramID() { return m_programID; }

private:
	unsigned int m_programID;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;

	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;

	std::vector< Fish* > m_axFishArray;

	glm::vec4 m_lightPos;
};
#endif//__AQUARIUM_H_