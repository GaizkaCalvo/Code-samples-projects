#include "Texture.h"
#include "Color.h"

#include "Camera/Camera.h"
#include "GameObjects/ObjectManager.h"
#include "Window/Window.h"
#include "Shader/GLSLProgram.h"

#include <vector>

///
// Brief description: General texture that we will create
///
static Color mTexture[]{
	Color::Blue,       Color::BrightBlue,  Color::Green,      Color::Yellow,     Color::Red,        Color::Purple,
	Color::BrightBlue, Color::Green,       Color::Yellow,     Color::Red,        Color::Purple,     Color::Blue,
	Color::Green,      Color::Yellow,      Color::Red,        Color::Purple,     Color::Blue,       Color::BrightBlue,
	Color::Yellow,     Color::Red,         Color::Purple,     Color::Blue,       Color::BrightBlue, Color::Green,
	Color::Red,        Color::Purple,      Color::Blue,       Color::BrightBlue, Color::Green,      Color::Yellow,
	Color::Purple,     Color::Blue,        Color::BrightBlue, Color::Green,      Color::Yellow,     Color::Red
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};


///
// Brief description: Load the general texture that we will use for now 
///
void MyTextures::LoadGeneralTexture()
{
	//SDL_Surface * surface = IMG_Load(filename.c_str());
	//
	//if (surface == nullptr)
	//{
	//	std::cout << "Could not load texture: " << filename.c_str() << std::endl;
	//	return 0;
	//}

	// Create texture
	glGenTextures(1, &mGeneralTextures);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glBindTexture(GL_TEXTURE_2D, mGeneralTextures);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Select pixel format from surface
	int pixelFormat = GL_RGBA;
	//if (surface->format->BytesPerPixel == 4) {
	//	pixelFormat = GL_RGBA;
	//}

	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, 6, 6, 0, pixelFormat, GL_UNSIGNED_BYTE, (unsigned char*)mTexture);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glGenerateMipmap(GL_TEXTURE_2D);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Free surface because OpenGL already has the data
	//SDL_FreeSurface(surface);

	//return texture;
}

void MyTextures::LoadTexture(const std::string & filename)
{
	SDL_Surface * surface = IMG_Load(filename.c_str());

	if (surface == nullptr)
	{
		std::cout << "Could not load texture: " << filename.c_str() << std::endl;
		abort();
	}

	// Create texture
	glGenTextures(1, &mNormalMap);
	glBindTexture(GL_TEXTURE_2D, mNormalMap);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Select pixel format from surface
	int pixelFormat = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {
		pixelFormat = GL_RGBA;
	}

	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, surface->w, surface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, surface->pixels);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glGenerateMipmap(GL_TEXTURE_2D);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Free surface because OpenGL already has the data
	SDL_FreeSurface(surface);
}

void MyTextures::CreateSkybox()
{
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void MyTextures::LoadCubemap(std::vector<std::string> faces)
{
	glGenTextures(1, &mCubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		SDL_Surface * surface = IMG_Load(faces[i].c_str());
		if (surface == nullptr)
		{
			std::cout << "Could not load texture: " << faces[i].c_str() << std::endl;
			abort();
		}

		// Select pixel format from surface
		int pixelFormat = GL_RGB;
		if (surface->format->BytesPerPixel == 4) {
			pixelFormat = GL_RGBA;
		}

		// Give pixel data to opengl
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, pixelFormat, surface->w, surface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, surface->pixels);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			abort();

		glGenerateMipmap(GL_TEXTURE_2D);
		error = glGetError();
		if (error != GL_NO_ERROR)
			abort();
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void MyTextures::CreateCubemap()
{
	// bind the fbo, save our OpenGL state
	//BeginRendering();

	for (int i = 0; i < 6; ++i)
	{
		// attach the correct cube face, clear it and set up the matrices
		DrawToFace(i);
	}
	// unbind fbo, return to backbuffer rendering, cubemap is now ready to use
	//endRendering();

}

void MyTextures::CreateFBOs()
{
	glGenTextures(1, &mCubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapID);

	const unsigned EnvMapSize = 512;
	for (GLuint i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, EnvMapSize, EnvMapSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	// Create and set up the FBO
	glGenFramebuffers(6, cubeFBO);
	for (GLuint i = 0; i < 6; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mCubeMapID, 0);
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MyTextures::BeginRendering()
{
}

void MyTextures::DrawToFace(int face)
{
	// setup lookat depending on current face
	glm::mat4 ViewMat;

	switch (face + 1)
	{
	case 1:
		ViewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
		break;

	case 2:
		ViewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
		break;

	case 3://top
		ViewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
		break;

	case 4://bottom
		ViewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
		break;

	case 5:
		ViewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
		break;

	case 6:
		ViewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
		break;

	default:
		break;
	};


	// draw the scene
	DrawScene(face, ViewMat);
}

void MyTextures::DrawScene(int face, glm::mat4 view)
{
	// Bind the frame buffer containing the shadow map
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO[face]);
	//we set the color of the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//we clear the buffer that we indicate in the flag 
	//clear the depth buffer 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//check error
	CheckError();

	glViewport(0, 0, 512, 512);

	//some things we will need 
	ObjectManager * ObjgMng = ObjectManager::GetInstance();
	MyCamera * mCamera = MyCamera::GetInstance();
	MyWindow * mWin = MyWindow::GetInstance();


	RenderSkybox(view);

	//shader that we will use
	mWin->mTexturedShaderProgram->Use();


	size_t size = static_cast<size_t>(ObjgMng->mGameObjects.size());
	for (size_t i = 0; i < size; i++)
	{

		//UNIFORMS WE WILL GIVE TO THE SHADER
		glm::mat4 transform = ObjgMng->mGameObjects[i]->mTransform.GetModelToWorld();
		//fov, aspec ratio, near, far, respect to the light
		glm::mat4 perspective = glm::perspective(glm::radians(90.f), 1.0f, mWin->mNear, mWin->mFar);

		mWin->mTexturedShaderProgram->SetUniform("ModelMat", transform);
		mWin->mTexturedShaderProgram->SetUniform("PerspectiveMat", perspective);
		mWin->mTexturedShaderProgram->SetUniform("ViewMat", view);
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

	glViewport(0, 0, (GLsizei)mWin->mWidth, (GLsizei)mWin->mHeight);
}

void MyTextures::RenderSkybox(glm::mat4 view)
{
	MyCamera * mCamera = MyCamera::GetInstance();
	MyWindow * mWin = MyWindow::GetInstance();

	// draw skybox as last
	// Enable front-face culling
	//glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	mWin->mSkyBoxProgram->Use();
	mWin->mSkyBoxProgram->SetUniform("skybox", 0);
	glm::mat4 perspective = glm::perspective(glm::radians(90.f), 1.0f, mWin->mNear, mWin->mFar);
	mWin->mSkyBoxProgram->SetUniform("view", view);
	mWin->mSkyBoxProgram->SetUniform("projection", perspective);
	// skybox cube
	glBindVertexArray(mWin->mTexture.skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mWin->mTexture.mCubeMapID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

inline void MyTextures::CheckError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();
}