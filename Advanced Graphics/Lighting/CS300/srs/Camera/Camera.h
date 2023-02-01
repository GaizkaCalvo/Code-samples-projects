///
// Brief description: Class where we will store the data that the camera will have
///
#pragma once
#include "GameObjects/Transform/Transform.h"

class MyCamera
{
public:
	static MyCamera * GetInstance()
	{
		static MyCamera mInstance;

		return &mInstance;
	}


private:
	MyCamera();

	~MyCamera()
	{}

public:
	MyCamera(MyCamera const &) = delete;
	void operator = (MyCamera const &) = delete;

	void Update(SDL_Scancode & event);

	glm::mat4 GetViewMatrix();

	glm::vec3 mCameraPos;

private:


	glm::vec3 mCameraTarget;

	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mRight;

	float mAngleX;
	float mAngleY;

	float mRadius;
};