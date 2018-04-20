#pragma once

#ifndef __MD5MODEL_H__
#define __MD5MODEL_H__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <map>
#include <vector>
#include <string>

#ifndef MAX_PATH
	#define MAX_PATH 1024
#endif

// A complete vertex structure with all the data needed from the MD5 file
class MD5Vertex
{
public:

	enum VertexAttributeFlags
	{
		POSITION	= (1 << 0), //The Position of the Vertex
		NORMAL		= (1 << 1), //The Normal for the Vertex
		TNAGENT		= (1 << 2), //The Tangent for the normal vector
		INDICES		= (1 << 3), //The Bones joint ID, limit to 4 bones per vertex
		WEIGHTS		= (1 << 4), //The Bones weight (This is position in the xyz component and the bias in the w component)
		TEXCOORD1	= (1 << 5), //The UV Coordinates for the mesh
	};

	enum Offsets
	{
		PositionOffset = 0,
		NormalOffset = PositionOffset + sizeof(glm::vec4),
		TangentOffset = NormalOffset + sizeof(glm::vec4),
		IndicesOffset = TangentOffset + sizeof(glm::vec4),
		WeightsOffset = IndicesOffset + sizeof(glm::vec4),
		TexCoord1Offset = WeightsOffset + sizeof(glm::vec4),
	};

	MD5Vertex();
	~MD5Vertex();

	glm::vec4	position;
	glm::vec4	normal;
	glm::vec4	tangent;
	glm::vec4	indices;
	glm::vec4	weights;
	glm::vec2	texCoord1;
	
	bool operator == (const MD5Vertex& a_rhs) const;
	bool operator < (const MD5Vertex& a_rhs) const;

};


// A single frame for a bone in an animation
class MD5KeyFrame
{
public:

	MD5KeyFrame();
	~MD5KeyFrame();

	float			m_scale;
	glm::quat		m_rotation;
	glm::vec3		m_translation;
};

// A collection of frames for a single bone in an animation
class MD5Track
{
public:

	MD5Track();
	~MD5Track();

	unsigned int	m_boneIndex;
	unsigned int	m_boneFrameDataFlags;
	unsigned int	m_frameDataIndex;
	int				m_boneParentID;

	std::string		m_boneName;
	std::vector<MD5KeyFrame>	m_keyframes;
};

// An animation that contains a collection of animated bone tracks
class MD5Animation
{
public:

	MD5Animation();
	~MD5Animation();

	void BuildFrameSkeleton(const unsigned int& a_frameNumber, const float* a_frameData);

	unsigned int	totalFrames() const;
	float			totalTime() const;
	float			frameTime() const;

	std::string		m_name;
	unsigned int	m_numJoints;
	unsigned int	m_MD5Version;
	unsigned int	m_frameRate;
	unsigned int	m_frameCount;
	unsigned int	m_frameDataSize;

	float			m_frameDuration;
	float			m_fAnimationDuration;

	std::vector<MD5Track>		m_tracks;
	//MD5 models have a base pose for the animation this is not the bind pose
	//we will store this as a collection of key frames for our base pose
	//this is beacuse not all bones will animate this stops us from storing key frames
	//for bones that do not move during an animation
	MD5KeyFrame*	m_baseKeyFrames;

};

//MD5Joint a single bone in the Skeleton
class MD5Joint
{
public:

	MD5Joint();
	~MD5Joint();

	std::string m_name; //the bones name
	int m_parentID;		//the bones parent id, this is an array index -1 if no parent
	glm::vec3 m_position; //the bones transform
	glm::quat m_orientation;//the orientation of the bone


};

//every MD5 model has only one skeleton
//MD5 models can have multiple mesh data but only one skeleton
class MD5Skeleton
{
public:
	MD5Skeleton();
	~MD5Skeleton();

	MD5Joint*		getBoneByIndex(const unsigned int& a_index) {};
	MD5Joint*		getJointByName(const char* a_name) {};

	unsigned int	m_jointCount;
	MD5Joint**		m_joints;

	glm::mat4*		m_bones;	// ready for use in skinning! (bind pose combined)
	glm::mat4*		m_bindPoses;//bind pose data


};

//a small structure to hold weight index for the starting weight that effects a joint and
//a counter that indicates how many weights effect this vertex position
struct MD5WeightIndex {
	int startWeight;
	int countWeight;
};

//a structure for the MD5 weight this weight effects a joint, has a bias and has it's own pos/rot in space
struct MD5Weight
{
	int m_jointID;
	float m_bias;
	glm::vec3 m_boneSpacePosition;
	glm::vec4 m_modelSpacePosition;
};

