#include "Helpers/pch.hpp"
#include "renderer.hpp"
#include "Shader/shader.hpp"
#include "GameObjects/object_manager.hpp"
#include "GJK/gjk3d.h"


namespace cs350{
	/**
	* we create the shader from what we got from the string
	* @param w width of the window
	* @param h height of the window
	* @param title title of the window 
	* @param hidden if the window is hiden or not
	*/
	void renderer::create(const unsigned w,const  unsigned h, const char* title, const bool hidden)
	{
		//we create the window
		if (!mWindow.create(w, h, title, hidden))
		{
			std::cout << "Error, Window was not created properly";
			exit(1);
		}
		//cmera creation
		mCamera.create(w, h);

		// Setup camera
		mCamera.set_position({ 0, 5, 10 });
		//mCamera.set_target({ 0, 0, 0 });
		mCamera.set_projection(60.0f, { w, h }, 0.001f, 1000.0f);
		//load all the resources that we have
		load_resources();
	}

	void renderer::initialize()
	{
		mEditor.initialize();
		ObjectManager* ObjMng = ObjectManager::get_instance();
		//ObjMng->AddGameObject("Blusti", { 0.0f, 0.0f, 0.0f }, mResources.mMesh.sphere, { 10.0f, 10.0f, 10.0f });


		ObjMng->AddGameObject("gjk1", { 1.0f, 0.0f, 0.0f } , mResources.mMesh.cube		, { 1.0f, 1.0f, 1.0f });
		ObjMng->AddGameObject("gjk2", { 0.0f, 0.0f, 0.0f } , mResources.mMesh.octohedron, { 1.0f, 1.0f, 1.0f });

		ObjMng->FindGameObject("gjk2")->mMinkowski.create_minkowski(ObjMng->FindGameObject("gjk2"), ObjMng->FindGameObject("gjk1"));



		//create the vertex in world
		std::vector<glm::vec3> hull_a = ObjMng->FindGameObject("gjk2")->mMod->mMeshData.vertexs;
		glm::mat4x4 w2wa = ObjMng->FindGameObject("gjk2")->mTransform.GetModelToWorld();
		for (auto& it : hull_a)
		{
			it = glm::vec3(w2wa * glm::vec4(it, 1.0f));
		}

		std::vector<glm::vec3> hull_b = ObjMng->FindGameObject("gjk1")->mMod->mMeshData.vertexs;
		glm::mat4x4 w2wb = ObjMng->FindGameObject("gjk1")->mTransform.GetModelToWorld();
		for (auto& it : hull_b)
		{
			it = glm::vec3(w2wb * glm::vec4(it, 1.0f));
		}
		//create the simplex
		cs350::gjk3d(hull_a, hull_b, mSimplexs);
	}

	/**
	* Destroy all the data that we have
	*/
	void renderer::destroy()
	{
		//we destroy de meshes that we loaded
		mResources.mMesh.triangle.reset();
		mResources.mMesh.bunny.reset();
		mResources.mMesh.cube.reset();
		mResources.mMesh.cylinder.reset();
		mResources.mMesh.gourd.reset();
		mResources.mMesh.icosahedron.reset();
		mResources.mMesh.line.reset();
		mResources.mMesh.octohedron.reset();
		mResources.mMesh.quad.reset();
		mResources.mMesh.segment.reset();
		mResources.mMesh.sphere.reset();
		//desstroy de shader
		//delete &mResources.mShaders.color;

	}

