#include "aquarium.h"
#include "Constants.h"
#include "BrainComponent.h"
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

	if (m_bPaused)
	{
		a_deltaTime = 0;
	}

	// clear all gizmos from last frame
	Gizmos::clear();

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

#pragma region GUI

#pragma region Fish Behavior
	//Fish Behavior
	ImGui::Begin("Fish Behavior");

	std::vector< Fish* >::iterator xIter2;
	for (xIter2 = m_axFishArray.begin(); xIter2 < m_axFishArray.end(); ++xIter2)
	{
		Entity* pCurrentEntity = *xIter2;
		if (pCurrentEntity)
		{
			BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(pCurrentEntity->FindComponentOfType(BRAIN));
			pCurrentBrain->m_fMAX_SPEED = m_fFishMAX_SPEED;
			pCurrentBrain->m_fJITTER = m_fFishJITTER;
			pCurrentBrain->m_fWANDER_RADIUS = m_fFishWANDER_RADIUS;
			pCurrentBrain->m_fSPHERE_FORWARD_MULTIPLIER = m_fFishSPHERE_FORWARD_MULTIPLIER;
			pCurrentBrain->m_fNEIGHBOURHOOD_RADIUS = m_fFishNEIGHBOURHOOD_RADIUS;
			pCurrentBrain->m_fMAX_SEE_AHEAD = m_fFishMAX_SEE_AHEAD;
			pCurrentBrain->m_fWall_CHECK_DISTANCE = m_fFishWall_CHECK_DISTANCE;
			pCurrentBrain->m_fINSTINCT_RANGE = m_fFishINSTINCT_RANGE;
			pCurrentBrain->m_fCOLOUR_CHANGE_TIME = m_fFishCOLOUR_CHANGE_TIME;

			//Total Force:
			pCurrentBrain->m_fWanderForce = m_fFishWanderForce;
			pCurrentBrain->m_fInstinctiveForce = m_fFishInstinctiveForce;
			pCurrentBrain->m_fSeperationForce = m_fFishSeperationForce;
			pCurrentBrain->m_fAlignmentForce = m_fFishAlignmentForce;
			pCurrentBrain->m_fCohesionForce = m_fFishCohesionForce;
			pCurrentBrain->m_fCollisionAvoidanceForce = m_fFishCollisionAvoidanceForce;
			pCurrentBrain->m_fContainmentForce = m_fFishContainmentForce;
		}
	}

	//View All Fish Gizmos
	if (ImGui::Button("View All Fish Gizmos", ImVec2(175, 50)))
	{
		std::vector< Fish* >::iterator xIter;
		for (xIter = m_axFishArray.begin(); xIter < m_axFishArray.end(); ++xIter)
		{
			Entity* pCurrentEntity = *xIter;
			if (pCurrentEntity)
			{
				BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(pCurrentEntity->FindComponentOfType(BRAIN));
				pCurrentBrain->m_bGizmos = true;
			}
		}
	
	}

	ImGui::NewLine();
	//Hide All Fish Gizmos
	if (ImGui::Button("Hide All Fish Gizmos", ImVec2(175, 50)))
	{
		std::vector< Fish* >::iterator xIter;
		for (xIter = m_axFishArray.begin(); xIter < m_axFishArray.end(); ++xIter)
		{
			Entity* pCurrentEntity = *xIter;
			if (pCurrentEntity)
			{
				BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(pCurrentEntity->FindComponentOfType(BRAIN));
				pCurrentBrain->m_bGizmos = false;
			}
		}
	}
	ImGui::NewLine();

	//Wander Force
	ImGui::Text("Wander Force");
	ImGui::SliderFloat("Wander Force", &m_fFishWanderForce, 0.0f, 1.0f);
	ImGui::NewLine();

	//Instinctive Force
	ImGui::Text("Instinctive Force");
	ImGui::SliderFloat("Instinctive Force", &m_fFishInstinctiveForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Separation Force
	ImGui::Text("Separation Force");
	ImGui::SliderFloat("Separation Force", &m_fFishSeperationForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Alignment Force
	ImGui::Text("Alignment Force");
	ImGui::SliderFloat("Alignment Force", &m_fFishAlignmentForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Cohesion Force
	ImGui::Text("Cohesion Force");
	ImGui::SliderFloat("Cohesion Force", &m_fFishCohesionForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Collision Force
	ImGui::Text("Collision Force");
	ImGui::SliderFloat("Collision Force", &m_fFishCollisionAvoidanceForce, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Containment Force
	ImGui::Text("Containment Force");
	ImGui::SliderFloat("Containment Force", &m_fFishContainmentForce, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Speed
	ImGui::Text("Speed");
	ImGui::SliderFloat("Max Speed", &m_fFishMAX_SPEED, 1.00f, 1000.0f);
	ImGui::NewLine();

	//Jitter
	ImGui::Text("Jitter");
	ImGui::SliderFloat("Jitter", &m_fFishJITTER, 0.0f, 10.0f);
	ImGui::NewLine();

	//Wander Radius
	ImGui::Text("Wander Radius");
	ImGui::SliderFloat("Wander Radius", &m_fFishWANDER_RADIUS, 0.0f, 10.0f);
	ImGui::NewLine();

	//Forward Multiplier
	ImGui::Text("Forward Multiplier");
	ImGui::SliderFloat("Forward Multiplier", &m_fFishSPHERE_FORWARD_MULTIPLIER, 0.0f, 100.0f);
	ImGui::NewLine();

	//Neighborhood Radius
	ImGui::Text("Neighborhood Radius");
	ImGui::SliderFloat("Neighborhood Radius", &m_fFishNEIGHBOURHOOD_RADIUS, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Obstacle View Distance
	ImGui::Text("Obstacle View Distance");
	ImGui::SliderFloat("Obstacle View Distance", &m_fFishMAX_SEE_AHEAD, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Wall View Distance
	ImGui::Text("Wall View Distance");
	ImGui::SliderFloat("Wall View Distance", &m_fFishWall_CHECK_DISTANCE, 0.0f, 100.0f);
	ImGui::NewLine();

	//Instinct Range
	ImGui::Text("Instinct Range");
	ImGui::SliderFloat("Instinct Range", &m_fFishINSTINCT_RANGE, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Color Change Rate
	ImGui::Text("Color Change Rate");
	ImGui::SliderFloat("Color Change Rate", &m_fFishCOLOUR_CHANGE_TIME, 0.0f, 1000.0f);
	ImGui::NewLine();

	ImGui::End();

#pragma endregion

#pragma region Shark Behavior
	//Shark Behavior
	ImGui::Begin("Shark Behavior");

	std::vector< Shark* >::iterator xIter3;
	for (xIter3 = m_axSharkArray.begin(); xIter3< m_axSharkArray.end(); ++xIter3)
	{
		Entity* pCurrentEntity = *xIter3;
		if (pCurrentEntity)
		{
			BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(pCurrentEntity->FindComponentOfType(BRAIN));
			pCurrentBrain->m_fMAX_SPEED = m_fSharkMAX_SPEED;
			pCurrentBrain->m_fJITTER = m_fSharkJITTER;
			pCurrentBrain->m_fWANDER_RADIUS = m_fSharkWANDER_RADIUS;
			pCurrentBrain->m_fSPHERE_FORWARD_MULTIPLIER = m_fSharkSPHERE_FORWARD_MULTIPLIER;
			pCurrentBrain->m_fNEIGHBOURHOOD_RADIUS = m_fSharkNEIGHBOURHOOD_RADIUS;
			pCurrentBrain->m_fMAX_SEE_AHEAD = m_fSharkMAX_SEE_AHEAD;
			pCurrentBrain->m_fWall_CHECK_DISTANCE = m_fSharkWall_CHECK_DISTANCE;
			pCurrentBrain->m_fINSTINCT_RANGE = m_fSharkINSTINCT_RANGE;
			pCurrentBrain->m_fCOLOUR_CHANGE_TIME = m_fSharkCOLOUR_CHANGE_TIME;

			//Total Force:
			pCurrentBrain->m_fWanderForce = m_fSharkWanderForce;
			pCurrentBrain->m_fInstinctiveForce = m_fSharkInstinctiveForce;
			pCurrentBrain->m_fSeperationForce = m_fSharkSeperationForce;
			pCurrentBrain->m_fAlignmentForce = m_fSharkAlignmentForce;
			pCurrentBrain->m_fCohesionForce = m_fSharkCohesionForce;
			pCurrentBrain->m_fCollisionAvoidanceForce = m_fSharkCollisionAvoidanceForce;
			pCurrentBrain->m_fContainmentForce = m_fSharkContainmentForce;
		}
	}

	//View All Shark Gizmos
	if (ImGui::Button("View All Shark Gizmos", ImVec2(175, 50)))
	{
		std::vector< Shark* >::iterator xIter;
		for (xIter = m_axSharkArray.begin(); xIter < m_axSharkArray.end(); ++xIter)
		{
			Entity* pCurrentEntity = *xIter;
			if (pCurrentEntity)
			{
				BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(pCurrentEntity->FindComponentOfType(BRAIN));
				pCurrentBrain->m_bGizmos = true;
			}
		}

	}

	ImGui::NewLine();
	//Hide All Shark Gizmos
	if (ImGui::Button("Hide All Shark Gizmos", ImVec2(175, 50)))
	{
		std::vector< Shark* >::iterator xIter;
		for (xIter = m_axSharkArray.begin(); xIter < m_axSharkArray.end(); ++xIter)
		{
			Entity* pCurrentEntity = *xIter;
			if (pCurrentEntity)
			{
				BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(pCurrentEntity->FindComponentOfType(BRAIN));
				pCurrentBrain->m_bGizmos = false;
			}
		}
	}
	ImGui::NewLine();

	//Wander Force
	ImGui::Text("Wander Force");
	ImGui::SliderFloat("Wander Force", &m_fSharkWanderForce, 0.0f, 1.0f);
	ImGui::NewLine();

	//Instinctive Force
	ImGui::Text("Instinctive Force");
	ImGui::SliderFloat("Instinctive Force", &m_fSharkInstinctiveForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Separation Force
	ImGui::Text("Separation Force");
	ImGui::SliderFloat("Separation Force", &m_fSharkSeperationForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Alignment Force
	ImGui::Text("Alignment Force");
	ImGui::SliderFloat("Alignment Force", &m_fSharkAlignmentForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Cohesion Force
	ImGui::Text("Cohesion Force");
	ImGui::SliderFloat("Cohesion Force", &m_fSharkCohesionForce, 0.0f, 10.0f);
	ImGui::NewLine();

	//Collision Force
	ImGui::Text("Collision Force");
	ImGui::SliderFloat("Collision Force", &m_fSharkCollisionAvoidanceForce, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Containment Force
	ImGui::Text("Containment Force");
	ImGui::SliderFloat("Containment Force", &m_fSharkContainmentForce, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Speed
	ImGui::Text("Speed");
	ImGui::SliderFloat("Max Speed", &m_fSharkMAX_SPEED, 1.00f, 1000.0f);
	ImGui::NewLine();

	//Jitter
	ImGui::Text("Jitter");
	ImGui::SliderFloat("Jitter", &m_fSharkJITTER, 0.0f, 10.0f);
	ImGui::NewLine();

	//Wander Radius
	ImGui::Text("Wander Radius");
	ImGui::SliderFloat("Wander Radius", &m_fSharkWANDER_RADIUS, 0.0f, 10.0f);
	ImGui::NewLine();

	//Forward Multiplier
	ImGui::Text("Forward Multiplier");
	ImGui::SliderFloat("Forward Multiplier", &m_fSharkSPHERE_FORWARD_MULTIPLIER, 0.0f, 100.0f);
	ImGui::NewLine();

	//Neighborhood Radius
	ImGui::Text("Neighborhood Radius");
	ImGui::SliderFloat("Neighborhood Radius", &m_fSharkNEIGHBOURHOOD_RADIUS, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Obstacle View Distance
	ImGui::Text("Obstacle View Distance");
	ImGui::SliderFloat("Obstacle View Distance", &m_fSharkMAX_SEE_AHEAD, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Wall View Distance
	ImGui::Text("Wall View Distance");
	ImGui::SliderFloat("Wall View Distance", &m_fSharkWall_CHECK_DISTANCE, 0.0f, 100.0f);
	ImGui::NewLine();

	//Instinct Range
	ImGui::Text("Instinct Range");
	ImGui::SliderFloat("Instinct Range", &m_fSharkINSTINCT_RANGE, 0.0f, 1000.0f);
	ImGui::NewLine();

	//Color Change Rate
	ImGui::Text("Color Change Rate");
	ImGui::SliderFloat("Color Change Rate", &m_fSharkCOLOUR_CHANGE_TIME, 0.0f, 1000.0f);
	ImGui::NewLine();

	ImGui::End();

#pragma endregion

#pragma region General Controls

	ImGui::Begin("General Controls");

	//Pause/Resume
	if (ImGui::Button(pauseName.c_str()))
	{
		if (pauseName == "Resume")
		{
			pauseName = "Pause";
		}
		else
		{
			pauseName = "Resume";
		}

		Pause();
	}
	ImGui::SameLine(60);

	//Reset
	if (ImGui::Button("Reset"))
	{
		ResetEntities();
	}
	ImGui::SameLine(110);

	//Destroy all fish
	if (ImGui::Button("Destroy All Fish"))
	{
		DestroyAllFish();
	}
	ImGui::SameLine(240);

	//Destroy all sharks
	if (ImGui::Button("Destroy All Sharks"))
	{
		DestroyAllSharks();
	}
	ImGui::SameLine(385);

	//Destroy all obstacles
	if (ImGui::Button("Destroy All Obstacles"))
	{
		DestroyAllObstacles();
	}
	ImGui::SameLine(550);

	//Destroy all entities
	if (ImGui::Button("Destroy All Entities"))
	{
		DestroyAllEntities();
	}

	ImGui::End();

#pragma endregion

#pragma region Fish Spawn
//Fish Spawn
ImGui::Begin("Spawn Fish");

//Name
ImGui::Text("Name");
ImGui::InputText("Name", cstrFishNameBuffer, IM_ARRAYSIZE(cstrFishNameBuffer));
sFishName = cstrFishNameBuffer;
ImGui::NewLine();

//Position
ImGui::Text("Position");
//sliders for x y and z
ImGui::SliderFloat("x", &v3FishPosition.x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("y", &v3FishPosition.y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("z", &v3FishPosition.z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::NewLine();

//Leaderness
ImGui::Text("Society Status");
ImGui::SliderInt("leaderness", &iFishLeaderness, 1, 10);
ImGui::NewLine();

//Color
ImGui::Text("Color");
ImGui::ColorPicker4("Color", m_fFishColour);
ImGui::NewLine();

if (ImGui::Button("Spawn Fish", ImVec2(175,50)))
{
	SpawnFish(v3FishPosition, iFishLeaderness, glm::vec4(m_fFishColour[0], m_fFishColour[1], m_fFishColour[2], m_fFishColour[3]), sFishName);
}

ImGui::End();

#pragma endregion

#pragma region Shark Spawn
//Shark Spawn
ImGui::Begin("Spawn Shark");

//Name
ImGui::Text("Name");
ImGui::InputText("Name", cstrSharkNameBuffer, IM_ARRAYSIZE(cstrSharkNameBuffer));
sSharkName = cstrSharkNameBuffer;
ImGui::NewLine();

//Position
ImGui::Text("Position");
//sliders for x y and z
ImGui::SliderFloat("x", &v3SharkPosition.x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("y", &v3SharkPosition.y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("z", &v3SharkPosition.z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::NewLine();

//Leaderness
ImGui::Text("Society Status");
ImGui::SliderInt("leaderness", &iSharkLeaderness, 1, 10);
ImGui::NewLine();

//Color
ImGui::Text("Color");
ImGui::ColorPicker4("Color", m_fSharkColour);
ImGui::NewLine();

if (ImGui::Button("Spawn Shark", ImVec2(175, 50)))
{
	SpawnShark(v3SharkPosition, iSharkLeaderness, glm::vec4(m_fSharkColour[0], m_fSharkColour[1], m_fSharkColour[2], m_fSharkColour[3]), sSharkName);
}

ImGui::End();

#pragma endregion

#pragma region Obstacle Spawn
//Obstacle Spawn
ImGui::Begin("Spawn Obstacle");

//Name
ImGui::Text("Name");
ImGui::InputText("Name", cstrObstacleNameBuffer, IM_ARRAYSIZE(cstrObstacleNameBuffer));
sObstacleName = cstrObstacleNameBuffer;
ImGui::NewLine();

//Position
ImGui::Text("Position");
//sliders for x y and z
ImGui::SliderFloat("x", &v3ObstaclePosition.x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("y", &v3ObstaclePosition.y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("z", &v3ObstaclePosition.z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::NewLine();

//Size
ImGui::Text("Size");
ImGui::SliderFloat("Radius", &iObstacleRadius, 0.01f, 100.0f);
ImGui::NewLine();

//Color
ImGui::Text("Color");
ImGui::ColorPicker4("Color", m_fObstacleColour);
ImGui::NewLine();

if (ImGui::Button("Spawn Obstacle", ImVec2(175, 50)))
{
	SpawnObstacle(v3ObstaclePosition, iObstacleRadius, glm::vec4(m_fObstacleColour[0], m_fObstacleColour[1], m_fObstacleColour[2], m_fObstacleColour[3]), sObstacleName);
}

ImGui::End();

#pragma endregion

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
	SpawnShark(glm::vec3(0, 2, 0), 2, glm::vec4(0, 0, 1, 1), "Shark 1");
	SpawnShark(glm::vec3(0, 4, 1), 2, glm::vec4(0, 0, 1, 1), "Shark 2");
	SpawnShark(glm::vec3(0, 6, 2), 2, glm::vec4(0, 0, 1, 1), "Shark 3");
	SpawnShark(glm::vec3(0, 8, 3), 2, glm::vec4(1, 0, 0, 1), "Shark 4");

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

void aquarium::SpawnFish(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_colour, std::string a_sName)
{
	Fish* fish = new Fish(a_v3Pos, a_iLeaderness, a_colour, a_sName);
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

			Entity* pCurrentEntity = *xIter;
			pCurrentEntity->RemoveEntity(pCurrentEntity);
		}
	}
}

void aquarium::DestroyAllFish()
{
	std::vector< Fish* >::iterator xIter;
	for (xIter = m_axFishArray.begin(); xIter < m_axFishArray.end(); ++xIter)
	{
		Entity* pCurrentEntity = *xIter;
		pCurrentEntity->RemoveEntity(pCurrentEntity);
	}
	m_axFishArray.clear();
}

void aquarium::SpawnShark(glm::vec3 pos, int a_iLeaderness, glm::vec4 a_colour, std::string a_sName)
{
	Shark* shark = new Shark(pos, a_iLeaderness, a_colour, a_sName);
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

			Entity* pCurrentEntity = *xIter;
			pCurrentEntity->RemoveEntity(pCurrentEntity);
		}
	}
}

void aquarium::DestroyAllSharks()
{
	std::vector< Shark* >::iterator xIter;
	for (xIter = m_axSharkArray.begin(); xIter < m_axSharkArray.end(); ++xIter)
	{
		Entity* pCurrentEntity = *xIter;
		pCurrentEntity->RemoveEntity(pCurrentEntity);
	}

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

			Entity* pCurrentEntity = *xIter;
			pCurrentEntity->RemoveEntity(pCurrentEntity);
		}
	}
}

void aquarium::DestroyAllObstacles()
{
	std::vector< Obstacle* >::iterator xIter;
	for (xIter = m_axObstacleArray.begin(); xIter < m_axObstacleArray.end(); ++xIter)
	{
		Entity* pCurrentEntity = *xIter;
		pCurrentEntity->RemoveEntity(pCurrentEntity);
	}
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

void aquarium::Pause()
{
	m_bPaused = !m_bPaused;
}
