#include "Window.h"

#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>

#include "Light/Light.h"
#include "Camera/Camera.h"
#include "Shader/GLSLProgram.h"
#include "GameObjects/ObjectManager.h"
#include "GameObjects/Transform/Transform.h"

///
// Brief description: Put the model that the object will use
///
void MyWindow::Display()
{
	ShadowDisplay();

	glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//we set the color of the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//we clear the buffer that we indicate in the flag 
	//clear the depth buffer 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//check error
	CheckError();

	//some things we will need 
	ObjectManager * ObjgMng = ObjectManager::GetInstance();
	MyCamera * mCamera = MyCamera::GetInstance();
	Light * mLight = Light::GetInstance();
	GLSLProgram * Shader;

	mLight->DisplayLights();

	size_t size = static_cast<size_t>(ObjgMng->mGameObjects.size());
	for (size_t i = 0; i < size; i++)
	{

		if (ObjgMng->mGameObjects[i]->mMod.mTextured)
		{
			Shader = mTexturedShaderProgram;
			mTexturedShaderProgram->Use();

			glActiveTexture(GL_TEXTURE1);
			CheckError();

			glBindTexture(GL_TEXTURE_2D, ObjgMng->mGameObjects[i]->mMod.mTexture.mGeneralTextures);
			CheckError();

			mTexturedShaderProgram->SetUniform("textureData1", 1);

#if TANBIACTIVE
			//2nd tecture
			glActiveTexture(GL_TEXTURE2);
			CheckError();

			glBindTexture(GL_TEXTURE_2D, ObjgMng->mGameObjects[i]->mMod.mTexture.mNormalMap);
			CheckError();
			mTexturedShaderProgram->SetUniform("textureData2", 2);
			CheckError();

			mTexturedShaderProgram->SetUniform("Mode", NormalMode);
#endif
		}
		else// Bind the glsl program and this object's VAO
		{
			Shader = mShaderProgram;
			mShaderProgram->Use();
		}


		Shader->SetUniform("shadowMap", 0);
		//lightspacematrix
		glm::mat4 lightProjection = glm::perspective(glm::radians(60.0f), static_cast<GLfloat>(SHADOW_WIDTH / SHADOW_HEIGHT), mNear, mFar);
		//light view
		glm::mat4 lightView = glm::lookAt(mLight->mLights[0]->mTransform.mPosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		Shader->SetUniform("lightSpaceMatrix", lightSpaceMatrix);

		//UNIFORMS WE WILL GIVE TO THE SHADER
		glm::mat4 transform = ObjgMng->mGameObjects[i]->mTransform.GetModelToWorld();
		//fov, aspec ratio, near, far
		glm::mat4 perspective = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(mWidth / mHeight), mNear, mFar);
		//camera matrix
		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = mCamera->GetViewMatrix();

		Shader->SetUniform("ModelMat", transform);
		Shader->SetUniform("PerspectiveMat", perspective);
		Shader->SetUniform("ViewMat", view);
		CheckError();

		if (ObjgMng->mGameObjects[i]->mMod.mTextured)
		{
			if (mLight->mLights.size() != 0)
			{
				//for the specular
				Shader->SetUniform("viewPos", mCamera->mCameraPos);
				CheckError();

				PassLightParameters(Shader, *mLight, *ObjgMng->mGameObjects[i]);
			}
			else
			{
				Shader->SetUniform("LightAmount", -1);
				CheckError();
			}
		}

		//we then use the vao and we draw the object
		glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetModelVao());
		CheckError();
		//draw
		glDrawElements(GL_TRIANGLES,
			static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelIndices.size())
						, GL_UNSIGNED_INT, 0);
		CheckError();

		// Unbind
		glBindVertexArray(0);
		//we dont use the program anymore
		glUseProgram(0);

		if (ObjgMng->mGameObjects[i]->mMod.mShowNormal)
		{
			mNormalShaderProgram->Use();

			mNormalShaderProgram->SetUniform("ModelMat", transform);
			mNormalShaderProgram->SetUniform("PerspectiveMat", perspective);
			mNormalShaderProgram->SetUniform("ViewMat", view);

			if (ObjgMng->mGameObjects[i]->mMod.mShowAverageNormal)
			{
				//avenormal
				glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetAverageNormalsVao());
				CheckError();

				mNormalShaderProgram->SetUniform("Color", glm::vec3(0.0f, 0.0f, 1.0f));
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelAverageNormals.size()));
				CheckError();
				//unbind
				glBindVertexArray(0);

#if TANBIACTIVE
				//avetangents
				mNormalShaderProgram->SetUniform("Color", glm::vec3(1.0f, 0.0f, 0.0f));
				glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetAverageTanVao());
				CheckError();
				// Draw normals
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelAveTan.size()));
				CheckError();
				//unbind
				glBindVertexArray(0);
				
				//avebitangents
				mNormalShaderProgram->SetUniform("Color", glm::vec3(0.0f, 1.0f, 0.0f));
				glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetAverageBiTanVao());
				CheckError();
				// Draw normals
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelAveBiTan.size()));
				CheckError();
