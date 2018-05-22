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

	//Create Fish.
	Fish* fish1 = new Fish(glm::vec3 (0,0,0), 1, glm::vec4(1, 1, 0, 1), "Fish 1");
	m_axFishArray.push_back(fish1);
	Fish* fish2 = new Fish(glm::vec3(0, 0, 2), 1, glm::vec4(1, 1, 0, 1), "Fish 2");
	m_axFishArray.push_back(fish2);
	Fish* fish3 = new Fish(glm::vec3(0, 0, 4), 1, glm::vec4(1, 1, 0, 1), "Fish 3");
	m_axFishArray.push_back(fish3);
	Fish* fish4 = new Fish(glm::vec3(0, 0, 6), 1, glm::vec4(1, 1, 0, 1), "Fish 4");
	m_axFishArray.push_back(fish4);

	//create Sharks.
	Shark* shark1 = new Shark(glm::vec3(0, 2, 0), 2, glm::vec4(0,1,1,1), "Shark 1");
	m_axSharkArray.push_back(shark1);
	Shark* shark2 = new Shark(glm::vec3(0, 4, 1), 2, glm::vec4(1,1,0,1), "Shark 2");
	m_axSharkArray.push_back(shark2);
	Shark* shark3 = new Shark(glm::vec3(0, 6, 2), 2, glm::vec4(1, 1, 1, 1), "Shark 3");
	m_axSharkArray.push_back(shark3);
	Shark* shark4 = new Shark(glm::vec3(0, 8, 3), 2, glm::vec4(0, 0, 0, 1), "Shark 4");
	m_axSharkArray.push_back(shark4);

	//Create Obstacles.
	Obstacle* obstacle1 = new Obstacle(glm::vec3(0, 0, 50),  15, glm::vec4(1, 0, 0, 1), "Obstacle 1");
	m_axObstacleArray.push_back(obstacle1);
	Obstacle* obstacle2 = new Obstacle(glm::vec3(0, 50, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 2");
	m_axObstacleArray.push_back(obstacle2);
	Obstacle* obstacle3 = new Obstacle(glm::vec3(0, 0, -50), 15, glm::vec4(1, 0, 0, 1), "Obstacle 3");
	m_axObstacleArray.push_back(obstacle3);
	Obstacle* obstacle4 = new Obstacle(glm::vec3(0, -50, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 4");
	m_axObstacleArray.push_back(obstacle4);
	Obstacle* obstacle5 = new Obstacle(glm::vec3(50, 0, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 5");
	m_axObstacleArray.push_back(obstacle5);
	Obstacle* obstacle6 = new Obstacle(glm::vec3(-50, 0, 0), 15, glm::vec4(1, 0, 0, 1), "Obstacle 6");
	m_axObstacleArray.push_back(obstacle6);

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

void aquarium::Destroy()
{
	Gizmos::destroy();
}
