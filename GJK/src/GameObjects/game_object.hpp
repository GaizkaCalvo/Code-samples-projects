///
// Brief description: Class of gameobject that we will use to track the tanks and cubes we create
///
#pragma once
#include "Transform/Transform.hpp"
#include "BVH/bvh.hpp"
#include "GJK/Minkowski.hpp"
#include "GJK/gjk_simplex.h"


class GameObject
{
public:
	GameObject() = delete;
	GameObject(const char * Name, const glm::vec3 & position, const glm::vec3 & scale, std::shared_ptr<cs350::mesh> form);
	~GameObject();

	void change_model(std::shared_ptr<cs350::mesh> form);

	Transform mTransform;
	std::string mName;

	std::shared_ptr<cs350::mesh> mMod;
	glm::vec4 mDebugColor{0.0f, 1.0f, 1.0f, 1.0f};
	bool mFaceRender;
	bool mDebugLines;


	cs350::bvh mBVH;
	cs350::minkowski mMinkowski;
	cs350::gjk_simplex mSimplex;

private:


};