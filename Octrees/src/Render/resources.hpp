/**
 * @file recourses.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief class that will have the information of the meshes that we loaded
 */
#pragma once
#include "../Shader/shader.hpp"
#include "mesh.hpp"

namespace cs350 {


	struct shaders
	{
		cs350::shader color;
		cs350::shader phong;
	};
	struct meshes
	{
		std::shared_ptr<cs350::mesh> bunny;
		std::shared_ptr<cs350::mesh> cube;
		std::shared_ptr<cs350::mesh> cylinder;
		std::shared_ptr<cs350::mesh> gourd;
		std::shared_ptr<cs350::mesh> icosahedron;
		std::shared_ptr<cs350::mesh> line;
		std::shared_ptr<cs350::mesh> octosahedron;
		std::shared_ptr<cs350::mesh> quad;
		std::shared_ptr<cs350::mesh> segment;
		std::shared_ptr<cs350::mesh> sphere;
		//std::shared_ptr<cs350::mesh> suzanne;
		std::shared_ptr<cs350::mesh> triangle;
		std::shared_ptr<cs350::mesh> sponza;
	};

	class resources
	{
	   private:

	   public:
		   resources();
		   ~resources();

		   shaders mShaders = {};
		   meshes mMesh = {};

	};
}