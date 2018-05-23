#include "aquarium.h"
// Core includes
#include <iostream>
#include <time.h>

// Framework includes
#include "Gizmos.h"
#include "Utilities.h"
#include "GL/glew.h"
#include "glfw3.h"
#include "glm/ext.hpp"

#include "Shark.h"
#include "Obstacle.h"
#include "Fish.h"

#include "imgui.h"

// Framework includes

#define DEFAULT_SCREENWIDTH 1280
#define DEFAULT_SCREENHEIGHT 720


aquarium::aquarium()
{
}


aquarium::~aquarium()
{
}

bool aquarium::onCreate()
{
	// initialise the Gizmos helper class.
	Gizmos::create();

	//Create all the entities.
	SetupEntities();

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(150, 150, 150), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, DEFAULT_SCREENWIDTH / (float)DEFAULT_SCREENHEIGHT, 0.1f, 1000.0f);

	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void aquarium::Update(float a_deltaTime)
{
	// update our camera matrix using the keyboard/mouse
	Utility::freeMovement(m_cameraMatrix, a_deltaTime, 10);

	// clear all gizmos from last frame
	Gizmos::clear();

	// add an identity matrix gizmo
	Gizmos::addTransform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

	// Create the bounding box
	Gizmos::addBox(glm::vec3(0.f, 0.f, 0.f), glm::vec3(100.f, 100.f, 100.f), false, glm::vec4(1.f, 1.0f, 1.f, 1.f));

	// Update all our fish
	std::vector< Fish* >::iterator xIter;
	for (xIter = m_axFishArray.begin(); xIter < m_axFishArray.end(); ++xIter)
	{
		Entity* pCurrentEntity = *xIter;
		if (pCurrentEntity)
		{
			pCurrentEntity->Update(a_deltaTime);
		}
	}

	// Update all our shark
	std::vector< Shark* >::iterator xIter1;
	for (xIter1 = m_axSharkArray.begin(); xIter1 < m_axSharkArray.end(); ++xIter1)
	{
		Entity* pCurrentEntity = *xIter1;
		if (pCurrentEntity)
		{
			pCurrentEntity->Update(a_deltaTime);
		}
	}

#pragma region ImGui

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static float f = 0.0f;
	static int counter = 0;
	ImGui::Begin("Spawn Entity");
	ImGui::End();


#pragma endregion

	// quit our application when escape is pressed
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		quit();


}

void aquarium::Draw()
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);

	// Draw all our fish
	std::vector< Fish* >::iterator xIter;
	for (xIter = m_axFishArray.begin(); xIter < m_axFishArray.end(); ++xIter)
	{
		Fish* pCurrentEntity = *xIter;
		if (pCurrentEntity)
		{
			pCurrentEntity->Draw(m_programID, m_vbo, m_ibo);
		}
	}

	// Draw all our sharks
	std::vector< Shark* >::iterator xIter1;
	for (xIter1 = m_axSharkArray.begin(); xIter1 < m_axSharkArray.end(); ++xIter1)
	{
		Shark* pCurrentEntity = *xIter1;
		if (pCurrentEntity)
		{
			pCurrentEntity->Draw(m_programID, m_vbo, m_ibo);
		}
	}

	// Draw all our obstacles.
	std::vector< Obstacle* >::iterator xIter2;
	for (xIter2 = m_axObstacleArray.begin(); xIter2 < m_axObstacleArray.end(); ++xIter2)
	{
		Obstacle* pCurrentEntity = *xIter2;
		if (pCurrentEntity)
		{
			pCurrentEntity->Draw(m_programID, m_vbo, m_ibo);
		}
	}

	// draw the gizmos from this frame
	Gizmos::draw(viewMatrix, m_projectionMatrix);
}

