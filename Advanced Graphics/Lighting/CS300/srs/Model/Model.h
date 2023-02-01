///
// Brief description: Class we use to charge the model 
///
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>

#include <GL/glew.h>

#include "Textures/Texture.h"

class Model
{
public:
	enum Forms
	{
		PLANE,
		CUBE,
		CONE,
		CYLINDER,
		SPHERE,
		NONE
	};

	Model();
	~Model();

	void Initialize(Model::Forms form);

	void ChangeModel(Model::Forms form, int precision = 20, int Stack = 10 , float radius = 0.5f);

	GLuint GetModelVao() const;
	GLuint GetNormalsVao() const;
	GLuint GetAverageNormalsVao() const;
	int GetPrecision() const;
	Forms GetForm() const;

	std::vector<unsigned int> mModelIndices;
	std::vector<float> mModelNormals;
	std::vector<float> mModelAverageNormals;

	bool mShowNormal;
	bool mShowAverageNormal;
	bool mShowAxis;
	bool mTextured;

	MyTextures mTexture;

	bool Deleted;
	void ClearLights();
	void LightModel();
private:
	std::vector<float> mModel;
	std::vector<float> mModelTextCoord;

	std::vector<glm::vec3> mModelVertexNormal;
	std::vector<glm::vec3> mNormal;
	std::vector<glm::vec3> mAveNormal;
	std::vector<std::vector<glm::vec3>> mModelVertexAveNormal;

	void MakeCone();
	void MakeCylinder();
	void MakeSphere();

	void MakeNormals();
	inline void PushPointToModel(float x, float y, float z, float w = 1.0f);

	//Some Model data
	int mStack;
	float mRadius;
	float mHeight;
	int mPrecision;

	Forms mForm;

	std::vector<float> mRed;

	void Clear();

	//VAO
	GLuint vao;
	GLuint mNormalVao;
	GLuint mAverageNormalVao;
	GLuint mAxisVao;

	GLuint EBO;

	//VBO
	GLuint positionBufferObject;
	GLuint mTextureBufferObject;
	GLuint mNormalBufferObject;
	GLuint mAverageNormalBufferObject;
	GLuint mNormals;
	GLuint mAverageNormals;
	GLuint mAxisPoints;
};