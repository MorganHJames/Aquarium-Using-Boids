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
	void SetEntityMatrixRow(MATRIX_ROW a_eRow, glm::vec3 a_v3Vec) { m_m4EntityMatrix[a_eRow] = glm::vec4(a_v3Vec, (a_eRow == POSITION_VECTOR ? 1.f : 0.f)); }
	glm::vec3 GetEntityMatrixRow(MATRIX_ROW a_eRow) { return m_m4EntityMatrix[a_eRow]; }

	glm::mat4 m_m4EntityMatrix;

public:
	TransformComponent(Entity* a_pOwner, glm::vec3 a_v3Pos, std::string a_sName);
	~TransformComponent();

	virtual void Update(float a_fDeltaTime) {};

	virtual void Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO) {};

	const glm::vec3 GetRightDirection() { return GetEntityMatrixRow(RIGHT_VECTOR); }
	const glm::vec3 GetUpDirection() { return GetEntityMatrixRow(UP_VECTOR); }
	const glm::vec3 GetFacingDirection() { return GetEntityMatrixRow(FORWARD_VECTOR); }
	const glm::vec3 GetCurrentPosition() { return GetEntityMatrixRow(POSITION_VECTOR); }

	void SetRightDirection(glm::vec3 a_vRightDirection) { SetEntityMatrixRow(RIGHT_VECTOR, a_vRightDirection); }
	void SetUpDirection(glm::vec3 a_vUpDirection) { SetEntityMatrixRow(UP_VECTOR, a_vUpDirection); }
	void SetFacingDirection(glm::vec3 a_vFacingDirection) { SetEntityMatrixRow(FORWARD_VECTOR, a_vFacingDirection); }
	void SetCurrentPosition(glm::vec3 a_vCurrentPosition) { SetEntityMatrixRow(POSITION_VECTOR, a_vCurrentPosition); }

	const glm::mat4& GetEntityMatrix() { return m_m4EntityMatrix; }

	std::string m_sName;
};

#endif // _TRANSFORM_COMPONENT_H