#ifndef __AQUARIUM_H_
#define __AQUARIUM_H_

// Includes
#include "Application.h"
#include "glm/ext.hpp"
#include <vector>

// Forward Declarations
class Fish;
class Shark;
class Obstacle;

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
	std::vector< Shark* > m_axSharkArray;
	std::vector< Obstacle* > m_axObstacleArray;

	glm::vec4 m_lightPos;

	void SetupEntities();
	void ResetEntities();

	void SpawnFish(glm::vec3 a_v3Pos, float a_fLeaderness, glm::vec4 a_colour, std::string a_sName);
	void DestroyFish(Fish* a_fish);
	void DestroyAllFish();

	void SpawnShark(glm::vec3 pos, float a_fLeaderness, glm::vec4 a_colour, std::string a_sName);
	void DestroyShark(Shark* a_shark);
	void DestroyAllSharks();

	void SpawnObstacle(glm::vec3 a_pos, float a_radius, glm::vec4 a_colour, std::string a_sName);
	void DestroyObstacle(Obstacle* a_obstacle);
	void DestroyAllObstacles();

	void DestroyAllEntities();
};
#endif//__AQUARIUM_H_