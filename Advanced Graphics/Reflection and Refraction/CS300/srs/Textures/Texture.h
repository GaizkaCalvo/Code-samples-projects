///
// Brief description: Class we will use to use the textures 
///
#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>


class MyTextures
{
public:
	//MyTextures();
	//~MyTextures();

	void LoadGeneralTexture();

	void LoadTexture(const std::string & filename);

	void CreateSkybox();
	void LoadCubemap(std::vector<std::string> faces);
	void CreateCubemap();
	void CreateFBOs();

	unsigned int mCubeMapID;
	GLuint cubeTexture[6];
	GLuint cubeFBO[6];

	std::vector<GLuint> mTextures;
	GLuint mGeneralTextures;

	GLuint mNormalMap;

	std::vector<std::string> faces
	{
		std::string("./data/textures/CubeMap/CubeMap_Right.png"),
		std::string("./data/textures/CubeMap/CubeMap_Left.png"),
		std::string("./data/textures/CubeMap/CubeMap_Top.png"),
		std::string("./data/textures/CubeMap/CubeMap_Bottom.png"),
		std::string("./data/textures/CubeMap/CubeMap_Front.png"),
		std::string("./data/textures/CubeMap/CubeMap_Back.png")
	};
	
	unsigned int skyboxVAO, skyboxVBO;


private:

	void BeginRendering();
	void DrawToFace(int i);
	void DrawScene(int i, glm::mat4);
	void RenderSkybox(glm::mat4);

	inline void CheckError();
};