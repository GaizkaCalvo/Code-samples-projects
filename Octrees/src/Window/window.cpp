#include "Helpers/pch.hpp"
#include "Helpers/file.hpp"
#include "Helpers/opengl.hpp"
#include "window.hpp"
#include "Render/renderer.hpp"
#include "GameObjects/object_manager.hpp"

namespace cs350{

	/**
	* Returns the cross product between a and v
	* @param error error number that they give
	* @param description description of the error
	* @return
	*/
	void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	/**
	* Callback when a debug information is provided
	* @source data of the callback
	* @type data of the callback
	* @id data of the callback
	* @severity data of the callback
	* @length data of the callback
	* @message data of the callback
	* @userParam data of the callback
	*/
	void debug_callback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		std::string _source;
		std::string _type;
		std::string _severity;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			return;
			//_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			break;

		default:
			_severity = "UNKNOWN";
			break;
		}

		printf("%d: %s of %s severity, raised from %s: %s\n",
			id, _type, _severity, _source, message);
	}

	/**
	* Whenever the window size changed (by OS or user resize) this callback function executes
	* @window window that we create
	* @width w of the window
	* @height h of the window
	*/
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	/**
	* Whenever the mouse is moving we get the data of it from this callback
	* @window window that we create
	* @xpos mouse x position
	* @ypos mouse y position
	*/
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		//get the camera 
		auto& cam = renderer::instance().camera();
		//if the first time that we are here
		if (cam.mFirstMouse)
		{
			cam.mLastX = xpos;
			cam.mLastY = ypos;
			cam.mFirstMouse = false;
		}
		//calculate the difference
		float xoffset = (float)(xpos - cam.mLastX);
		float yoffset = (float)(cam.mLastY - ypos); // reversed since y-coordinates go from bottom to top
		//update
		cam.mLastX = xpos;
		cam.mLastY = ypos;

		//cam.process_mouse_movement(xoffset, yoffset);
	}

	/**
	* Returns whenever the mouse scroll wheel scrolls, this callback is called
	* @param window window that we create
	* @param xoffset
	* @param yoffset
	*/
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto& cam = renderer::instance().camera();
		cam.process_mouse_scroll((float)yoffset);
	}

	void window::draw_all_BVH(GameObject* it)
	{
		if (it->mBVH.get_td_root() != nullptr)
		{
			draw_BVH(it->mBVH.get_td_root(), it, it->mBVH.mDrawBVHTriangles);
		}
		if (it->mBVH.get_bu_root() != nullptr)
		{
			draw_BVH(it->mBVH.get_bu_root(), it, it->mBVH.mDrawBVHTriangles, it->mMod->mMeshData.triangles);
		}
		if (it->mBVH.get_ins_root() != nullptr)
		{
			draw_BVH(it->mBVH.get_ins_root(), it, it->mBVH.mDrawBVHTriangles, it->mMod->mMeshData.triangles);
		}
	}

	void window::draw_BVH(cs350::BVH_TD_node* node,const GameObject* it, bool mDrawBVHTriangles)
	{
		if (node != nullptr)
		{
			if (node->mShowMesh)
			{
				auto m2w = it->mTransform.GetModelToWorld();
				glm::vec3 max = glm::vec3(m2w * glm::vec4(node->mBV.mMax, 1.0f));
				//max /= max.z;
				glm::vec3 min = glm::vec3(m2w * glm::vec4(node->mBV.mMin, 1.0f));
				//min = glm::vec3(0.0f, 0.0f, 0.0f);

				auto& rende = cs350::renderer::instance();
				//UNIFORMS WE WILL GIVE TO THE SHADER
				glm::vec3 scale = max - min;
				glm::mat4 model_2_world = glm::translate(glm::mat4(1), { min + scale / 2.0f });
				model_2_world = glm::scale(model_2_world, scale);
				//fov, aspec ratio, near, far
				glm::mat4 perspective = rende.camera().get_perspective_matrix();
				//camera matrix
				glm::mat4 view = rende.camera().get_view_matrix();
				//M2W
				glm::mat4 uniform_mvp = perspective * view * model_2_world;

				if (mDrawBVHTriangles)
				{
					for(int i = 0; i < node->mNumbrerObj; i++)
					{ 
						debug_draw_triangle(
							cs350::triangle(glm::vec3( m2w * glm::vec4(node->mObject[i][0], 1.0f)),
											glm::vec3( m2w * glm::vec4(node->mObject[i][1], 1.0f)),
											glm::vec3( m2w * glm::vec4(node->mObject[i][2], 1.0f))),
										glm::vec4(0.5f, 0.0f, 0.5f, 0.1f), it->mBVH.mDrawBVHTrianglesFaces);
					}
				}

				//DEBUG DRAW
				glLineWidth(GLfloat(0.5f));
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUseProgram(rende.resources().mShaders.color.handle());
				
				glUniformMatrix4fv(0, 1, GL_FALSE, &uniform_mvp[0][0]);
				glUniform4fv(1, 1, &glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)[0]);
				glBindVertexArray(node->mBV.mMesh->vao_handle());
				glDrawArrays(GL_TRIANGLES, 0, node->mBV.mMesh->get_count());
				
				//we dont use the program anymore
				glUseProgram(0);
				//quit
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glLineWidth(GLfloat(1.0f));
			}

			draw_BVH(node->mLeftNode, it, mDrawBVHTriangles);
			draw_BVH(node->mRightNode, it, mDrawBVHTriangles);
		}
	}

	void window::draw_BVH(cs350::BVH_node* node, const GameObject* it, bool mDrawBVHTriangles, std::vector<std::vector<glm::vec3>> & triangles)
	{
		if (node != nullptr)
		{
			if (node->mShowMesh)
			{
				auto m2w = it->mTransform.GetModelToWorld();
				glm::vec3 max = glm::vec3(m2w * glm::vec4(node->mBV.mMax, 1.0f));
				//max /= max.z;
				glm::vec3 min = glm::vec3(m2w * glm::vec4(node->mBV.mMin, 1.0f));
				//min = glm::vec3(0.0f, 0.0f, 0.0f);

				auto& rende = cs350::renderer::instance();
				//UNIFORMS WE WILL GIVE TO THE SHADER
				glm::vec3 scale = max - min;
				glm::mat4 model_2_world = glm::translate(glm::mat4(1), { min + scale / 2.0f });
				model_2_world = glm::scale(model_2_world, scale);
				//fov, aspec ratio, near, far
				glm::mat4 perspective = rende.camera().get_perspective_matrix();
				//camera matrix
				glm::mat4 view = rende.camera().get_view_matrix();
				//M2W
				glm::mat4 uniform_mvp = perspective * view * model_2_world;

				if (mDrawBVHTriangles)
				{
					int size = (int)node->mObject.size();
					for (int i = 0; i < size; i++)
					{
						debug_draw_triangle(
							cs350::triangle(glm::vec3(m2w * glm::vec4(triangles[node->mObject[i]][0], 1.0f)),
								            glm::vec3(m2w * glm::vec4(triangles[node->mObject[i]][1], 1.0f)),
								            glm::vec3(m2w * glm::vec4(triangles[node->mObject[i]][2], 1.0f))),
											glm::vec4(0.5f, 0.0f, 0.5f, 0.1f), it->mBVH.mDrawBVHTrianglesFaces);
					}
				}

				//DEBUG DRAW
				glLineWidth(GLfloat(0.5f));
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUseProgram(rende.resources().mShaders.color.handle());

				glUniformMatrix4fv(0, 1, GL_FALSE, &uniform_mvp[0][0]);
				glUniform4fv(1, 1, &glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)[0]);
				glBindVertexArray(node->mBV.mMesh->vao_handle());
				glDrawArrays(GL_TRIANGLES, 0, node->mBV.mMesh->get_count());

				//we dont use the program anymore
				glUseProgram(0);
				//quit
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glLineWidth(GLfloat(1.0f));
			}

			draw_BVH(node->mLeftNode , it, mDrawBVHTriangles, triangles);
			draw_BVH(node->mRightNode, it, mDrawBVHTriangles, triangles);
		}
	}

	/**
	* Window creation, returns true if correct, false otherwise
	* @param window window that we create
	* @param xoffset
	* @param yoffset
	* @return true if window was succes, false otherwise
	*/
	bool window::create(const int w, const  int h, const char* window_name, const  bool hidden)
	{
		std::cout << "Starting GLFW context, OpenGL 4.3" << std::endl;
		mSize = glm::ivec2{ w, h };

		// Init GLFW
		if (!glfwInit())
		{
			// Initialization failed
			return false;
		}

		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		//hide the window if we need to 
		if (hidden)
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

		// Create a GLFWwindow object that we can use for GLFW's functions
		mWindow = glfwCreateWindow(w, h, window_name, NULL, NULL);
		glfwMakeContextCurrent(mWindow);
		if (!mWindow)
		{
			// Window or context creation failed
			return false;
		}

		// Set the required callback functions
		glfwSetErrorCallback(error_callback);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//failed
			std::cout << "Failed to load OpenGL pointers \n" << std::endl;
			return false;
		}

		// Debug
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debug_callback, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);

		//some other callbacks if we want (self explanatory)
		//glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
		//glfwSetCursorPosCallback(mWindow, mouse_callback);
		//glfwSetScrollCallback(mWindow, scroll_callback);

		return true;
	}
	/**
	* Window update, to render what we got in the demo
	*/
	void  window::update()
	{
		renderer & rende = cs350::renderer::instance();
		ObjectManager * obj_mng = ObjectManager::get_instance();
		// input
		// -----

		//cs350::process_input(mWindow);

		glm::vec4 color( 1, 1, 1, 0.5 );

		//glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);
		// render
		// ------
		// color of the window
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, (GLsizei)mSize.x, (GLsizei)mSize.y);

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		//fov, aspec ratio, near, far
		glm::mat4 perspective = rende.camera().get_perspective_matrix();
		//camera matrix
		glm::mat4 view = rende.camera().get_view_matrix();
		//pv
		glm::mat4 pv = perspective * view;

		for (auto* it : obj_mng->mGameObjects)
		{
			if (it->mFaceRender)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glUseProgram(rende.resources().mShaders.color.handle());
				//UNIFORMS WE WILL GIVE TO THE SHADER
				auto  m2w = it->mTransform.GetModelToWorld();
				//M2W
				glm::mat4 uniform_mvp = pv * m2w;

				glUniformMatrix4fv(0, 1, GL_FALSE, &uniform_mvp[0][0]);
				glUniform4fv(1, 1, &color[0]);
				glBindVertexArray(it->mMod->vao_handle());
				glDrawArrays(GL_TRIANGLES, 0, it->mMod->get_count());

				//we dont use the program anymore
				glUseProgram(0);
				//quit
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			//if we want to print the debig lines
			if (it->mDebugLines)
			{
				//UNIFORMS WE WILL GIVE TO THE SHADER
				auto  m2w = it->mTransform.GetModelToWorld();
				//M2W
				glm::mat4 uniform_mvp = pv * m2w;

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//SECOND RENDER FOR DEBUG
				glUseProgram(rende.resources().mShaders.color.handle());

				glUniformMatrix4fv(0, 1, GL_FALSE, &uniform_mvp[0][0]);
				glUniform4fv(1, 1, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);
				glBindVertexArray(it->mMod->vao_handle());
				glDrawArrays(GL_TRIANGLES, 0, it->mMod->get_count());
				//we dont use the program anymore
				glUseProgram(0);
				//quit
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			//we draw the BVH if we need to 
			draw_all_BVH(it);
		}
	}
	/**
	* Window destroy, to clear 
	*/
	void window::destroy()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
	/**
	* Window geter, to get the window that we create
	* @return window pointer
	*/
	GLFWwindow* window::get_window()
	{
		return mWindow;
	}

	/**
	* To check if the window is still opened
	* @return true if we keep the window false otherwise
	*/
	bool window::keep_updating()
	{
		return !glfwWindowShouldClose(mWindow);
	}
}