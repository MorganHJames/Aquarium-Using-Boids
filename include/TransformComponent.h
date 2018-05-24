//\===========================================================================================
//\ File: TransformComponent.h
//\ Author: Morgan James
//\ Brief: Contains the transform  matrix data along with the entities name.
//\===========================================================================================

#ifndef _TRANSFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H

#include "Component.h"
#include <string>
#include "glm\ext.hpp"

enum MATRIX_ROW
{
	RIGHT_VECTOR,
	UP_VECTOR,
	FORWARD_VECTOR,
	POSITION_VECTOR,
};

class TransformComponent : public Component
{
private:
	void SetEntityMatrixRow(MATRIX_ROW eRow, glm::vec3 vVec) { m_m4EntityMatrix[eRow] = glm::vec4(vVec, (eRow == POSITION_VECTOR ? 1.f : 0.f)); }
	glm::vec3 GetEntityMatrixRow(MATRIX_ROW eRow) { return m_m4EntityMatrix[eRow]; }

	glm::mat4 m_m4EntityMatrix;

public:
	TransformComponent(Entity* pOwner, glm::vec3 pos, std::string a_sName);
	~TransformComponent();

	virtual void Update(float a_fDeltaTime) {};

	virtual void Draw(unsigned int a_ProgramID, unsigned int a_uVBO, unsigned int a_uIBO) {};

	const glm::vec3 GetRightDirection() { return GetEntityMatrixRow(RIGHT_VECTOR); }
	const glm::vec3 GetUpDirection() { return GetEntityMatrixRow(UP_VECTOR); }
	const glm::vec3 GetFacingDirection() { return GetEntityMatrixRow(FORWARD_VECTOR); }
	const glm::vec3 GetCurrentPosition() { return GetEntityMatrixRow(POSITION_VECTOR); }

	void SetRightDirection(glm::vec3 vRightDirection) { SetEntityMatrixRow(RIGHT_VECTOR, vRightDirection); }
	void SetUpDirection(glm::vec3 vUpDirection) { SetEntityMatrixRow(UP_VECTOR, vUpDirection); }
	void SetFacingDirection(glm::vec3 vFacingDirection) { SetEntityMatrixRow(FORWARD_VECTOR, vFacingDirection); }
	void SetCurrentPosition(glm::vec3 vCurrentPosition) { SetEntityMatrixRow(POSITION_VECTOR, vCurrentPosition); }

	const glm::mat4& GetEntityMatrix() { return m_m4EntityMatrix; }

	std::string m_sName;
};

#endif // _TRANSFORM_COMPONENT_H