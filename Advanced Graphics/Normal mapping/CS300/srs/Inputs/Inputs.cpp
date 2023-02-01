#include "Inputs.h"
#include "GameObjects/ObjectManager.h"
#include "Window/Window.h"
#include "Camera/Camera.h"
#include "Light/Light.h"


InputsManager::InputsManager() : quit(false)
{
}

InputsManager::~InputsManager()
{
}

///
// Brief description: General input that we got for the presented model and wireframe
///
void InputsManager::GeneralInputs()
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
	ObjectManager * ObjMng = ObjectManager::GetInstance();
	GameObject * KingObj = ObjMng->FindGameObject("Blusti");
	if (!KingObj)
		return;
	
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_T)
	{
		//KingObj->mMod.mTextured = !KingObj->mMod.mTextured;

		MyWindow * Win = MyWindow::GetInstance();
		Win->NormalMode++;
		if (Win->NormalMode > 3)
			Win->NormalMode = 0;
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_B)
		KingObj->mMod.mTextured = !KingObj->mMod.mTextured;
	
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_1)
	{ 
		KingObj->mMod.ChangeModel(Model::PLANE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_2)
	{
		KingObj->mMod.ChangeModel(Model::CUBE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_3)
	{
		KingObj->mMod.ChangeModel(Model::CONE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_4)
	{
		KingObj->mMod.ChangeModel(Model::CYLINDER);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_5)
	{
		KingObj->mMod.ChangeModel(Model::SPHERE);
	}

	if (mEvent.key.keysym.scancode == SDL_SCANCODE_N)
	{
		KingObj->mMod.mShowNormal = !KingObj->mMod.mShowNormal;
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_F)
	{
		KingObj->mMod.mShowAverageNormal = !KingObj->mMod.mShowAverageNormal;
		if(KingObj->mMod.mShowAverageNormal)
			KingObj->mMod.ChangeToAverage();
		else
			KingObj->mMod.ChangeToNormals();

	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_G)
	{
		KingObj->mMod.mShowAxis = !KingObj->mMod.mShowAxis;
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_5)
	{
		KingObj->mMod.ChangeModel(Model::SPHERE);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
	{
		Model::Forms ObjectForm = KingObj->mMod.GetForm();
		int preci = KingObj->mMod.GetPrecision() - 1;
			if (preci < 5)
			preci = 5;

		if (ObjectForm == Model::SPHERE)
			KingObj->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);

		if (ObjectForm == Model::CYLINDER)
			KingObj->mMod.ChangeModel(Model::CYLINDER, preci, preci/2);

		if (ObjectForm == Model::CONE)
			KingObj->mMod.ChangeModel(Model::CONE, preci, preci / 2);
	}
	if (mEvent.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
	{
		Model::Forms ObjectForm = KingObj->mMod.GetForm();
		int preci = KingObj->mMod.GetPrecision() + 1;

		if (ObjectForm == Model::SPHERE)
			KingObj->mMod.ChangeModel(Model::SPHERE, preci, preci / 2);

		if (ObjectForm == Model::CYLINDER)
			KingObj->mMod.ChangeModel(Model::CYLINDER, preci, preci / 2);

		if (ObjectForm == Model::CONE)
			KingObj->mMod.ChangeModel(Model::CONE, preci, preci / 2);
	}
}

///
// Brief description: All the inpunts for the project
///
void InputsManager::Inputs(GameObject * MainGameObj)
{
	MyCamera * mCamera = MyCamera::GetInstance();
	Light * mLights = Light::GetInstance();

	while (SDL_PollEvent(&mEvent))
	{
		switch (mEvent.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			GeneralInputs();

			MainGameObj->mTransform.GetInputToRotate(mEvent.key.keysym.scancode);
			mCamera->Update(mEvent.key.keysym.scancode);
			mLights->Input(mEvent.key.keysym.scancode);

			if (mEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				quit = true;
			break;
		}
	}
}
