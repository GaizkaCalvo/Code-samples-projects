///
// Brief description: Window class that we will use to store the window data
///
#pragma once
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

class Light;
class GameObject;
class GLSLProgram;

class MyWindow
{
public:
	static MyWindow * GetInstance()
	{
		static MyWindow mInstance;

		return &mInstance;
	}
	//Called to update the display.
	//You should call SDL_GL_SwapWindow after all of your rendering to display what you rendered.
	void Display();

	void Initialize();

	void DebugPrints();

	glm::mat4 GetPerspectiveMatrix();

private:
	MyWindow() : mWidth(1080.0f), mHeight(720.0f), mNear(0.1f), mFar(500.0F), mWireframe(false)
	{}
	~MyWindow();

	float mWidth;
	float mHeight;

	GLfloat mNear;
	GLfloat mFar;

	inline void CheckError();
	
	void PassLightParameters(GLSLProgram * Shader, Light & Li, GameObject & obj);

public:
	SDL_Window * mWindow;
	SDL_GLContext mContext;

	GLSLProgram * mShaderProgram;
	GLSLProgram * mTexturedShaderProgram;
	GLSLProgram * mNormalShaderProgram;

	bool mWireframe;
};