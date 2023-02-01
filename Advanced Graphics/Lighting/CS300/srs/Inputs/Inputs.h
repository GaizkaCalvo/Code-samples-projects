///
// Brief description: Input manager
///
#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>

class GameObject;

class InputsManager
{
public:
	static InputsManager * GetInstance()
	{
		static InputsManager mInstance;

		return &mInstance;
	}


private:
	InputsManager();
	~InputsManager();

	SDL_Event mEvent;

	void GeneralInputs();

public:
	void Inputs(GameObject* MainGameObj);
	bool quit;
};