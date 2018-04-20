#include "MD5Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <SOIL.h>
#include <iostream>
#include <fstream>
#include <algorithm>

MD5Model::MD5Model()
{
}

MD5Model::~MD5Model()
{
	Unload();
}

//Load in the MD5 Model from file
bool MD5Model::Load(const char* a_filename, float a_fScale)
{
	m_fScale = a_fScale;
	std::fstream file;
	std::string line;
	//open the file for reading
	file.open(a_filename, std::ios_base::in);
	if (file.is_open())
	{
		//Get the size of the file to ensure this isn't just and empty file
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize fileLength = file.gcount();
		file.clear();   //  Since ignore will have set eof.
		file.seekg(0, std::ios_base::beg);
		
		if (fileLength == 0)
		{
			std::cout << "File contains no data!" << std::endl;
			return false;
		}

		std::string param;
		do
		{
			file >> param;
		
			if (param == "MD5Version")
			{
				file >> m_iMD5Version;
			}
			else if (param == "commandline")
			{
				//This is not important to us for rendering the file
				file.ignore(fileLength, '\n');
			}
			else if (param == "numJoints")
			{
				file >> m_iNumJoints;
			}
			else if (param == "numMeshes")
			{
				file >> m_iNumMeshes;
			}
			else if (param == "joints")
			{
				//we have found the joints so create our skeleton and allocate the memory required for this
				m_skeleton = new MD5Skeleton();
				m_skeleton->m_jointCount = m_iNumJoints;
				m_skeleton->m_joints = new MD5Joint*[m_iNumJoints];
				m_skeleton->m_bones = new glm::mat4[m_iNumJoints];
				m_skeleton->m_bindPoses = new glm::mat4[m_iNumJoints];
				//ignore the '{' symbol in the file
				file.ignore(fileLength, '{');

				for (int i = 0; i < m_iNumJoints; ++i)
				{
					MD5Joint* joint = new MD5Joint();
					file >> joint->m_name;
					//the name that has just been read in will contain some \" characters, these should be removed
					//using a standard std::algorithm for this
					joint->m_name.erase( std::remove(joint->m_name.begin(), joint->m_name.end(), '\"'), joint->m_name.end() );
					file >> joint->m_parentID;
					//the position and orientation data is surrounded by '(' ')' parenthesis. Ignore these 
					file.ignore(fileLength, '(');
					//read in the position for the joint
					glm::vec3 position;
					glm::quat orientation;
					file >> position.x >> position.y >> position.z;
					file.ignore(fileLength, '(');
					file >> orientation.x >> orientation.y >> orientation.z;
					//ignore everything else up to the '\n' character
					file.ignore(fileLength, '\n');

					//calculate the w component of the quaternion
					float t = 1.0f - (orientation.x * orientation.x) - (orientation.y * orientation.y) - (orientation.z * orientation.z);
					orientation.w = (t < 0.0f) ? 0.0f : -sqrtf(t);

					position *= m_fScale;

					//change the orientation of the root bone
					//MD5 format has Z axis stored as up and X axis stored as forwards the following converts from z Up to y up and from x forward to z forward
					glm::quat rotateToYUp = glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
					glm::quat rotateXToZ = glm::angleAxis(glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
					orientation = rotateToYUp * orientation;
					orientation = rotateXToZ * orientation;
					position = rotateToYUp * position;
					position = rotateXToZ * position;

					
					joint->m_position = position;
					joint->m_orientation = orientation;
					m_skeleton->m_joints[i] = joint;

					//convert the orientation to a rotation matrix
					glm::mat4 mRot = glm::mat4_cast(orientation);
					glm::mat4 mTranslate = glm::translate(position);

					m_skeleton->m_bones[i] = mTranslate * mRot;
					m_skeleton->m_bindPoses[i] = glm::inverse(mTranslate * mRot);
					

				}
				file.ignore(fileLength, '}');

			}
			else if (param == "mesh")
			{
				std::vector<std::string> vMeshNames;

				
				//This section of the file holds the mesh data
				MD5Mesh* mesh = new MD5Mesh();

				int numVerts, numTris, numWeights;

				file.ignore(fileLength, '{');
				do
				{
					file >> param;
					if (param == "//") {
						//this is a comment line in the md5 file skip this line
						file.ignore(fileLength, '\n');
					}
					else if (param == "numverts")
					{
						file >> numVerts;
						mesh->m_vertices.resize(numVerts);
						mesh->m_weightIndexes.resize(numVerts);
					}
					else if (param == "numtris")
					{
						file >> numTris;
						//for every triangle there are three indicies
						mesh->m_indices.resize(3 * numTris);
						file.ignore(fileLength, '\n');
					}
					else if (param == "numweights")
					{
						file >> numWeights;
						mesh->m_weights.resize(numWeights);
					}
					else if (param == "shader")
					{
						//The parameter for shader in MD5 models is for texture(s) that this model uses
						std::string shaderName;
						file >> shaderName;
						shaderName.erase(std::remove(shaderName.begin(), shaderName.end(), '\"'), shaderName.end());

						//base path for loading, used the a_filename argument passed into this function to extract any path then append a local path 
						//onto this, 
						std::string texturePath(a_filename);
						if (texturePath.find_last_of("\\/") != std::string::npos)
						{
							texturePath = texturePath.substr(0, texturePath.find_last_of("\\/"));
						}
						else //we probably have no path
						{
							texturePath = "";
						}
						//this shader name may or may not have the file path already appended to it
						//serach string for directory slash tokens if they are present assume it has path information
						if (shaderName.find_last_of("\\/") != std::string::npos)
						{
							texturePath = "../Resources/Textures/.";
							texturePath += shaderName;
						}
						else //no path, append path information
						{
							//all of my textures live in the texture path
							texturePath += "../Resources/Textures/";
						}

						//call our mesh's material to load the required textures from the base texture path
						mesh->m_material.LoadMaterials(texturePath, m_textures);
						

					}
					else if( param  == "vert")
					{
						int index = 0;
						file >> index;

						file.ignore(fileLength, '(');
						MD5Vertex vertex;

						file >> vertex.texCoord1.x >> vertex.texCoord1.y;
						file.ignore(fileLength, ')');

						MD5WeightIndex weightIndex;
						file >> weightIndex.startWeight >> weightIndex.countWeight;
						//file.ignore(fileLength, '\n');
						mesh->m_vertices[index] = vertex;
						mesh->m_weightIndexes[index] = weightIndex;
						
					}
					else if( param == "tri")
					{
						int triIndex = 0;
						file >> triIndex;
						//multiply the index by 3 as we are storing indices not triangles
						triIndex *= 3;
						//Remember that OpenGL has an anti-clockwise winding order MD5 is clockwise
						file >> mesh->m_indices[triIndex + 2];
						file >> mesh->m_indices[triIndex + 1];
						file >> mesh->m_indices[triIndex];
						
					}
					else if( param == "weight")
					{
						MD5Weight weight;

						int weightIndex = 0;
						file >> weightIndex;
						
						file >> weight.m_jointID >> weight.m_bias;
						file.ignore(fileLength, '(');
						file >> weight.m_boneSpacePosition.x >> weight.m_boneSpacePosition.y >> weight.m_boneSpacePosition.z;
						weight.m_boneSpacePosition *= m_fScale;
						mesh->m_weights[weightIndex] = weight;
						
					}

				} while (param != "}");

				//Calculate mesh vertex positions
				mesh->CalculateWeightModelSpacePositions(m_skeleton);
				mesh->CalculateVertexPositions(m_skeleton);
				mesh->CalculateVertexNormals(m_skeleton);

				m_meshes.push_back(mesh);
			}
			else
			{
				file.ignore(fileLength, '\n');
			}

		} while (!file.eof());

		file.sync();
		file.close();
		file.clear();




	}
	else
	{
		std::cout << "Unable to Open File for reading" << std::endl;
		return false;
	}
	return true;

}

//Calculate the world space positions of the model

void MD5Mesh::CalculateWeightModelSpacePositions(MD5Skeleton* a_skeleton)
{
	for (unsigned int i = 0; i < m_weights.size(); ++i)
	{
		MD5Weight& weight = m_weights[i];
		MD5Joint* joint = a_skeleton->m_joints[weight.m_jointID];
		weight.m_modelSpacePosition = glm::vec4(joint->m_orientation * weight.m_boneSpacePosition, 0.f);
		weight.m_modelSpacePosition += glm::vec4(joint->m_position, 1.0);
	}
}

void MD5Mesh::CalculateVertexPositions(MD5Skeleton* a_skeleton)
{

	// Compute vertex positions
	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		
		//Grab our vertex and grab our weight index values
		MD5Vertex& vert = m_vertices[i];
		vert.position = glm::vec4(0);
		vert.normal = vert.tangent = glm::vec4(0);
		MD5WeightIndex& weightIndex = m_weightIndexes[i];

		// Sum the position of the weights
		for (int j = 0; j < weightIndex.countWeight; ++j)
		{
			//Get the actual weight and from this get the joint that weight effects
			MD5Weight& weight = m_weights[weightIndex.startWeight + j];
			MD5Joint* joint = a_skeleton->m_joints[weight.m_jointID];
			//calculate the vertex position with it's weighted values 
			glm::vec3 tempPos = ((joint->m_orientation * weight.m_boneSpacePosition) + joint->m_position) * weight.m_bias;
			vert.position += glm::vec4(tempPos , weight.m_bias);
			//as far as the format demonstrates only 4 bones can effect a vertex but just in case
			//use a limit here
			if (j < 4) //if j is greater than 4 just ignore this weight and bone value.
			{
				vert.weights[j] = weight.m_bias;
				vert.indices[j] = weight.m_jointID;
			}
		}

	}
}


