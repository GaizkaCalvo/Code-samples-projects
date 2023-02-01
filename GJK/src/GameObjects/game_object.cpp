#include "Helpers/pch.hpp"
#include "game_object.hpp"

///
// Brief description: Constructor 
///
GameObject::GameObject(const char * Name, const glm::vec3 & position, const glm::vec3 & scale, std::shared_ptr<cs350::mesh> form)
	: mName(Name), mFaceRender(true), mDebugLines(true)
{
	//we copy the data that will be given 
	mTransform.mPosition = position;
	mTransform.mScale = scale;
	mMod = form;
}
///
// Brief description: Destructor
///
GameObject::~GameObject()
{
	mBVH.clear();
}

void GameObject::change_model(std::shared_ptr<cs350::mesh> form)
{
	mMod = form;
	mBVH.clear();
}
