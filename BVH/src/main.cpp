#include "Helpers/pch.hpp"
#include "Render/renderer.hpp"
#include <GLFW/glfw3.h>

int main(int argc, const char* argv[])
{
	cs350::renderer & demo = cs350::renderer::instance();
	demo.create(1280, 800, "Cs 350", false);
	demo.initialize();
	while (demo.update())
	{
		demo.camera().update();
		demo.window().update();
		demo.editor().update();
	}
	demo.destroy();
	return 0;
}