//\===========================================================================================
//\ File: aquarium.cpp
//\ Author: Morgan James
//\ Brief: Contains the function definitions for aquarium.h.
//\===========================================================================================

#include "aquarium.h"
#include "Constants.h"
#include "BrainComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
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

void aquarium::Update(float a_fDeltaTime)
{
	// update our camera matrix using the keyboard/mouse
	if (m_bFreeLook)
	Utility::freeMovement(m_cameraMatrix, a_fDeltaTime, 10);

	if (m_bPaused)
	{
		a_fDeltaTime = 0;
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
			pCurrentEntity->Update(a_fDeltaTime);
		}
	}

	// Update all our shark
	std::vector< Shark* >::iterator xIter1;
	for (xIter1 = m_axSharkArray.begin(); xIter1 < m_axSharkArray.end(); ++xIter1)
	{
		Entity* pCurrentEntity = *xIter1;
		if (pCurrentEntity)
		{
			pCurrentEntity->Update(a_fDeltaTime);
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
	if (ImGui::Button(m_sPauseName.c_str()))
	{
		if (m_sPauseName == "Resume")
		{
			m_sPauseName = "Pause";
		}
		else
		{
			m_sPauseName = "Resume";
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
ImGui::InputText("Name", m_cstrFishNameBuffer, IM_ARRAYSIZE(m_cstrFishNameBuffer));
m_sFishName = m_cstrFishNameBuffer;
ImGui::NewLine();

//Position
ImGui::Text("Position");
//sliders for x y and z
ImGui::SliderFloat("x", &m_v3FishPosition.x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("y", &m_v3FishPosition.y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("z", &m_v3FishPosition.z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::NewLine();

//Leaderness
ImGui::Text("Society Status");
ImGui::SliderInt("leaderness", &m_iFishLeaderness, 1, 10);
ImGui::NewLine();

//Color
ImGui::Text("Color");
ImGui::ColorPicker4("Color", m_fFishColour);
ImGui::NewLine();

if (ImGui::Button("Spawn Fish", ImVec2(175,50)))
{
	SpawnFish(m_v3FishPosition, m_iFishLeaderness, glm::vec4(m_fFishColour[0], m_fFishColour[1], m_fFishColour[2], m_fFishColour[3]), m_sFishName);
}

ImGui::End();

#pragma endregion

#pragma region Shark Spawn
//Shark Spawn
ImGui::Begin("Spawn Shark");

//Name
ImGui::Text("Name");
ImGui::InputText("Name", m_cstrSharkNameBuffer, IM_ARRAYSIZE(m_cstrSharkNameBuffer));
m_sSharkName = m_cstrSharkNameBuffer;
ImGui::NewLine();

//Position
ImGui::Text("Position");
//sliders for x y and z
ImGui::SliderFloat("x", &m_v3SharkPosition.x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("y", &m_v3SharkPosition.y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("z", &m_v3SharkPosition.z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::NewLine();

//Leaderness
ImGui::Text("Society Status");
ImGui::SliderInt("leaderness", &m_iSharkLeaderness, 1, 10);
ImGui::NewLine();

//Color
ImGui::Text("Color");
ImGui::ColorPicker4("Color", m_fSharkColour);
ImGui::NewLine();

if (ImGui::Button("Spawn Shark", ImVec2(175, 50)))
{
	SpawnShark(m_v3SharkPosition, m_iSharkLeaderness, glm::vec4(m_fSharkColour[0], m_fSharkColour[1], m_fSharkColour[2], m_fSharkColour[3]), m_sSharkName);
}

ImGui::End();

#pragma endregion

#pragma region Obstacle Spawn
//Obstacle Spawn
ImGui::Begin("Spawn Obstacle");

//Name
ImGui::Text("Name");
ImGui::InputText("Name", m_cstrObstacleNameBuffer, IM_ARRAYSIZE(m_cstrObstacleNameBuffer));
m_sObstacleName = m_cstrObstacleNameBuffer;
ImGui::NewLine();

//Position
ImGui::Text("Position");
//sliders for x y and z
ImGui::SliderFloat("x", &m_v3ObstaclePosition.x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("y", &m_v3ObstaclePosition.y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::SliderFloat("z", &m_v3ObstaclePosition.z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
ImGui::NewLine();

//Size
ImGui::Text("Size");
ImGui::SliderFloat("Radius", &m_iObstacleRadius, 0.01f, 100.0f);
ImGui::NewLine();

//Color
ImGui::Text("Color");
ImGui::ColorPicker4("Color", m_fObstacleColour);
ImGui::NewLine();

if (ImGui::Button("Spawn Obstacle", ImVec2(175, 50)))
{
	SpawnObstacle(m_v3ObstaclePosition, m_iObstacleRadius, glm::vec4(m_fObstacleColour[0], m_fObstacleColour[1], m_fObstacleColour[2], m_fObstacleColour[3]), m_sObstacleName);
}

ImGui::End();

#pragma endregion

#pragma region Fish Traits

//Fish Traits
ImGui::Begin("Fish Traits");

if (ImGui::Button("De-select"))
{
	m_pSelectedSharkEntity = nullptr;
	m_pSelectedObstacleEntity = nullptr;
	m_pSelectedFishEntity = nullptr;
}

std::vector< Fish* >::iterator xIter4;
for (xIter4 = m_axFishArray.begin(); xIter4 < m_axFishArray.end(); ++xIter4)
{
	Entity* pCurrentEntity = *xIter4;
	if (pCurrentEntity)
	{
		TransformComponent* pCurrentTransform = static_cast<TransformComponent*>(pCurrentEntity->FindComponentOfType(TRANSFORM));

		if (ImGui::Button(pCurrentTransform->m_sName.c_str()))
		{
			m_pSelectedSharkEntity = nullptr;
			m_pSelectedObstacleEntity = nullptr;
			m_pSelectedFishEntity = pCurrentEntity;
		}
	}
}

if (m_pSelectedFishEntity != nullptr)
{
	TransformComponent* pCurrentTransform = static_cast<TransformComponent*>(m_pSelectedFishEntity->FindComponentOfType(TRANSFORM));
	BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(m_pSelectedFishEntity->FindComponentOfType(BRAIN));
	ModelComponent* pCurrentModel = static_cast<ModelComponent*>(m_pSelectedFishEntity->FindComponentOfType(MODEL));

	//Follow
	m_bFreeLook = false;
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(50, 50, 50), pCurrentTransform->GetCurrentPosition(), glm::vec3(0, 1, 0)));

	//Name
	ImGui::Text("Name");
	char *cstrSelectedFishNameBuffer = &pCurrentTransform->m_sName[0u];
	ImGui::InputText("Name", cstrSelectedFishNameBuffer, IM_ARRAYSIZE(cstrSelectedFishNameBuffer));
	ImGui::NewLine();

	//Position
	ImGui::Text("Position");
	//sliders for x y and z
	float x = pCurrentTransform->GetCurrentPosition().x;
	float y = pCurrentTransform->GetCurrentPosition().y;
	float z = pCurrentTransform->GetCurrentPosition().z;
	ImGui::SliderFloat("x", &x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	ImGui::SliderFloat("y", &y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	ImGui::SliderFloat("z", &z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	pCurrentTransform->SetCurrentPosition(glm::vec3(x, y, z));
	ImGui::NewLine();

	//Leaderness
	ImGui::Text("Society Status");
	ImGui::SliderInt("leaderness", &pCurrentBrain->m_iLEADERNESS, 1, 10);
	ImGui::NewLine();

	//Color	
	ImGui::Text("Color");
	m_fSelectedFishColour[0] = pCurrentModel->m_v4Colour.x;
	m_fSelectedFishColour[1] = pCurrentModel->m_v4Colour.y;
	m_fSelectedFishColour[2] = pCurrentModel->m_v4Colour.z;
	m_fSelectedFishColour[3] = pCurrentModel->m_v4Colour.w;
	ImGui::ColorPicker4("Color", m_fSelectedFishColour);
	pCurrentModel->m_v4Colour = glm::vec4(m_fSelectedFishColour[0], m_fSelectedFishColour[1], m_fSelectedFishColour[2], m_fSelectedFishColour[3]);
	
	ImGui::NewLine();
}
else
{
	m_bFreeLook = true;
}

ImGui::End();

#pragma endregion

#pragma region Shark Traits

//Shark Traits
ImGui::Begin("Shark Traits");

if (ImGui::Button("De-select"))
{
	m_pSelectedSharkEntity = nullptr;
	m_pSelectedObstacleEntity = nullptr;
	m_pSelectedSharkEntity = nullptr;
}
	
std::vector< Shark* >::iterator xIter5;
for (xIter5 = m_axSharkArray.begin(); xIter5 < m_axSharkArray.end(); ++xIter5)
{
	Entity* pCurrentEntity = *xIter5;
	if (pCurrentEntity)
	{
		TransformComponent* pCurrentTransform = static_cast<TransformComponent*>(pCurrentEntity->FindComponentOfType(TRANSFORM));

		if (ImGui::Button(pCurrentTransform->m_sName.c_str()))
		{
			m_pSelectedSharkEntity = nullptr;
			m_pSelectedObstacleEntity = nullptr;
			m_pSelectedSharkEntity = pCurrentEntity;
		}
	}
}

if (m_pSelectedSharkEntity != nullptr)
{
	TransformComponent* pCurrentTransform = static_cast<TransformComponent*>(m_pSelectedSharkEntity->FindComponentOfType(TRANSFORM));
	BrainComponent* pCurrentBrain = static_cast<BrainComponent*>(m_pSelectedSharkEntity->FindComponentOfType(BRAIN));
	ModelComponent* pCurrentModel = static_cast<ModelComponent*>(m_pSelectedSharkEntity->FindComponentOfType(MODEL));
	
	//Follow
	m_bFreeLook = false;
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(50, 50, 50), pCurrentTransform->GetCurrentPosition(), glm::vec3(0, 1, 0)));

	//Name
	ImGui::Text("Name");
	char *cstrSelectedSharkNameBuffer = &pCurrentTransform->m_sName[0u];
	ImGui::InputText("Name", cstrSelectedSharkNameBuffer, IM_ARRAYSIZE(cstrSelectedSharkNameBuffer));
	ImGui::NewLine();

	//Position
	ImGui::Text("Position");
	//sliders for x y and z
	float x = pCurrentTransform->GetCurrentPosition().x;
	float y = pCurrentTransform->GetCurrentPosition().y;
	float z = pCurrentTransform->GetCurrentPosition().z;
	ImGui::SliderFloat("x", &x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	ImGui::SliderFloat("y", &y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	ImGui::SliderFloat("z", &z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	pCurrentTransform->SetCurrentPosition(glm::vec3(x, y, z));
	ImGui::NewLine();

	//Leaderness
	ImGui::Text("Society Status");
	ImGui::SliderInt("leaderness", &pCurrentBrain->m_iLEADERNESS, 1, 10);
	ImGui::NewLine();

	//Color	
	ImGui::Text("Color");
	m_fSelectedSharkColour[0] = pCurrentModel->m_v4Colour.x;
	m_fSelectedSharkColour[1] = pCurrentModel->m_v4Colour.y;
	m_fSelectedSharkColour[2] = pCurrentModel->m_v4Colour.z;
	m_fSelectedSharkColour[3] = pCurrentModel->m_v4Colour.w;
	ImGui::ColorPicker4("Color", m_fSelectedSharkColour);
	pCurrentModel->m_v4Colour = glm::vec4(m_fSelectedSharkColour[0], m_fSelectedSharkColour[1], m_fSelectedSharkColour[2], m_fSelectedSharkColour[3]);

	ImGui::NewLine();
}
else
{
	m_bFreeLook = true;
}

ImGui::End();

#pragma endregion

#pragma region Obstacle Traits

//Obstacle Traits
ImGui::Begin("Obstacle Traits");

if (ImGui::Button("De-select"))
{
	m_pSelectedObstacleEntity = nullptr;
	m_pSelectedObstacleEntity = nullptr;
	m_pSelectedObstacleEntity = nullptr;
}

std::vector< Obstacle* >::iterator xIter6;
for (xIter6 = m_axObstacleArray.begin(); xIter6 < m_axObstacleArray.end(); ++xIter6)
{
	Entity* pCurrentEntity = *xIter6;
	if (pCurrentEntity)
	{
		TransformComponent* pCurrentTransform = static_cast<TransformComponent*>(pCurrentEntity->FindComponentOfType(TRANSFORM));

		if (ImGui::Button(pCurrentTransform->m_sName.c_str()))
		{
			m_pSelectedObstacleEntity = nullptr;
			m_pSelectedObstacleEntity = nullptr;
			m_pSelectedObstacleEntity = pCurrentEntity;
		}
	}
}

if (m_pSelectedObstacleEntity != nullptr)
{
	TransformComponent* pCurrentTransform = static_cast<TransformComponent*>(m_pSelectedObstacleEntity->FindComponentOfType(TRANSFORM));
	ModelComponent* pCurrentModel = static_cast<ModelComponent*>(m_pSelectedObstacleEntity->FindComponentOfType(MODEL));

	//Follow
	m_bFreeLook = false;
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(50, 50, 50), pCurrentTransform->GetCurrentPosition(), glm::vec3(0, 1, 0)));

	//Name
	ImGui::Text("Name");
	char *cstrSelectedObstacleNameBuffer = &pCurrentTransform->m_sName[0u];
	ImGui::InputText("Name", cstrSelectedObstacleNameBuffer, IM_ARRAYSIZE(cstrSelectedObstacleNameBuffer));
	ImGui::NewLine();

	//Position
	ImGui::Text("Position");
	//sliders for x y and z
	float x = pCurrentTransform->GetCurrentPosition().x;
	float y = pCurrentTransform->GetCurrentPosition().y;
	float z = pCurrentTransform->GetCurrentPosition().z;
	ImGui::SliderFloat("x", &x, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	ImGui::SliderFloat("y", &y, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	ImGui::SliderFloat("z", &z, -g_fAQUARIUM_SIZE + 10, g_fAQUARIUM_SIZE - 10);
	pCurrentTransform->SetCurrentPosition(glm::vec3(x, y, z));
	ImGui::NewLine();

	//Size
	ImGui::Text("Size");
	ImGui::SliderFloat("Radius", &pCurrentModel->m_fRadius, 0.01f, 100.0f);
	ImGui::NewLine();

	//Color	
	ImGui::Text("Color");
	m_fSelectedObstacleColour[0] = pCurrentModel->m_v4Colour.x;
	m_fSelectedObstacleColour[1] = pCurrentModel->m_v4Colour.y;
	m_fSelectedObstacleColour[2] = pCurrentModel->m_v4Colour.z;
	m_fSelectedObstacleColour[3] = pCurrentModel->m_v4Colour.w;
	ImGui::ColorPicker4("Color", m_fSelectedObstacleColour);
	pCurrentModel->m_v4Colour = glm::vec4(m_fSelectedObstacleColour[0], m_fSelectedObstacleColour[1], m_fSelectedObstacleColour[2], m_fSelectedObstacleColour[3]);

	ImGui::NewLine();
}
else
{
	m_bFreeLook = true;
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
	SpawnFish(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(1, 3), glm::vec4(1, 1, 0, 1), "Fish 1");
	SpawnFish(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(1, 3), glm::vec4(1, 1, 0, 1), "Fish 2");
	SpawnFish(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(1, 3), glm::vec4(1, 1, 0, 1), "Fish 3");
	SpawnFish(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(1, 3), glm::vec4(1, 1, 0, 1), "Fish 4");

	//create Sharks.
	SpawnShark(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(2, 4), glm::vec4(0, 0, 1, 1), "Shark 1");
	SpawnShark(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(2, 4), glm::vec4(0, 0, 1, 1), "Shark 2");
	SpawnShark(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(2, 4), glm::vec4(0, 0, 1, 1), "Shark 3");
	SpawnShark(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)), glm::linearRand(2, 4), glm::vec4(0, 0, 1, 1), "Shark 4");

	//Create Obstacles.
	SpawnObstacle(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE),  glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)),(float)glm::linearRand(1, 25), glm::vec4(1, 0, 0, 1), "Obstacle 1");
	SpawnObstacle(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE),  glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)),(float)glm::linearRand(1, 25), glm::vec4(1, 0, 0, 1), "Obstacle 2");
	SpawnObstacle(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE),  glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)),(float)glm::linearRand(1, 25), glm::vec4(1, 0, 0, 1), "Obstacle 3");
	SpawnObstacle(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE),  glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)),(float)glm::linearRand(1, 25), glm::vec4(1, 0, 0, 1), "Obstacle 4");
	SpawnObstacle(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE),  glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)),(float)glm::linearRand(1, 25), glm::vec4(1, 0, 0, 1), "Obstacle 5");
	SpawnObstacle(glm::vec3(glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE),glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE), glm::linearRand(-g_fAQUARIUM_SIZE, g_fAQUARIUM_SIZE)),  (float)glm::linearRand(1, 25), glm::vec4(1, 0, 0, 1), "Obstacle 6");
}

