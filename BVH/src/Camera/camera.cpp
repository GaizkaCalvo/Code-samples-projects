#include "Helpers/pch.hpp"
#include "camera.hpp"
#include "Render/renderer.hpp"

namespace cs350 {

    /**
    * @description update of the camera
    */
    void camera::update()
    {
    }
    /**
    * @description we create all we need for the camera
    */
    void camera::create(const int w, const  int h)
    {
        mPerspective = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(w / h), 0.001f, 1000.0f);
    }
    /**
    * @description we set the new position of the camera
    * @param pos the position where we set the camera
    */
    void camera::set_position(glm::vec3 pos)
    {
        mPosition = pos;
    }
    /**
    * @description we set the new target of the camera and we update all the values that we need
    * @param target where the target is located
    */
    void camera::set_target(glm::vec3 target)
    {
        mFront = target - mPosition;
        mFront = glm::normalize(mFront);
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        mUp = glm::normalize(glm::cross(mRight, mFront));
    }
    /**
    * @description we set the new projection that we are going to use
    * @param fov 
    * @param proportions the ratio of height and width
    * @param a near
    * @param b far
    */
    void camera::set_projection(float FOV , glm::vec2 Proportions, float a, float b)
    {
        mPerspective = glm::perspective(glm::radians(FOV), static_cast<GLfloat>(Proportions[0] / Proportions[1]), a, b);
    }
    /**
    * @description constructor of the camera
    * @param position
    * @param up
    * @param yaw
    * @param pitch
    */
    camera::camera(const glm::vec3 position, const  glm::vec3 up, const float yaw, const  float pitch)
        : mPosition(position), mWorldUp(up), mYaw(yaw), mPitch(pitch),
        mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(0.5f), mMouseSensitivity(0.1f), mZoom(45.0f)
    {
        updateCameraVectors();
    }
    /**
    * @description constructor of camera
    * @param Camera values
    */
    camera::camera(const float posX, const  float posY, const  float posZ, const  float upX, const  float upY, const  float upZ, const  float yaw, const  float pitch)
        : mPosition(glm::vec3(posX, posY, posZ)), mWorldUp(glm::vec3(upX, upY, upZ)), mYaw(yaw), mPitch(pitch),
        mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(0.5f), mMouseSensitivity(0.1f), mZoom(45.0f)
    {
        updateCameraVectors();
    }
    /**
    * @description we return the view matrix
    * @return view matrix
    */
    glm::mat4 camera::get_view_matrix()
    {
        return glm::lookAt(mPosition, mPosition + mFront, mUp);
    }
    /**
    * @description we return the perspective
    * @return mPerspective we return the perspective
    */
    glm::mat4 camera::get_perspective_matrix()
    {
        return mPerspective;
    }
    /**
    * @description we update the camera vectors that we need to 
    */
    void camera::updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        front.y = sin(glm::radians(mPitch));
        front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront = glm::normalize(front);
        // also re-calculate the Right and Up vector
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        mUp = glm::normalize(glm::cross(mRight, mFront));
    }
    /**
    * @description we check if we need to zoom 
    * @param toffset the offset that the user did with the mouse scroll
    */
    void camera::process_mouse_scroll(const float yoffset)
    {
        mZoom -= (float)yoffset;
        if (mZoom < 1.0f)
            mZoom = 1.0f;
        if (mZoom > 45.0f)
            mZoom = 45.0f;
    }
    /**
    * @description we proccess the input that the window has take 
    * @param win the windown
    */
    void camera::proccess_inputs(GLFWwindow* Win)
    {
        //controls for moving with wasd
        float cameraSpeed = mMovementSpeed;
        if (glfwGetKey(Win, GLFW_KEY_W) == GLFW_PRESS)
            mPosition += cameraSpeed * mFront;
        if (glfwGetKey(Win, GLFW_KEY_S) == GLFW_PRESS)
            mPosition -= cameraSpeed * mFront;
        if (glfwGetKey(Win, GLFW_KEY_A) == GLFW_PRESS)
            mPosition -= glm::normalize(glm::cross(mFront, mUp)) * cameraSpeed;
        if (glfwGetKey(Win, GLFW_KEY_D) == GLFW_PRESS)
            mPosition += glm::normalize(glm::cross(mFront, mUp)) * cameraSpeed;

        //return;
        //if we are pressinf right click
        if (glfwGetMouseButton(Win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            //first time, we get the position for next frame
            if (!mFirstMouse)
            {
                mFirstMouse = true;
                glfwGetCursorPos(Win, &mLastX, &mLastY);
                return;
            }
            //if its not we get the new position
            double Xnow, Ynow;
            glfwGetCursorPos(Win, &Xnow, &Ynow);
            //we get the difference
            double xoffset = Xnow - mLastX;
            double yoffset = mLastY - Ynow;
            //update the late position
            mLastX = Xnow;
            mLastY = Ynow;
            //go to where we need to go 
            xoffset *= mMouseSensitivity;
            yoffset *= mMouseSensitivity;
            //we update what we need
            mYaw += (float)xoffset;
            mPitch += (float)yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            //if (constrainPitch)
            //{
            //    if (mPitch > 89.0f)
            //        mPitch = 89.0f;
            //    if (mPitch < -89.0f)
            //        mPitch = -89.0f;
            //}

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }
        else//we did not click 
            mFirstMouse = false;

    }

}