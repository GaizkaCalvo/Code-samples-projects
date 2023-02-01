///
// Brief description: Object manager that we will use to control the game objects
///
#pragma once

#include "game_object.hpp"

class ObjectManager
{
public:
	static ObjectManager * get_instance()
	{
		static ObjectManager mInstance;

		return &mInstance;
	}


	std::vector<GameObject *> mGameObjects;
private:
	ObjectManager();
	~ObjectManager();


public:
	ObjectManager(ObjectManager const &) = delete;
	void operator = (ObjectManager const &) = delete;

	GameObject * AddGameObject(const char * Name, const glm::vec3 & position,const std::shared_ptr<cs350::mesh> form, const glm::vec3 scale = { 1.0f,1.0f ,1.0f });
	GameObject * FindGameObject(const char * Name);
};