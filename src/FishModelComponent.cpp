#include "FishModelComponent.h"
#include <glm/ext.hpp>
#include "aquarium.h"
#include "Entity.h"
#include "TransformComponent.h"

typedef Component PARENT;
glm::mat4 temp;
const glm::mat4& GetEntityMatrix() { return temp; }

FishModelComponent::FishModelComponent(Entity* pOwnerEntity) : PARENT(pOwnerEntity)
{
	m_eComponentType = MODEL;
}

FishModelComponent::~FishModelComponent()
{

}

void FishModelComponent::Update(float a_fDeltaTime)
{
	// Apply the delta time
	static float ftime = 0.f;
	ftime += a_fDeltaTime * 0.2f;
}

void FishModelComponent::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
	const glm::mat4* pEntityMatrix = nullptr;
	pEntityMatrix = &GetEntityMatrix();

	if (!pEntityMatrix) return; // Early out if we have no matrix

	// Draw our FBX Model
	// for each mesh in the model...
	//for (unsigned int i = 0; i < m_xModel->getMeshCount(); ++i)
	//{
	//	for (int i = 0; i < m_xModel->getSkeletonCount(); i++)
	//	{
	//		// Bind the array of bones
	//		FBXSkeleton* skeleton = m_xModel->getSkeletonByIndex(i);
	//		int boneLocation = glGetUniformLocation(a_uProgramID, "Bones");
	//		glUniformMatrix4fv(boneLocation, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);
	//	}
	//
	//	// get the current mesh
	//	FBXMeshNode* pMesh = m_xModel->getMeshByIndex(i);
	//
	//	unsigned int modelUniform = glGetUniformLocation(a_uProgramID, "Model");
	//	glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(*pEntityMatrix));
	//
	//	// Bind the texture to one of the ActiveTextures
	//	// if your shader supported multiple textures, you would bind each texture to a new Active Texture ID here
	//	//bind our textureLocation variable from the shaders and set it's value to 0 as the active texture is texture 0
	//	unsigned int texUniformID = glGetUniformLocation(a_uProgramID, "DiffuseTexture");
	//	glUniform1i(texUniformID, 0);
	//
	//	//set our active texture, and bind our loaded texture
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, pMesh->m_material->textureIDs[FBXMaterial::DiffuseTexture]);
	//
	//	texUniformID = glGetUniformLocation(a_uProgramID, "NormalMap");
	//	glUniform1i(texUniformID, 1);
	//
	//	//set our active texture, and bind our loaded texture
	//	glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, pMesh->m_material->textureIDs[FBXMaterial::NormalTexture]);
	//
	//	texUniformID = glGetUniformLocation(a_uProgramID, "SpecularMap");
	//	glUniform1i(texUniformID, 2);
	//
	//	//set our active texture, and bind our loaded texture
	//	glActiveTexture(GL_TEXTURE2);
	//	glBindTexture(GL_TEXTURE_2D, pMesh->m_material->textureIDs[FBXMaterial::SpecularTexture]);
	//	// Send the vertex data to the VBO
	//	glBindBuffer(GL_ARRAY_BUFFER, a_uVBO);
	//	glBufferData(GL_ARRAY_BUFFER, pMesh->m_vertices.size() * sizeof(FBXVertex), pMesh->m_vertices.data(), GL_STATIC_DRAW);
	//
	//
	//	// send the index data to the IBO
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_uIBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_indices.size() * sizeof(unsigned int), pMesh->m_indices.data(), GL_STATIC_DRAW);
	//
	//	glDrawElements(GL_TRIANGLES, pMesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	//}
}