void aquarium::ResetEntities()
{ 
	DestroyAllEntities();
	SetupEntities();
}

void aquarium::SpawnFish(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName)
{
	Fish* fish = new Fish(a_v3Pos, a_iLeaderness, a_v4Colour, a_sName);
	m_axFishArray.push_back(fish);
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

	m_pSelectedFishEntity = nullptr;
}

void aquarium::SpawnShark(glm::vec3 a_v3Pos, int a_iLeaderness, glm::vec4 a_v4Colour, std::string a_sName)
{
	Shark* shark = new Shark(a_v3Pos, a_iLeaderness, a_v4Colour, a_sName);
	m_axSharkArray.push_back(shark);
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

	m_pSelectedSharkEntity = nullptr;
}

void aquarium::SpawnObstacle(glm::vec3 a_v3Pos, float a_fRadius, glm::vec4 a_v4Colour, std::string a_sName)
{
	Obstacle* obstacle = new Obstacle(a_v3Pos, a_fRadius, a_v4Colour, a_sName);
	m_axObstacleArray.push_back(obstacle);
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
	m_pSelectedObstacleEntity = nullptr;
}

void aquarium::DestroyAllEntities()
{
	DestroyAllFish();
	DestroyAllSharks();
	DestroyAllObstacles();
	m_pSelectedSharkEntity = nullptr;
	m_pSelectedObstacleEntity = nullptr;
	m_pSelectedFishEntity = nullptr;
}

void aquarium::Destroy()
{
	Gizmos::destroy();
}

void aquarium::Pause()
{
	m_bPaused = !m_bPaused;
}
