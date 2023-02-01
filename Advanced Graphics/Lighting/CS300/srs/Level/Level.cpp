#include "Level.h"

#include "GameObjects/ObjectManager.h"
#include "Light/Light.h"

#include <glm/gtc/constants.hpp>

Level::Level()
{
	ObjectManager * ObjMng = ObjectManager::GetInstance();

	GameObject * Floor = ObjMng->AddGameObject("Floor", glm::vec3{ 0.0f, -15.0f , 0.0f },
		Model::PLANE, glm::vec3{ 50.0f, 50.0f , 50.0f });

	Floor->mTransform.RotateAround(Transform::Directions::Ri, glm::pi<float>()/2 );

	//GameObject * Light = ObjMng->AddGameObject("Light", glm::vec3{ 3.0f, 0.0f , 0.0f },
	//	Model::SPHERE, glm::vec3{ 0.1f, 0.1f , 0.1f });
	//Light->mTransform.mLight.mAmbient;	
}

Level::~Level()
{
}