// A material/texture holder for the MD5 model 
struct MD5Material
{
	enum TextureTypes
	{
		DiffuseTexture = 0,
		SpecularTexture,
		NormalTexture,
		DisplacementTexture,

		TextureTypes_Count
	};

	MD5Material() {};
	~MD5Material() {};

	void LoadMaterials(std::string& a_baseTextureName, std::map<std::string, unsigned int>&	a_textures);

	std::string		textureFilenames[TextureTypes_Count];		// Filename
	unsigned int	textureIDs[TextureTypes_Count];				// OpenGL texture handle
};


//an MD5 Mesh class, each mesh has vertex and material data
class MD5Mesh
{
public:

	MD5Mesh();
	virtual ~MD5Mesh();



	unsigned int				m_vertexAttributes;
	MD5Material					m_material;
	std::vector<unsigned int>	m_textureIds;
	std::vector<MD5Vertex>		m_vertices;
	std::vector<MD5WeightIndex>	m_weightIndexes;
	std::vector<MD5Weight>		m_weights;
	std::vector<unsigned int>	m_indices;

	void CalculateWeightModelSpacePositions(MD5Skeleton* a_skeleton);
	void CalculateVertexPositions(MD5Skeleton* a_skeleton);
	void CalculateVertexNormals(MD5Skeleton* a_skeleton);
	
};

//the overall MD5 Model class, this has a skelton, may have multiple meshes and animations
class MD5Model
{

public:
	MD5Model();
	virtual ~MD5Model();

	bool Load(const char* a_filename, float a_fScale);
	bool LoadAnims(unsigned int a_animationsCount, const char** a_animationFileNames);
	void Unload() {};

	unsigned int getMeshCount() { return m_meshes.size(); }
	void evaluateAnimation(const char* a_animationName, float a_time, bool a_loop);
	void UpdateBones();

	MD5Skeleton* m_skeleton;
	std::vector<MD5Mesh*> m_meshes;
	std::map<std::string, unsigned int>	m_textures;
	std::map<std::string, MD5Animation*>	m_animations;

private:
	int	m_iMD5Version;
	int m_iNumJoints;
	int m_iNumMeshes;
	bool m_bHasAnimation;
	float m_fScale;
	glm::mat4 m_globalTransform;

};

//\=========================================================================================================================================
//\ Iniline Functions
//\=========================================================================================================================================
//MD5Vertex
inline MD5Vertex::MD5Vertex() : position(0, 0, 0, 1), normal(0, 0, 0, 0), tangent(0.f,0.f,0.f, 0.f), indices(0, 0, 0, 0), weights(0, 0, 0, 0),	texCoord1(0, 0) {}
inline MD5Vertex::~MD5Vertex(){}

inline bool MD5Vertex::operator == (const MD5Vertex& a_rhs) const
{
	return memcmp(this, &a_rhs, sizeof(MD5Vertex)) == 0;
}

inline bool MD5Vertex::operator < (const MD5Vertex& a_rhs) const
{
	return memcmp(this, &a_rhs, sizeof(MD5Vertex)) < 0;
}
//End - MD5Vertex

//MD5Mesh
inline MD5Mesh::MD5Mesh() : m_vertexAttributes(0)
{
}

inline MD5Mesh::~MD5Mesh()
{

}
//End - MD5Mesh

//MD5Joint
inline MD5Joint::MD5Joint() : m_parentID(0){}
inline MD5Joint::~MD5Joint() {}
//End - MD5 joint

//MD5Joint
inline MD5Skeleton::MD5Skeleton() : m_jointCount(0) {}
inline MD5Skeleton::~MD5Skeleton() 
{
	delete[] m_bones;
	delete[] m_bindPoses;
	delete[] m_joints;
}
//End - MD5 joint

//MD5KeyFrame
inline MD5KeyFrame::MD5KeyFrame() : m_scale(1.0f), m_rotation(), m_translation() {}
inline MD5KeyFrame::~MD5KeyFrame() {}
//End MD5KeyFrame

//MD5Track
inline MD5Track::MD5Track() : m_boneIndex(0), m_frameDataIndex(0) {}
inline MD5Track::~MD5Track() {
	m_keyframes.clear();
}

//End - MD5Track


//MD5Animation
inline MD5Animation::MD5Animation() : m_MD5Version(0), m_frameRate(0), m_frameCount(0), m_frameDataSize(0) {}
inline MD5Animation::~MD5Animation() {
	m_tracks.clear();
}
inline unsigned int	MD5Animation::totalFrames() const {	return m_frameCount; }
inline float MD5Animation::totalTime() const { return m_fAnimationDuration; }
inline float MD5Animation::frameTime() const { return m_frameDuration; }
//End - MD5Animation
#endif //__MD5MODEL
