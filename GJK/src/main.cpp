#include "Helpers/pch.hpp"
#include "Helpers/file.hpp"
#include "Render/renderer.hpp"
#include <GLFW/glfw3.h>
#include "Octree/demo_octree.hpp"

int main(int argc, const char* argv[])
{
	cs350::renderer & demo = cs350::renderer::instance();
	demo.create(1280, 800, "Cs 350", false);
	demo.initialize();
	while (demo.update())
	{
		demo.camera().update();
		cs350::process_input(demo.window().get_window());
		demo.window().update();
		demo.editor().update();
	}
	demo.destroy();
	return 0;

	//cs350::demo_octree demo;
	//
	//demo.create();
	//while (demo.update())
	//{
	//
	//}

	return 0;
}