void aquarium::SetupEntities()
{
	//Create Fish.
	SpawnFish(glm::vec3(0, 0, 0), 1, glm::vec4(1, 1, 0, 1), "Fish 1");
	SpawnFish(glm::vec3(0, 0, 2), 1, glm::vec4(1, 1, 0, 1), "Fish 2");
	SpawnFish(glm::vec3(0, 0, 4), 1, glm::vec4(1, 1, 0, 1), "Fish 3");
	SpawnFish(glm::vec3(0, 0, 6), 1, glm::vec4(1, 1, 0, 1), "Fish 4");

	//create Sharks.
	SpawnShark(glm::vec3(0, 2, 0), 2, glm::vec4(0, 1, 1, 1), "Shark 1");
	SpawnShark(glm::vec3(0, 4, 1), 2, glm::vec4(1, 1, 0, 1), "Shark 2");
	SpawnShark(glm::vec3(0, 6, 2), 2, glm::vec4(1, 1, 1, 1), "Shark 3");
	SpawnShark(glm::vec3(0, 8, 3), 2, glm::vec4(0, 0, 0, 1), "Shark 4");

	//Create Obstacles.
	SpawnObstacle(glm::vec3(0, 0, 50), 15, glm::vec4(1, 0, 0, 1), "Obstacle 1");
	SpawnObstacle(glm::vec3(0, 50, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 2");
	SpawnObstacle(glm::vec3(0, 0, -50), 15, glm::vec4(1, 0, 0, 1), "Obstacle 3");
	SpawnObstacle(glm::vec3(0, -50, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 4");
	SpawnObstacle(glm::vec3(50, 0, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 5");
	SpawnObstacle(glm::vec3(-50, 0, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 6");
}

void aquarium::ResetEntities()
{ 
	DestroyAllEntities();
	SetupEntities();
}

void aquarium::SpawnFish(glm::vec3 a_v3Pos, float a_fLeaderness, glm::vec4 a_colour, std::string a_sName)
{
	Fish* fish = new Fish(a_v3Pos, a_fLeaderness, a_colour, a_sName);
	m_axFishArray.push_back(fish);
}

void aquarium::DestroyFish(Fish* a_fish)
{
	std::vector< Fish* >::iterator xIter;
	for (xIter = m_axFishArray.begin(); xIter < m_axFishArray.end(); ++xIter)
	{
		if (*xIter == a_fish)
		{
			m_axFishArray.erase(xIter);
		}
	}
}

void aquarium::DestroyAllFish()
{
	m_axFishArray.clear();
}

void aquarium::SpawnShark(glm::vec3 pos, float a_fLeaderness, glm::vec4 a_colour, std::string a_sName)
{
	Shark* shark = new Shark(pos, a_fLeaderness, a_colour, a_sName);
	m_axSharkArray.push_back(shark);
}

void aquarium::DestroyShark(Shark* a_shark)
{
	std::vector< Shark* >::iterator xIter;
	for (xIter = m_axSharkArray.begin(); xIter < m_axSharkArray.end(); ++xIter)
	{
		if (*xIter == a_shark)
		{
			m_axSharkArray.erase(xIter);
		}
	}
}

void aquarium::DestroyAllSharks()
{
	m_axSharkArray.clear();
}

void aquarium::SpawnObstacle(glm::vec3 a_pos, float a_radius, glm::vec4 a_colour, std::string a_sName)
{
	Obstacle* obstacle = new Obstacle(a_pos, a_radius, a_colour, a_sName);
	m_axObstacleArray.push_back(obstacle);
}

void aquarium::DestroyObstacle(Obstacle* a_obstacle)
{
	std::vector< Obstacle* >::iterator xIter;
	for (xIter = m_axObstacleArray.begin(); xIter < m_axObstacleArray.end(); ++xIter)
	{
		if (*xIter == a_obstacle)
		{
			m_axObstacleArray.erase(xIter);
		}
	}
}

void aquarium::DestroyAllObstacles()
{
	m_axObstacleArray.clear();
}

void aquarium::DestroyAllEntities()
{
	DestroyAllFish();
	DestroyAllSharks();
	DestroyAllObstacles();
}

void aquarium::Destroy()
{
	Gizmos::destroy();
}