void MD5Mesh::CalculateVertexNormals(MD5Skeleton* a_skeleton)
{
	
	// Loop through all triangles and calculate the face normal for triangle
	for (unsigned int i = 0; i < m_indices.size(); i+=3 )
	{
		glm::vec3 v1 = m_vertices[ m_indices[i+2]].position.xyz- m_vertices[m_indices[i]].position.xyz;
		glm::vec3 v2 = m_vertices[ m_indices[i+1]].position.xyz - m_vertices[m_indices[i]].position.xyz;

		glm::vec4 normal = glm::vec4(glm::cross(v2, v1), 0.f);

		m_vertices[m_indices[i  ]].normal += normal;
		m_vertices[m_indices[i+1]].normal += normal;
		m_vertices[m_indices[i+2]].normal += normal;

		//This is to calculate the tangent to the normal to be used in lighting 
		//and use of the normal map.
		//The maths used here is taken from Mathematics for 3D Games Programmeing and Computer Graphics by Eric Lengyel
		glm::vec2 uv0 = m_vertices[m_indices[i]].texCoord1;
		glm::vec2 uv1 = m_vertices[m_indices[i + 1]].texCoord1;
		glm::vec2 uv2 = m_vertices[m_indices[i + 2]].texCoord1;

		glm::vec2 uvVec1 = uv2 - uv0;
		glm::vec2 uvVec2 = uv1 - uv0;

		float coefficient = 1.0f / uvVec1.x * uvVec2.y - uvVec1.y * uvVec2.x;

		glm::vec4 tangent;
		tangent[0] = coefficient * (v2[0] * uvVec2[1] + v1[0] * -uvVec1[1]);
		tangent[1] = coefficient * (v2[1] * uvVec2[1] + v1[1] * -uvVec1[1]);
		tangent[2] = coefficient * (v2[2] * uvVec2[1] + v1[2] * -uvVec1[1]);
		tangent[3] = 0.f;

		m_vertices[m_indices[i]].tangent += tangent;
		m_vertices[m_indices[i + 1]].tangent += tangent;
		m_vertices[m_indices[i + 2]].tangent += tangent;

	}

	// for each vertex normalise the normal from the accumulated face normals
	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		MD5Vertex& vert = m_vertices[i];

		glm::vec4 normal = glm::normalize(vert.normal);
		glm::vec4 tangent = glm::normalize(vert.tangent);
		// As the normal is going to be calculated in local-joint space we need to reset this
		vert.normal = glm::vec4(0);
		vert.tangent = glm::vec4(0);

		// Put the bind-pose normal into joint-local space
		// so the animated normal can be computed faster later
		// need to get the weights start and count details, vertexes and weightindexes share same array index
		MD5WeightIndex& weightIndex = m_weightIndexes[i];

		for (int j = 0; j < weightIndex.countWeight; ++j)
		{
			//Get joint and weights to put normals back in to local joint space
			const MD5Weight& weight = m_weights[weightIndex.startWeight + j];
			const MD5Joint* joint = a_skeleton->m_joints[weight.m_jointID];
			vert.normal += (normal * glm::inverse(joint->m_orientation));
			vert.tangent += (tangent * glm::inverse(joint->m_orientation));
		}
	}
}

