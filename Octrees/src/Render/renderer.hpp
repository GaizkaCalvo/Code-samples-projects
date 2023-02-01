/**
 * @file renderer.hpp
 * @author Eder Beldad (eder.b1@digipen.edu)
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 * @brief Forward declarations, so it they don't need to happen anywhere else
 */
#pragma once

#include "Camera/camera.hpp"
#include "Window/window.hpp"
#include "Debug/geometry.hpp"
#include "Render/resources.hpp"
#include "Editor/editor.hpp"

namespace cs350{
	/**
	 * Contains all the necessary information for the assignment. It's a Singleton
	 *
	 * Keeps track of the needed systems.
	 */
	class renderer {
	private:		

	public:
		static renderer& instance()
		{
			static renderer inst;
			return inst;
		}
		~renderer() {destroy();}

		// Core features
		void create(const unsigned w, const  unsigned h, const char* title, const  bool hidden);
		void initialize();
		void destroy();
		bool update();

		//Classees that we will use
		cs350::camera & camera();
		cs350::window & window();
		cs350::editor& editor();
		cs350::resources & resources();

	private:
		renderer() = default;
		renderer(const renderer& rhs) = delete;
		renderer& operator=(const renderer& rhs) = delete;

		cs350::camera mCamera = {};
		cs350::window mWindow = {};
		cs350::resources mResources = {};
		cs350::editor mEditor = {};


		void load_resources();
	};
}