	/**
	* load all the recourses that we have
	*/
	void renderer::load_resources()
	{
		//create the shader
		mResources.mShaders.color = *mResources.mShaders.color.create_shader_program("resources/shaders/color.vs", "resources/shaders/color.fs");
		mResources.mShaders.phong = *mResources.mShaders.phong.create_shader_program("resources/shaders/phong.vs", "resources/shaders/phong.fs");
		//load all resources we need
		mResources.mMesh.triangle	 = std::make_shared<mesh>(load_obj("resources/meshes/triangle.obj"));
		mResources.mMesh.bunny		 = std::make_shared<mesh>(load_obj("resources/meshes/bunny.obj"));
		mResources.mMesh.cube		 = std::make_shared<mesh>(load_obj("resources/meshes/cube.obj"));
		mResources.mMesh.cylinder	 = std::make_shared<mesh>(load_obj("resources/meshes/cylinder.obj"));
		mResources.mMesh.gourd		 = std::make_shared<mesh>(load_obj("resources/meshes/gourd.obj"));
		mResources.mMesh.icosahedron = std::make_shared<mesh>(load_obj("resources/meshes/icosahedron.obj"));
		mResources.mMesh.line		 = std::make_shared<mesh>(load_obj("resources/meshes/line.obj"));
		mResources.mMesh.octohedron = std::make_shared<mesh>(load_obj("resources/meshes/octohedron.obj"));
		mResources.mMesh.quad		 = std::make_shared<mesh>(load_obj("resources/meshes/quad.obj"));
		mResources.mMesh.segment	 = std::make_shared<mesh>(load_obj("resources/meshes/segment.obj"));
		mResources.mMesh.sphere      = std::make_shared<mesh>(load_obj("resources/meshes/sphere.obj"));
		mResources.mMesh.sponza      = std::make_shared<mesh>(load_obj("resources/meshes/sponza.obj"));

	}
	/**
	* we see if the camera keeps running
	* @return we return true if it keeps, false otherwise
	*/
	bool renderer::update()
	{
		ObjectManager* ObjMng = ObjectManager::get_instance();

		GameObject* objA = ObjMng->FindGameObject("gjk2");
		GameObject* objB = ObjMng->FindGameObject("gjk1");

		objA->mMinkowski.create_minkowski(objA, objB);

		//create the vertex in world
		std::vector<glm::vec3> hull_a = objA->mMod->mMeshData.vertexs;
		glm::mat4x4 w2wa = objA->mTransform.GetModelToWorld();
		for (auto& it : hull_a)
		{
			it = glm::vec3(w2wa * glm::vec4(it, 1.0f));
		}

		std::vector<glm::vec3> hull_b = ObjMng->FindGameObject("gjk1")->mMod->mMeshData.vertexs;
		glm::mat4x4 w2wb = ObjMng->FindGameObject("gjk1")->mTransform.GetModelToWorld();
		for (auto& it : hull_b)
		{
			it = glm::vec3(w2wb * glm::vec4(it, 1.0f));
		}

		//create the simplex, and change the color if we need to 
		if (cs350::gjk3d(hull_a, hull_b, mSimplexs))
		{
			ObjMng->FindGameObject("gjk1")->mDebugColor = glm::vec4({ 1.0f, 0.0f, 0.0f, 1.0f });
			ObjMng->FindGameObject("gjk2")->mDebugColor = glm::vec4({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		else
		{
			ObjMng->FindGameObject("gjk1")->mDebugColor = glm::vec4({ 0.0f, 1.0f, 1.0f, 1.0f });
			ObjMng->FindGameObject("gjk2")->mDebugColor = glm::vec4({ 0.0f, 1.0f, 1.0f, 1.0f });
		}


		return mWindow.keep_updating();
	}
	/**
	* geter of the camera that we have
	* @return camera
	*/
	cs350::camera & renderer::camera()
	{
		return mCamera;
	}
	/**
	* geter of the window that we have
	* @return window
	*/
	cs350::window & renderer::window()
	{
		return mWindow;
	}
	cs350::editor& renderer::editor()
	{
		return mEditor;
	}
	/**
	* geter of the resources that we have
	* @return resources
	*/
	cs350::resources& renderer::resources()
	{
		return mResources;
	}
	bool renderer::get_simplex_debug_draw()
	{
		return mSimplexDebug;
	}
	void renderer::set_simplex_debug_draw(bool state)
	{
		mSimplexDebug = state;
	}
	int renderer::get_steps()
	{
		return mSteps;
	}
	void renderer::set_steps(int step)
	{
		mSteps = step;
	}
}
