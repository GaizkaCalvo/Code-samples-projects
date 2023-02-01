#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/GL.h>
#include <SDL2/SDL.h>

#include "Shader/GLSLProgram.h"
#include "Window/Window.h"
#include "GameObjects/ObjectManager.h"
#include "Level/Level.h"
#include "Inputs/Inputs.h"
#include "Light/Light.h"

#define TANBIACTIVE = 0;

//sdl has its own main, necesary to do this
#undef main
int main(int argc, char* args[])
{
	MyWindow * mWindow = MyWindow::GetInstance();
	ObjectManager * ObjMng = ObjectManager::GetInstance();
	InputsManager * InputMng = InputsManager::GetInstance();

	mWindow->Initialize();

	Light * mLights = Light::GetInstance();
	
	GameObject* GameObj = ObjMng->AddGameObject("Blusti", glm::vec3{ 0.0f, 0.0f , 0.0f },
		Model::SPHERE, glm::vec3{ 10.0f, 10.0f , 10.0f });

	Level * MyLevel = Level::GetInstance();

	while (!InputMng->quit)
	{
		InputMng->Inputs(GameObj);

		mLights->Update();
		MyLevel->Update();

		mWindow->Display();
	}


	//Clear everything at the end
	return 0;
}