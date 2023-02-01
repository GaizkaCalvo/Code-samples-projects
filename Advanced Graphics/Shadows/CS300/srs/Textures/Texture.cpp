#include "Texture.h"
#include "Color.h"

#include <vector>

///
// Brief description: General texture that we will create
///
static Color mTexture[]{
	Color::Blue,       Color::BrightBlue,  Color::Green,      Color::Yellow,     Color::Red,        Color::Purple,
	Color::BrightBlue, Color::Green,       Color::Yellow,     Color::Red,        Color::Purple,     Color::Blue,
	Color::Green,      Color::Yellow,      Color::Red,        Color::Purple,     Color::Blue,       Color::BrightBlue,
	Color::Yellow,     Color::Red,         Color::Purple,     Color::Blue,       Color::BrightBlue, Color::Green,
	Color::Red,        Color::Purple,      Color::Blue,       Color::BrightBlue, Color::Green,      Color::Yellow,
	Color::Purple,     Color::Blue,        Color::BrightBlue, Color::Green,      Color::Yellow,     Color::Red
};

///
// Brief description: Load the general texture that we will use for now 
///
void MyTextures::LoadGeneralTexture()
{
	//SDL_Surface * surface = IMG_Load(filename.c_str());
	//
	//if (surface == nullptr)
	//{
	//	std::cout << "Could not load texture: " << filename.c_str() << std::endl;
	//	return 0;
	//}

	// Create texture
	glGenTextures(1, &mGeneralTextures);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glBindTexture(GL_TEXTURE_2D, mGeneralTextures);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Select pixel format from surface
	int pixelFormat = GL_RGBA;
	//if (surface->format->BytesPerPixel == 4) {
	//	pixelFormat = GL_RGBA;
	//}

	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, 6, 6, 0, pixelFormat, GL_UNSIGNED_BYTE, (unsigned char*)mTexture);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glGenerateMipmap(GL_TEXTURE_2D);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Free surface because OpenGL already has the data
	//SDL_FreeSurface(surface);

	//return texture;
}

void MyTextures::LoadTexture(const std::string & filename)
{
	SDL_Surface * surface = IMG_Load(filename.c_str());

	if (surface == nullptr)
	{
		std::cout << "Could not load texture: " << filename.c_str() << std::endl;
		abort();
	}

	// Create texture
	glGenTextures(1, &mNormalMap);
	glBindTexture(GL_TEXTURE_2D, mNormalMap);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Select pixel format from surface
	int pixelFormat = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {
		pixelFormat = GL_RGBA;
	}

	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, surface->w, surface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, surface->pixels);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glGenerateMipmap(GL_TEXTURE_2D);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Free surface because OpenGL already has the data
	SDL_FreeSurface(surface);
}
