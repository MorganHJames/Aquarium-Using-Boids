//\===========================================================================================
//\ File: aquarium.h
//\ Author: Morgan James
//\ Brief: Holds the "Game loop" contains all the entities and calls upon their functions.
//\===========================================================================================

#ifndef __AQUARIUM_H_
#define __AQUARIUM_H_

// Includes
#include "Application.h"
#include "glm/ext.hpp"
#include <vector>

// Forward Declarations
class Entity;
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

	bool m_bPaused = false;
	std::string m_sPauseName = "Pause";

	//Fish Spawner
	std::string m_sFishName = "Morgan";
	char *m_cstrFishNameBuffer = &m_sFishName[0u];
	glm::vec3 m_v3FishPosition;
	int m_iFishLeaderness = 1;
	float m_fFishColour[4];

	//Shark Spawner
	std::string m_sSharkName = "Henry";
	char *m_cstrSharkNameBuffer = &m_sSharkName[0u];
	glm::vec3 m_v3SharkPosition;
	int m_iSharkLeaderness = 1;
	float m_fSharkColour[4];

	//Obstacle Spawner
	std::string m_sObstacleName = "James";
	char *m_cstrObstacleNameBuffer = &m_sObstacleName[0u];
	glm::vec3 m_v3ObstaclePosition;
	float m_iObstacleRadius = 1;
	float m_fObstacleColour[4];

	//Fish Behavior
	float m_fFishMAX_SPEED = 5.5f;
	float m_fFishJITTER = 0.9f;
	float m_fFishWANDER_RADIUS = 2.0f;
	float m_fFishSPHERE_FORWARD_MULTIPLIER = 10.0f;
	float m_fFishNEIGHBOURHOOD_RADIUS = 40.0f;
	float m_fFishMAX_SEE_AHEAD = 15.0f;
	float m_fFishWall_CHECK_DISTANCE = 1.0f;
	float m_fFishINSTINCT_RANGE = 50.0f;
	float m_fFishCOLOUR_CHANGE_TIME = 0.0000001f;

	//Fish Total Force:
	float m_fFishWanderForce = 0.3f;
	float m_fFishInstinctiveForce = 0.3f;
	float m_fFishSeperationForce = 2.0f;
	float m_fFishAlignmentForce = 0.8f;
	float m_fFishCohesionForce = 1.5f;
	float m_fFishCollisionAvoidanceForce = 10.0f;
	float m_fFishContainmentForce = 100.0f;

	//Shark Behavior
	float m_fSharkMAX_SPEED = 5.5f;
	float m_fSharkJITTER = 0.9f;
	float m_fSharkWANDER_RADIUS = 2.0f;
	float m_fSharkSPHERE_FORWARD_MULTIPLIER = 10.0f;
	float m_fSharkNEIGHBOURHOOD_RADIUS = 40.0f;
	float m_fSharkMAX_SEE_AHEAD = 15.0f;
	float m_fSharkWall_CHECK_DISTANCE = 1.0f;
	float m_fSharkINSTINCT_RANGE = 50.0f;
	float m_fSharkCOLOUR_CHANGE_TIME = 0.0000001f;

	//Shark Total Force:
	float m_fSharkWanderForce = 0.3f;
	float m_fSharkInstinctiveForce = 0.3f;
	float m_fSharkSeperationForce = 2.0f;
	float m_fSharkAlignmentForce = 0.8f;
	float m_fSharkCohesionForce = 1.5f;
	float m_fSharkCollisionAvoidanceForce = 10.0f;
	float m_fSharkContainmentForce = 100.0f;

	Entity* m_pSelectedFishEntity = nullptr;
	Entity* m_pSelectedSharkEntity = nullptr;
	Entity* m_pSelectedObstacleEntity = nullptr;
	bool m_bFreeLook = true;

	float m_fSelectedFishColour[4];
	float m_fSelectedSharkColour[4];
	float m_fSelectedObstacleColour[4];

	void Pause();

	void SetupEntities();
	void ResetEntities();

	void SpawnFish(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName);
	void DestroyAllFish();

	void SpawnShark(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName);
	void DestroyAllSharks();

	void SpawnObstacle(glm::vec3 a_v3Pos, float a_fRadius, glm::vec4 a_v4Colour, std::string a_sName);
	void DestroyAllObstacles();

	void DestroyAllEntities();
};
#endif//__AQUARIUM_H_