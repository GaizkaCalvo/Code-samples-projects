#include "Light.h"

#include "Camera/Camera.h"
#include "Window/Window.h"
#include "Shader/GLSLProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameObjects/GameObject.h"

///
// Brief description: Some Easy forms we will compute by hand
///
namespace LightForms
{

	std::vector<float> Cube = {
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//0 
	 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//1
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//2
	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//3
													 //
	-0.5f, -0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//4
	 0.5f, -0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//5
	 0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//6
	-0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//7
													 //
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//8--
	 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//9--
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//10--
	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//11--
													 //
	-0.5f, -0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//12--
	 0.5f, -0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//13--
	 0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//14--
	-0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//15--
													 //
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//16--
	 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//17--
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//18--
	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//19--
													 //
	-0.5f, -0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//20--
	 0.5f, -0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//21
	 0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//22--
	-0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//23--
	};

	std::vector<float> CubeTextCoord = {
		1.0f, 1.0f,   //0 
		1.0f, 0.0f,   // 1
		0.0f, 0.0f,   // 2
		0.0f, 1.0f,   // 3

		1.0f, 0.0f,   // 4
		0.0f, 0.0f,   // 5
		0.0f, 1.0f,   // 6
		1.0f, 1.0f,   // 7

		1.0f, 0.0f,   // 8
		1.0f, 0.0f,   // 9
		1.0f, 1.0f,   // 10
		1.0f, 1.0f,   // 11

		0.0f, 0.0f,   // 12
		1.0f, 0.0f,   // 13
		1.0f, 0.0f,   // 14
		0.0f, 1.0f,   // 15

		0.0f, 0.0f,   // 16
		0.0f, 0.0f,   // 17
		0.0f, 1.0f,   // 18
		0.0f, 1.0f,   // 19

		0.0f, 1.0f,   // 20
		1.0f, 1.0f,   // 21
		1.0f, 1.0f,   // 22
		0.0f, 0.0f    // 23
	};

	std::vector<unsigned int> CubeIndices = {
		0, 1, 2,
		2, 3, 0,
		7, 6, 5,
		5, 4, 7,
		15,12, 8,
		15,8,11,
		14,23,19,
		14,19,10,
		18,17,13,
		18,13,22,
		9,16,20,
		9,20,21
	};
};

Light::Light() : mAngle(0.0f), mRadius(20.0f), mStop(false)
{
	mLight.mAmbientColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mLight.mDiffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mLight.mSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mLight.mAmbientStreng = 0.2f;

	mLight.mConstantAttenuation = 0.0f;
	mLight.mLinearAttenuation = 0.0f;
	mLight.mQuadraticAttenuation = 0.001f;

	mLight.mCutOff = glm::cos(glm::radians(10.0f));
	mLight.mOuterCutOff = glm::cos(glm::radians(30.0f));
	mLight.mSpotExponent = 1.0f;

	//object that will represent the ligts that we got
	GameObject * mLightObject = new GameObject("Light", glm::vec3{ 3.0f, 0.0f , 0.0f },
									glm::vec3{ 0.6f, 0.6f , 0.6f }, Model::CUBE);

	//we set the actuall position of the object
	mLight.mPosition = mLightObject->mTransform.mPosition;
	mLights.push_back(mLightObject);

	CreateModel();

	//shader we will use to create the light 
	mLightSadher = GLSLProgram::CreateShaderProgram("srs/Shaders/lightsvertex.vs", "srs/Shaders/lightsfragment.fs");
}

Light::~Light()
{
	ClearLights();

	// Delete the VAO
	glDeleteVertexArrays(1, &vao);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Delete the VBOs
	glDeleteBuffers(1, &positionBufferObject);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();



	//Clear vectors
	mModel.clear();
	mModelIndices.clear();
	mModelTextCoord.clear();
}

void Light::ClearLights()
{
	size_t size = mLights.size();
	for (size_t i = 0; i < size; i++)
	{
		delete mLights[i];
	}
	mLights.clear();
}

void Light::CreateLight()
{
	size_t size = mLights.size();

	if (size == 8)
		return;

	//object that will represent the ligts that we got
	GameObject * mLightObject = new GameObject("Light", glm::vec3{ 3.0f, 0.0f , 0.0f },
		glm::vec3{ 0.6f, 0.6f , 0.6f });

	size++;
	//we set the actuall position of the object
	mLight.mPosition = mLightObject->mTransform.mPosition;
	//push the object
	mLights.push_back(mLightObject);

	float AngleDiff = 2 * glm::pi<float>() / size;
	for (size_t i = 0; i < size; i++)
	{
		mLights[i]->mTransform.mPosition.x = sin(AngleDiff * i + mAngle) * mRadius;
		mLights[i]->mTransform.mPosition.y = 10 * sin(AngleDiff * i + 2 * mAngle);
		mLights[i]->mTransform.mPosition.z = cos(AngleDiff * i + mAngle) * mRadius;
	}

}

