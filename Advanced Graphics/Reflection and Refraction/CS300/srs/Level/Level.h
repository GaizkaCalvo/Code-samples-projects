///
// Brief description: Class we use to create the Level that we will show
///
#pragma once
#include <SDL2/SDL.h>

class GameObject;

class Level
{
public:
	static Level * GetInstance()
	{
		static Level mInstance;

		return &mInstance;
	}

	void Inputs(SDL_Scancode & mEvent);

	void Update();

	bool mUpdate;

private:

	Level();
	~Level();

	GameObject * Little1;
	GameObject * Little2;

	float mSpeed = 0.05f;
};