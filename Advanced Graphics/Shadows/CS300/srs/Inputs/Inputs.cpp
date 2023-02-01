#include "Inputs.h"
#include "GameObjects/ObjectManager.h"
#include "Window/Window.h"
#include "Camera/Camera.h"
#include "Light/Light.h"
#include "Level/Level.h"


InputsManager::InputsManager() : quit(false)
{
}

InputsManager::~InputsManager()
{
}

///
// Brief description: General input that we got for the presented model and wireframe
///
void InputsManager::GeneralInputs(GameObject * MainGameObj)
{
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_M)
	{
		MyWindow * Win = MyWindow::GetInstance();
		//change the wireframe mode 
		if (Win->mWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//update the bool
		Win->mWireframe = !Win->mWireframe;
	}


	//INPUTS FOR THE OBJECT PRESENTED IN THE SCREEN
	
	//if (mEvent.key.keysym.scancode == SDL_SCANCODE_T)
	//{
	//	//KingObj->mMod.mTextured = !KingObj->mMod.mTextured;
	//
	//	MyWindow * Win = MyWindow::GetInstance();
	//	Win->NormalMode++;
	//	if (Win->NormalMode > 3)
	//		Win->NormalMode = 0;
	//}

	ObjectManager * mObjMng = ObjectManager::GetInstance();

	GameObject * Little1 = mObjMng->FindGameObject("Blusti1");
	GameObject * Little2 = mObjMng->FindGameObject("Blusti2");

	if (mEvent.key.keysym.scancode == SDL_SCANCODE_B)
	{
		MainGameObj->mMod.mTextured = !MainGameObj->mMod.mTextured;
		Little1->mMod.mTextured = !Little1->mMod.mTextured;
		Little2->mMod.mTextured = !Little2->mMod.mTextured;
	}
	
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_1)
	{ 
		MainGameObj->mMod.ChangeModel(Model::PLANE);
		Little1->mMod.ChangeModel(Model::PLANE);
		Little2->mMod.ChangeModel(Model::PLANE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_2)
	{
		MainGameObj->mMod.ChangeModel(Model::CUBE);
		Little1->mMod.ChangeModel(Model::CUBE);
		Little2->mMod.ChangeModel(Model::CUBE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_3)
	{
		MainGameObj->mMod.ChangeModel(Model::CONE);
		Little1->mMod.ChangeModel(Model::CONE);
		Little2->mMod.ChangeModel(Model::CONE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_4)
	{
		MainGameObj->mMod.ChangeModel(Model::CYLINDER);
		Little1->mMod.ChangeModel(Model::CYLINDER);
		Little2->mMod.ChangeModel(Model::CYLINDER);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_5)
	{
		MainGameObj->mMod.ChangeModel(Model::SPHERE);
		Little1->mMod.ChangeModel(Model::SPHERE);
		Little2->mMod.ChangeModel(Model::SPHERE);
	}

	if (mEvent.key.keysym.scancode == SDL_SCANCODE_N)
	{
		MainGameObj->mMod.mShowNormal = !MainGameObj->mMod.mShowNormal;
		Little1->mMod.mShowNormal = !Little1->mMod.mShowNormal;
		Little2->mMod.mShowNormal = !Little2->mMod.mShowNormal;
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_F)
	{
		MainGameObj->mMod.mShowAverageNormal = !MainGameObj->mMod.mShowAverageNormal;
		Little1->mMod.mShowAverageNormal = !Little1->mMod.mShowAverageNormal;
		Little2->mMod.mShowAverageNormal = !Little2->mMod.mShowAverageNormal;

		if (MainGameObj->mMod.mShowAverageNormal)
		{
			MainGameObj->mMod.ChangeToAverage();
			Little1->mMod.ChangeToAverage();
			Little2->mMod.ChangeToAverage();
		}
		else
		{
			MainGameObj->mMod.ChangeToNormals();
			Little1->mMod.ChangeToNormals();
			Little2->mMod.ChangeToNormals();
		}

	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
	{
		Model::Forms ObjectForm = MainGameObj->mMod.GetForm();
		int preci = MainGameObj->mMod.GetPrecision() - 1;

		if (preci < 5)
			preci = 5;

		if (ObjectForm == Model::SPHERE)
		{
			MainGameObj->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);
			Little1->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);
			Little2->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);
		}

		if (ObjectForm == Model::CYLINDER)
		{
			MainGameObj->mMod.ChangeModel(Model::CYLINDER, preci, preci/2);
			Little1->mMod.ChangeModel(Model::CYLINDER, preci, preci/2);
			Little2->mMod.ChangeModel(Model::CYLINDER, preci, preci/2);
		}

		if (ObjectForm == Model::CONE)
		{
			MainGameObj->mMod.ChangeModel(Model::CONE, preci, preci / 2);
			Little1->mMod.ChangeModel(Model::CONE, preci, preci / 2);
			Little2->mMod.ChangeModel(Model::CONE, preci, preci / 2);
		}
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
	{
		Model::Forms ObjectForm = MainGameObj->mMod.GetForm();
		int preci = MainGameObj->mMod.GetPrecision() + 1;

		if (ObjectForm == Model::SPHERE)
		{
			MainGameObj->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);
			Little1->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);
			Little2->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);
		}

		if (ObjectForm == Model::CYLINDER)
		{
			MainGameObj->mMod.ChangeModel(Model::CYLINDER, preci, preci / 2);
			Little1->mMod.ChangeModel(Model::CYLINDER, preci, preci / 2);
			Little2->mMod.ChangeModel(Model::CYLINDER, preci, preci / 2);
		}

		if (ObjectForm == Model::CONE)
		{
			MainGameObj->mMod.ChangeModel(Model::CONE, preci, preci / 2);
			Little1->mMod.ChangeModel(Model::CONE, preci, preci / 2);
			Little2->mMod.ChangeModel(Model::CONE, preci, preci / 2);
		}
	}
}

///
// Brief description: All the inpunts for the project
///
void InputsManager::Inputs(GameObject * MainGameObj)
{
	MyCamera * mCamera = MyCamera::GetInstance();
	Light * mLights = Light::GetInstance();
	Level * mLevel = Level::GetInstance();

	while (SDL_PollEvent(&mEvent))
	{
		switch (mEvent.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			GeneralInputs(MainGameObj);

			MainGameObj->mTransform.GetInputToRotate(mEvent.key.keysym.scancode);
			mCamera->Inputs(mEvent.key.keysym.scancode);
			mLights->Input(mEvent.key.keysym.scancode);
			mLevel->Inputs(mEvent.key.keysym.scancode);

			if (mEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				quit = true;
			break;
		}
	}
}
