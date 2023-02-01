#include "Camera.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

MyCamera::MyCamera() : mAngleX(0.0f), mAngleY(0.0f), mRadius(50.0f)
{
	mCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(mCameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	mUp = glm::cross(cameraDirection, cameraRight);
}

///
// Brief description: Update of the camera to check the inputs 
///
void MyCamera::Inputs(SDL_Scancode & mEvent)
{
	if (mEvent == SDL_SCANCODE_W)
	{
		mAngleY += 0.5f;
	}
	if (mEvent == SDL_SCANCODE_A)
	{
		mAngleX -= 0.07f;
	}
	if (mEvent == SDL_SCANCODE_S)
	{
		mAngleY -= 0.5f;
	}
	if (mEvent == SDL_SCANCODE_D)
	{
		mAngleX += 0.07f;
	}
	if (mEvent == SDL_SCANCODE_E)
	{
		mRadius += 0.5f;
	}
	if (mEvent == SDL_SCANCODE_Q)
	{
		mRadius -= 0.5f;
		if (mRadius < 0.1f)
			mRadius = 0.1f;
	}
}

///
// Brief description: Get viewmatrix
///
glm::mat4 MyCamera::GetViewMatrix()
{
	glm::mat4 view;
	
	float camX = sin(mAngleX) * mRadius;
	float camZ = cos(mAngleX) * mRadius;

	//float camY = sin(mAngleY) * mRadius;
	//camZ -= (1-cos(mAngleY)) * mRadius;

	view = glm::lookAt(glm::vec3(camX, mAngleY, camZ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f));

	mCameraPos = glm::vec3(camX, mAngleY, camZ);

	return view;
}