//utility function to test for the existance of a file at the provided filepath
bool file_exists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void MD5Material::LoadMaterials(std::string& a_textureBaseName, std::map<std::string, unsigned int>&	a_textures)
{
	//The texture name is the base name textures can have the following extensions to indicate their uses
	// name, name_d = diffuse texture
	// name_dis = disolve texture, this is for the dissolve effect found in doom 3
	// name_b, name_bmp = bump map
	// name_s = specular
	// name_local = normal map

	// some versions of MD5 models only have one texture listed as a shader if so 
	//just load that texture as the models diffuse texture
	if (a_textureBaseName.find(".tga") == std::string::npos)
	{
		//no extension present
		//loacte diffuse texture

		std::string diffusePath = a_textureBaseName;
		diffusePath += ".tga";
		if (!file_exists(diffusePath.c_str()))
		{
			diffusePath = a_textureBaseName;
			diffusePath += "_d.tga";
		}
		textureFilenames[DiffuseTexture] = diffusePath;

		std::string bump = a_textureBaseName;
		bump += "_bmp.tga";
		if (!file_exists(bump.c_str()))
		{
			bump = a_textureBaseName;
			bump += "_b.tga";
			if (file_exists(bump.c_str()))
			{
				textureFilenames[DisplacementTexture] = bump;
			}
		}
		else
		{
			textureFilenames[DisplacementTexture] = bump;
		}

		std::string specular = a_textureBaseName;
		specular += "_s.tga";
		if (file_exists(specular.c_str()))
		{
			textureFilenames[SpecularTexture] = specular;
		}

		std::string normal = a_textureBaseName;
		normal += "_local.tga";
		if (file_exists(normal.c_str()))
		{
			textureFilenames[NormalTexture] = normal;
		}
		
	
	}
	else
	{
		textureFilenames[DiffuseTexture] = a_textureBaseName;
	}

	for (int i = 0; i < TextureTypes_Count; ++i)
	{
		if (textureFilenames[i].size() > 0)
		{
			auto iter = a_textures.find(textureFilenames[i]);
			if (iter != a_textures.end())
			{
				textureIDs[i] = iter->second;
			}
			else
			{
				unsigned int textureID = SOIL_load_OGL_texture(textureFilenames[i].c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);

				if (textureID > 0)
				{
					textureIDs[i] = textureID;
					a_textures[textureFilenames[i]] = textureID;
				}
				else
				{
					std::cout << "Failed to load texture: " << textureFilenames[i] << std::endl;
				}
			}
		}
	}
}

