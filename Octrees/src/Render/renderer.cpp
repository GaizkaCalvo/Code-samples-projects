#include "Helpers/pch.hpp"
#include "renderer.hpp"
#include "Shader/shader.hpp"
#include "GameObjects/object_manager.hpp"


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
		mCamera.set_position({ 0, 0, 50 });
		//mCamera.set_target({ 0, 0, 0 });
		mCamera.set_projection(60.0f, { w, h }, 0.001f, 1000.0f);
		//load all the resources that we have
		load_resources();

		//ObjectManager* ObjMng = ObjectManager::get_instance();
		//ObjMng->AddGameObject("Blusti", {0.0f, 0.0f, 0.0f}, mResources.mMesh.sphere, { 10.0f, 10.0f, 10.0f });
	}

	void renderer::initialize()
	{
		mEditor.initialize();
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
		mResources.mMesh.octosahedron.reset();
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
		//mResources.mMesh.octosahedron= std::make_shared<mesh>(load_obj("resources/meshes/octosahedron.obj"));
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
}
