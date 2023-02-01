///
// Brief description: Class we will use to store the data of the object that we will create
// his position, rotation and scale in the world
///
#pragma once
#include "Helpers/opengl.hpp"

class GameObject;

class Transform
{
public:
	Transform();
	~Transform();

	enum class Directions
	{
		Forw,
		Ri,
		Up
	};

	glm::mat4x4 GetModelToWorld() const;

	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::vec3 mRotation;

	GameObject * mParent;
	std::vector<GameObject *> mChildVector;

	glm::vec3 GetWorldPosition() const;

	//getters
	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;


	float mRotationAroundY;
	float mRotationAroundX;

	void RotateAround(Directions direction, float angle);
	
private:

	//data of the rotations
	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mRight;
};
