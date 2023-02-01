///
// Brief description: Class we will use to store the data of the object that we will create
// his position, rotation and scale in the world
///
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <SDL2/SDL.h>

class GameObject;

class Transform
{
public:
	Transform();
	~Transform();

	enum Directions
	{
		Forw,
		Ri,
		Up
	};

	glm::mat4x4 GetModelToWorld() const;

	glm::vec3 mPosition;
	glm::vec3 mScale;

	GameObject * mParent;
	std::vector<GameObject *> mChildVector;

	glm::vec3 GetWorldPosition() const;

	//getters
	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	//data of the rotations
	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mRight;

	float mRotationAroundY;
	float mRotationAroundX;

	void GetInputToRotate(SDL_Scancode & event);

	void RotateAround(Directions direction, float angle);
	
private:
};
