///
// Brief description: Class of gameobject that we will use to track the tanks and cubes we create
///
#pragma once

#include <glm/glm.hpp>
#include "Transform/Transform.h"
#include "Model/Model.h"

struct MaterialParameters
{
	//glm::vec4 emission;
	//k
	glm::vec4 mMaterialAmbient;
	glm::vec4 mMaterialDiffuse;
	glm::vec4 mMaterialSpecular;
	//shininess
	float mMaterialShininess;
};

class GameObject
{
public:
	GameObject() = delete;
	GameObject(const char * Name, const glm::vec3 & position, const glm::vec3 & scale, Model::Forms form = Model::NONE);
	~GameObject();

	Transform mTransform;
	std::string mName;
	Model mMod;

	MaterialParameters mMaterialParameters;
private:

};