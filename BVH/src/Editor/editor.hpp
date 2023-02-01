#pragma once
#include "BVH/nodes.hpp"

namespace cs350 {

	class editor
	{
	public:
		editor();
		~editor();

		void initialize();
		void update();

	private:

		// Our state
		bool show_demo_window;
		bool show_another_window;
		glm::vec4 clear_color;

		//BVH editor
		void bounding_volumes();
		void show_bvh_nodes(cs350::BVH_TD_node* obj, std::string name);
		void show_bvh_nodes(cs350::BVH_node* obj, std::string name);
	};

}