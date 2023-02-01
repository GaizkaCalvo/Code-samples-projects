///
// Brief description: Class of gameobject that we will use to track the tanks and cubes we create
///
#pragma once
#include "../Render/mesh.hpp"
#include "Transform/Transform.hpp"
#include "BVH/bvh.hpp"


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
	bool mFaceRender;
	bool mDebugLines;

	cs350::bvh mBVH;

private:


};