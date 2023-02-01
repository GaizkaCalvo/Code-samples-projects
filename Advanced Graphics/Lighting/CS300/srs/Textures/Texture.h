///
// Brief description: Class we will use to use the textures 
///
#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>


class MyTextures
{
public:
	//MyTextures();
	//~MyTextures();

	void LoadGeneralTexture();

	std::vector<GLuint> mTextures;
	GLuint mGeneralTextures;
private:
};