#include "GameObject.h"

///
// Brief description: Constructor 
///
GameObject::GameObject(const char * Name, const glm::vec3 & position, const glm::vec3 & scale, Model::Forms form)
	: mName(Name)
{
	//we copy the data that will be given 
	mTransform.mPosition = position;
	mTransform.mScale = scale;
	mMod.Initialize(form);

	mMaterialParameters.mMaterialAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mMaterialParameters.mMaterialDiffuse = glm::vec4(1.0f);
	mMaterialParameters.mMaterialSpecular = glm::vec4(1.0f);
	mMaterialParameters.mMaterialShininess = 50;
}
///
// Brief description: Destructor
///
GameObject::~GameObject()
{
}
