///
// Brief description: Class we will use to store the data of the light 
///
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <GL/glew.h>

class GameObject;
class GLSLProgram;

struct LightSourceParameters
{
	int mLightType;

	glm::vec4 mAmbientColor;
	glm::vec4 mDiffuseColor;
	glm::vec4 mSpecularColor;
	glm::vec3 mPosition;
	glm::vec3 mSpotDirection;

	//ambient intensiti is mAmbientStreng * mAmbientColor, Iambient = IaK
	float mAmbientStreng; //from 0 to 1

	float mSpotExponent;  // falloff
	float mCutOff;
	float mOuterCutOff;

	float mConstantAttenuation;
	float mLinearAttenuation;
	float mQuadraticAttenuation;
};

class Light
{
public:
	static Light * GetInstance()
	{
		static Light mInstance;

		return &mInstance;
	}

	enum LightType
	{
		PointLight,
		SpotLight,
		DirectionalLight
	};


	LightSourceParameters mLight;

	void Update();
	void DisplayLights();

	void Input(SDL_Scancode & event);

	std::vector<GameObject * > mLights;
private:
	Light();
	~Light();
	void ClearLights();


	GLSLProgram * mLightSadher;
	//void LightsShaderInit();
	void CreateLight();

	float mRadius;
	float mAngle;

	bool mStop;

	LightType mType;

	void CreateModel();
	//VAO
	GLuint vao;
	//EBO
	GLuint EBO;
	//VBO
	GLuint positionBufferObject;

	//model data
	std::vector<float> mModel;
	std::vector<float> mModelTextCoord;
	std::vector<unsigned int> mModelIndices;

	inline void GetError();
};