void Light::CreateModel()
{
	mModel = LightForms::Cube;
	mModelIndices = LightForms::CubeIndices;
	mModelTextCoord = LightForms::CubeTextCoord;

	// VAO, 
	glGenVertexArrays(1, &vao);
	GetError();
	// VBO
	glGenBuffers(1, &positionBufferObject);
	GetError();
	//EBO
	glGenBuffers(1, &EBO);
	GetError();
	glBindVertexArray(vao);
	GetError();
	//we bind the VBO to use it with the vertices that we need
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	GetError();
	//here we put the vertices in the VBO, and as its binded we dont need to specify anything
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModel.size(), &mModel[0], GL_STATIC_DRAW);
	GetError();

	//put the vbo into the vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mModelIndices.size(), &mModelIndices[0], GL_STATIC_DRAW);
	GetError();

	// Insert the VBO into the VAO
	// position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	GetError();
	//// color attribute
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GetError();
}

inline void Light::GetError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();
}

///
// Brief description: We create the shader that we will use for our lights
///
//void Light::LightsShaderInit()
//{
//	//shader we will use to create the light 
//	mLightSadher = GLSLProgram::CreateShaderProgram("srs/Shaders/lightsvertex.vs", "srs/Shaders/lightsfragment.fs");
//
//	//we set the actuall position of the object
//	mLight.mPosition = mLightObject->mTransform.mPosition;
//
//	mLightObject->mMod.LightModel();
//}

///
// Brief description: We display the lights that we got using the shader that we created
///
void Light::DisplayLights()
{
	size_t size = mLights.size();
	if (size == 0)
		return;

	MyCamera * mCamera = MyCamera::GetInstance();
	MyWindow * mWindow = MyWindow::GetInstance();

	GLSLProgram * Shader = mLightSadher;
	Shader->Use();

	for (size_t i = 0; i < size; i++)
	{
		//UNIFORMS WE WILL GIVE TO THE SHADER
		glm::mat4 transform = mLights[i]->mTransform.GetModelToWorld();
		//fov, aspec ratio, near, far
		glm::mat4 perspective = mWindow->GetPerspectiveMatrix();
		//camera matrix
		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = mCamera->GetViewMatrix();

		Shader->SetUniform("ModelMat", transform);
		Shader->SetUniform("PerspectiveMat", perspective);
		Shader->SetUniform("ViewMat", view);
		//check error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			abort();

		//we then use the vao and we draw the object
		glBindVertexArray(vao);
		//check error
		error = glGetError();
		if (error != GL_NO_ERROR)
			abort();
		//draw
		glDrawElements(GL_TRIANGLES,
			static_cast<GLsizei>(mModelIndices.size())
			, GL_UNSIGNED_INT, 0);
		//check error
		error = glGetError();
		if (error != GL_NO_ERROR)
			abort();

		// Unbind
		glBindVertexArray(0);
	}
	//we dont use the program anymore
	glUseProgram(0);
}

///
// Brief description: Update the position of the lights 
///
void Light::Update()
{
	if (mStop)
		return;

	size_t size = mLights.size();
	float AngleDiff = 2 * glm::pi<float>() / size;
	for (size_t i = 0; i < size; i++)
	{
		mLight.mPosition = mLights[i]->mTransform.mPosition;

		mLights[i]->mTransform.mPosition.x = sin(AngleDiff * i + mAngle) * mRadius;
		mLights[i]->mTransform.mPosition.y = 10 * sin(AngleDiff * i + 2 * mAngle);
		mLights[i]->mTransform.mPosition.z = cos(AngleDiff * i + mAngle) * mRadius;
	}

	mAngle += 0.005f;
}

///
// Brief description: Inputs that will change some lights parameters
///
void Light::Input(SDL_Scancode & event)
{
	if (event == SDL_SCANCODE_P)
		mStop = !mStop;
	if (event == SDL_SCANCODE_KP_7 || event == SDL_SCANCODE_7)
	{
		mType = Light::PointLight;
		mLight.mLightType = 0;
	}
	if (event == SDL_SCANCODE_KP_8 || event == SDL_SCANCODE_8)
	{
		mType = Light::SpotLight;
		mLight.mLightType = 2;
	}
	if (event == SDL_SCANCODE_KP_9 || event == SDL_SCANCODE_9)
	{
		mType = Light::DirectionalLight;
		mLight.mLightType = 1;
	}
	if (event == SDL_SCANCODE_L)
	{
		CreateLight();
	}
	if (event == SDL_SCANCODE_R)
	{
		ClearLights();
	}
}
