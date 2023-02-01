cmake_minimum_required(VERSION 3.6.2)

##################################
# Build configuration
set(PRJ_NAME cs350_renderer)
set(PRJ_TEST_NAME ${PRJ_NAME}_test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Source files
set(SRC
		"src/Debug/geometry.cpp"
		"src/Debug/geometry.hpp"
		"src/Debug/debug.cpp"
		"src/Debug/debug.hpp"
		"src/Helpers/math.hpp"
		"src/Helpers/math.cpp"
		"src/Helpers/pch.hpp"
		"src/Helpers/fwd.hpp"
		"src/Helpers/opengl.hpp"
		"src/Helpers/opengl.cpp"
		"src/Window/window.hpp"
		"src/Window/window.cpp"
		"src/Camera/camera.hpp"
		"src/Camera/camera.cpp"
		"src/Render/renderer.hpp"
		"src/Render/renderer.cpp"
		"src/Render/resources.hpp"
		"src/Render/resources.cpp"
		"src/Shader/shader.hpp"
		"src/Shader/shader.cpp"
		"src/Helpers/file.hpp"
		"src/Helpers/file.cpp"
		"src/Render/mesh.hpp"
		"src/Render/mesh.cpp"
		"src/Render/mesh_data.hpp"
		"src/Render/mesh_data.cpp"
		"src/Helpers/imgui.hpp"
		"src/GameObjects/game_object.hpp"
		"src/GameObjects/game_object.cpp"
		"src/GameObjects/object_manager.cpp"
		"src/GameObjects/object_manager.cpp"
		"src/GameObjects/Transform/transform.cpp"
		"src/GameObjects/Transform/transform.hpp"
		"src/Editor/editor.cpp"
		"src/Editor/editor.hpp"
		"src/BVH/nodes.cpp"
		"src/BVH/nodes.hpp"
		"src/BVH/bvh.hpp"
		"src/BVH/bvh.cpp"
		"src/Octree/demo_octree.hpp"
		"src/Octree/demo_octree.cpp"
		"src/Octree/octree.hpp"
		"src/Octree/octree.cpp"
		"src/Octree/octree.inl"
		"src/GJK/Minkowski.hpp"
		"src/GJK/Minkowski.cpp"
		 "src/GJK/Minkowski.hpp"
		 "src/GJK/Minkowski.cpp"
		 "src/GJK/gjk_simplex.cpp"
		 "src/GJK/gjk3d.cpp"
		)

include_directories(src)


# Test files
set(SRC_TEST
		"src/Test/test_octree.cpp"
		"src/Test/test_common.hpp")

# Projects
project(${PRJ_NAME})
project(${PRJ_TEST_NAME})

##################################
# Libraries
if (WIN32)
	# Directory in which libraries are
	set(DEPENDENCIES_DIR $ENV{USERPROFILE}/documents/digipen/dependencies)
	string(REPLACE "\\" "/" DEPENDENCIES_DIR ${DEPENDENCIES_DIR})
elseif (UNIX)
	# Directory in which libraries are
	set(DEPENDENCIES_DIR $ENV{HOME}/digipen/dependencies)
endif ()

# glm
include_directories("${DEPENDENCIES_DIR}/glm")

# gtest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Make GTest match runtime libraries
add_subdirectory("${DEPENDENCIES_DIR}/googletest" "googletest")
enable_testing()

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${DEPENDENCIES_DIR}/glfw" "glfw")
include_directories("${DEPENDENCIES_DIR}/glfw/include/")

# glad
add_subdirectory("${DEPENDENCIES_DIR}/glad" "glad")
include_directories("${CMAKE_CACHEFILE_DIR}/glad/include/")

# lodepng
include_directories("${DEPENDENCIES_DIR}/lodepng")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/lodepng/lodepng.cpp")

# imgui
include_directories("${DEPENDENCIES_DIR}/imgui")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/imgui/examples/imgui_impl_glfw.cpp")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/imgui/examples/imgui_impl_opengl3.cpp")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/imgui/imgui.cpp")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/imgui/imgui_demo.cpp")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/imgui/imgui_draw.cpp")
set(SRC_EXTERNAL "${SRC_EXTERNAL}" "${DEPENDENCIES_DIR}/imgui/imgui_widgets.cpp")
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

##################################
# Compile arguments
set(CMAKE_CXX_STANDARD 20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	message("Using MSVC")
	# Visual Studio Configuration
	# Enable warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

	# Warnings as errors
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") 
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

	# Disable specific warning
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4201") # nameless struct/union
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message("Using G++")
	# G++ configuration
	# Enable warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
	# Warnings as errors
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	# Disable specific warning
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile") # Disable GLM warning on overflow function
endif ()

# Binaries
add_executable(${PRJ_NAME} ${SRC} ${SRC_EXTERNAL} src/main.cpp)
target_link_libraries(${PRJ_NAME} glfw glad)