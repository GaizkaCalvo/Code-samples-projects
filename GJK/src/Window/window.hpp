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

class GameObject;

namespace cs350{
	class window {
	private:
		GLFWwindow* mWindow = {};

		void draw_all_BVH(GameObject* it);
		void draw_BVH(cs350::BVH_TD_node* node, const GameObject* it, bool mDrawBVHTriangles);
		void draw_BVH(cs350::BVH_node* node, const GameObject* it, bool mDrawBVHTriangles, std::vector<std::vector<glm::vec3>> & triangles);

		void draw_minkowski(GameObject * it);
		void draw_simplex_steps();

	public:
		window() {};
		~window() {destroy();};
		bool create(const int w,const  int h, const char* window_name, const  bool hidden);
		void update();
		void destroy();

		GLFWwindow * get_window();

		bool keep_updating();

		[[nodiscard]] glm::vec2 size() const noexcept {return mSize;}
		[[nodiscard]] GLFWwindow* handle() const noexcept {return mWindow;}

		glm::vec2 mSize = {};
	};
}