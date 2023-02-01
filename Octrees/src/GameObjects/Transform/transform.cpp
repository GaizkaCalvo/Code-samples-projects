///
// Brief description: Function definitions 
///
#include "Helpers/pch.hpp"
#include "Transform.hpp"
#include "GameObjects/game_object.hpp"
#include "Helpers/math.hpp"

///
// Brief description: Constructor
///
Transform::Transform() : mParent(nullptr), mForward(0.0f, 0.0f, 1.0f)
, mUp(0.0f, 1.0f, 0.0f), mRotationAroundY(0.0f), mRotationAroundX(0.0f)
, mScale(glm::vec3(1)), mPosition(glm::vec3(1))
{
	mRight = glm::cross(glm::vec3{ mForward.x, mForward.y, mForward.z }, glm::vec3{ 0.0f, 1.0f, 0.0f});
}
///
// Brief description: Destructor
///
Transform::~Transform()
{
}
///
// Brief description: Return the matrix of the model to world transformation 
///
glm::mat4x4 Transform::GetModelToWorld() const
{

	glm::mat4x4 RotationMatrix(
		-mRight.x, mUp.x, mForward.x, 0.0f,
		-mRight.y, mUp.y, mForward.y, 0.0f,
		-mRight.z, mUp.z, mForward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// make sure to initialize matrix to identity matrix first
	glm::mat4 transform = glm::mat4(1.0f); 
	//put the translation 
	transform = glm::translate(transform, glm::vec3(mPosition.x, mPosition.y, mPosition.z));
	//then the custom rotation that we have to use forward up and right
	transform = transform * RotationMatrix;
	//then we scale
	transform = glm::scale(transform, glm::vec3(mScale.x, mScale.y, mScale.z));

	return transform;
}

///
// Brief description: Get World position
///
glm::vec3 Transform::GetWorldPosition() const
{
	glm::vec3 Position = mPosition;
	//no parenting yet
	return Position;
}
///
// Brief description: Get Forward
///
glm::vec3 Transform::GetForward() const
{
	return mForward;
}
///
// Brief description: Get Up
///
glm::vec3 Transform::GetUp() const
{
	return mUp;
}
///
// Brief description: Get Right
///
glm::vec3 Transform::GetRight() const
{
	return mRight;
}

///
// Brief description: Rotate the object around one of the three axes (forward, up or right)
///
void Transform::RotateAround(Directions direction, float angle)
{
	//we check which one to do 
	glm::vec3 n;
	if (direction == Directions::Forw)
		n = mForward;
	else if (direction == Directions::Ri)
		n = mRight;
	else
		n = mUp;

	//we create the matrix with the data of the object 
	glm::mat4 ICos(1);
	ICos *= cosf(angle);
	glm::mat4 Tensor(
		n.x*n.x, n.x*n.y, n.x*n.z, 0.0f,
		n.y*n.x, n.y*n.y, n.y*n.z, 0.0f,
		n.z*n.x, n.z*n.y, n.z*n.z, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);

	glm::mat4 CrossMatrix(
		0.0f, -n.z, n.y, 0.0f,
		n.z, 0.0f, -n.x, 0.0f,
		-n.y, n.x, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
	glm::mat4 T = ICos + Tensor * (1 - cosf(angle)) + CrossMatrix * sinf(angle);
	//we finally get what we need 
	glm::mat4 Result(
		T[0].x, T[0].y, T[0].z, 0.0f,
		T[1].x, T[1].y, T[1].z, 0.0f,
		T[2].x, T[2].y, T[2].z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//w finally update the other 2 axes that we move
	if (direction == Directions::Forw)
	{
		mUp = glm::vec3(Result * glm::vec4(mUp, 1.0f));
		mUp = glm::normalize(mUp);
		mRight = glm::vec3(Result * glm::vec4(mRight, 1.0f));
		mRight = glm::normalize(mRight);
	}
	else if (direction == Directions::Ri)
	{
		mUp = glm::vec3(Result * glm::vec4(mUp, 1.0f));
		mUp = glm::normalize(mUp);
		mForward = glm::vec3(Result * glm::vec4(mForward, 1.0f));
		mForward = glm::normalize(mForward);
	}
	else
	{
		mForward = glm::vec3(Result * glm::vec4(mForward, 1.0f));
		mForward = glm::normalize(mForward);
		mRight = glm::vec3(Result * glm::vec4(mRight, 1.0f));
		mRight = glm::normalize(mRight);
	}
}