#endif

				//unbind
				glBindVertexArray(0);
			}
			else
			{
				mNormalShaderProgram->SetUniform("Color", glm::vec3(0.0f, 0.0f,1.0f));
				glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetNormalsVao());
				CheckError();
				// Draw normals
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelNormals.size()));
				CheckError();
				//unbind
				glBindVertexArray(0);


#if TANBIACTIVE
				//tangents
				mNormalShaderProgram->SetUniform("Color", glm::vec3(1.0f, 0.0f, 0.0f));
				glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetTangentsVao());
				CheckError();
				// Draw normals
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelTan.size()));
				CheckError();
				//unbind
				glBindVertexArray(0);

				//bitangents
				mNormalShaderProgram->SetUniform("Color", glm::vec3(0.0f, 1.0f, 0.0f));
				glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetBiTangentsVao());
				CheckError();
				// Draw normals
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelBiTan.size()));
				CheckError();
#endif

				//unbind
				glBindVertexArray(0);
			}
			//unbind
			glBindVertexArray(0);
			//we dont use the program anymore
			glUseProgram(0);
		}
	}

	ShadowDebugDisplay();

	SDL_GL_SwapWindow(mWindow);
}

void MyWindow::ShadowDisplay()
{
	// 1. first render to depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// Bind the frame buffer containing the shadow map
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//
	glClear(GL_DEPTH_BUFFER_BIT);
	// Select the fragment shader subroutine to record the depth
	// Enable front-face culling
	glCullFace(GL_FRONT);

	//we set the color of the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//we clear the buffer that we indicate in the flag 
	//clear the depth buffer 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//check error
	CheckError();

	//some things we will need 
	ObjectManager * ObjgMng = ObjectManager::GetInstance();
	MyCamera * mCamera = MyCamera::GetInstance();
	Light * mLight = Light::GetInstance();

	//shader that we will use
	mShadowShaderProgram->Use();

	size_t size = static_cast<size_t>(ObjgMng->mGameObjects.size());
	for (size_t i = 0; i < size; i++)
	{

		//UNIFORMS WE WILL GIVE TO THE SHADER
		glm::mat4 transform = ObjgMng->mGameObjects[i]->mTransform.GetModelToWorld();
		//fov, aspec ratio, near, far, respect to the light
		glm::mat4 lightProjection = glm::perspective(glm::radians(60.0f), static_cast<GLfloat>(SHADOW_WIDTH / SHADOW_HEIGHT), mNear, mFar);
		//light view
		glm::mat4 lightView = glm::lookAt(mLight->mLights[0]->mTransform.mPosition ,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		mShadowShaderProgram->SetUniform("model", transform);
		CheckError();
		mShadowShaderProgram->SetUniform("lightSpaceMatrix", lightSpaceMatrix);
		CheckError();

		//we then use the vao and we draw the object
		glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetModelVao());
		CheckError();
		//draw
		glDrawElements(GL_TRIANGLES,
			static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelIndices.size())
			, GL_UNSIGNED_INT, 0);
		CheckError();

		// Unbind
		glBindVertexArray(0);
	}

	//we dont use the program anymore
	glUseProgram(0);

	//quit
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// 2. then render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
}

void MyWindow::ShadowDebugDisplay()
{
	glViewport((GLsizei)-SHADOW_WIDTH /10.5f, (GLsizei)-SHADOW_HEIGHT /10.5f, (GLsizei)SHADOW_WIDTH /2.5f, (GLsizei)SHADOW_HEIGHT /2.5f);

	// render Depth map to quad for visual debugging
	mDebugShadowShaderProgram->Use();
	mDebugShadowShaderProgram->SetUniform("near_plane", mNear);
	mDebugShadowShaderProgram->SetUniform("far_plane", mFar);
	mDebugShadowShaderProgram->SetUniform("depthMap", 0);
	//
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthTex);

	//some things we will need 
	ObjectManager * ObjgMng = ObjectManager::GetInstance();
	size_t size = static_cast<size_t>(ObjgMng->mGameObjects.size());
	for (size_t i = 0; i < size; i++)
	{
		//we then use the vao and we draw the object
		glBindVertexArray(ObjgMng->mGameObjects[i]->mMod.GetModelVao());
		CheckError();
		//draw
		glDrawElements(GL_TRIANGLES,
			static_cast<GLsizei>(ObjgMng->mGameObjects[i]->mMod.mModelIndices.size())
			, GL_UNSIGNED_INT, 0);
		CheckError();

		// Unbind
		glBindVertexArray(0);
	}

	//we dont use the program anymore
	glUseProgram(0);
}

