#pragma once

#include "Render/mesh.hpp"
//#include "Helpers/opengl.hpp"

class GameObject;

namespace cs350
{
	class minkowski
	{
	public:
		minkowski();
		~minkowski();

		void create_minkowski(GameObject * ob1, GameObject* ob2);
		bool minkoski_created();
		void delete_minkowski();


		bool get_debug_draw() const;
		void set_debug_draw(bool state);


		cs350::mesh mMesh;
	private:

		bool mDebugDraw;
	};
}