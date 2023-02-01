#include "Level.h"

#include "GameObjects/ObjectManager.h"
#include "Light/Light.h"
#include "Textures/Texture.h"

#include <glm/gtc/constants.hpp>

Level::Level() : mUpdate(true)
{
	ObjectManager * ObjMng = ObjectManager::GetInstance();

	//GameObject * Floor = ObjMng->AddGameObject("Floor", glm::vec3{ 0.0f, -15.0f , 0.0f },
	//	Model::PLANE, glm::vec3{ 50.0f, 50.0f , 50.0f });
	//Floor->mTransform.RotateAround(Transform::Directions::Ri, glm::pi<float>()/2 );

	Little1 = ObjMng->AddGameObject("Blusti1", glm::vec3{ 13.0f, 0.0f , 0.0f },
		Model::SPHERE, glm::vec3{ 5.0f, 5.0f , 5.0f });
	Little2 = ObjMng->AddGameObject("Blusti2", glm::vec3{ -13.0f, 0.0f , 0.0f },
		Model::SPHERE, glm::vec3{ 5.0f, 5.0f , 5.0f });

	//ObjMng->AddGameObject("ELGORDO", glm::vec3{ 200.0f, 0.0f , 0.0f },
	//	Model::SPHERE, glm::vec3{ 50.0f, 50.0f , 50.0f });
}

Level::~Level()
{
}

void Level::Inputs(SDL_Scancode & mEvent)
{
	if (mEvent == SDL_SCANCODE_O)
	{
		mUpdate = !mUpdate;
	}
}

void Level::Update()
{
	if (!mUpdate)
		return;
	mSpeed += 0.005f;
	Little1->mTransform.mPosition.y = sin(mSpeed) * 10.0f;
	Little2->mTransform.mPosition.y = sin(mSpeed) * 10.0f;

	float mRadio = 20.0f;

	Little1->mTransform.mPosition.x = sin(mSpeed) *  mRadio;
	Little2->mTransform.mPosition.x = sin(mSpeed) * -mRadio;
	Little1->mTransform.mPosition.z = cos(mSpeed) *  mRadio;
	Little2->mTransform.mPosition.z = cos(mSpeed) *- mRadio;
}