///
// Brief description: Initialize the window and more stuff
///
void MyWindow::Initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}

	//We create the window 
	mWindow = SDL_CreateWindow("CS300", 100, 100, static_cast<int>(mWidth), static_cast<int>(mHeight), SDL_WINDOW_OPENGL);
	if (mWindow == nullptr)//check error
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	//Initialize openGL, context with SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext context_ = SDL_GL_CreateContext(mWindow);
	if (context_ == nullptr)//check error
	{
		SDL_DestroyWindow(mWindow);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	//Initialize  Glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		SDL_GL_DeleteContext(context_);
		SDL_DestroyWindow(mWindow);
		std::cout << "GLEW Error: Failed to init" << std::endl;
		SDL_Quit();
		exit(1);
	}

	//Initialize the Shaders that we will use
	mShaderProgram = GLSLProgram::CreateShaderProgram("data/Shaders/vertex.vs", "data/Shaders/fragment.fs");

#if TANBIACTIVE
	mTexturedShaderProgram = GLSLProgram::CreateShaderProgram("data/Shaders/texturednormalvertex.vs", "data/Shaders/texturednormalfragment.fs");
#else
	mTexturedShaderProgram = GLSLProgram::CreateShaderProgram("data/Shaders/texturedvertex.vs", "data/Shaders/texturedfragment.fs");
#endif
	mNormalShaderProgram = GLSLProgram::CreateShaderProgram("data/Shaders/normalvertex.vs", "data/Shaders/normalfragment.fs");
	mShadowShaderProgram = GLSLProgram::CreateShaderProgram("data/Shaders/simpleDepthShader.vs", "data/Shaders/simpleDepthShader.fs");
	mDebugShadowShaderProgram = GLSLProgram::CreateShaderProgram("data/Shaders/DebugShadows.vs", "data/Shaders/DebugShadows.fs");
	CheckError();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	InitializeDepthBuffer();
}

///
// Brief description: Debug prints that might use 
///
void MyWindow::DebugPrints()
{
	// print GPU data, QUERIES 

	std::cout << "GL_VENDOR: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL_VERSION: "  << glGetString(GL_VERSION) << std::endl;

	GLint totalMemKb = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &totalMemKb);
	std::cout << "GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX: " << totalMemKb << std::endl;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemKb);
	std::cout << "GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX: " << totalMemKb << std::endl;

	std::cout << std::endl
		<< "Extensions: "
		<< std::endl;
	int numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	for (int i = 0; i < numExtensions; i++)
	{
		std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
	}
}

glm::mat4 MyWindow::GetPerspectiveMatrix()
{
	return glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(mWidth / mHeight), mNear, mFar);
}

MyWindow::~MyWindow()
{
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

inline void MyWindow::CheckError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();
}

void MyWindow::PassLightParameters(GLSLProgram * Shader, Light & Li, GameObject & obj)
{
	int size = static_cast<int>(Li.mLights.size());

	Shader->SetUniform("LightAmount", size);


	for (int i = 0; i < size; i++)
	{
		std::string a = "LightSource[";
		a.append(std::to_string(i));
		a.append("]");

		Shader->SetUniform( a + ".mLightType", Li.mLight.mLightType);
		Shader->SetUniform( a +".mAmbientColor", Li.mLight.mAmbientColor);
		Shader->SetUniform( a +".mDiffuseColor", Li.mLight.mDiffuseColor);
		//position of the object
		Shader->SetUniform( a +".mPosition", Li.mLights[i]->mTransform.mPosition);
		Shader->SetUniform( a +".mSpecularColor", Li.mLight.mSpecularColor);
		//direction from the object to the light at (0,0,0)
		Shader->SetUniform( a +".mSpotDirection", -Li.mLights[i]->mTransform.mPosition);
		//Shader->SetUniform("LightSource.mAmbientStreng", Li.mLight.mAmbientStreng);

		Shader->SetUniform( a +".mConstantAttenuation", Li.mLight.mConstantAttenuation);
		Shader->SetUniform( a +".mLinearAttenuation", Li.mLight.mLinearAttenuation);
		Shader->SetUniform( a +".mQuadraticAttenuation", Li.mLight.mQuadraticAttenuation);

		Shader->SetUniform( a +".mCutOff", Li.mLight.mCutOff);
		Shader->SetUniform( a +".mOuterCutOff", Li.mLight.mOuterCutOff);
		Shader->SetUniform( a +".mSpotExponent", Li.mLight.mSpotExponent);
	}
		
	Shader->SetUniform("Material.mMaterialAmbient", obj.mMaterialParameters.mMaterialAmbient);
	Shader->SetUniform("Material.mMaterialDiffuse", obj.mMaterialParameters.mMaterialDiffuse);
	Shader->SetUniform("Material.mMaterialSpecular", obj.mMaterialParameters.mMaterialSpecular);
	Shader->SetUniform("Material.mMaterialShininess", obj.mMaterialParameters.mMaterialShininess);
}

void MyWindow::InitializeDepthBuffer()
{
	// Create to render texture (use window resolution)
	glGenTextures(1, &mDepthTex);
	glBindTexture(GL_TEXTURE_2D, mDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Expansion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create and set up the FBO
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	// Set output color texture at attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, attachments);
	//
	//// Add depth render buffer to the FBO
	//GLuint depthRenderBuffer;
	//glGenRenderbuffers(1, &depthRenderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);


	//check result
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer is complete.\n");
	}
	else
	{
		printf("Framebuffer is not complete.\n");
	}

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
