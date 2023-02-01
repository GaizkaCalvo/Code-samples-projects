#include "Helpers/pch.hpp"
#include "Render/renderer.hpp" 

namespace cs350 {
    /**
    * We process the inputs given
    * @param mWindow data where the inputs are located
    */
    void process_input(GLFWwindow* mWindow)
    {
        //in case the user press scape key
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        //inputs for the camera
        cs350::renderer::instance().camera().proccess_inputs(mWindow);
    }
}