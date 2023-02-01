/**
 * @file window.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief Window class where we store the information and usefull functions
 */
#pragma once
#include "Helpers/math.hpp"
#include "BVH/nodes.hpp"

namespace cs350{
	class window {
	private:
		GLFWwindow* mWindow = {};

		void draw_BVH(cs350::BVH_TD_node* node, const glm::mat4 & m2w, bool mDrawBVHTriangles);
		void draw_BVH(cs350::BVH_node* node, const glm::mat4 & m2w, bool mDrawBVHTriangles, std::vector<std::vector<glm::vec3>> & triangles);

	public:
		~window() {destroy();};
		bool create(const int w,const  int h, const char* window_name, const  bool hidden);
		void update();
		void destroy();

		GLFWwindow * get_window();

		bool keep_updating();

		[[nodiscard]] glm::ivec2 size() const noexcept {return mSize;}
		[[nodiscard]] GLFWwindow* handle() const noexcept {return mWindow;}

		glm::ivec2 mSize = {};
	};
}