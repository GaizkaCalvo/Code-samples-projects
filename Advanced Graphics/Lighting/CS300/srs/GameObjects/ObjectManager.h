///
// Brief description: Object manager that we will use to control the game objects
///
#pragma once

#include "GameObject.h"

#include "Model/Model.h"

class ObjectManager
{
public:
	static ObjectManager * GetInstance()
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

	GameObject * AddGameObject(const char * Name, const glm::vec3 & position,const Model::Forms form = Model::Forms::PLANE, const glm::vec3 scale = { 1.0f,1.0f ,1.0f });
	GameObject * FindGameObject(const char * Name);
};