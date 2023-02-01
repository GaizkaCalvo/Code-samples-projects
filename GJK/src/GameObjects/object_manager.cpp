#include "Helpers/pch.hpp"
#include "object_manager.hpp"

///
// Brief description: Constructor
///
ObjectManager::ObjectManager()
{
}
///
// Brief description: Destructor
///
ObjectManager::~ObjectManager()
{
	int size = static_cast<int>(mGameObjects.size());
	//clean all the objects that we create 
	for (int i = 0; i < size; i++)
	{
		delete mGameObjects[i];
	}
}
///
// Brief description: Function we will use to add a new object to the game
///
GameObject * ObjectManager::AddGameObject(const char * Name, const glm::vec3 & position, const std::shared_ptr<cs350::mesh> form, const glm::vec3 scale)
{
	//push the new object with the data given 
	mGameObjects.push_back(new GameObject(Name, position, scale, form));
	//return the new object that we created
	return mGameObjects.back();
}
///
// Brief description: Function we will use to find the game object with the name given
// Return value: Null in case it doesn't find anything 
///
GameObject * ObjectManager::FindGameObject(const char * Name)
{
	int size = static_cast<int>(mGameObjects.size());
	//search in all the vector
	for (int i = 0; i < size; i++)
	{
		//if the object has the same name we return it
		if (mGameObjects[i]->mName == Name)
			return mGameObjects[i];
	}
	//in case we didn't find it
	return nullptr;
}