//all animations are kepts separately from md5 mesh data in their own files. Load them in.
bool MD5Model::LoadAnims(unsigned int a_animationsCount, const char** a_animationFileNames)
{
	for (int i = 0; i < a_animationsCount; ++i)
	{
		std::fstream file;
		std::string line;
		//open the file for reading
		file.open(a_animationFileNames[i], std::ios_base::in);
		if (file.is_open())
		{
			//Get the size of the file to ensure this isn't just and empty file
			file.ignore(std::numeric_limits<std::streamsize>::max());
			std::streamsize fileLength = file.gcount();
			file.clear();   //  Since ignore will have set eof.
			file.seekg(0, std::ios_base::beg);

			if (fileLength == 0)
			{
				std::cout << "File contains no data!" << std::endl;
				return false;
			}
			//We have an animation file it is now time to load in the animation
			MD5Animation* anim = new MD5Animation();
			//Set our animations name to the filename we are loading minus the path and the extension
			std::string animName(a_animationFileNames[i]);
			if (animName.find_last_of("\\/") != std::string::npos)
			{
				animName = animName.substr(animName.find_last_of("\\/")+1, animName.size());
			}
			if (animName.find_last_of(".") != std::string::npos)
			{
				animName = animName.substr(0, animName.find_last_of("."));
			}
			anim->m_name = animName;
			//double check that we aren't trying to load the same animation twice
			auto iter = m_animations.find(anim->m_name);
			if (iter == m_animations.end())
			{
				std::string param;

				do
				{
					file >> param;

					if (param == "MD5Version")
					{
						file >> anim->m_MD5Version;
					}
					else if (param == "commandline")
					{
						//This is not important to us for rendering the file
						file.ignore(fileLength, '\n');
					}
					else if (param == "numFrames")
					{
						file >> anim->m_frameCount;
					}
					else if (param == "numJoints")
					{
						file >> anim->m_numJoints;
						//better allocate evenmore memory for a skeleton
						anim->m_baseKeyFrames = new MD5KeyFrame[anim->m_numJoints];
						anim->m_tracks.resize(anim->m_numJoints);
					}
					else if (param == "frameRate")
					{
						file >> anim->m_frameRate;
						anim->m_fAnimationDuration = (float)anim->m_frameCount / (float)anim->m_frameRate;
						anim->m_frameDuration = 1.f / (float)anim->m_frameRate;
					}
					else if (param == "numAnimatedComponents")
					{
						file >> anim->m_frameDataSize;
						
					}
					else if (param == "hierarchy")
					{
						//ignore everything to the end of the line then start processing bones
						file.ignore(fileLength, '\n');
						
						for (unsigned int i = 0; i < anim->m_numJoints; ++i)
						{
							//The hierarcy is the complete skeleton of the mesh
							//not all of the bones may be used in the animation 
							std::string boneName;
							int boneParentID;
							unsigned int boneAnimFlags;
							unsigned int boneStartFrame;

							file >> boneName >> boneParentID >> boneAnimFlags >> boneStartFrame;
							
							MD5Track track;
							//strip off bone name "" characters
							boneName.erase(std::remove(boneName.begin(), boneName.end(), '\"'), boneName.end());
							track.m_boneName = boneName;
							track.m_boneIndex = i;
							track.m_frameDataIndex = boneStartFrame;
							track.m_boneFrameDataFlags = boneAnimFlags;
							track.m_boneParentID = boneParentID;
							track.m_keyframes.resize(anim->m_frameCount);

							anim->m_tracks[i] = track;

							file.ignore(fileLength, '\n');
						}
						file.ignore(fileLength, '}');

					}
					else if (param == "bounds")
					{

					}
					else if (param == "baseframe")
					{
						file.ignore(fileLength, '\n');
						for (unsigned int i = 0; i < anim->m_numJoints; ++i)
						{
							file.ignore(fileLength, '(');
							glm::vec3 pos;
							glm::quat ori;
							file >> pos.x >> pos.y >> pos.z;
							file.ignore(fileLength, '(');
							file >> ori.x >> ori.y >> ori.z;

							//calculate the w component of the quaternion
							float t = 1.0f - (ori.x * ori.x) - (ori.y * ori.y) - (ori.z * ori.z);
							ori.w = (t < 0.0f) ? 0.0f : -sqrtf(t);

							anim->m_baseKeyFrames[i].m_scale = m_fScale;
							anim->m_baseKeyFrames[i].m_translation = pos;
							anim->m_baseKeyFrames[i].m_rotation = ori;

							file.ignore(fileLength, '\n');
						}
						file.ignore(fileLength, '}');
					}
					else if (param == "frame")
					{
						unsigned int frameIndex;
						file >> frameIndex;
						file.ignore(fileLength, '{');

						float* frameData = new float[anim->m_frameDataSize];
						memset(frameData, 0, sizeof(float) * anim->m_frameDataSize);
						for (int i = 0; i < anim->m_frameDataSize; ++i)
						{
							file >> frameData[i];
						}

						file.ignore(fileLength, '}');

						anim->BuildFrameSkeleton(frameIndex, frameData);
					}

				} while (!file.eof());
				file.close();

				m_animations[anim->m_name] = anim;
			}
		}
	}
}
//build a skeleton for this frame
//animations when loaded build a skeleton for each frame and store this in a track for each bone 
void MD5Animation::BuildFrameSkeleton(const unsigned int& a_frameNumber, const float* a_frameData)
{
	for (int i = 0; i < m_tracks.size(); ++i)
	{
		//get the animation track we are about to alter
		MD5Track& track = m_tracks[i];
		MD5KeyFrame& keyFrame = track.m_keyframes[a_frameNumber];

		//get the base key frame for the bone we are going to reference
		const MD5KeyFrame& baseKeyFrame = m_baseKeyFrames[track.m_boneIndex];
		keyFrame.m_rotation = baseKeyFrame.m_rotation;
		keyFrame.m_translation = baseKeyFrame.m_translation;
		keyFrame.m_scale = baseKeyFrame.m_scale;


		unsigned int frameDataIndex = track.m_frameDataIndex;

		if (track.m_boneFrameDataFlags & 1) // Pos.x
		{
			keyFrame.m_translation.x = a_frameData[frameDataIndex];
			++frameDataIndex;
		}
		if (track.m_boneFrameDataFlags & 2) // Pos.y
		{
			keyFrame.m_translation.y = a_frameData[frameDataIndex];
			++frameDataIndex;
		}
		if (track.m_boneFrameDataFlags & 4) // Pos.x
		{
			keyFrame.m_translation.z = a_frameData[frameDataIndex];
			++frameDataIndex;
		}
		if (track.m_boneFrameDataFlags & 8) // Orient.x
		{
			keyFrame.m_rotation.x = a_frameData[frameDataIndex];
			++frameDataIndex;
		}
		if (track.m_boneFrameDataFlags & 16) // Orient.y
		{
			keyFrame.m_rotation.y = a_frameData[frameDataIndex];
			++frameDataIndex;
		}
		if (track.m_boneFrameDataFlags & 32) // Orient.z
		{
			keyFrame.m_rotation.z = a_frameData[frameDataIndex];
			++frameDataIndex;
		}
		//The orientation and translation data for this key frame are compelte
		//calculate the w component and convert this bones orientation to suit our OpenGL 
		float t = 1.0f - (keyFrame.m_rotation.x * keyFrame.m_rotation.x) - (keyFrame.m_rotation.y * keyFrame.m_rotation.y) - (keyFrame.m_rotation.z * keyFrame.m_rotation.z);
		keyFrame.m_rotation.w = (t < 0.0f) ? 0.0f : -sqrtf(t);

		keyFrame.m_translation *= keyFrame.m_scale;

		if (track.m_boneParentID == -1)
		{
			//y is up scene, with z being forward.
			//MD5 format has Z axis stored as up and X axis stored as forwards the following converts from z Up to y up and from x forward to z forward
			glm::quat rotateToYUp = glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
			glm::quat rotateXToZ = glm::angleAxis(glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
			keyFrame.m_rotation = rotateToYUp * keyFrame.m_rotation;
			keyFrame.m_rotation = rotateXToZ * keyFrame.m_rotation;
			
			keyFrame.m_translation = rotateToYUp * keyFrame.m_translation;
			keyFrame.m_translation = rotateXToZ * keyFrame.m_translation;
		}
			//The key frames orientation and position are now kept in local space ??
			//NOTE: we assume that this joint's parent has already been calculated
			//due to the order of traversal of this array of tracks

		// Do we have a parent bone?
		if (track.m_boneParentID >= 0)
		{
			MD5Track& parentTrack = m_tracks[track.m_boneParentID];
			MD5KeyFrame& parentFrame = parentTrack.m_keyframes[a_frameNumber];
			glm::vec3 rotPos = parentFrame.m_rotation * keyFrame.m_translation;

			keyFrame.m_translation = parentFrame.m_translation + rotPos;
			keyFrame.m_rotation = parentFrame.m_rotation * keyFrame.m_rotation;
			keyFrame.m_rotation = glm::normalize(keyFrame.m_rotation);
		}


	}
}

//this function evaluates the animation at a given point in time and if between frames interpolates 
//between them
void MD5Model::evaluateAnimation(const char* a_animationName, float a_time, bool a_loop)
{
	MD5Animation* anim = m_animations[a_animationName];
	if (anim != nullptr)
	{
		int totalFrames = anim->m_frameCount;
		float animDuration = anim->totalTime();

		// get time through frame
		float animTime = 0;
		if (a_loop)
			animTime = glm::max(glm::mod(a_time, animDuration), 0.0f);
		else
			animTime = glm::min(glm::max(a_time, 0.0f), animDuration);

		unsigned int frameIndex = (unsigned int)(floor((animTime / animDuration) * (totalFrames))) % (totalFrames-1);
		unsigned int nextFrameIndex = frameIndex;
		if (totalFrames > 1)
		{
			nextFrameIndex = (frameIndex + 1) % (totalFrames - 1);
		}
		
		float fInterValue = fmodf(animTime, anim->m_frameDuration) / anim->m_frameDuration;


		const MD5Track* track = nullptr;
		const MD5KeyFrame* startFrame = nullptr;
		const MD5KeyFrame* endFrame = nullptr;

		for (unsigned int i = 0; i < anim->m_tracks.size(); ++i)
		{
			track = &(anim->m_tracks[i]);
			startFrame = &(track->m_keyframes[frameIndex]);
			endFrame = &(track->m_keyframes[nextFrameIndex]);

			// translation
			glm::vec3 T = glm::mix(startFrame->m_translation, endFrame->m_translation, fInterValue);

			// scale
			glm::vec3 S = glm::vec3(glm::mix(startFrame->m_scale, endFrame->m_scale, fInterValue));

			// rotation (quaternion slerp)
			glm::quat R = glm::normalize(glm::slerp(startFrame->m_rotation, endFrame->m_rotation, fInterValue));

			// build matrix
			glm::mat4 mRot = glm::mat4_cast(R);
			glm::mat4 mScale = glm::scale(S);
			glm::mat4 mTranslate = glm::translate(T);
			m_skeleton->m_joints[track->m_boneIndex]->m_orientation = R;
			m_skeleton->m_joints[track->m_boneIndex]->m_position = T;
			m_skeleton->m_bones[track->m_boneIndex] = mTranslate * mRot;
			
		}

	}

}
//this is called to update the bones into locall space by multiplying them by their inverse bind pose position
void MD5Model::UpdateBones()
{
	for (int i = 0; i < m_skeleton->m_jointCount; ++i)
	{
		m_skeleton->m_bones[i] = m_skeleton->m_bones[i] * m_skeleton->m_bindPoses[i];
	}
}
